// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2008 Gael Guennebaud <gael.guennebaud@inria.fr>
// Copyright (C) 2006-2008 Benoit Jacob <jacob.benoit.1@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EIGEN_GENERIC_PACKET_MATH_H
#define EIGEN_GENERIC_PACKET_MATH_H

/** \internal
  * \file GenericPacketMath.h
  *
  * Default implementation for types not supported by the vectorization.
  * In practice these functions are provided to make easier the writing
  * of generic vectorized code.
  */

#ifndef EIGEN_DEBUG_ALIGNED_LOAD
#define EIGEN_DEBUG_ALIGNED_LOAD
#endif

#ifndef EIGEN_DEBUG_UNALIGNED_LOAD
#define EIGEN_DEBUG_UNALIGNED_LOAD
#endif

#ifndef EIGEN_DEBUG_ALIGNED_STORE
#define EIGEN_DEBUG_ALIGNED_STORE
#endif

#ifndef EIGEN_DEBUG_UNALIGNED_STORE
#define EIGEN_DEBUG_UNALIGNED_STORE
#endif

int
    HasHalfPacket = 0,

    HasAdd    = 1,
    HasSub    = 1,
    HasMul    = 1,
    HasNegate = 1,
    HasAbs    = 1,
    HasArg    = 0,
    HasAbs2   = 1,
    HasMin    = 1,
    HasMax    = 1,
    HasConj   = 1,
    HasSetLinear = 1,
    HasBlend  = 0,

    HasDiv    = 0,
    HasSqrt   = 0,
    HasRsqrt  = 0,
    HasExp    = 0,
    HasLog    = 0,
    HasLog1p  = 0,
    HasLog10  = 0,
    HasPow    = 0,

    HasSin    = 0,
    HasCos    = 0,
    HasTan    = 0,
    HasASin   = 0,
    HasACos   = 0,
    HasATan   = 0,
    HasSinh   = 0,
    HasCosh   = 0,
    HasTanh   = 0,
    HasLGamma = 0,
    HasDiGamma = 0,
    HasZeta = 0,
    HasPolygamma = 0,
    HasErf = 0,
    HasErfc = 0,
    HasIGamma = 0,
    HasIGammac = 0,
    HasBetaInc = 0,

    HasRound  = 0,
    HasFloor  = 0,
    HasCeil   = 0,

    HasSign   = 0,



    Vectorizable = 0,
    size = 1,
    AlignedOnScalar = 0,
    //HasHalfPacket = 0,

    //HasAdd    = 0,
    //HasSub    = 0,
    //HasMul    = 0,
    //HasNegate = 0,
    //HasAbs    = 0,
    //HasAbs2   = 0,
    //HasMin    = 0,
    //HasMax    = 0,
    //HasConj   = 0,
    //HasSetLinear = 0

    VectorizedCast = 0,
    SrcCoeffRatio = 1,
    TgtCoeffRatio = 1;


/** \internal \returns static_cast<TgtType>(a) (coeff-wise) */
//EIGEN_DEVICE_FUNC inline TgtPacket pcast(const Packet& a) {  return static_cast<TgtPacket>(a); }
//EIGEN_DEVICE_FUNC inline TgtPacket pcast(const Packet& a, const Packet& /*b*/) {  return static_cast<TgtPacket>(a); }
//EIGEN_DEVICE_FUNC inline TgtPacket pcast(const Packet& a, const Packet& /*b*/, const Packet& /*c*/, const Packet& /*d*/) {  return static_cast<TgtPacket>(a);}

/** \internal \returns a + b (coeff-wise) */
inline Packet padd(const Packet& a, const Packet& b) { return a+b; }

/** \internal \returns a - b (coeff-wise) */
EIGEN_DEVICE_FUNC inline Packet
psub(const Packet& a,
        const Packet& b) { return a-b; }

/** \internal \returns -a (coeff-wise) */
EIGEN_DEVICE_FUNC inline Packet
pnegate(const Packet& a) { return -a; }

/** \internal \returns conj(a) (coeff-wise) */

/** \internal \returns a * b (coeff-wise) */
EIGEN_DEVICE_FUNC inline Packet
pmul(const Packet& a,
        const Packet& b) { return a*b; }

