# Git@OSC通过Eclipse生成SSH keys RSA公钥私钥 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年01月14日 13:59:06[boonya](https://me.csdn.net/boonya)阅读数：5802








  由于我使用的系统是windows，所以不支持ssh-keygen命令没法自动生成，虽然有专业的ssh-keygen生成工具，但是懒得去下载了，直接用Eclipse比较方便，这里就讲Eclipse生成SSH keys生成RSA公钥私钥的方法。

### 1.安装好Git工具

安装就不再说了，如前一篇文章所述：[http://blog.csdn.net/boonya/article/details/50516079](http://blog.csdn.net/boonya/article/details/50516079)

### 2.设置用户和用户邮箱

**用户名：**




第一步，你需要告诉git你的名字，这个名字会出现在你的提交记录中。


git config --global user.name "你的名字"



**Email：**

然后是你的Email，同样，这个Email也会出现在你的提交记录中，请尽量保持此Email和您的Git@OSC的注册Email一致。
git config --global user.email "你的Email"
### 3.生成SSH RSA 公钥私钥

找到Elcipse下:Window>Preferences打开选择如下界面：


![](https://img-blog.csdn.net/20160114134451005?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

点击Generate RSA key...按钮,生成RSA密钥。

![](https://img-blog.csdn.net/20160114134658163?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

点击如下红色框保存私钥（公钥在id_rsa.pub）：

![](https://img-blog.csdn.net/20160114134756402?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

保存后可以在用户目录下找到.ssh目录，公钥私钥都保存在这里（公钥在id_rsa.pub）：

![](https://img-blog.csdn.net/20160114134845126?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**id_rsa.pub**文件不能直接打开可以用EditPlus打开：

![](https://img-blog.csdn.net/20160114134918804?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

登录GIT@OCS找到SSH公钥并添加：


![](https://img-blog.csdn.net/20160114135053212?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

添加完成后可以将GIT@OSC上面的项目下载到本地仓库进行编辑，只需复制git的地址：

![](https://img-blog.csdn.net/20160114135242366?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

然后打开Eclipse Git视图，选择仓库：

![](https://img-blog.csdn.net/20160114135341187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

将复制的git地址通过如下界面，添加克隆到项目到本地仓库：


![](https://img-blog.csdn.net/20160114135415061?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20160114135617747?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

user,password是你的Git@OSC用户和密码。

最后我们会看到下载到背地仓库的项目：

![](https://img-blog.csdn.net/20160114135733667?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


本地仓库位置：如上图Working Directory.如此就可以正常的使用Git@OSC了。




