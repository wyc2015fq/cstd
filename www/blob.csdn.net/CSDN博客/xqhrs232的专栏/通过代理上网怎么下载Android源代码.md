# 通过代理上网怎么下载Android源代码 - xqhrs232的专栏 - CSDN博客
2012年02月01日 16:58:15[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1481
CSDN相关帖子
1.Android的源代码可以去同步下载，Android的Linux部分的源代码怎么获取？Linux网站？该获取那个版本呢？
[http://topic.csdn.net/u/20110328/16/ab03326f-9309-428e-b52c-0c122cedb5c9.html](http://topic.csdn.net/u/20110328/16/ab03326f-9309-428e-b52c-0c122cedb5c9.html)
2.下载android的源代码时，curl命令执行怎么老是报错::curl:(6) couldnot resolve host.为什么？
[http://topic.csdn.net/u/20110322/16/330b5e20-76fd-4b17-a2ad-77542d48d68f.html](http://topic.csdn.net/u/20110322/16/330b5e20-76fd-4b17-a2ad-77542d48d68f.html)
3.下源码报Unable to look up android.git.kernel.org(port 9418) Name or service not know
[http://topic.csdn.net/u/20110323/11/922f9968-0c50-45d4-9a4d-aead5f2f7473.html](http://topic.csdn.net/u/20110323/11/922f9968-0c50-45d4-9a4d-aead5f2f7473.html)
4.Android源码不同版本对应的交叉编译工具链怎么获取?自己去编译?对应编译器的各个组件(工具软件)要找什么样的版本呢?
[http://topic.csdn.net/u/20101104/23/e6893ca1-ed3b-4f14-9792-c7454c5a76b0.html](http://topic.csdn.net/u/20101104/23/e6893ca1-ed3b-4f14-9792-c7454c5a76b0.html)
几个比较有参考价值的网页
　　1.如何下载Android源码（window和Linux）  
[http://blog.sina.com.cn/s/blog_91e5d1d70100vtdk.html](http://blog.sina.com.cn/s/blog_91e5d1d70100vtdk.html)
        2.获取Android源码时如何解决【fatal: Unable to look up android.git.kernel.org (port 9418) (Name or service not known)】的问题
[http://www.cnblogs.com/yyangblog/archive/2011/03/24/1993796.html](http://www.cnblogs.com/yyangblog/archive/2011/03/24/1993796.html)
//=====================================================================
//2012--2--2 xqh 今天记录一下可以成功下载Android源代码的过程
可以参考Android官方网站的下载参考方式----[http:](http://source.android.com/source/downloading.html)//source.android.com/source/downloading.html
1。用curl命令去下载repo文件----通过代理上网用把自己的用户名跟密码做为参数带上----不要去用http://android.git.kernel.org/repo网址----因为已经挂了的网址
curl https://dl-ssl.google.com/dl/googlesource/git-repo/repo > ~/bin/repo
curl -u<proxy_user_id>:<proxy_password>-x<proxy_server>:<proxy_port>
 https://dl-ssl.google.com/dl/googlesource/git-repo/repo > ~/bin/repo
2。修改repo文件的可执行属性
chmod a+x~/bin/repo
repo文件中的REPO_URL要不要修改成http或者https----两个都可以试一下
3。在当前打开的终端窗口下执行如下命令----限于通过代理上网的下载方式
$ export HTTP_PROXY=http://<proxy_user_id>:<proxy_password>@<proxy_server>:<proxy_port>
$ export HTTPS_PROXY=http://<proxy_user_id>:<proxy_password>@<proxy_server>:<proxy_port>
4。导出repo文件的路径到PATH环境变量并执行----repo init 不要去用git://android.git.kernel.org/platform/manifest.git
 网址----因为已经挂了的网址
$ PATH=~/bin:$PATH
$ repo init -u https://android.googlesource.com/platform/manifest -b android-4.0.1_r1
如果成功会要求输入你的用户名跟你的邮箱----方便联系到你，注意你要下的版本不一样后面带的版本号要对应改变
5。得到隐藏的.repo文件夹---如果正常的话这个文件夹的大小会变化注意观察一下
6。下一步就是执行./repo sync文件同步了
如果同步的过程中有repo sync failed就重新执行./repo sync命令----一般是支持断点处重新下载的
//==================================================
//备注:
1。我上面的方法是一般性的方法，不同的环境要对应修改
1》你准备存储源代码的路径 
2》你上网代理用的用户名及密码及你机子的代理服务器地址跟端口号
3》你要下载的源代码版本 
4》你自己的环境是否对上面用的命令都支持
2。这个过程没有用到git命令工具
　　如果要用也需要进行相应设置----$ git config --global core.gitproxy"http-proxy-gw for kernel.org"
