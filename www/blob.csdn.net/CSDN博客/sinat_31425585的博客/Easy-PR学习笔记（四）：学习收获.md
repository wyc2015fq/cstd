# Easy-PR学习笔记（四）：学习收获 - sinat_31425585的博客 - CSDN博客
2018年06月08日 19:24:07[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：536
一个好的框架对应于一个完整的工程，学习Easy-PR框架现阶段收获如下：
1、如何读取一个文件路径下所有图片的路径
这里使用的是<io.h>中的**_finddata_t **结构体，可以参考麒麟川的博客：[_finddata_t用法](https://blog.csdn.net/wangqingchuan92/article/details/77979669)
_finddata_t 结构体如下：
```cpp
struct _finddata_t
{
	// 文件属性的存储位置：_A_ARCH（存档）、 _A_HIDDEN（隐藏）、
	// _A_NORMAL（正常）、_A_RDONLY（只读）、_A_SUBDIR（文件夹）、
	// _A_SYSTEM（系统） 
	unsigned attrib;  
	// 文件创建时间
	time_t time_create;
	// 文件最后一次访问时间
	time_t time_access;
	// 文件最后一次被修改的时间
	time_t time_write;
	// 文件大小
	_fsize_t size;
	// 文件名
	char name[_MAX_FNAME];
};
```
主要用到_findfirst（）、_findnext（）和_fineclose（）三个接口，三个接口如下：
_findfirst（）函数：
```cpp
long _findfirst( char *filespec, struct _finddata_t *fileinfo )；
```
_findfirst 查找文件，如果查找成功，返还一个唯一编号，反之，返还-1
_findnext（）函数：
```cpp
int _findnext( long handle, struct _finddata_t *fileinfo );
```
查找handle对应路径下文件，文件信息存入fileinfo中，若查找成功返回0，反之返回-1。
_findclose（）函数：
```cpp
int _findclose( long handle );
```
查找结束后，关闭文件句柄。
测试程序如下：
```cpp
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <io.h>	   // read file 
using namespace cv;
using namespace cv::ml;
using namespace std;
vector<string> getFiles(const string & folder,
	const bool all = true)
{
	vector<string> files;
	list<string> subfolders;
	subfolders.push_back(folder);
	while (!subfolders.empty())
	{
		string current_folder(subfolders.back());
		if (*(current_folder.end() - 1) != '/')
		{
			current_folder.append("/*");
		}
		else
		{
			current_folder.append("*");
		}
		subfolders.pop_back();
		struct _finddata_t file_info;
		// _findfirst 查找文件，如果查找成功，返还一个唯一编号，反之，返还-1
		auto file_handler = _findfirst(current_folder.c_str(), &file_info);
		while (file_handler != -1)
		{
			if (all && (!strcmp(file_info.name, ".") || !strcmp(file_info.name, "..")))
			{
				if (_findnext(file_handler, &file_info) != 0)break;
				continue;
			}
			// 如果是一个文件夹
			if (file_info.attrib & _A_SUBDIR)
			{
				if (all)
				{
					// 获取子文件夹名
					string folder_t(current_folder);
					folder_t.pop_back();
					folder_t.append(file_info.name);
					subfolders.push_back(folder_t.c_str());
				}
			}
			else
			{
				string file_path;
				// assign：将原字符串清空，然后赋予新的值
				// pop_back：删除字符串最后一个字符
				file_path.assign(current_folder.c_str()).pop_back();
				file_path.append(file_info.name);
				files.push_back(file_path);
			}
			if (_findnext(file_handler, &file_info) != 0) break;
		}  // end while
		_findclose(file_handler);
	}
	return files;
}
int main()
{
	vector<string> names;
	string folder = "has/test";
	names = getFiles(folder);
	for (int i = 0; i < names.size(); i++)
	{
		cout << i << "-th  picture is: " << names[i] << endl;
	}
	
	system("pause");
	return 0;
}
```
效果如下：
![](https://img-blog.csdn.net/20180608192239541)
这样就可以在windows系统下指定文件路径下的所有文件的绝对路径。
2、训练SVM+HOG的分类器
HOG特征提取代码为：
```cpp
void getHOGFeature(const Mat& image, Mat& features)
{
	HOGDescriptor hog(cvSize(128, 64), cvSize(16, 16), cvSize(8, 8), cvSize(8, 8), 3); //these parameters work well
	vector<float> descriptor;
	// resize input image to (128,64) for compute
	Size dsize = Size(128, 64);
	Mat trainImg = Mat(dsize, CV_32S);
	resize(image, trainImg, dsize);
	// compute descripter
	hog.compute(trainImg, descriptor, Size(8, 8));
	// copy the result
	Mat mat_feature(descriptor);
	mat_feature.copyTo(features);
	//mat_feature.reshape(1, 1);
	//mat_feature.convertTo(features, CV_32FC1);
}
```
读入训练集图片后，分别提取每个样本的HOG特征，然后转换成训练SVM所需要的数据格式：CV_32FC1
```cpp
int main()
{
	string folder_has_train = "has/train";
	auto has_file_train_list = getFiles(folder_has_train);
	random_shuffle(has_file_train_list.begin(), has_file_train_list.end());
	string folder_has_test = "has/test";
	auto has_file_test_list = getFiles(folder_has_test);
	random_shuffle(has_file_test_list.begin(), has_file_test_list.end());
	string folder_no_train = "no/train";
	auto no_file_train_list = getFiles(folder_no_train);
	random_shuffle(no_file_train_list.begin(), no_file_train_list.end());
	string folder_no_test = "no/test";
	auto no_file_test_list = getFiles(folder_no_test);
	random_shuffle(no_file_test_list.begin(), no_file_test_list.end());
	vector<TrainItem> train_file_list_;
	fprintf(stdout, ">> Collecting train data...\n");
	for (auto file : has_file_train_list)
		train_file_list_.push_back({ file, 1 });
	for (auto file : no_file_train_list)
		train_file_list_.push_back({ file, 0 });
	auto train_data = convertData(train_file_list_);   // 转换成opencv需要的SVM数据格式
	// create SVM classifier
	Ptr<SVM> _svm = SVM::create();
	_svm->setType(SVM::C_SVC);
	_svm->setKernel(SVM::CHI2);
	_svm->setDegree(0.1);
	_svm->setGamma(0.1);
	_svm->setCoef0(0.1);
	_svm->setC(1);
	_svm->setNu(0.1);
	_svm->setTermCriteria(cvTermCriteria(CV_TERMCRIT_ITER, 20000, 0.0001));
	auto start_time = static_cast<long>(cv::getTickCount());
	_svm->trainAuto(train_data, 10, SVM::getDefaultGrid(SVM::C),
		SVM::getDefaultGrid(SVM::GAMMA), SVM::getDefaultGrid(SVM::P),
		SVM::getDefaultGrid(SVM::NU), SVM::getDefaultGrid(SVM::COEF),
		SVM::getDefaultGrid(SVM::DEGREE), true);
	auto end_time = static_cast<long>(cv::getTickCount());
	cout << "VarCount: " << _svm->getVarCount() << endl;
	fprintf(stdout, ">> Training done. Time elapse: %ldms\n", end_time - start_time);
	fprintf(stdout, ">> Saving model file...\n");
	_svm->save("model/svm.xml");
	fprintf(stdout, ">> Your SVM Model was saved to model/svm.xml\n");
}
```
训练完毕后，会在model路径下生成模型文件：svm.xml            
