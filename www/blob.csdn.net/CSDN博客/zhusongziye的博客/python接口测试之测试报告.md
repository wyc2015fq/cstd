# python接口测试之测试报告 - zhusongziye的博客 - CSDN博客





2018年10月29日 22:09:22[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：297








 在本文章中，主要使用jenkins和编写的自动化测试代码，来生成漂亮的测试报告，关于什么是CI这些我就不详细的介绍了，这里我们主要是实战为主。

       首先搭建java的环境，这个这里不做介绍。搭建好java的环境后，在https://jenkins.io/上下载jenkins，以及在https://jenkins.io/上下载tomcat，都下载完成后，进行解压，把jenkins.war放在tomcat的webapps的目录下，在tomcat的bin目录下，点击start.bat启动tomcat，启动后，在浏览器中访问http://localhost:8080/jenkins，首次使用会看到需要输入密码，在windows环境下，密码是在C:\Users\Administrator\.jenkins\secrets目录下，打开initialAdminPassword文件，复制该内容，把它copy到jenkins的密码输入，然后下来选择安装插件，关于插件这里就不详细的说明了。

     这里我们使用allure来生成测试报告，我们知道，在python的自动化测试中，常用生成测试报告使用的是HTMLTestRunner库，但是该库生成的测试报告不是很漂亮，我们使用allure可以生成很直观的测试报告而且测试报告比较渲，下面就来详细的说明如何使用它来生成很酷的测试报告步骤。

   首先需要在jenkins中安装插件Allure Jenkins Plugin，在jenkins的插件管理中，直接搜索allure可以搜索到，然后选择安装就可以了。安装该插件成功后，点击jenkins的“系统管理”，在系统管理中，点击Global Tool Configuration，在Global Tool Configuration的界面安装allure，见截图：

![](https://img-blog.csdnimg.cn/20181029220559275.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

点击“Allure Commandline安装...”，会出现如下的界面，见截图：

![](https://img-blog.csdnimg.cn/20181029220609981.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

选择好后，点击save保存成功。下来我们需要安装pytest，安装的命令为：

**    pip install pytest**
**    pip install pytest-allure-adaptor**

在线安装成功后，在cmd的命令中输入pytest，如果显示信息如下，表示安装OK，见截图：

![](https://img-blog.csdnimg.cn/20181029220624161.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

     OK，下面我们来编写代码，来进行测试，比如编写如下的测试代码，见编写后的源码：

#!/usr/bin/env python 
#-*-coding:utf-8-*-

import  unittest

def div(a,b):
    return a-b

class DivTest(unittest.TestCase):
    def test_div_001(self):
        self.assertEqual(div(3,2),1)

    def test_div_002(self):
        self.assertEqual(div(3,3),0)

    def test_div_003(self):
        self.assertEqual(abs(div(2,3)),1) 
下面我们通过pytest来执行该文件，见执行后的结果信息，见如下的截图：

![](https://img-blog.csdnimg.cn/20181029220655773.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

     下面我们结合jenkins,pytest,allure来生成测试报告，在jenekins的系统设置中指定allure的测试报告目录，见配置的截图：

![](https://img-blog.csdnimg.cn/20181029220706936.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



我们创建新的job，在构建步骤中选择windows batch command，填写执行的命令，见截图：

![](https://img-blog.csdnimg.cn/20181029220719290.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

在构建后操作选择Allure Report，见截图：

![](https://img-blog.csdnimg.cn/20181029220728675.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

点击保存，刚才创建的 job是blog，创建成功后，在项目详情页面，可以看到显示allure测试报告的图标，我们立即构建后，见构建后成功后的部分打印出的信息

C:\Users\Administrator\.jenkins\workspace\blog\report -c -o C:\Users\Administrator\.jenkins\workspace\blog\allure-report
**Report successfully generated to **C:\Users\Administrator\.jenkins\workspace\blog\allure-report
**Allure report was successfully generated.**Creating artifact for the build.
Artifact was added to the build.
**Finished: SUCCESS**
见blog的job的详情页面，见截图：

![](https://img-blog.csdnimg.cn/20181029220800425.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

见点击Allure Report后的测试报告，见截图：

![](https://img-blog.csdnimg.cn/20181029220811357.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

Author: QQ2839168630



