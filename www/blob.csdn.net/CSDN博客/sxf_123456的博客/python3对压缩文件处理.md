# python3对压缩文件处理 - sxf_123456的博客 - CSDN博客
2017年09月05日 21:45:55[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：4349
python3对压缩文件处理.
python3 对压缩文件处理
zipfile，但是除了zip之外，rar，war，jar这些压缩（或者打包）文件格式也都可以处理。
ZipFile(filename[,mode[,compression[,allowZip64]]])　　构造zipfile文件对象。mode可选r,w,a代表不同的打开文件的方式。compression指出这个zipfile用什么压缩方法，默认是ZIP_STORED,另一种选择是ZIP_DEFLATED。allowZip64是个bool型变量，当设置为True的时候就是说可以用来创建大小大于2G的zip文件，默认值是True
ZipInfo　　包含一个zip文件中的子文件的信息，字段包括filename（包括相对zip包的路径），date_time（一个时间元组，该子文件最后修改时间），compress_type（该子文件的压缩格式）等等。
对于ZipFile实例z，有以下方法：
　　　　z.close()　　关闭文件
　　　　z.extract(name[,path[,pwd]])　　从zip中提取一个文件，将它放到指定的path下，pwd是密码，用于被加密的zip文件
　　　　z.extractall(path[,pwd])　　将所有文件按照namelist中显示得那样的目录结构从当前zip中提取出来并放到path下。//这两个extract的path若不存在都会自动创建出来的，且这个path必须是个目录，解压时一定是把一个文件，包含其相对zip包路径的所有目录一起解压出来。总之有点坑，自己测试一下就知道了
　　　　z.namelist()　　返回一个列表，内容是zip文件中所有子文件的path（相对于zip文件包而言的）。相当于是一个保存了zip内部目录结构的列表
　　　　z.infolist()　　返回一个列表，内容是每个zip文件中子文件的ZipInfo对象，这个对象有上文中提到的那些字段
　　　　z.printdir()　　将zip文件的目录结构打印到stdout上，包括每个文件的path，修改时间和大小
　　　　z.open(name[,mode[,pwd]])　　获取一个子文件的文件对象，可以将其用来read,readline,write等等操作
　　　　z.setpassword(psw)　　可以为zip文件设置默认密码
　　　　z.testzip()　　读取zip中的所有文件，验证他们的CRC校验和。返回第一个损坏文件的名称，如果所有文件都是完整的就返回None
　　　　z.write(filename[,arcname[,compression_type]])　　将zip外的文件filename写入到名为arcname的子文件中（当然arcname也是带有相对zip包的路径的），compression_type指定了压缩格式，也是ZIP_STORED或ZIP_DEFLATED。z的打开方式一定要是w或者a才能顺利写入文件
读取zip文件内容
from zipfile import ZipFile
def print_file(inputfile_path):
    myzip  = ZipFile(inputfile_path,'r')
    for file_name in myzip.namelist():
        for  data in myzip.open(file_name,'r'):
            print(data)
    myzip.close()
if __name__=='__main__':
    inputfile_path = "C:\\Users\\Administrator\\Desktop\\apk数据\\14_31.zip"
    print_file(inputfile_path)
压缩目录
import  os
import  zipfile
def zip_dir(dirname,zipfilename):
    filelist = []
    if os.path.isfile(dirname): #判断压缩目录是否是文件，是则添加到文件列表filelist中
        filelist.append(dirname)
    else:
        for root,dirs,files in os.walk(dirname): #root是根目录,dirs存放的是子目录，files存放的是根目录下边的文件
            for name in files:
                filelist.append(os.path.join(root,name))
    zf = zipfile.ZipFile(zipfilename,"w",zipfile.ZIP_DEFLATED,allowZip64=True)
    for tar in filelist:
        arcname = tar[len(dirname):] #文件名
        zf.write(tar,arcname)  #将tar源文件的内容写到arcname文件中
    zf.close()
if __name__=='__main__':
    dirname = "C:\\Users\\Administrator\\Desktop\\2017-9-1"
    zipname = "2017-9-1.zip"
    zip_dir(dirname,zipname)
解压文件
import os
import zipfile
def unzip_dir(zipfilename):
    fullzipfilepath = os.path.abspath(zipfilename)  #压缩文件的绝对路径C:\Users\Administrator\Desktop\apk数据\2017-9-1.zip
    print(fullzipfilepath)
    unzipdir = fullzipfilepath.split('.zip')[0][0:] #解压文件的根目录C:\Users\Administrator\Desktop\apk数据\2017-9-1
    if not os.path.exists(fullzipfilepath):
        print("Dir %s is not exists,input fullzipfilepaht")
        fullzipfilepath = raw_input()
    if not os.path.exists(unzipdir):
        os.mkdir(unzipdir)
    zf = zipfile.ZipFile(fullzipfilepath,'r')  #读方式打开压缩
    for filename in zf.namelist():  #zf.namelist() 获取压缩包文件中的文件列表
        eachfilepath = os.path.normpath(os.path.join(unzipdir,filename))  #将文件路径转化为正常路径，
                                                             # 从压缩文件获取的文件列表中，获取的文件格式是test1/2017-9-1.txt,
                                                             #无法创建目录或文件
        eachfiledir = os.path.dirname(eachfilepath)
        if not os.path.exists(eachfiledir):
            os.mkdir(eachfiledir)
            # os.makedirs(eachfiledir)  #使用makedirs(),递归创建目录,使用mkdir(),上级目录不存在,会报出异常
        fp = open(eachfilepath,'w')
        fp.write(str(zf.read(filename)))
        fp.close()
    zf.close()
if __name__=='__main__':
    zipfilename = "C:\\Users\\Administrator\\Desktop\\apk数据\\2017-9-1.zip"
    unzip_dir(zipfilename)
注意：
os.makedirs(path[,mode])与os.mkdir(path[,mode])的区别
os.makedirs(paht[,mode])创建目录，如果目录是多级，则创建最后一级，如果最后一级的上级目录不存在，则会报出异常
os.mkdir(path[,mode])递归创建目录,如果子目录创建失败或者已经存在，会抛出一个OSError的异常，
linux上常用的tar文件不被zipfile支持，应该要用tarfile模块来处理tar文件，无论tar文件是否被压缩还是仅仅被打包，都可以读取和写入tar文件。和zipfile模块类似的，tarfile有以下一些方法和类：
　　is_tarfile(filename)　　检查是否是个有效的tar文件
　　open([name[,mode]])　　和zipfile的ZipFile有所不同的是，这里的open除了指出打开文件的方式以外还指出了文件的压缩方式。通过filemode[:compression]的方式可以指出很多种文件模式：
　　　　'r'　　读打开，如果文件是压缩得会被透明（？？？）地解压缩。这是默认打开方式
　　　　'r:'　　读打开，不压缩文件
　　　　'r:gz'　　读打开，使用gzip压缩文件
　　　　'r:bz2'　　读打开，使用bzip2压缩文件
　　　　'a','a:'　　追加打开，不压缩文件　　//注意，a模式下不能加压缩格式的。如果想要给压缩包添加什么东西的话最好另寻他路
　　　　'w','w:'　　写打开，不压缩文件
　　　　'w:gz'　　写打开，使用gzip压缩文件
　　　　'w:bz2'　　写打开只用bzip2压缩文件
　　TarInfo类对象 和ZipInfo类似的，一个子文件的TarInfo对象存储了这个子文件的一些信息。TarInfo对象有一些方法和属性：
　　　　it.gid/gname　　获取这个子文件的组ID和组名称
　　　　it.uid/uname　　获取这个子文件的用户id和用户名称
　　　　ti.isdir()　　判断这个子文件是否是个目录
　　　　ti.isfile()　　判断是否是个普通文件
　　　　ti.name　　文件名
　　　　ti.mode　　权限
　　　　ti.size　　大小
　　　　ti.mtime　　最后修改时间
　　由open返回的一个tarfile实例t有以下方法：
　　　　t.add(name[,arcname,[recursive]])　　将tar包外的文件或目录name添加到tar包内的arcname，当name是个目录时可把recursive设置为True来递归地加入tar包
　　　　t.close()
　　　　t.errorlevel　　可以设置提取tar包中文件时如何确定处理错误，当这一属性为0时，错误将被忽略，为1时错误将导致IOError和OSError，如果设置为2非致命性错误将会导致TarError
　　　　t.extract(member[,path])　　从tar包中提取一个子文件，保存到指定目录下
　　　　t.extractfile(member)　　从tar包中提取一个子文件，但返回的是个类文件对象，可以通过read，write等方法来操作文件的内容
　　　　t.getnames()　　类似于zipfile中的namelist()
　　　　t.ignore_zeros　　若这一个属性被设置为True的话，读取tar包时会跳过空块，如果这已设置为False则空块表示tar包的结束。这个属性的设置有利于读取损坏的tar包
　　　　t.list()　　类似于zipfile的printdir(),但是其列出的信息更加详细，如果不要这么详细的信息，可以加上参数False
　　　　t.getmemebers()　　返回一个列表，内容是所有文件的TarInfo对象
