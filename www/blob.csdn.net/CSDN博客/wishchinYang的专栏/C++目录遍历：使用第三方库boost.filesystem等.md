# C++目录遍历：使用第三方库boost.filesystem等 - wishchinYang的专栏 - CSDN博客
2013年09月22日 09:18:58[wishchin](https://me.csdn.net/wishchin)阅读数：1294
### [1. opencv 目录文件遍历](http://blog.csdn.net/zxlstudio/article/details/10100345)
注释：2014 0814 这个代码是基于java的，Java使用托管代码进行目录管理，C++就不那么跨平台了.
原文链接：[http://blog.csdn.net/zxlstudio/article/details/10100345](http://blog.csdn.net/zxlstudio/article/details/10100345)
在做图像处理的时候，可能进行一个文件夹的所有文件的遍历。
使用c 的文件夹遍历方式，代码太难理解，而且如果在windows中使用还需要使用wchar_t宽字符。
opencv本身就有目录遍历的类库，非常方便，我以前还一直傻傻的使用c的方式进行遍历。
示例代码：非常简单的操作
```cpp
#include "iostream"  
#include "stdio.h"   
#include "opencv\cv.h"  
#include "opencv\highgui.h"  
#include <opencv2\opencv.hpp>  
  
using namespace std;  
using namespace cv;  
  
int main(int argc, char* argv[])  
{  
    string dir_path = "C:/Users/zxl/Desktop/XOXO/New folder/";  
    Directory dir;  
    vector<string> fileNames = dir.GetListFiles(dir_path, "*.jpg", false);  
  
    for(int i=0; i < fileNames.size(); i++)  
    {  
        string fileName = fileNames[i];  
        string fileFullName = dir_path + fileName;  
        cout<<"file name:"<<fileName<<endl;  
        cout<<"file paht:"<<fileFullName<<endl;  
    }  
    system("pause");  
    return 0;  
}
```
效果：![](https://img-blog.csdn.net/20130820110705875)
**[2. 用C++遍历目录](http://blog.chinaunix.net/uid-24462747-id-2980901.html)：**
http://blog.chinaunix.net/uid-24462747-id-2980901.html
## 3..使用boost::filesystem实现目录遍历
http://blog.sina.com.cn/s/blog_48d4cf2d0100mx4o.html
```cpp
下面的代码实现了深度优先和广度优先两种遍历方式，可以指定最大遍历深度，可以指定结果中是否包含子文件夹
======================================================================
#include <string>
#include <vector>
#include <deque>
#include <utility>
#include<boost/filesystem/operations.hpp>
#include<boost/filesystem/path.hpp>
class file_tool
{
public:
 
   enum traverse_order_t
    {
        DEPTH_FIRST = 1,
        BREADTH_FIRST =2,   
    };
    enum { UNLIMITED_DEPTH =-1};
    static bool get_sub_files(conststd::string& path,std::vector<std::string>&files, int max_depth = UNLIMITED_DEPTH, bool include_sub_dirs =false, traverse_order_t order = BREADTH_FIRST)
    {
        using namespace std;
        namespace fs =boost::filesystem;
        typedefstd::pair<string,int> path_and_depth_t;      
       deque<path_and_depth_t> qu;
        {
           fs::path root(path);
            if(!fs::exists(root) ||!fs::is_directory(root))
           {
               return false;
           }
            if(max_depth <= 0 &&max_depth != UNLIMITED_DEPTH)
           {
               return true;          
           }          
           fs::directory_iteratorend_iter;
            for(fs::directory_iteratorfile_itr(root); file_itr != end_iter; ++file_itr)
           {
              qu.push_back(path_and_depth_t(fs::system_complete(*file_itr).native_directory_string(),1));                                 
           }          
       }       
        while (!qu.empty())
        {
           path_and_depth_t path_and_depth = (order == DEPTH_FIRST) ?qu.back() : qu.front();
           string& file_str(path_and_depth.first);
            int depth= path_and_depth.second;
            if (order== DEPTH_FIRST)
           {
               qu.pop_back();
           }
            else
           {
               qu.pop_front();
           }
           fs::path file(file_str);
            if(fs::exists(file))
           {
               if(fs::is_directory(file))
               {
                   if (include_sub_dirs)
                   {
                      files.push_back(file_str);                     
                   }
                   if (depth <max_depth || max_depth == UNLIMITED_DEPTH)
                   {
                       intnext_depth = depth + 1;
                      fs::directory_iteratorend_iter;
                       for(fs::directory_iteratorfile_itr(file); file_itr != end_iter; ++file_itr)
                      {
                         qu.push_back(path_and_depth_t(fs::system_complete(*file_itr).native_directory_string(),next_depth));                                
                      }
                   }
               }
               else
               {
                  files.push_back(file_str);                  
               }
           }          
        }
        return true;
    }
};
```
# 4.使用boost filesystem递归遍历文件夹
原文链接：ht[tp://www.th7.cn/Program/cp/2012/02/21/60128.shtml]()
编译环境vc 9 
```cpp
#ifndef SCANALLFILES_H
#define SCANALLFILES_H
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include <iostream>
using namespace std;
class ScanAllFiles{
public:
    static const vector<string>& scanFiles(const string&,vector<string>&); //方法一,自己写递归，用filesystem里的directory_iterator
    static const vector<string>& scanFilesUseRecursive(const string&,vector<string>&); //方法二，直接用boost的filesystem里的recursive_directory_iterator
};
//方法一，自己写递归
const vector<string>& ScanAllFiles::scanFiles(const string& rootPath,vector<string>& container=*(new vector<string>())){
    namespace fs = boost::filesystem;
    fs::path fullpath (rootPath, fs::native);
    vector<string> &ret = container;
   
    if(!fs::exists(fullpath)){return ret;}
    fs::directory_iterator end_iter;  /**无参构造函数是最后那个iterator的value 摘抄如下
*If the end of the directory elements is reached, the iterator becomes equal to the end iterator value. The constructor directory_iterator() with no arguments always constructs an end iterator object, which is the only legitimate iterator to be used for the end condition. The result of operator* on an end iterator is not defined. For any other iterator value a const directory_entry& is returned. The result ofoperator-> on an end iterator is not defined. For any other iterator value a const directory_entry* is returned.
*
**/
    for(fs::directory_iterator iter(fullpath);iter!=end_iter;iter++){
        try{
            if (fs::is_directory( *iter ) ){
                std::cout<<*iter << "is dir.whose parent path is " << iter->path().branch_path() << std::endl;
                ret.push_back(iter->path().string()); //递归前push_back进去一个
                ScanAllFiles::scanFiles(iter->path().string(),ret);//递归，把vector也传进去
            }else{
                ret.push_back(iter->path().string());
                std::cout << *iter << " is a file" << std::endl;
            }
        } catch ( const std::exception & ex ){
            std::cerr << ex.what() << std::endl;
            continue;
        }
    }
    return ret;
}
//方法二，直接用boost的filesystem里的recursive_directory_iterator
const vector<string>& ScanAllFiles::scanFilesUseRecursive(const string& rootPath,vector<string>& container=*(new vector<string>())){
    namespace fs = boost::filesystem;
    fs::path fullpath (rootPath, fs::native);
    vector<string> &ret = container;
   
    if(!fs::exists(fullpath)){return ret;}
    fs::recursive_directory_iterator end_iter;
    for(fs::recursive_directory_iterator iter(fullpath);iter!=end_iter;iter++){
        try{
            if (fs::is_directory( *iter ) ){
                std::cout<<*iter << "is dir" << std::endl;
                ret.push_back(iter->path().string());
                //ScanAllFiles::scanFiles(iter->path().string(),ret);
            }else{
                ret.push_back(iter->path().string());
                std::cout << *iter << " is a file" << std::endl;
            }
        } catch ( const std::exception & ex ){
            std::cerr << ex.what() << std::endl;
            continue;
        }
    }
    return ret;
}
#endif
```
**5.我的代码：......**
```cpp
bool CCvMLP::loadFileList(const boost::filesystem::path &base_dir, const std::string &extension,
    std::vector<std::string> &FileList)
{
    if (!boost::filesystem::exists (base_dir) && !boost::filesystem::is_directory (base_dir))
        return true;
    boost::filesystem::directory_iterator it(base_dir);
    for (;
        it != boost::filesystem::directory_iterator ();
        ++it)
    {
        if (boost::filesystem::is_directory (it->status ()))
        {
            std::stringstream ss;
            ss << it->path ();
            loadFileList (it->path (), extension, FileList);
        }
        if (boost::filesystem::is_regular_file (it->status ()) && boost::filesystem::extension (it->path ()) == extension)
        {
            std::string Path;
            Path =base_dir.string();
            Path.append("/");
            Path.append(it->path().filename().string());  
            FileList.push_back (Path);
        }
    }
    return (true);
}
```
**6.我的代码-第二个版本**
```cpp
int loadFilelist(std::string  folder, const std::string  extension, std::vector<std::string>  &Filelist)
{
	//std::vector<std::string>  Filelist(0);
	Filelist.resize(0);
	Traverse(folder.c_str(), extension.c_str(), Filelist);
	//for ( auto ptr = Filelist.begin(); ptr != Filelist.end(); ++ptr )
	//{
	//}
	if (Filelist.size() > 0){
		for (int i = 0; i < Filelist.size(); ++i){
			std::string T = Filelist[i];
			std::string tF(folder);
			tF.append(T);
			Filelist[i] = tF;
		}
	}
	//return int32_t(1);
	return (1);
}
void Traverse( const char *pszPath, const char *extension, std::vector<std::string>& vctFileName )
{
	char szFind[MAX_PATH] = { 0 };
	char szFile[MAX_PATH] = { 0 };
	WIN32_FIND_DATAA FindFileData;
	//strcpy(szFind, pszPath);
	//strcpy_s(szFind, strlen(pszPath), pszPath);
	strcpy_s(szFind, 200, pszPath);
	//strcat(szFind, "//*.bmp");
	if (0){
		strcat_s(szFind, sizeof(szFind), "//*.bmp");
	}
	else{
		strcat_s(szFind, sizeof(szFind), "//*");
		strcat_s(szFind, sizeof(szFind), extension);
	}
	HANDLE  hFind = ::FindFirstFileA(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)	{
		return;
	}
	while (TRUE)	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	{
			//if(FindFileData.cFileName[0]!='.')  
			//{  
			//    strcpy(szFile,lpPath);  
			//    strcat(szFile,"//");  
			//    strcat(szFile,FindFileData.cFileName);  
			//    Traverse(szFile);  
			//}  
		}
		else{
			//cout<<FindFileData.cFileName<<endl;  
			vctFileName.push_back(FindFileData.cFileName);
		}
		if (!FindNextFileA(hFind, &FindFileData)){
			break;
		}
	}
	FindClose(hFind);
}
```
```cpp
#include <windows.h>
```
