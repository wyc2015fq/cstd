
# git初级--配置 - 致林 - 博客园






# [git初级--配置](https://www.cnblogs.com/bincoding/p/6110755.html)
source: http://www.cnblogs.com/sakurayeah/p/5800424.html
## 一、注册github账号
github网址[https://github.com/](https://github.com/)
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160823173904620-513665781.png)
下一步
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160823174419667-499800904.png)
然后，你的邮箱会收到一封邮件，进行邮箱验证就行了

## 二、创建github仓库
登录你的github，在首页的右方可以看到下图所示（我已经创建过项目了，没有创建过项目的显示有点不一样，但是都是这个按钮）
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160823174916151-182564924.png)
下一步
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160823175840761-1601000420.png)
创建好了，如下图所示
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824092156542-466040332.png)

## 三、配置SSH keys
首先要安装git（下载地址[https://www.git-scm.com/download/](https://www.git-scm.com/download/)）
新建一个文件夹（例如workspace，用来存放你所有的项目），在workspace文件夹下git bash here（可以直接在workspace文件夹下右键，如下图所示）
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824091252011-230566840.png)
下一步
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824091825339-175182132.png)
下一步，在github上操作，找到setting（如下图所示）
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824092347995-528738101.png)
下一步（注意，这里需要上面要求复制的东西，往上数第二张图）
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824102708011-1542151983.png)
下一步，输入你的密码
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824092859808-1651374431.png)
好了就能看见下图所示的东西
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824093000808-1076976553.png)
你的邮箱
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824093129995-1724000973.png)

## 四、git 配置用户名和邮箱
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824093459683-368503162.png)

## 五、上传你的项目
github上，找到刚创建的仓库abc
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824093734136-268345529.png)
在git上操作
git clone "复制的网址"
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824094130230-901643427.png)
这时候在workspace文件夹里面就有了abc文件夹
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824094324464-816154454.png)
进入abc文件夹，可以在git里面输入 cd abc，也可以直接打开abc文件夹，右键git bash here
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824094532542-454153514.png)
然后将你的项目放在abc文件夹下面就行了
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824094734026-1273312971.png)
下面上传到github上面去
git status 可以查看你修改了哪些文件（下面红色标出来的）
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824094903683-80903394.png)
git add .
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824095123448-1648266409.png)
git commit -m "这里写你本次更新的备注，比如更新了什么功能或什么文件等"
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824095252855-1385872510.png)
git push
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824095405714-1998274956.png)
上图所示，有master ->master的时候，就上传成功了
在git push的时候，可能要求你输入你的用户名和密码（如下图所示）
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824102800370-1554223320.png)
当你上传成功了，就可以去github上面看一下，你新建的文件都上传上去了，如下图所示
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824095627292-145695124.png)
每次你修改了你的项目（abc文件里面的东西修改了，这里的修改包括修改代码、添加文件、删除文件），都需要执行这几步命令（如下所示），在github上面更新
git add .
git commit -m "备注"
git push
上面的git add . 一定不要忘记有个点
git add . 和 git commit -m "备注" ， 可以合并成为git commit -a -m "备注"
另外还有一个常用的git命令
git pull 更新本地的代码（多人合作管理一个仓库的时候需要用到，别人push了代码，那你就需要pull一下，把别人写的代码更新到本地来）
下面会介绍怎么给一个项目添加其他人来一起管理仓库
这里附一张git命令图
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824100127870-1820786836.png)

## 六、添加其他人一起管理abc仓库
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824101319886-1064908989.png)
下一步
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824101515401-1115433367.png)
下一步
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824102900261-959872590.png)
下面这张图是合作者打开链接后的图
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824101955651-112995667.png)
对方同意了之后，下图所示
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824102042886-445625856.png)

## 七、删除仓库
这里演示删除abc仓库
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824100319401-770758857.png)
就可以看到下图所示
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824100407870-787565754.png)
往下滚动页面，找到下图所示
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824100445401-670428246.png)
下一步
![](https://images2015.cnblogs.com/blog/1009686/201608/1009686-20160824100623823-1321738021.png)





