void usmsharping() 
{
  // TODO: Add your control notification handler code here
  //CFile file2;
  int m_THRSHOLD=1;
  int m_RADIUS=1;
  int m_NUMBER=0;
  //char* strFilename = "test.bmp";
  char* strFilename = "E:\\code\\cstd\\src\\TestVideo\\aaa.bmp";
  
  //file2.Open(pFileName,CFile::modeCreate | CFile::modeWrite);
  
  FILE* file;
  file = fopen(strFilename, "rb");
  if (!file)
  {
    return;
  }
  BITMAPFILEHEADER fileHeader = {0};
  BITMAPINFOHEADER infoHeader = {0};
#define FREAD(f, buf, n)  fread(buf, n, 1, f)
#define FWRITE(f, buf, n)  fwrite(buf, n, 1, f)
  //读取数据
  FREAD(file, &fileHeader, sizeof(fileHeader));
  FREAD(file, &infoHeader, sizeof(infoHeader));
  //观察位图数据
  int ksw=infoHeader.biBitCount;//看看是多少位的位图
  long hLog=infoHeader.biHeight;
  long wLog=infoHeader.biWidth;
  int kk=infoHeader.biCompression;
  int kkk=infoHeader.biSizeImage;
  
  //每行大小，4字节对齐
  int dwTemp = ((infoHeader.biWidth * 3) % 4);
  int dwSize = 0;//DWORD
  if (dwTemp > 0)
  {
    dwSize =  infoHeader.biWidth * 3 +(4- dwTemp);
  }
  else
  {
    dwSize = infoHeader.biWidth * 3;
  }
  
  LPBYTE pBits = new BYTE[dwSize * infoHeader.biHeight];
  BYTE *pBits_i = new BYTE[infoHeader.biWidth * infoHeader.biHeight];//存储原图亮度值
  BYTE *pBits_iafter = new BYTE[infoHeader.biWidth * infoHeader.biHeight];//存储模糊后的亮度值
  BYTE *pBits_isub = new BYTE[infoHeader.biWidth * infoHeader.biHeight];//相减后的值
  BYTE *pBits_g = new BYTE[dwSize * infoHeader.biHeight];//存储模糊后的数据
  BYTE *pBits_usm = new BYTE[dwSize * infoHeader.biHeight];//存储usm之后的图像
  BYTE *pBits_ori32bmp = new BYTE[infoHeader.biWidth*4*infoHeader.biHeight];//存储32位位图的源数据
  
  if(infoHeader.biBitCount==32)//判断是多少位的位图
    FREAD(file, pBits_ori32bmp, infoHeader.biWidth*4 * infoHeader.biHeight);
  else
    FREAD(file, pBits, dwSize * infoHeader.biHeight);
  fclose(file);
  
  
  if(infoHeader.biBitCount==32)//32位放在24位处理，最后再传给pBits_ori32bmp数组
  {
    
    for(int i1=0;i1<infoHeader.biHeight; i1++)
      for (int j1=0; j1<infoHeader.biWidth;j1++)
        for(int r1=0,r2=1;r1<3,r2<4;r1++,r2++)
        {
          pBits[i1*dwSize+j1*3+r1]=pBits_ori32bmp[i1*infoHeader.biWidth*4+j1*4+r2];
          
        }
  }
  for(int i6=0;i6<infoHeader.biHeight; i6++)//pBits传给pBits_usm，用来保存边缘信息
    for (int j6=0; j6<infoHeader.biWidth;j6++)
      for(int r6=0;r6<3;r6++)
      {
        pBits_usm[i6*dwSize+j6*3+r6]=pBits[i6*dwSize+j6*3+r6];///////////////////////////////
      }
      /*for(int i=2,j=0;i<dwSize * infoHeader.biHeight,
      j<infoHeader.biWidth * infoHeader.biHeight;i=i+3,j++)
      { 
      pBits_v[j]=pBits_hsv[i];
}*/
/*for(int l=0;l<infoHeader.biWidth * infoHeader.biHeight;l++)
{
pBits_vb[j]=pBits_v[j];
}*/
      //模糊处理使用“搞死”模板1，2，1；2，4，2；1，2，1 
      //权值设置为8左右效果不错，不过没有仔细调整这个参数，也许有更好的
      int ksa;
      for (int h=1; h<infoHeader.biHeight-1; h++)
      {
        for (int w=1; w<dwSize-1; w++)
        {
          
          // ksa=abs(8.8*pBits[h* dwSize+w]-pBits[(h-1)* dwSize+w]
          // -pBits[h* dwSize+w-3]-pBits[h* dwSize+w+3]-pBits[(h+1)* dwSize+w]-pBits[(h-1)* dwSize+w-3]-pBits[(h-1)* dwSize+w+3]-pBits[(h+1)* dwSize+w-3]-pBits[(h+1)* dwSize+w+3]);
          ksa=(int)(0.083)*(4*pBits[h* dwSize+w]+2*pBits[(h-1)* dwSize+w]
            +2*pBits[h* dwSize+w-3]+2*pBits[h* dwSize+w+3]+2*pBits[(h+1)* dwSize+w]
            +pBits[(h-1)* dwSize+w-3]+pBits[(h-1)* dwSize+w+3]
            +pBits[(h+1)* dwSize+w-3]+pBits[(h+1)* dwSize+w+3]);//高斯系数可以调整，效果会不一样
          if(ksa>255)
            pBits_g[h* dwSize+w]=255;
          else if(ksa<0)
            pBits_g[h* dwSize+w]=0;
          else
            pBits_g[h* dwSize+w]=ksa;
        }
      }
      for(int h1=0;h1<infoHeader.biHeight;h1++)
        for(int w1=0;w1<infoHeader.biWidth;w1++)
        {
          pBits_i[h1*infoHeader.biWidth+w1]=(pBits[h1*dwSize+w1*3]+pBits[h1*dwSize+w1*3+1]+pBits[h1*dwSize+w1*3+2])/3;
        }
        for(int h2=0;h2<infoHeader.biHeight;h2++)
          for(int w2=0;w2<infoHeader.biWidth;w2++)
          {
            pBits_iafter[h2*infoHeader.biWidth+w2]=(pBits_g[h2*dwSize+w2*3]+pBits_g[h2*dwSize+w2*3+1]+pBits_g[h2*dwSize+w2*3+2])/3;
          }
          for(int h3=0;h3<infoHeader.biHeight;h3++)
            for(int w3=0;w3<infoHeader.biWidth;w3++)
            {
              pBits_isub[h3*infoHeader.biWidth+w3]=abs(int(pBits_i[h3*infoHeader.biWidth+w3]-pBits_iafter[h3*infoHeader.biWidth+w3]));
              
            }
            //亮度灰度图均衡化atou leave
            int max=pBits_isub[0],min=pBits_isub[1]; 
            for(int h5=0;h5<infoHeader.biHeight;h5++)
              for(int w5=2;w5<infoHeader.biWidth;w5++)
              {
                if(pBits_isub[h5*infoHeader.biWidth+w5]>max)
                  max=pBits_isub[h5*infoHeader.biWidth+w5];
                else if(pBits_isub[h5*infoHeader.biWidth+w5]<min)
                  min=pBits_isub[h5*infoHeader.biWidth+w5];
                else 
                  continue;
              }
              for(int h6=0;h6<infoHeader.biHeight;h6++)
                for(int w6=2;w6<infoHeader.biWidth;w6++)
                {
                  pBits_isub[h6*infoHeader.biWidth+w6]=(1-(max-pBits_isub[h6*infoHeader.biWidth+w6])/(max-min))*255;
                }
                for(int h7=0;h7<infoHeader.biHeight;h7++)
                  for(int w7=2;w7<infoHeader.biWidth;w7++)
                  {
                    if(pBits_isub[h7*infoHeader.biWidth+w7]<m_THRSHOLD)
                      pBits_isub[h7*infoHeader.biWidth+w7]=0;
                  }
                  
                  //制作usm模板
                  int xNumber;//x个数
                  int whModle;//模板的长宽
                  int nBanjing;// 模板的半径长度
                  if((int(m_RADIUS*2))%2==0)
                  {
                    xNumber=(int(m_RADIUS*2)+1)*(int(m_RADIUS*2)+1)-1;
                    whModle=int(m_RADIUS*2)+1;
                    nBanjing=((int)(m_RADIUS*2))/2;
                  }
                  else
                  {
                    xNumber=((int)(m_RADIUS*2))*((int)(m_RADIUS*2))-1;
                    whModle=(int)(m_RADIUS*2);
                    nBanjing=((int)(m_RADIUS*2))/2;
                  }
                  
                  float xValue;//x的值
                  float pValue;//模板中心点p的值
                  pValue=(float)1+m_NUMBER;
                  if(m_RADIUS>40)
                  {
                    if(infoHeader.biHeight*infoHeader.biWidth>=480000)
                    {
                      xValue=-1*(m_NUMBER/(xNumber/3));
                      for (int h4=(whModle-1)/2; h4<infoHeader.biHeight-(whModle-1)/2; h4++)
                      {
                        for (int w4=(whModle-1)/2; w4<infoHeader.biWidth-(whModle-1)/2; w4++)
                        {
                          //这东西想的头疼,patience
                          if(pBits_isub[h4* infoHeader.biWidth+w4]==0)
                            for(int k1=0;k1<3;k1++)
                            {
                              pBits_usm[h4* dwSize+w4*3+k1]=pBits[h4* dwSize+w4*3+k1];
                            }
                            else
                            { 
                              for(int k=0;k<3;k++)
                              {
                                float sum=0;
                                for(int hModle=h4-nBanjing;hModle<=h4+nBanjing;hModle++)
                                  for(int wModle=w4-nBanjing;wModle<=w4+nBanjing;wModle=wModle+3)
                                    
                                  {
                                    sum+=xValue*pBits[hModle* dwSize+wModle*3+k];
                                  }
                                  int ksa1;
                                  ksa1=int(pBits[h4* dwSize+w4*3+k]*(pValue-xValue)+sum);
                                  if(ksa1>255)
                                    pBits_usm[h4* dwSize+w4*3+k]=255;
                                  else if(ksa1<0)
                                    pBits_usm[h4* dwSize+w4*3+k]=0;
                                  else
                                    pBits_usm[h4* dwSize+w4*3+k]=ksa1;     
                              }
                              
                              
                            }
                        }
                      }
                    }
                    else
                    {
                      xValue=-1*(m_NUMBER/(xNumber/5));
                      for (int h4=(whModle-1)/2; h4<infoHeader.biHeight-(whModle-1)/2; h4++)
                      {
                        for (int w4=(whModle-1)/2; w4<infoHeader.biWidth-(whModle-1)/2; w4++)
                        {
                          //这东西想的头疼,patience
                          if(pBits_isub[h4* infoHeader.biWidth+w4]==0)
                            for(int k1=0;k1<3;k1++)
                            {
                              pBits_usm[h4* dwSize+w4*3+k1]=pBits[h4* dwSize+w4*3+k1];
                            }
                            else
                            { 
                              for(int k=0;k<3;k++)
                              {
                                float sum=0;
                                for(int hModle=h4-nBanjing;hModle<=h4+nBanjing;hModle++)
                                  for(int wModle=w4-nBanjing;wModle<=w4+nBanjing;wModle=wModle+3)
                                    
                                  {
                                    sum+=xValue*pBits[hModle* dwSize+wModle*3+k];
                                  }
                                  int ksa1;
                                  ksa1=int(pBits[h4* dwSize+w4*3+k]*(pValue-xValue)+sum);
                                  if(ksa1>255)
                                    pBits_usm[h4* dwSize+w4*3+k]=255;
                                  else if(ksa1<0)
                                    pBits_usm[h4* dwSize+w4*3+k]=0;
                                  else
                                    pBits_usm[h4* dwSize+w4*3+k]=ksa1;     
                              }
                              
                              
                            }
                        }
                      }
                    }
                  }
                  else if(m_RADIUS>=15)
                  {
                    if(infoHeader.biWidth*infoHeader.biHeight>=480000)
                    {
                      xValue=-1*(m_NUMBER/(xNumber/2));
                      for (int h4=(whModle-1)/2; h4<infoHeader.biHeight-(whModle-1)/2; h4++)
                      {
                        for (int w4=(whModle-1)/2; w4<infoHeader.biWidth-(whModle-1)/2; w4++)
                        {
                          //这东西想的头疼,patience
                          if(pBits_isub[h4* infoHeader.biWidth+w4]==0)
                            for(int k1=0;k1<3;k1++)
                            {
                              pBits_usm[h4* dwSize+w4*3+k1]=pBits[h4* dwSize+w4*3+k1];
                            }
                            else
                            { 
                              for(int k=0;k<3;k++)
                              {
                                float sum=0;
                                for(int hModle=h4-nBanjing;hModle<=h4+nBanjing;hModle++)
                                  for(int wModle=w4-nBanjing;wModle<=w4+nBanjing;wModle=wModle+2)
                                    
                                  {
                                    sum+=xValue*pBits[hModle* dwSize+wModle*3+k];
                                  }
                                  int ksa1;
                                  ksa1=int(pBits[h4* dwSize+w4*3+k]*(pValue-xValue)+sum);
                                  if(ksa1>255)
                                    pBits_usm[h4* dwSize+w4*3+k]=255;
                                  else if(ksa1<0)
                                    pBits_usm[h4* dwSize+w4*3+k]=0;
                                  else
                                    pBits_usm[h4* dwSize+w4*3+k]=ksa1;     
                              }
                              
                              
                            }
                        }
                      }
                    }
                    else 
                    {
                      xValue=-1*(m_NUMBER/(xNumber/2));
                      for (int h4=(whModle-1)/2; h4<infoHeader.biHeight-(whModle-1)/2; h4++)
                      {
                        for (int w4=(whModle-1)/2; w4<infoHeader.biWidth-(whModle-1)/2; w4++)
                        {
                          //这东西想的头疼,patience
                          if(pBits_isub[h4* infoHeader.biWidth+w4]==0)
                            for(int k1=0;k1<3;k1++)
                            {
                              pBits_usm[h4* dwSize+w4*3+k1]=pBits[h4* dwSize+w4*3+k1];
                            }
                            else
                            { 
                              for(int k=0;k<3;k++)
                              {
                                float sum=0;
                                for(int hModle=h4-nBanjing;hModle<=h4+nBanjing;hModle++)
                                  for(int wModle=w4-nBanjing;wModle<=w4+nBanjing;wModle=wModle+2)
                                    
                                  {
                                    sum+=xValue*pBits[hModle* dwSize+wModle*3+k];
                                  }
                                  int ksa1;
                                  ksa1=int(pBits[h4* dwSize+w4*3+k]*(pValue-xValue)+sum);
                                  if(ksa1>255)
                                    pBits_usm[h4* dwSize+w4*3+k]=255;
                                  else if(ksa1<0)
                                    pBits_usm[h4* dwSize+w4*3+k]=0;
                                  else
                                    pBits_usm[h4* dwSize+w4*3+k]=ksa1;     
                              }
                              
                              
                            }
                        }
                      }
                    }
                  }
                  else
                  {
                    xValue=-1*(m_NUMBER/xNumber);
                    for (int h4=(whModle-1)/2; h4<infoHeader.biHeight-(whModle-1)/2; h4++)
                    {
                      for (int w4=(whModle-1)/2; w4<infoHeader.biWidth-(whModle-1)/2; w4++)
                      {
                        //这东西想的头疼,patience
                        if(pBits_isub[h4* infoHeader.biWidth+w4]==0)
                          for(int k1=0;k1<3;k1++)
                          {
                            pBits_usm[h4* dwSize+w4*3+k1]=pBits[h4* dwSize+w4*3+k1];
                          }
                          else
                          { 
                            for(int k=0;k<3;k++)
                            {
                              float sum=0;
                              for(int hModle=h4-nBanjing;hModle<=h4+nBanjing;hModle++)
                                for(int wModle=w4-nBanjing;wModle<=w4+nBanjing;wModle=wModle++)
                                  
                                {
                                  sum+=xValue*pBits[hModle* dwSize+wModle*3+k];
                                }
                                int ksa1;
                                ksa1=int(pBits[h4* dwSize+w4*3+k]*(pValue-xValue)+sum);
                                if(ksa1>255)
                                  pBits_usm[h4* dwSize+w4*3+k]=255;
                                else if(ksa1<0)
                                  pBits_usm[h4* dwSize+w4*3+k]=0;
                                else
                                  pBits_usm[h4* dwSize+w4*3+k]=ksa1;     
                            }
                            
                            
                          }
                      }
                    }
                  }
                  
                  
                  for(int i3=(whModle-1)/2;i3<infoHeader.biHeight-(whModle-1)/2; i3++)
                    for (int j3=(whModle-1)/2; j3<infoHeader.biWidth-(whModle-1)/2;j3++)
                      for(int q1=0,q2=1;q1<3,q2<4;q1++,q2++)
                      {
                        pBits_ori32bmp[i3*infoHeader.biWidth*4+j3*4+q2]=pBits_usm[i3*dwSize+j3*3+q1];
                      }
                      
                      //HsvToRgb(pBits_hsv,pBits,infoHeader.biWidth * infoHeader.biHeight);
                      FILE* file1;               //测试是否能够正确显示
                      char* pFileName = "PICTURE.bmp";
                      file1 = fopen(pFileName, "wb");
                      FWRITE(file1,&fileHeader, sizeof(fileHeader));
                      FWRITE(file1,&infoHeader, sizeof(infoHeader));
                      
                      if(infoHeader.biBitCount==32)
                        FWRITE(file1,pBits_ori32bmp,infoHeader.biWidth*4 * infoHeader.biHeight);
                      else
                        FWRITE(file1,pBits_usm,dwSize * infoHeader.biHeight);
                      delete   []pBits_usm; 
                      delete   []pBits;
                      delete   []pBits_i;
                      delete   []pBits_iafter;
                      delete   []pBits_isub;
                      delete   []pBits_g;
                      delete   []pBits_ori32bmp;
}
