
#if BV_USE_VDSP
#    include <Accelerate/Accelerate.h>
#    define BV_VDSP_FUNC_SWITCH(FloatFuncName, DoubleFuncName, ...)               \
        if constexpr (std::is_same_v< Type, float >) FloatFuncName (__VA_ARGS__); \
        else                                                                      \
            DoubleFuncName (__VA_ARGS__);
#endif

namespace bav::vecops
{
template < typename Type >
void fill (Type* vector, Type value, int count)
{
#if BV_USE_VDSP
    BV_VDSP_FUNC_SWITCH (vDSP_vfill, vDSP_vfillD,
                         &value, vector, vDSP_Stride (1), vDSP_Length (count))
#else
    juce::FloatVectorOperations::fill (vector, value, count);
#endif
}
template void fill (float*, float, int);
template void fill (double*, double, int);

template <>
void fill (int* vector, int value, int count)
{
    memset (vector, value, (size_t) count * sizeof (int));
}

template < typename Type1, typename Type2 >
void convert (Type1* const dst, const Type2* const src, int count)
{
#if BV_USE_VDSP
    if constexpr (std::is_same_v< Type1, double >)
        vDSP_vspdp (src, vDSP_Stride (1), dst, vDSP_Stride (1), vDSP_Length (count));
    else
        vDSP_vdpsp (src, vDSP_Stride (1), dst, vDSP_Stride (1), vDSP_Length (count));
#else
    for (int i = 0; i < count; ++i)
    {
        dst[i] = static_cast< Type1 > (src[i]);
    }
#endif
}
template void convert (float* const, const double* const, int);
template void convert (double* const, const float* const, int);

template < typename Type >
void addC (Type* vector, Type value, int count)
{
#if BV_USE_VDSP
    BV_VDSP_FUNC_SWITCH (vDSP_vsadd, vDSP_vsaddD,
                         vector, vDSP_Stride (1), &value, vector, vDSP_Stride (1), vDSP_Length (count))
#else
    juce::FloatVectorOperations::add (vector, value, count);
#endif
}
template void addC (float*, float, int);
template void addC (double*, double, int);

template <>
void addC (int* vector, int value, int count)
{
    for (int i = 0; i < count; ++i)
        vector[i] += value;
}

template < typename Type >
void addV (Type* vecA, const Type* vecB, int count)
{
#if BV_USE_VDSP
    BV_VDSP_FUNC_SWITCH (vDSP_vadd, vDSP_vaddD,
                         vecB, vDSP_Stride (1), vecA, vDSP_Stride (1), vecA, vDSP_Stride (1), vDSP_Length (count))
#else
    juce::FloatVectorOperations::add (vecA, vecB, count);
#endif
}
template void addV (float*, const float*, int);
template void addV (double*, const double*, int);

template <>
void addV (int* vecA, const int* vecB, int count)
{
    for (int i = 0; i < count; ++i)
        vecA[i] += vecB[i];
}

template < typename Type >
void subtractC (Type* vector, Type value, int count)
{
#if BV_USE_VDSP
    const auto val = -value;

    BV_VDSP_FUNC_SWITCH (vDSP_vsadd, vDSP_vsaddD,
                         vector, vDSP_Stride (1), &val, vector, vDSP_Stride (1), vDSP_Length (count))
#else
    juce::FloatVectorOperations::add (vector, -value, count);
#endif
}
template void subtractC (float*, float, int);
template void subtractC (double*, double, int);

template <>
void subtractC (int* vector, int value, int count)
{
    for (int i = 0; i < count; ++i)
        vector[i] -= value;
}

template < typename Type >
void subtractV (Type* vecA, const Type* vecB, int count)
{
#if BV_USE_VDSP
    BV_VDSP_FUNC_SWITCH (vDSP_vsub, vDSP_vsubD,
                         vecA, vDSP_Stride (1), vecB, vDSP_Stride (1), vecA, vDSP_Stride (1), vDSP_Length (count))
#else
    for (int i = 0; i < count; ++i)
        vecA[i] = vecA[i] - vecB[i];
#endif
}
template void subtractV (float*, const float*, int);
template void subtractV (double*, const double*, int);

template <>
void subtractV (int* vecA, const int* vecB, int count)
{
    for (int i = 0; i < count; ++i)
        vecA[i] -= vecB[i];
}

template < typename Type >
void multiplyC (Type* vector, Type value, int count)
{
#if BV_USE_VDSP
    BV_VDSP_FUNC_SWITCH (vDSP_vsmul, vDSP_vsmulD,
                         vector, vDSP_Stride (1), &value, vector, vDSP_Stride (1), vDSP_Length (count))
#else
    juce::FloatVectorOperations::multiply (vector, value, count);
#endif
}
template void multiplyC (float*, float, int);
template void multiplyC (double*, double, int);

template <>
void multiplyC (int* vector, int value, int count)
{
    for (int i = 0; i < count; ++i)
        vector[i] *= value;
}


template < typename Type >
void multiplyV (Type* vecA, const Type* vecB, int count)
{
#if BV_USE_VDSP
    BV_VDSP_FUNC_SWITCH (vDSP_vmul, vDSP_vmulD,
                         vecA, vDSP_Stride (1), vecB, vDSP_Stride (1), vecA, vDSP_Stride (1), vDSP_Length (count))
#else
    juce::FloatVectorOperations::multiply (vecA, vecB, count);
#endif
}
template void multiplyV (float*, const float*, int);
template void multiplyV (double*, const double*, int);

template <>
void multiplyV (int* vecA, const int* vecB, int count)
{
    for (int i = 0; i < count; ++i)
        vecA[i] *= vecB[i];
}

template < typename Type >
void divideC (Type* vector, Type value, int count)
{
#if BV_USE_VDSP
    BV_VDSP_FUNC_SWITCH (vDSP_vsdiv, vDSP_vsdivD,
                         vector, vDSP_Stride (1), &value, vector, vDSP_Stride (1), vDSP_Length (count))
#else
    juce::FloatVectorOperations::multiply (vector, (Type) 1. / value, count);
#endif
}
template void divideC (float*, float, int);
template void divideC (double*, double, int);

template <>
void divideC (int* vector, int value, int count)
{
    auto val = (float) value;

    for (int i = 0; i < count; ++i)
        vector[i] = juce::roundToInt ((float) vector[i] / val);
}


template < typename Type >
void divideV (Type* vecA, const Type* vecB, int count)
{
#if BV_USE_VDSP
    BV_VDSP_FUNC_SWITCH (vDSP_vdiv, vDSP_vdivD,
                         vecB, vDSP_Stride (1), vecA, vDSP_Stride (1), vecA, vDSP_Stride (1), vDSP_Length (count))
#else
    for (int i = 0; i < count; ++i)
        vecA[i] /= vecB[i];
#endif
}
template void divideV (float*, const float*, int);
template void divideV (double*, const double*, int);

template <>
void divideV (int* vecA, const int* vecB, int count)
{
    for (int i = 0; i < count; ++i)
        vecA[i] = juce::roundToInt ((float) vecA[i] / (float) vecB[i]);
}


template < typename Type >
void squareRoot (Type* data, int dataSize)
{
#if BV_USE_VDSP
    BV_VDSP_FUNC_SWITCH (vvsqrtf, vvsqrt,
                         data, data, &dataSize)
#else
    for (int i = 0; i < dataSize; ++i)
        data[i] = std::sqrt (data[i]);
#endif
}
template void squareRoot (float*, int);
template void squareRoot (double*, int);

template < typename Type >
void square (Type* data, int dataSize)
{
#if BV_USE_VDSP
    BV_VDSP_FUNC_SWITCH (vDSP_vsq, vDSP_vsqD,
                         data, vDSP_Stride (1), data, vDSP_Stride (1), vDSP_Length (dataSize))
#else
    juce::FloatVectorOperations::multiply (data, data, dataSize);
#endif
}
template void square (float*, int);
template void square (double*, int);

template <>
void square (int* data, int size)
{
    for (int i = 0; i < size; ++i)
        data[i] *= data[i];
}


template < typename Type >
void absVal (Type* data, int dataSize)
{
#if BV_USE_VDSP
    BV_VDSP_FUNC_SWITCH (vvfabsf, vvfabs,
                         data, data, &dataSize)
#else
    juce::FloatVectorOperations::abs (data, data, dataSize);
#endif
}
template void absVal (float*, int);
template void absVal (double*, int);

template <>
void absVal (int* data, int size)
{
    for (int i = 0; i < size; ++i)
        data[i] = abs (data[i]);
}


template < typename Type >
int findIndexOfMinElement (const Type* data, int dataSize)
{
#if BV_USE_VDSP
    unsigned long index   = 0.0;
    Type          minimum = Type (0);

    BV_VDSP_FUNC_SWITCH (vDSP_minvi, vDSP_minviD,
                         data, vDSP_Stride (1), &minimum, &index, vDSP_Length (dataSize))

    return static_cast< int > (index);
#else
    return static_cast< int > (std::min_element (data, data + dataSize) - data);
#endif
}
template int findIndexOfMinElement (const float*, int);
template int findIndexOfMinElement (const double*, int);

template <>
int findIndexOfMinElement (const int* data, int size)
{
    return static_cast< int > (std::min_element (data, data + size) - data);
}


template < typename Type >
int findIndexOfMaxElement (const Type* data, int dataSize)
{
#if BV_USE_VDSP
    unsigned long index   = 0.0;
    Type          maximum = Type (0);

    BV_VDSP_FUNC_SWITCH (vDSP_maxvi, vDSP_maxviD,
                         data, vDSP_Stride (1), &maximum, &index, vDSP_Length (dataSize))

    return static_cast< int > (index);
#else
    return static_cast< int > (std::max_element (data, data + dataSize) - data);
#endif
}
template int findIndexOfMaxElement (const float*, int);
template int findIndexOfMaxElement (const double*, int);

template <>
int findIndexOfMaxElement (const int* data, int size)
{
    return static_cast< int > (std::max_element (data, data + size) - data);
}

template < typename Type >
void findMinAndMinIndex (const Type* data,
                         int         dataSize,
                         Type&       minimum,
                         int&        minIndex)
{
#if BV_USE_VDSP
    unsigned long index = 0.0;

    BV_VDSP_FUNC_SWITCH (vDSP_minvi, vDSP_minviD,
                         data, vDSP_Stride (1), &minimum, &index, vDSP_Length (dataSize))

    minIndex = static_cast< int > (index);
#else
    auto* lowestElement    = std::min_element (data, data + dataSize);
    minimum                = *lowestElement;
    minIndex               = static_cast< int > (lowestElement - data);
#endif
}
template void findMinAndMinIndex (const float*, int, float&, int&);
template void findMinAndMinIndex (const double*, int, double&, int&);

template <>
void findMinAndMinIndex (const int* data, int dataSize, int& minimum, int& minIndex)
{
    auto* lowestElement = std::min_element (data, data + dataSize);
    minimum             = *lowestElement;
    minIndex            = static_cast< int > (lowestElement - data);
}

template < typename Type >
void findMaxAndMaxIndex (const Type* data,
                         int         dataSize,
                         Type&       maximum,
                         int&        maxIndex)
{
#if BV_USE_VDSP
    unsigned long index = 0.0;

    BV_VDSP_FUNC_SWITCH (vDSP_maxvi, vDSP_maxviD,
                         data, vDSP_Stride (1), &maximum, &index, vDSP_Length (dataSize))

    maxIndex = static_cast< int > (index);
#else
    auto* highestElement   = std::max_element (data, data + dataSize);
    maximum                = *highestElement;
    maxIndex               = static_cast< int > (highestElement - data);
#endif
}
template void findMaxAndMaxIndex (const float*, int, float&, int&);
template void findMaxAndMaxIndex (const double*, int, double&, int&);

template <>
void findMaxAndMaxIndex (const int* data, int dataSize, int& maximum, int& maxIndex)
{
    auto* highestElement = std::max_element (data, data + dataSize);
    maximum              = *highestElement;
    maxIndex             = static_cast< int > (highestElement - data);
}

template < typename Type >
void locateGreatestAbsMagnitude (const Type* data,
                                 int         dataSize,
                                 Type&       greatestMagnitude,
                                 int&        index)
{
#if BV_USE_VDSP
    unsigned long i = 0.0;

    BV_VDSP_FUNC_SWITCH (vDSP_maxmgvi, vDSP_maxmgviD,
                         data, vDSP_Stride (1), &greatestMagnitude, &i, vDSP_Length (dataSize))

    index = static_cast< int > (i);
#else
    int  strongestMagIndex = 0;
    auto strongestMag      = abs (data[0]);

    for (int i = 1; i < dataSize; ++i)
    {
        const auto current = abs (data[i]);

        if (current > strongestMag)
        {
            strongestMag      = current;
            strongestMagIndex = i;
        }
    }

    greatestMagnitude    = strongestMag;
    index                = strongestMagIndex;
#endif
}
template void locateGreatestAbsMagnitude (const float*, int, float&, int&);
template void locateGreatestAbsMagnitude (const double*, int, double&, int&);

template <>
void locateGreatestAbsMagnitude (const int* data, int dataSize, int& greatestMagnitude, int& index)
{
    int  strongestMagIndex = 0;
    auto strongestMag      = abs (data[0]);

    for (int i = 1; i < dataSize; ++i)
    {
        const auto current = abs (data[i]);

        if (current > strongestMag)
        {
            strongestMag      = current;
            strongestMagIndex = i;
        }
    }

    greatestMagnitude = strongestMag;
    index             = strongestMagIndex;
}

template < typename Type >
void locateLeastAbsMagnitude (const Type* data,
                              int         dataSize,
                              Type&       leastMagnitude,
                              int&        index)
{
#if BV_USE_VDSP
    unsigned long i = 0.0;

    BV_VDSP_FUNC_SWITCH (vDSP_minmgvi, vDSP_minmgviD,
                         data, vDSP_Stride (1), &leastMagnitude, &i, vDSP_Length (dataSize))

    index = static_cast< int > (i);
#else
    int  weakestMagIndex = 0;
    auto weakestMag      = abs (data[0]);

    for (int i = 1; i < dataSize; ++i)
    {
        const auto current = abs (data[i]);

        if (current < weakestMag)
        {
            weakestMag      = current;
            weakestMagIndex = i;
        }
    }

    leastMagnitude = weakestMag;
    index          = weakestMagIndex;
#endif
}
template void locateLeastAbsMagnitude (const float*, int, float&, int&);
template void locateLeastAbsMagnitude (const double*, int, double&, int&);

template <>
void locateLeastAbsMagnitude (const int* data, int dataSize, int& leastMagnitude, int& index)
{
    int  weakestMagIndex = 0;
    auto weakestMag      = abs (data[0]);

    for (int i = 1; i < dataSize; ++i)
    {
        const auto current = abs (data[i]);

        if (current < weakestMag)
        {
            weakestMag      = current;
            weakestMagIndex = i;
        }
    }

    leastMagnitude = weakestMag;
    index          = weakestMagIndex;
}

template < typename Type >
void findExtrema (const Type* data, int dataSize, Type& min, Type& max)
{
#if BV_USE_VDSP

    BV_VDSP_FUNC_SWITCH (vDSP_minv, vDSP_minvD,
                         data, vDSP_Stride (1), &min, vDSP_Length (dataSize))

    BV_VDSP_FUNC_SWITCH (vDSP_maxv, vDSP_maxvD,
                         data, vDSP_Stride (1), &max, vDSP_Length (dataSize))
#else
    auto range     = juce::FloatVectorOperations::findMinAndMax (data, dataSize);
    min            = range.getStart();
    max            = range.getEnd();
#endif
}
template void findExtrema (const float*, int, float&, float&);
template void findExtrema (const double*, int, double&, double&);

template <>
void findExtrema (const int* data, int dataSize, int& min, int& max)
{
    min = *(std::min_element (data, data + dataSize));
    max = *(std::max_element (data, data + dataSize));
}

template < typename Type >
Type findRangeOfExtrema (const Type* data,
                         int         dataSize)
{
#if BV_USE_VDSP
    Type min = 0.0f, max = 0.0f;
    findExtrema (data, dataSize, min, max);
    return max - min;
#else
    return juce::FloatVectorOperations::findMinAndMax (data, dataSize).getLength();
#endif
}
template float  findRangeOfExtrema (const float*, int);
template double findRangeOfExtrema (const double*, int);

template <>
int findRangeOfExtrema (const int* data, int dataSize)
{
    return *(std::max_element (data, data + dataSize))
         - *(std::min_element (data, data + dataSize));
}

template < typename Type >
void normalize (Type* vector, int size)
{
#if BV_USE_VDSP
    Type          max = Type (0);
    unsigned long i   = 0.0;

    BV_VDSP_FUNC_SWITCH (vDSP_maxmgvi, vDSP_maxmgviD,
                         vector, vDSP_Stride (1), &max, &i, vDSP_Length (size))

    if (max == Type (0))
    {
        BV_VDSP_FUNC_SWITCH (vDSP_vfill, vDSP_vfillD,
                             &max, vector, vDSP_Stride (1), vDSP_Length (size))
    }
    else
    {
        const auto oneOverMax = Type (1) / max;

        BV_VDSP_FUNC_SWITCH (vDSP_vsmul, vDSP_vsmulD,
                             vector, vDSP_Stride (1), &oneOverMax, vector, vDSP_Stride (1), vDSP_Length (size))
    }
#else
    Type max = Type (0);
    int  location;

    locateGreatestAbsMagnitude (vector, size, max, location);

    if (max == Type (0))
    {
        fill (vector, Type (0), size);
    }
    else
    {
        multiplyC (vector, Type (1) / max, size);
    }
#endif
}
template void normalize (float*, int);
template void normalize (double*, int);


/* copies the contents of one vector to another. */
template < typename Type >
void copy (const Type* const source, Type* const dest, int count)
{
    memcpy (dest, source, (size_t) count * sizeof (Type));
}
template void copy (const float* const, float* const, int);
template void copy (const double* const, double* const, int);
template void copy (const int* const, int* const, int);


constexpr bool isUsingVDSP()
{
#if BV_USE_VDSP
    return true;
#else
    return false;
#endif
}

constexpr bool isUsingFallback()
{
    return ! isUsingVDSP();
}


#undef BV_VDSP_FUNC_SWITCH

}  // namespace bav::vecops
