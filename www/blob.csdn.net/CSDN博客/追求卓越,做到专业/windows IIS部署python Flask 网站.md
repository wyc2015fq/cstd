
# windows IIS部署python Flask 网站 - 追求卓越,做到专业 - CSDN博客


2018年12月14日 13:25:11[Waldenz](https://me.csdn.net/enter89)阅读数：236


# 安装IIS,启用CGI
![](https://img-blog.csdnimg.cn/2018121409590685.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
# 安装Anaconda
我一般机器都安装Anaconda,虽然比较大， 但是管理python版本以及包会很方便。
![](https://img-blog.csdnimg.cn/20181214100147497.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
# 安装wfastcgi并激活
# 。
打开Anaconda Prompt。
![](https://img-blog.csdnimg.cn/20181214104410393.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20181214113531609.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
进入python安装目录下的scripts文件夹然后运行wfastcgi-enable
![](https://img-blog.csdnimg.cn/2018121411041459.png)
**打开"处理程序映射"模块，添加映射模块**
![](https://img-blog.csdnimg.cn/20181214111802132.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/2018121411172012.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
**web.config文件如下：**
其中“mypythontest.app”，为python项目中
```python
<?xml version="1.0" encoding="UTF-8"?>
<configuration>
    <system.webServer>
        <handlers>
            <add name="FlaskFastCGI" path="*" verb="*" modules="FastCgiModule" scriptProcessor="C:\ProgramData\Anaconda3\python.exe|c:\ProgramData\anaconda3\lib\site-packages\wfastcgi.py" resourceType="Unspecified" />
        </handlers>
    </system.webServer>
	<appSettings>
		<!-- Required settings -->
		<add key="WSGI_HANDLER" value="mypythontest.app" />
		<add key="PYTHONPATH" value="C:\ProgramData\Anaconda3" />
	</appSettings>
</configuration>
```


