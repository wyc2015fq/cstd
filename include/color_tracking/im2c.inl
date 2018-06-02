
// w2c 2^15 * 10
int im2c(const img_t* im, const double* w2c, int color, double* out, int step) {
  
  // input im should be DOUBLE !
  //w2c��2^32 x 11,ÿ��RGBֵ����Ϊ��ĳ��color name�Ŀ����ԣ����п����ԣ���ÿ�У���Ϊ1��
  // Ϊ�˽���cos������ʱ�����ٻ��Σ���w2c��׼����2^32 x 10.�������ס�23���õ���ӳ��������2^32 x 10��
  // color=0 is color names out
  // color=-1 is colored image with color names out
  // color=1-11 is prob of colorname=color out;
  // color=-2 return probabilities
  // order of color names: black ,   blue   , brown       , grey       , green   , orange   , pink     , purple  , red     , white    , yellow
  
  const double color_values[][3] = {{0,0,0},{0,0,1},{.5,.4,.25},{.5,.5,.5},{0,1,0},{1,.8,0},{1,.5,1},{1,0,1},{1,0,0},{1,1,1},{1,1,0}};
  img_t index_im[1] = {0};
  int i, j;
  ASSERT(im->c>=3);
  //RR=im(:,:,1);GG=im(:,:,2);BB=im(:,:,3);
  
  //ÿ��ͨ��ռ5bit ��15bit
  //index_im = 1+floor(RR(:)/8)+32*floor(GG(:)/8)+32*32*floor(BB(:)/8);
#if 0
  imsetsize(index_im, im->h, im->w, sizeof(int), 1);
  for (i=0; i<im->h; ++i) {
    const uchar* im1 = im->tt.data + im->s*i;
    int* index_im1 = (int*)(index_im->tt.data + index_im->s*i);
    for (j=0; j<im->w; ++j) {
      const uchar* bgr = im1 + j*im->c;
      int rr = bgr[2], gg = bgr[1], bb = bgr[0];
      index_im1[j] = (rr>>3)+((gg>>3)<<5)+((bb>>3)<<10);
    }
  }
  imshowmat_u4(index_im); cvWaitKey(-1);
  
#endif
  
  if (color==0) {
    ASSERT(0);
    //[max1,w2cM]=max(w2c,[],2);
    //out=reshape(w2cM(index_im(:)),size(im,1),size(im,2));
  } else if(color>0 && color < 12) {
    ASSERT(0);
    //w2cM=w2c(:,color);
    //out=reshape(w2cM(index_im(:)),size(im,1),size(im,2));
  } else if(color==-1) {
    ASSERT(0);
#if 0
    out=im;
    [max1,w2cM]=max(w2c,[],2);  
    out2=reshape(w2cM(index_im(:)),size(im,1),size(im,2));
    
    for jj=1:size(im,1) {
      for ii=1:size(im,2) {
        //out(jj,ii,:)=color_values{out2(jj,ii)}'*255;
      }
    }
#endif
  } else if(color==-2) {
    //out=reshape(w2c(index_im,:),size(im,1),size(im,2),size(w2c,2));
    //imsetsize(out, im->h, im->w, W2C_W*sizeof(double), 1);
    for (i=0; i<im->h; ++i) {
      const uchar* im1 = im->tt.data + im->s*i;
      //const int* index_im1 = (const int*)(index_im->tt.data + index_im->s*i);
      double* out1 = out + step*i;
      for (j=0; j<im->w; ++j) {
        const uchar* bgr = im1 + j*im->c;
        int rr = bgr[2], gg = bgr[1], bb = bgr[0];
        int ii = (rr>>3)+((gg>>3)<<5)+((bb>>3)<<10);
        memcpy(out1+j*W2C_W, w2c+ii*W2C_W, W2C_W*sizeof(double));
      }
    }
  }
  
  return 0;
}