/** \internal \returns a / b (coeff-wise) */
EIGEN_DEVICE_FUNC inline Packet
pdiv(const Packet& a,
        const Packet& b) { return a/b; }

/** \internal \returns the min of \a a and \a b  (coeff-wise) */
EIGEN_DEVICE_FUNC inline Packet
pmin(const Packet& a,
        const Packet& b) { return numext::mini(a, b); }

/** \internal \returns the max of \a a and \a b  (coeff-wise) */
EIGEN_DEVICE_FUNC inline Packet
pmax(const Packet& a,
        const Packet& b) { return numext::maxi(a, b); }

/** \internal \returns the absolute value of \a a */
EIGEN_DEVICE_FUNC inline Packet
pabs(const Packet& a) { return abs(a); }


//EIGEN_DEVICE_FUNC inline Packet pand(const Packet& a, const Packet& b) { return a & b; }
//EIGEN_DEVICE_FUNC inline Packet por(const Packet& a, const Packet& b) { return a | b; }
//EIGEN_DEVICE_FUNC inline Packet pxor(const Packet& a, const Packet& b) { return a ^ b; }
//EIGEN_DEVICE_FUNC inline Packet pandnot(const Packet& a, const Packet& b) { return a & (!b); }

/** \internal \returns a packet version of \a *from, from must be 16 bytes aligned */
EIGEN_DEVICE_FUNC inline Packet pload(const T* from) { return *from; }

/** \internal \returns a packet version of \a *from, (un-aligned load) */
EIGEN_DEVICE_FUNC inline Packet
ploadu(const T* from) { return *from; }

/** \internal \returns a packet with constant coefficients \a a, e.g.: (a,a,a,a) */
EIGEN_DEVICE_FUNC inline Packet
pset1(const T& a) { return a; }

/** \internal \returns a packet with constant coefficients \a a[0], e.g.: (a[0],a[0],a[0],a[0]) */
EIGEN_DEVICE_FUNC inline Packet
pload1(const T  *a) { return pset1(*a); }

/** \internal \returns a packet with elements of \a *from duplicated.
  * For instance, for a packet of 8 elements, 4 scalars will be read from \a *from and
  * duplicated to form: {from[0],from[0],from[1],from[1],from[2],from[2],from[3],from[3]}
  * Currently, this function is only used for scalar * complex products.
  */
EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE Packet
ploaddup(const T* from) { return *from; }

/** \internal \returns a packet with elements of \a *from quadrupled.
  * For instance, for a packet of 8 elements, 2 scalars will be read from \a *from and
  * replicated to form: {from[0],from[0],from[0],from[0],from[1],from[1],from[1],from[1]}
  * Currently, this function is only used in matrix products.
  * For packet-size smaller or equal to 4, this function is equivalent to pload1 
  */
EIGEN_DEVICE_FUNC inline Packet
ploadquad(const T* from)
{ return pload1(from); }

/** \internal equivalent to
  * \code
  * a0 = pload1(a+0);
  * a1 = pload1(a+1);
  * a2 = pload1(a+2);
  * a3 = pload1(a+3);
  * \endcode
  * \sa pset1, pload1, ploaddup, pbroadcast2
  */
EIGEN_DEVICE_FUNC
inline void pbroadcast4(const T *a,
                        Packet& a0, Packet& a1, Packet& a2, Packet& a3)
{
  a0 = pload1(a+0);
  a1 = pload1(a+1);
  a2 = pload1(a+2);
  a3 = pload1(a+3);
}

/** \internal equivalent to
  * \code
  * a0 = pload1(a+0);
  * a1 = pload1(a+1);
  * \endcode
  * \sa pset1, pload1, ploaddup, pbroadcast4
  */
EIGEN_DEVICE_FUNC
inline void pbroadcast2(const T *a,
                        Packet& a0, Packet& a1)
{
  a0 = pload1(a+0);
  a1 = pload1(a+1);
}

/** \internal \brief Returns a packet with coefficients (a,a+1,...,a+packet_size-1). */
EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE Packet
plset(const T& a) { return a; }

/** \internal copy the packet \a from to \a *to, \a to must be 16 bytes aligned */
EIGEN_DEVICE_FUNC inline void pstore(Scalar* to, const Packet& from)
{ (*to) = from; }

