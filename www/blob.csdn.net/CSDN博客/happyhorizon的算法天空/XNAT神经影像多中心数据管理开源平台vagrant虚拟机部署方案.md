# XNAT神经影像多中心数据管理开源平台vagrant虚拟机部署方案 - happyhorizon的算法天空 - CSDN博客
2018年09月17日 17:46:20[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：362标签：[xnat																[mri																[data web](https://so.csdn.net/so/search/s.do?q=data web&t=blog)](https://so.csdn.net/so/search/s.do?q=mri&t=blog)](https://so.csdn.net/so/search/s.do?q=xnat&t=blog)
个人分类：[MRI data](https://blog.csdn.net/happyhorizion/article/category/7463172)
XNAT( extensible Neuroimaging Archive Toolkit )是一个开源的软件平台，旨在帮助神经影像及其相关数据的管理和挖掘。XNAT包括一个加密的后端数据库和一个基于万维网的丰富的人机界面（XND）。
XNAT项目由华盛顿大学、哈佛大学、哈佛大学胡格斯医学院（ Howard Hughes Medical Institute）和生物医学信息技术研究组织（ Biomedical Informatics Research Network）共同提供帮助和支持。
项目网址: [https://www.xnat.org/](https://www.xnat.org/)
# 安装要求:
这里介绍的是在vagrant虚拟机上快速部署xnat的方法. 要求预装 vagrant (>1.8.7) , Git, VirtualBox(>5.1)
## XNAT的安装
xnat有很多版本, 例如dev版本, release版本等. 这里默认安装的是release版本.
### 典型安装(快速安装方法,采用默认的虚拟机系统配置.)
xnat提供了一行命令快速安装方式, 具体命令如下:
```
git clone --branch master https://bitbucket.org/xnatdev/xnat-vagrant.git && cd xnat-vagrant && ./run xnat setup
```
快速安装采用了默认的环境设置. 虚拟机网络IP是10.1.1.17, 内存2G. 安装过程中会在shell中打印出具体的过程,安装成功后会在shell控制台上打印出默认参数具体如下:
```
xnat-release: #
    xnat-release: xnatUrl=10.1.1.17
    xnat-release: siteName=XNAT
    xnat-release: adminEmail=admin@yourorg.org
    xnat-release: smtpServer=mail.server
    xnat-release: destination=/data/xnat/pipeline
    xnat-release: Building pipeline engine
    xnat-release:  * From:        /data/xnat/src/pipeline
    xnat-release:  * To:          /data/xnat/pipeline
    xnat-release:  * Email:       admin@yourorg.org
    xnat-release:  * Site name:   XNAT
    xnat-release:  * Site URL:    10.1.1.17
    xnat-release:  * SMTP server: mail.server
    xnat-release: 
    xnat-release: 
    xnat-release: Resource folders:
    xnat-release:  * /data/xnat/src/pipeline/templates/resources
    xnat-release: Script folders:
    xnat-release:  * /data/xnat/src/pipeline/templates/scripts
    xnat-release: Binary folders:
    xnat-release: No value set for the MODULES configuration, no custom functionality will be included.
    xnat-release: Setting xnat as owner of the following folder(s) and contents: /data /data/xnat /var/lib/tomcat7
    xnat-release: Enabling and starting Tomcat...
    xnat-release: Tomcat 7 is not running, starting.
    xnat-release: Found requested token in Tomcat 7 startup log: INFO: Server startup in 56597 ms
    xnat-release: ===========================================================
    xnat-release: Your VM's IP address is 10.1.1.17. 
    xnat-release: Your XNAT server will be available at: 
    xnat-release: http://10.1.1.17
    xnat-release: ===========================================================
'./.vagrant/machines/xnat-release/virtualbox/index_uuid' -> './.work/index_uuid'
Provisioning completed.
/home/dl/Software/vagrant/xnat-vagrant/configs/xnat-release
```
### 个性化安装
首先下载xnat文件并进入下载目录:
```
git clone --branch master https://bitbucket.org/xnatdev/xnat-vagrant.git  
cd xnat-vagrant
```
xnat配置文件存放于:`/xnat下载路径/configs/xnat-release/config.yaml`
很多时候都可以直接用xnat的默认配置.如果要修改, 最好创建一个 local.yaml, 然后再修改. 可以定义自己的值和相关属性. 具体例子可以见sample.local.yaml
一般比较常见的是根据local的网络设置配置IP地址, 或者增加虚拟机的RAM容量. 例如默认的 config.yaml 文件中配置如下:
```
vm_ip:          10.1.1.17
ram:            2048
```
可以修改为:
```
vm_ip:          192.168.1.150
ram:            4096
```
修改配置参数后,就可以创建虚拟机了. 最简单的方式是在xnat-release目录中运行`./run xnat setup`语句, 自动完成后续安装.
如果对xnat的安装有一些更细节的个性化需求, 还可以查看/xnat-vagrant/configs/xnat-release/setup.sh并修改之. 当然, 还可以用vagrant的命令逐步安装. 新手且对系统理解不深入者. 慎重. 还是简单地:
```
./run xnat setup
```
run.sh脚本除了setup命令参数以外, 还有ssh, start, stop, destroy几个命令参数选项.
start和stop是Vagrant自身的 reload and halt 命令的别名. 用来启动和停止setup后的虚拟机.
安装完成后, 可以打开virtualbox查看, 已经配置了xnat虚拟机. 在shell中输入`virtualbox`, 弹出virtualbox控制界面:
![这里写图片描述](https://img-blog.csdn.net/20180917174114954?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 浏览XNAT数据管理网站
在浏览器中输入: [http://10.1.1.17:8080/app/template/Login.vm](http://10.1.1.17:8080/app/template/Login.vm)
就可以看到xnat的登录界面了:
![这里写图片描述](https://img-blog.csdn.net/20180917174155148?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
输入 USER: admin, PASSWORD: admin, 就可以进入xnat. 第一次登录会有个设置页面, 可以选择默认设置, 点击ok按钮后, 进入正式的登录环境:
![这里写图片描述](https://img-blog.csdn.net/20180917174223946?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
自此, 在vagrant环境下配置xnat网站完成. 在虚拟机上调试后, 就可以考虑安装在正式的服务器上了.
补充:
如果虚拟机关机了, 再次启动xnat-vagrant服务器的时候, 一定要进入到xnat-vagrant/config/xnat-release/路径下, 输入`vagrant reload --provision` 命令重新启动服务. 在浏览器中再次输入ip: 10.1.1.17:8080就可以再次看到xnat的登录页面了.
