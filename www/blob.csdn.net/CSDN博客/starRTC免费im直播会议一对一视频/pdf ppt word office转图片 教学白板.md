# pdf ppt word office转图片 教学白板 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年08月09日 10:31:13[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：93
[https://zh-cn.libreoffice.org/](https://zh-cn.libreoffice.org/)
[http://www.imagemagick.org/script/](http://www.imagemagick.org/script/)
首先用libreoffice将ppt转换为pdf格式，然后再用convert将pdf转换为图片
libreoffice可以实现doc,ppt转pdf
- 下载rpm | deb包(Debian/Ubuntu系统 (.deb包):) ，下载地址[https://zh-cn.libreoffice.org/download/](https://zh-cn.libreoffice.org/download/libreoffice-still/?version=5.2&lang=zh-CN#change)，自行选择对应版本
解压后 进入 RPMS 目录下 yum install *.rpm  
or
dpkg -i ./LibreOffice_5.2.6.2_Linux_x86-64_rpm/DEBS/*.deb
安装convert命令：yum install ImageMagick or apt install ImageMagick
Unable to locate package ImageMagick
用apt install imagemagick
soffice命令在Linux下安装后可能没有被放到 /usr/local/bin 中，这时候应该执行如下操作
# 手动更新locate数据库 sudo updatedb # 查找soffice命令安装位置 我的是在 /opt/libreoffice5.2/program/下 locate soffice
locate: command not found
apt-get install mlocate
# 在/usr/local/bin中建立软链，便于使用 ln -s /opt/libreoffice5.2/program/soffice /usr/local/bin/soffice
验证：
soffice -h
convert -h
开始转换
1. ppt -> pdf
# 当前文件夹下生成同名sample.pdf soffice --convert-to pdf:writer_pdf_Export sample.ppt
pdf -> jpg
# 将pdf文件的每一页存为jpg图片，会在当前文件夹下生成 sample-0.jpg sample-1.jpg ... # 图片后缀的数字是ppt的页码索引 convert test.pdf sample.jpg
使用 [unoconv](https://github.com/dagwieers/unoconv)
unoconv 利用 LibreOffice/OpenOffice 的 UNO 接口实现了命令行文档格式转换功能，支持所有 LibreOffice/OpenOfficee 支持的文件格式。
unoconv -f pdf 123.ppt     //将123.ppt转成pdf  
[https://docs.moodle.org/31/en/Installing_unoconv](https://docs.moodle.org/31/en/Installing_unoconv)
[https://github.com/dagwieers/unoconv](https://github.com/dagwieers/unoconv)
[https://linux.die.net/man/1/unoconv](https://linux.die.net/man/1/unoconv)
[https://qii404.me/2017/05/04/liboffice-ppt-to-image.html](https://qii404.me/2017/05/04/liboffice-ppt-to-image.html)
