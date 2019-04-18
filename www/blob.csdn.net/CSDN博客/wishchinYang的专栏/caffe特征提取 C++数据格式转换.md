# caffe特征提取/C++数据格式转换 - wishchinYang的专栏 - CSDN博客
2015年04月05日 20:33:43[wishchin](https://me.csdn.net/wishchin)阅读数：2027
[Caffe生成的数据分为2种格式：Lmdb 和 Leveldb](http://www.open-open.com/lib/view/open1421995285109.html)- 它们都是键/值对（Key/Value Pair）嵌入式数据库管理系统编程库。
- 虽然lmdb的内存消耗是leveldb的1.1倍，但是lmdb的速度比leveldb快10%至15%，更重要的是lmdb允许多种训练模型同时读取同一组数据集。
- 因此lmdb取代了leveldb成为Caffe默认的数据集生成格式。
create_babyface.sh调用的convertData的源代码如下：
```cpp
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include <stdio.h>
#include<string.h>
#include <fstream>  // NOLINT(readability/streams)
#include <string>
#include <vector>
#include "boost/scoped_ptr.hpp"
#include "glog/logging.h"
#include "google/protobuf/text_format.h"
#include "stdint.h"
#include "caffe/proto/caffe.pb.h"
#include "caffe/util/db.hpp"
#include <opencv/cv.h>
#include <opencv/highgui.h>
using caffe::Datum;
using boost::scoped_ptr;
using std::string;
namespace db = caffe::db;
using namespace std;
const int kCIFARSize = 32;
const int kCIFARChannelBytes = 1024;
const int kCIFARImageNBytes = 3072;
const int kCIFARBatchSize = 1000;//1000 for a batch!
const int kCIFARTrainBatches = 5;
void read_image(std::ifstream* file, int* label, char* buffer) {
    char label_char;
    file->read(&label_char, 1);
    *label = label_char;
    file->read(buffer, kCIFARImageNBytes);
    return;
}
//Read IPLimage to the buffer
void read_image(
        IplImage* out, char* buffer,
        char* RC, char* GC, char* BC)
{
    int x,y;
    int idx =0;
    for(y = 0; y<out->height; y++){
        char *ptr= out->imageData + y * out->widthStep;
        for( x = 0;x< out->width;x++){
            idx =y*out->height + x;
            BC[idx]= ptr[3*x];
            GC[idx]= ptr[3*x+1];
            RC[idx]=  ptr[3*x+2]; //这样就可以添加自己的操作，这里我使三通道颜色一样，就彩色图转黑白图了
        }
    }
    memcpy( buffer ,RC, kCIFARChannelBytes*sizeof(char) );
    memcpy( buffer+ kCIFARChannelBytes*sizeof(char) ,      GC,kCIFARChannelBytes*sizeof(char) );
    memcpy( buffer+ kCIFARChannelBytes*sizeof(char)  *2, BC,kCIFARChannelBytes*sizeof(char) );
    return;
}
//Travel the folder and load the filelist!
//使用linux dirent遍历目录
 int traveldir(char* path ,int depth, vector<string > &FileList)
{
    DIR* d;// a
    struct dirent *file; struct stat  sb;
    if( !(d=opendir(path ) ) ){
        printf("Read path %s error,wishchin! ", path);
        return -1;
    }
    while( (file= readdir(d ) ) != NULL ) {
        if(0== strncmp(file->d_name,  ".", 1 ) ) continue;
        char filename[256];
        strcpy( filename , file->d_name );
        string  Sfilename(filename);string  Spath(path);
        Spath.append(Sfilename);
        FileList.push_back(Spath);
    }
    if( stat(file->d_name,  &sb)>=0 && S_ISDIR(sb.st_mode) && depth <=4 )
        traveldir(file->d_name,depth+1,FileList);
    closedir(d);
    return 1;
}
// convert the data to the lmdb format !
void convert_dataset(
        const string& input_folder,
        const string& output_folder,
        const string& db_type) {
    scoped_ptr<db::DB> train_db(db::GetDB(db_type));
    train_db->Open(output_folder + "/babyface_train_" + db_type, db::NEW);
    scoped_ptr<db::Transaction> txn(train_db->NewTransaction());
    char* path=new char[256];
    int depth=2;
    vector<string > FileList(0);
    // Data buffer
    int label;
    IplImage* ImageS;
    char str_buffer[kCIFARImageNBytes];
    char* RC=new char[kCIFARChannelBytes];
    char* GC=new char[kCIFARChannelBytes];
    char* BC=new char[kCIFARChannelBytes];
    Datum datum;
    datum.set_channels(3);
    datum.set_height(kCIFARSize);
    datum.set_width(kCIFARSize);
    //"Writing Training data"//载入训练数据
    LOG(INFO) << "Writing Training data";
    strcpy(path,( input_folder+(string)("train1") ).c_str() );
    traveldir( path , depth, FileList);
    for (int fileid = 0; fileid < kCIFARTrainBatches; ++fileid) {
        // Open files
        LOG(INFO) << "Training Batch " << fileid + 1;
        snprintf(str_buffer, kCIFARImageNBytes, "/data_batch_%d.bin", fileid + 1);
        //CHECK(data_file) << "Unable to open train file #" << fileid + 1;
        label=1;//The Batch has 10000 pics!
        for (int itemid = 0; itemid < kCIFARBatchSize; ++itemid) {
            ImageS =cvLoadImage( (FileList[ fileid*kCIFARTrainBatches + itemid] ).c_str() );
            read_image( ImageS, str_buffer, RC,  GC, BC);
            datum.set_label(label);//datum.set_label(label);
            datum.set_data(str_buffer, kCIFARImageNBytes);
            int length = snprintf(str_buffer,  kCIFARImageNBytes,
                                  "%05d", fileid * kCIFARBatchSize + itemid);
            string out;
            CHECK(datum.SerializeToString( &out)  )  ;
            txn->Put(string(str_buffer, length),  out);//The main sentence ,put data to the txn!
        }
    }
    strcpy(path,( input_folder+(string)("train0") ).c_str() );
    traveldir( path , depth, FileList);
    for (int fileid = 0; fileid < kCIFARTrainBatches; ++fileid) {
        LOG(INFO) << "Training Batch " << fileid + 1;
        snprintf(str_buffer, kCIFARImageNBytes, "/data_batch_%d.bin", fileid + 1);
        //CHECK(data_file) << "Unable to open train file #" << fileid + 1;
        label=0;//The Batch has 10000 pics!
        for (int itemid = 0; itemid < kCIFARBatchSize; ++itemid) {
            ImageS =cvLoadImage( (FileList[ fileid*kCIFARTrainBatches + itemid] ).c_str() );
            read_image( ImageS, str_buffer, RC,  GC, BC);
            datum.set_label(label);//datum.set_label(label);
            datum.set_data(str_buffer, kCIFARImageNBytes);
            int length = snprintf(str_buffer,  kCIFARImageNBytes,
                                  "%05d", fileid * kCIFARBatchSize + itemid);
            string out;
            CHECK(datum.SerializeToString( &out)  )  ;
            txn->Put(string(str_buffer, length),  out);//The main sentence ,put data to the txn!
        }
    }
    txn->Commit();
    train_db->Close();
    //写入测试数据！
    LOG(INFO) << "Writing Testing data";
    scoped_ptr<db::DB> test_db(db::GetDB(db_type));
    test_db->Open(output_folder + "/babyface_test_" + db_type, db::NEW);
    txn.reset(test_db->NewTransaction());
    strcpy(path,( input_folder+(string)("test1") ).c_str() );
    traveldir( path , depth, FileList);
    for (int fileid = 0; fileid < 2; ++fileid) {
        LOG(INFO) << "Training Batch " << fileid + 1;
        snprintf(str_buffer, kCIFARImageNBytes, "/data_batch_%d.bin", fileid + 1);
        label=1;//The Batch has 10000 pics!
        for (int itemid = 0; itemid < kCIFARBatchSize; ++itemid) {
            ImageS =cvLoadImage( (FileList[ fileid*2 + itemid] ).c_str() );
            read_image( ImageS, str_buffer, RC,  GC, BC);
            datum.set_label(label);//datum.set_label(label);
            datum.set_data(str_buffer, kCIFARImageNBytes);
            int length = snprintf(str_buffer,  kCIFARImageNBytes,
                                  "%05d", fileid * kCIFARBatchSize + itemid);
            string out;
            CHECK(datum.SerializeToString( &out)  )  ;
            txn->Put(string(str_buffer, length),  out);//The main sentence ,put data to the txn!
        }
    }
    strcpy(path,( input_folder+(string)("test0") ).c_str() );
    traveldir( path , depth, FileList);
    for (int fileid = 0; fileid < 2; ++fileid) {
        LOG(INFO) << "Training Batch " << fileid + 1;
        snprintf(str_buffer, kCIFARImageNBytes, "/data_batch_%d.bin", fileid + 1);
        label=0;//The Batch has 10000 pics!
        for (int itemid = 0; itemid < kCIFARBatchSize; ++itemid) {
            ImageS =cvLoadImage( (FileList[ fileid*2 + itemid] ).c_str() );
            read_image( ImageS, str_buffer, RC,  GC, BC);
            datum.set_label(label);//datum.set_label(label);
            datum.set_data(str_buffer, kCIFARImageNBytes);
            int length = snprintf(str_buffer,  kCIFARImageNBytes,
                                  "%05d", fileid * kCIFARBatchSize + itemid);
            string out;
            CHECK(datum.SerializeToString( &out)  )  ;
            txn->Put(string(str_buffer, length),  out);//The main sentence ,put data to the txn!
        }
    }
    txn->Commit();
    test_db->Close();
    cvReleaseImage(&ImageS);
    delete [] RC;delete [] GC;delete [] BC;
}
int main(int argc, char** argv) {
    if (argc != 4) {
        printf("This script converts the CIFAR dataset to the leveldb format used\n"
               "by caffe to perform classification.\n"
               "Usage:\n"
               "    convert_cifar_data input_folder output_folder db_type\n"
               "Where the input folder should contain the binary batch files.\n"
               "The CIFAR dataset could be downloaded at\n"
               "    http://www.cs.toronto.edu/~kriz/cifar.html\n"
               "You should gunzip them after downloading.\n");
    } else {
        google::InitGoogleLogging(argv[0]);
        convert_dataset(string(argv[1]), string(argv[2]), string(argv[3]));
    }
    return 0;
}
```
        后记：目的是载入32×32的三通道图像，直接输入3072维的char向量进行训练，至于怎样训练网络，还得仔细查看一下。
      后记：代码出现 coredump 问题，利用 gcc path/...bin  -o coredemo -g ，出现caffe.pb.h 包含丢失现象，why？？？
