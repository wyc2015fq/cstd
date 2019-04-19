# 常用linux命令 - KinboSong的博客 - CSDN博客
2017年05月10日 19:20:15[songjinbo3](https://me.csdn.net/KinboSong)阅读数：154
个人分类：[C++开发](https://blog.csdn.net/KinboSong/article/category/6854166)
定位软件包的命令：
locate <软件包名字> (*可以不是完整的名字，如libopencv-dev可以直接输入opencv来查询)
可以显示头文件和库文件所在的地方
which <软件包名字> 
显示可执行文件在的地方，和man手册在的位置
whereis <软件包名字>
和which一样
终端快捷键：
ctrl+shift+t：在以打开的终端中再以选项卡的形式添加一个终端
alt+1,2,3,：在终端有多个选项卡时，选择不同的选项卡
ctrl+a：移动到命令行首
ctrl+e：移动到命令行尾
ctrl+w:  清除光标前的一个单词
ctrl+k:   清除光标处到开始位置的所有内容
ctrl+u：清除光标处到终点位置的所有内容
ctrl+<方向键>： 以单词为单位移动光标
ctrl+alt+t：打开新终端
ctrl+d：关闭当前终端
ctrl+l：清屏
ctrl+r：搜索历史命令（按esc键退出）
ctrl+ --->：光标向右移动一个单词
ctrl+ <---：光标向左移动一个单词
查看系统运行的情况：
(1)top （2）free (3)ps
杀死进程
kill PID
软链接和硬链接：
软链接可以跨系统（如windows和linux），并且每创建一个软链接就会创建一个inode；硬链接则都是指向同一个inode。
语句：ln -s 源文件 目标文件
查看磁盘情况：
du命令：查看当前目录下各文件的磁盘空间
显示时间、日期的命令：
date
apt常用命令：
在Ubuntu下软件源的文件是/etc/apt/sources.list；/etc/apt/sources.list.d/目录下的文件是第三方软件源地址
1、apt-get类：
　　apt-get install packagename 安装包
　　apt-get install packagename --reinstall 重新安装包
　　apt-get -f install packagename 修复安装”-f = –fix-missing”
**apt-get -f install **是修复依赖关系（depends）的命令，就是假如你的系统上有某个package不满足依赖条件，这个命令就会自动修复，安装那个package依赖的package。
　　apt-get remove packagename 删除包
　　apt-get remove packagename --purge 删除包，包括删除配置文件等
　　apt-get update 更新源
　　apt-get upgrade 更新已安装的包
　　apt-get clean 清理无用的包
　　apt-get check 检查是否有损坏的依赖
      apt-cache search packagename 搜索包
　　apt-cache show packagename 获取包的相关信息，如说明、大小、版本等
　　apt-cache depends packagename 了解使用依赖
　　apt-cache rdepends packagename 是查看该包被哪些包依赖
dpkg常用命令：
dpkg -i packagename.deb #安装包 
dpkg -r packagename #删除包 
dpkg -P packagename #删除包（包括配置文件） 
dpkg -L packagename #列出与该包关联的文件 
dpkg -l packagename #显示该包的版本 
dpkg -l #列出当前已安装的包
环境变量问题：
env显示用户的环境变量；
set 显示当前shell的定义的私有变量，包括用户的环境变量，按变量名称排序；
export 显示当前导出成用户变量的shell变量，并显示变量的属性(是否只读)，按变量名称排序；
declare 同set 一样，显示当前shell的定义的变量，包括用户的环境变量
declare [+/-][选项] 变量名
选项：
   -：给变量舍得类型属性
   +：取消变量的类型属性
  -a：将变量声明为数组型
  -i：将变量声明为整型
  -x：将变量声明为环境变量
  -r：将变量声明为只读变量
  -p：查看变量的被声明的类型
查看本文件夹一级目录的大小：
du -h --max-depth=1
查看linux命令的使用方法：
1、help <命令>
2、<命令> --help
3、man <命令>
