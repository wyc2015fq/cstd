
#include "cstd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned short readUS(FILE* fp)
{
  unsigned short nVal;
  fseek(fp, 2, SEEK_CUR); // Skip Length bytes (2)
  fread(&nVal, 1, sizeof(unsigned short), fp);
  return nVal;
}

static void removeTailingSpace(char* str)
{
  unsigned short nLen = strlen(str);
  char* tail = str + nLen - 1 ;

  while ((tail != str) && (*tail == '\t' || *tail == ' ')) {
    *tail--  = 0;
  }
}

static void readString(FILE* fp, char* str)
{
  unsigned short nLength = 0;
  fread(&nLength, 2, 1, fp);

  if (nLength < 0 || nLength > 64) {
    *str = 0;
    return;
  }

  fread(str, 1, nLength, fp);

  if (nLength != 64) {
    str[nLength] = 0;
  }

  removeTailingSpace(str);
}

static unsigned short readIS(FILE* fp)
{
  char szTemp[64] = "";
  unsigned short nVal = 0;

  readString(fp, szTemp);
  sscanf(szTemp, "%d", &nVal);

  return nVal;
}

float readDS(FILE* fp)
{
  char szTemp[64] = "";
  float nVal = 0;

  readString(fp, szTemp);
  sscanf(szTemp, "%f", &nVal);

  return nVal;
}

unsigned char* findUIDItem(unsigned char* lpFMBuf, unsigned long ulFMLen, unsigned short gTag, unsigned short eTag)
{
  unsigned short* pTmp = (unsigned short*)lpFMBuf;

  while ((unsigned char*)(pTmp + 1) < lpFMBuf + ulFMLen) {
    if (*pTmp == gTag) {
      if (*(pTmp + 1) == eTag) {
        break;
      }
    }

    pTmp++;
  }

  if ((unsigned char*)(pTmp + 1) < lpFMBuf + ulFMLen) {
    pTmp += 2;
    //skip VR
    pTmp++;
    return (unsigned char*)pTmp;
  }

  return 0;
}

unsigned char* convertTo8Bit_window(unsigned char* pData, unsigned long nNumPixels, unsigned short bIsSigned, unsigned short nHighBit,
    float fRescaleSlope, float fRescaleIntercept,
    float fWindowCenter, float fWindowWidth)
{
  unsigned char* pNewData = 0;
  unsigned long nCount;
  unsigned short* pp;

  // 1. Clip the high bits.
  if (nHighBit < 15) {
    unsigned short nMask;
    unsigned short nSignBit;

    pp = (unsigned short*)pData;
    nCount = nNumPixels;

    if (bIsSigned == 0) { // Unsigned integer
      nMask = 0xffff << (nHighBit + 1);

      while (nCount-- > 0) {
        *(pp ++) &= ~nMask;
      }
    }
    else {
      // 2's complement representation
      nSignBit = 1 << nHighBit;
      nMask = 0xffff << (nHighBit + 1);

      while (nCount -- > 0) {
        if ((*pp & nSignBit) != 0) {
          *(pp ++) |= nMask;
        }
        else {
          *(pp ++) &= ~nMask;
        }
      }
    }
  }

  // 2. Rescale if needed (especially for CT)
  if ((fRescaleSlope != 1.0f) || (fRescaleIntercept != 0.0f)) {
    float fValue;
    short* ps = (short*)pData;
    nCount = nNumPixels;

    while (nCount-- > 0) {
      fValue = (*ps) * fRescaleSlope + fRescaleIntercept;
      *ps = (short)fValue;
      ps++;
    }
  }

  // 3. Window-level or rescale to 8-bit
  if ((fWindowCenter != 0) || (fWindowWidth != 0)) {
    float fSlope;
    float fShift;
    float fValue;
    short* ps;
    float f_tmp;
    unsigned char* np = NULL;
    MYREALLOC(np, nNumPixels);

    pNewData = np;

    // Since we have window level info, we will only map what are
    // within the Window.

    fShift = fWindowCenter - fWindowWidth / 2.0f;
    fSlope = 255.0f / fWindowWidth;

    nCount = nNumPixels;
    ps = (short*)pData;
    f_tmp = *ps;

    while (nCount-- > 0) {
      fValue = ((*ps ++) - fShift) * fSlope;

      if (fValue < 0) {
        fValue = 0;
      }
      else if (fValue > 255) {
        fValue = 255;
      }

      *np ++ = (unsigned char) fValue;
    }
  }
  else {
    // We will map the whole dynamic range.
    float fSlope;
    float fValue;
    int nMin, nMax;
    short* ps;
    unsigned char* np = NULL;
    MYREALLOC(np, nNumPixels);

    pNewData = np;

    // First compute the min and max.
    nCount = nNumPixels;
    ps = (short*)pData;
    nMin = nMax = *ps;

    while (nCount-- > 0) {
      if (*ps < nMin) {
        nMin = *ps;
      }

      if (*ps > nMax) {
        nMax = *ps;
      }

      ps ++;
    }

    // Calculate the scaling factor.
    if (nMax != nMin) {
      fSlope = 255.0f / (nMax - nMin);
    }
    else {
      fSlope = 1.0f;
    }

    nCount = nNumPixels;
    ps = (short*)pData;

    while (nCount-- > 0) {
      fValue = ((*ps ++) - nMin) * fSlope;

      if (fValue < 0) {
        fValue = 0;
      }
      else if (fValue > 255) {
        fValue = 255;
      }

      *np ++ = (unsigned char) fValue;
    }
  }

  return pNewData;
}

