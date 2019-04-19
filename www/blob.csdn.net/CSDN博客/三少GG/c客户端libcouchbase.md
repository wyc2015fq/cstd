# c客户端libcouchbase - 三少GG - CSDN博客
2014年08月01日 17:16:23[三少GG](https://me.csdn.net/scut1135)阅读数：1783
C版本下载：
http://packages.couchbase.com/clients/c/index.html
api文档：
[http://www.couchbase.com/autodocs/](http://www.couchbase.com/autodocs/)[http://www.couchbase.com/autodocs/couchbase-c-client-2.3.2/index.html](http://www.couchbase.com/autodocs/couchbase-c-client-2.3.2/index.html)
C++版本（慎用）
https://github.com/couchbaselabs/lcb-cxx/

### [Couchbase] Couchbase 介紹 - Client - 1 - C&C++ Version
Couchbase C&C++ Version
這邊介紹如何用C&C++與Couchbase的"簡易溝通" (複雜的我後來都用PHP比較方便)
官網C Library:[http://www.couchbase.com/develop/c/current](http://www.couchbase.com/develop/c/current)
這邊附註一下, v1.8跟v2.0有一段落差, 我會稍微補充一點點不一樣的地方
比較完整個差異可以看這個人的Blog - 應該是Couchbase的工程師
[http://trondn.blogspot.tw/2012/08/libcouchbase-overhauling.html](http://trondn.blogspot.tw/2012/08/libcouchbase-overhauling.html)
1. 首先要先安裝Couchbase Library, 這邊我直接用Source Archive安裝
用相對平台的rpm or deb也行, 官網其實都有很明確地安裝流程了
除了Source Archive, 如果是Source Archive安裝也很簡易:
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
```bash
tar
```
```bash
zcvf
 libcouchbase-2.0.3.
```
```bash
tar
```
```bash
.gz
```
```bash
cd
```
```bash
libcouchbase-2.0.3
```
```bash
.
```
```bash
/configure
```
```bash
sudo
```
```bash
make
```
```bash
&&
```
```bash
make
```
```bash
install
```
```
安裝好了之後就可以利用C&C++來溝通Couchbase
這邊我先拿官方網站來作範例
首先是要先把要使用的Couchbase library 加進去 #include <libcouchbase/couchbase.h>
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
```
#include
 <libcouchbase/couchbase.h>
```
```
#include
 <stdlib.h>
```
```
#include
 <stdio.h>
```
```
接著開始我們要先設定要連Couchbase的Server IP:Port & Bucket Name
可以的話最好連要連Server的帳號密碼也一起設定
先用lcb_create_st來宣告一個Couchbase I/O的instance -> create_options
可以用來設定要連接的Couchbase Server IP/Port, Bucket, Account/Password
宣告一個lcb_t的instance, 之後可以用來操作Couchbase上的服務
※ 新舊版本在宣告的命名上有點差異, 以前叫做libcouchbase名稱都改成lcb
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
`struct`
```
lcb_create_st
 create_options;
```
```
lcb_t
 instance;
```
`memset`
```
(&create_options,
 0,
```
`sizeof``(create_options));`
```
create_options.v.v0.host
 =
```
`"myserver:8091"``;`
```
create_options.v.v0.user
 =
```
`"mybucket"``;`
```
create_options.v.v0.passwd
 =
```
`"secret"``;`
```
create_options.v.v0.bucket
 =
```
`"mybucket"``;`
```
接著call lcb_create, 先建立一個Couchbase Server的instance, 失敗會有相對處理
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
```
err
 = lcb_create(&instance, &create_options);
```
`if`
```
(err
 != LCB_SUCCESS) {
```
`    ``fprintf``(stderr,`
```
"Failed
 to create libcouchbase instance: %s\n"
```
```
,
 lcb_strerror(NULL, err));
```
`    ``return``1;`
`}`
```
再下去之前先講一下另外一個東西 Callback Function
Couchbase目前我看他範例, 幾乎都會針對錯誤或者是成功的動作, 做 Callback Function處理
那當然也是方便將結果可以回存到你想要儲存的variable, 在他範例這邊是 cookie variable
官方範例做了兩個 Function
一個是對Couchbase Server溝通時錯誤處理的Error Callback Function
基本上也只是印出錯誤結束程式
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
`static``void`
```
error_callback(lcb_t instance, lcb_error_t err,
```
`const``char`
```
*errinfo)
```
`{`
`    ``fprintf``(stderr,`
```
"Error
 %s: %s"
```
```
,
 lcb_strerror(instance, err), errinfo ? errinfo :
```
`""``);`
`    ``exit``(EXIT_FAILURE);`
`}`
```
另外一個是Get Callback Function, 顧名思義, 他是針對去取data所作的Callback Function
這就其實他還有再細分狀態, 若是失敗他在Get Callback function就會先印出錯誤訊息了
這邊他成功並沒做啥太多事情, 只是印出取到的結果
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
`static``void`
```
get_callback(lcb_t instance,
```
`                         ``const``void`
```
*cookie,
```
`                         `
```
lcb_error_t
 error,
```
`                         ``const`
```
lcb_get_resp_t
 *resp)
```
`{`
`    ``if`
```
(error
 != LCB_SUCCESS) {
```
`        ``fprintf``(stderr,`
```
"Failed
 to retrieve \""
```
`);`
`        ``fwrite`
```
(resp->v.v0.key,
 1, resp->v.v0.nkey, stderr);
```
`        ``fprintf``(stderr,`
```
"\":
 %s\n"
```
```
,
 lcb_strerror(instance, error));
```
`    ``}``else``{`
`        ``fprintf``(stderr,`
```
"Data
 for key: \""
```
`);`
`        ``fwrite`
```
(resp->v.v0.key,
 1, resp->v.v0.nkey, stderr);
```
`        ``fprintf``(stderr,`
```
"\"
 is : "
```
`);`
`        ``fwrite`
```
(resp->v.v0.bytes,
 1, resp->v.v0.nbytes, stderr);
```
`    ``}`
`}`
```
首先要記住, Callback function都要用static宣告, 後面會稍微提一下multithread會出的狀況
那要讓程式知道Callback function要先設定
這邊分成兩種, get成功跟set失敗的Callback Function
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
```
lcb_set_error_callback(instance,
 error_callback);
```
```
//
 設定動作失敗 Callback Function
```
```
lcb_set_get_callback(instance,
 get_callback);
```
```
//
 設定動作成功 Callback Function
```
```
接著要連線到目標機器上了
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
`if`
```
((err
 = lcb_connect(instance)) != LCB_SUCCESS) {
```
`        ``fprintf``(stderr,`
```
"Failed
 to initiate connect: %s\n"
```
`,`
`                `
```
lcb_strerror(NULL,
 err));
```
`        ``return``1;`
`    ``}`
`lcb_wait(instance);`
```
這邊有個有趣的東西, lcb_wai(instance), 其實不管做任何的lcb指令
都要呼叫lcb_wait才會真的去運作, 運作完結束了, 才會離開這行指令繼續往下做
這邊的lcb_connect的回傳, 只是確定instance還正常, 指令有沒有下錯之類的
加上告知instance, 等一下要做connect, 然後call lcb_wait才真的會去connect
所以後續的不管是存data, 取data, 連線等, 都要記得呼叫 lcb_wait(instance)
再來就是要取data了, 這邊會看到一個lcb_get_cmd_t, 用來存放你要query的內容
流程是: key是foo, nkey放長度3, 然後用lcb_get去query結果並做偵錯(err)
然後記得放上lcb_wait去實際運作
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
```
lcb_get_cmd_t
 cmd;
```
`const`
```
lcb_get_cmd_t
 *
```
`const`
```
commands[1]
 = { &cmd };
```
`memset`
```
(&cmd,
 0,
```
`sizeof``(cmd));`
```
cmd.v.v0.key
 =
```
`"foo"``;`
```
cmd.v.v0.nkey
 = 3;
```
```
err
 = lcb_get(instance, NULL, 1, commands);
```
`if`
```
(err
 != LCB_SUCCESS) {
```
`    ``fprintf``(stderr,`
```
"Failed
 to get: %s\n"
```
```
,
 lcb_strerror(NULL, err));
```
`    ``return``1;`
`}`
`lcb_wait(instance);`
```
你會發現, 只要是設定字串的行為, 他都有特別用一個struct來接收
不管是設定Couchbase Server的create_option 或是 Query的 command
這是在上一版本沒有的設定, V2.0版將參數都做了結構上的調整
原因我上面提及的工程師網誌有稍微解釋, 有binary compatibility之類的原因
- 另外一件事情, 前面有提到Callback function因為是static, 所以如果你是用multithread來query資料, 沒做防護的話很容易多個thread都在跑Callback, 程式就會dump一堆error給你看. 所以如果程式不複雜, 在做lcb_get ~ lcb_wait前後記得加上pthread_mutex_lock pthread_mutex_unlock即可, 不過這還是要看個人需求決定
編譯也相當簡單, 只要確定安裝Library完畢, 編譯時記得加上-lcouchbase即可
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
```bash
gcc
 example.c -lcouchbase -o example
```
```
下面是完整的Code, 取自官方網站:[http://www.couchbase.com/develop/c/current](http://www.couchbase.com/develop/c/current)
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
```
#include
 <libcouchbase/couchbase.h>
```
```
#include
 <stdlib.h>
```
```
#include
 <stdio.h>
```
`static``void`
```
error_callback(lcb_t instance, lcb_error_t err,
```
`const``char`
```
*errinfo)
```
`{`
`    ``fprintf``(stderr,`
```
"Error
 %s: %s"
```
```
,
 lcb_strerror(instance, err),
```
`            `
```
errinfo
 ? errinfo :
```
`""``);`
`    ``exit``(EXIT_FAILURE);`
`}`
`static``void`
```
get_callback(lcb_t instance,
```
`                         ``const``void`
```
*cookie,
```
`                         `
```
lcb_error_t
 error,
```
`                         ``const`
```
lcb_get_resp_t
 *resp)
```
`{`
`    ``if`
```
(error
 != LCB_SUCCESS) {
```
`        ``fprintf``(stderr,`
```
"Failed
 to retrieve \""
```
`);`
`        ``fwrite`
```
(resp->v.v0.key,
 1, resp->v.v0.nkey, stderr);
```
`        ``fprintf``(stderr,`
```
"\":
 %s\n"
```
```
,
 lcb_strerror(instance, error));
```
`    ``}``else``{`
`        ``fprintf``(stderr,`
```
"Data
 for key: \""
```
`);`
`        ``fwrite`
```
(resp->v.v0.key,
 1, resp->v.v0.nkey, stderr);
```
`        ``fprintf``(stderr,`
```
"\"
 is : "
```
`);`
`        ``fwrite`
```
(resp->v.v0.bytes,
 1, resp->v.v0.nbytes, stderr);
```
`    ``}`
`}`
`int``main(``void``)`
`{`
`    ``struct`
```
lcb_create_st
 create_options;
```
`    `
```
lcb_t
 instance;
```
`    `
```
lcb_error_t
 err;
```
`    ``memset`
```
(&create_options,
 0,
```
`sizeof``(create_options));`
`    `
```
create_options.v.v0.host
 =
```
`"myserver:8091"``;`
`    `
```
create_options.v.v0.user
 =
```
`"mybucket"``;`
`    `
```
create_options.v.v0.passwd
 =
```
`"secret"``;`
`    `
```
create_options.v.v0.bucket
 =
```
`"mybucket"``;`
`    `
```
err
 = lcb_create(&instance, &create_options);
```
`    ``if`
```
(err
 != LCB_SUCCESS) {
```
`        ``fprintf``(stderr,`
```
"Failed
 to create libcouchbase instance: %s\n"
```
`,`
`                `
```
lcb_strerror(NULL,
 err));
```
`        ``return``1;`
`    ``}`
`    `
```
/*
 Set up the handler to catch all errors! */
```
`    `
```
lcb_set_error_callback(instance,
 error_callback);
```
`    ``/*`
`     `
```
*
 Initiate the connect sequence in libcouchbase
```
`     ``*/`
`    ``if`
```
((err
 = lcb_connect(instance)) != LCB_SUCCESS) {
```
`        ``fprintf``(stderr,`
```
"Failed
 to initiate connect: %s\n"
```
`,`
`                `
```
lcb_strerror(NULL,
 err));
```
`        ``return``1;`
`    ``}`
`    `
```
/*
 Run the event loop and wait until we've connected */
```
`    ``lcb_wait(instance);`
`    ``/*`
`     `
```
*
 Set up a callback for our get requests
```
`     ``*/`
`    `
```
lcb_set_get_callback(instance,
 get_callback);
```
`    `
```
lcb_get_cmd_t
 cmd;
```
`    ``const`
```
lcb_get_cmd_t
 *
```
`const`
```
commands[1]
 = { &cmd };
```
`    ``memset`
```
(&cmd,
 0,
```
`sizeof``(cmd));`
`    `
```
cmd.v.v0.key
 =
```
`"foo"``;`
`    `
```
cmd.v.v0.nkey
 = 3;
```
`    `
```
err
 = lcb_get(instance, NULL, 1, commands);
```
`    ``if`
```
(err
 != LCB_SUCCESS) {
```
`        ``fprintf``(stderr,`
```
"Failed
 to get: %s\n"
```
`,`
`                `
```
lcb_strerror(NULL,
 err));
```
`        ``return``1;`
`    ``}`
`    ``lcb_wait(instance);`
`    ``lcb_destroy(instance);`
`    ``exit``(EXIT_SUCCESS);`
`}`
```
其實還有不少API, 但是文件我覺得因為v2.0才出來沒有很久, 官方文件不算是相當完整
要用C&C++來溝通, 最好要有查資料查半天找不到用法的心理準備
不然就是上他們forum去問問題, 基本上都會有人回答
下一篇會簡單介紹PHP的用法, PHP相對C來講, 真的是簡單好用很多!!
張貼者： [Tu
 Han-Hsing](https://plus.google.com/114036893041400132391)於 [下午9:00](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html)
[以電子郵件傳送這篇文章](http://www.blogger.com/share-post.g?blogID=1099001297101871030&postID=2909350560606465438&target=email)[BlogThis！](http://www.blogger.com/share-post.g?blogID=1099001297101871030&postID=2909350560606465438&target=blog)[分享至
 Twitter](http://www.blogger.com/share-post.g?blogID=1099001297101871030&postID=2909350560606465438&target=twitter)[分享至
 Facebook](http://www.blogger.com/share-post.g?blogID=1099001297101871030&postID=2909350560606465438&target=facebook)[分享到
 Pinterest](http://www.blogger.com/share-post.g?blogID=1099001297101871030&postID=2909350560606465438&target=pinterest)
標籤： [C](http://hhtucode.blogspot.com/search/label/C), [C++](http://hhtucode.blogspot.com/search/label/C%2B%2B), [Couchbase](http://hhtucode.blogspot.com/search/label/Couchbase), [NoSQL](http://hhtucode.blogspot.com/search/label/NoSQL), [Tutorial](http://hhtucode.blogspot.com/search/label/Tutorial)



## 2013年2月26日星期二
### [Couchbase] Couchbase 介紹 - Client - 1 - C&C++ Version
Couchbase C&C++ Version
這邊介紹如何用C&C++與Couchbase的"簡易溝通" (複雜的我後來都用PHP比較方便)
官網C Library:[http://www.couchbase.com/develop/c/current](http://www.couchbase.com/develop/c/current)
這邊附註一下, v1.8跟v2.0有一段落差, 我會稍微補充一點點不一樣的地方
比較完整個差異可以看這個人的Blog - 應該是Couchbase的工程師
[http://trondn.blogspot.tw/2012/08/libcouchbase-overhauling.html](http://trondn.blogspot.tw/2012/08/libcouchbase-overhauling.html)
1. 首先要先安裝Couchbase Library, 這邊我直接用Source Archive安裝
用相對平台的rpm or deb也行, 官網其實都有很明確地安裝流程了
除了Source Archive, 如果是Source Archive安裝也很簡易:
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
```bash
tar
```
```bash
zcvf
 libcouchbase-2.0.3.
```
```bash
tar
```
```bash
.gz
```
```bash
cd
```
```bash
libcouchbase-2.0.3
```
```bash
.
```
```bash
/configure
```
```bash
sudo
```
```bash
make
```
```bash
&&
```
```bash
make
```
```bash
install
```
```
安裝好了之後就可以利用C&C++來溝通Couchbase
這邊我先拿官方網站來作範例
首先是要先把要使用的Couchbase library 加進去 #include <libcouchbase/couchbase.h>
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
```
#include
 <libcouchbase/couchbase.h>
```
```
#include
 <stdlib.h>
```
```
#include
 <stdio.h>
```
```
接著開始我們要先設定要連Couchbase的Server IP:Port & Bucket Name
可以的話最好連要連Server的帳號密碼也一起設定
先用lcb_create_st來宣告一個Couchbase I/O的instance -> create_options
可以用來設定要連接的Couchbase Server IP/Port, Bucket, Account/Password
宣告一個lcb_t的instance, 之後可以用來操作Couchbase上的服務
※ 新舊版本在宣告的命名上有點差異, 以前叫做libcouchbase名稱都改成lcb
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
`struct`
```
lcb_create_st
 create_options;
```
```
lcb_t
 instance;
```
`memset`
```
(&create_options,
 0,
```
`sizeof``(create_options));`
```
create_options.v.v0.host
 =
```
`"myserver:8091"``;`
```
create_options.v.v0.user
 =
```
`"mybucket"``;`
```
create_options.v.v0.passwd
 =
```
`"secret"``;`
```
create_options.v.v0.bucket
 =
```
`"mybucket"``;`
```
接著call lcb_create, 先建立一個Couchbase Server的instance, 失敗會有相對處理
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
```
err
 = lcb_create(&instance, &create_options);
```
`if`
```
(err
 != LCB_SUCCESS) {
```
`    ``fprintf``(stderr,`
```
"Failed
 to create libcouchbase instance: %s\n"
```
```
,
 lcb_strerror(NULL, err));
```
`    ``return``1;`
`}`
```
再下去之前先講一下另外一個東西 Callback Function
Couchbase目前我看他範例, 幾乎都會針對錯誤或者是成功的動作, 做 Callback Function處理
那當然也是方便將結果可以回存到你想要儲存的variable, 在他範例這邊是 cookie variable
官方範例做了兩個 Function
一個是對Couchbase Server溝通時錯誤處理的Error Callback Function
基本上也只是印出錯誤結束程式
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
`static``void`
```
error_callback(lcb_t instance, lcb_error_t err,
```
`const``char`
```
*errinfo)
```
`{`
`    ``fprintf``(stderr,`
```
"Error
 %s: %s"
```
```
,
 lcb_strerror(instance, err), errinfo ? errinfo :
```
`""``);`
`    ``exit``(EXIT_FAILURE);`
`}`
```
另外一個是Get Callback Function, 顧名思義, 他是針對去取data所作的Callback Function
這就其實他還有再細分狀態, 若是失敗他在Get Callback function就會先印出錯誤訊息了
這邊他成功並沒做啥太多事情, 只是印出取到的結果
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
`static``void`
```
get_callback(lcb_t instance,
```
`                         ``const``void`
```
*cookie,
```
`                         `
```
lcb_error_t
 error,
```
`                         ``const`
```
lcb_get_resp_t
 *resp)
```
`{`
`    ``if`
```
(error
 != LCB_SUCCESS) {
```
`        ``fprintf``(stderr,`
```
"Failed
 to retrieve \""
```
`);`
`        ``fwrite`
```
(resp->v.v0.key,
 1, resp->v.v0.nkey, stderr);
```
`        ``fprintf``(stderr,`
```
"\":
 %s\n"
```
```
,
 lcb_strerror(instance, error));
```
`    ``}``else``{`
`        ``fprintf``(stderr,`
```
"Data
 for key: \""
```
`);`
`        ``fwrite`
```
(resp->v.v0.key,
 1, resp->v.v0.nkey, stderr);
```
`        ``fprintf``(stderr,`
```
"\"
 is : "
```
`);`
`        ``fwrite`
```
(resp->v.v0.bytes,
 1, resp->v.v0.nbytes, stderr);
```
`    ``}`
`}`
```
首先要記住, Callback function都要用static宣告, 後面會稍微提一下multithread會出的狀況
那要讓程式知道Callback function要先設定
這邊分成兩種, get成功跟set失敗的Callback Function
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
```
lcb_set_error_callback(instance,
 error_callback);
```
```
//
 設定動作失敗 Callback Function
```
```
lcb_set_get_callback(instance,
 get_callback);
```
```
//
 設定動作成功 Callback Function
```
```
接著要連線到目標機器上了
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
`if`
```
((err
 = lcb_connect(instance)) != LCB_SUCCESS) {
```
`        ``fprintf``(stderr,`
```
"Failed
 to initiate connect: %s\n"
```
`,`
`                `
```
lcb_strerror(NULL,
 err));
```
`        ``return``1;`
`    ``}`
`lcb_wait(instance);`
```
這邊有個有趣的東西, lcb_wai(instance), 其實不管做任何的lcb指令
都要呼叫lcb_wait才會真的去運作, 運作完結束了, 才會離開這行指令繼續往下做
這邊的lcb_connect的回傳, 只是確定instance還正常, 指令有沒有下錯之類的
加上告知instance, 等一下要做connect, 然後call lcb_wait才真的會去connect
所以後續的不管是存data, 取data, 連線等, 都要記得呼叫 lcb_wait(instance)
再來就是要取data了, 這邊會看到一個lcb_get_cmd_t, 用來存放你要query的內容
流程是: key是foo, nkey放長度3, 然後用lcb_get去query結果並做偵錯(err)
然後記得放上lcb_wait去實際運作
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
```
lcb_get_cmd_t
 cmd;
```
`const`
```
lcb_get_cmd_t
 *
```
`const`
```
commands[1]
 = { &cmd };
```
`memset`
```
(&cmd,
 0,
```
`sizeof``(cmd));`
```
cmd.v.v0.key
 =
```
`"foo"``;`
```
cmd.v.v0.nkey
 = 3;
```
```
err
 = lcb_get(instance, NULL, 1, commands);
```
`if`
```
(err
 != LCB_SUCCESS) {
```
`    ``fprintf``(stderr,`
```
"Failed
 to get: %s\n"
```
```
,
 lcb_strerror(NULL, err));
```
`    ``return``1;`
`}`
`lcb_wait(instance);`
```
你會發現, 只要是設定字串的行為, 他都有特別用一個struct來接收
不管是設定Couchbase Server的create_option 或是 Query的 command
這是在上一版本沒有的設定, V2.0版將參數都做了結構上的調整
原因我上面提及的工程師網誌有稍微解釋, 有binary compatibility之類的原因
- 另外一件事情, 前面有提到Callback function因為是static, 所以如果你是用multithread來query資料, 沒做防護的話很容易多個thread都在跑Callback, 程式就會dump一堆error給你看. 所以如果程式不複雜, 在做lcb_get ~ lcb_wait前後記得加上pthread_mutex_lock pthread_mutex_unlock即可, 不過這還是要看個人需求決定
編譯也相當簡單, 只要確定安裝Library完畢, 編譯時記得加上-lcouchbase即可
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
```bash
gcc
 example.c -lcouchbase -o example
```
```
下面是完整的Code, 取自官方網站:[http://www.couchbase.com/develop/c/current](http://www.couchbase.com/develop/c/current)
[?](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html#)
```
```
#include
 <libcouchbase/couchbase.h>
```
```
#include
 <stdlib.h>
```
```
#include
 <stdio.h>
```
`static``void`
```
error_callback(lcb_t instance, lcb_error_t err,
```
`const``char`
```
*errinfo)
```
`{`
`    ``fprintf``(stderr,`
```
"Error
 %s: %s"
```
```
,
 lcb_strerror(instance, err),
```
`            `
```
errinfo
 ? errinfo :
```
`""``);`
`    ``exit``(EXIT_FAILURE);`
`}`
`static``void`
```
get_callback(lcb_t instance,
```
`                         ``const``void`
```
*cookie,
```
`                         `
```
lcb_error_t
 error,
```
`                         ``const`
```
lcb_get_resp_t
 *resp)
```
`{`
`    ``if`
```
(error
 != LCB_SUCCESS) {
```
`        ``fprintf``(stderr,`
```
"Failed
 to retrieve \""
```
`);`
`        ``fwrite`
```
(resp->v.v0.key,
 1, resp->v.v0.nkey, stderr);
```
`        ``fprintf``(stderr,`
```
"\":
 %s\n"
```
```
,
 lcb_strerror(instance, error));
```
`    ``}``else``{`
`        ``fprintf``(stderr,`
```
"Data
 for key: \""
```
`);`
`        ``fwrite`
```
(resp->v.v0.key,
 1, resp->v.v0.nkey, stderr);
```
`        ``fprintf``(stderr,`
```
"\"
 is : "
```
`);`
`        ``fwrite`
```
(resp->v.v0.bytes,
 1, resp->v.v0.nbytes, stderr);
```
`    ``}`
`}`
`int``main(``void``)`
`{`
`    ``struct`
```
lcb_create_st
 create_options;
```
`    `
```
lcb_t
 instance;
```
`    `
```
lcb_error_t
 err;
```
`    ``memset`
```
(&create_options,
 0,
```
`sizeof``(create_options));`
`    `
```
create_options.v.v0.host
 =
```
`"myserver:8091"``;`
`    `
```
create_options.v.v0.user
 =
```
`"mybucket"``;`
`    `
```
create_options.v.v0.passwd
 =
```
`"secret"``;`
`    `
```
create_options.v.v0.bucket
 =
```
`"mybucket"``;`
`    `
```
err
 = lcb_create(&instance, &create_options);
```
`    ``if`
```
(err
 != LCB_SUCCESS) {
```
`        ``fprintf``(stderr,`
```
"Failed
 to create libcouchbase instance: %s\n"
```
`,`
`                `
```
lcb_strerror(NULL,
 err));
```
`        ``return``1;`
`    ``}`
`    `
```
/*
 Set up the handler to catch all errors! */
```
`    `
```
lcb_set_error_callback(instance,
 error_callback);
```
`    ``/*`
`     `
```
*
 Initiate the connect sequence in libcouchbase
```
`     ``*/`
`    ``if`
```
((err
 = lcb_connect(instance)) != LCB_SUCCESS) {
```
`        ``fprintf``(stderr,`
```
"Failed
 to initiate connect: %s\n"
```
`,`
`                `
```
lcb_strerror(NULL,
 err));
```
`        ``return``1;`
`    ``}`
`    `
```
/*
 Run the event loop and wait until we've connected */
```
`    ``lcb_wait(instance);`
`    ``/*`
`     `
```
*
 Set up a callback for our get requests
```
`     ``*/`
`    `
```
lcb_set_get_callback(instance,
 get_callback);
```
`    `
```
lcb_get_cmd_t
 cmd;
```
`    ``const`
```
lcb_get_cmd_t
 *
```
`const`
```
commands[1]
 = { &cmd };
```
`    ``memset`
```
(&cmd,
 0,
```
`sizeof``(cmd));`
`    `
```
cmd.v.v0.key
 =
```
`"foo"``;`
`    `
```
cmd.v.v0.nkey
 = 3;
```
`    `
```
err
 = lcb_get(instance, NULL, 1, commands);
```
`    ``if`
```
(err
 != LCB_SUCCESS) {
```
`        ``fprintf``(stderr,`
```
"Failed
 to get: %s\n"
```
`,`
`                `
```
lcb_strerror(NULL,
 err));
```
`        ``return``1;`
`    ``}`
`    ``lcb_wait(instance);`
`    ``lcb_destroy(instance);`
`    ``exit``(EXIT_SUCCESS);`
`}`
```
其實還有不少API, 但是文件我覺得因為v2.0才出來沒有很久, 官方文件不算是相當完整
要用C&C++來溝通, 最好要有查資料查半天找不到用法的心理準備
不然就是上他們forum去問問題, 基本上都會有人回答
下一篇會簡單介紹PHP的用法, PHP相對C來講, 真的是簡單好用很多!!
張貼者： [Tu
 Han-Hsing](https://plus.google.com/114036893041400132391)於 [下午9:00](http://hhtucode.blogspot.com/2013/02/couchbase-client-1-c-version.html)
[以電子郵件傳送這篇文章](http://www.blogger.com/share-post.g?blogID=1099001297101871030&postID=2909350560606465438&target=email)[BlogThis！](http://www.blogger.com/share-post.g?blogID=1099001297101871030&postID=2909350560606465438&target=blog)[分享至
 Twitter](http://www.blogger.com/share-post.g?blogID=1099001297101871030&postID=2909350560606465438&target=twitter)[分享至
 Facebook](http://www.blogger.com/share-post.g?blogID=1099001297101871030&postID=2909350560606465438&target=facebook)[分享到
 Pinterest](http://www.blogger.com/share-post.g?blogID=1099001297101871030&postID=2909350560606465438&target=pinterest)
標籤： [C](http://hhtucode.blogspot.com/search/label/C), [C++](http://hhtucode.blogspot.com/search/label/C%2B%2B), [Couchbase](http://hhtucode.blogspot.com/search/label/Couchbase), [NoSQL](http://hhtucode.blogspot.com/search/label/NoSQL), [Tutorial](http://hhtucode.blogspot.com/search/label/Tutorial)
