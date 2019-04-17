# qtcreator使用git和gitk工具管理自己的代码，可回溯至历史版本代码 - 站在巨人的肩膀上coding - CSDN博客





2018年05月12日 14:23:56[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：482








        本文带大家了解一个深受广大开发者稀饭的版本控制器git。单独的一条一条git命令也很难记忆，去管理版本会比较麻烦【但是还是需要知道一些版本管理git命令，参考文末连接】。这里我们结合qtcreator开发工具，每实现完一版本的代码都可以提交一次，gitk工具帮你记录了所有的修改，最后借助该gitk工具做历史版本的回退。


# 准备：安装git gitk

sudo apt-get install git

sudo apt-get install gitk

然后设置账户：



 git config --global user.email "you@example.com"

  git config --global user.name "Your Name"

否则会报错：

![](https://img-blog.csdn.net/20180601231425746)



# 一、qtcreator新建项目选择git支持

首先，在新建qtcreator项目的时候加入git版本控制器支持。这时候在菜单栏tool工具栏会出现git选项

![](https://img-blog.csdn.net/20180512135201607)


# 二、建立Repository Directory

   Tools-->Git-->Create Repository，选择目录，为整个项目代码目录。

![](https://img-blog.csdn.net/20180512142329646)




# 三、准备commit代码

    写好第一版代码之后，点击Tool-->Git-->Local Responstory-->Commit，提交代码。

    提交代码的时候可以在descrip栏中填写描述，项目工程版本号，修改地方的描述。Files选择要提交的内容，一般选择所有。

![](https://img-blog.csdn.net/20180512135100125)

# 四、查看历史版本代码

    查看已历史提交及代码修改的位置： Tools-->Git-->Git Tools-->gitk

注意，这里如果报错“无法执行gitk”原因是你没有装gitk(需要单独安装)。sudo apt-get install gitk，安装gitk工具

![](https://img-blog.csdn.net/20180512135744454)


这里可以看到这个版本修改的地方，绿色的++位置就是我们修改的位置。


# 五、还原到历史版本

       假设我们的“socket change”版本写得不好，或者把原有的代码改混乱了，我们想还原到原来的版本1.0那该怎么办呢？很简单，使用gitk就可以解决问题。打开gitk，出现这样的界面：

![](https://img-blog.csdn.net/20180512140749795)  选择1.project，点击右键，选择“Reset master branch to here”，在弹出的对话框中选择Hard，并且单击OK就完成了版本的还原。需要注意的是，选择Hard模式意味着在新版本上的一切修改都作废，所以要慎重处理。即还原到历史那个版本，两个版本之间的所有修改都作废【建议保存一个副本，重新在原来的代码上做修改】

        最后，回到QtCreator界面，发现提示重新载入文件，点击“全部是”就可以了。

![](https://img-blog.csdn.net/2018051214165160)





参考链接：

[1]qtcreator下使用git版本控制系统：https://blog.csdn.net/hcx25909/article/details/8991642   


[2]git命令行使用方法：https://www.cnblogs.com/flipped/p/6659827.html

[3]git使用，精华版：http://www.bootcss.com/p/git-guide/


[4] gitk乱码解决办法：https://blog.csdn.net/u014132720/article/details/51471630

[5] gitk使用方法：https://wenku.baidu.com/view/25301b2fbe23482fb5da4c5b.html




