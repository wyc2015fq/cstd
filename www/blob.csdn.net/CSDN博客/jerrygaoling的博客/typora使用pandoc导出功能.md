
# typora使用pandoc导出功能 - jerrygaoling的博客 - CSDN博客


2018年05月14日 18:17:40[jerrygaoling](https://me.csdn.net/jerrygaoling)阅读数：2417


# 使用typora导出md格式为其他格式
## 安装typora
登录[typora官网](https://www.typora.io/)，下载最新版typora，点击安装即可`typora-setup-x64.exe`。
![这里写图片描述](https://img-blog.csdn.net/20180514181507897?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
安装后进入原始界面：
![这里写图片描述](https://img-blog.csdn.net/20180514181517495?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
typora会默认使用系统语言作为软件界面语言。
## 使用导出功能
在日常使用过程中，需要用到其他的格式，typora默认支持`.md`文件转`.pdf`与`.html`格式，其余的常见格式需要使用`pandoc`扩展程序来支持。
导出方法：
![这里写图片描述](https://img-blog.csdn.net/20180514181527858?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在导出界面可以看到，typora本身支持的导出格式在横线之上。点击其他格式是，会提示安装`pandoc`扩展文件。
![这里写图片描述](https://img-blog.csdn.net/20180514181538299?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 安装pandoc扩展程序
进入[pandoc官网](https://github.com/jgm/pandoc/releases/tag/2.2.1)，点击下载适合自己操作系统的版本，如博主的操作系统为window10-64版。
![这里写图片描述](https://img-blog.csdn.net/20180514181546929?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
下载过程中，可能遇到下载失败，连接超时等情况，原因为，此官网为美国服务器，可能数据连接被墙，可自行想办法，博主为让外国留学的同学帮忙下载并回传。如若需要可留言。
### 点击安装程序
点击`pandoc-2.2.1-windows-x86_64.msi`安装程序。
![这里写图片描述](https://img-blog.csdn.net/20180514181558531?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
选取第一个选项，第二个选项将需要获取管理员权限，点击安装。
安装为默认选项，没有可修改项，安装默认到C盘。
![这里写图片描述](https://img-blog.csdn.net/20180514181606813?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
安装完毕后，记得配置==环境变量==：
将安装目录放置于`path`中，默认安装会自动将路径配置到`path`中，如果有用户配置的path，则需要手动将其添加到系统path中。
![这里写图片描述](https://img-blog.csdn.net/20180514181616411?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 检测安装是否成功
运行`cmd`输入`pandoc --help`如果安装不成功，将提示`pandoc`不是系统程序。安装成功，将出现帮助项
![这里写图片描述](https://img-blog.csdn.net/20180514181624189?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 使用typora导出
在安装完`pandoc`之后，可使用typora的导出功能。
注：可能存在安装完之后，还是提示安装`pandoc`，此时可重新启动电脑，或者卸载typora，然后在安装。
完成上述操作之后，将可以方便的使用pandoc的`.md`转其他格式。
![这里写图片描述](https://img-blog.csdn.net/2018051418163539?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
typora将自动调用pandoc，并在后台静默操作。
## 测试导出结果
原始文件
![这里写图片描述](https://img-blog.csdn.net/20180514181643475?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
导出到docx的文件
![这里写图片描述](https://img-blog.csdn.net/20180514181651517?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## pandoc使用
pandoc是一个很强大的文件转换工具，以上只阐述了使用typora来讲md文件导出为其他格式。
详细的pandoc转换方式，可以自行查看pandoc的帮助文档。

