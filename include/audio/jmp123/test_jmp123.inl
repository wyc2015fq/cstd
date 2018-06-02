#include "cstd.h"
#define null 0
CC_INLINE void arraycopy(const byte* src, int srcPos, byte* dest, int destPos, int length) {
  memcpy(dest + destPos, src + srcPos, length);
}
//typedef BOOL boolean;
#include "IAudio.inl"
#include "Header.inl"
#include "Synthesis.inl"
#include "AbstractLayer.inl"
#include "BitStream.inl"
#include "BitStreamMainData.inl"
#include "Layer1.inl"
#include "Layer2.inl"
#include "Layer3.inl"
//#include "AbstractDecoder.inl"
int test_jmp123()
{
  return 0;
}

