# c libghttp  ghttp  库使用指南 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年04月25日 09:28:10[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1535
libghttp是一个很好用的 http 库，能够轻松地实现同步和异步的HTTP请求
## 目录
 [[隐藏](http://172.30.24.252/elesos_com/index.php?title=Libghttp%E5%BA%93%E4%BD%BF%E7%94%A8%E6%8C%87%E5%8D%97#)] 
- [1 安装](http://172.30.24.252/elesos_com/index.php?title=Libghttp%E5%BA%93%E4%BD%BF%E7%94%A8%E6%8C%87%E5%8D%97#.E5.AE.89.E8.A3.85)
- [2 GET示例](http://172.30.24.252/elesos_com/index.php?title=Libghttp%E5%BA%93%E4%BD%BF%E7%94%A8%E6%8C%87%E5%8D%97#GET.E7.A4.BA.E4.BE.8B)
- [3 POST示例](http://172.30.24.252/elesos_com/index.php?title=Libghttp%E5%BA%93%E4%BD%BF%E7%94%A8%E6%8C%87%E5%8D%97#POST.E7.A4.BA.E4.BE.8B)
- [4 相关函数](http://172.30.24.252/elesos_com/index.php?title=Libghttp%E5%BA%93%E4%BD%BF%E7%94%A8%E6%8C%87%E5%8D%97#.E7.9B.B8.E5.85.B3.E5.87.BD.E6.95.B0)
- [5 艺搜参考](http://172.30.24.252/elesos_com/index.php?title=Libghttp%E5%BA%93%E4%BD%BF%E7%94%A8%E6%8C%87%E5%8D%97#.E8.89.BA.E6.90.9C.E5.8F.82.E8.80.83)
## 安装
库文件下载：
在64位机器下configure时出现错误信息:
...
checking host system type... Invalid configuration **`**x86_64-unknown-linux-gnu': machine `x86_64-unknown' not recognized
checking build system type... Invalid configuration **`**x86_64-unknown-linux-gnu': machine `x86_64-unknown' not recognized
...
ltconfig: you must specify a host **type****if** you use **`**--no-verify'
Try `ltconfig --help'**for****more** information.
configure: error: libtool configure failed
...
即configure无法识别系统的类型, 所以提示you must specify a host type.
解决方法：
用 /usr/share/libtool/config/config.guess覆盖源码包中的config.guess
tar -zxvf libghttp-1.0.9.tar.gz 
cd libghttp-1.0.9
**cp****/**usr**/**share**/**libtool**/**config**/**config.guess  .**/**config.guess
用 /usr/share/libtool/config/config.sub 覆盖源码包中的 config.sub
**cp****/**usr**/**share**/**libtool**/**config**/**config.sub  .**/**config.sub
这样configure就可以猜出系统的类型了.
开始安装：
.**/**configure --prefix=**/**usr**/****local****make****sudo****make****install**
## GET示例
*/* * libghttp_get.c *  http get test *  Created on: 2013年10月25日 *      Author: elesos.com */*#include <stdio.h>#include <string.h>#include <ghttp.h>int main(int argc,char**argv){char*uri ="http://www.elesos.com/%E9%A6%96%E9%A1%B5";
    ghttp_request *request = NULL;
    ghttp_status status;
    FILE * pFile;char*buf;int bytes_read;int size;
 
    pFile =fopen("elesos.html","wb");
 
    request = ghttp_request_new();if(ghttp_set_uri(request, uri)==-1)return-1;if(ghttp_set_type(request, ghttp_type_get)==-1)*//get*return-1;
    ghttp_prepare(request);
    status = ghttp_process(request);if(status == ghttp_error)return-1;printf("Status code -> %d**\n**", ghttp_status_code(request));
    buf = ghttp_get_body(request);
 
    bytes_read = ghttp_get_body_len(request);
    size =strlen(buf);*//size == bytes_read*fwrite(buf ,1,size , pFile );fclose(pFile);return0;}
## POST示例
int post_test(){char szXML[2048];char szVal[256];
 
	ghttp_request *request = NULL;
	ghttp_status status;char*buf;char retbuf[128];int len;strcpy(szXML,"POSTDATA=");sprintf(szVal,"%d",15);strcat(szXML, szVal);printf("%s**\n**", szXML);*//test*
 
	request = ghttp_request_new();if(ghttp_set_uri(request, uri)==-1)return-1;if(ghttp_set_type(request, ghttp_type_post)==-1)*//post*return-1;
 
	ghttp_set_header(request, http_hdr_Content_Type,"application/x-www-form-urlencoded");*//ghttp_set_sync(request, ghttp_sync); //set sync*
	len =strlen(szXML);
	ghttp_set_body(request, szXML, len);*//*
	ghttp_prepare(request);
	status = ghttp_process(request);if(status == ghttp_error)return-1;
	buf = ghttp_get_body(request);*//test*sprintf(retbuf,"%s", buf);
	ghttp_clean(request);return0;}
源代码打包下载：[艺搜下载](http://dl.vmall.com/c0n088ymiz)
一般在do{}while(1)中，选择使用同步的方式；
如果是set(callback)的方式，这时可以使用异步的方式。如果是异步的方式，一般涉及到对接收包的排序问题。参见[同步和异步的区别](http://172.30.24.252/elesos_com/index.php?title=%E5%90%8C%E6%AD%A5%E5%92%8C%E5%BC%82%E6%AD%A5%E7%9A%84%E5%8C%BA%E5%88%AB)
## 相关函数
ghttp_set_sync(request, ghttp_sync);*//设置同步**// This is the http request object*
ghttp_request *request = NULL;*// Allocate a new empty request object*
request = ghttp_request_new();*// Set the URI for the request object*
ghttp_set_uri(request,"http://localhost:8080/index.html");*// Close the connection after you are done.*
ghttp_set_header(request, http_hdr_Connection,"close");*//Prepare the connection*
ghttp_prepare(request);*// Process the request*
ghttp_process(request);*// Write out the body. Note that the body of the request may not be null terminated so we have to be careful of the length.*fwrite(ghttp_get_body(request), ghttp_get_body_len(request),1, stdout);*//Destroy the request. This closes any file descriptors that may be open and will free any memory associated with the request.*
ghttp_request_destroy(request);
## 艺搜参考
[http://lfs.linuxsir.org/htdocs/blfscvs/gnome/libghttp.html](http://lfs.linuxsir.org/htdocs/blfscvs/gnome/libghttp.html)
[http://oss.org.cn/ossdocs/gnu_linux/lfs/blfs-1.0/gnome/libghttp.html](http://oss.org.cn/ossdocs/gnu_linux/lfs/blfs-1.0/gnome/libghttp.html)
