# Linux环境变量设置指南 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月01日 17:08:37[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：171
个人分类：[未分类](https://blog.csdn.net/elesos/article/category/6361263)

以配置java环境变量为例
## 目录
 [[隐藏](http://172.30.24.252/wiki_elesos_com/index.php?title=Linux%E7%8E%AF%E5%A2%83%E5%8F%98%E9%87%8F%E8%AE%BE%E7%BD%AE%E6%8C%87%E5%8D%97#)] 
- [1 修改/etc/profile文件](http://172.30.24.252/wiki_elesos_com/index.php?title=Linux%E7%8E%AF%E5%A2%83%E5%8F%98%E9%87%8F%E8%AE%BE%E7%BD%AE%E6%8C%87%E5%8D%97#.E4.BF.AE.E6.94.B9.2Fetc.2Fprofile.E6.96.87.E4.BB.B6)
- [2 修改用户目录下的.bash_profile](http://172.30.24.252/wiki_elesos_com/index.php?title=Linux%E7%8E%AF%E5%A2%83%E5%8F%98%E9%87%8F%E8%AE%BE%E7%BD%AE%E6%8C%87%E5%8D%97#.E4.BF.AE.E6.94.B9.E7.94.A8.E6.88.B7.E7.9B.AE.E5.BD.95.E4.B8.8B.E7.9A.84.bash_profile)
- [3 修改.bashrc文件](http://172.30.24.252/wiki_elesos_com/index.php?title=Linux%E7%8E%AF%E5%A2%83%E5%8F%98%E9%87%8F%E8%AE%BE%E7%BD%AE%E6%8C%87%E5%8D%97#.E4.BF.AE.E6.94.B9.bashrc.E6.96.87.E4.BB.B6)
- [4 直接在shell下设置](http://172.30.24.252/wiki_elesos_com/index.php?title=Linux%E7%8E%AF%E5%A2%83%E5%8F%98%E9%87%8F%E8%AE%BE%E7%BD%AE%E6%8C%87%E5%8D%97#.E7.9B.B4.E6.8E.A5.E5.9C.A8shell.E4.B8.8B.E8.AE.BE.E7.BD.AE)
- [5 查看环境变量](http://172.30.24.252/wiki_elesos_com/index.php?title=Linux%E7%8E%AF%E5%A2%83%E5%8F%98%E9%87%8F%E8%AE%BE%E7%BD%AE%E6%8C%87%E5%8D%97#.E6.9F.A5.E7.9C.8B.E7.8E.AF.E5.A2.83.E5.8F.98.E9.87.8F)
- [6 删除环境变量](http://172.30.24.252/wiki_elesos_com/index.php?title=Linux%E7%8E%AF%E5%A2%83%E5%8F%98%E9%87%8F%E8%AE%BE%E7%BD%AE%E6%8C%87%E5%8D%97#.E5.88.A0.E9.99.A4.E7.8E.AF.E5.A2.83.E5.8F.98.E9.87.8F)
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=Linux%E7%8E%AF%E5%A2%83%E5%8F%98%E9%87%8F%E8%AE%BE%E7%BD%AE%E6%8C%87%E5%8D%97&action=edit&section=1)]修改/etc/profile文件
此法对所有用户生效
在文件末尾加入以下内容：
JAVA_HOME=**/**usr**/**share**/**jdk1.5.0_05
PATH=$JAVA_HOME**/**bin:$PATHCLASSPATH=.:$JAVA_HOME**/**lib**/**dt.jar:$JAVA_HOME**/**lib**/**tools.jar
**export** JAVA_HOME
**export** PATH
**export** CLASSPATH
生效需要重新登录，如果想要马上生效需要运行
# **source****/**etc**/**profile
注释：
- 用冒号“:”分隔路径
- CLASSPATH中当前目录“.”不能丢
- export是把这三个变量导出为全局变量
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=Linux%E7%8E%AF%E5%A2%83%E5%8F%98%E9%87%8F%E8%AE%BE%E7%BD%AE%E6%8C%87%E5%8D%97&action=edit&section=2)]修改用户目录下的.bash_profile
此法对单一用户生效
**vi****/**home**/**elesos**/**.bash_profile
添加如下内容：
**export**CLASSPATH=.**/**JAVA_HOME**/**lib;$JAVA_HOME**/**jre**/**lib
如果想要马上生效需要运行
$ **source****/**home**/**elesos**/**.bash_profile
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=Linux%E7%8E%AF%E5%A2%83%E5%8F%98%E9%87%8F%E8%AE%BE%E7%BD%AE%E6%8C%87%E5%8D%97&action=edit&section=3)]修改.bashrc文件
此法可以把环境变量的使用权限控制到用户级别，只需要修改该用户主目录下的.bashrc文件就可以了。
在.bashrc文件末尾加入：
**set**JAVA_HOME=**/**usr**/**share**/**jdk1.5.0_05
**export** JAVA_HOME
**set**PATH=$JAVA_HOME**/**bin:$PATH**export** PATH
**set**CLASSPATH=.:$JAVA_HOME**/**lib**/**dt.jar:$JAVA_HOME**/**lib**/**tools.jar
**export** CLASSPATH
然后重新登录。
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=Linux%E7%8E%AF%E5%A2%83%E5%8F%98%E9%87%8F%E8%AE%BE%E7%BD%AE%E6%8C%87%E5%8D%97&action=edit&section=4)]直接在shell下设置
不赞成这种方法，因为换个shell你的设置就无效了，因此这种方法仅仅是临时使用，以后要使用的时候又要重新设置，比较麻烦。
只需在shell终端执行类似下列命令即可：
**export**PATH=$JAVA_HOME**/**bin:$PATH
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=Linux%E7%8E%AF%E5%A2%83%E5%8F%98%E9%87%8F%E8%AE%BE%E7%BD%AE%E6%8C%87%E5%8D%97&action=edit&section=5)]查看环境变量
可输入命令export进行查看，也可以通过echo命令：
# **echo**$PATH
或使用env命令显示所有的环境变量
Windows下可用“set 环境变量名”进行查看
## [[编辑](http://172.30.24.252/wiki_elesos_com/index.php?title=Linux%E7%8E%AF%E5%A2%83%E5%8F%98%E9%87%8F%E8%AE%BE%E7%BD%AE%E6%8C%87%E5%8D%97&action=edit&section=6)]删除环境变量
**unset**$TEST
