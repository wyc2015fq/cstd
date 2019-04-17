# Boost之使用篇(1) - LeftNotEasy - 博客园







# [Boost之使用篇(1)](https://www.cnblogs.com/LeftNotEasy/archive/2009/12/01/1614638.html)





    在Effective C++中提到:让自己熟悉TR1与Boost库, 重复造轮子是一件不太划算的事情. 本来还计划把一些内容自己写写, 想了想还是算了, 时间应该用来做一些更需要自己写的事情.

    使用Boost之前当然是配置它, 在VS2008下的配置方法如下:

    首先去 [http://www.boost.org/](http://www.boost.org/) 把最新版本的下载下来(目前是1.41最新了)

    解压到某一个文件夹之下,  然后新建一个Empty Project, 在所建的工程上面按右键, 选择属性

![image](https://images.cnblogs.com/cnblogs_com/LeftNotEasy/WindowsLiveWriter/Boost1_DAF0/image_thumb.png)

    在C++/常规中

![image](https://images.cnblogs.com/cnblogs_com/LeftNotEasy/WindowsLiveWriter/Boost1_DAF0/image_thumb_1.png)

    中的"附加包含目录"中, 将Boost所在目录(我的电脑中为 D:\boost_1_41_0\)添加进去.

    现在应该可以正常的包含文件了.

    下面要做的就是编译Boost, Boost自带了一个工具来做这个事情, 在Boost的目录下有一个bootstrap.bat,运行之, 经过几十分钟之后, 就编译好了, 编译后的lib文件位于stage/lib/

    下面就要设置一下连接器的目录: 在项目右键 > 属性 > 连接器 > 常规 > 附加库目录中, 把刚刚那个lib的目录填进去 : "D:\boost_1_41_0\stage\lib"

    现在应该可以正常的跑实例了.



    第一个实例我是看的filesystem, 主要是比较想找一个现成的filesystem代码.


   1:#include"boost/filesystem.hpp"// includes all needed Boost.Filesystem declarations   2:#include <iostream>               // for std::cout   3:using boost::filesystem;          // for ease of tutorial presentation;   4://  a namespace alias is preferred practice in real code   5:   6:bool find_file( const path & dir_path,         // in this directory,   7:const std::string & file_name, // search for this name,   8:                 path & path_found )            // placing path here if found   9: {  10:if ( !exists( dir_path ) ) return false;  11:   directory_iterator end_itr; // default construction yields past-the-end  12:for ( directory_iterator itr( dir_path );  13:         itr != end_itr;  14:         ++itr )  15:   {  16:if ( is_directory(itr->status()) )  17:     {  18:if ( find_file( itr->path(), file_name, path_found ) ) return true;  19:     }  20:elseif ( itr->leaf() == file_name ) // see below  21:     {  22:       path_found = itr->path();  23:return true;  24:     }  25:   }  26:return false;  27: }


    把这个例子加入进去, 另外要注意把itr->leaf(), 改为 itr->filename(), 新版的boost改变了一下函数的命名, 这就实现了在某个目录下查找某个文件了.

    代码很简单, 对着boost的文档看看就理解了.

    另外filesystem里面还有一些比较好的功能, file_size(path&)可以得到文件的大小, path这个类还重载了operator /, 可以很方便的连接目录. path与std::string是兼容的, 也可以进行比如 string s = path p这样的操作.












