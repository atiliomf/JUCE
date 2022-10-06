
class PitchYIN
{
public:
    PitchYIN (int sampleRate = 8192, unsigned int bufferSize = 8192) 
    : yin (1, bufferSize)
    , bufferSize (bufferSize)
    , sampleRate (sampleRate)
    , tolerance (0.15)
    , deltaWasNegative (false)
    {}
    
    void setSampleRate (unsigned int newSampleRate) {sampleRate = newSampleRate;}
	void setBufferSize (unsigned int newBufferSize) {bufferSize = newBufferSize;}
	void setTolerance (float newTolerance) {tolerance = newTolerance;}
    
	float getPitchInHz (const float* inputData) noexcept
    {
        float pitch = calculatePitch (inputData);
        return pitch > 0 ? sampleRate / (pitch + 0.0) : 0.0;
    }
    
    /** Full YIN algorithm */
    float calculatePitch (const float* inputData) noexcept
    {
        int period;
        float delta = 0.0, runningSum = 0.0;
        float *yinData = yin.getWritePointer(0);

        yinData[0] = 1.0;
        for (int tau = 1; tau < yin.getNumSamples(); ++tau)
        {
            yinData[tau] = 0.0;
            
            for (int j = 0; j < yin.getNumSamples(); ++j)
            {
                delta = inputData[j] - inputData[j + tau];
                yinData[tau] += (delta * delta);
            }
            
            runningSum += yinData[tau];
            
            if (runningSum != 0)
                yinData[tau] *= tau / runningSum;
            else
                yinData[tau] = 1.0;

            period = tau - 3;
            
            if (tau > 4 && (yinData[period] < tolerance) &&
                    (yinData[period] < yinData[period + 1]))
            {
                return quadraticPeakPosition (yin.getReadPointer(0), period);
            }
        }
        return 0.0f;
    }

private:
    AudioSampleBuffer yin; 
    unsigned int bufferSize;
    unsigned int sampleRate;
    float tolerance; 
    bool deltaWasNegative;
    Logger *log;

    float quadraticPeakPosition (const float *data, unsigned int pos) noexcept
    {
        float s0, s1, s2;
        unsigned int x0, x2;
        if (pos == 0 || pos == bufferSize - 1) return pos;
        x0 = (pos < 1) ? pos : pos - 1;
        x2 = (pos + 1 < bufferSize) ? pos + 1 : pos;
        if (x0 == pos) return (data[pos] <= data[x2]) ? pos : x2;
        if (x2 == pos) return (data[pos] <= data[x0]) ? pos : x0;
        s0 = data[x0];
        s1 = data[pos];
        s2 = data[x2];
        return pos + 0.5 * (s0 - s2 ) / (s0 - 2.* s1 + s2);
    }
};
