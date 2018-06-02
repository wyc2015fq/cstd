#ifndef _TIFFSTRUCT_
#define _TIFFSTRUCT_

typedef struct tagIMAGEFILEHEADER
{
	WORD byteOrder;
	WORD version;
	DWORD offsetToIFD;
}IFH;

typedef struct tagDIRECTORYENTRY
{
	WORD tag;
	WORD type;
	DWORD length;
	DWORD valueOffset;
}DE;

#endif