void readItem(FILE* fp)
{
  unsigned short usItemLen;
  unsigned short gTag, eTag;
  fread(&usItemLen, sizeof(unsigned short), 1, fp);

  if (usItemLen == 0xffffffff) {
    printf("Item's length should be defined!");

    // undefine length
    while (1) {
      fread(&gTag, 2, 1, fp);

      if (gTag == 0xfffe) {
        fread(&eTag, 2, 1, fp);

        if (eTag == 0xe00d) {
          unsigned long useless;
          fread(&useless, sizeof(unsigned long), 1, fp);
          break;
        }
        else {
          fseek(fp, -2L, SEEK_CUR);
        }
      }
    }
  }
  else {
    // define length
    fseek(fp, (int)usItemLen, SEEK_CUR);
  }
}

bool readWItem(FILE* fp, char* vr)
{
  unsigned short tmp;
  unsigned long len;
  fread(&tmp, 2, 1, fp);

  if (tmp != 0x0000) {
    printf("0x0000 is expected!");
    return false;
  }

  if (!strcmp(vr, "OB") || !strcmp(vr, "OW") || !strcmp(vr, "OF") || !strcmp(vr, "UT") || !strcmp(vr, "UN")) {
    // "OB" "Other Byte String"
    // "OW" "Other Word String"
    // "OF" "Other Float String"
    // "UT" "Unlimited Text"
    // "UN" "Unknow"
    fread(&len, sizeof(unsigned long), 1, fp);

    if (len != 0xffffffff) {
      fseek(fp, (long)len, SEEK_CUR);
      return true;
    }
    else {
      unsigned short gTag, eTag;

      while (1) {
        fread(&gTag, 2, 1, fp);

        if (gTag == 0xfffe) {
          fread(&eTag, 2, 1, fp);

          if (eTag == 0xe00d) {
            unsigned long useless;
            fread(&useless, sizeof(unsigned long), 1, fp);
            break;
          }
          else {
            fseek(fp, -2L, SEEK_CUR);
          }
        }
      }
    }
  }
  else if (!strcmp(vr, "SQ")) {
    // "SQ" "Sequence Of Items"
    fread(&len, sizeof(unsigned long), 1, fp);

    if (len != 0xffffffff) {
      fseek(fp, (long)len, SEEK_CUR);
      return true;
    }
    else {
      unsigned short gTag, eTag;

      while (1) {
        fread(&gTag, 2, 1, fp);

        if (gTag == 0xfffe) {
          fread(&eTag, 2, 1, fp);

          if (eTag == 0xe0dd) {
            unsigned long useless;
            fread(&useless, sizeof(unsigned long), 1, fp);
            return true;
          }
          else {
            fseek(fp, -2L, SEEK_CUR);
          }
        }
      }
    }
  }
  else {
    printf("Error!");
    return false;
  }

  return true;
}

