
# 再议libcurl编程 - 高科的专栏 - CSDN博客

2013年12月24日 15:12:35[高科](https://me.csdn.net/pbymw8iwm)阅读数：10090


那是2年前用libcurl了，我肯定好久不用的知识，放置久了就会遗忘，现在我又重拾起这个知识点，重头再来，至于前面的基础知识，可以参考我的  http://blog.csdn.net/pbymw8iwm/article/details/6675754
假设你要获取URL所表示的远程主机上的资源。你需要写一段程序用来完成数据传输，你可能希望直接保存接收到的数据而不是简单的在输出窗口中打印它们。所以，你必须首先写一个回调函数用来保存接收到的数据。回调函数的原型如下：
size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);
可以使用下面的语句来注册回调函数，回调函数将会在接收到数据的时候被调用：
curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_data);
可以给回调函数提供一个自定义参数，libcurl不处理该参数，只是简单的传递：
curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &internal_struct);
如果你没有通过CURLOPT_WRITEFUNCTION属性给easy handle设置回调函数，libcurl会提供一个默认的回调函数，它只是简单的将接收到的数据打印到标准输出。你也可以通过 CURLOPT_WRITEDATA属性给默认回调函数传递一个已经打开的文件指针，用于将数据输出到文件里。
下面是一些平台相关的注意点。在一些平台上，libcurl不能直接操作由应用程序打开的文件。所以，如果使用默认的回调函数，同时通过 CURLOPT_WRITEDATA属性给easy handle传递一个文件指针，应用程序可能会执行失败。如果你希望自己的程序能跑在任何系统上，你必须避免出现这种情况。
如果以win32动态连接库的形式来使用libcurl，在设置CURLOPT_WRITEDATA属性时，你必须同时 使用CURLOPT_WRITEFUNCTION来注册回调函数。否则程序会执行失败（笔者尝试只传递一个打开的文件指针而不显式设置回调函数，程序并没有崩溃。可能是我使用的方式不正确。）。
当然，libcurl还支持许多其他的属性，在接下来的篇幅里，你将会逐步地接触到它们。调用下面的函数，将执行真正的数据通信：
success = curl_easy_perform(easy_handle);
curl_easy_perfrom将连接到远程主机，执行必要的命令，并接收数据。当接收到数据时，先前设置的回调函数将被调用。libcurl可能一次只接收到1字节的数据，也可能接收到好几K的数据，libcurl会尽可能多、及时的将数据传递给回调函数。回调函数返回接收的数据长度。如果回调函数返回的数据长度与传递给它的长度不一致（即返回长度 != size * nmemb），libcurl将会终止操作，并返回一个错误代码。
当数据传递结束的时候，curl_easy_perform将返回一个代码表示操作成功或失败。如果需要获取更多有关通信细节的信息，你可以设置 CURLOPT_ERRORBUFFER属性，让libcurl缓存许多可读的错误信息。
easy handle在完成一次数据通信之后可以被重用。这里非常建议你重用一个已经存在的easy handle。如果在完成数据传输之后，你创建另一个easy handle来执行其他的数据通信，libcurl在内部会尝试着重用上一次创建的连接。
对于有些协议，下载文件可能包括许多复杂的子过程：日志记录、设置传输模式、选择当前文件夹，最后下载文件数据。使用libcurl，你不需要关心这一切，你只需简单地提供一个URL，libcurl会给你做剩余所有的工作。
多线程问题
首先一个基本原则就是：绝对不应该在线程之间共享同一个libcurl handle，不管是easy handle还是multi handle（将在下文中介绍）。一个线程每次只能使用一个handle。
libcurl是线程安全的，但有两点例外：信号(signals)和SSL/TLS handler。 信号用于超时失效名字解析(timing out name resolves)。libcurl依赖其他的库来支持SSL/STL，所以用多线程的方式访问HTTPS或FTPS的URL时，应该满足这些库对多线程操作的一些要求。
libcurl提供协议无关的方式进行数据传输。所以上传一个文件到FTP服务器，跟向HTTP服务器提交一个PUT请求的操作方式是类似的：
1. 创建easy handle或者重用先前创建的easy handle。
2. 设置CURLOPT_URL属性。
3. 编写回调函数。在执行上传的时候，libcurl通过回调函数读取要上传的数据。（如果要从远程服务器下载数据，可以通过回调来保存接收到的数据。）回调函数的原型如下：
size_t function(char *bufptr, size_t size, size_t nitems, void *userp);
bufptr指针表示缓冲区，用于保存要上传的数据，size * nitems是缓冲区数据的长度，userp是一个用户自定义指针，libcurl不对该指针作任何操作，它只是简单的传递该指针。可以使用该指针在应用程序与libcurl之间传递信息。
4. 注册回调函数，设置自定义指针。语法如下：
// 注册回调函数
curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, read_function);
// 设置自定义指针
curl_easy_setopt(easy_handle, CURLOPT_READDATA, &filedata);
5. 告诉libcurl，执行的是上传操作。
curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
有些协议在没有预先知道上传文件大小的情况下，可能无法正确判断上传是否结束，所以最好预先使用CURLOPT_INFILESIZE_LARGE属性：告诉它要上传文件的大小：
/* in this example, file_size must be an curl_off_t variable */
curl_easy_setopt(easy_handle, CURLOPT_INFILESIZE_LARGE, file_size);
下面的这个例子演示了如何获取网页源码，将其保存到本地文件，并同时将获取的源码输出到控制台上。
/**
* @brief libcurl接收到数据时的回调函数
*
* 将接收到的数据保存到本地文件中，同时显示在控制台上。
*
* @param [in] buffer 接收到的数据所在缓冲区
* @param [in] size 数据长度
* @param [in] nmemb 数据片数量
* @param [in/out] 用户自定义指针
* @return 获取的数据长度
*/
size_t process_data(void *buffer, size_t size, size_t nmemb, void *user_p)
{
FILE *fp = (FILE *)user_p;
size_t return_size = fwrite(buffer, size, nmemb, fp);
cout << (char *)buffer << endl; return return_size;
}
int main(int argc, char **argv)
{
// 初始化libcurl
CURLcode return_code;
return_code = curl_global_init(CURL_GLOBAL_WIN32);
if (CURLE_OK != return_code)
{
cerr << "init libcurl failed." << endl;
return -1;
}
// 获取easy handle
CURL *easy_handle = curl_easy_init();
if (NULL == easy_handle)
{
cerr << "get a easy handle failed." << endl;
curl_global_cleanup(); return -1;
}
FILE *fp = fopen("data.html", "ab+"); //
// 设置easy handle属性
curl_easy_setopt(easy_handle, CURLOPT_URL,[http://blog.csdn.net/JGood](http://blog.csdn.net/JGood));
curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, &process_data);
curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, fp);
// 执行数据请求
curl_easy_perform(easy_handle);
// 释放资源
fclose(fp);
curl_easy_cleanup(easy_handle);
curl_global_cleanup();
return 0;
}



