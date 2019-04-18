# eclipse(indigo)安装maven插件失败 - Joe的博客 - CSDN博客





2016年10月18日 21:47:43[Joe-Han](https://me.csdn.net/u010089444)阅读数：2545








eclipse的indigo版本在安装maven插件的时候一直报错，最开始不管是用“Install New Software”、”eclipse market place“ 还是离线安装都没法安装成功

> 
Cannot complete the install because one or more required items could not be found. 

    Software being installed: m2e - Maven Integration for Eclipse (includes Incubating components) 1.5.0.20140606-0033 (org.eclipse.m2e.feature.feature.group 1.5.0.20140606-0033) 

    Missing requirement: Maven Integration for Eclipse 1.5.0.20140606-0033 (org.eclipse.m2e.core 1.5.0.20140606-0033) requires ‘bundle com.google.guava [14.0.1,16.0.0)’ but it could not be found 

    Cannot satisfy dependency: 

      From: Maven Integration for Eclipse 1.5.0.20140606-0033 (org.eclipse.m2e.core.ui 1.5.0.20140606-0033) 

      To: bundle org.eclipse.m2e.core [1.5.0,1.6.0)
后面找到原因是eclipse的indigo版本在安装大于1.3 以上的maven都会安装失败

可以参考 ： [https://www.eclipse.org/m2e/m2e-downloads.html](https://www.eclipse.org/m2e/m2e-downloads.html) 安装一个小于1.4的插件版本，具体方式如下：

依次点击 Help -> Install New Software , 在Work with: 中输入如下语句

> 
m2e - [http://download.eclipse.org/technology/m2e/releases/1.3/1.3.1.20130219-1424](http://download.eclipse.org/technology/m2e/releases/1.3/1.3.1.20130219-1424)


然后安装即可

![图片名称](https://img-blog.csdn.net/20161018214641222)