static unsigned char* convertTo8Bit_dynamic_linear(unsigned char* pData, unsigned long nNumPixels, unsigned short bIsSigned, unsigned short nHighBit,
    float fRescaleSlope, float fRescaleIntercept,
    float fWindowCenter, float fWindowWidth)
{
  unsigned char* pNewData = 0;
  unsigned long nCount;
  unsigned short* pp;
  float fSlope_3;
  float fValue_3;
  int nMin, nMax;
  unsigned char* np = NULL;
  short* ps;

  // 1. Clip the high bits.
  if (nHighBit < 15) {
    unsigned short nMask;
    unsigned short nSignBit;

    pp = (unsigned short*)pData;
    nCount = nNumPixels;

    if (bIsSigned == 0) { // Unsigned integer
      nMask = 0xffff << (nHighBit + 1);

      while (nCount-- > 0) {
        *(pp ++) &= ~nMask;
      }
    }
    else {
      // 2's complement representation
      nSignBit = 1 << nHighBit;
      nMask = 0xffff << (nHighBit + 1);

      while (nCount -- > 0) {
        if ((*pp & nSignBit) != 0) {
          *(pp ++) |= nMask;
        }
        else {
          *(pp ++) &= ~nMask;
        }
      }
    }
  }

  // 2. Rescale if needed (especially for CT)
  if ((fRescaleSlope != 1.0f) || (fRescaleIntercept != 0.0f)) {
    float fValue;
    short* ps = (short*)pData;
    nCount = nNumPixels;

    while (nCount-- > 0) {
      fValue = (*ps) * fRescaleSlope + fRescaleIntercept;
      *ps = (short)fValue;
      ps++;
    }
  }

  // 3. Window-level or rescale to 8-bit
  // We will map the whole dynamic range.
  MYREALLOC(np, nNumPixels);

  pNewData = np;

  // First compute the min and max.
  nCount = nNumPixels;
  ps = (short*)pData;
  nMin = nMax = *ps;

  while (nCount-- > 0) {
    if (*ps < nMin) {
      nMin = *ps;
    }

    if (*ps > nMax) {
      nMax = *ps;
    }

    ps ++;
  }

  // Calculate the scaling factor.
  if (nMax != nMin) {
    fSlope_3 = 255.0f / (nMax - nMin);
  }
  else {
    fSlope_3 = 1.0f;
  }

  nCount = nNumPixels;
  ps = (short*)pData;

  while (nCount-- > 0) {
    fValue_3 = ((*ps ++) - nMin) * fSlope_3;

    if (fValue_3 < 0) {
      fValue_3 = 0;
    }
    else if (fValue_3 > 255) {
      fValue_3 = 255;
    }

    *np ++ = (unsigned char) fValue_3;
  }

  return pNewData;
}


