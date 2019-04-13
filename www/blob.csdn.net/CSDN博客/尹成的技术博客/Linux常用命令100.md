
# Linux常用命令100 - 尹成的技术博客 - CSDN博客

2018年11月16日 09:50:51[尹成](https://me.csdn.net/yincheng01)阅读数：34个人分类：[GO语言](https://blog.csdn.net/yincheng01/article/category/7679307)[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)[](https://blog.csdn.net/yincheng01/article/category/7679307)



**@日常命令**
`cd .. // 返回上一级目录
cd - // 返回上一次目录
cd或cd ~ // 返回家目录
cd / // 进入根目录
pwd // 显示当前目录
ls // 显示当前目录文件
ls localrepo/ // 显示指定目录的文件
ll // 查看目录文件的的详细信息
la // 查看全部包括隐藏的目录文件
tree // 查看当前目录文件树
tree mydir // 查看指定目录的文件树
touch hello.py // 创建空文件
mkdir mydir // 创建文件夹
mkdir -p a/b/c // 创建层级目录
mkdir -p a/{b,c}/{d,e} // 同一级创建多个文件夹
cp hello.py ~/hello2.py // 拷贝文件
cp hello.py -i hello2.py // 拷贝文件，如果有同名文件询问是否覆盖
cp hello.py -f hello2.py // 拷贝文件，如果有同名文件强制覆盖
cp hello.py -n hello2.py // 拷贝文件，如果有同名文件不覆盖
mv hello2.py hellocp.py // 重命名文件
mv hello.py ~/Desktop // 移动文件
rm hellocp.py // 删除文件
rm -rf localrepo/ // 递归删除目录
find ~/Desktop/ -name '*.py' // 在指定目录中查找文件
whereis python // 查找可执行程序
which python // 查看当前使用的是哪个目录中的python
ln -s /usr/bin/python ./mypylink // 创建可执行程序的软链接
alias rrm='rm -rf' // 给命令起别名，需要root权限(不建议使用)
history // 查看命令历史
clear // 清屏(快捷键Ctrl+l)`**@命令速查**
`rm --help // 查看rm命令帮助
man rm // 查看rm命令的手册解释
info rm // 查看rm命令详细信息`**@更多查找**
`find ~/ -name '*.py' -exec rm {} \;	查找家目录下所有的py文件并删除，{}与exec结合匹配所有文件
find ~/ -name '*.py' -exec chown wife {} \;		查找家目录下所有py文件并将所有权转让给wife用户
find ~/ -name '*.py' -exec cat {} \;> record.txt	查找文件并将全部内容拼接并写入文件中
find ~/ -name '*.py' -exec cp {} mydir \;		查找文件并拷贝到指定目录
grep 'def.*' a.py	在a.py中查找指定内容
grep 'def.*' a.py b.py c.py	在多个文件中寻找指定内容
grep -v 'def.*' a.py	在a.py中查找指定内容以外的内容
grep -i "HELLO" a.py	在a.py中查找指定内容并忽略大小写
grep -E "[1-9]+" a.py	在a.py中查找正则表达式匹配的内容（匹配1到多个数字）
grep -F "[1-9]+" a.py	在a.py中查找纯文本内容（严格匹配字符串"[1-9]+"）
grep -e "def.*" -e "class.*" a.py	在a.py中查找多个内容
grep "text" ./ -r -n	在当前目录下递归查找内容`**@系统管理**
`poweroff	关机
reboot	重启
shutdown -h 12:00	定时关机
shutdown -h now		立刻关机
shutdown -c			取消定时关机
#结合1,查看所有CPU平均负载/每个CPU的具体负载
#结合M/P/T,按内存占用/CPU占用/运行时间的降序，查看进程信息
top					显示所有系统任务
free -m 			查看内存占用，以M为单位
uname -a			打印操作系统信息
uptime -p			查看系统运行时间
echo $PATH			查看环境变量
export PATH=$PATH:/home/sirouyang/Desktop/	追加环境变量
lsof | head -n 10	查看进程打开的文件（一切皆文件，数据文件、网络连接、硬件设备）
lscpu				查看CPU信息
time lsof		统计命令执行时间
cal/date			查看日历和时间
date +%y-%m-%d-%H-%M-%S	格式化地查看时间
# 0-6:0停机-1单用户安全模式-2多用户安全模式-3多用户网络模式-4系统保留级别-5图形界面-6重启
runlevel		显示当前运行级别
init 6			切换运行级别`**@软件安装**
`//方式一：通过包管理工具apt进行管理
sudo apt update // 更新软件源
sudo apt search xxx//查找xxx软件包
sudo apt install net-tools //安装net-tools
sudo apt remove --purge net-tools //彻底卸载net-tools
//方式二：编译源码
wget "http://...........xxx.tar.gz" //下载源码包
tar -zxvf xxx.tar.gz //解压源码包
make clean	清除编译的垃圾
make		执行编译
make install	安装
make uninstall	卸载
可选：./configure	如果源码包中自带配置程序则执行之
可选：sudo apt search libxxx //如果编译失败通常会提示缺少xxx依赖，此时应该搜索并安装该依赖
可选：sudo apt install libxxx-dev //安装检索到的依赖`**@文本命令**
`cat hello.py // 查看文件
gedit hello.py // 使用gedit查看文件
head -n 10 hello.py // 查看前10行
tail -n 10 hello.py // 查看后10行
less hello.py // 一次查看少量内容，PageUp和PageDown键翻页
vim hello.py // 使用vim打开文件`**@管道与重定向**
`sort hello.py | uniq -c // 排序后统计次数
sort hello.py | uniq -d // 排序后找出重复的行
cat /proc/cpuinfo | grep -c processor // 显示CPU核心数
echo 'newline' > hello.py // 将结果写入文件（覆盖）
echo 'newline' >> hello.py // 将结果追加到文件
cat < ~/proc/cpuinfo //将指定的文件作为cat的输入`**@压缩和解压**
`tar -cvf /home/abc.tar /home/abc    只打包，不压缩（c=创建压缩包,v=打印信息,f=指定文件名）
tar -zcvf /home/abc.tar.gz /home/abc    打包，并用gzip压缩
tar -jcvf /home/abc.tar.bz2 /home/abc    打包，并用bzip2压缩
tar -xvf abc.tar 解压普通tar包(x=解压缩)
tar -zxvf abc.tar.gz 解压gz压缩包
tar -jxvf abc.tar.bz2 解压bz2压缩包
zip -r xxx.zip abc/* fuck/* temp/* 递归压缩多个文件夹到xxx.zip
unzip xxx.zip #解压zip压缩包`**@磁盘管理**
`df -h /					查看磁盘使用情况
sudo du -h ~			查看用户目录的磁盘使用情况
du -sh					当前目录的总体占用情况
du abc.tar.gz abc.tar	查看文件和目录的磁盘占用
dd if=/dev/zero of=temp.txt bs=1K count=1024	写入一个充满0的文件，每块1K，共1024块，总大小1M
time dd if=/dev/zero of=temp.txt bs=1M count=1024	测试写入1G文件的时间（以此衡量磁盘的读写效率）`**@登录日志**
`last 查看用户近期登陆情况
last -2 最近登陆的两个用户
last -2 root 查看root用户最近两次登陆情况
id	显示用户及其群组信息
groups	查看当前用户所加入的组
whoami	显示当前用户名称`**@用户与权限**
`groups // 查看所属的工作组
whoami // 显示当前用户名称
sudo useradd -m wife // 新建wife用户，-m=自动创建家目录
sudo useradd -d /home/myson -m son //创建son用户，指定其家目录为/home/myson
sudo useradd -g guests -G root,adm -m him //创建him用户，属于group组，其次也属于adm,root组
sudo useradd -G guests,sudo -m -s /bin/bash mywife // 新增一个用户,-m=自动创建家目录,-s指定bash
userdel rose 删除rose 账号
userdel -r rose 删除用户及相关目录下的文件（/etc/passwd, /etc/shadow, /etc/group等）
usermod -s /bin/ksh -d /home/z –g developer him // 修改him用户的shell、家目录、所属组等信息
sudo passwd // 修改自己的密码
sudo passwd mywife // 修改mywife的密码
sudo su mywife // 切换用户
sudo su // 切换到root用户
sudo groupadd guests // 添加guests组
sudo groupadd -g 101 group1 //添加组group1，识别号为101
groupdel group1 //删除组
groupmod –g 10000 -n group3 group2 //将组group2的标识号改为10000，组名修改为group3
gpasswd -a u1 g1 将u1 加入到 g1 组
gpasswd -d u1 g1 将u1 退出 g1 组
gpasswd -A u1 g1 将g1 组的管理员指派给u1
chmod 421 hello.py // 设置自己、用户组、其他人的权限分别为只读、只写、只执行
chmod a+r,u+wx,g+x,o-wx hello.py // 详细修改全部用户、拥有者、用户组、其他人的权限`**/etc/passwd文件说明：**
是用户管理工作涉及的最重要的一个文件
内容包含7个字段，例如    him❌200:50:Sam san:/usr/sam:/bin/sh
含义分别为    用户名:口令:用户标识号:组标识号:注释性描述:主目录:登录Shell
**@网络管理**
`sudo apt install net-tools 安装网络工具
ifconfig // 查看网卡状态
ping 192.168.59.129	// 检查网络是否连通
wget "http://download.redis.io/releases/redis-4.0.9.tar.gz" // 下载redis
sudo apt install traceroute  安装路径追踪工具
traceroute -m 100 www.sina.com		追踪路由信息，最大100跳
netstat -t		列出所有TCP端口
netstat -r		显示路由表
netstat -i		显示接口（网卡）列表
netstat -tpnl	查看已连接的TCP端口和程序名
netstat -anp | grep 3306 -c		查看MySQL的连接数`**@远程登录**
`//安装并启动ssh服务
sudo apt install openssh-server 安装openssh-server
sudo ps -ef | grep ssh 查看SSH服务是否启动，还可以netstat -tl | grep 22或systemctl status ssh
sudo systemctl start ssh 如果没有启动则启动SSH服务
//访问远程主机（前提：对方开启了ssh服务，且你知道对方的密码，或对方持有你的ssh公钥）
ssh root@127.0.0.1 访问远程主机
exit 退出远程访问
//执行远程拷贝
scp -r sirouyang@192.168.158.129:/abc /home/wife/Desktop/		从远程主机拷贝内容到本地
scp -r /abc sirouyang@192.168.158.129:/home/sirouyang/Desktop/	从本地拷贝内容弄到远程主机（对方没有本地的公钥时需要密码认证）
# ssh公钥交换过程：远程主机持有本地的公钥时，ssh以及scp可以免密执行
ssh-keygen -t rsa	生成ssh秘钥对
scp ~/.ssh/id_rsa.pub sirouyang@192.168.158.129:/home/sirouyang/.ssh/	上传公钥给远程主机
ssh sirouyang@192.168.158.129	ssh登录远程主机
cat Desktop/id_rsa.pub >> ~/.ssh/authorized_keys	操作远程主机添加公钥认证（今后远程对本地ssh免密）
//一站式让远程持有自己的公钥
ssh-copy-id sirouyang@192.168.158.128	让远程持有自己的公钥（对方不必添加公钥到authorized_keys）`如果远程持有自己的公钥后依然无法免密登录，需要修改配置文件，请参考：[https://blog.csdn.net/b_x_p/article/details/78534423](https://blog.csdn.net/b_x_p/article/details/78534423)
**@进程管理**
`ps -ef // 查看所有进程（包括登录的其它用户的进程）
ps -aux //查看所有进程（仅当前用户的进程）
ps -ef | grep redis	// 查找redis进程
kill -9 77157	// 强行杀死指定pid的进程
kill -15 77157	// 终止指定进程`**@服务管理**
`#chkconfig服务管理
apt-get install sysv-rc-conf	安装chkconfig服务
cp /usr/sbin/sysv-rc-conf /usr/sbin/chkconfig	重命名一下可执行程序
chkconfig --list			查看所有服务的开机启动状态
chkconfig --list mysqld		查看mysql的开机启动状态
chkconfig mysqld on			设置mysql开机启动
chkconfig mysqld off		禁止mysql开机启动
sudo apt install firewalld	安装firewalld
firewall-cmd --state		查看防火墙状态
	
#原生的service服务管理
service mysql start			启动mysql
service mysql stop			停止mysql
service mysql restart		重启mysql
/etc/init.d/mysql start		启动mysql
/etc/init.d/mysql stop		停止mysql
/etc/init.d/mysql restart	重启mysql
#systemctl服务管理（推荐方式）
systemctl status firewalld		查看服务状态
systemctl start firewalld 		启动服务
systemctl stop firewalld 		停止服务
systemctl restart firewalld 	重启服务
systemctl is-enabled firewalld 	查看是否开机启动
systemctl enable firewalld		设置服务开机自启
systemctl disable firewalld 	禁止服务开机自启`**@其它命令**
`ls -i /usr/bin/python //查看文件的inode
ln /usr/bin/python ./phln // 创建硬链接（共享文件入口）(硬链接不能跨磁盘)
source ~/.bashrc // 使修改后的bashrc生效`**@一键安装Ubuntu常用库**
`sudo apt install man gcc make sudo lsof ssh openssl tree vim dnsutils iputils‑ping net‑
tools psmisc sysstat curl telnet traceroute wget libbz2‑dev libpcre3 libpcre3‑dev
libreadline‑dev libsqlite3‑dev libssl‑dev zlib1g‑dev git mysql‑server mysql‑client zip
p7zip golang-go libncurses5-dev libmysqlclient-dev`学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116092700978.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

