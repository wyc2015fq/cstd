# C/C++遍历文件夹和文件 - 文章 - 伯乐在线
原文出处： [踏莎行hyx](http://blog.csdn.net/u012234115/article/details/43533667)
本方法可用于windows和linux双平台，采用C/C++标准库函数。
**库函数**
包含头文件 #include
用到数据结构**_finddata_t**，文件信息结构体的指针。


```
struct _finddata_t  
{  
    unsigned attrib;     //文件属性  
    time_t time_create;  //文件创建时间  
    time_t time_access;  //文件上一次访问时间  
    time_t time_write;   //文件上一次修改时间  
    _fsize_t size;  //文件字节数  
    char name[_MAX_FNAME]; //文件名  
};
```
文件属性是无符号整数，取值为相应的宏：_A_ARCH(存档)，_A_SUBDIR(文件夹)，_A_HIDDEN(隐藏)，_A_SYSTEM(系统)，_A_NORMAL(正常)，_A_RDONLY(只读)。容易看出，通过这个结构体，我们可以得到关于该文件的很多信息。结合以下函数，我们可以将文件信息存储到这个结构体中：


```
//按FileName命名规则匹配当前目录第一个文件  
_findfirst(_In_ const char * FileName, _Out_ struct _finddata64i32_t * _FindData);   
 //按FileName命名规则匹配当前目录下一个文件  
_findnext(_In_ intptr_t _FindHandle, _Out_ struct _finddata64i32_t * _FindData);  
 //关闭_findfirst返回的文件句柄  
_findclose(_In_ intptr_t _FindHandle);
```
_findfirst 函数返回的是匹配到文件的句柄，数据类型为long。遍历过程可以指定文件类型。
**实例**


```
#include <iostream>  
#include <string>  
#include <io.h>  
using namespace std;  
  
//遍历当前目录下的文件夹和文件,默认是按字母顺序遍历  
bool TraverseFiles(string path,int &file_num)  
{  
    _finddata_t file_info;  
    string current_path=path+"/*.*"; //可以定义后面的后缀为*.exe，*.txt等来查找特定后缀的文件，*.*是通配符，匹配所有类型,路径连接符最好是左斜杠/，可跨平台  
    //打开文件查找句柄  
    int handle=_findfirst(current_path.c_str(),&file_info);  
    //返回值为-1则查找失败  
    if(-1==handle)  
        return false;  
    do  
    {  
        //判断是否子目录  
        string attribute;  
        if(file_info.attrib==_A_SUBDIR) //是目录  
            attribute="dir";  
        else  
            attribute="file";  
        //输出文件信息并计数,文件名(带后缀)、文件最后修改时间、文件字节数(文件夹显示0)、文件是否目录  
        cout<<file_info.name<<' '<<file_info.time_write<<' '<<file_info.size<<' '<<attribute<<endl; //获得的最后修改时间是time_t格式的长整型，需要用其他方法转成正常时间显示  
        file_num++;  
  
    }while(!_findnext(handle,&file_info));  //返回0则遍历完  
    //关闭文件句柄  
    _findclose(handle);  
    return true;  
}  
  
//深度优先递归遍历当前目录下文件夹和文件及子文件夹和文件  
void DfsFolder(string path,int layer)  
{  
    _finddata_t file_info;  
    string current_path=path+"/*.*"; //也可以用/*来匹配所有  
    int handle=_findfirst(current_path.c_str(),&file_info);  
    //返回值为-1则查找失败  
    if(-1==handle)  
    {  
        cout<<"cannot match the path"<<endl;  
        return;  
    }  
  
    do  
    {  
        //判断是否子目录  
        if(file_info.attrib==_A_SUBDIR)  
        {  
            //递归遍历子目录  
            //打印记号反映出深度层次  
            for(int i=0;i<layer;i++)  
                cout<<"--";  
            cout<<file_info.name<<endl;  
            int layer_tmp=layer;  
            if(strcmp(file_info.name,"..")!=0&&strcmp(file_info.name,".")!=0)  //.是当前目录，..是上层目录，必须排除掉这两种情况  
                DfsFolder(path+'/'+file_info.name,layer_tmp+1); //再windows下可以用\\转义分隔符，不推荐  
        }  
        else  
        {  
            //打印记号反映出深度层次  
            for(int i=0;i<layer;i++)  
                cout<<"--";  
            cout<<file_info.name<<endl;  
        }  
    }while(!_findnext(handle,&file_info));  //返回0则遍历完  
    //关闭文件句柄  
    _findclose(handle);  
}  
  
int main(int argc,char *argv[])  
{  
    //遍历单个目录  
    int file_num=0;  
    if(!TraverseFiles("E:/android-ndk",file_num))  //此处路径连接符只能用/，根盘符大小写都行  
        cout<<"traverse files failed"<<endl;  
    cout<<"-------------------\n"<<"file number: "<<file_num<<endl;  
  
    //递归遍历文件夹  
    DfsFolder("E:/personal_profile/tinyxml",0);  
    return 0;  
}
```
（ps：貌似注释多了点，有点碍眼T_T）