static unsigned char* convertTo8Bit_direct(unsigned char* pData, unsigned long nNumPixels, unsigned short bIsSigned, unsigned short nHighBit,
    float fRescaleSlope, float fRescaleIntercept,
    float fWindowCenter, float fWindowWidth)
{
  unsigned char* pNewData = 0;
  unsigned long nCount;
  unsigned short* pp;
  float fSlope_3;
  float fValue_3;
  int nMin, nMax;
  unsigned char* np = NULL;
  short* ps;

  // 1. Clip the high bits.
  if (nHighBit < 15) {
    unsigned short nMask;
    unsigned short nSignBit;

    pp = (unsigned short*)pData;
    nCount = nNumPixels;

    if (bIsSigned == 0) { // Unsigned integer
      nMask = 0xffff << (nHighBit + 1);

      while (nCount-- > 0) {
        *(pp ++) &= ~nMask;
      }
    }
    else {
      // 2's complement representation
      nSignBit = 1 << nHighBit;
      nMask = 0xffff << (nHighBit + 1);

      while (nCount -- > 0) {
        if ((*pp & nSignBit) != 0) {
          *(pp ++) |= nMask;
        }
        else {
          *(pp ++) &= ~nMask;
        }
      }
    }
  }

  // 2. Rescale if needed (especially for CT)
  // 3. Window-level or rescale to 8-bit
  // We will map the whole dynamic range.
  MYREALLOC(np, nNumPixels);

  pNewData = np;

  // First compute the min and max.
  nCount = nNumPixels;
  ps = (short*)pData;
  nMin = nMax = *ps;

  while (nCount-- > 0) {
    if (*ps < nMin) {
      nMin = *ps;
    }

    if (*ps > nMax) {
      nMax = *ps;
    }

    ps ++;
  }

  // Calculate the scaling factor.
  if (nMax != nMin) {
    fSlope_3 = 255.0f / (nMax - nMin);
  }
  else {
    fSlope_3 = 1.0f;
  }

  nCount = nNumPixels;
  ps = (short*)pData;

  while (nCount-- > 0) {
    fValue_3 = ((*ps ++) - nMin) * fSlope_3;

    if (fValue_3 < 0) {
      fValue_3 = 0;
    }
    else if (fValue_3 > 255) {
      fValue_3 = 255;
    }

    *np ++ = (unsigned char) fValue_3;
  }

  return pNewData;
}

static unsigned char* convertTo8Bit_bone(unsigned char* pData, unsigned long nNumPixels, unsigned short bIsSigned, unsigned short nHighBit,
    float fRescaleSlope, float fRescaleIntercept,
    float fWindowCenter, float fWindowWidth)
{
  unsigned char* pNewData = 0;
  unsigned long nCount;
  unsigned short* pp;
  float fSlope;
  float fShift;
  float fValue;
  unsigned char* np;
  short* ps;
  float f_tmp;

  fWindowCenter = 800 + 1000;
  fWindowWidth = 2000;

  // 1. Clip the high bits.
  if (nHighBit < 15) {
    unsigned short nMask;
    unsigned short nSignBit;

    pp = (unsigned short*)pData;
    nCount = nNumPixels;

    if (bIsSigned == 0) { // Unsigned integer
      nMask = 0xffff << (nHighBit + 1);

      while (nCount-- > 0) {
        *(pp ++) &= ~nMask;
      }
    }
    else {
      // 2's complement representation
      nSignBit = 1 << nHighBit;
      nMask = 0xffff << (nHighBit + 1);

      while (nCount -- > 0) {
        if ((*pp & nSignBit) != 0) {
          *(pp ++) |= nMask;
        }
        else {
          *(pp ++) &= ~nMask;
        }
      }
    }
  }

  // 3. Window-level or rescale to 8-bit
  // w = 2000, l=800
  MYREALLOC(np, nNumPixels);

  pNewData = np;

  // Since we have window level info, we will only map what are
  // within the Window.

  fShift = fWindowCenter - fWindowWidth / 2.0f;
  fSlope = 255.0f / fWindowWidth;

  nCount = nNumPixels;
  ps = (short*)pData;
  f_tmp = *ps;

  while (nCount-- > 0) {
    fValue = ((*ps ++) - fShift) * fSlope;

    if (fValue < 0) {
      fValue = 0;
    }
    else if (fValue > 255) {
      fValue = 255;
    }

    *np ++ = (unsigned char) fValue;
  }


  return pNewData;
}

