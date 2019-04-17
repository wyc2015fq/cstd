# Docker构建JIRA BUG跟踪管理工具镜像 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年09月07日 11:21:10[boonya](https://me.csdn.net/boonya)阅读数：3322









JIRA是Atlassian公司出品的项目与事务跟踪工具，被广泛应用于缺陷跟踪、客户服务、需求收集、流程审批、任务跟踪、项目跟踪和敏捷管理等工作领域。JIRA中配置灵活、功能全面、部署简单、扩展丰富，其超过150项特性得到了全球115个国家超过19,000家客户的认可。通过Docker镜像我们可以快速的将JIRA纳入到我们的日常BUG管理事务中。Jira是商业的BUG跟踪，提供30天的试用期，如果有能力还是建议购买使用。





## Dockerfile构建Jira镜像





构建文件位于：[https://github.com/docker-atlassian/jira](https://github.com/docker-atlassian/jira)只需要下载zip包到centos就可以进行构建了。




镜像构建命令：docker build -t boonyadocker/centos-jira .

容器运行命令：docker run --name myjira  -t -p 9999:8080 boonyadocker/centos-jira





公共仓库镜像：docker pull boonyadocker/centos-jira




![](https://img-blog.csdn.net/20170907111001826)





## Jira常用特性




### BUG管理

![](https://img-blog.csdn.net/20170907111352170)

文件上传：

![](https://img-blog.csdn.net/20170907111437434)

### 项目管理

![](https://img-blog.csdn.net/20170907111513231)




### 活动管理



![](https://img-blog.csdn.net/20170907111550057)






### 用户管理





![](https://img-blog.csdn.net/20170907111617430)





### 邮件通知


![](https://img-blog.csdn.net/20170907111732070)





### 系统仪表盘





![](https://img-blog.csdn.net/20170907111803306)

可以添加Dashboard:

![](https://img-blog.csdn.net/20170908131254118)






### 项目看板





![](https://img-blog.csdn.net/20170907111924759)





更多特性后面慢慢再研究。还是再强调一下：Jira是商业的BUG跟踪，提供30天的试用期，如果有能力还是建议购买使用。







