
static int struct_get_2(const buf_t* bf, int* poffset, int big_endian, void* out)   
{
  if (*poffset+2 <= bf->len) {
    uchar* p = bf->data + *poffset;
    *poffset += 2;
    *(short*)out = big_endian ? ((int) p[0] << 8) + p[1]
      : ((int)p[1] << 8) + p[0];
    return 1;
  }
  return 0;
}

static int struct_get_4(const buf_t* bf, int* poffset, int big_endian, void* out)  
{
  if (*poffset+4 <= bf->len) {
    uchar* p = bf->data + *poffset;
    *poffset += 4;
    *(int*)out = big_endian ? (((int) p[0] << 24) + ((int)p[1] << 16) + ((int)p[2] << 8) + p[3])
      : (((int) p[3] << 24) + ((int)p[2] << 16) + ((int)p[1] << 8) + p[0]);
    return 1;
  }
  return 0;
}
static int struct_get_8(const buf_t* bf, int* poffset, int big_endian, void* out)  
{
  if (*poffset+8 <= bf->len) {
    uchar* p = bf->data + *poffset;
    *poffset += 8;
    *(int64*)out = big_endian ? (((int64) p[0] << 56) + ((int64) p[1] << 48) + ((int64) p[2] << 40) + ((int64) p[3] << 32) + ((int64) p[4] << 24) + ((int64)p[5] << 16) + ((int64)p[6] << 8) + p[3])
      : (((int64) p[7] << 56) + ((int64) p[6] << 48) + ((int64) p[5] << 40) + ((int64) p[4] << 32) + ((int64) p[3] << 24) + ((int64)p[2] << 16) + ((int64)p[1] << 8) + p[3]);
    return 1;
  }
  return 0;
}
// Format 	C Type 	Python 	Notes
// x 	pad byte 	no value 	 
// c 	char 	bytes of length 1 	 
// b 	signed char 	integer 	(1)
// B 	unsigned char 	integer 	 
// ? 	_Bool 	bool 	(2)
// h 	short 	integer 	 
// H 	unsigned short 	integer 	 
// i 	int 	integer 	 
// I 	unsigned int 	integer 	 
// l 	long 	integer 	 
// L 	unsigned long 	integer 	 
// q 	long long 	integer 	(3)
// Q 	unsigned long long 	integer 	(3)
// f 	float 	float 	 
// d 	double 	float 	 
// s 	char[] 	bytes 	(1)
// p 	char[] 	bytes 	(1)
// P 	void * 	integer
static int struct_unpack_from_v(const buf_t* bf, int* poffset, const char* fmt, va_list ap ) {
  int big_endian = 'b'==ENDIANNESS;
  int ret = 0;
  for (; *fmt ; ++fmt) {
    switch (*fmt) {
    case '<': big_endian = 0; break;
    case '>': big_endian = 1; break;
    case '@':
    case '=': big_endian = 'b'==ENDIANNESS; break;
    case 'h':
    case 'H': ret += struct_get_2(bf, poffset, big_endian, va_arg(ap,short*)); break;
    case 'i':
    case 'I':
    case 'f': ret += struct_get_4(bf, poffset, big_endian, va_arg(ap,int*)); break;
    case 'q':
    case 'Q':
    case 'd': ret += struct_get_8(bf, poffset, big_endian, va_arg(ap,int64*)); break;
    default: break;
    }
  }
  return ret;
}

static int struct_unpack_from(const buf_t* bf, int* poffset, const char* fmt, ...)
{
  int ret=0;
  va_list arglist;
  va_start(arglist, fmt);
  ret = struct_unpack_from_v(bf, poffset, fmt, arglist);
  va_end(arglist);
  return ret;
}
static img_t* read_lable(const char* fn, img_t* im) {
  buf_t bf[1] = {0};
  int ret = 0;
  if (buf_load(fn, bf)) {
    int offset=0, magic=0, num_items=0;
    if (2==struct_unpack_from(bf, &offset, ">II", &magic, &num_items)) {
      if (num_items + offset <= bf->len) {
        cvSetMats(im, 1, 1, CC_8U, 1, num_items, NULL, 0);
        memcpy(im->data, bf->data + offset, num_items);
        ret = 1;
      }
    }
    bffree(bf);
  }
  return ret ? im : NULL;
}
static img_t* read_images(const char* fn, img_t* im) {
  buf_t bf[1] = {0};
  int big_endian = true, ret = 0;
  if (buf_load(fn, bf)) {
    int offset=0, magic=0, num_images=0, rows=0, cols=0;
    if (4==struct_unpack_from(bf, &offset, ">IIII", &magic, &num_images, &rows, &cols)) {
      int n = rows * cols * num_images;
      if (n + offset <= bf->len) {
        cvSetMats(im, rows, cols, CC_8U, 1, num_images, NULL, 0);
        memcpy(im->data, bf->data + offset, n);
        ret = 1;
      }
    }
    bffree(bf);
  }
  return ret ? im : NULL;
}
