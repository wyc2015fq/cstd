# Eclipse 安装Groovy插件 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年04月30日 22:22:46[boonya](https://me.csdn.net/boonya)阅读数：17663









**Eclipse集成Groovy插件（以eclipse-4.3.0为例）**


步骤一：


          下载eclipse4.3.0，地址：[http://www.eclipse.org/downloads/。](http://www.eclipse.org/downloads/)


步骤二：


启动eclipse，点击help -> Install New Software...
![](http://dl2.iteye.com/upload/attachment/0089/6949/04c9f73b-c47d-3c99-bad4-369dcdcb1aee.png)



在弹出的窗口中点击：Add...
![](http://dl2.iteye.com/upload/attachment/0089/6960/085811ae-386e-3149-a425-4be0151c52b5.png)



从Groovy官网复制安装groovy插件的地址：根据你当前eclipse版本选择对应的地址


Groovy官网：http://groovy.codehaus.org/


Groovy插件的地址：``[http://dist.springsource.org/release/GRECLIPSE/e4.3/](http://dist.springsource.org/release/GRECLIPSE/e4.3/)``




![](http://dl2.iteye.com/upload/attachment/0089/6964/88ab979e-0ea4-39ad-b024-e9287e445c86.png)



在弹出的Install窗口中填写如下：


         Name:可任意填写名称，如：Groovy plugin


         Location:将上面的地址复制到这里，如：

`[http://dist.springsource.org/release/GRECLIPSE/e4.3/](http://dist.springsource.org/release/GRECLIPSE/e4.3/)点击OK`


![](http://dl2.iteye.com/upload/attachment/0089/6962/98bc2f14-d6c0-32ef-b3af-b919e1da96a6.png)



点击Select All，点击Next，再点finish。eclipse会自动进行下载（由于我的eclipse已经安装了Groovy，点不了Next和Finish,还望谅解，到了这里正常情况就不会出错了，如果eclipse进行下载的时候失败了，可以重头开始重做一遍）
![](http://dl2.iteye.com/upload/attachment/0089/6966/01c1b49b-2790-302c-8367-94965f490621.png)



步骤三：新建你的第一个Groovy项目


      右键 -> New -> Other...
![](http://dl2.iteye.com/upload/attachment/0089/6970/0bb934b8-71a1-3a4f-af7e-817e2632a04f.png)



在搜索框中输入groovy，选择Groovy Project点击Next -> Finish完成在Eclipse中的第一个Groovy项目的建立
![](http://dl2.iteye.com/upload/attachment/0089/6972/e0dfc933-8653-37ad-912d-71de60048553.png)




