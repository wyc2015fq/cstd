
#include "parser/xml.inl"

CC_IMPL void
cvComment(vdstr_t* v, const char* comment) {

}

int test_opencv_xml() 
{
  char buf[1096] = {0};
  if (1) {
    // 创建文件存储对象 
    CFileStorage fs[1]={0};
    //pf = fopen("test.xml", "wb");  fclose(pf);
    memstream_init( fs->file, buf, countof(buf));
    fstream_init( fs->file, stdout );
    cvFileStorageBegin(fs, 0);
    // 写注释
    cvWriteComment(fs,"测试写XML文件",1);
    // 开始写结构，类型是图map，也就是有名字的无序节点集合 
    cvWriteStructBegin(fs,"Employee");
    // 注释
    cvWriteComment(fs,"MAP类型，姓名，年龄，薪水", 1);
    // 姓名
    cvWriteString(fs,"name","刘越");
    // 年龄
    cvWriteInt(fs,"age",18);
    // 薪水
    cvWriteReal(fs,"salary",2780.3);
    // 销售次数
    cvWriteInt(fs,"sales_count",4);
    
    {
      // 销售具体数据 
      int sales_record[]={30000,4200,50090};
      // 注释 
      cvWriteComment(fs,"SEQ类型，销售记录",1);
      // 开始写结构，类型是序列sequence，无名字的有序节点集合 
      cvWriteStructBegin(fs,"sales_record");
      // 前3条销售记录 
      cvWriteRawData(fs,sales_record,3,"i");
      // 第4条销售记录，注意无名字 
      cvWriteInt(fs,0,6100);
      // 结束 
      cvWriteStructEnd(fs);
    }
    {
      // 注释 
      cvWriteComment(fs,"MAP类型，亲友",1);
      // 开始 
      cvWriteStructBegin(fs,"Parent");
      // 父亲 
      cvWriteString(fs,"father","杨舜");
      // 母亲 
      cvWriteString(fs,"mother","王娟");
      // 结束 
      cvWriteStructEnd(fs);
    }
    if (1) {
      img_t mat[1]={0};
      cvSetMat(mat, 3,3,CC_32F);//创建一个矩阵  
      cvSetIdentity(mat);  
      cvWriteMat(fs,"A",mat, NULL);//写入一个对象，如CvMat  
      imfree(mat);//释放矩阵空间  
    }
    // 结束 
    cvWriteStructEnd(fs);
    // 释放文件存储对象 
    cvFileStorageEnd(fs);
  }
  if (1) {
    int indexFaces[] = {0, 1, 2};
    char* namePerson[] = {"aaa", "bbb", "ccc"};
    char* pathFaces[] = {"C:\\Pictures\\kobe", "C:\\Pictures\\Li", "C:\Pictures\Sun"};
    // 创建文件存储对象
    CFileStorage fs[1]={0};
    //pf = fopen("test.xml", "wb");  fclose(pf);
    memstream_init( fs->file, buf, countof(buf));
    fstream_init( fs->file, stdout );
    cvFileStorageBegin(fs, 0);
    cvWriteInt( fs, "nFaces", countof(indexFaces) );  
    cvWriteStructBegin( fs, "CVFaceRecog" );
    int i;
    for (i=0;i<countof(indexFaces);i++) {
      char person[100];  
      snprintf( person, countof(person), "person_%d", (i+1) );//必须区分开，否则读的时候会出问题  
      cvWriteStructBegin(fs, person);
      cvWriteInt( fs, "index", indexFaces[i] );
      cvWriteString( fs, "name", namePerson[i]);
      cvWriteString( fs, "directory", pathFaces[i]);
      cvWriteStructEnd(fs);
    }
    cvWriteStructEnd(fs);
    cvFileStorageEnd(fs);
  }

  if (1) {
    int i;
    char* namePerson[3] = {0};
    char* pathFaces[3] = {0};
    int indexFaces[3] = {0};
    vdstr_t dv[1] = {0};
#if 0
    CvFileNode* root = cvGetRootFileNode( fileStorage, 0);  
    CvFileNode* data = cvGetFileNodeByName( fileStorage, root, "CVFaceRecog" );  
    seq = data->data.seq;  
    cvStartReadSeq( seq, &reader, 0 );  
    int nFaces = cvReadIntByName( fileStorage, 0, "nFaces", 0 );  
    for(i = 0; i < nFaces; i++)  
    {
      CFileNode *pt = (CvFileNode*)reader.ptr;  
      namePerson.push_back(cvReadStringByName(fileStorage, pt, "name", 0));  
      pathFaces.push_back(cvReadStringByName(fileStorage, pt, "directory", 0));  
      indexFaces.push_back(cvReadIntByName(fileStorage,pt,"index",0));  
      CV_NEXT_SEQ_ELEM(seq->elem_size, reader);  
    }  
    cvReleaseFileStorage( &fileStorage );  
#endif
  }
  if (1) {
    // 文件节点
    CFileNode * node, *node2;
    char * str;
    int count;
    int * d;
    
    //cve_dm.debug_break();
    // 打开XML文件
    buf_t fs[1] = {0};
    buf_load("test.xml", fs);
#if 0
    // 获得第一层数据节点
    node = cvGetFileNodeByName(fs,0,"Employee");
    str = cvReadStringByName(fs,node,"name");
    printf("\n姓名=%s",str);
    printf("\n年龄=%d",cvReadIntByName(fs,node,"age"));
    printf("\n薪水=%g",cvReadRealByName(fs,node,"salary"));
    count = cvReadIntByName(fs,node,"sales_count");
    printf("\n销售%d条",count);
    d = cvAlloc(sizeof(int)*count);
    if(d) 
    { 
      int i;
      node2 = cvGetFileNodeByName(fs,node,"sales_record");
      if(node2) 
      { 
        cvReadRawData(fs,node2,d,"i");
        printf("\n销售记录=");
        for(i=0;i<count;i++) 
          printf("%d, ",d[i]);
      } 
      cvFree(&d);
    } 
    // 获得第二层节点 
    node = cvGetFileNodeByName(fs,node,"Parent");
    printf("\n根节点=%s",cvGetFileNodeName(node));
    str = cvReadStringByName(fs,node,"father");
    printf("\n父亲=%s",str);
    str = cvReadStringByName(fs,node,"mother");
    printf("\n母亲=%s",str);
#endif
    bffree(fs);
  }
  return 0;
}
