# Linux常用命令 - qq_30006593的博客 - CSDN博客





2017年08月15日 17:53:57[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：265标签：[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[linux](https://blog.csdn.net/qq_30006593/article/category/7102885)









## linux 常用命令
- sudo chmod 777 ***

-R为递归处理目录下所有文件 

777为权限（r=4,w=2,x=1读，写，执行）- dpkg -i xx.deb 安装 -r 卸载
- rename a b a?：第三个参数为名称，前两个要替换的和替换成的。例子：将a1-a9重命名为b1-b9
- touch均可新建文件
- ls -a 列出隐藏文件 

-l 列表形式：前缀为文件类型，d rwx r– r-x 

-h 列出大小- du：disk usage查看磁盘使用情况 

–max-depth=1：只看这一层
- df：查看目录下文件大小，只能看到能看到的 

-i：查看索引节点，表示能放多少文件
- sudo nethogs 查看网络流量
- tar -zxvf xx.tar.gz -C /xx/xx  –skip-old-files 解压 zcvf压缩， gz有损，压缩率最低
- tar xvf xx.tar 解压 cvf压缩 无损压缩
- unzip xx.zip 开放规范的压缩文件，算法不定，7z类似，有损
- rar x xx.rar 解压 rar a xx.rar dir 压缩  算法专有，有损 

sudo apt-get install rar unrar 
[区别](https://www.zhihu.com/question/26026741)- ln a b，创建a的硬链接b，多个文件指向同一个索引节点。 

-s：创建软连接，文件包含另一个文件的位置信息
- rsync –progress -a -v 
- cp -v -r -n

## linux 常识

初始化文件： 

.profile(.bash_profile in CentOS/RHEL)： login时调用 

.bashrc：每次打开bash 

/etc/bashrc：系统层面，不针对用户。






