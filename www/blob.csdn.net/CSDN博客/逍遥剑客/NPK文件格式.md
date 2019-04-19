# NPK文件格式 - 逍遥剑客 - CSDN博客
2010年02月13日 14:02:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：15447标签：[file																[header																[n2																[types																[dll																[table](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=types&t=blog)](https://so.csdn.net/so/search/s.do?q=n2&t=blog)](https://so.csdn.net/so/search/s.do?q=header&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)
个人分类：[引擎研究](https://blog.csdn.net/xoyojank/article/category/337622)
用驴子拖了个<<Drakensang>>, 想把资源弄出来利用一下
这游戏是用N2+N3做的, 文件格式什么的, 还是N2的
到N2的代码里找了下, 果然有读取NPK文件的代码
想想自己全部解压出来太麻烦, 不如做个查看器
但是从头自己做一个性价比不是很高, 再加上我是TC的死忠
那么, 就做个像[MPQ](http://blog.csdn.net/xoyojank/archive/2008/10/20/3111802.aspx)那样的插件好了
NPK的文件包格式, 看完N2的注释就明白了, 无非就是把文件一个个都拼了起来
数据段就是FourCC+Size+Data的这种Chunk结构, 没有版本号的概念
这样无压缩也无加密, 属于最原始的文件包格式
    npk file format:
    @verbatim
    # header
    block HEADER {
        uint32 'NPK0'       // magic number
        uint32 blockLen     // number of following bytes in block
        uint32 dataOffset   // byte offset of data block from beginning of file
    }
    # table of contents, sequence of blocks of 3 types:
    block DIR {
        uint32 'DIR_'               // magic number of directory block
        uint32 blockLen             // number of following bytes in block
        uint16 dirNameLength        // length of the following name
        char[] dirName              // name of directory
    }
    block FILE {
        uint32 'FILE'               // magic number of file block
        uint32 blockLen             // number of following bytes in block
        uint32 fileOffset           // start of file data inside data block
        uint32 fileLength           // length of file data in bytes
        uint16 fileNameLength       // length of the following name
        char[] fileName             // name of file
    }
    block ENDOFDIR {
        uint32 'DEND'               // magic number of end of dir block
        uint32 blockLen             // number of following bytes in block (0)
    }
    # the data block
    block DATA {
        uint32 'DATA'               // magic number of data block
        uint32 blockLen             // number of following bytes in block
        char[] data                 // the data
    }
    @endverbatim
整个文件的结构, 是这样组织的:
- NPK0
- DIR_ 
- DIR_ 
- FILE
- FILE
- ...
- DEND
- DIR_ 
- FILE
- FILE
- ...
- DEND
- ...
- DEND
- DATA
简单得不能再简单了, 要写一个文件读取的话, 连N2的库都不用依赖
又下了一个TC的ISO源码看了下, 原来就是个DLL, 把相应的函数实现一下, 然后扔给TC就可以用
=========================================================
更新一个TC插件:
![](http://images.cnblogs.com/cnblogs_com/xoyojank/233556/r_tc_npk.png)
![](http://images.cnblogs.com/cnblogs_com/xoyojank/233556/r_tc_npk_set.png)
![](http://images.cnblogs.com/cnblogs_com/xoyojank/233556/r_tc_npk_tex.png)
