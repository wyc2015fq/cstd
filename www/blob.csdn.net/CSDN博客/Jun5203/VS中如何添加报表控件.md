# VS中如何添加报表控件 - Jun5203 - CSDN博客
2019年03月20日 11:43:54[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：98
所属专栏：[C#](https://blog.csdn.net/column/details/29824.html)
有的人在VS中找不到关于报表这一项，这是因为你在装VS的时没有勾选Microsoft Office 开发人员工具、Microsoft SQL Server Data Tools，所以就出现了上述问题，那么如何解决这个问题呢？
**方案一**
Visual Studio Installer-修改，添加Microsoft Office 开发人员工具、Microsoft SQL Server Data Tools
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190419114051170.?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
**方案二**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190419114327108.?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
搜索Rdlc Report ，找到类似如下标志的插件安装即可。(温馨提示：不同版本可能略有差异）
![在这里插入图片描述](https://img-blog.csdnimg.cn/201904191144522.)
上述操作完成之后你就可以在工具箱里找到ReportViewer。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190419114815367.)
**方案三**
**【第一步】**
打开VS-工具-NuGe包管理器-程序包管理器控制台
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190320113417494.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
**【第二步】**
输入命令并执行：Install-Package Microsoft.ReportingServices.ReportViewerControl.WinForms -Pre
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019032011342762.jpg)
执行完毕
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190320113432852.jpg)
**【第三步】**
打开“工具箱”，点击右键，选择“选择项”
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190320113438618.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
**【第四步】**
点击“浏览”，找到刚才安装的根目录C:\Users\JJ.nuget\packages\microsoft.reportingservices.reportviewercontrol.winforms\150.900.148\lib\net40
选择
Microsoft.ReportViewer.WinForms.dll
点击“确定”，添加成功
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019032011344428.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
添加成功
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190320114204989.jpg)
问题存在的必要性是为了让有远见的人去解决的，勿急、勿躁、要稳住！
