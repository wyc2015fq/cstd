# Use Case Diagram - Jun5203 - CSDN博客
2018年10月01日 21:20:59[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：83
个人分类：[【UML】](https://blog.csdn.net/Ellen5203/article/category/8065077)
所属专栏：[UML](https://blog.csdn.net/column/details/26918.html)
### 前言
用例图描述系统的静态使用情况，展现了一组用例、参与者以及它们间的关系，在对系统行为组织和建模方面，用例图是相当重要的。
### 芝士
- 什么是用例图？
关键：描述用户的需求
细节：【功能和角色】从用户的角度描述系统的功能，并指出各功能的执行者，强调谁在使用系统，系统为执行者完成哪些功能
- 用例图的基本元素
![](https://img-blog.csdn.net/20181001211403549)- 角色（Actor）
角色是系统外部的一个实体（Everything——系统用户、其他的系统、运行的进程），通过向系统输入或请求系统输入某些事件来触发系统的执行。
- 系统用户
例：某鞋专卖店的导购员，通常情况下，他是导购员但是当他自己要买鞋的时候，就变成了客户。
- 其他的系统
例：超市购物支付需要与外部应用程序建立联系，验证微信以便付款。其中，外部应用程序是一个参与者，是另一个系统。
- 运行的进程
例：在支付宝花呗中，到了还钱的时候客户还没有归还，系统会自动提醒用户。
- 用例
用例是对系统的用户需求（主要是功能需求）的描述，用例表达了系统的功能和所提供的服务。
- 关系
角色与用例之间的关系
![在这里插入图片描述](https://img-blog.csdn.net/2018100209324076?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- 泛化（Generalization）关系
通俗的理解就是继承关系，子用例将继承父用例的所有结构、行为和关系。子用例可以使用父用例的一段行为，可以重载它。
剪头的方向由子用例指向父用例
![在这里插入图片描述](https://img-blog.csdn.net/20181002090103732?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 关联（Association）关系
表示参与者与用例之间的通信，任何一方都可发送或接收消息
![在这里插入图片描述](https://img-blog.csdn.net/20181002090232491?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 包含（Include）关系
指的是其中一个用例（基础用例）的行为包含了另一个用例（包含用例）的行为。
![在这里插入图片描述](https://img-blog.csdn.net/20181002104104264?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
箭头指向被包含的用例
![在这里插入图片描述](https://img-blog.csdn.net/20181002103839832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
用例“查询用户”被用例“修改用户信息”、“查看详细信息”和“删除用户信息”所包含
包含关系通常在以下2种情况下发生：
（1）如果两个以上用例有重复的功能，则可以将重复的功能分解到另一个用例中。其他用例可以和这个用例建立包含关系
（2）一个用例的功能太多时，可以用包含关系创建多个子用例
- 扩展（Extend）关系
指的是对基础用例的增量扩展，也就是说为基础用例提供一个附加功能，通俗的理解就是对用例功能的延伸。
基础用例提供了一组扩展点，在这些扩展点中添加新的行为，而扩展用例提供了一组插入片段，这些片段能够插入到基础用例的扩展点中。
![在这里插入图片描述](https://img-blog.csdn.net/20181002105023330?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
箭头指向被扩展的用例（基础用例）
![在这里插入图片描述](https://img-blog.csdn.net/20181002105532185?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 依赖（Dependency）关系
其中一个模型元素是独立的，另一个模型元素不是独立的，它依赖于独立的模型元素，如果独立的模型元素改变，将影响依赖于它的元素，通俗的理解就是使用关系。
- 用例的主要属性
- 事件流
通俗理解：拨打10086，一系列引导性信息，按下哪个数字就执行哪个过程
- 前置条件
通俗理解：查询账户余额，前提是正确输入账号和密码
- 后置条件
用例（事件）结束时系统的状态
- 特殊要求
- 扩展点
- 问题说明
- 用例的粒度与范围
- 概述级
![在这里插入图片描述](https://img-blog.csdn.net/20181001211306359?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 用户目标级
![在这里插入图片描述](https://img-blog.csdn.net/20181001211317661?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 子功能级
![在这里插入图片描述](https://img-blog.csdn.net/20181001211325394?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
对于用例的粒度的理解也是“大化小”。
### 附加——机房收费系统
用例图是由软件需求分析到最终实现的第一步，它描述人们希望如何使用一个系统。用例图显示谁将是相关的用户、用户希望系统提供什么服务，以及用户需要为系统提供的服务，以便使系统的用户更容易的理解这些元素的用途，也便于软件开发人员最终实现这些元素。
趁热打铁，学到东西就要及时总结，那就跟着我来看看机房收费系统的用例图吧~
首先分享一下我是怎么画的用例图，一开始刚接触这个的时候我是有点抵触的，因为我不知道用例图是什么？它能干什么？后来转念一想，万物皆有它存在的理由，而且学习这件事儿不能将就，然后我的用例图之旅就开始啦！
第一步：视频+书
在这个过程中有点枯燥，也有点激动，话说为什么呢？枯燥是因为视频和讲义根本不同步，很抓狂有没有？要说激动，那是由于自己在学习的过程中，渐渐懂得了它们其中的联系，这对我无异于是最大的激励。
第二步：理清机房收费系统功能间的关系
这个很简单，只要你还记得你是怎么做的机房收费系统就行
第三步：画图+站在巨人的肩膀上
我看了师哥师姐的一些博客，个人有个人的思想，当然，我的思想和他们也不同。
其次，就来看看我的用例图吧~
- 一般用户
![在这里插入图片描述](https://img-blog.csdn.net/2018100216023181?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 操作员
![在这里插入图片描述](https://img-blog.csdn.net/20181002160237824?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 管理员
![在这里插入图片描述](https://img-blog.csdn.net/20181002160245650?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
大千世界，无奇不有，不知我的用例图是否给你带来了帮助，如果您有更好的图解欢迎一起交流，同时，感谢您驻留此博客！
### 小结
连续好几天没有补充“能量”，今天忙里偷闲小酌了一点，感觉还不错哦，明天继续，加油！
