
# 大数据之HDFS应用开发（java API） - Magician的博客 - CSDN博客


2017年10月31日 14:50:03[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：243所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)



[目录](#目录)[1、搭建开发环境](#1搭建开发环境)
[2、获取api中的客户端对象](#2获取api中的客户端对象)
[3、DistributedFileSystem实例对象所具备的方法](#3distributedfilesystem实例对象所具备的方法)
[4、HDFS客户端操作数据代码示例](#4hdfs客户端操作数据代码示例)


# 目录
## 1、搭建开发环境
window下开发的说明:
A、在windows的某个目录下解压一个hadoop的安装包
B、将安装包下的lib和bin目录用对应windows版本平台编译的本地库替换
（这里我的环境是win10、hadoop2.6.4。
对应的win10的编译的本地库下载地址：[http://pan.baidu.com/s/1dFzYVKH](http://pan.baidu.com/s/1dFzYVKH)
密码:pdj9）
C、在window系统中配置HADOOP_HOME指向你解压的安装包
D、在windows系统的path变量中加入hadoop的bin目录
E、根据需要将hadoop下的jar包导入对应的工程中中生成新的Jar包（开发什么程序打成什么jar包。如开发HDFS程序就把hadoop下hdfs下对应的jar包打成一个新的jar包供工程使用）
## 2、获取api中的客户端对象
在java中操作hdfs，首先要获得一个客户端实例
```python
Configuration
```
```python
conf =
```
```python
new
```
```python
Configuration
```
```python
() 
FileSystem fs = FileSystem.get(conf)
```
而我们的操作目标是HDFS，所以获取到的fs对象应该是DistributedFileSystem的实例；
get方法是从何处判断具体实例化那种客户端类呢？
——从conf中的一个参数 fs.defaultFS的配置值判断；
如果我们的代码中没有指定fs.defaultFS，并且工程classpath下也没有给定相应的配置，conf中的默认值就来自于hadoop的jar包中的core-default.xml，默认值为： file:///，则获取的将不是一个DistributedFileSystem的实例，而是一个本地文件系统的客户端对象
## 3、DistributedFileSystem实例对象所具备的方法
![这里写图片描述](https://img-blog.csdn.net/20171031143949207?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 4、HDFS客户端操作数据代码示例
**4.1、文件的增删改查**
```python
/**
 * 
 * 客户端去操作hdfs时，是有一个用户身份的
 * 默认情况下，hdfs客户端api会从jvm中获取一个参数来作为自己的用户身份：
-DHADOOP_USER_NAME=hadoop
 * 也可以在构造客户端fs对象时，通过参数传递进去
 *
```
```python
@author
```
```python
*
 */
```
```python
public
```
```python
class
```
```python
HdfsClient
```
```python
{
```
```python
FileSystem fs =
```
```python
null
```
```python
;
```
```python
@Before
```
```python
public
```
```python
void
```
```python
init
```
```python
()
```
```python
throws
```
```python
Exception {
```
```python
// 构造一个配置参数对象，设置一个参数：我们要访问的hdfs的URI
```
```python
// 从而FileSystem.get()方法就知道应该是去构造一个访问hdfs文件系统的客户端，以及hdfs的访问地址
```
```python
// new Configuration();的时候，它就会去加载jar包中的hdfs-default.xml
```
```python
// 然后再加载classpath下的hdfs-site.xml
```
```python
Configuration conf =
```
```python
new
```
```python
Configuration();
                     conf.set(
```
```python
"fs.defaultFS"
```
```python
,
```
```python
"hdfs://hdp-node01:9000"
```
```python
);
```
```python
/**
                      * 参数优先级： 1、客户端代码中设置的值 2、classpath下的用户自定义配置文件 3、然后是服务器的默认配置
                      */
```
```python
// 获取一个hdfs的访问客户端，根据参数，这个实例应该是DistributedFileSystem的实例
```
```python
// fs = FileSystem.get(conf);
```
```python
// 如果这样去获取，那conf里面就可以不要配"fs.defaultFS"参数，而且，这个客户端的身份标识已经是hadoop用户
```
```python
// fs = FileSystem.get(new URI("hdfs://hdp-node01:9000"), conf, "hadoop");
```
```python
}
```
```python
/**
            * 往hdfs上传文件
            *
            *
```
```python
@throws
```
```python
Exception
            */
```
```python
@Test
```
```python
public
```
```python
void
```
```python
testAddFileToHdfs
```
```python
()
```
```python
throws
```
```python
Exception {
```
```python
// 要上传的文件所在的本地路径
```
```python
Path src =
```
```python
new
```
```python
Path(
```
```python
"g:/redis-recommend.zip"
```
```python
);
```
```python
// 要上传到hdfs的目标路径
```
```python
Path dst =
```
```python
new
```
```python
Path(
```
```python
"/aaa"
```
```python
);
                     fs.copyFromLocalFile(src, dst);
                     fs.close();
           }
```
```python
/**
            * 从hdfs中复制文件到本地文件系统
            *
            *
```
```python
@throws
```
```python
IOException
            *
```
```python
@throws
```
```python
IllegalArgumentException
            */
```
```python
@Test
```
```python
public
```
```python
void
```
```python
testDownloadFileToLocal
```
```python
()
```
```python
throws
```
```python
IllegalArgumentException, IOException {
                     fs.copyToLocalFile(
```
```python
new
```
```python
Path(
```
```python
"/jdk-7u65-linux-i586.tar.gz"
```
```python
),
```
```python
new
```
```python
Path(
```
```python
"d:/"
```
```python
));
                     fs.close();
           }
```
```python
@Test
```
```python
public
```
```python
void
```
```python
testMkdirAndDeleteAndRename
```
```python
()
```
```python
throws
```
```python
IllegalArgumentException, IOException {
```
```python
// 创建目录
```
```python
fs.mkdirs(
```
```python
new
```
```python
Path(
```
```python
"/a1/b1/c1"
```
```python
));
```
```python
// 删除文件夹 ，如果是非空文件夹，参数2必须给值true
```
```python
fs.delete(
```
```python
new
```
```python
Path(
```
```python
"/aaa"
```
```python
),
```
```python
true
```
```python
);
```
```python
// 重命名文件或文件夹
```
```python
fs.rename(
```
```python
new
```
```python
Path(
```
```python
"/a1"
```
```python
),
```
```python
new
```
```python
Path(
```
```python
"/a2"
```
```python
));
           }
```
```python
/**
            * 查看目录信息，只显示文件
            *
            *
```
```python
@throws
```
```python
IOException
            *
```
```python
@throws
```
```python
IllegalArgumentException
            *
```
```python
@throws
```
```python
FileNotFoundException
            */
```
```python
@Test
```
```python
public
```
```python
void
```
```python
testListFiles
```
```python
()
```
```python
throws
```
```python
FileNotFoundException, IllegalArgumentException, IOException {

                     RemoteIterator<LocatedFileStatus> listFiles = fs.listFiles(
```
```python
new
```
```python
Path(
```
```python
"/"
```
```python
),
```
```python
true
```
```python
);
```
```python
while
```
```python
(listFiles.hasNext()) {
                                LocatedFileStatus fileStatus = listFiles.next();
                                System.out.println(fileStatus.getPath().getName());
                                System.out.println(fileStatus.getBlockSize());
                                System.out.println(fileStatus.getPermission());
                                System.out.println(fileStatus.getLen());
                                BlockLocation[] blockLocations = fileStatus.getBlockLocations();
```
```python
for
```
```python
(BlockLocation bl : blockLocations) {
                                          System.out.println(
```
```python
"block-length:"
```
```python
+ bl.getLength() +
```
```python
"--"
```
```python
+
```
```python
"block-offset:"
```
```python
+ bl.getOffset());
                                          String[] hosts = bl.getHosts();
```
```python
for
```
```python
(String host : hosts) {
                                                     System.out.println(host);
                                          }
                                }
                                System.out.println(
```
```python
"--------------为angelababy打印的分割线--------------"
```
```python
);
                     }
           }
```
```python
/**
            * 查看文件及文件夹信息
            *
            *
```
```python
@throws
```
```python
IOException
            *
```
```python
@throws
```
```python
IllegalArgumentException
            *
```
```python
@throws
```
```python
FileNotFoundException
            */
```
```python
@Test
```
```python
public
```
```python
void
```
```python
testListAll
```
```python
()
```
```python
throws
```
```python
FileNotFoundException, IllegalArgumentException, IOException {
                     FileStatus[] listStatus = fs.listStatus(
```
```python
new
```
```python
Path(
```
```python
"/"
```
```python
));
                     String flag =
```
```python
"d--             "
```
```python
;
```
```python
for
```
```python
(FileStatus fstatus : listStatus) {
```
```python
if
```
```python
(fstatus.isFile())  flag =
```
```python
"f--         "
```
```python
;
                                System.out.println(flag + fstatus.getPath().getName());
                     }
           }
}
```

