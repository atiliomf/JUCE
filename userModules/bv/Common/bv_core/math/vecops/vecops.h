
#pragma once

namespace bav::vecops
{
/* fills a vector with the specified value. */
template < typename Type >
void fill (Type* vector, Type value, int count);


/* copies the contents of one vector to another. */
template < typename Type >
void copy (const Type* const source, Type* const dest, int count);


/* copies each value of src into dst. The vectors may have different value types. If they are the same type, this is the same as using copy */
template < typename Type1, typename Type2 >
void convert (Type1* const dst, const Type2* const src, int count);


/* adds a single operand to each value in the vector */
template < typename Type >
void addC (Type* vector, Type value, int count);


/* performs element-wise addition of two vectors and writes the output to vecA */
template < typename Type >
void addV (Type* vecA, const Type* vecB, int count);


/* subtracts a single operand from every element in the vector */
template < typename Type >
void subtractC (Type* vector, Type value, int count);


/* performs element-wise subtraction of two vectors and writes the output to vecA */
template < typename Type >
void subtractV (Type* vecA, const Type* vecB, int count);


/* multiplies every element in the vector by a single operand */
template < typename Type >
void multiplyC (Type* vector, Type value, int count);


/* performs element-wise multiplication of two vectors and writes the output to vecA */
template < typename Type >
void multiplyV (Type* vecA, const Type* vecB, int count);


/* divides every element in the vector by a single operand */
template < typename Type >
void divideC (Type* vector, Type value, int count);


/* performs element-wise division of two vectors and writes the output to vecA */
template < typename Type >
void divideV (Type* vecA, const Type* vecB, int count);


/* replaces every element in the passed vector with its square root */
template < typename Type >
void squareRoot (Type* data, int dataSize);


/* replaces every element in the passed vector with its square */
template < typename Type >
void square (Type* data, int dataSize);


/* replaces every element in the passed vector with its absolute value */
template < typename Type >
void absVal (Type* data, int dataSize);


/* returns the index in the vector of the minimum element */
template < typename Type >
int findIndexOfMinElement (const Type* data, int dataSize);


/* returns the index in the vector of the maximum element */
template < typename Type >
int findIndexOfMaxElement (const Type* data, int dataSize);


/* returns both the minimum element and its index in the vector into the variables minimum and minIndex */
template < typename Type >
void findMinAndMinIndex (const Type* data,
                         int         dataSize,
                         Type&       minimum,
                         int&        minIndex);


/* returns both the maximum element and its index in the vector into the variables maximum and maxIndex */
template < typename Type >
void findMaxAndMaxIndex (const Type* data,
                         int         dataSize,
                         Type&       maximum,
                         int&        maxIndex);


/* locates the element with the highest absolute value and its index in the vector, and returns them into the variables greatestMagnitude and index */
template < typename Type >
void locateGreatestAbsMagnitude (const Type* data,
                                 int         dataSize,
                                 Type&       greatestMagnitude,
                                 int&        index);


/* locates the element with the lowest absolute value and its index in the vector, and returns them into the variables leastMagnitude and index */
template < typename Type >
void locateLeastAbsMagnitude (const Type* data,
                              int         dataSize,
                              Type&       leastMagnitude,
                              int&        index);


/* finds both the maximum and minimum elements in the vector and returns them into the variables max and min */
template < typename Type >
void findExtrema (const Type* data, int dataSize, Type& min, Type& max);


/* returns the distance between the maximum and minimum element of the vector */
template < typename Type >
Type findRangeOfExtrema (const Type* data,
                         int         dataSize);


/* Normalises a set of samples to the absolute maximum contained within the buffer. */
template < typename Type >
void normalize (Type* vector, int size);


extern constexpr bool isUsingVDSP();

extern constexpr bool isUsingFallback();


}  // namespace bav::vecops
