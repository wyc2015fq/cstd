
# Python 学习入门（16）—— zipfile - 阳光岛主 - CSDN博客

2013年12月05日 22:14:32[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：11150所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



zipfile 是python里用来做zip格式编码的压缩和解压缩的，由于是很常见的zip格式，所以这个模块使用频率也是比较高。
zipfile里有两个非常重要的class, 分别是ZipFile和ZipInfo, 在绝大多数的情况下，只需要使用这两个class就可以。
1）ZipFile是主要的类，用来创建和读取zip文件；
2）ZipInfo是存储的zip文件的每个文件的信息的。

**1）简单应用**
如果你仅仅是希望用python来做压缩和解压缩，那么就不用去翻文档了，这里提供一个简单的用法，让你一看就能明白。

```python
#!/usr/bin/python
# -*- coding:utf-8 -*-
#
# http://blog.ithomer.net
import zipfile
def test1():
    for i in range(1, 4):
        f = open("file" + str(i) + ".txt", 'w')
        f.write(str(i))
        f.close()
    f = zipfile.ZipFile('filename.zip', 'w', zipfile.ZIP_DEFLATED)
    f.write('file1.txt')
    f.write('file2.txt')
    f.write('file3.txt')
    f.close()
    f = zipfile.ZipFile('filename.zip')
    f.extractall()
    f.close()
if __name__ == "__main__":
    test1()
```
1.1zipfile.ZipFile(fileName[, mode[, compression[, allowZip64]]])
fileName是没有什么疑问的了。
mode和一般的文件操作一样,'r'表示打开一个存在的只读ZIP文件；'w'表示清空并打开一个只写的ZIP文件，或创建一个只写的ZIP文件；'a'表示打开一个ZIP文件，并添加内容。
compression表示压缩格式，可选的压缩格式只有2个：ZIP_STORE;ZIP_DEFLATED。ZIP_STORE是默认的，表示不压缩；ZIP_DEFLATED表示压缩，如果你不知道什么是Deflated，那么建议你去补补课。
allowZip64为True时，表示支持64位的压缩，一般而言，在所压缩的文件大于2G时，会用到这个选项；默认情况下，该值为False，因为Unix系统不支持。
1.2zipfile.close()
说真的，这个没什么可说的，如果有的话，那就是你写入的任何文件在关闭之前不会真正写入磁盘。
1.3zipfile.write(filename[, arcname[, compress_type]])
acrname是压缩文件中该文件的名字，默认情况下和filename一样
compress_type的存在是因为zip文件允许被压缩的文件可以有不同的压缩类型。
1.4zipfile.extractall([path[, member[, password]]])
path解压缩目录，没什么可说的
member需要解压缩的文件名儿列表
password当zip文件有密码时需要该选项
对于简单的应用，这么多就够了。
**2）高级应用**
2.1zipfile.is_zipfile(filename)
判断一个文件是不是压缩文件
2.2ZipFile.namelist()
返回文件列表
2.3ZipFile.open(name[, mode[, password]])
打开压缩文档中的某个文件
2.4ZipFile.infolist()
2.5ZipFile.getinfo(name)
上述文件返回ZipInfo对象，只不过一个返回的是列表，一个返回的是一个ZipInfo
示例：

```python
def test2():
    # 判断是不是压缩文件
    print zipfile.is_zipfile('filename.zip')
    # 文件列表
    f = zipfile.ZipFile('filename.zip')
    print f.namelist()          # ['file1.txt', 'file2.txt', 'file3.txt']
    print f.infolist()                  # [<zipfile.ZipInfo object at 0x7fdcfddd0438>, <zipfile.ZipInfo object at 0x7fdcfddd0500>, <zipfile.ZipInfo object at 0x7fdcfddd0370>]
    print f.getinfo('file1.txt')        # <zipfile.ZipInfo object at 0x7fdcfddd0438>
    f.close()
```

**ZipInfo类**
2.6、 ZipInfo.filename
2.7、ZipInfo.date_time
返回值的格式为(year,month,date,hour,minute,second)
2.8、ZipInfo.compress_type
2.9、ZipInfo.comment
2.10、ZipInfo.extra
2.11、ZipInfo.create_system
2.12、ZipInfo.extract_version
2.13、ZipInfo.reserved 总是0
2.14、ZipInfo.flag_bits
2.15、ZipInfo.volume
2.16、ZipInfo.internal_attr
2.17、ZipInfo.external_attr
2.18、ZipInfo.header_offset
2.19、ZipInfo.CRC
2.20、ZipInfo.file_size
2.21、ZipInfo.compress_size
2.22、ZipFile.testzip()
检查每个文件和它对应的CRC，如果有错误返回对应的文件列表
2.23、ZipFile.setpassword(password)
2.24、ZipFile.read(name[,password])
返回对应的文件
2.25、ZipFile.printdir()
打印压缩文件夹的信息
2.26、ZipFile.writestr(zipinfo_or_arcname, bytes)
**PyZipFile类**
zipfile.PyZipFile除了上面的方法和属性之外，还有一个特殊的方法
PyZipFile.writepy(pathname,basename)
一般情况下，仅仅压缩.pyc和.pyo文件，不压缩.py文件
**zip文件格式信息**
一个 ZIP 文件由三个部分组成：压缩源文件数据区+压缩源文件目录区+压缩源文件目录结束标志
1、压缩源文件数据区
在这个数据区中每一个压缩的源文件/目录都是一条记录，记录的格式如下： [文件头+ 文件数据 + 数据描述符]
a、文件头结构
组成 长度
文件头标记 4 bytes (0x04034b50)
解压文件所需 pkware 版本 2 bytes
全局方式位标记 2 bytes
压缩方式 2 bytes
最后修改文件时间 2 bytes
最后修改文件日期 2 bytes
CRC-32校验 4 bytes
压缩后尺寸 4 bytes
未压缩尺寸 4 bytes
文件名长度 2 bytes
扩展记录长度 2 bytes
文件名 （不定长度）
扩展字段 （不定长度）
b、数据描述符
组成 长度
CRC-32校验 4 bytes
压缩后尺寸 4 bytes
未压缩尺寸 4 bytes
这个数据描述符只在全局方式位标记的第３位设为１时才存在（见后详解），紧接在压缩数据的最后一个字节后。这个数据描述符只用在不能对输出的 ZIP 文件进行检索时使用。例如：在一个不能检索的驱动器（如：磁带机上）上的 ZIP 文件中。如果是磁盘上的ZIP文件一般没有这个数据描述符。
2、压缩源文件目录区
在这个数据区中每一条纪录对应在压缩源文件数据区中的一条数据
组成 长度
目录中文件文件头标记 4 bytes (0x02014b50)
压缩使用的pkware 版本 2 bytes
解压文件所需 pkware 版本 2 bytes
全局方式位标记 2 bytes
压缩方式 2 bytes
最后修改文件时间 2 bytes
最后修改文件日期 2 bytes
ＣＲＣ－３２校验 4 bytes
压缩后尺寸 4 bytes
未压缩尺寸 4 bytes
文件名长度 2 bytes
扩展字段长度 2 bytes
文件注释长度 2 bytes
磁盘开始号 2 bytes
内部文件属性 2 bytes
外部文件属性 4 bytes
局部头部偏移量 4 bytes
文件名 （不定长度）
扩展字段 （不定长度）
文件注释 （不定长度）
3、压缩源文件目录结束标志
组成 长度
目录结束标记 4 bytes (0x02014b50)
当前磁盘编号 2 bytes
目录区开始磁盘编号 2 bytes
本磁盘上纪录总数 2 bytes
目录区中纪录总数 2 bytes
目录区尺寸大小 4 bytes
目录区对第一张磁盘的偏移量 4 bytes
ZIP 文件注释长度 2 bytes
ZIP 文件注释 （不定长度）