#if 0
typedef enum {
  // Warning: Do not write if ( vr & VR::INVALID ) but if ( vr == VR::INVALID )
  VR_INVALID = 0, // For Item/(Seq) Item Delimitation Item
  VR_AE = 1,
  VR_AS = 2,
  VR_AT = 4,
  VR_CS = 8,
  VR_DA = 16,
  VR_DS = 32,
  VR_DT = 64,
  VR_FD = 128,
  VR_FL = 256,
  VR_IS = 512,
  VR_LO = 1024,
  VR_LT = 2048,
  VR_OB = 4096,
  VR_OD = 134217728, // 2^27
  VR_OF = 8192,
  VR_OW = 16384,
  VR_PN = 32768,
  VR_SH = 65536,
  VR_SL = 131072,
  VR_SQ = 262144,
  VR_SS = 524288,
  VR_ST = 1048576,
  VR_TM = 2097152,
  VR_UI = 4194304,
  VR_UL = 8388608,
  VR_UN = 16777216,
  VR_US = 33554432,
  VR_UT = 67108864,
  VR_OB_OW = VR_OB | VR_OW,
  VR_US_SS = VR_US | VR_SS,
  VR_US_SS_OW = VR_US | VR_SS | VR_OW,
  // The following do not have a VRString equivalent (ie cannot be found in PS 3.6)
  VR_VL16 = VR_AE | VR_AS | VR_AT | VR_CS | VR_DA | VR_DS | VR_DT | VR_FD | VR_FL | VR_IS | VR_LO | VR_LT | VR_PN | VR_SH | VR_SL | VR_SS | VR_ST | VR_TM | VR_UI | VR_UL | VR_US, // if( VR & VL16 ) => VR has its VL coded over 16bits
  VR_VL32 = VR_OB | VR_OW | VR_OD | VR_OF | VR_SQ | VR_UN | VR_UT, // if( VR & VL32 ) => VR has its VL coded over 32bits
  VR_VRASCII = VR_AE | VR_AS | VR_CS | VR_DA | VR_DS | VR_DT | VR_IS | VR_LO | VR_LT | VR_PN | VR_SH | VR_ST | VR_TM | VR_UI | VR_UT,
  VR_VRBINARY = VR_AT | VR_FL | VR_FD | VR_OB | VR_OD | VR_OF | VR_OW | VR_SL | VR_SQ | VR_SS | VR_UL | VR_UN | VR_US, // FIXME: UN ?
  // PS 3.5:
  // Data Elements with a VR of SQ, OD, OF, OW, OB or UN shall always have a Value Multiplicity of one.
  // GDCM is adding a couple more: AS, LT, ST, UT
  VR_VR_VM1 = VR_AS | VR_LT | VR_ST | VR_UT | VR_SQ | VR_OF | VR_OD | VR_OW | VR_OB | VR_UN, // All those VR have a VM1
  VR_VRALL = VR_VRASCII | VR_VRBINARY,
  VR_VR_END = VR_UT + 1 // Invalid VR, need to be max(VRType)+1
} VRType;


typedef enum {
  VM0 = 0, // aka the invalid VM
  VM1 = 1,
  VM2 = 2,
  VM3 = 4,
  VM4 = 8,
  VM5 = 16,
  VM6 = 32,
  VM8 = 64,
  VM9 = 128,
  VM10 = 256,
  VM12 = 512, //1024,
  VM16 = 1024, //2048,
  VM18 = 2048, //4096,
  VM24 = 4096, //8192,
  VM28 = 8192, //16384,
  VM32 = 16384, //32768,
  VM35 = 32768, //65536,
  VM99 = 65536, //131072,
  VM256 = 131072, //262144,
  VM1_2  = VM1 | VM2,
  VM1_3  = VM1 | VM2 | VM3,
  VM1_4  = VM1 | VM2 | VM3 | VM4,
  VM1_5  = VM1 | VM2 | VM3 | VM4 | VM5,
  VM1_8  = VM1 | VM2 | VM3 | VM4 | VM5 | VM6 | VM8,
  // The following need some work:
  VM1_32 = VM1 | VM2 | VM3 | VM4 | VM5 | VM6 | VM8 | VM9 | VM16 | VM24 | VM32,
  VM1_99 = VM1 | VM2 | VM3 | VM4 | VM5 | VM6 | VM8 | VM9 | VM16 | VM24 | VM32 | VM99,
  VM1_n  = VM1 | VM2 | VM3 | VM4 | VM5 | VM6 | VM8 | VM9 | VM16 | VM24 | VM32 | VM99 | VM256,
  VM2_2n =       VM2       | VM4       | VM6 | VM8       | VM16 | VM24 | VM32        | VM256,
  VM2_n  =       VM2 | VM3 | VM4 | VM5 | VM6 | VM8 | VM9 | VM16 | VM24 | VM32 | VM99 | VM256,
  VM3_4  =             VM3 | VM4,
  VM3_3n =             VM3 |             VM6       | VM9        | VM24        | VM99 | VM256,
  VM3_n  =             VM3 | VM4 | VM5 | VM6 | VM8 | VM9 | VM16 | VM24 | VM32 | VM99 | VM256,
  VM4_4n =                   VM4                         | VM16 | VM24 | VM32        | VM256,
  VM6_6n =                               VM6             | VM12 | VM18 | VM24               ,
  VM7_7n,
  VM30_30n,
  VM47_47n,
  VM_END = VM1_n + 1  // Custom tag to count number of entry
} VMType;

