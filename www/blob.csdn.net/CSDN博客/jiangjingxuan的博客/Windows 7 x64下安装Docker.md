# Windows 7 x64下安装Docker - jiangjingxuan的博客 - CSDN博客





2017年02月07日 11:31:17[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：762









一、安装前提：

  1、操作系统必须是64位系统。

  2、系统必须支持“ Hardware Virtualization Technology”，并且，“virtualization ”可用。

  启用CPU硬件虚拟化，进入BOIS,如下图：

![](https://img-blog.csdn.net/20160908223055864?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

二、安装DockerToolbox.

  1、在下面地址下载Docker-ToolBox, [https://www.docker.com/products/docker-toolbox](https://www.docker.com/products/docker-toolbox)

  2、安装步骤：

![](https://img-blog.csdn.net/20170207113619184?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmdqaW5neHVhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170207113637840?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmdqaW5neHVhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170207113658091?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmdqaW5neHVhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170207113717872?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmdqaW5neHVhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



安装成功后桌面出现下图3个图标。

![](https://img-blog.csdn.net/20170207113756421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmdqaW5neHVhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




**安装后所遇问题**：

        在windows7下安装DockerToolbox后，启动Docker Quickstart Terminal会报下面错误。

![](https://img-blog.csdn.net/20170207113825374?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmdqaW5neHVhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**解决办法**：


        这是因为VirtualBox兼容性引起的，在VirtualBox安装目录中找到VBoxManage.exe，右击选择属性，设置Windows
 Server 2008兼容模式运行。再执行vagrant up，正常运行，问题解决。

![](https://img-blog.csdn.net/20170207114143004?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmdqaW5neHVhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






