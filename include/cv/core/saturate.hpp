
#ifndef OPENCC_CORE_SATURATE_HPP
#define OPENCC_CORE_SATURATE_HPP

//! @addtogroup core_utils
//! @{

/////////////// saturate_cast (used in image & signal processing) ///////////////////

/** @brief Template function for accurate conversion from one primitive type to another.

 The functions saturate_cast resemble the standard C++ cast operations, such as static_cast\<T\()
 and others. They perform an efficient and accurate conversion from one primitive type to another
 (see the introduction chapter). saturate in the name means that when the input value v is out of the
 range of the target type, the result is not formed just by taking low bits of the input, but instead
 the value is clipped. For example:
 @code
 uchar a = saturate_cast_uchar(-100); // a = 0 (UCHAR_MIN)
 short b = saturate_cast_short(33333.33333); // b = 32767 (SHRT_MAX)
 @endcode
 Such clipping is done when the target type is unsigned char , signed char , unsigned short or
 signed short . For 32-bit integers, no clipping is done.

 When the parameter is a floating-point value and the target type is an integer (8-, 16- or 32-bit),
 the floating-point value is first rounded to the nearest integer and then clipped if needed (when
 the target type is 8- or 16-bit).

 This operation is used in the simplest or most complex image processing functions in OpenCV.

 @param v Function parameter.
 @sa add, subtract, multiply, divide, CvMat::convertTo
 */

#define int2uchar(v)   (uchar)((unsigned)v <= UCHAR_MAX ? v : v > 0 ? UCHAR_MAX : 0)
#define int2schar(v)   (schar)((unsigned)(v-SCHAR_MIN) <= (unsigned)UCHAR_MAX ? v : v > 0 ? SCHAR_MAX : SCHAR_MIN)
#define int2ushort(v)  (ushort)((unsigned)v <= (unsigned)USHRT_MAX ? v : v > 0 ? USHRT_MAX : 0)
#define int2short(v)   (short)((unsigned)(v - SHRT_MIN) <= (unsigned)USHRT_MAX ? v : v > 0 ? SHRT_MAX : SHRT_MIN)

