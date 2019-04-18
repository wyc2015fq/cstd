# 一个Matlab中批量处理文件的实例 - fighting！！！ - CSDN博客
2018年04月09日 19:06:40[dujiahei](https://me.csdn.net/dujiahei)阅读数：1430

> 
### 发现Markdown用着很爽，虽然不熟练，很多高级技巧还不会，但是一用就爱上了，么么哒
#### 不想看论文了，发一点小儿科的东西，大神来鄙视我吧
### （1）Matlab中的基本文件操作命令
Matlab的基本文件操作命令和DOS中的很像，但是功能更强大，更容易使用 
常见的，比如： 
dir
```
>> content=dir('MSRA1000')
content = 
1002x1 struct array with fields:
    name
    date
    bytes
    isdir
    datenum
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
返回的 content是一个结构体，结构体中的类目分别是：文件名称，修改日期，大小（字节），是否为目录（是否是文件夹，是为1，否则为0），数据的数目。 
帮助文档说明如下：
```
D = dir('directory_name') returns the results in an M-by-1 structure with the fields: 
        name    -- Filename
        date    -- Modification date
        bytes   -- Number of bytes allocated to the file
        isdir   -- 1 if name is a directory and 0 if not
        datenum -- Modification date as a MATLAB serial date number.
                   This value is locale-dependent.
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
刚才我那条命令的结果如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20150729200702674)
有了这个结果就可以随心所欲的处理文件了。 
还有一个类似的命令：ls 
该命令返回纯文件名的字符串数组，如果不需要其他信息的话，用ls更爽 
例如：
```
>> a=ls('3dpart_toolbox')
a =
.                  
..                 
SaliencyToolbox    
SaliencyToolbox.chm
prtools            
sift-0.9           
siftDemoV4         
vlfeat-0.9.20
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
只有文件名，清爽。 
还有一个命令：cd
```
cd     Change current working directory.
    cd directory-spec sets the current directory to the one specified.
    cd .. moves to the directory above the current one.
    cd, by itself, prints out the current directory.
    WD = cd returns the current directory as a string.
```
- 1
- 2
- 3
- 4
- 5
- 6
如以下效果：
```
>> a=cd
a =
D:\Administrator\ImageProcess_Matlab
```
- 1
- 2
- 3
- 4
- 5
还有其他命令：mkdir  ,  delete 在这里不做过多说明了，自行去 help 吧 
有了这几个命令，写一个简单的批处理脚本就很easy了
### （2）实例
我要实现的功能是将某个包含有1000张彩色图像的文件夹中所有图像转换成灰度图，并按照相同的名字以bmp格式保存到指定的文件夹内，代码如下，处理的很哈皮，心里爽爽的
```
clc;
clear;
mkdir('MSRA1000_gray');
dir_src=dir('MSRA1000');
route_src=cd;
route_src=strcat(route_src,'\MSRA1000\');
route_dst=cd;
route_dst=strcat(route_dst,'\MSRA1000_gray\');
for i=3:length(dir_src)
    name=dir_src(i).name;
    I=imread(strcat(route_src,name));
    I=rgb2gray(I);
    imwrite(I,strcat(route_dst,name(1:(length(name)-4)),'.bmp'));
end
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
至于循环为什么从3开始，因为前两个文件名称是个目录，不是文件，就酱紫，是不是很easy  
    o(∩_∩)o 哈哈
                             版权声明：本文为博主原创文章，转载请注明出处            https://blog.csdn.net/Kena_M/article/details/47133431 
