# UTF-8带BOM格式与UTF-8无BOM格式转换 - fanyun的博客 - CSDN博客
2017年10月28日 19:59:27[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：5618
                
        Qt在windows下使用VS建立的工程文件转到Linux环境下编译报出大量错误，反过来也一样，原来是编码的问题。
VS采用UTF-8 BOM格式编码，而Linux下采用UTF-8无BOM格式。BOM用来标记编码的字节顺序，但是由于编码字节顺序对于UTF-8来说无关紧要，而且Unicode标准也并不推荐使用带BOM的UTF-8编码格式。总之，由于Windows和Linux的不同，为避免使用Notepad++手动更改大量文件的繁琐，提供shell转换脚本如下：
        UTF-8带BOM转UTF-8无BOM：
#!/bin/bash
#将UTF-8带BOM编码的文件转化为UTF-8无BOM格式
```python
if [[ -z "$1" ]];then
    echo '用法：./rmbom.sh [folder | file]'
    echo '将UTF-8编码的文件转化为UTF-8无BOM格式'
    exit 1
fi
path=$1
find $path -type f -name "*" -print | xargs -i sed -i '1 s/^\xef\xbb\xbf//' {}
echo "Convert finish"
UTF-8无BOM转UTF-8带BOM：
#!/bin/bash
#将UTF-8无BOM编码的文件转化为UTF-8带BOM格式
if [[ -z "$1" ]];then
    echo '用法：./addbom.sh [folder | file]'
    echo '将UTF-8无BOM格式编码的文件转化为UTF-8带BOM'
    exit 1
fi
path=$1
find $path -type f -name "*" -print | xargs -i sed -i '1 s/^/\xef\xbb\xbf&/' {}
echo "Convert finish"
```
