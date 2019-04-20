# curl 以http方式读取文件并保存 - sandro_zhang的专栏 - CSDN博客
2011年10月14日 16:19:33[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：8724
以下为主函数代码：
```cpp
void main(){
    //initialization
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    
    if(curl){
        //define file for store downloaded file
        FILE* fp;
        fp = fopen( "c:/test.rar", "wb");
        string fileurl = "http://localhost/files/test.rar";
        curl_easy_setopt(curl, CURLOPT_URL, fileurl.c_str() );
        
        /* Define our callback to get called when there's data to be written */ 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlCallBackFileWrite);
        /* Set a pointer to our struct to pass to the callback */ 
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        
        //always close file
        fclose(fp);
        }
    }
    
    //always cleanup
    curl_easy_cleanup(curl);
}
```
以下为 curlCallBackFileWrite 回调处理：
```cpp
static size_t curlCallBackFileWrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
    int written = fwrite(buffer, size, nmemb , (FILE *)stream); 
    return written; 
}
```
完。
这当然也可以存储页面文件。
curl 的功能还有很多，比如FTP,SSL等，可以参考官方示例
链接：http://curl.haxx.se/libcurl/c/example.html
