
# Go语言开发环境搭建 - 尹成的技术博客 - CSDN博客

2018年11月16日 11:07:32[尹成](https://me.csdn.net/yincheng01)阅读数：88


\#开发环境搭建
\#\#下载地址
Go编译器
[https://golang.google.cn/dl/](https://golang.google.cn/dl/)
Goland官网下载
[https://www.jetbrains.com/go/download/\#section=windows](https://www.jetbrains.com/go/download/#section=windows)
在线激活地址
[http://idea.youbbs.org](http://idea.youbbs.org)
\#\#卸载过程如下：
点击设置
![这里写图片描述](https://img-blog.csdn.net/20180913085942843?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
点击应用
![这里写图片描述](https://img-blog.csdn.net/20180913090131977?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
找到go之后点击卸载
![这里写图片描述](https://img-blog.csdn.net/20180913090246586?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
卸载过程如下
![这里写图片描述](https://img-blog.csdn.net/20180913090355132?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
\#\#安装过程如下
双击go1.9.2.windows-amd64.mis
![这里写图片描述](https://img-blog.csdn.net/20180913090819675?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
点击Next
![这里写图片描述](https://img-blog.csdn.net/2018091309092265?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
勾选同意协议，点击Next
![这里写图片描述](https://img-blog.csdn.net/2018091309105669?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
选择安装路径，我这里使用得是默认路径，然后点击Next
![这里写图片描述](https://img-blog.csdn.net/20180913091218388?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
点击Install
![这里写图片描述](https://img-blog.csdn.net/20180913091318357?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
安装过程如下
![这里写图片描述](https://img-blog.csdn.net/20180913091437508?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
点击Finish,完成安装
![这里写图片描述](https://img-blog.csdn.net/20180913091605502?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
\#\#测试是否安装成功
Win+R,打开CMD终端
![这里写图片描述](https://img-blog.csdn.net/20180913091730814?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在命令行下输入go version
![这里写图片描述](https://img-blog.csdn.net/2018091309190610?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
\#\#配置环境变量
Go 语言依赖一个重要的环境变量：$GOPATH,GOPATH允许多个目录，当有多个目录时，请注意分隔符，多个目录的时候Windows是分号，Linux系统是冒号，当有多个GOPATH时，默认会将go get的内容放在第一个目录下。
$GOPATH目录约定有三个子目录：
src 存放源代码（比如：.go .c .h .s等）
pkg 编译后生成的文件（比如：.a）
bin 编译后生成的可执行文件（为了方便，可以把此目录加入到 $PATH 变量中）
选择高级系统设置
![这里写图片描述](https://img-blog.csdn.net/2018091309590620?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
选择环境变量
![这里写图片描述](https://img-blog.csdn.net/20180913100226975?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
选择新建
![这里写图片描述](https://img-blog.csdn.net/20180913100438217?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
添加GOPATH
![这里写图片描述](https://img-blog.csdn.net/20180913100624242?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
选择Path,点击编辑
![这里写图片描述](https://img-blog.csdn.net/20180913100752878?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
添加刚刚新建得GOPATH到Path中
![这里写图片描述](https://img-blog.csdn.net/20180913100928402?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
\#\#测试环境变量是否配置成功
Win+R键，打开CMD终端
![这里写图片描述](https://img-blog.csdn.net/20180913101346199?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
输入go env查看变量是否设置成功
![这里写图片描述](https://img-blog.csdn.net/20180913101504200?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
\#安装goland
双击goland-2017.3.2.exe
![这里写图片描述](https://img-blog.csdn.net/20180913101742942?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
出现以下界面，点击next
![这里写图片描述](https://img-blog.csdn.net/20180913101905744?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
选择安装路径，点击next
![这里写图片描述](https://img-blog.csdn.net/2018091310215541?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如果电脑是32位选择32-bit launcher,64位则选择64-bit launcher,然后点击Next。
![这里写图片描述](https://img-blog.csdn.net/20180913102344676?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
点击Install
![这里写图片描述](https://img-blog.csdn.net/20180913102447467?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
安装过程如下
![这里写图片描述](https://img-blog.csdn.net/20180913102536258?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
点击Finish
![这里写图片描述](https://img-blog.csdn.net/20180913102605974?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116110724858.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

