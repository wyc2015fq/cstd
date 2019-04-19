# python实现从FTP下载数据 - sxf_123456的博客 - CSDN博客
2018年05月08日 22:59:14[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1832
#！/usr/bin/python3
#-*-coding:utf-8-*-
"""
FTP常用操作
"""
*from *ftplib *import *FTP
*import *os
*class *FTP_OP(object):
*def *__init__(self, *host*, *username*, *password*, *port*):
"""
        初始化ftp
        :param host: ftp主机ip
        :param username: ftp用户名
        :param password: ftp密码
        :param port:  ftp端口 （默认21）
        """
self.host = *host*self.username = *username*self.password = *password*self.port = *port**def *ftp_connect(self):
"""
        连接ftp
        :return:
        """
ftp = FTP()
        ftp.set_debuglevel(0)  # 不开启调试模式
ftp.connect(host=self.host, port=self.port)  # 连接ftp
ftp.login(self.username, self.password)  # 登录ftp
*return *ftp
    *def *download_file(self, *ftp_file_path*, *dst_file_path*):
"""
        从ftp下载文件到本地
        :param ftp_file_path: ftp下载文件路径
        :param dst_file_path: 本地存放路径
        :return:
        """
buffer_size = 10240  #默认是8192
ftp = self.ftp_connect()
        # print(ftp.getwelcome()) #显示登录ftp信息
file_list = ftp.nlst(*ftp_file_path*)
        *for *file_name *in *file_list:
ftp_file = os.path.join(*ftp_file_path*, file_name)
            write_file = os.path.join(*dst_file_path*, file_name)
            *with *open(write_file, "wb") *as *f:
ftp.retrbinary('RETR {0}'.format(ftp_file), f.write, buffer_size)
            f.close()
        ftp.quit()
*if *__name__ == '__main__':
host = "10.0.0.1"
username = "test"
password = "test"
port = "21"
ftp_file_path = "/data/ana"
dst_file_path = "/data/tmp"
ftp = FTP_OP(host=host, username=username, password=password, port=port)
    ftp.download_file(ftp_file_path=ftp_file_path, dst_file_path=dst_file_path
                      
                      
