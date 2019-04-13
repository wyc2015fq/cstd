
# eclipse安装pydev方法，配置python编译环境 - jerrygaoling的博客 - CSDN博客


2018年07月17日 01:09:44[jerrygaoling](https://me.csdn.net/jerrygaoling)阅读数：251


## Eclipse配置Python编译环境
1.安装eclipse软件。
直接进官网下载安装即可。[https://www.eclipse.org/downloads/](https://www.eclipse.org/downloads/)
2.下载Python的编译器，直接从官网下载msi版就行，一键安装。
[https://www.python.org/](https://www.python.org/)
我下载的为3.7版本的安装包x64：[https://www.python.org/ftp/python/3.7.0/python-3.7.0-amd64.exe](https://www.python.org/ftp/python/3.7.0/python-3.7.0-amd64.exe)
直接安装即可。安装时选择配置path文件。
3.使用pydev来配置eclipse的python编译环境。
插件下载地址是：[http://sourceforge.net/projects/pydev/files](http://sourceforge.net/projects/pydev/files)
![这里写图片描述](https://img-blog.csdn.net/20180717010738318?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
解压到Eclipse的安装目录下的dropins文件夹下：
![这里写图片描述](https://img-blog.csdn.net/20180717010748615?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180717010757882?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
启动eclipse软件，开始配置编译环境。
点击Window —>  preferences ，在左边的标题栏里找到Pydev
在Pydev里找到含有Python的一栏，点击，然后点击New，在文件查找里选择你的Python的安放目录下的python.exe文件。如下图：
![这里写图片描述](https://img-blog.csdn.net/20180717010811970?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/2018071701082171?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180717010829539?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
点击ok就完成了所有的环境配置。
编写简单项目，测试是否配置成功。
![这里写图片描述](https://img-blog.csdn.net/2018071701084083?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
确认项目名称：
![这里写图片描述](https://img-blog.csdn.net/20180717010849721?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
添加python文件：
![这里写图片描述](https://img-blog.csdn.net/20180717010857101?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
属性选择带类的：
![这里写图片描述](https://img-blog.csdn.net/20180717010905900?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
输入测试数据：
'''
Created on 2018年7月17日
@author: Jerry
'''classMyClass(object):'''
    classdocs
    '''def__init__(self):'''
        Constructor
        '''self.test ='hello,world!'defout_wo(self):print(self.test)if__name__ =='__main__':
    obj_spider = MyClass()
    obj_spider.out_wo()保存文件，然后右键点击Run as，选择Python run，在控制台会显示 hello world.
![这里写图片描述](https://img-blog.csdn.net/20180717010918573?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
至此，使用eclipse编译python的配置方法宣告成功。

