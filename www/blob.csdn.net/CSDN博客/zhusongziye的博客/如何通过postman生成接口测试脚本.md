# 如何通过postman生成接口测试脚本 - zhusongziye的博客 - CSDN博客





2017年09月21日 22:16:20[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：9563








下面再给大家介绍如何通过postman生成接口测试脚本。接口测试环境：Python+requests（requests库，可以通过cmd命令窗口pip
 install requests安装）

我们打开postman，使用V2EX
 API 接口平台提供的接口去测试我们要测试的接口。

请求方式是GET。输入接口地址：https://www.v2ex.com/api/members/show.json?id=1，界面如下：

![](https://img-blog.csdn.net/20170921221643738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




可以看到，接口访问成功，返回的是Json字符串。这个时候我们点击红色框内的Code，弹出如下界面，可以看出，和selenium
 IDE一样，可以选择多语言，因为今天主讲的是python语言，所以我们选择python-->Request,关于Request介绍可查看http://cn.python-requests.org/zh_CN/latest/user/quickstart.html#id2这个文章，这个文章写的是真的好。

![](https://img-blog.csdn.net/20170921221715629?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




选择python-->Request后生成代码，我们把该代码复制到python编辑器中。下面是生成的脚本：

![](https://img-blog.csdn.net/20170921221747312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




执行结果如下：

![](https://img-blog.csdn.net/20170921221810254?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Postman生成接口自动化脚本就是这么简单，postman 也是我非常喜欢的一款接口测试工具，因为他真很强大！