/** \internal copy the packet \a from to \a *to, (un-aligned store) */
EIGEN_DEVICE_FUNC inline void pstoreu(Scalar* to, const Packet& from)
{  (*to) = from; }

 EIGEN_DEVICE_FUNC inline Packet pgather(const Scalar* from, Index /*stride*/)
 { return ploadu(from); }

 EIGEN_DEVICE_FUNC inline void pscatter(Scalar* to, const Packet& from, Index /*stride*/)
 { pstore(to, from); }

/** \internal tries to do cache prefetching of \a addr */
EIGEN_DEVICE_FUNC inline void prefetch(const Scalar* addr)
{
#ifdef __CUDA_ARCH__
#if defined(__LP64__)
  // 64-bit pointer operand constraint for inlined asm
  asm(" prefetch.L1 [ %1 ];" : "=l"(addr) : "l"(addr));
#else
  // 32-bit pointer operand constraint for inlined asm
  asm(" prefetch.L1 [ %1 ];" : "=r"(addr) : "r"(addr));
#endif
#elif (!EIGEN_COMP_MSVC) && (EIGEN_COMP_GNUC || EIGEN_COMP_CLANG || EIGEN_COMP_ICC)
  __builtin_prefetch(addr);
#endif
}

/** \internal \returns the first element of a packet */
EIGEN_DEVICE_FUNC inline T pfirst(const Packet& a)
{ return a; }

/** \internal \returns a packet where the element i contains the sum of the packet of \a vec[i] */
EIGEN_DEVICE_FUNC inline Packet
preduxp(const Packet* vecs) { return vecs[0]; }

/** \internal \returns the sum of the elements of \a a*/
EIGEN_DEVICE_FUNC inline T predux(const Packet& a)
{ return a; }

/** \internal \returns the sum of the elements of \a a by block of 4 elements.
  * For a packet {a0, a1, a2, a3, a4, a5, a6, a7}, it returns a half packet {a0+a4, a1+a5, a2+a6, a3+a7}
  * For packet-size smaller or equal to 4, this boils down to a noop.
  */
EIGEN_DEVICE_FUNC inline
T predux_downto4(const Packet& a)
{ return a; }

/** \internal \returns the product of the elements of \a a*/
EIGEN_DEVICE_FUNC inline T predux_mul(const Packet& a)
{ return a; }

/** \internal \returns the min of the elements of \a a*/
EIGEN_DEVICE_FUNC inline T predux_min(const Packet& a)
{ return a; }

/** \internal \returns the max of the elements of \a a*/
EIGEN_DEVICE_FUNC inline T predux_max(const Packet& a)
{ return a; }

/** \internal \returns the reversed elements of \a a*/
EIGEN_DEVICE_FUNC inline Packet preverse(const Packet& a)
{ return a; }

/**************************
* Special math functions
***************************/

/** \internal \returns the sine of \a a (coeff-wise) */
EIGEN_DECLARE_FUNCTION_ALLOWING_MULTIPLE_DEFINITIONS
Packet psin(const Packet& a) { return sin(a); }

/** \internal \returns the cosine of \a a (coeff-wise) */
EIGEN_DECLARE_FUNCTION_ALLOWING_MULTIPLE_DEFINITIONS
Packet pcos(const Packet& a) { return cos(a); }

/** \internal \returns the tan of \a a (coeff-wise) */
EIGEN_DECLARE_FUNCTION_ALLOWING_MULTIPLE_DEFINITIONS
Packet ptan(const Packet& a) { return tan(a); }

/** \internal \returns the arc sine of \a a (coeff-wise) */
EIGEN_DECLARE_FUNCTION_ALLOWING_MULTIPLE_DEFINITIONS
Packet pasin(const Packet& a) { return asin(a); }

/** \internal \returns the arc cosine of \a a (coeff-wise) */
EIGEN_DECLARE_FUNCTION_ALLOWING_MULTIPLE_DEFINITIONS
Packet pacos(const Packet& a) { return acos(a); }

/** \internal \returns the arc tangent of \a a (coeff-wise) */
EIGEN_DECLARE_FUNCTION_ALLOWING_MULTIPLE_DEFINITIONS
Packet patan(const Packet& a) { return atan(a); }

