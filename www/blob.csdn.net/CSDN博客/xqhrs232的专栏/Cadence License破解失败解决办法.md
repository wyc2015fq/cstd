# Cadence License破解失败解决办法 - xqhrs232的专栏 - CSDN博客
2018年01月03日 09:27:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4428
原文地址::[http://blog.csdn.net/pieces_thinking/article/details/72872186](http://blog.csdn.net/pieces_thinking/article/details/72872186)
相关文章
1、Cadence_SPB_16.6安装详细破解步骤----[https://wenku.baidu.com/view/46b88875fe4733687e21aae7.html](https://wenku.baidu.com/view/46b88875fe4733687e21aae7.html)
2、[安装cadence16.6/16.5经验(Unable to restart Cadence License Server...](http://blog.csdn.net/mali0205/article/details/71249425)----[http://blog.csdn.net/mali0205/article/details/71249425](http://blog.csdn.net/mali0205/article/details/71249425)
3、Cadence16.6史上最详细安装破解攻略-不成功破解，求喷----[http://download.csdn.net/download/yangpan19871001/9253265](http://download.csdn.net/download/yangpan19871001/9253265)
4、cadence16.3安装问题解决（解决最后的license的问题）----[http://blog.csdn.net/charcy/article/details/6256450](http://blog.csdn.net/charcy/article/details/6256450)
# 问题1：Unable to restart Cadence License Server with the new license file
这个问题是在运行License Server Configuration Utility时可能遇到的。
Cadence破解license，指定license文件时，提升下列的报错：
- 
Unable to restart Cadence License Server with the new license file ‘D:\Cadence\LicenseManager\license.dat’.’ 
Please check the license log file ‘D:\Cadence\LicenseManager\debug.log’ for details.
- 
The new license server setting ‘5280@xxxx’ was successfully added to your CDS_LIC_FILE license path environment variable.
![这里写图片描述](https://img-blog.csdn.net/20170605222936631?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUGllY2VzX3RoaW5raW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- **产生问题的原因：
cdslmd.exe文件未破解到。从另外电脑拷贝已破解的cdslmd.exe文件进行替换。
若没有，请联系博客。
将下载的文件解压出来，将cdslmd.exe文件放到
D:\Cadence\LicenseManager进行替换，再次指定license，提示如下：
*- Cadence License Server restarted successfully with the new license file ‘D:\Cadence\LicenseManager\license.dat’. 
- The new license server setting ‘5280@xxxx’ was successfully added to your CDS_LIC_FILE license path environment variable.*
若出现上列的文字，则license更新成功。破解完成。**
# 问题2：ERROR Unable to update the CDS_LIC_FILE license path environment variable.。
按照crack破解指南，破解安装cadence16.6之后。 
在最后的License client configuration Unility启动阶段，弹出ERROR Unable to update the CDS_LIC_FILE license path environment variable.。
我的解决办法是，在快捷方式中，直接点击License client configuration Unility，而不是，所有程序-cadence-License Manager-License client configuration Unility。
或者直接用管理员权限进行运行也不会出现问题。
有问题可以关注本人微博。
