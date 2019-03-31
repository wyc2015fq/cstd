
#ifndef __FILEIOEX_H__
#define __FILEIOEX_H__

#include "std/fileio_c.h"
#include "std/str_c.h"
#include "std/iconv_c.h"

static int str_toasni(const str_t* s, str_t* d)
{
	const BYTE* pByte = (BYTE*)s->s;
	uint32_t dwSize = s->l;
	wchar_t* w_str;
	ICONV_CODEPAGE encoding = GetCodePage(pByte, dwSize);
	str_setsize(d, dwSize * 2);
	iconv_c(encoding, ICONV_GB2312, (char*)pByte, dwSize, (char*)d->s, d->l);
	return 0;
}

CC_INLINE int str_load(const char* fname, str_t* s)
{
	int len, readed_len;
	FILE* pf;
	pf = fopen(fname, "rb");
	if (pf) {
		len = fsize(pf);
		str_setsize(s, len);
		readed_len = fread(s->s, 1, len, pf);
		assert(readed_len == len);
		fclose(pf);
		return 1;
	}
	return 0;
}
CC_INLINE int str_loadasni(const char* fn, str_t* s)
{
	if (str_load(fn, s)) {
		str_toasni(s, s);
		return 1;
	}
	return 0;
}
CC_INLINE int str_save(const char* fname, const str_t* s)
{
	int writeed_len;
	FILE* pf;
	pf = fopen(fname, "wb");
	if (pf) {
		writeed_len = (int)fwrite(s->s, 1, s->l, pf);
		printf("%d %d", writeed_len, s->l);
		//ASSERT(writeed_len == s->l);
		fclose(pf);
		return 1;
	}
	return 0;
}

CC_INLINE int vstr_save(const char* fname, vstr_t* sv) {
	str_t s[1] = { 0 };
	vstr_merge(sv, s, "\n");
	str_save(fname, s);
	str_free(s);
	return 0;
}
CC_INLINE int vstr_load(const char* fname, vstr_t* sv)
{
	str_t s[1] = { 0 };
	if (str_load(fname, s)) {
		vstr_split_str(sv, s->s, 0, s->l, "\n", "\r\n", 1);
		str_free(s);
		return 1;
	}
	return 0;
}


#endif // __FILEIOEX_H__
