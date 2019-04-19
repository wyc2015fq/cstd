# UML建模详解（7）—Rose时序图绘制总结 - fanyun的博客 - CSDN博客
2016年08月11日 09:18:00[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：5082
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
### 使用Rose创建序列图案例分析
#### 1. 需求分析
我们可以通过更加具体的描述来确定工作流程，基本工作流程如下：
    （1）客户希望通过系统查询某名学生的学科成绩。
    （2）客户通过用户界面写入自己的账户信息。
    （3）用户界面根据客户的ID号向数据库访问层请求客户信息。
    （4）数据库访问层根据客户的ID加载客户信息。
    （5）数据库访问层根据客户信息和申请业务获取该客户的账户信息。
    （6）数据库访问层将客户信息和账户信息提供给用户界面。
    （7）用户界面将客户信息和账户信息显示出来。
#### 2. 确定序列对象
     建模序列图的下一步是从左到右布置在该工作流程中所有的参与者和对象，同时也包含要添加消息的对象生命线。
![](https://img-blog.csdn.net/20160614174425562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