typedef struct {
  uint16_t group;
  uint16_t element;
  VRType vr;
  VMType vm;
  const char* name;
  const char* keyword;
  bool ret;
} DICT_ENTRY;

const DICT_ENTRY* find_dict_entry(uint tag)
{
  static const DICT_ENTRY DICOMV3DataDict [] = {
#define TAGDICTSDEF(a, b, c, d, e, f, g)  {a, b, c, d, e, f, g},
#include "Tagdictsdef.txt"
#undef TAGDICTSDEF
  };
  int l = 0;
  int r = countof(DICOMV3DataDict) - 1;

  for (; l < r;) {
    int m = (l + r) >> 1;
    uint tag1 = (DICOMV3DataDict[m].group << 16) | DICOMV3DataDict[m].element;

    if (tag1 < tag) {
      l = m;
      continue;
    }
    else if (tag1 > tag) {
      r = m;
      continue;
    }
    else {
      return DICOMV3DataDict + m;
    }
  }

  return NULL;
}
#endif

typedef enum CONVERT_MODE {
  NONE = 0,
    DIRECT,
    WINDOW,
    DYNAMIC_LINEAR,
    BONE
} CONVERT_MODE;
typedef struct dcmfmt {
  int NumberOfFrames;
  int m_iFrameSize;
  short Cols;
  short Rows;
  short m_sBytesP;
  int m_nBitsAllocated;
  char m_szPhotometric[256];
  uchar* m_pData;
} dcmfmt;

int dcmfmt_free(dcmfmt* s) {
  if (s->m_pData) {
    FREE(s->m_pData);
  }
  return 0;
}

