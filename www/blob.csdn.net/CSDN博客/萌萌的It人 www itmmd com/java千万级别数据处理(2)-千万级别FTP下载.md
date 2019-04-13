
# java千万级别数据处理(2)-千万级别FTP下载 - 萌萌的It人 www.itmmd.com - CSDN博客


置顶2012年04月23日 10:55:11[Jlins](https://me.csdn.net/dyllove98)阅读数：28173


**这个也是以前做过的一个程序，目的主要是去ftp主机(最多100左右)去取xx数据文件.**

千万级别只是个概念，代表数据量等于千万或者大于千万的数据
本分享不牵扯分布式采集存储之类的.是在一台机器上处理数据，如果数据量很大很大的话，可以考虑分布式处理，如果以后我有这方面的经验，会及时分享的.

**1、程序采用的ftp工具**， apache 的 commons-net-ftp-2.0.jar
**2、千万级别ftp核心关键的部分--列目录到文件**，只要是这块做好了，基本上性能就没有太大的问题了.
可以通过apache 发送ftp命令 "NLST" 的方式列目录到文件中去
\# ftp列目录执行的命令 以环境变量的配置优先,不配置则使用默认的列目录方式 NLST
Java代码
\# DS_LIST_CMD = NLST
publicFile sendCommandAndListToFile(String command,String localPathName)throwsIOException
{
try{
returnclient.createFile(command, localPathName);
}catch(IOException e) {
log.error(e);
thrownewIOException("the command "+command +" is incorrect");
}
}

当然应该还有其他形式的，大家可以自己研究一下
十万级别以上的数据量的话千万不要使用下面这种方式,如果用的话 ==== 找死
**FTPFile[] dirList = client.listFiles();**
**3、分批次从文件中读取 要下载的文件名**.  加载到内存中处理，或者读取一个文件名就下载一个文件，不要把所有的数据都加载到内存，如果很多的话会出问题
为啥要分批次？
因为是大数据量，如果有1000W条记录，列出来的目录文件的大小 1G以上吧


**4、文件下载的核心代码**----关于文件的断点续传, 获得ftp文件的大小和本地文件的大小进行判断，然后使用ftp提供的断点续传功能
下载文件一定要使用二进制的形式
**client.enterLocalPassiveMode();// 设置为被动模式**
**ftpclient.binary();  // 一定要使用二进制模式**


Java代码
/** 下载所需的文件并支持断点续传,下载后删除FTP文件，以免重复
* @param pathName 远程文件
* @param localPath 本地文件
* @param registerFileName 记录本文件名称目录
* @param size 上传文件大小
* @return true 下载及删除成功
* @throws IOException
* @throws Exception
*/
publicbooleandownLoad(String pathName, String localPath)throwsIOException {
booleanflag =false;
File file =newFile(localPath+".tmp");//设置临时文件
FileOutputStream out =null;
try{
client.enterLocalPassiveMode();// 设置为被动模式
client.setFileType(FTP.BINARY_FILE_TYPE);//设置为二进制传输
if(lff.getIsFileExists(file)){//判断本地文件是否存在，如果存在并且长度小于FTP文件的长度时断点续传;返之新增
longsize =this.getSize(pathName);
longlocalFileSize = lff.getSize(file);
if(localFileSize > size){
returnfalse;
}
out =newFileOutputStream(file,true);
client.setRestartOffset(localFileSize);
flag = client.retrieveFile(newString(pathName.getBytes(),client.getControlEncoding()),out);
out.flush();
}else{
out =newFileOutputStream(file);
flag = client.retrieveFile(newString(pathName.getBytes(),client.getControlEncoding()),out);
out.flush();
}
}catch(IOException e){
log.error(e);
log.error("file download error !");
throwe;
}finally{
try{
if(null!=out)
out.close();
if(flag)
lff.rename(file, localPath);
}catch(IOException e){
throwe;
}
}
returnflag;
}
/**
* 获取文件长度
* @param fileNamepath 本机文件
* @return
* @throws IOException
*/
publiclonggetSize(String fileNamepath)throwsIOException{
FTPFile [] ftp = client.listFiles(newString(fileNamepath.getBytes(),client.getControlEncoding()));
returnftp.length==0?0: ftp[0].getSize();
}
检测本地文件是否已经下载，如果下载文件的大小.
/**
*本地文件的 获取文件的大小
* @param file
* @return
*/
publiclonggetSize(File file){
longsize =0;
if(getIsFileExists(file)){
size = file.length();
}
returnsize;
}
**5、因为程序要跑最多100多个线程，在线程监控上做了一些处理，可以检测那些死掉的线程，并及时的拉起来。**
t.setUncaughtExceptionHandler(new ThreadException(exList));
原理：给每个线程添加 UncaughtExceptionHandler,死掉的时候把线程对应的信息加入到一个list里面，然后让主线程每隔一段时间扫描一下list，如果有数据，直接重新建一个线程运行即可

**6、如果程序是常驻内存的话，别忘记了在finally中关闭掉 不用的ftp连接**

**7、做大数据库采集程序必须考虑到的一件事情   磁盘空间已满的处理**
java虚拟机对于磁盘空间已满，在英文环境下的  linux aix 机器上 一般报
There is not enough space in the file system
中文环境下 一般报 "磁盘空间已满"
大家可以使用下面的代码进行验证

Java代码
//检测磁盘控件是否已满的异常
Java代码
//linux aix There is not enough space in the file system
// window There is not enough space in the file system
if(e.toString().contains("enough space")||e.toString().contains("磁盘空间已满"))
{
log.error("channel "+channel_name +" There is not enough space on the disk ");
Runtime.getRuntime().exit(0);
}

<千万级别数据生成xml文件>
原创文章 by dyllove98 @[http://jlins.iteye.com](http://jlins.iteye.com/)
原创文章 by dyllove98 @[http://blog.csdn.net/dyllove98](http://blog.csdn.net/dyllove98)
转载请表明出处。
<script type="text/javascript" src="http://pagead2.googlesyndication.com/pagead/show_ads.js"></script>

