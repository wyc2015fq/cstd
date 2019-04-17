# linux 基本操作随笔 - miner_zhu的博客 - CSDN博客





2018年10月18日 19:05:33[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：29








### 一、创建/删除文件

创建：

创建一个名为aa的文件夹：mkdir aa

递归创建一个层级目录：    mkdir aa/bb/cc

创建文件：touch xx.html （已存在会更新时间）

删除：

删除空目录: rmdir

删除xx文件夹以及其下所有内容：rm -rf  /var/log/xx

删除文件access.log： rm -f  /hte/access.log

查看目录层级情况：

tree aa： `|——aa　　|——bb　　　|——cc`

###  二、移动/重命名文件

重命名：将aa重命名为bb：mv aa bb

移动：将aa移动到/home/hao文件夹下：mv aa   /home/hao 

### 三、拷贝目录 

拷贝aa到目录/home/hao下并更名为bb：cp -r aa /home/hao/bb

### 四、打开文件

打开文件：cat index.html

### 五、more//less打开部分文件

more /home/log  分批打开/home/log文件，按回车打开更多

head -5  /home/log   显示文件前几行

tail  /home/log   显示为文件后几行

###  六、WC

测量文件index.html的行数/单词数/字节数： wc  index.html

显示：7 10 48 index.html

即7行 10个单词 48个字符 文件名index.html

### 七、du查看文件的大小

du -hm aa  以M为单位显示文件aa的大小 

du -hb aa   以B为单位显示文件aa的大小

du -hk aa   以K为单位显示文件aa的大小

du -sh *     查看当前目录文件及文件夹大小

### 八、df显示磁盘的使用情况

以GB为单位查看使用情况：df --block-size=GB 

### 其他

vim中zi可以展开和折叠隐藏内容

### 基本操作

日期时间：date

日历：cal

计算器：bc  小数点位数 scale=3

命令补齐和文件名补齐：tab键

       接在第一个字段后为 命令补齐

       接在第二个字段后为 文件名补齐

       在命令 --后两次tab显示所有参数

终止当前程序：ctrl + c

键盘输入结束：ctrl + d

命令行翻页：shift +【PgUp】【PgDn】

查看命令的选项参数：命令名 + --help

查看命令的详细信息或文件格式：man + 命令名

第一页：home键

最后一页：end键

向下查找：/字符串

向上查找：?字符串

下一个：n

上一个：N










