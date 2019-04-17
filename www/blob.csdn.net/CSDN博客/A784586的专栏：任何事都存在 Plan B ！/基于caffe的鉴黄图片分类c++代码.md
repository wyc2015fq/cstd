# 基于caffe的鉴黄图片分类c++代码 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年06月08日 13:45:37[QuJack](https://me.csdn.net/A784586)阅读数：1379








基于caffe的鉴黄图片分类c++代码

icity.cpp:




```cpp
#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "caffe/caffe.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "sys/time.h"

#include <dlib/dir_nav.h>
#include <iostream>
#include <fstream>
#include <string>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>

using caffe::Blob;
using caffe::Caffe;
using caffe::Net;
using caffe::Layer;
using caffe::vector;

int copyfile(std::string original,std::string destination){
	int length=0;
	char originalname[100];
	length=original.copy(originalname, original.size());
	originalname[length]='\0';
	
	int length2=0;
	char destinationname[100];
	length2=destination.copy(destinationname, destination.size());
	destinationname[length2]='\0';
	
	char c = '\0';  
    int in = -1, out = -1;  
      
    //以只读方式打开数据文件  
    in = open(originalname, O_RDONLY);  
    //以只写方式创建文件，如果文件不存在就创建一个新的文件  
    //文件属主具有读和写的权限  
    out = open(destinationname, O_WRONLY|O_CREAT,  S_IRUSR| S_IWUSR);  
    while(read(in, &c, 1) == 1)//读一个字节的数据  
        write(out, &c, 1);//写一个字节的数据  
  
    //关闭文件描述符  
    close(in);  
    close(out);  
    return 0;  
	
}

int main(int argc, char** argv) {
	std::string FLAGS_weights;
	std::string net_config;
	int model_tag=0;
	for(int model=0;model<=1;model++){
		std::cout<<"print model"<<model<<std::endl;
		if(model==0){
			std::cout<<"AAA"<<std::endl;
			//定义模型
			FLAGS_weights = "nsfw.caffemodel";
			//网络定义
			net_config="deploy.prototxt";
			model_tag=0;
		}else{
			//定义模型
			std::cout<<"BBB"<<std::endl;
			FLAGS_weights = "jianhuang.caffemodel";
			//网络定义
			net_config="deploy_my.prototxt";
			model_tag=1;
		}
	//获取现在的时间
	timeval start,end;
    gettimeofday(&start, NULL);
	
	Net<float> net(net_config, caffe::TEST);
	
	net.CopyTrainedLayersFrom(FLAGS_weights);
	vector<Blob<float>* > input_blobs = net.input_blobs();
	vector<Blob<float>* > output_blobs = net.output_blobs();

	//define the map from labels to output
	int num_outputs = output_blobs[0]->count();

	//turn to new
	dlib::directory test("/opt/jiangjiangchu/jianhuang/test_picture/");
	std::vector<dlib::directory> dirs= test.get_dirs();
	sort(dirs.begin(),dirs.end());
	int num_class= dirs.size();//统计下有多少个文件夹可以读图片
	std::string fname;
	std::string filename;
	int ttotal=0;
	int tcorr = 0;
	for(int i=0;i<dirs.size();i++)
	{
		int total=0;
		int correct=0;
		std::vector<dlib::file> files = dirs[i].get_files();
		for(int j=0; j<files.size();j++)//读取每个文件夹下的图片
		{
			total = total + 1;
			timeval start,end;
			gettimeofday(&start, NULL);
			//读取每张图片预处理
			fname = files[j].full_name();
			filename=files[j].name();
			cv::Mat src= cv::imread(fname);
                        std::cout<<"fname:"<<fname<<std::endl;
			int widthOrigin = src.cols;
			int heightOrigin = src.rows;
			if(widthOrigin==0 || heightOrigin==0)
				continue;
			cv::resize(src, src, cv::Size(224,224),0,0);
			src.convertTo(src, CV_32FC3);
			cv::Mat cv_data_mean(224, 224, CV_32FC3, cv::Scalar(104.00698793, 116.66876762, 122.67891434));
			src -= cv_data_mean;
			vector<cv::Mat> mat_vec;
			split(src, mat_vec);

			float* dst_data = input_blobs[0]->mutable_cpu_data();
			float* src_data;
			//
			for (int k = 0; k < mat_vec.size(); ++k)
			{
				for (int y = 0; y<mat_vec[k].rows; y++)
				{
					src_data = mat_vec[k].ptr<float>(y);
					memcpy(dst_data, src_data, sizeof(float)*mat_vec[k].cols);
					dst_data += mat_vec[k].cols;
				}
			}

			vector<int> test_score_output_id;
			vector<float> test_score;
			float loss = 0;
			//计算
			const vector<Blob<float>*>& result =net.Forward(input_blobs);
			int my_class=0;
			for (int j = 0; j < result.size(); ++j) {
				int maxind=0;
				float maxval=-100;
				const float* result_vec = result[j]->cpu_data();
				for(int k=0; k<result[j]->count(); k++)
				{
					//打印每个值
					std::cout<<k<<"\t"<<result_vec[k]<<std::endl;
					//求最大值的坐标
					if(result_vec[k] > maxval)
					{
						maxval = result_vec[k];
						maxind = k;
					}
				}
				//std::cout<<"model :"<<model_tag<<std::endl;
				std::cout<<"Image is classified as class:"<<maxind<<std::endl;
				my_class=maxind;
			}
			std::string original_name=fname;//"./dbNudeDetection/"+filename;
			std::string imagename=filename;
			//std::fstream f;
			//f.open(original_name.c_str());
			std::cout<<"original_name:"<<original_name<<std::endl;
			std::cout<<" total: "<<total<<std::endl;
			std::string jpgname=imagename;
			//int length=0;
			if(model_tag==0){
				std::cout<<"model_1:"<<model_tag<<std::endl;
				if(my_class==2){
					std::string destination="/opt/jiangjiangchu/jianhuang/classfy_pic/nsfwclass2/"+ jpgname;
					copyfile(original_name,destination);
				//std::string str="cp "+original_name +" /opt/jiangjiangchu/jianhuang/classfy_pic/nsfwclass2/"+ jpgname;
				//std::cout<<"comand:"<<str<<std::endl;
				//char comand[100];
				//length=str.copy(comand, str.size());
				//comand[length]='\0';
				//system(comand);
				}else if(my_class==1){
					std::string destination="/opt/jiangjiangchu/jianhuang/classfy_pic/nsfwclass1/"+ jpgname;
					copyfile(original_name,destination);
				//std::string str="cp "+original_name +" /opt/jiangjiangchu/jianhuang/classfy_pic/nsfwclass1/"+ jpgname;
				//std::cout<<"comand:"<<str<<std::endl;
				//char comand[100];
				//length=str.copy(comand, str.size());
				//comand[length]='\0';
				//system(comand);
				}else if(my_class==0){
					std::string destination="/opt/jiangjiangchu/jianhuang/classfy_pic/nsfwclass0/"+ jpgname;
					copyfile(original_name,destination);
				//std::string str="cp "+original_name +" /opt/jiangjiangchu/jianhuang/classfy_pic/nsfwclass0/"+ jpgname;
				///std::cout<<"comand:"<<str<<std::endl;
				//char comand[100];
				//length=str.copy(comand, str.size());
				//comand[length]='\0';
				//system(comand);
				}
			}else if(model_tag==1){
				std::cout<<"model_2:"<<model_tag<<std::endl;
				if(my_class==2){
					std::string destination="/opt/jiangjiangchu/jianhuang/classfy_pic/ourclass2/"+ jpgname;
					copyfile(original_name,destination);
				//std::string str="cp "+original_name +" /opt/jiangjiangchu/jianhuang/classfy_pic/ourclass2/"+ jpgname;
				//std::cout<<"comand:"<<str<<std::endl;
				//char comand[100];
				//length=str.copy(comand, str.size());
				//comand[length]='\0';
				//system(comand);
				}else if(my_class==1){
					std::string destination="/opt/jiangjiangchu/jianhuang/classfy_pic/ourclass1/"+ jpgname;
					copyfile(original_name,destination);
				//std::string str="cp "+original_name +" /opt/jiangjiangchu/jianhuang/classfy_pic/ourclass1/"+ jpgname;
				//std::cout<<"comand:"<<str<<std::endl;
				//char comand[100];
				//length=str.copy(comand, str.size());
				//comand[length]='\0';
				//system(comand);
				}else if(my_class==0){
					std::string destination="/opt/jiangjiangchu/jianhuang/classfy_pic/ourclass0/"+ jpgname;
					copyfile(original_name,destination);
				//std::string str="cp "+original_name +" /opt/jiangjiangchu/jianhuang/classfy_pic/ourclass0/"+ jpgname;
				//std::cout<<"comand:"<<str<<std::endl;
				//char comand[100];
				//length=str.copy(comand, str.size());
				//comand[length]='\0';
				//system(comand);
			}
			}
			
		}
	}
	//计算耗时
	gettimeofday(&end, NULL);
    int t=(end.tv_sec - start.tv_sec)*1000000 +(end.tv_usec - start.tv_usec);
    std::cout<<"elapsed time:"<<t<<std::endl;
  }
	
}
```

