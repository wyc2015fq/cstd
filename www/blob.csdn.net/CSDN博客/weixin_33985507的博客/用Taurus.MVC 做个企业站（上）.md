# 用Taurus.MVC 做个企业站（上） - weixin_33985507的博客 - CSDN博客
2016年08月16日 09:07:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
# 前言：
之前是打算写一篇文章叫：Taurus.MVC 从入门到精通，一篇完事篇！
后来转指一念，还是把教程集在这个企业站项目上吧！！！
# 企业站风格：
之前发过一个帮师妹写的企业站：[最近花了几个夜晚帮师妹整了一个企业网站](http://www.cnblogs.com/cyq1162/p/3573726.html)
技术风格是：文本数据库(txt)+WebForm
转型的风格：文本数据库(txt)+Taurus.MVC
今天要完成的界面主要是首页：
![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815211949093-179801402.jpg)
OK，下面开始，看看一步一步怎么从无到有的做这个企业项目：
# 1：新建ASP.NET空的Web应用程序项目：
![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815212221343-89583303.jpg)
# 2：添加一个类库项目，叫Taurus.Controllers
![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815212413359-851128632.jpg)
# 3：在Taurus.Controllers项目上用Nuget引用Taurus.MVC：
![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815213555546-101558165.jpg)
当然，你也可以用源码项目，或者找到源码里的两个DLL添加引用：
![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815213751406-969399371.jpg)
注意EnterPriseSite项目需要引用Taurus.Controllers项目，毕竟编绎后的DLL要集中在EnterPriseSite项目上的。
# 4：在EnterPriseSite项目上添加Views文件夹，和几个空页面：
default.html等页面没放到default文件夹，是不对的，后面的截图会修正。
![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815214134140-1985750123.jpg)
# 5：从原WebForm项目里把Style、App_Data目录下的文本数据库Copy过来：
顺便把项目名称改成EnterPriseSite.View
![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815214438125-426006235.jpg)
# 6：把原项目的用户控件的html标签Copy过来，放到Shared目录下master.html里：
![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815214545390-1644609426.jpg)![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815214700640-795341470.jpg)
用Taurus.MVC的时候，就木有Repeater的概念了，只有Html和JS。
**有业务条件代码的，可以用JS处理，也可以后台处理，这里先用Js处理（后面有演示后台处理的）：**
![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815214804093-847679335.jpg)
**模板页的节点可以随便放的，只要节点有名称（id或name)，就可以被其它html引用。**
**引用的属性名称可以是id,name或者个别tag如（head,body,title,script,style,form,meta,link）**
**所以你能用master.head,master,body,master.title,master.script（都只取第一个节点）...**
# 7：从原来的界面复制Default页面的标签过来：
 原来的Default.aspx：
![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815215818406-1327502485.jpg)
现在Default.html
![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815215841250-1921045925.jpg)
界面都搞完了，现在开始要写代码了：
# 8：新建DefaultController.cs，同时把CodeFirst的几个表类Copy过来：
弄了个Logic文件夹来放，项目简单的时候，就不想建太多工程，用文件夹了：
![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815220107859-2125175894.jpg)
然后，写几个方法上去，这样才能加载到对应的html文件：
![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815220445984-501237012.jpg)
**所以html都是放在/views/default/目录下的。**
**补充说明：**
默认访问路径是：localhost/default/index、locahost/default/artilelist...
为了把default去掉，默认的路由我补充了一个；以前路由模式只有1和2，现在多了个0。
# 9：看看Web.Config配置：
![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815221309765-1406093337.jpg)
# 10：写逻辑代码绑定页面：
由于项目简单，我把业务逻辑，直接建了一个类，扔Controller里面了，叫DefaultLogic。
为了分层清晰，同学们还是自己新建一个Taurus.Logic类库项目放。
**逻辑类需要继承自Taurus.Core.LogicBase，这样才能传递View对象到逻辑类中，注意有构造函数哦。**
![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815221615875-582326010.jpg)
**由于html的节点id按约定：表名View，所以Bind（View）就可以了，不需要指定名称。**
![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815221718734-1356398969.jpg)
**这里多了一个事件View.Onforeach，用来格式化界面呈现的时间的：**
![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815221852250-898178364.jpg)
当然后台不先格式好化，那就前台去处理了，就像上面的截图中有一个是用JS处理的代码。
# 11：Controller调用逻辑代码，渲染页面：
![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815222335109-1346588678.jpg)
最后，一个漂亮的首页就出来了：
![](https://images2015.cnblogs.com/blog/17408/201608/17408-20160815222739859-1422879440.jpg)
# 打完收工：
后续几个页面，包括文章列表，文章详情、产品中心，还有后台管理，下篇再介绍。
以前都是整个项目做完再写文章的，现在项目写一半，文章补一篇，哈。。。
源码：
网址打开：[http://code.taobao.org/p/cyqopen/src/trunk/Taurus.MVC.GettingStarted/](http://code.taobao.org/p/cyqopen/src/trunk/Taurus.MVC.GettingStarted/)
svn checkout: http://code.taobao.org/svn/cyqopen/trunk/Taurus.MVC.GettingStarted
感谢大伙支持！！！
