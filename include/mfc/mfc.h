

#ifndef __MFC_H__
#define __MFC_H__


#include "typedef.h"
#include "gdi/gdi.h"


struct CString;

struct DefStruct {
	virtual DefStruct* def(const char* name, CString* string) = 0;
	virtual DefStruct* def(const char* name, char* i8) = 0;
	virtual DefStruct* def(const char* name, unsigned char* u8) = 0;
	virtual DefStruct* def(const char* name, short* i16) = 0;
	virtual DefStruct* def(const char* name, unsigned short* u16) = 0;
	virtual DefStruct* def(const char* name, int* i32) = 0;
	virtual DefStruct* def(const char* name, unsigned int* u32) = 0;
	virtual DefStruct* def(const char* name, long* l) = 0;
	virtual DefStruct* def(const char* name, unsigned long* ul) = 0;
	virtual DefStruct* def(const char* name, int64_t* i64) = 0;
	virtual DefStruct* def(const char* name, uint64_t* i64) = 0;
	virtual DefStruct* def(const char* name, float* f) = 0;
	virtual DefStruct* def(const char* name, double* d) = 0;
	virtual DefStruct* sub(const char* name) = 0;
	template <typename T>	DefStruct* def(T* x) {
		return x->def(this);
	}
	template <typename T>	DefStruct* subdef(const char* name, T* x) {
		return x->def(sub(name));
	}
};



#endif // __MFC_H__
