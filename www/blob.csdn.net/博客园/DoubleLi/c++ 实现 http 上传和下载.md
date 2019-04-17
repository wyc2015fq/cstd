# c++ 实现 http 上传和下载 - DoubleLi - 博客园






代码下载地址:   [http://download.csdn.net/detail/mtour/8243527](http://download.csdn.net/detail/mtour/8243527)



      最近写了个程序需要用到http通讯，由于flash空间比较小，没有考虑 libcurl库，用c++封装了一个http类，实现了http  文件上传和下载







**[cpp]**[view plain](http://blog.csdn.net/mtour/article/details/41845129#)[copy](http://blog.csdn.net/mtour/article/details/41845129#)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- #include <stdio.h>    
- #include <unistd.h>    
- #include <string.h>    
- #include <net/if.h>    
- #include <arpa/inet.h>    
- #include <sys/ioctl.h>    
- #include "HttpClient.h"  
- 
- 
- 
- int main()  
- {  
-     CHttpClient httpclient;  
- 
- char* pResponse=new char[32*1024];  
-     memset(pResponse,0,32*1024);  
- 
- int nRet=httpclient.ConnectServer("127.0.0.1", 80);  
- 
- if (0!=nRet) {  
- return -1;  
-     }  
- 
-     nRet=httpclient.HttpGet("/archives/user/10000025/jbox/m6cfaa74922bd00/JssConfig.xml", pResponse);  
- 
- if (0!=nRet) {  
-         printf("http get failed\n");  
- return -1;  
-     }  
- 
-     printf("------------- split line --------------  \n");  
- 
- char* pTmp=strstr(pResponse, "\r\n\r\n");  
-     pTmp+=4;  
- 
-     printf("%s\n",pTmp);  
- 
- 
- char url[128]="/file/";   
- 
-     httpclient.HttpPostFile(url, "yourfile");  
- 
-     getchar();  
- //delete [] pResponse;  
- return 0;  
- }  






类 接口定义







**[cpp]**[view plain](http://blog.csdn.net/mtour/article/details/41845129#)[copy](http://blog.csdn.net/mtour/article/details/41845129#)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- #ifndef __Design__HttpClient__  
- #define __Design__HttpClient__  
- 
- #include <stdio.h>  
- #include "net/Net.h"  
- #include <errno.h>  
- #include <netdb.h>  
- #include <sys/types.h>  
- #include <netinet/in.h>  
- #include <arpa/inet.h>  
- #include <string.h>  
- #include <stdlib.h>  
- 
- 
- class CHttpClient {  
- public:  
- int ConnectServer(char* sHost,int nPort);  
- int DisconnetServer();  
- int HttpGet(char* sUrl,char* sResponse);  
- int HttpPostFile(char* sUrl,char* sFileName);  
- private:  
- char m_sHost[32];  
- char m_sHostIP[32];  
- int  m_nPort;  
-     CTcp m_tcp;  
- };  
- 
- #endif /* defined(__Design__HttpClient__) */  



实现









**[cpp]**[view plain](http://blog.csdn.net/mtour/article/details/41845129#)[copy](http://blog.csdn.net/mtour/article/details/41845129#)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- //  
- //  HttpClient.cpp  
- //  Design  
- //  
- //  Created by cwliu on 14-12-5.  
- //  Copyright (c) 2014-12-08  cwliu. All rights reserved.  
- //  
- 
- #include "HttpClient.h"  
- 
- int CHttpClient::ConnectServer(char* sHost,int nPort)  
- {  
- //gethostbyname  
- 
- struct hostent *h;  
- 
- if((h=gethostbyname(sHost))==NULL)  
-     {  
-         printf("gethostbyname failed\n");  
- return -1;  
-     }  
-     printf("HostName :%s\n",h->h_name);  
-     sprintf(m_sHostIP, "%s",inet_ntoa(*((struct in_addr *)h->h_addr)));  
-     printf("IP Address :%s\n",m_sHostIP);  
-     m_nPort=nPort;  
- 
- int nRet=m_tcp.Open(nPort, m_sHostIP);  
- if (0!=nRet) {  
-         printf("socket connect failed\n");  
- return -1;  
-     }  
- return 0;  
- }  
- int CHttpClient::DisconnetServer()  
- {  
-     m_tcp.Close();  
- return 0;  
- }  
- 
- int CHttpClient::HttpGet(char* sUrl,char* sResponse)  
- {  
- 
- char post[300],host[100],content_len[100];  
- char *lpbuf,*ptmp;  
- int len=0;  
-     lpbuf = NULL;  
- const char *header2="User-Agent: linux_http_client Http 1.0\r\nCache-Control: no-cache\r\nContent-Type: application/x-www-form-urlencoded\r\nAccept: */*\r\n";  
-     sprintf(post,"GET %s HTTP/1.0\r\n",sUrl);  
-     sprintf(host,"HOST: %s:%d\r\n",m_sHostIP,m_nPort);  
-     sprintf(content_len,"Content-Length: %d\r\n\r\n",1);  
-     len = strlen(post)+strlen(host)+strlen(header2)+strlen(content_len)+1;  
-     lpbuf = (char*)malloc(len);  
- if(lpbuf==NULL)  
-     {  
- return -1;  
-     }  
-     strcpy(lpbuf,post);  
-     strcat(lpbuf,host);  
-     strcat(lpbuf,header2);  
-     strcat(lpbuf,content_len);  
-     strcat(lpbuf," ");  
- 
-     m_tcp.Send((unsigned char*)lpbuf,len);  
- 
- int nRet=m_tcp.Recv();  
- if (nRet>0) {  
-         memcpy(sResponse,m_tcp.GetBuffer(), nRet);  
-         printf("%s\n",sResponse);  
-     }  
- 
- return 0;  
- }  
- 
- int CHttpClient::HttpPostFile(char* sUrl,char* sFileName)  
- {  
- // check file and read it  
- long  nFileLen=0;  
- 
- FILE* pFile=fopen(sFileName,"r");  
- if (!pFile) {  
-         printf("read file failed\n");  
- return -1;  
-     }  
- 
-     fseek(pFile, 0, SEEK_END);  
-     nFileLen=ftell(pFile);  
-     printf("File length is %ld\n", nFileLen);  
- 
- if (!nFileLen) {  
-         printf("file len is 0\n");  
- return -1;  
-     }  
- 
-     fseek(pFile, 0, SEEK_SET);  
- char* sFileText=new char[nFileLen+1];  
-     memset(sFileText, 0, nFileLen+1);  
- 
-     fread(sFileText, 1, nFileLen, pFile);  
- 
-     fclose(pFile);  
- 
- 
- char* pBody=new char[32*1024];  
-     memset(pBody,0,32*1024);  
- 
- char sContentDisp[128];  
-     sprintf(sContentDisp, "Content-Disposition: form-data;name=\"file\";filename=\"%s\"\r\n",sFileName);  
-     strcat(pBody,"-------------------------------7db372eb000e2\r\n");  
-     strcat(pBody, sContentDisp);  
-     strcat(pBody, "Content-Type: text/plain\r\n\r\n");      
-     strcat(pBody, sFileText);      
-     strcat(pBody, "\r\n-------------------------------7db372eb000e2--\r\n");  
- 
- 
- char post[300];  
-     sprintf(post,"POST %s HTTP/1.0\r\n",sUrl);  
- char header[1024];  
- char host[64];  
-     sprintf(host,"HOST: %s:%d\r\n",m_sHostIP,m_nPort);  
- 
- 
- char sContentLen[32];  
-     sprintf(sContentLen, "Content-Length: %ld\r\n",strlen(pBody)+1);  
- 
- // read file and caluate the file  
- 
- 
-     sprintf(header, "%s","Accept:*/*\r\n");      
-     strcat(header, host);      
-     strcat(header, "User-Agent: JboxHttpClient\r\n");  
-     strcat(header, sContentLen);  
-     strcat(header,"Expect: 100-continue\r\n");  
-     strcat(header, "Content-Type:multipart/form-data;boundary=-----------------------------7db372eb000e2\r\n\r\n");  
- 
- char* pBuf=new char[64*1024];  
-     memset(pBuf, 0, 64*1024);  
-     strcat(pBuf, post);  
-     strcat(pBuf, header);   
-     strcat(pBuf,pBody);  
- 
- 
-     printf("%s\n",pBuf);  
- 
-     m_tcp.Send((unsigned char*)pBuf, strlen(pBuf)+1);  
- 
- char sResponse[1024];  
-     memset(sResponse, 0, 1024);  
- 
- int nRet=m_tcp.Recv();  
- if (nRet>0) {  
-         memcpy(sResponse, m_tcp.GetBuffer(), nRet);  
-         printf("%s\n",sResponse);  
- 
- if (strstr(sResponse,"200"))  
-         {  
- delete[] pBody;  
- delete[] pBuf;  
-             printf("post file success \n");  
- return 0;  
-         }  
- else  
-         {  
-             printf("post file failed \n");  
-         }         
-     }     
- delete[] pBody;  
- delete[] pBuf;  
- return -1;  
- }  










