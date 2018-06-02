{
int flag = (!!istrans)*4+(!!ismirrx)*2+(!!ismirry)*1;
switch(cn) {
case 1: {
switch(flag) {
case 0: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + j * dststep;
    for (i=0; i<w-3; i+=4) {
      dst0[((i+0))] = src0[(i+0)];
      dst0[((i+1))] = src0[(i+1)];
      dst0[((i+2))] = src0[(i+2)];
      dst0[((i+3))] = src0[(i+3)];
    }
  }
} break;
case 1: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + (h-1-j) * dststep;
    for (i=0; i<w-3; i+=4) {
      dst0[((i+0))] = src0[(i+0)];
      dst0[((i+1))] = src0[(i+1)];
      dst0[((i+2))] = src0[(i+2)];
      dst0[((i+3))] = src0[(i+3)];
    }
  }
} break;
case 2: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + j * dststep;
    for (i=0; i<w-3; i+=4) {
      dst0[(w-1-(i+0))] = src0[(i+0)];
      dst0[(w-1-(i+1))] = src0[(i+1)];
      dst0[(w-1-(i+2))] = src0[(i+2)];
      dst0[(w-1-(i+3))] = src0[(i+3)];
    }
  }
} break;
case 3: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + (h-1-j) * dststep;
    for (i=0; i<w-3; i+=4) {
      dst0[(w-1-(i+0))] = src0[(i+0)];
      dst0[(w-1-(i+1))] = src0[(i+1)];
      dst0[(w-1-(i+2))] = src0[(i+2)];
      dst0[(w-1-(i+3))] = src0[(i+3)];
    }
  }
} break;
case 4: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + j * dstcn;
    for (i=0; i<w-3; i+=4) {
      dst0[((i+0)) * dststep] = src0[(i+0)];
      dst0[((i+1)) * dststep] = src0[(i+1)];
      dst0[((i+2)) * dststep] = src0[(i+2)];
      dst0[((i+3)) * dststep] = src0[(i+3)];
    }
  }
} break;
case 5: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + (h-1-j) * dstcn;
    for (i=0; i<w-3; i+=4) {
      dst0[((i+0)) * dststep] = src0[(i+0)];
      dst0[((i+1)) * dststep] = src0[(i+1)];
      dst0[((i+2)) * dststep] = src0[(i+2)];
      dst0[((i+3)) * dststep] = src0[(i+3)];
    }
  }
} break;
case 6: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + j * dstcn;
    for (i=0; i<w-3; i+=4) {
      dst0[(w-1-(i+0)) * dststep] = src0[(i+0)];
      dst0[(w-1-(i+1)) * dststep] = src0[(i+1)];
      dst0[(w-1-(i+2)) * dststep] = src0[(i+2)];
      dst0[(w-1-(i+3)) * dststep] = src0[(i+3)];
    }
  }
} break;
case 7: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + (h-1-j) * dstcn;
    for (i=0; i<w-3; i+=4) {
      dst0[(w-1-(i+0)) * dststep] = src0[(i+0)];
      dst0[(w-1-(i+1)) * dststep] = src0[(i+1)];
      dst0[(w-1-(i+2)) * dststep] = src0[(i+2)];
      dst0[(w-1-(i+3)) * dststep] = src0[(i+3)];
    }
  }
} break;
}
}break;
case 2: {
switch(flag) {
case 0: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + j * dststep;
    for (i=0; i<w-3; i+=4) {
      dst0[((i+0)) * 2 + 0] = src0[(i+0) * 2+0];
      dst0[((i+0)) * 2 + 1] = src0[(i+0) * 2+1];
      dst0[((i+1)) * 2 + 0] = src0[(i+1) * 2+0];
      dst0[((i+1)) * 2 + 1] = src0[(i+1) * 2+1];
      dst0[((i+2)) * 2 + 0] = src0[(i+2) * 2+0];
      dst0[((i+2)) * 2 + 1] = src0[(i+2) * 2+1];
      dst0[((i+3)) * 2 + 0] = src0[(i+3) * 2+0];
      dst0[((i+3)) * 2 + 1] = src0[(i+3) * 2+1];
    }
  }
} break;
case 1: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + (h-1-j) * dststep;
    for (i=0; i<w-3; i+=4) {
      dst0[((i+0)) * 2 + 0] = src0[(i+0) * 2+0];
      dst0[((i+0)) * 2 + 1] = src0[(i+0) * 2+1];
      dst0[((i+1)) * 2 + 0] = src0[(i+1) * 2+0];
      dst0[((i+1)) * 2 + 1] = src0[(i+1) * 2+1];
      dst0[((i+2)) * 2 + 0] = src0[(i+2) * 2+0];
      dst0[((i+2)) * 2 + 1] = src0[(i+2) * 2+1];
      dst0[((i+3)) * 2 + 0] = src0[(i+3) * 2+0];
      dst0[((i+3)) * 2 + 1] = src0[(i+3) * 2+1];
    }
  }
} break;
case 2: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + j * dststep;
    for (i=0; i<w-3; i+=4) {
      dst0[(w-1-(i+0)) * 2 + 0] = src0[(i+0) * 2+0];
      dst0[(w-1-(i+0)) * 2 + 1] = src0[(i+0) * 2+1];
      dst0[(w-1-(i+1)) * 2 + 0] = src0[(i+1) * 2+0];
      dst0[(w-1-(i+1)) * 2 + 1] = src0[(i+1) * 2+1];
      dst0[(w-1-(i+2)) * 2 + 0] = src0[(i+2) * 2+0];
      dst0[(w-1-(i+2)) * 2 + 1] = src0[(i+2) * 2+1];
      dst0[(w-1-(i+3)) * 2 + 0] = src0[(i+3) * 2+0];
      dst0[(w-1-(i+3)) * 2 + 1] = src0[(i+3) * 2+1];
    }
  }
} break;
case 3: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + (h-1-j) * dststep;
    for (i=0; i<w-3; i+=4) {
      dst0[(w-1-(i+0)) * 2 + 0] = src0[(i+0) * 2+0];
      dst0[(w-1-(i+0)) * 2 + 1] = src0[(i+0) * 2+1];
      dst0[(w-1-(i+1)) * 2 + 0] = src0[(i+1) * 2+0];
      dst0[(w-1-(i+1)) * 2 + 1] = src0[(i+1) * 2+1];
      dst0[(w-1-(i+2)) * 2 + 0] = src0[(i+2) * 2+0];
      dst0[(w-1-(i+2)) * 2 + 1] = src0[(i+2) * 2+1];
      dst0[(w-1-(i+3)) * 2 + 0] = src0[(i+3) * 2+0];
      dst0[(w-1-(i+3)) * 2 + 1] = src0[(i+3) * 2+1];
    }
  }
} break;
case 4: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + j * dstcn;
    for (i=0; i<w-3; i+=4) {
      dst0[((i+0)) * dststep + 0] = src0[(i+0) * 2+0];
      dst0[((i+0)) * dststep + 1] = src0[(i+0) * 2+1];
      dst0[((i+1)) * dststep + 0] = src0[(i+1) * 2+0];
      dst0[((i+1)) * dststep + 1] = src0[(i+1) * 2+1];
      dst0[((i+2)) * dststep + 0] = src0[(i+2) * 2+0];
      dst0[((i+2)) * dststep + 1] = src0[(i+2) * 2+1];
      dst0[((i+3)) * dststep + 0] = src0[(i+3) * 2+0];
      dst0[((i+3)) * dststep + 1] = src0[(i+3) * 2+1];
    }
  }
} break;
case 5: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + (h-1-j) * dstcn;
    for (i=0; i<w-3; i+=4) {
      dst0[((i+0)) * dststep + 0] = src0[(i+0) * 2+0];
      dst0[((i+0)) * dststep + 1] = src0[(i+0) * 2+1];
      dst0[((i+1)) * dststep + 0] = src0[(i+1) * 2+0];
      dst0[((i+1)) * dststep + 1] = src0[(i+1) * 2+1];
      dst0[((i+2)) * dststep + 0] = src0[(i+2) * 2+0];
      dst0[((i+2)) * dststep + 1] = src0[(i+2) * 2+1];
      dst0[((i+3)) * dststep + 0] = src0[(i+3) * 2+0];
      dst0[((i+3)) * dststep + 1] = src0[(i+3) * 2+1];
    }
  }
} break;
case 6: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + j * dstcn;
    for (i=0; i<w-3; i+=4) {
      dst0[(w-1-(i+0)) * dststep + 0] = src0[(i+0) * 2+0];
      dst0[(w-1-(i+0)) * dststep + 1] = src0[(i+0) * 2+1];
      dst0[(w-1-(i+1)) * dststep + 0] = src0[(i+1) * 2+0];
      dst0[(w-1-(i+1)) * dststep + 1] = src0[(i+1) * 2+1];
      dst0[(w-1-(i+2)) * dststep + 0] = src0[(i+2) * 2+0];
      dst0[(w-1-(i+2)) * dststep + 1] = src0[(i+2) * 2+1];
      dst0[(w-1-(i+3)) * dststep + 0] = src0[(i+3) * 2+0];
      dst0[(w-1-(i+3)) * dststep + 1] = src0[(i+3) * 2+1];
    }
  }
} break;
case 7: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + (h-1-j) * dstcn;
    for (i=0; i<w-3; i+=4) {
      dst0[(w-1-(i+0)) * dststep + 0] = src0[(i+0) * 2+0];
      dst0[(w-1-(i+0)) * dststep + 1] = src0[(i+0) * 2+1];
      dst0[(w-1-(i+1)) * dststep + 0] = src0[(i+1) * 2+0];
      dst0[(w-1-(i+1)) * dststep + 1] = src0[(i+1) * 2+1];
      dst0[(w-1-(i+2)) * dststep + 0] = src0[(i+2) * 2+0];
      dst0[(w-1-(i+2)) * dststep + 1] = src0[(i+2) * 2+1];
      dst0[(w-1-(i+3)) * dststep + 0] = src0[(i+3) * 2+0];
      dst0[(w-1-(i+3)) * dststep + 1] = src0[(i+3) * 2+1];
    }
  }
} break;
}
}break;
case 3: {
switch(flag) {
case 0: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + j * dststep;
    for (i=0; i<w-3; i+=4) {
      dst0[((i+0)) * 3 + 0] = src0[(i+0) * 3+0];
      dst0[((i+0)) * 3 + 1] = src0[(i+0) * 3+1];
      dst0[((i+0)) * 3 + 2] = src0[(i+0) * 3+2];
      dst0[((i+1)) * 3 + 0] = src0[(i+1) * 3+0];
      dst0[((i+1)) * 3 + 1] = src0[(i+1) * 3+1];
      dst0[((i+1)) * 3 + 2] = src0[(i+1) * 3+2];
      dst0[((i+2)) * 3 + 0] = src0[(i+2) * 3+0];
      dst0[((i+2)) * 3 + 1] = src0[(i+2) * 3+1];
      dst0[((i+2)) * 3 + 2] = src0[(i+2) * 3+2];
      dst0[((i+3)) * 3 + 0] = src0[(i+3) * 3+0];
      dst0[((i+3)) * 3 + 1] = src0[(i+3) * 3+1];
      dst0[((i+3)) * 3 + 2] = src0[(i+3) * 3+2];
    }
  }
} break;
case 1: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + (h-1-j) * dststep;
    for (i=0; i<w-3; i+=4) {
      dst0[((i+0)) * 3 + 0] = src0[(i+0) * 3+0];
      dst0[((i+0)) * 3 + 1] = src0[(i+0) * 3+1];
      dst0[((i+0)) * 3 + 2] = src0[(i+0) * 3+2];
      dst0[((i+1)) * 3 + 0] = src0[(i+1) * 3+0];
      dst0[((i+1)) * 3 + 1] = src0[(i+1) * 3+1];
      dst0[((i+1)) * 3 + 2] = src0[(i+1) * 3+2];
      dst0[((i+2)) * 3 + 0] = src0[(i+2) * 3+0];
      dst0[((i+2)) * 3 + 1] = src0[(i+2) * 3+1];
      dst0[((i+2)) * 3 + 2] = src0[(i+2) * 3+2];
      dst0[((i+3)) * 3 + 0] = src0[(i+3) * 3+0];
      dst0[((i+3)) * 3 + 1] = src0[(i+3) * 3+1];
      dst0[((i+3)) * 3 + 2] = src0[(i+3) * 3+2];
    }
  }
} break;
case 2: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + j * dststep;
    for (i=0; i<w-3; i+=4) {
      dst0[(w-1-(i+0)) * 3 + 0] = src0[(i+0) * 3+0];
      dst0[(w-1-(i+0)) * 3 + 1] = src0[(i+0) * 3+1];
      dst0[(w-1-(i+0)) * 3 + 2] = src0[(i+0) * 3+2];
      dst0[(w-1-(i+1)) * 3 + 0] = src0[(i+1) * 3+0];
      dst0[(w-1-(i+1)) * 3 + 1] = src0[(i+1) * 3+1];
      dst0[(w-1-(i+1)) * 3 + 2] = src0[(i+1) * 3+2];
      dst0[(w-1-(i+2)) * 3 + 0] = src0[(i+2) * 3+0];
      dst0[(w-1-(i+2)) * 3 + 1] = src0[(i+2) * 3+1];
      dst0[(w-1-(i+2)) * 3 + 2] = src0[(i+2) * 3+2];
      dst0[(w-1-(i+3)) * 3 + 0] = src0[(i+3) * 3+0];
      dst0[(w-1-(i+3)) * 3 + 1] = src0[(i+3) * 3+1];
      dst0[(w-1-(i+3)) * 3 + 2] = src0[(i+3) * 3+2];
    }
  }
} break;
case 3: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + (h-1-j) * dststep;
    for (i=0; i<w-3; i+=4) {
      dst0[(w-1-(i+0)) * 3 + 0] = src0[(i+0) * 3+0];
      dst0[(w-1-(i+0)) * 3 + 1] = src0[(i+0) * 3+1];
      dst0[(w-1-(i+0)) * 3 + 2] = src0[(i+0) * 3+2];
      dst0[(w-1-(i+1)) * 3 + 0] = src0[(i+1) * 3+0];
      dst0[(w-1-(i+1)) * 3 + 1] = src0[(i+1) * 3+1];
      dst0[(w-1-(i+1)) * 3 + 2] = src0[(i+1) * 3+2];
      dst0[(w-1-(i+2)) * 3 + 0] = src0[(i+2) * 3+0];
      dst0[(w-1-(i+2)) * 3 + 1] = src0[(i+2) * 3+1];
      dst0[(w-1-(i+2)) * 3 + 2] = src0[(i+2) * 3+2];
      dst0[(w-1-(i+3)) * 3 + 0] = src0[(i+3) * 3+0];
      dst0[(w-1-(i+3)) * 3 + 1] = src0[(i+3) * 3+1];
      dst0[(w-1-(i+3)) * 3 + 2] = src0[(i+3) * 3+2];
    }
  }
} break;
case 4: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + j * dstcn;
    for (i=0; i<w-3; i+=4) {
      dst0[((i+0)) * dststep + 0] = src0[(i+0) * 3+0];
      dst0[((i+0)) * dststep + 1] = src0[(i+0) * 3+1];
      dst0[((i+0)) * dststep + 2] = src0[(i+0) * 3+2];
      dst0[((i+1)) * dststep + 0] = src0[(i+1) * 3+0];
      dst0[((i+1)) * dststep + 1] = src0[(i+1) * 3+1];
      dst0[((i+1)) * dststep + 2] = src0[(i+1) * 3+2];
      dst0[((i+2)) * dststep + 0] = src0[(i+2) * 3+0];
      dst0[((i+2)) * dststep + 1] = src0[(i+2) * 3+1];
      dst0[((i+2)) * dststep + 2] = src0[(i+2) * 3+2];
      dst0[((i+3)) * dststep + 0] = src0[(i+3) * 3+0];
      dst0[((i+3)) * dststep + 1] = src0[(i+3) * 3+1];
      dst0[((i+3)) * dststep + 2] = src0[(i+3) * 3+2];
    }
  }
} break;
case 5: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + (h-1-j) * dstcn;
    for (i=0; i<w-3; i+=4) {
      dst0[((i+0)) * dststep + 0] = src0[(i+0) * 3+0];
      dst0[((i+0)) * dststep + 1] = src0[(i+0) * 3+1];
      dst0[((i+0)) * dststep + 2] = src0[(i+0) * 3+2];
      dst0[((i+1)) * dststep + 0] = src0[(i+1) * 3+0];
      dst0[((i+1)) * dststep + 1] = src0[(i+1) * 3+1];
      dst0[((i+1)) * dststep + 2] = src0[(i+1) * 3+2];
      dst0[((i+2)) * dststep + 0] = src0[(i+2) * 3+0];
      dst0[((i+2)) * dststep + 1] = src0[(i+2) * 3+1];
      dst0[((i+2)) * dststep + 2] = src0[(i+2) * 3+2];
      dst0[((i+3)) * dststep + 0] = src0[(i+3) * 3+0];
      dst0[((i+3)) * dststep + 1] = src0[(i+3) * 3+1];
      dst0[((i+3)) * dststep + 2] = src0[(i+3) * 3+2];
    }
  }
} break;
case 6: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + j * dstcn;
    for (i=0; i<w-3; i+=4) {
      dst0[(w-1-(i+0)) * dststep + 0] = src0[(i+0) * 3+0];
      dst0[(w-1-(i+0)) * dststep + 1] = src0[(i+0) * 3+1];
      dst0[(w-1-(i+0)) * dststep + 2] = src0[(i+0) * 3+2];
      dst0[(w-1-(i+1)) * dststep + 0] = src0[(i+1) * 3+0];
      dst0[(w-1-(i+1)) * dststep + 1] = src0[(i+1) * 3+1];
      dst0[(w-1-(i+1)) * dststep + 2] = src0[(i+1) * 3+2];
      dst0[(w-1-(i+2)) * dststep + 0] = src0[(i+2) * 3+0];
      dst0[(w-1-(i+2)) * dststep + 1] = src0[(i+2) * 3+1];
      dst0[(w-1-(i+2)) * dststep + 2] = src0[(i+2) * 3+2];
      dst0[(w-1-(i+3)) * dststep + 0] = src0[(i+3) * 3+0];
      dst0[(w-1-(i+3)) * dststep + 1] = src0[(i+3) * 3+1];
      dst0[(w-1-(i+3)) * dststep + 2] = src0[(i+3) * 3+2];
    }
  }
} break;
case 7: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + (h-1-j) * dstcn;
    for (i=0; i<w-3; i+=4) {
      dst0[(w-1-(i+0)) * dststep + 0] = src0[(i+0) * 3+0];
      dst0[(w-1-(i+0)) * dststep + 1] = src0[(i+0) * 3+1];
      dst0[(w-1-(i+0)) * dststep + 2] = src0[(i+0) * 3+2];
      dst0[(w-1-(i+1)) * dststep + 0] = src0[(i+1) * 3+0];
      dst0[(w-1-(i+1)) * dststep + 1] = src0[(i+1) * 3+1];
      dst0[(w-1-(i+1)) * dststep + 2] = src0[(i+1) * 3+2];
      dst0[(w-1-(i+2)) * dststep + 0] = src0[(i+2) * 3+0];
      dst0[(w-1-(i+2)) * dststep + 1] = src0[(i+2) * 3+1];
      dst0[(w-1-(i+2)) * dststep + 2] = src0[(i+2) * 3+2];
      dst0[(w-1-(i+3)) * dststep + 0] = src0[(i+3) * 3+0];
      dst0[(w-1-(i+3)) * dststep + 1] = src0[(i+3) * 3+1];
      dst0[(w-1-(i+3)) * dststep + 2] = src0[(i+3) * 3+2];
    }
  }
} break;
}
}break;
case 4: {
switch(flag) {
case 0: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + j * dststep;
    for (i=0; i<w-3; i+=4) {
      dst0[((i+0)) * 4 + 0] = src0[(i+0) * 4+0];
      dst0[((i+0)) * 4 + 1] = src0[(i+0) * 4+1];
      dst0[((i+0)) * 4 + 2] = src0[(i+0) * 4+2];
      dst0[((i+0)) * 4 + 3] = src0[(i+0) * 4+3];
      dst0[((i+1)) * 4 + 0] = src0[(i+1) * 4+0];
      dst0[((i+1)) * 4 + 1] = src0[(i+1) * 4+1];
      dst0[((i+1)) * 4 + 2] = src0[(i+1) * 4+2];
      dst0[((i+1)) * 4 + 3] = src0[(i+1) * 4+3];
      dst0[((i+2)) * 4 + 0] = src0[(i+2) * 4+0];
      dst0[((i+2)) * 4 + 1] = src0[(i+2) * 4+1];
      dst0[((i+2)) * 4 + 2] = src0[(i+2) * 4+2];
      dst0[((i+2)) * 4 + 3] = src0[(i+2) * 4+3];
      dst0[((i+3)) * 4 + 0] = src0[(i+3) * 4+0];
      dst0[((i+3)) * 4 + 1] = src0[(i+3) * 4+1];
      dst0[((i+3)) * 4 + 2] = src0[(i+3) * 4+2];
      dst0[((i+3)) * 4 + 3] = src0[(i+3) * 4+3];
    }
  }
} break;
case 1: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + (h-1-j) * dststep;
    for (i=0; i<w-3; i+=4) {
      dst0[((i+0)) * 4 + 0] = src0[(i+0) * 4+0];
      dst0[((i+0)) * 4 + 1] = src0[(i+0) * 4+1];
      dst0[((i+0)) * 4 + 2] = src0[(i+0) * 4+2];
      dst0[((i+0)) * 4 + 3] = src0[(i+0) * 4+3];
      dst0[((i+1)) * 4 + 0] = src0[(i+1) * 4+0];
      dst0[((i+1)) * 4 + 1] = src0[(i+1) * 4+1];
      dst0[((i+1)) * 4 + 2] = src0[(i+1) * 4+2];
      dst0[((i+1)) * 4 + 3] = src0[(i+1) * 4+3];
      dst0[((i+2)) * 4 + 0] = src0[(i+2) * 4+0];
      dst0[((i+2)) * 4 + 1] = src0[(i+2) * 4+1];
      dst0[((i+2)) * 4 + 2] = src0[(i+2) * 4+2];
      dst0[((i+2)) * 4 + 3] = src0[(i+2) * 4+3];
      dst0[((i+3)) * 4 + 0] = src0[(i+3) * 4+0];
      dst0[((i+3)) * 4 + 1] = src0[(i+3) * 4+1];
      dst0[((i+3)) * 4 + 2] = src0[(i+3) * 4+2];
      dst0[((i+3)) * 4 + 3] = src0[(i+3) * 4+3];
    }
  }
} break;
case 2: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + j * dststep;
    for (i=0; i<w-3; i+=4) {
      dst0[(w-1-(i+0)) * 4 + 0] = src0[(i+0) * 4+0];
      dst0[(w-1-(i+0)) * 4 + 1] = src0[(i+0) * 4+1];
      dst0[(w-1-(i+0)) * 4 + 2] = src0[(i+0) * 4+2];
      dst0[(w-1-(i+0)) * 4 + 3] = src0[(i+0) * 4+3];
      dst0[(w-1-(i+1)) * 4 + 0] = src0[(i+1) * 4+0];
      dst0[(w-1-(i+1)) * 4 + 1] = src0[(i+1) * 4+1];
      dst0[(w-1-(i+1)) * 4 + 2] = src0[(i+1) * 4+2];
      dst0[(w-1-(i+1)) * 4 + 3] = src0[(i+1) * 4+3];
      dst0[(w-1-(i+2)) * 4 + 0] = src0[(i+2) * 4+0];
      dst0[(w-1-(i+2)) * 4 + 1] = src0[(i+2) * 4+1];
      dst0[(w-1-(i+2)) * 4 + 2] = src0[(i+2) * 4+2];
      dst0[(w-1-(i+2)) * 4 + 3] = src0[(i+2) * 4+3];
      dst0[(w-1-(i+3)) * 4 + 0] = src0[(i+3) * 4+0];
      dst0[(w-1-(i+3)) * 4 + 1] = src0[(i+3) * 4+1];
      dst0[(w-1-(i+3)) * 4 + 2] = src0[(i+3) * 4+2];
      dst0[(w-1-(i+3)) * 4 + 3] = src0[(i+3) * 4+3];
    }
  }
} break;
case 3: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + (h-1-j) * dststep;
    for (i=0; i<w-3; i+=4) {
      dst0[(w-1-(i+0)) * 4 + 0] = src0[(i+0) * 4+0];
      dst0[(w-1-(i+0)) * 4 + 1] = src0[(i+0) * 4+1];
      dst0[(w-1-(i+0)) * 4 + 2] = src0[(i+0) * 4+2];
      dst0[(w-1-(i+0)) * 4 + 3] = src0[(i+0) * 4+3];
      dst0[(w-1-(i+1)) * 4 + 0] = src0[(i+1) * 4+0];
      dst0[(w-1-(i+1)) * 4 + 1] = src0[(i+1) * 4+1];
      dst0[(w-1-(i+1)) * 4 + 2] = src0[(i+1) * 4+2];
      dst0[(w-1-(i+1)) * 4 + 3] = src0[(i+1) * 4+3];
      dst0[(w-1-(i+2)) * 4 + 0] = src0[(i+2) * 4+0];
      dst0[(w-1-(i+2)) * 4 + 1] = src0[(i+2) * 4+1];
      dst0[(w-1-(i+2)) * 4 + 2] = src0[(i+2) * 4+2];
      dst0[(w-1-(i+2)) * 4 + 3] = src0[(i+2) * 4+3];
      dst0[(w-1-(i+3)) * 4 + 0] = src0[(i+3) * 4+0];
      dst0[(w-1-(i+3)) * 4 + 1] = src0[(i+3) * 4+1];
      dst0[(w-1-(i+3)) * 4 + 2] = src0[(i+3) * 4+2];
      dst0[(w-1-(i+3)) * 4 + 3] = src0[(i+3) * 4+3];
    }
  }
} break;
case 4: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + j * dstcn;
    for (i=0; i<w-3; i+=4) {
      dst0[((i+0)) * dststep + 0] = src0[(i+0) * 4+0];
      dst0[((i+0)) * dststep + 1] = src0[(i+0) * 4+1];
      dst0[((i+0)) * dststep + 2] = src0[(i+0) * 4+2];
      dst0[((i+0)) * dststep + 3] = src0[(i+0) * 4+3];
      dst0[((i+1)) * dststep + 0] = src0[(i+1) * 4+0];
      dst0[((i+1)) * dststep + 1] = src0[(i+1) * 4+1];
      dst0[((i+1)) * dststep + 2] = src0[(i+1) * 4+2];
      dst0[((i+1)) * dststep + 3] = src0[(i+1) * 4+3];
      dst0[((i+2)) * dststep + 0] = src0[(i+2) * 4+0];
      dst0[((i+2)) * dststep + 1] = src0[(i+2) * 4+1];
      dst0[((i+2)) * dststep + 2] = src0[(i+2) * 4+2];
      dst0[((i+2)) * dststep + 3] = src0[(i+2) * 4+3];
      dst0[((i+3)) * dststep + 0] = src0[(i+3) * 4+0];
      dst0[((i+3)) * dststep + 1] = src0[(i+3) * 4+1];
      dst0[((i+3)) * dststep + 2] = src0[(i+3) * 4+2];
      dst0[((i+3)) * dststep + 3] = src0[(i+3) * 4+3];
    }
  }
} break;
case 5: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + (h-1-j) * dstcn;
    for (i=0; i<w-3; i+=4) {
      dst0[((i+0)) * dststep + 0] = src0[(i+0) * 4+0];
      dst0[((i+0)) * dststep + 1] = src0[(i+0) * 4+1];
      dst0[((i+0)) * dststep + 2] = src0[(i+0) * 4+2];
      dst0[((i+0)) * dststep + 3] = src0[(i+0) * 4+3];
      dst0[((i+1)) * dststep + 0] = src0[(i+1) * 4+0];
      dst0[((i+1)) * dststep + 1] = src0[(i+1) * 4+1];
      dst0[((i+1)) * dststep + 2] = src0[(i+1) * 4+2];
      dst0[((i+1)) * dststep + 3] = src0[(i+1) * 4+3];
      dst0[((i+2)) * dststep + 0] = src0[(i+2) * 4+0];
      dst0[((i+2)) * dststep + 1] = src0[(i+2) * 4+1];
      dst0[((i+2)) * dststep + 2] = src0[(i+2) * 4+2];
      dst0[((i+2)) * dststep + 3] = src0[(i+2) * 4+3];
      dst0[((i+3)) * dststep + 0] = src0[(i+3) * 4+0];
      dst0[((i+3)) * dststep + 1] = src0[(i+3) * 4+1];
      dst0[((i+3)) * dststep + 2] = src0[(i+3) * 4+2];
      dst0[((i+3)) * dststep + 3] = src0[(i+3) * 4+3];
    }
  }
} break;
case 6: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + j * dstcn;
    for (i=0; i<w-3; i+=4) {
      dst0[(w-1-(i+0)) * dststep + 0] = src0[(i+0) * 4+0];
      dst0[(w-1-(i+0)) * dststep + 1] = src0[(i+0) * 4+1];
      dst0[(w-1-(i+0)) * dststep + 2] = src0[(i+0) * 4+2];
      dst0[(w-1-(i+0)) * dststep + 3] = src0[(i+0) * 4+3];
      dst0[(w-1-(i+1)) * dststep + 0] = src0[(i+1) * 4+0];
      dst0[(w-1-(i+1)) * dststep + 1] = src0[(i+1) * 4+1];
      dst0[(w-1-(i+1)) * dststep + 2] = src0[(i+1) * 4+2];
      dst0[(w-1-(i+1)) * dststep + 3] = src0[(i+1) * 4+3];
      dst0[(w-1-(i+2)) * dststep + 0] = src0[(i+2) * 4+0];
      dst0[(w-1-(i+2)) * dststep + 1] = src0[(i+2) * 4+1];
      dst0[(w-1-(i+2)) * dststep + 2] = src0[(i+2) * 4+2];
      dst0[(w-1-(i+2)) * dststep + 3] = src0[(i+2) * 4+3];
      dst0[(w-1-(i+3)) * dststep + 0] = src0[(i+3) * 4+0];
      dst0[(w-1-(i+3)) * dststep + 1] = src0[(i+3) * 4+1];
      dst0[(w-1-(i+3)) * dststep + 2] = src0[(i+3) * 4+2];
      dst0[(w-1-(i+3)) * dststep + 3] = src0[(i+3) * 4+3];
    }
  }
} break;
case 7: {
  int i, j;
  for (j=0; j<h; ++j) {
    const uchar* src0 = src + j * srcstep;
    uchar* dst0 = dst + (h-1-j) * dstcn;
    for (i=0; i<w-3; i+=4) {
      dst0[(w-1-(i+0)) * dststep + 0] = src0[(i+0) * 4+0];
      dst0[(w-1-(i+0)) * dststep + 1] = src0[(i+0) * 4+1];
      dst0[(w-1-(i+0)) * dststep + 2] = src0[(i+0) * 4+2];
      dst0[(w-1-(i+0)) * dststep + 3] = src0[(i+0) * 4+3];
      dst0[(w-1-(i+1)) * dststep + 0] = src0[(i+1) * 4+0];
      dst0[(w-1-(i+1)) * dststep + 1] = src0[(i+1) * 4+1];
      dst0[(w-1-(i+1)) * dststep + 2] = src0[(i+1) * 4+2];
      dst0[(w-1-(i+1)) * dststep + 3] = src0[(i+1) * 4+3];
      dst0[(w-1-(i+2)) * dststep + 0] = src0[(i+2) * 4+0];
      dst0[(w-1-(i+2)) * dststep + 1] = src0[(i+2) * 4+1];
      dst0[(w-1-(i+2)) * dststep + 2] = src0[(i+2) * 4+2];
      dst0[(w-1-(i+2)) * dststep + 3] = src0[(i+2) * 4+3];
      dst0[(w-1-(i+3)) * dststep + 0] = src0[(i+3) * 4+0];
      dst0[(w-1-(i+3)) * dststep + 1] = src0[(i+3) * 4+1];
      dst0[(w-1-(i+3)) * dststep + 2] = src0[(i+3) * 4+2];
      dst0[(w-1-(i+3)) * dststep + 3] = src0[(i+3) * 4+3];
    }
  }
} break;
}
}break;
}
}