/** \internal \returns the hyperbolic sine of \a a (coeff-wise) */
EIGEN_DECLARE_FUNCTION_ALLOWING_MULTIPLE_DEFINITIONS
Packet psinh(const Packet& a) { return sinh(a); }

/** \internal \returns the hyperbolic cosine of \a a (coeff-wise) */
EIGEN_DECLARE_FUNCTION_ALLOWING_MULTIPLE_DEFINITIONS
Packet pcosh(const Packet& a) { return cosh(a); }

/** \internal \returns the hyperbolic tan of \a a (coeff-wise) */
EIGEN_DECLARE_FUNCTION_ALLOWING_MULTIPLE_DEFINITIONS
Packet ptanh(const Packet& a) { return tanh(a); }

/** \internal \returns the exp of \a a (coeff-wise) */
EIGEN_DECLARE_FUNCTION_ALLOWING_MULTIPLE_DEFINITIONS
Packet pexp(const Packet& a) { return exp(a); }

/** \internal \returns the log of \a a (coeff-wise) */
EIGEN_DECLARE_FUNCTION_ALLOWING_MULTIPLE_DEFINITIONS
Packet plog(const Packet& a) { return log(a); }

/** \internal \returns the log1p of \a a (coeff-wise) */
EIGEN_DECLARE_FUNCTION_ALLOWING_MULTIPLE_DEFINITIONS
Packet plog1p(const Packet& a) { return log1p(a); }

/** \internal \returns the log10 of \a a (coeff-wise) */
EIGEN_DECLARE_FUNCTION_ALLOWING_MULTIPLE_DEFINITIONS
Packet plog10(const Packet& a) { return log10(a); }

/** \internal \returns the square-root of \a a (coeff-wise) */
EIGEN_DECLARE_FUNCTION_ALLOWING_MULTIPLE_DEFINITIONS
Packet psqrt(const Packet& a) { return sqrt(a); }

/** \internal \returns the reciprocal square-root of \a a (coeff-wise) */
EIGEN_DECLARE_FUNCTION_ALLOWING_MULTIPLE_DEFINITIONS
Packet prsqrt(const Packet& a) {
  return pdiv(pset1(1), psqrt(a));
}

/** \internal \returns the rounded value of \a a (coeff-wise) */
EIGEN_DECLARE_FUNCTION_ALLOWING_MULTIPLE_DEFINITIONS
Packet pround(const Packet& a) { return round(a); }

/** \internal \returns the floor of \a a (coeff-wise) */
EIGEN_DECLARE_FUNCTION_ALLOWING_MULTIPLE_DEFINITIONS
Packet pfloor(const Packet& a) { return floor(a); }

/** \internal \returns the ceil of \a a (coeff-wise) */
EIGEN_DECLARE_FUNCTION_ALLOWING_MULTIPLE_DEFINITIONS
Packet pceil(const Packet& a) { return ceil(a); }

/***************************************************************************
* The following functions might not have to be overwritten for vectorized types
***************************************************************************/

/** \internal copy a packet with constant coeficient \a a (e.g., [a,a,a,a]) to \a *to. \a to must be 16 bytes aligned */
// NOTE: this function must really be templated on the packet type (think about different packet types for the same scalar type)

inline void pstore1(T* to, const T& a)
{
  pstore(to, pset1(a));
}

/** \internal \returns a * b + c (coeff-wise) */
EIGEN_DEVICE_FUNC inline Packet
pmadd(const Packet&  a,
         const Packet&  b,
         const Packet&  c)
{ return padd(pmul(a, b),c); }

/** \internal \returns a packet version of \a *from.
  * The pointer \a from must be aligned on a \a Alignment bytes boundary. */

EIGEN_DEVICE_FUNC EIGEN_ALWAYS_INLINE Packet ploadt(const T* from)
{
    //return pload(from);
    return ploadu(from);
}

/** \internal copy the packet \a from to \a *to.
  * The pointer \a from must be aligned on a \a Alignment bytes boundary. */

EIGEN_DEVICE_FUNC EIGEN_ALWAYS_INLINE void pstoret(Scalar* to, const Packet& from)
{
    //pstore(to, from);
    pstoreu(to, from);
}

