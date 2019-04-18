# Linux系统修改PATH环境变量方法 - Likes的博客 - CSDN博客
2019年03月01日 11:36:53[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：7标签：[Linux																[PATH																[环境变量																[增																[删](https://so.csdn.net/so/search/s.do?q=删&t=blog)](https://so.csdn.net/so/search/s.do?q=增&t=blog)](https://so.csdn.net/so/search/s.do?q=环境变量&t=blog)](https://so.csdn.net/so/search/s.do?q=PATH&t=blog)](https://so.csdn.net/so/search/s.do?q=Linux&t=blog)
个人分类：[Linux基本操作](https://blog.csdn.net/songchuwang1868/article/category/8629581)
# [Linux系统修改PATH环境变量方法](https://www.cnblogs.com/cursorhu/p/5806596.html)
在Linux安装一些软件通常要添加路径环境变量PATH.PATH环境变量通俗的讲就是把程序的路径"备案"到系统中,这样执行这些程序时就不需要输入完整路径,直接在bash输入程序名就可以执行.比如常用的ls命令就是添加好了环境变量才可以直接执行ls
**0查看PATH环境变量**
终端输入echo $PATH返回如下,各路径用:隔开. $符号用于展开变量的值.
echo $PATH
/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games
**1添加路径到PATH**
添加路径可以直接给PATH赋值
export PATH=$PATH:新添加的路径
$PATH是当前的路径1:路径2:路径3 ,在后面追加:新添加的路径,然后把这个新的路径集赋值给PATH本身,相当于覆盖了原PATH变量.export命令是导出变量,相当于更新PATH
**2删除PATH中的路径**
删除PATH中的某个路径也是用以上重新给PATH赋值的方法,比如当前PATH是
echo $PATH
路径1:路径2:路径3:路径4
要删除路径4,只需要copy上面的路径1:路径2:路径3
然后赋值给PATH
export PATH=路径1:路径2:路径3
**3在系统文件修改环境变量**
修改环境变量常用3种方法:
1.直接在命令行输入语句
2.在文本文件(常命名为为.sh脚本文件,也可无后缀)中写语句,source这个文件使修改生效
3.在系统文件(如HOME下的.bashrc)中写语句,source或重启生效
用命令行修改PATH,只针对当前shell有用,关了终端就失效了
普通文本文件修改PATH每次登陆需要source,常用在安装软件和交叉编译
系统文件中修改相当于每次登陆系统自动source,参考[添加环境变量到系统级或用户级的文件中](http://blog.csdn.net/netwalk/article/details/9455893)
(推荐用root账户修改系统文件,[设置root账户的方法](http://www.cnblogs.com/cursorhu/p/5803072.html))
登陆root,在家目录(root)的.bashrc文件添加新路径到PATH
export PATH=$PATH:新增路径
更直接的方法:在.bashrc写明PATH的所有路径,注意先要echo $PATH然后copy系统自带的路径
export PATH=路径1:路径2:路径n
这样增加和删除路径直接改写.bashrc即可
要立即生效只需要source一下.bashrc文件,之后无需再source 
source .bashrc
.bashrc中的修改对于当前账户的每次登陆都有效
要恢复系统默认PATH,删掉.bashrc中的修改语句即可
