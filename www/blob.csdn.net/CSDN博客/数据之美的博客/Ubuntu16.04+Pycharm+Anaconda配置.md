# Ubuntu16.04+Pycharm+Anaconda配置 - 数据之美的博客 - CSDN博客
2017年12月18日 00:06:41[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：261
原创 2016年11月03日
 09:08:34
- 
标签：
- [anaconda](http://so.csdn.net/so/search/s.do?q=anaconda&t=blog)
- 9594
作者：JUDGE_MENT
邮箱：gao19920804@126.com
CSDN博客：http://blog.csdn.net/sinat_23137713
最后编辑时间：2016.12.17  V1.1
声明：
1）该资料结合官方文档及网上大牛的博客进行撰写，如有参考会在最后列出引用列表。
2）本文仅供学术交流，非商用。如果不小心侵犯了大家的利益，还望海涵，并联系博主删除。
3）本人才疏学浅，难免出错，还望各位大牛悉心指正。
4）转载请注明出处。
配置完Eclipse+pydev之后用了一段时间，其实感觉还是不错的。但是听学弟说Pycharm用起来更加爽，那试着换一个IDE试试。
# 1. 下载
https://www.jetbrains.com/pycharm/download/#section=linux 。 选择[Linux](http://lib.csdn.net/base/linux)，选择下载免费的Community。
# 2. 安装PyCharm
# 下载完之后，将安装包放到Downloads目录下，进入Downloads。
[plain][view
 plain](http://blog.csdn.net/sinat_23137713/article/details/53018104#)[copy](http://blog.csdn.net/sinat_23137713/article/details/53018104#)
- $ cd Downloads/  
# 解压缩
[plain][view
 plain](http://blog.csdn.net/sinat_23137713/article/details/53018104#)[copy](http://blog.csdn.net/sinat_23137713/article/details/53018104#)
- $ tar xfz pycharm-*.tar.gz  
# 将解压完的文件夹移动到/usr/local目录
[plain][view
 plain](http://blog.csdn.net/sinat_23137713/article/details/53018104#)[copy](http://blog.csdn.net/sinat_23137713/article/details/53018104#)
- $ sudo mv pycharm-* /usr/local  
# 进入文件夹的bin文件夹内
[plain][view
 plain](http://blog.csdn.net/sinat_23137713/article/details/53018104#)[copy](http://blog.csdn.net/sinat_23137713/article/details/53018104#)
- $ cd /usr/local/pycharm-*/bin/  
# 运行sh文件安装
[plain][view
 plain](http://blog.csdn.net/sinat_23137713/article/details/53018104#)[copy](http://blog.csdn.net/sinat_23137713/article/details/53018104#)
- $ ./pycharm.sh  
# 安装过程一直选yes，最后选择create desktop快捷方式。
# 3. 调整主题和配色 
主题：File --> Settings --> Appearance --> Theme : Darcula 
配色：File --> Settings --> Editor --> Colors & Fonts ---> Scheme : WarmNeon
最终效果：
![](https://img-blog.csdn.net/20161103095512461?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
对比Eclipse，其实Eclipse也蛮好看的嘛~我感觉比pycharm更好看嘞。
![](https://img-blog.csdn.net/20161103093454924?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 4. 安装 Anaconda
下载地址：https://www.continuum.io/downloads
下载linux版本，根据python版本选择相应的。
4.1 将下载的安装包放到某目录下，然后执行
[plain][view
 plain](http://blog.csdn.net/sinat_23137713/article/details/53018104#)[copy](http://blog.csdn.net/sinat_23137713/article/details/53018104#)
- sudo bash 你的目录/Anaconda2-4.2.0-Linux-x86_64.sh  
4.2  读完license，回复个
[plain][view
 plain](http://blog.csdn.net/sinat_23137713/article/details/53018104#)[copy](http://blog.csdn.net/sinat_23137713/article/details/53018104#)
- yes  
4.3 输入安装地址
[plain][view
 plain](http://blog.csdn.net/sinat_23137713/article/details/53018104#)[copy](http://blog.csdn.net/sinat_23137713/article/details/53018104#)
- /usr/local/anaconda2  
4.4 安装路径放到/home/xx/.bashrc , 回复yes
[plain][view
 plain](http://blog.csdn.net/sinat_23137713/article/details/53018104#)[copy](http://blog.csdn.net/sinat_23137713/article/details/53018104#)
- yes  
4.5 重新载入一下这个文件
[plain][view
 plain](http://blog.csdn.net/sinat_23137713/article/details/53018104#)[copy](http://blog.csdn.net/sinat_23137713/article/details/53018104#)
- source ~/.bashrc  
4.6 运行python，发现已经安装成功ana
![](https://img-blog.csdn.net/20161114161837311?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 5. 改变pycharm解释器为-anaconda python解释器
1) 打开pycharm， 依次点击 file - > Settings(如果以后想所有项目都用anaconda python解释器就点击Default settings) - >Project: xx - >Project Interpreter 
2) 点击小齿轮，点击弹出来的add local
![](https://img-blog.csdn.net/20161206094307187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3）弹出的地址中选择：/usr/local/anaconda2/bin/python2.7这里，然后选择让这个文件当解释器
4）选择好，点击OK
# 6. ubuntu下shell中切换普通解释器和anaconda解释器的方法（可选）
1）打开shell，输入：
[python][view
 plain](http://blog.csdn.net/sinat_23137713/article/details/53018104#)[copy](http://blog.csdn.net/sinat_23137713/article/details/53018104#)
- gedit ~/.bashrc  
2）在文件最下面，找到：
[python][view
 plain](http://blog.csdn.net/sinat_23137713/article/details/53018104#)[copy](http://blog.csdn.net/sinat_23137713/article/details/53018104#)
- export PATH="/usr/local/anaconda2/bin:$PATH"
如果将这一行注释（前面加上#），则自动切换回默认python解释器；取消注释，就变回anaconda解释器。
3）命令行输入,重新载入一下这个文件：
[python][view
 plain](http://blog.csdn.net/sinat_23137713/article/details/53018104#)[copy](http://blog.csdn.net/sinat_23137713/article/details/53018104#)
- source ~/.bashrc  
# 7. anaconda使用中碰到的问题
## 1）使用anaconda以后再要使用不在conda环境中的包，要怎么安装？
> 
# 首先，将原来安装到/usr/lib/python2.7/dist-packages的包卸载：
[python][view
 plain](http://blog.csdn.net/sinat_23137713/article/details/53018104#)[copy](http://blog.csdn.net/sinat_23137713/article/details/53018104#)
- pip uninstall <例如tqdm>  
然后，使用anaconda目录下的pip重新安装：
[python][view
 plain](http://blog.csdn.net/sinat_23137713/article/details/53018104#)[copy](http://blog.csdn.net/sinat_23137713/article/details/53018104#)
- sudo /usr/local/anaconda2/bin/pip install <例如tqdm>  
最后，查看anaconda下面的包，发现了tqdm的踪影：
[python][view
 plain](http://blog.csdn.net/sinat_23137713/article/details/53018104#)[copy](http://blog.csdn.net/sinat_23137713/article/details/53018104#)
- conda list  
## 2)  anaconda目录中第三方包安装的位置在：
/usr/local/anaconda2/lib/python2.7/site-packages
### 3) anaconda目录获取超级权限
因为conda命令不能sudo执行，所以有的时候需要获取conda目录的超级权限。
[plain][view
 plain](http://blog.csdn.net/sinat_23137713/article/details/53018104#)[copy](http://blog.csdn.net/sinat_23137713/article/details/53018104#)
- sudo chmod 777 -R anaconda2  
## 4）运行matplotlib时，出现了“[This application failed to start because it could not find or load the Qt platform plugin “xcb”](http://stackoverflow.com/questions/29405689/qt-5-4-1-this-application-failed-to-start-because-it-could-not-find-or-load-th)”的解决办法
> 
产生这个问题的原因，是Qt依赖没有安装好，其中可以通过ldd libqxcb.so查看QT缺失的的依赖项，如图有几个依赖项都是no found.
> 
![](https://img-blog.csdn.net/20161219191852649?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjMxMzc3MTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
解决办法：重新安装anaconda里面的qt依赖包。
进入/anaconda2/bin，执行（提示：安装过程需要从国外网站下载，所以如果下载不动或者中断，你需要科学上网“latern”）
[python][view
 plain](http://blog.csdn.net/sinat_23137713/article/details/53018104#)[copy](http://blog.csdn.net/sinat_23137713/article/details/53018104#)
- sudo ./conda remove qt  
- sudo ./conda remove pyqt  
- sudo ./conda install qt  
- sudo ./conda install pyqt  
## 5）Anaconda建立独立环境，并在环境里安装包
i. 新建conda环境（意思似乎就是说不安装到anaconda2/lib/python2.7目录下，自己给这么庞大的程序单独安装到一个文件夹：envs/tensorflow）
注意anaconda2/envs文件夹没有权限，如果直接安装，不会撞到usr/local/anaconda2/envs，而是安装到home/wayne/.conda/envs文件夹，所以我们要先获取文件夹的超级权限。
> 
sudo chmod 777 -R /usr/local/anaconda2  
conda create -n tensorflow Python=2.7  
ii. 激活tensorflow环境，并安装
> 
source activate tensorflow  
export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-1.0.0-cp27-none-linux_x86_64.whl  
sudo pip install --ignore-installed --upgrade $TF_BINARY_URL  
iii. 给环境里安装包
> 
conda install --name tensorflow numpy
网址：https://docs.continuum.io/anaconda/pkg-docs 给出了所有conda可以安装的包
罒. 退出tensorflow环境，完成安装 （以后每次使用tensorflow之前都需要激活一下tensorflow环境）
> 
source deactivate tensorflow  
conda install --name tensorflow six
# 8. 安全卸载anaconda
删除`~/.bash_profile` 文件中的`$PATH` ...anaconda
 那一行 
删除anaconda文件夹
