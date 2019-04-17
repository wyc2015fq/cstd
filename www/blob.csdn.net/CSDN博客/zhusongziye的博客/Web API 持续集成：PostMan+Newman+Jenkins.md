# Web API 持续集成：PostMan+Newman+Jenkins - zhusongziye的博客 - CSDN博客





2018年12月20日 21:34:53[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：126








> 
作者：葡萄城技术开发团队

www.cnblogs.com/powertoolsteam/p/9881398.html




上篇文章我们已经完成了API测试工具选型，接下来是一系列周期性的开发测试过程：接口开发、检出代码、运行测试、记录结果、发送报告。为了快速发现问题，并减少重复过程以节省时间、费用和工作量，我们需要一套完整的持续集成解决方案，除接口开发之外其他环节全部自动完成，无需太多的人工干预。

这篇文章将对持续集成解决方案进行一步一步的讲解，希望对大家能有帮助。



**1. Web Api 测试工具选型**

目前市场有很多的用于API 测试的工具，如Postman, SoapUI, YApi, HttpRunner等等。

在进行了很多的尝试后，我们最终还是决定在Postman和Soap UI间做出抉择，在经过我们详细的对比后：https://www.cnblogs.com/powertoolsteam/p/9772211.html

最终我们选择了Postman。



**2. 用Postman创建项目**

选型做好了，第二步当然是Postman用起来了，创建自己的项目。参照Postman官网的文档。https://learning.getpostman.com/docs/postman/collections/intro_to_collections/
- 
创建 LeyserkidsApi项目


![](https://img-blog.csdnimg.cn/20181220212831482.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**3. 源码管理**

因为我们项目使用TFS做源码管理，这里就以TFS为例。
- 
把Postman中项目LeyserkidsApi的Collection和Envrionment导出为json文件。

- 
TFS创建项目LeyserKids，追加Postman导出的JSON文件，进行资源版本管理。接下来展示的项目结构如下：




![](https://img-blog.csdnimg.cn/20181220212909104.png)

**4. Jenkins服务搭建**

我们称搭建Jenkins Server的机器为：机器A，后续步骤需要对这台机器安装一些Services。



**5. Jenkins与TFS关联**

创建FreeStyle Project ：Test

![](https://img-blog.csdnimg.cn/20181220212944495.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181220212954158.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



Jenkins主页->Manage Jenkins->Manage Plugins->找到Team Foundation Server插件并安装。

![](https://img-blog.csdnimg.cn/20181220213013750.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



进入Project Test详细页面->Configure->Source Code Management

![](https://img-blog.csdnimg.cn/20181220213049267.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



Build Triggers 设置当源码发生变化时，重新构建。可以每天定时构建。

![](https://img-blog.csdnimg.cn/20181220213104835.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**6. Jenkins构建（Build）**

Newman是用命令行来执行Postman的Collection。可以很容易的和Jenkins集成。Newman是基于Nodejs的，所以首先得安装Nodejs。
- 
在机器A（Jenkins Server）上安装nodejs和newman。参照：https://learning.getpostman.com/docs/postman/collection_runs/command_line_integration_with_newman

- 
进入Test详细页面->Configure ->Build->Add Build Step->Execute Windows batch command->command


![](https://img-blog.csdnimg.cn/20181220213125126.png)



**7. 邮件配置**

Jenkins构建之后，发送邮件，通知构建结果。邮件配置步骤如下：

Jenkins主页->ManageJenkins->Manage Plugins->找到Email Extension Plugin插件并安装->插件安装之后需要重启jenkins 服务

![](https://img-blog.csdnimg.cn/20181220213138176.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



Jenkins主页->Manage Jenkins->Configure System-> Extended E-mail Notification， 配置邮件Server

![](https://img-blog.csdnimg.cn/20181220213149619.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



进入Test详细页面->Configure-> Post-build Actions->Add post-build action（Editable-Email Notification）
- 
进入Test详细页面


![](https://img-blog.csdnimg.cn/20181220213159884.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)
- 
对Test进行配置


![](https://img-blog.csdnimg.cn/20181220213206668.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)
- 
Post-build Actions追加动作：Editable-Email Notification


![](https://img-blog.csdnimg.cn/20181220213214370.png)
- 
配置Editable-Email Notification


![](https://img-blog.csdnimg.cn/20181220213222283.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**8. 项目构建**

![](https://img-blog.csdnimg.cn/20181220213233179.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



经过以上步骤的配置，构建完成之后，会收到邮件，根据链接在console output查看构建结果，不是很直观。下一步中将进行改善。

![](https://img-blog.csdnimg.cn/20181220213248321.png)



**9. 更优美的邮件模板**

默认的邮件内容过于简单，用Email Extension Plugin 提供的Groovy标准HTML模板:groovy-html.template

进入Test详细页面->Configure-> Editable-Email Notification-> Default Content配置模板

![](https://img-blog.csdnimg.cn/20181220213259235.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

重新Build，查看邮件，邮件内容变的易读行，发送错误红色显示很是醒目。成功则显示为绿色，一目了然。

![](https://img-blog.csdnimg.cn/20181220213336307.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**10. 更简洁精炼，美观的构建报告：Html Report**

查看Console Output 中的构建结果，易读性差，而且也不美观，所以我们要改进。用newman生成HTML Report，再通过HTML Publisher插件展示到Jenkins。

Jenkins主页->ManageJenkins->Manage Plugins->找到HTML Publisher插件并安装。

![](https://img-blog.csdnimg.cn/20181220213352455.png)

在机器A（Jenkins Server）上通过Nodejs安装reporter-html-template$ npm install -g newman-reporter-html

newman-reporter-html使用参见：https://github.com/postmanlabs/newman-reporter-html#readme

环境准备好了，把准备好的模板：htmlreqres.hbs上传到TFS项目的templates下。我用了https://github.com/MarcosEllys/awesome-newman-html-template 这个小伙伴的模板，进行了些许改造。

![](https://img-blog.csdnimg.cn/20181220213401460.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

进入Test详细页面->Configure-> Execute Windows batch command->command

把导出的index.html放在了当前Job对于的newman文件夹（文件夹会自动创建）下。此路径将在下一步配置中使用。

![](https://img-blog.csdnimg.cn/20181220213413142.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

进入Test详细页面->Configure-> Post-build Actions->Add post-build action（Publish Html report）配置Html Report ，展示上一个步骤生成的Index.html文件。

![](https://img-blog.csdnimg.cn/20181220213420317.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

重新构建，查看生成的HTML Report，结果如下，是不是好很多
- 
进入Test详细，查看最新Html Report


![](https://img-blog.csdnimg.cn/2018122021342949.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)
- 
查看某次构建的Report，选中一个build history，查看HTML Report。


![](https://img-blog.csdnimg.cn/20181220213437234.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)
- 
Report详细


![](https://img-blog.csdnimg.cn/2018122021344665.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

如果你的Report样式或JS脚本加载不正确，解决方案如下：

https://testerhome.com/topics/9476



以上，内容比较多，写的比较粗，欢迎交流。