make

makefile:

CXX=g++

jianhuang: icity.cpp

        $(CXX) -Ofast  -std=c++11 -DCPU_ONLY -DUSE_MKL -I./dlib-19.3   -I./include -I/opt/jiangjiangchu/intelcaffe/mkl/include icity.cpp    -L./lib -lcaffe -lopencv_highgui -lopencv_imgproc -lopencv_core -lprotobuf ./lib/libgflags.a  -lboost_system    -lhdf5
 -lhdf5_hl -L./lib/  -lcaffe -lopencv_highgui -lopencv_core -lmkldnn ./dlib-19.3/examples/build/dlib_build/libdlib.a  -o  jianhuang 


./run.sh >out.log 2>&1

run.sh:

export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH

export PATH=./lib:$PATH

./jianhuang








------------------  附 -------------------------------------------------------

linux 下C++没有copyfile类似的函数，上面是加入的自己的复制函数，下面调用系统调用system(),复制图片分类：



```cpp
#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "caffe/caffe.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "sys/time.h"

#include <dlib/dir_nav.h>
#include <iostream>
#include <fstream>
#include <string>
#include<unistd.h>
#include<stdlib.h>

using caffe::Blob;
using caffe::Caffe;
using caffe::Net;
using caffe::Layer;
using caffe::vector;

//int copyfile(std::string original,std::string destination){
//	char c = '\0';  
 //   int in = -1, out = -1;  
      
    //以只读方式打开数据文件  
 //   in = open(original, std::O_RDONLY);  
    //以只写方式创建文件，如果文件不存在就创建一个新的文件  
    //文件属主具有读和写的权限  
 //   out = open(destination, std::O_WRONLY|std::O_CREAT, std::S_IRUSR|std::S_IWUSR);  
 //   while(read(in, &c, 1) == 1)//读一个字节的数据  
 //       write(out, &c, 1);//写一个字节的数据  
  
    //关闭文件描述符  
  //  close(in);  
  //  close(out);  
  //  return 0;  
	
//}

int main(int argc, char** argv) {
	std::string FLAGS_weights;
	std::string net_config;
	int model_tag=0;
	for(int model=0;model<=1;model++){
		std::cout<<"print model"<<model<<std::endl;
		if(model==0){
			std::cout<<"AAA"<<std::endl;
			//定义模型
			FLAGS_weights = "nsfw.caffemodel";
			//网络定义
			net_config="deploy.prototxt";
			model_tag=0;
		}else{
			//定义模型
			std::cout<<"BBB"<<std::endl;
			FLAGS_weights = "jianhuang.caffemodel";
			//网络定义
			net_config="deploy_my.prototxt";
			model_tag=1;
		}
	//获取现在的时间
	timeval start,end;
    gettimeofday(&start, NULL);
	
	Net<float> net(net_config, caffe::TEST);
	
	net.CopyTrainedLayersFrom(FLAGS_weights);
	vector<Blob<float>* > input_blobs = net.input_blobs();
	vector<Blob<float>* > output_blobs = net.output_blobs();

	//define the map from labels to output
	int num_outputs = output_blobs[0]->count();

	//turn to new
	dlib::directory test("/opt/jiangjiangchu/jianhuang/test_picture/");
	std::vector<dlib::directory> dirs= test.get_dirs();
	sort(dirs.begin(),dirs.end());
	int num_class= dirs.size();//统计下有多少个文件夹可以读图片
	std::string fname;
	std::string filename;
	int ttotal=0;
	int tcorr = 0;
	for(int i=0;i<dirs.size();i++)
	{
		int total=0;
		int correct=0;
		std::vector<dlib::file> files = dirs[i].get_files();
		for(int j=0; j<files.size();j++)//读取每个文件夹下的图片
		{
			total = total + 1;
			timeval start,end;
			gettimeofday(&start, NULL);
			//读取每张图片预处理
			fname = files[j].full_name();
			filename=files[j].name();
			cv::Mat src= cv::imread(fname);
                        std::cout<<"fname:"<<fname<<std::endl;
			int widthOrigin = src.cols;
			int heightOrigin = src.rows;
			cv::resize(src, src, cv::Size(224,224),0,0);
			src.convertTo(src, CV_32FC3);
			cv::Mat cv_data_mean(224, 224, CV_32FC3, cv::Scalar(104.00698793, 116.66876762, 122.67891434));
			src -= cv_data_mean;
			vector<cv::Mat> mat_vec;
			split(src, mat_vec);

			float* dst_data = input_blobs[0]->mutable_cpu_data();
			float* src_data;
			//
			for (int k = 0; k < mat_vec.size(); ++k)
			{
				for (int y = 0; y<mat_vec[k].rows; y++)
				{
					src_data = mat_vec[k].ptr<float>(y);
					memcpy(dst_data, src_data, sizeof(float)*mat_vec[k].cols);
					dst_data += mat_vec[k].cols;
				}
			}

			vector<int> test_score_output_id;
			vector<float> test_score;
			float loss = 0;
			//计算
			const vector<Blob<float>*>& result =net.Forward(input_blobs);
			int my_class=0;
			for (int j = 0; j < result.size(); ++j) {
				int maxind=0;
				float maxval=-100;
				const float* result_vec = result[j]->cpu_data();
				for(int k=0; k<result[j]->count(); k++)
				{
					//打印每个值
					std::cout<<k<<"\t"<<result_vec[k]<<std::endl;
					//求最大值的坐标
					if(result_vec[k] > maxval)
					{
						maxval = result_vec[k];
						maxind = k;
					}
				}
				std::cout<<"model :"<<model_tag<<std::endl;
				std::cout<<"Image is classified as class:"<<maxind<<std::endl;
				my_class=maxind;
			}
			std::string original_name=fname;//"./dbNudeDetection/"+filename;
			std::string imagename=filename;
			//std::fstream f;
			//f.open(original_name.c_str());
			std::cout<<"original_name:"<<original_name<<std::endl;
			std::string jpgname=imagename;
			int length=0;
			if(model_tag==0){
				std::cout<<"model_1:"<<model_tag<<std::endl;
				if(my_class==2){
				std::string str="cp "+original_name +" /opt/jiangjiangchu/jianhuang/classfy_pic/nsfwclass2/"+ jpgname;
				std::cout<<"comand:"<<str<<std::endl;
				char comand[200];
				length=str.copy(comand, str.size());
				comand[length]='\0';
				system(comand);
				}else if(my_class==1){
				std::string str="cp "+original_name +" /opt/jiangjiangchu/jianhuang/classfy_pic/nsfwclass1/"+ jpgname;
				std::cout<<"comand:"<<str<<std::endl;
				char comand[200];
				length=str.copy(comand, str.size());
				comand[length]='\0';
				system(comand);
				}else if(my_class==0){
				std::string str="cp "+original_name +" /opt/jiangjiangchu/jianhuang/classfy_pic/nsfwclass0/"+ jpgname;
				std::cout<<"comand:"<<str<<std::endl;
				char comand[200];
				length=str.copy(comand, str.size());
				comand[length]='\0';
				system(comand);
				}
			}else if(model_tag==1){
				std::cout<<"model_2:"<<model_tag<<std::endl;
				if(my_class==2){
				std::string str="cp "+original_name +" /opt/jiangjiangchu/jianhuang/classfy_pic/ourclass2/"+ jpgname;
				std::cout<<"comand:"<<str<<std::endl;
				char comand[200];
				length=str.copy(comand, str.size());
				comand[length]='\0';
				system(comand);
				}else if(my_class==1){
				std::string str="cp "+original_name +" /opt/jiangjiangchu/jianhuang/classfy_pic/ourclass1/"+ jpgname;
				std::cout<<"comand:"<<str<<std::endl;
				char comand[200];
				length=str.copy(comand, str.size());
				comand[length]='\0';
				system(comand);
				}else if(my_class==0){
				std::string str="cp "+original_name +" /opt/jiangjiangchu/jianhuang/classfy_pic/ourclass0/"+ jpgname;
				std::cout<<"comand:"<<str<<std::endl;
				char comand[200];
				length=str.copy(comand, str.size());
				comand[length]='\0';
				system(comand);
			}
			}
			
		}
	}
	//计算耗时
	gettimeofday(&end, NULL);
    int t=(end.tv_sec - start.tv_sec)*1000000 +(end.tv_usec - start.tv_usec);
    std::cout<<"elapsed time:"<<t<<std::endl;
  }
	
}
```










