
#include <stdlib.h>

enum { SURROGATE_LEAD_FIRST = 0xD800 };
enum { SURROGATE_TRAIL_FIRST = 0xDC00 };
enum { SURROGATE_TRAIL_LAST = 0xDFFF };

unsigned int UTF8Length(const wchar_t *uptr, unsigned int tlen) {
  unsigned int len = 0;
	unsigned int i = 0;
	for (i = 0; i < tlen && uptr[i];) {
		unsigned int uch = uptr[i];
		if (uch < 0x80) {
			len++;
		} else if (uch < 0x800) {
			len += 2;
		} else if ((uch >= SURROGATE_LEAD_FIRST) &&
			(uch <= SURROGATE_TRAIL_LAST)) {
			len += 4;
			i++;
		} else {
			len += 3;
		}
		i++;
	}
	return len;
}

void UTF8FromUTF16(const wchar_t *uptr, unsigned int tlen, char *putf, unsigned int len) {
  int k = 0;
  unsigned int i = 0;
	for (i = 0; i < tlen && uptr[i];) {
		unsigned int xch, uch = uptr[i];
		if (uch < 0x80) {
			putf[k++] = (char)(uch);
		} else if (uch < 0x800) {
			putf[k++] = (char)(0xC0 | (uch >> 6));
			putf[k++] = (char)(0x80 | (uch & 0x3f));
		} else if ((uch >= SURROGATE_LEAD_FIRST) &&
			(uch <= SURROGATE_TRAIL_LAST)) {
			// Half a surrogate pair
			i++;
			xch = 0x10000 + ((uch & 0x3ff) << 10) + (uptr[i] & 0x3ff);
			putf[k++] = (char)(0xF0 | (xch >> 18));
			putf[k++] = (char)(0x80 | ((xch >> 12) & 0x3f));
			putf[k++] = (char)(0x80 | ((xch >> 6) & 0x3f));
			putf[k++] = (char)(0x80 | (xch & 0x3f));
		} else {
			putf[k++] = (char)(0xE0 | (uch >> 12));
			putf[k++] = (char)(0x80 | ((uch >> 6) & 0x3f));
			putf[k++] = (char)(0x80 | (uch & 0x3f));
		}
		i++;
	}
	putf[len] = '\0';
}

unsigned int UTF8CharLength(unsigned char ch) {
	if (ch < 0x80) {
		return 1;
	} else if (ch < 0x80 + 0x40 + 0x20) {
		return 2;
	} else if (ch < 0x80 + 0x40 + 0x20 + 0x10) {
		return 3;
	} else {
		return 4;
	}
}

unsigned int UTF16Length(const char *s, unsigned int len) {
	unsigned int ulen = 0;
  unsigned int charLen;
	unsigned int i = 0;
	for (i=0; i<len;) {
		unsigned char ch = (unsigned char)(s[i]);
		if (ch < 0x80) {
			charLen = 1;
		} else if (ch < 0x80 + 0x40 + 0x20) {
			charLen = 2;
		} else if (ch < 0x80 + 0x40 + 0x20 + 0x10) {
			charLen = 3;
		} else {
			charLen = 4;
			ulen++;
		}
		i += charLen;
		ulen++;
	}
	return ulen;
}

unsigned int UTF16FromUTF8(const char *s, unsigned int len, wchar_t *tbuf, unsigned int tlen) {
	unsigned int ui=0;
	const unsigned char *us = (const unsigned char *)(s);
	unsigned int i=0;
	while ((i<len) && (ui<tlen)) {
		unsigned char ch = us[i++];
		if (ch < 0x80) {
			tbuf[ui] = ch;
		} else if (ch < 0x80 + 0x40 + 0x20) {
			tbuf[ui] = (wchar_t)((ch & 0x1F) << 6);
			ch = us[i++];
			tbuf[ui] = (wchar_t)(tbuf[ui] + (ch & 0x7F));
		} else if (ch < 0x80 + 0x40 + 0x20 + 0x10) {
			tbuf[ui] = (wchar_t)((ch & 0xF) << 12);
			ch = us[i++];
			tbuf[ui] = (wchar_t)(tbuf[ui] + ((ch & 0x7F) << 6));
			ch = us[i++];
			tbuf[ui] = (wchar_t)(tbuf[ui] + (ch & 0x7F));
		} else {
			// Outside the BMP so need two surrogates
			int val = (ch & 0x7) << 18;
			ch = us[i++];
			val += (ch & 0x3F) << 12;
			ch = us[i++];
			val += (ch & 0x3F) << 6;
			ch = us[i++];
			val += (ch & 0x3F);
			tbuf[ui] = (wchar_t)(((val - 0x10000) >> 10) + SURROGATE_LEAD_FIRST);
			ui++;
			tbuf[ui] = (wchar_t)((val & 0x3ff) + SURROGATE_TRAIL_FIRST);
		}
		ui++;
	}
	return ui;
}
