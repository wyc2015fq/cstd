# libcurl 通过http协议下载文件并显示下载进度 - DoubleLi - 博客园






vc6 测试工程下载地址：[http://download.csdn.net/detail/mtour/8068053](http://download.csdn.net/detail/mtour/8068053)



代码如下：







**[cpp]**[view plain](http://blog.csdn.net/mtour/article/details/40375947#)[copy](http://blog.csdn.net/mtour/article/details/40375947#)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- size_t my_write_func(void *ptr, size_t size, size_t nmemb, FILE *stream)  
- {  
- return fwrite(ptr, size, nmemb, stream);  
- }   
- 
- int my_progress_func(char *progress_data,  
- double t, /* dltotal */  
- double d, /* dlnow */  
- double ultotal,  
- double ulnow)  
- {  
-   printf("%s %g / %g (%g %%)\n", progress_data, d, t, d*100.0/t);  
- return 0;  
- }  
- 
- int main(int argc, char **argv)  
- {  
-   CURL *curl;  
-   CURLcode res;  
- FILE *outfile;  
- char *url = "http://10.10.1.4/d/c00000000000039/2014-10-22/10-28-35.ps";  
- char *progress_data = "* ";  
- 
-   curl = curl_easy_init();  
- if(curl)  
-   {  
-     outfile = fopen("test.ps", "wb");  
- 
-     curl_easy_setopt(curl, CURLOPT_URL, url);  
-     curl_easy_setopt(curl, CURLOPT_WRITEDATA, outfile);  
-     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write_func);  
-     curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);  
-     curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, my_progress_func);  
-     curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, progress_data);  
- 
-     res = curl_easy_perform(curl);  
- 
-     fclose(outfile);  
- /* always cleanup */  
-     curl_easy_cleanup(curl);  
-   }  
- return 0;  
- }  










