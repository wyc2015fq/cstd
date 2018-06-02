

#include<stdio.h>

#include "Node.inl"
#include "RandomForest.inl"
#include "Sample.inl"
#include "Tree.inl"
#include "RandomForest.h"
#include "cfile.h"


inline void revertInt(int&x)
{
  x=((x&0x000000ff)<<24)|((x&0x0000ff00)<<8)|((x&0x00ff0000)>>8)|((x&0xff000000)>>24);
}

// read mnist dataset and do preprocess
void readMnistData(float** dataset,float*labels,const char* dataPath,const char*labelPath)
{
  FILE* dataFile=fopen(dataPath,"rb");
  FILE* labelFile=fopen(labelPath,"rb");
  int mbs=0,number=0,col=0,row=0;
  fread(&mbs,4,1,dataFile);
  fread(&number,4,1,dataFile);
  fread(&row,4,1,dataFile);
  fread(&col,4,1,dataFile);
  revertInt(mbs);
  revertInt(number);
  revertInt(row);
  revertInt(col);
  fread(&mbs,4,1,labelFile);
  fread(&number,4,1,labelFile);
  revertInt(mbs);
  revertInt(number);
  unsigned char temp;
  for(int i=0;i<number;++i)
  {
    for(int j=0;j<row*col;++j)
    {
      fread(&temp,1,1,dataFile);
      dataset[i][j]=static_cast<float>(temp);
    }
    fread(&temp,1,1,labelFile);
    labels[i]=static_cast<float>(temp);
  }
  fclose(dataFile);
  fclose(labelFile);
}

float** malloc2f(int row, int col) {
  float** ppf = MALLOC(float*, row+1);
  if (ppf) {
    int i;
    ppf[row] = NULL;
    for(i=0;i<row;++i) {
      ppf[i] = MALLOC(float, col);
    }
  }
  return ppf;
}

int free2f(float** ppf) {
  int i;
  for (i=0; ppf[i]; ++i) {
    FREE(ppf[i]);
  }
  FREE(ppf);
  return 0;
}

//#include "cstd.h"
//#include "str.h"
#include "cfile.h"
//#include "img/imgio.inl"
//#include "face/facetrack.inl"
//#include "face/facetrack.inl"
#include "face/face_recog.inl"
//#include "face/face.inl"
#include "cap/capimgseq.inl"

int test_RandomForests1() {
  int high = 0;
  capdev cap[1] = {0};
  uchar stdface[10000];
  int xywh[12] = {0};
  float dat[10000];
  int ret=0;
  if (0) {
    ret = caperoad_open(cap, 0, 640, 480) > 0;
  }
  if (1) {
    sys_chdir("D:/dt/dt_5");
    ret = capimgseq_open(cap, "list.txt", 0);
  }
  
  if (ret) {
    int fram = 0;
    char ch = 0;
    int i;
    img_t im[1] = {0};
    char* path;
    path = "F:/dataset/CAS-PEAL/CAS-PEAL-gender";
    path = "D:/pub/faceplusplus/gender/out";
    sys_chdir(path);
    float out = 0;
    RandomForest randomForest(50,10,10,0);
    randomForest.readModel("RandomForest2.Model");
    sys_mkdir("out");
  
    for (; cap_getframe(cap, im, CAP_RGB) && 'q' != ch && 'Q' != ch; ++fram) {
      //printf("%d w=%d h=%d\n", fram, im->w, im->h);
      if (face_recog(im->h, im->w, im->tt.data, im->s, im->c, "rgb", "df", 0.5, 1, xywh, stdface, NULL, NULL, NULL, 0, NULL, NULL)) {
        for (i=0; i<countof(stdface); ++i) {
          dat[i] = stdface[i];
        }
        cvShowImage("adf", 100, 100, stdface, 100, 1);
        randomForest.predict(dat, out);
        printf("%5d %s\n", fram, out>0 ? "ÄÐ" : "Å®");
        char buf[256];
        _snprintf(buf, countof(buf), "./out/%d_%s.jpg", fram, out>0 ? "ÄÐ" : "Å®");
        //imwrite(buf, im);
        //imsave(buf, im->h, im->w, im->tt.data, im->s, im->c);
        imsave(buf, 100, 100, stdface, 100, 1);
      }
      imshow(im);
      ch = cvWaitKey(100);
    }
    
    cap_close(cap);
    imfree(im);
  }

  return 0;
}

