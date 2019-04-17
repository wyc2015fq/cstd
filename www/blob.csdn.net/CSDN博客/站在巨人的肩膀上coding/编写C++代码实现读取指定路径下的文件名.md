# 编写C++代码实现读取指定路径下的文件名 - 站在巨人的肩膀上coding - CSDN博客





2018年04月18日 11:04:39[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：335








# 编写C++代码实现读取指定路径下的文件名

```cpp
#include <iostream>
#include <vector>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <io.h>

using namespace std;

//获取特定格式的文件名  
void GetAllFormatFiles(string path, vector<string>& files, string format)
{
	//文件句柄    
	long   hFile = 0;
	//文件信息    
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*" + format).c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );  
					GetAllFormatFiles(p.assign(path).append("\\").append(fileinfo.name), files, format);
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}
//获取子文件夹的文件名
void GetAllFiles(string path, vector<string>& files) {
    //文件句柄
     long hFile = 0;
	    //文件信息
	 struct _finddata_t fileinfo;  //很少用的文件信息读取结构
	     string p;  //string类很有意思的一个赋值函数:assign()，有很多重载版本
	     if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
		         do {
			             if ((fileinfo.attrib & _A_SUBDIR)) {  //比较文件类型是否是文件夹
				                 if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					                     files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					                     GetAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);					
				}
				
			}
			else {
				                 files.push_back(p.assign(path).append("\\").append(fileinfo.name));				
			}		
		} while (_findnext(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1
		        _findclose(hFile);	
	}
}

//主函数
int main()
{
	string filePath = "F:\\学术&工程\\ObjectDetect\\小论文\\实验\\2018.4.18分类实验\\svm_images";
	vector<string> files;
	char * distAll = "F:\\学术&工程\\ObjectDetect\\小论文\\实验\\2018.4.18分类实验\\SVM_classify\\myImageList.txt";

	//读取所有的文件，包括子文件的文件  
	GetAllFiles(filePath, files);  

	//读取所有格式为jpg的文件  
	string format = ".jpg";
	GetAllFormatFiles(filePath, files, format);
	ofstream ofn(distAll);
	int size = files.size();
	ofn << size << endl;
	for (int i = 0; i<size; i++)
	{
		ofn << files[i] << endl;
		cout << files[i] << endl;
	}
	ofn.close();
	return 0;
}
```


还有其他方法，参考以下链接：

【1】https://www.cnblogs.com/tgyf/p/3839894.html


【2】https://www.2cto.com/kf/201407/316515.html



