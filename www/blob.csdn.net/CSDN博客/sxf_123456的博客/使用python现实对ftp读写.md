# 使用python现实对ftp读写 - sxf_123456的博客 - CSDN博客
2018年05月08日 23:16:10[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：795
*from *ftplib *import *FTP
ftp = FTP()
ftp.set_debuglevel(2) #开启调试模式，显示详细信息
ftp.connect(host=ip, port=port)
ftp.login(user=username, passwd=password)
print(ftp.getwelcome()) #输出ftp欢迎界面
buffer_size = 1024 #每次从ftp获取数据大小
filename = "filename.txt" #ftp上文件
filename_dst = "filename_dst.txt" #本地存放下载的数据
*with *open(filename_dst, "wb") *as *f:
ftp.retrbinary("RETR {0}".format(filename), filename_dst, buffer_size)
ftp.set_debuglevel(0) #关闭调试模式
ftp.quit() #退出ftp远程连接
# ftp 常用操作命令
ftp.cwd(pathname) #设置ftp的当前路径
ftp.dir()  #显示ftp目录下所有目录信息
ftp.nlst() #获取目录下文件
ftp.mkd(path)  #新建ftp远程目录
ftp.pwd()  #返回当前路径
ftp.rmd(dirname)  #删除远程目录
ftp.delete(filename) #删除远程文件
ftp.rename(fromname, toname) #将ftp服务器上的fromname修改为toname
ftp.storbinary("STOR filename.txt", filename_handle, buffer_size) #上传目标文件
ftp.retrbinary("RETR filename.txt", filename_handle, buffer_size) #下载ftp文件