#define SATURATE_CAST_FUNC_DEF_DEF(SATURATE_CAST_FUNC_DEF) \
SATURATE_CAST_FUNC_DEF(uchar, schar, (uchar)MAX((int)v, 0) ) \
SATURATE_CAST_FUNC_DEF(uchar, ushort, (uchar)MIN((unsigned)v, (unsigned)UCHAR_MAX) ) \
SATURATE_CAST_FUNC_DEF(uchar, int, int2uchar(v)  ) \
SATURATE_CAST_FUNC_DEF(uchar, short, int2uchar(v) ) \
SATURATE_CAST_FUNC_DEF(uchar, unsigned, (uchar)MIN(v, (unsigned)UCHAR_MAX) ) \
SATURATE_CAST_FUNC_DEF(uchar, float, int2uchar((int)ROUND(v)) ) \
SATURATE_CAST_FUNC_DEF(uchar, double, int2uchar((int)ROUND(v)) ) \
SATURATE_CAST_FUNC_DEF(uchar, int64, (uchar)((uint64)v <= (uint64)UCHAR_MAX ? v : v > 0 ? UCHAR_MAX : 0) ) \
SATURATE_CAST_FUNC_DEF(uchar, uint64, (uchar)MIN(v, (uint64)UCHAR_MAX) ) \
SATURATE_CAST_FUNC_DEF(schar, uchar, (schar)MIN((int)v, SCHAR_MAX) ) \
SATURATE_CAST_FUNC_DEF(schar, ushort, (schar)MIN((unsigned)v, (unsigned)SCHAR_MAX) ) \
SATURATE_CAST_FUNC_DEF(schar, int, int2schar(v) ) \
SATURATE_CAST_FUNC_DEF(schar, short, int2schar((int)v) ) \
SATURATE_CAST_FUNC_DEF(schar, unsigned, (schar)MIN(v, (unsigned)SCHAR_MAX) ) \
SATURATE_CAST_FUNC_DEF(schar, float, int2schar((int)ROUND(v)) ) \
SATURATE_CAST_FUNC_DEF(schar, double, int2schar((int)ROUND(v)) ) \
SATURATE_CAST_FUNC_DEF(schar, int64, (schar)((uint64)((int64)v-SCHAR_MIN) <= (uint64)UCHAR_MAX ? v : v > 0 ? SCHAR_MAX : SCHAR_MIN) ) \
SATURATE_CAST_FUNC_DEF(schar, uint64, (schar)MIN(v, (uint64)SCHAR_MAX) ) \
SATURATE_CAST_FUNC_DEF(ushort, schar, (ushort)MAX((int)v, 0) ) \
SATURATE_CAST_FUNC_DEF(ushort, short, (ushort)MAX((int)v, 0) ) \
SATURATE_CAST_FUNC_DEF(ushort, int, int2ushort(v) ) \
SATURATE_CAST_FUNC_DEF(ushort, unsigned, (ushort)MIN(v, (unsigned)USHRT_MAX) ) \
SATURATE_CAST_FUNC_DEF(ushort, float, int2ushort((int)ROUND(v)) ) \
SATURATE_CAST_FUNC_DEF(ushort, double, int2ushort((int)ROUND(v)) ) \
SATURATE_CAST_FUNC_DEF(ushort, int64, (ushort)((uint64)v <= (uint64)USHRT_MAX ? v : v > 0 ? USHRT_MAX : 0) ) \
SATURATE_CAST_FUNC_DEF(ushort, uint64, (ushort)MIN(v, (uint64)USHRT_MAX) ) \
SATURATE_CAST_FUNC_DEF(short, ushort, (short)MIN((int)v, SHRT_MAX) ) \
SATURATE_CAST_FUNC_DEF(short, int, int2short(v) ) \
SATURATE_CAST_FUNC_DEF(short, unsigned, (short)MIN(v, (unsigned)SHRT_MAX) ) \
SATURATE_CAST_FUNC_DEF(short, float, int2short((int)ROUND(v)) ) \
SATURATE_CAST_FUNC_DEF(short, double, int2short((int)ROUND(v)) ) \
SATURATE_CAST_FUNC_DEF(short, int64, (short)((uint64)((int64)v - SHRT_MIN) <= (uint64)USHRT_MAX ? v : v > 0 ? SHRT_MAX : SHRT_MIN) ) \
SATURATE_CAST_FUNC_DEF(short, uint64, (short)MIN(v, (uint64)SHRT_MAX) ) \
SATURATE_CAST_FUNC_DEF(int, float, ROUND(v) ) \
SATURATE_CAST_FUNC_DEF(int, double, ROUND(v) ) \
SATURATE_CAST_FUNC_DEF(unsigned, float, ROUND(v) ) \
SATURATE_CAST_FUNC_DEF(unsigned, double, ROUND(v) )

// we intentionally do not clip negative numbers, to make -1 become 0xffffffff etc.
//! @}

#define SATURATE_CAST_DEF(T, Y, todo) static T Y ## _to_ ## T(Y v) { return todo;}
SATURATE_CAST_FUNC_DEF_DEF(SATURATE_CAST_DEF)
#define SATURATE_CAST_DEF2(T, Y, todo) static T Y ## _ ## T(Y v) { return todo;}
SATURATE_CAST_FUNC_DEF_DEF(SATURATE_CAST_DEF2)

template <typename T, typename Y> static void saturate_cast(T& x, Y v) {x = v;}

#define SATURATE_CAST_DEF3(T, Y, todo) static void saturate_cast(T& x, Y v) { x = todo;}
SATURATE_CAST_FUNC_DEF_DEF(SATURATE_CAST_DEF3)

#endif // OPENCC_CORE_SATURATE_HPP