int test_RandomForests(int argc, const char* argv[])
{
  int i;
  //return test_RandomForests1();
  const char* train_dataPath = argv[1];
  const char* train_labelPath = argv[2];
  const char* test_dataPath = argv[3];
  const char* test_labelPath = argv[4];
  
  //1. prepare data
  float** trainset;
  float** testset;
  float* trainlabels;
  float* testlabels;
    char* path;
  //_chdir("F:/dataset/mnist");
  int TRAIN_NUM =60000, TEST_NUM=10000, FEATURE =784, NUMBER_OF_CLASSES =10;

  if (0) {
    path = "F:/dataset/mnist";
    sys_chdir(path);
    TRAIN_NUM =60000, TEST_NUM=10000, FEATURE =784, NUMBER_OF_CLASSES =10;
    trainset=malloc2f(TRAIN_NUM, FEATURE);
    testset=malloc2f(TEST_NUM, FEATURE);
    trainlabels=MALLOC(float, TRAIN_NUM);
    testlabels=MALLOC(float, TEST_NUM);
    
    train_dataPath = "train-images.idx3-ubyte";
    train_labelPath = "train-labels.idx1-ubyte";
    test_dataPath = "t10k-images.idx3-ubyte";
    test_labelPath = "t10k-labels.idx1-ubyte";
    
    readMnistData(trainset,trainlabels,train_dataPath, train_labelPath);
    readMnistData(testset,testlabels,test_dataPath,test_labelPath);
  }
  if (1) {
    vstr_t sv[1] = {0};
    int i, j;
    img_t im[1] = {0};
    path = "F:/dataset/CAS-PEAL/CAS-PEAL-gender";
    path = "D:/pub/faceplusplus/gender/out1";
    sys_chdir(path);
    file_t f[1] = {0};
    FILE* pf = fopen("rongyi10000.df", "rb");
    if (pf) {
      fstream_init(f, pf);
      df_loadimage(f, 0, im);
      fclose(pf);
    }
    if (1) {
      int cnt[2] = {0};
      int cnttest[2] = {0};
      TRAIN_NUM = 6000;
      TEST_NUM = im->h - TRAIN_NUM;
      FEATURE = im->w-1;
      NUMBER_OF_CLASSES = 2;
      trainset=malloc2f(TRAIN_NUM, FEATURE);
      testset=malloc2f(TEST_NUM, FEATURE);
      trainlabels=MALLOC(float, TRAIN_NUM);
      testlabels=MALLOC(float, TEST_NUM);
      for(i=0; i<im->h; ++i) {
        int istest = 0;
        int cnt1 = cnt[0]+cnt[1];
        if ((cnt1)<TRAIN_NUM) {
          if (im->tt.data[i*im->s]>0 ) {
            if (cnt[1]<TRAIN_NUM/2) {
              cnt[1]++;
              istest = 0;
            } else {
              istest = 1;
            }
          } else {
            if (cnt[0]<TRAIN_NUM/2) {
              cnt[0]++;
              istest = 0;
            } else {
              istest = 1;
            }
          }
        } else {
          istest = 1;
        }
        float* data = !istest ? trainset[cnttest[0]] : (testset[cnttest[1]]);
        float* labels = !istest ? trainlabels+cnttest[0] : (testlabels+cnttest[1]);
        *labels = im->tt.data[i*im->s];
        if (0 && FEATURE>=10000) {
          img_t im1[1] = {0};
          uchar* data1 = &im->tt.data[i*im->s + 1];
          IMINIT(im1, 100, 100, data1, 100, 1, 1);
          imshow(im1);cvWaitKey(-1);
        }
        for (j=0; j<FEATURE; ++j) {
          data[j] = im->tt.data[i*im->s + j+1];
        }
        cnttest[istest]++;
      }
      //testset = trainset;
      //testlabels = trainlabels;
      //TEST_NUM = TRAIN_NUM;
    } else {
      TRAIN_NUM = 4000;
      TEST_NUM = TRAIN_NUM;
      NUMBER_OF_CLASSES = 2;
      trainset=malloc2f(TRAIN_NUM, FEATURE);
      testset=malloc2f(TEST_NUM, FEATURE);
      trainlabels=MALLOC(float, TRAIN_NUM);
      testlabels=MALLOC(float, TEST_NUM);
      for(i=0; i<TRAIN_NUM; ++i) {
        testlabels[i] = trainlabels[i] = im->tt.data[i*im->s];
        if (0) {
          img_t im1[1] = {0};
          uchar* data1 = &im->tt.data[i*im->s + 1];
          IMINIT(im1, 100, 100, data1, 100, 1, 1);
          imshow(im1);cvWaitKey(-1);
        }
        for (j=0; j<FEATURE; ++j) {
          testset[i][j] = trainset[i][j] = im->tt.data[i*im->s + j+1];
        }
      }
    }
    imfree(im);
    strv_free(sv);
  }
  //2. create RandomForest class and set some parameters
  RandomForest randomForest(10,10,10,0);
  //randomForest.readModel("RandomForest2.Model");
  //3. start to train RandomForest
  randomForest.train(trainset,trainlabels,TRAIN_NUM,FEATURE,10,false);
  
  //read and write model to file
  randomForest.saveModel("RandomForest2.Model");
  //	randomForest.readModel("E:\\RandomForest.Model");
  //	RandomForest randomForest("E:\\RandomForest2.Model");
  
  //predict a list of samples
  float*resopnses=new float[TEST_NUM];
  randomForest.predict(testset,TEST_NUM,resopnses);
  float errorRate=0;
  for(i=0;i<TEST_NUM;++i)
  {
    if(resopnses[i]!=testlabels[i])
    {
      errorRate+=1.0f;
    }
    //for regression
    //		float diff=abs(resopnses[i]-testlabels[i]);
    //		errorRate+=diff;
  }
  errorRate/=TEST_NUM;
  printf("the total error rate is:%f\n",errorRate);
  
  delete[] resopnses;
  for(i=0;i<TRAIN_NUM;++i)
  {delete[] trainset[i];}
  for(i=0;i<TEST_NUM;++i)
  {
    delete[] testset[i];
  }
  delete[] trainlabels;
  delete[] testlabels;
  delete[] trainset;
  delete[] testset;
  return 0;
};



