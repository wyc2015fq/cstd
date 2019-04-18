# CGI 文件下载 - ljx0305的专栏 - CSDN博客
2013年07月17日 17:38:49[ljx0305](https://me.csdn.net/ljx0305)阅读数：6596
文件名：download.c 用交叉编译工具编译生成download
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>
#define MAX_FILE_LEN  (1024*30)
#define DOWNLOAD_FILE_PATH"/works/config/"
#define DOWNLOAD_FILE_NAME"SWConf.ini"
int main(){
FILE *fp;
char filebuf[MAX_FILE_LEN];
char cmd[512];
struct stat sb;
sprintf(cmd, "%s%s", DOWNLOAD_FILE_PATH, DOWNLOAD_FILE_NAME);
stat(cmd, &sb); //取待下载文件的大小
//输出HTTP头信息，输出附加下载文件、文件长度以及内容类型
printf("Content-Disposition:attachment;filename=%s", DOWNLOAD_FILE_NAME);
printf("\r\n"); 
printf("Content-Length:%d", sb.st_size);
printf("\r\n");
//printf("Content-Type:application/octet-stream %c%c", 13,10); (ascii:"13--\r", "10--\n") 与下一行等同
printf("Content-Type:application/octet-stream\r\n");
printf("\r\n");
sprintf(cmd, "%s%s", DOWNLOAD_FILE_PATH, DOWNLOAD_FILE_NAME);
if(fp=fopen(cmd, "r+b")){  
//成功打开文件，读取文件内容
do{
int rs = fread(filebuf, 1, sizeof(filebuf), fp);
fwrite(filebuf, rs, 1, stdout);
}while(!feof(fp));
fclose(fp);
}
return 1;
}
html中代码的一部分是：超链接。
<tr>
<th>文件下载：</th>
<td><a href="/cgi-bin/download">文件下载</a>
</td>
</tr>
html中代码的一部分是：button。
$('#swdownload').click(function(){
location.href="/cgi-bin/download";
});
<tr>
<th>文件下载：</th>
<td>
<input type="button" id="swdownload" value="下载">
</td>
</tr>
[http://hezhao2000.blog.163.com/blog/static/122436367201331742413578/](http://hezhao2000.blog.163.com/blog/static/122436367201331742413578/)
