# libcurl 上传文件至 web服务器 - DoubleLi - 博客园






测试环境搭建， 使用 wamp server （windows下的 apache+[MySQL](http://lib.csdn.net/base/14)+php） 



libcurl vc6 工程代码  下载地址：  [http://download.csdn.net/detail/mtour/8060775](http://download.csdn.net/detail/mtour/8060775)



处理上传文件 php  代码







**[php]**[view plain](http://blog.csdn.net/mtour/article/details/40304925#)[copy](http://blog.csdn.net/mtour/article/details/40304925#)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- <?php  
- 
- if ($_FILES["file"]["error"] > 0)  
-     {  
- echo "Return Code: " . $_FILES["file"]["error"] . "<br />";  
-     }  
- else  
-     {  
- echo "Upload: " . $_FILES["file"]["name"] . "<br />";  
- echo "Type: " . $_FILES["file"]["type"] . "<br />";  
- echo "Size: " . ($_FILES["file"]["size"] / 1024) . " Kb<br />";  
- echo "Temp file: " . $_FILES["file"]["tmp_name"] . "<br />";  
- 
- if (file_exists("./" . $_FILES["file"]["name"]))  
-       {  
- echo $_FILES["file"]["name"] . " already exists. ";  
-       }  
- else  
-       {  
-       move_uploaded_file($_FILES["file"]["tmp_name"],  
- "./" . $_FILES["file"]["name"]);  
- echo "Stored in: " . "/" . $_FILES["file"]["name"];  
-       }  
-     }  
- ?>  



libcurl  代码









**[cpp]**[view plain](http://blog.csdn.net/mtour/article/details/40304925#)[copy](http://blog.csdn.net/mtour/article/details/40304925#)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- // libCur1.cpp : Defines the entry point for the console application.  
- //  
- 
- #include "stdafx.h"  
- #include <stdio.h>  
- #include "curl/curl.h"  
- 
- #define MAX_BUFF_LEN 1048576 /*1M*/  
- #define POST_URL "http://10.10.1.4/d/upload.php"  
- 
- 
- int get_file_size(char *filename)  
- {  
- FILE* fp = NULL;  
- int nFileLen = 0;  
- 
-     fp = fopen(filename, "rb");  
- 
- if (fp == NULL)  
-     {  
- return 0;  
-     }  
- 
-     fseek(fp,0,SEEK_END); //定位到文件末   
-     nFileLen = ftell(fp); //文件长度  
- return nFileLen;  
- }  
- 
- int http_post_file(const char *url, const char *filename)  
- {  
-     CURL *curl = NULL;  
-        CURLcode res;  
- 
- struct curl_httppost *post=NULL;  
- struct curl_httppost *last=NULL;  
- struct curl_slist *headerlist=NULL;  
- 
- if(filename == NULL || url == NULL)  
- return -1;  
- 
-     printf("URL: %s\n", url);  
-     printf("filename: %s\n", filename);  
- 
- /* Add simple file section */  
- if( curl_formadd(&post, &last, CURLFORM_COPYNAME, "file",  
-                CURLFORM_FILE, filename, CURLFORM_END) != 0)  
-     {  
-         fprintf(stderr, "curl_formadd error.\n");  
- return -1;  
-     }  
- 
- /* Fill in the submit field too, even if this is rarely needed */  
-       curl_formadd(&post, &last,  
-                CURLFORM_COPYNAME, "submit",  
-                CURLFORM_COPYCONTENTS, "OK",  
-                CURLFORM_END);  
- 
- //curl_global_init(CURL_GLOBAL_ALL);  
-     curl = curl_easy_init();  
- if(curl == NULL)  
-     {  
-         fprintf(stderr, "curl_easy_init() error.\n");  
-         curl_formfree(post);  
- return -1;  
-     }  
- 
-     curl_easy_setopt(curl, CURLOPT_HEADER, 0);  
-     curl_easy_setopt(curl, CURLOPT_URL, url); /*Set URL*/  
-     curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);  
- int timeout = 5;  
-     curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);  
-     curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);  
-     curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1);  
- 
-     res = curl_easy_perform(curl);  
- if(res != CURLE_OK)  
-     {  
-         fprintf(stderr, "curl_easy_perform[%d] error.\n", res);  
-         curl_formfree(post);  
- return -1;  
-     }  
- 
-     curl_easy_cleanup(curl);      
- 
- return 0;  
- }  
- 
- int main()  
- {     
- 
- char sFilePath[128]="d:\\20130828131421113.jpg";  
- 
- //Check File Size  
- if(get_file_size(sFilePath) >= MAX_BUFF_LEN)  
-     {  
-         fprintf(stderr, "File Size is Big!\n");  
- return -1;  
-     }  
- 
- //POST File  
-     http_post_file(POST_URL, sFilePath);  
- 
-     getchar();  
- 
- return 0;  
- }  
- 










