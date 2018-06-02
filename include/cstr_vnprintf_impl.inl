{
  int len, i, j;
  int flags, bits = 32, field_width;
  uchar flagsmap[128] = {0};
  uchar* s;
  int* ip;
  /* width of output field */
  int precision;        /* min. # of digits for integers; max i2str of chars for from string */
  j = 0;
  //int qualifier=0;        /* 'h', 'l', or 'L' for integer fields */
  flagsmap['-'] = CSTR_LEFT;
  flagsmap['+'] = CSTR_PLUS;
  flagsmap[' '] = CSTR_SPACE;
  //flagsmap['#'] = CSTR_SPECIAL;
  flagsmap['0'] = CSTR_ZEROPAD;
  for (; *fmt ; ++fmt) {
    if (*fmt != '%') {
      buf[j++] = *fmt;
      continue;
    }
    flags = 0;
    while (flagsmap[(uchar)*++fmt]) {
      flags |= flagsmap[(uchar)*fmt];
    }
    /* get field width */
    field_width = -1;
    len = -1;
    if (is_digit(*fmt)) {
      field_width = skip_atoi(&fmt);
    }
    else if ('*' == *fmt) {
      /* it's the next argument */
      field_width = cstr_va_arg(args, int);
      if (field_width < 0) {
        field_width = -field_width;
        flags |= CSTR_LEFT;
      }
      ++fmt;
    }
    else if ('#' == *fmt) {
      len = cstr_va_arg(args, int);
      ++fmt;
    }
    /* get the precision */
    precision = -1;
    if (*fmt == '.') {
      ++fmt;
      if (is_digit(*fmt)) {
        precision = skip_atoi(&fmt);
      }
      else if (*fmt == '*') {
        /* it's the next argument */
        precision = cstr_va_arg(args, int);
      }
      if (precision < 0) {
        precision = 0;
      }
    }
    // %I32d
    bits = 0;
    if ('I' == *fmt || 'J' == *fmt) {
      //is_complex1 = 'J' == *fmt;
      ++fmt;
      if ('6' == fmt[0] && '4' == fmt[1]) {
        bits = 64;
        fmt += 2;
      }
      else if ('3' == fmt[0] && '2' == fmt[1]) {
        bits = 32;
        fmt += 2;
      }
      else if ('1' == fmt[0] && '6' == fmt[1]) {
        bits = 16;
        fmt += 2;
      }
      else if ('8' == fmt[0]) {
        bits = 8;
        fmt += 1;
      }
      else {
        ASSERT(0 && "bad bits");
      }
    }
    if (!isarr) {
      bits = MAX(bits, 32);
    }
    /* get the conversion qualifier */
    //qualifier = -1;
    if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
      //qualifier = *fmt;
      ++fmt;
    }
    if (NULL == buf) {
      switch (*fmt) {
      case 'c':
        j += field_width + 1;
        break;
      case 's':
        s = cstr_va_arg(args, uchar*);
        len = len < 0 ? cstr_len(s) : len;
        j += MAX(len, field_width);
        break;
      case 'o':
      case 'p':
      case 'x':
      case 'X':
      case 'd':
      case 'i':
      case 'u':
        j += MAX(10, field_width);
        break;
      case ('e'):
      case ('E'):
      case ('G'):
      case ('f'):
      case ('g'):
        j += MAX(32, field_width);
        break;
      default:
        break;
      }
      continue;
    }
    switch (*fmt) {
    case 'c':
      if (!(flags & CSTR_LEFT)) {
        while (--field_width > 0) {
          buf[j++] = ' ';
        }
      }
      buf[j++] = (unsigned char) cstr_va_arg(args, int);
      while (--field_width > 0) {
        buf[j++] = ' ';
      }
      break;
    case 's':
      s = cstr_va_arg(args, uchar*);
      len = len < 0 ? cstr_len(s) : len;
      if (precision < 0) {
        precision = len;
      }
      else if (len > precision) {
        len = precision;
      }
      len = MIN(len, buflen - 1);
      field_width = MIN(field_width, buflen - 1);
      if (!(flags & CSTR_LEFT)) {
        while (len < field_width--) {
          buf[j++] = ' ';
        }
      }
      for (i = 0; i < len; ++i) {
        buf[j++] = *s++;
      }
      while (len < field_width--) {
        buf[j++] = ' ';
      }
      break;
      //#define va_argt(ap,t)    ( *(t *)((ap += sizeof(t)) - sizeof(t)) )
    case 'd':
    case 'i':
      {
        flags |= CSTR_SIGN;
        TGETUINT(10);
      }
      break;
    case 'o':
      TGETUINT(8);
      break;
    case 'u':
      TGETUINT(10);
      break;
    case 'x':
      flags |= CSTR_SMALL;
    case 'X':
      TGETUINT(16);
      break;
    case 'p':
      if (field_width == -1) {
        field_width = 8;
        flags |= CSTR_ZEROPAD;
      }
      j += i2str(buf + j, buflen - j, (int64)cstr_va_arg(args, void*), 16, field_width, precision, flags);
      break;
    case 'n':
      ip = cstr_va_arg(args, int*);
      *ip = j;
      break;
    case ('e'):
      flags |= CSTR_SMALL;
    case ('E'):
      flags |= CSTR_CAPEXP;
    case ('G'):
    case ('f'):
    case ('g'):
      if (precision < 0) {
        precision = 5;
      }
      if (32 == bits && isarr) {
        float d = (float)(va_argt_float(args));
        j += f2str(buf + j, buflen - j, d, 10, field_width, precision, flags);
      }
      else {
        double d = cstr_va_arg(args, double);
        j += d2str(buf + j, buflen - j, d, 10, field_width, precision, flags);
      }
      break;
    case ('z'): {
      xtoa_t xtoa = cstr_va_arg(args, xtoa_t);
      void* xptr = cstr_va_arg(args, void*);
      j += xtoa(buf + j, buflen - j, xptr);
    }
    break;
    default:
      if (*fmt != '%') {
        buf[j++] = '%';
      }
      if (*fmt) {
        buf[j++] = *fmt;
      }
      else {
        --fmt;   //
      }
      break;
    }
  }
  buf[j] = '\0';
}
#undef va_argt
#undef va_argt1
#undef TGETUINT
#undef cstr_va_arg

