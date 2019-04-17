# C/C++ 如何把数据写到文件。 - 家家的专栏 - CSDN博客





2014年08月04日 10:00:32[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：3294








For example:

// 方法一************************************************************************

#include <fstream>

#include <iostream>

using namespace std;

ofstream fout(“filename”); 

fout << num_ <<"  "  ;
fout << channels_ <<"  "  ;
fout << height_ <<"  "  ;
fout << width_ <<"  "  ;
for (int i = 0; i < count_; ++i) {
fout << data_->cpu_data() <<"  "  ;
}

fout.close();


如果想在matlab中读取，只需Data = load("filename")即可读取。






// 方法二************************************************************************

FILE* fp = fopen(file_name.c_str(), "wb");
CHECK(fp);
fwrite(&num_, sizeof(int), 1, fp);
fwrite(&channels_, sizeof(int), 1, fp);
fwrite(&height_, sizeof(int), 1, fp);
fwrite(&width_, sizeof(int), 1, fp);

       //fwrite(data_->cpu_data(), sizeof(Dtype), count_, fp); //cpudata
fwrite(mydata, sizeof(Dtype), 10000, fp); //cpudata
fclose(fp);


// *********************************************************************************








Writes data to a stream.












size_t fwrite(
   const void *buffer,
   size_t size,
   size_t count,
   FILE *stream 
);







## Parameters



- buffer


Pointer to data to be written.
- size


Item size, in bytes.
- count


Maximum number of items to be written.
- stream


Pointer to **FILE** structure.



转自：http://msdn.microsoft.com/query/dev11.query?appId=Dev11IDEF1&l=EN-US&k=k(stdio%2Ffwrite);k(fwrite);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true









