# Github简单操作详解——初识Git - DEVELOPER - CSDN博客





2017年10月02日 01:09:43[学术袁](https://me.csdn.net/u012827205)阅读数：373
所属专栏：[Hadoop-学习](https://blog.csdn.net/column/details/20399.html)









这大半夜的，还要写一下我的博客！为了不让我这一下午的成果不在丢失。

还记得2014年几个大学同学一起开发android项目，使用的第一个项目托管就是Github，时隔多年已“乡音无改鬓毛衰”。

由于在公司现在负责一个项目的开发本来是用不着git的。但是，一些无理的需求让我不得不再次使用git。但是时隔那么久，我已经不知道怎么使用了。然后临时抱佛脚，跟大家简单分享一下心得。

git作为版本控制工具，我们也是一直在使用她的：增、删、改、查

但是我们使用git基本上都是要连接并添加到远程仓库，那该怎么做呢？![](https://img-blog.csdn.net/20171002012404337?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

No1.我们看到jiujia这个文件下有两个代码文件。我们这时候要把这个目录变成Git可以管理的仓库。



```java
$ git init
Initialized empty Git repository in E:/Jiujia/.git/
```




然后会变成这样的成功效果，多了一个.git文件，

![](https://img-blog.csdn.net/20171002012936836?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


No.2，如果这时候我们想要把这两个文件MainActivity.java和README.md添加到版本库中，我们要这么操作，



```java
$ git add MainActivity.java
$ git add README.md
```
这时候就添加到了“缓冲区”，真正提交到版本库，需要这一行代码，





```java
$ git commit -m "这个是当前版本库进行提交的注释"
[master (root-commit) f6cebf7] 这个是当前版本库进行提交的注释
 2 files changed, 70 insertions(+)
 create mode 100644 MainActivity.java
 create mode 100644 README.md
```
这时候就可以看到我们成功了！已经成功提交到了本地仓库。

这时候我们来看一看：增、删、改、查



增：

![](https://img-blog.csdn.net/20171002014107435?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


如我们添加一个文件personal.py，这个时候我们需要这几个步骤就能完成需求，如下图

![](https://img-blog.csdn.net/20171002014300169?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


改：

这时候，我们就对personal.py这个文件进行修改，添加一行文字<使用Notepad++进行编辑>“fall in love at first sight.”，修改完了文件之后，使用git进行提交到版本库中。使用下面的命令行，这样就算是成功了，

![](https://img-blog.csdn.net/20171002014827929?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


查：

查，分很多种类。有查看你的提交记录，命令行和结果如图，

```java
$git log
```



![](https://img-blog.csdn.net/20171002015059468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


假如这个时候我又修改了personal.py文件，然后执行命令和查询当前的状态，<可以看到说明，在版本库中的主分支上有文件改动，提醒我们去提交>

![](https://img-blog.csdn.net/20171002015544296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20171002015544296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20171002015544296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20171002015544296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20171002015609154?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171002015544296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


我们还能查看文件的内容，通过命令，<我只截图了一部分>

![](https://img-blog.csdn.net/20171002015923267?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


删：

通过命令就能把文件给删除，删除之后不要忘记提交到版本库中哦，

![](https://img-blog.csdn.net/20171002020215747?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


假如说这个时候，我不小心提交的“版本说明“有问题，我需要重新提交说明，但是不能妨碍我提交的文件。可以这样

![](https://img-blog.csdn.net/20171027133738786?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





重要的地方就是要连接远程仓库并提交到远程仓库中的操作。

首先弄一个错误显示出来，并指出解决方案，

![](https://img-blog.csdn.net/20171002020757758?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


表示远程连接已经存在了，请不要再次提交！解决方法，先将原先的删除，然后再发起远程的连接。

![](https://img-blog.csdn.net/20171002021056591?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


然后就是提交到远程仓库了，接下来再弄出一个提交的错误来，

![](https://img-blog.csdn.net/20171002021337473?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


先看出错原因是，我们在github上新建一个仓库会出现一个文件 README.md ,但是如果我们要把代码提交到远程仓库，本地（本机）的代码管理仓库中也必须拥有这个文件。该怎么解决呢？把这个文件从远程仓库中拉到本机仓库。然后我们可以从图片中看到拉取和提交成功的命令反馈，

![](https://img-blog.csdn.net/20171002022026291?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





再记录一下如何把代码推送到远程分支上，如果在分支上修改，完事之后要把代码推到远程分支的仓库。这个时候要保证您所在的Git分支与远程仓库分支同名。

假如这个时候是我在 GitBash 上已经新创建过了分支。然后再本地仓库中新建了一个文件，然后要把文件推到远程的分支仓库中。我现在操作：从别的分支切换到主分支 master 上，然后从主分支切换到新建的分支上， 然后在新建的分支上做操作，并推到远程对应的分支上。

![](https://img-blog.csdn.net/20171002145151713?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20171002145623125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


20171003今天上传代码的时候，想要查看一下我与远程连接没，有没有推送的权利。结果忘记了，然后又上网查了查，在此记录一下，

![](https://img-blog.csdn.net/20171003233804668?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





以上是在家里的电脑进行的，这样家里的电脑可以进行修改并提交推送到远程仓库。但是公司的怎么办？使用公司的电脑我没有推送权限只能修改提交不能推送到远程，这样就不能实时与远程服务器连接，因为在github服务器建立远程仓库，如果要要推送权限必须要有授权，这个授权就是由生成的公钥添加到github上并能被成功授权才行。接下来我演示一下怎么同步公司电脑！

步骤是生成 ssh 公钥和私钥 然后把公钥添加（add）到github上。就像这样，

![](https://img-blog.csdn.net/20171005115654560?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


但是怎么成？这样敲代码命令，当然先安装git软件，并设置个人信息。然后，生成钥匙，

![](https://img-blog.csdn.net/20171005120027905?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


输入密码就成功了！但是，在这里，看最后一行为什么会出现这个呢？因为我第一次无法生成，生成的.ssh是个空的文件夹，英语的意思是让你选择一个文件夹把你刚生成的钥匙放进去。我们就按照他的指示输入"/c/Users/YJH/.ssh/id_rsa"，当然你的可能不一样的地址。最后输入密码就成功生成了。（注意：执行命令生成钥匙前，把之前生成的空的.ssh文件删掉）

把公钥中的内容添加到github上之后，回来测试一下是否成功，并被授权。命令流程如下，

![](https://img-blog.csdn.net/20171005120548882?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







