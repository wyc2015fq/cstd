
#ifndef _BLURDETECT_INL_
#define _BLURDETECT_INL_

#include "impl/blurdetect.inl"

int blurdetect(const img_t* im) {
  int i, j, cnt=0;
  int flag = 1;
  int isum=0, imin=100000, imax=-100000;
  ASSERT(im->c==1);
  if (0==flag) {
    // 运动模糊
    //-1    -1    -1    -1    -1    -1    -1    -1
    // 1     1     1     1     1     1     1     1
    for (i=0; i<im->h-2; ++i) {
      const uchar* src0 = im->tt.data + i*im->s;
      const uchar* src1 = im->tt.data + (i+1)*im->s;
      for (j=0; j<im->w; ++j) {
        int isub = src1[j] - src0[j];
        isum += isub;
        imin = MIN(imin, isub);
        imax = MIN(imax, isub);
      }
    }
  } else {
    // 聚焦模糊
    //     -1    -1    -1    -1    -1
    //     -1     2     2     2    -1
    //     -1     2     0     2    -1
    //     -1     2     2     2    -1
    //     -1    -1    -1    -1    -1
    int aaa[] = {
      -1,-1,-1,-1,-1,
        -1,2,2,2,-1,
        -1,2,0,2,-1,
        -1,2,2,2,-1,
-1,-1,-1,-1,-1
    };
#if 0
    int stp[25]={0};
    for (i=0; i<5; ++i) {
      for (j=0; j<5; ++j) {
        stp[i*5+j] = i*im->s+j;
      }
    }
    for (i=0; i<im->h-5; ++i) {
      const uchar* src0 = im->tt.data + i*im->s;
      for (j=0; j<im->w-5; ++j, ++src0) {
        int isub = BLURDETECT(src0, stp);
        isum += isub;
        imin = MIN(imin, isub);
        imax = MAX(imax, isub);
        ++cnt;
      }
    }
#else
    for (i=0; i<im->h-5; ++i) {
      const uchar* src0 = im->tt.data + i*im->s;
      for (j=0; j<im->w-5; ++j, ++src0) {
        int x, y;
        int isub=0;
        for (y=0; y<5; ++y) {
          for (x=0; x<5; ++x) {
            isub += src0[y*im->s+x]*aaa[y*5+x];
          }
        }
        if (isub>0) {
          isum += isub;
          imin = MIN(imin, isub);
          imax = MAX(imax, isub);
          ++cnt;
          imin = 0;
        }
      }
    }
#endif
  }
  if (imax>imin) {
    isum = isum*256/cnt;
    //isum = (isum-imin)*255/(imax-imin);
  }
  return isum;
}


double VideoBlurDetect(const img_t* im)
{
	double result;
	int i, j, mul, t;
  int width = im->w, height = im->h;
  ushort* sobelTable = MALLOC(ushort, width*height);
	uchar* udata = im->tt.data;
  
  int totLen = 0;
  int totCount = 1;
  
  uchar suddenThre = 50;
	uchar sameThre = 3;

	memset(sobelTable, 0, width*height*sizeof(ushort));

  for(i = 1, mul = i*width; i < height - 1; i++, mul += width) {
    for(j = 1; j < width - 1; j++) {
			sobelTable[mul+j]=(ushort)abs(udata[mul+j-width-1] + 2*udata[mul+j-1] + udata[mul+j-1+width] - udata[mul+j+1-width] - 2*udata[mul+j+1] - udata[mul+j+width+1]);
    }
  }

  for(i = 1, mul = i*width; i < height - 1; i++, mul += width) {
    for(j = 1; j < width - 1; j++) {
      if(sobelTable[mul+j] < 50 || sobelTable[mul+j] <= sobelTable[mul+j-1] || sobelTable[mul+j] <= sobelTable[mul+j+1]) {
        sobelTable[mul+j] = 0;
      }
    }
  }

	for(i = 1, mul = i*width; i < height - 1; i++, mul += width)
	{
		for(j = 1; j < width - 1; j++)
		{
			if(sobelTable[mul+j])
      {
					int s, tmp;
				int   count = 0;
				uchar tmpThre = 5;
				uchar max = udata[mul+j] > udata[mul+j-1] ? 0 : 1;

				for(t = j; t > 0; t--)
        {
					count++;
					if(abs(udata[mul+t] - udata[mul+t-1]) > suddenThre)
						break;

					if(max && udata[mul+t] > udata[mul+t-1])
						break;

					if(!max && udata[mul+t] < udata[mul+t-1])
						break;

          tmp = 0;
					for(s = t; s > 0; s--)
					{
						if(abs(udata[mul+t] - udata[mul+s]) < sameThre)
						{
							tmp++;
							if(tmp > tmpThre) break;
						}
						else break;
					}

					if(tmp > tmpThre) break;
				}

				max = udata[mul+j] > udata[mul+j+1] ? 0 : 1;

				for(t = j; t < width; t++)
				{
					count++;
					if(abs(udata[mul+t] - udata[mul+t+1]) > suddenThre)
						break;

					if(max && udata[mul+t] > udata[mul+t+1])
						break;

					if(!max && udata[mul+t] < udata[mul+t+1])
						break;

					tmp = 0;
					for(s = t; s < width; s++)
					{
						if(abs(udata[mul+t] - udata[mul+s]) < sameThre)
						{
							tmp++;
							if(tmp > tmpThre) break;
						}
						else break;
					}

					if(tmp > tmpThre) break;
				}
				count--;

				totCount++;
				totLen += count;
			}
		}
	}

  result = (double)totLen/totCount;
	free(sobelTable);
	return result;
}

#endif // _BLURDETECT_INL_