/** \internal \returns a packet version of \a *from.
  * Unlike ploadt, ploadt_ro takes advantage of the read-only memory path on the
  * hardware if available to speedup the loading of data that won't be modified
  * by the current computation.
  */

EIGEN_DEVICE_FUNC EIGEN_ALWAYS_INLINE Packet ploadt_ro(const T* from)
{
  return ploadt(from);
}

/** \internal default implementation of palign() allowing partial specialization */
template<int Offset,typename PacketType>
struct palign_impl
{
  // by default data are aligned, so there is nothing to be done :)
  static inline void run(PacketType&, const PacketType&) {}
};

/** \internal update \a first using the concatenation of the packet_size minus \a Offset last elements
  * of \a first and \a Offset first elements of \a second.
  * 
  * This function is currently only used to optimize matrix-vector products on unligned matrices.
  * It takes 2 packets that represent a contiguous memory array, and returns a packet starting
  * at the position \a Offset. For instance, for packets of 4 elements, we have:
  *  Input:
  *  - first = {f0,f1,f2,f3}
  *  - second = {s0,s1,s2,s3}
  * Output: 
  *   - if Offset==0 then {f0,f1,f2,f3}
  *   - if Offset==1 then {f1,f2,f3,s0}
  *   - if Offset==2 then {f2,f3,s0,s1}
  *   - if Offset==3 then {f3,s0,s1,s3}
  */
template<int Offset,typename PacketType>
inline void palign(PacketType& first, const PacketType& second)
{
  palign_impl<Offset,PacketType>::run(first,second);
}

/***************************************************************************
* Fast complex products (GCC generates a function call which is very slow)
***************************************************************************/

// Eigen+CUDA does not support complexes.
#ifndef __CUDACC__

inline complex pmul(const complex& a, const complex& b) {
  complex c;
  c.re = real(a)*real(b) - imag(a)*imag(b);
  c.im = imag(a)*real(b) + real(a)*imag(b);
  return c;
}


#endif


/***************************************************************************
 * PacketBlock, that is a collection of N packets where the number of words
 * in the packet is a multiple of N.
***************************************************************************/
#define TEMPLATE_PACKETBLOCK(N) struct PacketBlock##N { Packet packet[N]; }

TEMPLATE_PACKETBLOCK(1);
TEMPLATE_PACKETBLOCK(4);
TEMPLATE_PACKETBLOCK(8);
TEMPLATE_PACKETBLOCK(16);

EIGEN_DEVICE_FUNC inline void
ptranspose(PacketBlock1& /*kernel*/) {
  // Nothing to do in the scalar case, i.e. a 1x1 matrix.
}

#if 0
/***************************************************************************
 * Selector, i.e. vector of N boolean values used to select (i.e. blend)
 * words from 2 packets.
***************************************************************************/
template <size_t N> struct Selector {
  bool select[N];
};

EIGEN_DEVICE_FUNC inline Packet
pblend(const Selector<unpacket_traits::size>& ifPacket, const Packet& thenPacket, const Packet& elsePacket) {
  return ifPacket.select[0] ? thenPacket : elsePacket;
}

/** \internal \returns \a a with the first coefficient replaced by the scalar b */
EIGEN_DEVICE_FUNC inline Packet
pinsertfirst(const Packet& a, T b)
{
  // Default implementation based on pblend.
  // It must be specialized for higher performance.
  Selector<unpacket_traits::size> mask;
  mask.select[0] = true;
  // This for loop should be optimized away by the compiler.
  for(Index i=1; i<unpacket_traits::size; ++i)
    mask.select[i] = false;
  return pblend(mask, pset1(b), a);
}

/** \internal \returns \a a with the last coefficient replaced by the scalar b */
EIGEN_DEVICE_FUNC inline Packet
pinsertlast(const Packet& a, T b)
{
  // Default implementation based on pblend.
  // It must be specialized for higher performance.
  Selector<unpacket_traits::size> mask;
  // This for loop should be optimized away by the compiler.
  for(Index i=0; i<unpacket_traits::size-1; ++i)
    mask.select[i] = false;
  mask.select[unpacket_traits::size-1] = true;
  return pblend(mask, pset1(b), a);
}


#endif

#endif // EIGEN_GENERIC_PACKET_MATH_H