int dcmfmt_read(dcmfmt* s, const char* lpPathName, CONVERT_MODE mode)
{
  FILE* fin = fopen(lpPathName, "rb");
  char dicm[4] = {0};
  unsigned short gTag, eTag, eLength;
  unsigned long fmLength;
  unsigned char* fmBuffer = 0;
  char vr[3];
  char transferSyntax[64];
  unsigned char* p = NULL;


  if (!fin) {
    return 0;
  }

  // part 1. file meta
  fseek(fin, 128, SEEK_SET);
  fread(&dicm, 1, 4, fin);

  if (memcmp("DICM", dicm, 4) != 0) {
    fclose(fin);
    return 0;
  }

  fread(&gTag, 1, 2, fin);
  fread(&eTag, 1, 2, fin);

  // skip VR
  fseek(fin, 2, SEEK_CUR);

  // fill file meta buffer
  fread(&eLength, 1, 2, fin);
  fread(&fmLength, eLength, 1, fin);

  if (!(fmBuffer = (unsigned char*) malloc(sizeof(char) * fmLength))) {
    printf("cannot alloc memory!");
    return 0;
  }

  fmLength = fread(fmBuffer, 1, fmLength, fin);

  // get tansferSyntax
  p = findUIDItem(fmBuffer, fmLength, 0x0002, 0x0010);
  eLength = (unsigned short) * p;
  strncpy(transferSyntax, (char*)(p + 2), eLength);
  transferSyntax[eLength] = 0;

  {
    char* tmp = transferSyntax + eLength - 1;

    while ((*tmp == ' ' || *tmp == '\t') && tmp != transferSyntax) {
      *tmp-- = 0;
    }
  }

  if (strcmp("1.2.840.10008.1.2.1", transferSyntax)) {
    printf("image format is not matched!");
    fclose(fin);
    return 0;
  }

  // free memory
  if (fmBuffer) {
    free(fmBuffer);
  }

  //part 2. get related infomation data
  {
    unsigned short nBitsStored, nHighBit, nSamplesPerPixel = 1;
    float fWindowWidth = 0, fWindowCenter = 0, fRescaleSlope = 1, fRescaleIntercept = 0;
    unsigned short bIsSigned = 0;
    unsigned long nLength, nFrameSize = 0;
    s->m_szPhotometric[0] = 0;
    s->NumberOfFrames = 1;

    while (fread(&gTag, 2, 1, fin) == 1) {
      fread(&eTag, 2, 1, fin);
      fread(&vr, 2, 1, fin);
      vr[2] = 0;

      //image info
      if (gTag == 0x0028 && eTag == 0x0002) {
        // Samples per Pixel
        nSamplesPerPixel = readUS(fin);
        continue;
      }

      if (gTag == 0x0028 && eTag == 0x0004) {
        // Photometric Interpolation
        readString(fin, s->m_szPhotometric);
        continue;
      }

      if (gTag == 0x0028 && eTag == 0x0008) {
        // Number of frames
        s->NumberOfFrames = readIS(fin);
        continue;
      }

      if (gTag == 0x0028 && eTag == 0x0010) {
        // Rows
        s->Rows = readUS(fin);
        continue;
      }

      if (gTag == 0x0028 && eTag == 0x0011) {
        // Columns
        s->Cols = readUS(fin);
        continue;
      }

      if (gTag == 0x0028 && eTag == 0x0100) {
        // Bits allocated
        s->m_nBitsAllocated = readUS(fin);
        continue;
      }

      if (gTag == 0x0028 && eTag == 0x0101) {
        // Bits stored
        nBitsStored = readUS(fin);
        continue;
      }

      if (gTag == 0x0028 && eTag == 0x0102) {
        // High bit
        nHighBit = readUS(fin);
        continue;
      }

      if (gTag == 0x0028 && eTag == 0x0103) {
        // Is signed?
        bIsSigned = readUS(fin);
        continue;
      }

      if (gTag == 0x0028 && eTag == 0x1050) {
        fWindowCenter = readDS(fin);
        continue;
      }

      if (gTag == 0x0028 && eTag == 0x1051) {
        fWindowWidth = readDS(fin);
        continue;
      }

      if (gTag == 0x0028 && eTag == 0x1052) {
        fRescaleIntercept = readDS(fin);
        continue;
      }

      if (gTag == 0x0028 && eTag == 0x1053) {
        fRescaleSlope = readDS(fin);
        continue;
      }

      // part 3. get pixel data
      // image pixels and save()
      if (gTag == 0x7fe0 && eTag == 0x0010) {
        unsigned long pixelLen;
#if 0
        FILE* flog = fopen("log.txt", "wb");
        fprintf(flog, "SamplesPerPixel = %d\n", nSamplesPerPixel);
        fprintf(flog, "Photometric = %s\n", s->m_szPhotometric);
        fprintf(flog, "NumFrames = %d\n", s->NumberOfFrames);
        fprintf(flog, "Rows = %d\n", s->Rows);
        fprintf(flog, "Cols = %d\n", s->Cols);
        fprintf(flog, "BitsAllocated = %d\n", s->m_nBitsAllocated);
        fprintf(flog, "BitsStored = %d\n", nBitsStored);
        fprintf(flog, "HighBit = %d\n", nHighBit);
        fprintf(flog, "IsSigned = %d\n", bIsSigned);
        fprintf(flog, "WindowCenter = %f\n", fWindowCenter);
        fprintf(flog, "WindowWidth = %f\n", fWindowWidth);
        fprintf(flog, "RescaleIntercept = %f\n", fRescaleIntercept);
        fprintf(flog, "RescaleSlope = %f\n", fRescaleSlope);
        fclose(flog);
#endif
        s->m_sBytesP = nSamplesPerPixel * s->m_nBitsAllocated / 8;
        nFrameSize = s->Cols * s->Rows * s->m_sBytesP;
        nLength = s->NumberOfFrames * nFrameSize;

        if (!(!strcmp(vr, "OB") || !strcmp(vr, "OW"))) {
          printf("cannot handle NOT OB or OW");
          return 0;
        }

        fseek(fin, 2, SEEK_CUR); // skip 0x0000
        fread(&pixelLen, sizeof(unsigned long), 1, fin);

        MYREALLOC(s->m_pData, pixelLen);

        if (s->m_pData) {
          if (fread(s->m_pData, 1, pixelLen, fin) != pixelLen) {
            printf("Failed to read all pixel data.");
            fclose(fin);
            return 0;
          }
        }
        else {
          fclose(fin);
          return 0;
        }

        //go out of while(file.Read(&gTag,2)==2)
        break;
      }

      // "OB" "Other Byte String"
      // "OW" "Other Word String"
      // "OF" "Other Float String"
      // "SQ" "Sequence Of Items"
      // "UT" "Unlimited Text"
      // "UN" "Unknow"
      if (!strcmp(vr, "OB") || !strcmp(vr, "OW") || !strcmp(vr, "OF") || !strcmp(vr, "SQ") || !strcmp(vr, "UT") || !strcmp(vr, "UN")) {
        if (!readWItem(fin, vr)) {
          printf("cannot read Wide Item correctly!");
          return 0;
        }
      }
      else {
        readItem(fin);
      }
    }

    // close file
    fclose(fin);

    // part 4  convert pixel to 8 bit in different way  u can add "case" here , if needed.
    if (s->m_pData) {
      if (s->m_nBitsAllocated > 8) {
        unsigned char* pNewData = NULL;

        switch (mode) {
        case DIRECT:
          // directly convert
          pNewData = convertTo8Bit_direct(s->m_pData, nLength / 2, bIsSigned, nHighBit,
              fRescaleSlope, fRescaleIntercept, fWindowCenter, fWindowWidth);
          break;

        case WINDOW:
          // convert according to window center and window width
          // or map whole dynamic range without window center and width
          pNewData = convertTo8Bit_window(s->m_pData, nLength / 2, bIsSigned, nHighBit,
              fRescaleSlope, fRescaleIntercept, fWindowCenter, fWindowWidth);
          break;

        case DYNAMIC_LINEAR:
          // convert to map whole dynamic range
          pNewData = convertTo8Bit_dynamic_linear(s->m_pData, nLength / 2, bIsSigned, nHighBit,
              fRescaleSlope, fRescaleIntercept, fWindowCenter, fWindowWidth);

        case BONE:
          pNewData = convertTo8Bit_bone(s->m_pData, nLength / 2, bIsSigned, nHighBit,
              fRescaleSlope, fRescaleIntercept, fWindowCenter, fWindowWidth);
          break;
        }

        if (pNewData) {
          FREE(s->m_pData);
          s->m_pData = pNewData;
          s->m_sBytesP = 1;
          nFrameSize /= 2;
          nLength /= 2;
        }

        s->m_iFrameSize = nFrameSize;
      }
    } // end of "if(s->m_pData)"

    // part 6.  prepare for displaying the image only for 8bit gray scales image
    if (strncmp(s->m_szPhotometric, "RGB", 3) && strncmp(s->m_szPhotometric, "YBR", 3)) {
    }
  }
  return 0;
}
