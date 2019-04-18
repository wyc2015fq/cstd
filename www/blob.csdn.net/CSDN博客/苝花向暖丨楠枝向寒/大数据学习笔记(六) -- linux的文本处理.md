# 大数据学习笔记(六) -- linux的文本处理 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月24日 16:04:23[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：49


**Table of Contents**

[cut](#cut)

[sort](#sort)

[wc](#wc)

[sed](#sed)

[awk](#awk)

# cut

![](https://img-blog.csdn.net/20180923204639107?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```python
[root@node001 ~]# cat grep.txt 
aaa bbb  ccc
111 222
xx yy zz
oo 
qq ttt wwx
88888
666
AA BB CC
```

以 空格为 分隔符，将每一行分为多列，然后显示第2列

```python
[root@node001 ~]# cut -d' ' -f2 -s grep.txt 
bbb
222
yy

ttt
BB
```

注意第三行是 一个空格，之所以是空格是因为 我grep.txt中 第三行的 oo 后面加了一个空格，而 88888和666 这两行没有空格，所以这两行对应位置没有空格，上面用了 参数 s 代表没有 分隔符的行就不显示。如果我没有加上 -s 参数，那么 88888和666也会被显示出来，如下

```bash
[root@node001 ~]# cut -d' ' -f2 grep.txt 
bbb
222
yy

ttt
88888
666
BB
```

# sort

![](https://img-blog.csdn.net/2018092321110570?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```bash
[root@node001 ~]# cat sort.txt 
banana 12
apple 1
orange 8

[root@node001 ~]# sort sort.txt 

apple 1
banana 12
orange 8
[root@node001 ~]# sort -t' ' -k2 sort.txt 

apple 1
banana 12
orange 8
[root@node001 ~]# sort -t' ' -k2 -n sort.txt 

apple 1
orange 8
banana 12
[root@node001 ~]# sort -t' ' -k2 -n -r sort.txt 
banana 12
orange 8
apple 1
```

# wc

```bash
[root@node001 ~]# cat grep.txt 
aaa bbb  ccc
111 222
xx yy zz
oo 
qq ttt wwx
88888
666
AA BB CC
[root@node001 ~]# wc grep.txt  
 8 17 64 grep.txt
```

wc 命令 默认输出的为 文件的行数、单词数、以及字符数

也可通过参数指定查询某项内容

![](https://img-blog.csdn.net/20180923212421455?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```bash
[root@node001 ~]# wc -m grep.txt 
64 grep.txt
[root@node001 ~]# wc -c grep.txt 
64 grep.txt
[root@node001 ~]# wc -l grep.txt 
8 grep.txt
[root@node001 ~]# wc -L grep.txt 
12 grep.txt
[root@node001 ~]# wc -w grep.txt 
17 grep.txt
```

# sed

![](https://img-blog.csdn.net/20180923215237536?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/2018092321525961?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180923215315975?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**例一**：

-n 只显示 符合条件的行，p和数字结合来筛选出指定行。

```bash
[root@node001 ~]# cat sort.txt 
banana 12
apple 1
orange 8

[root@node001 ~]# sed -n "2p" sort.txt 
apple 1
[root@node001 ~]# sed "3p" sort.txt 
banana 12
apple 1
orange 8
orange 8
```

**例二**：

-i 会根据后面给出的命令 ，对源文件做对应操作， d和数字结合 代表删除指定行，如果不确定的话可以先去掉-i 看效果 

```bash
[root@node001 ~]# sed -i "3d" sort.txt 
[root@node001 ~]# cat sort.txt 
banana 12
apple 1
```

**例三**：

2a代表在第二行的后面追加一行然后插入内容 

```bash
[root@node001 ~]# sed "2a\orange 8" sort.txt 
banana 12
apple 1
orange 8
[root@node001 ~]# sed -i "2a\orange 8" sort.txt 
[root@node001 ~]# cat sort.txt 
banana 12
apple 1
orange 8
```

**例四**：

 /apple 查找到apple这个字符串，然后 i 代表在其上面追加一行 ，内容为 hello zzh

```bash
[root@node001 ~]# sed "/apple/i\hello zzh" sort.txt 
banana 12
hello zzh
apple 1
orange 8
[root@node001 ~]# sed -i "/apple/i\hello zzh" sort.txt 
[root@node001 ~]# cat sort.txt 
banana 12
hello zzh
apple 1
orange 8
```

**例五**：

/^h  代表匹配当以字母h开头的行，d 代表删除。 

```bash
[root@node001 ~]# sed "/^h/d" sort.txt 
banana 12
apple 1
orange 8
[root@node001 ~]# sed -i "/^h/d" sort.txt 
[root@node001 ~]# cat sort.txt 
banana 12
apple 1
orange 8
```

**例六**：

我们先cp 一个文件到 用户目录

```bash
[root@node001 ~]# cp /etc/inittab  ./
[root@node001 ~]# ll
total 24
-rw-r--r--. 1 root root  175 Sep 21 22:37 grep
-rw-r--r--. 1 root root   65 Sep 21 23:48 grep.txt
-rw-r--r--. 1 root root  884 Sep 22 00:09 inittab
-rw-r--r--. 1 root root 1796 Sep 21 20:20 profile
-rw-r--r--. 1 root root   28 Sep 22 00:09 sort.txt
-rw-r--r--. 1 root root   22 Sep 21 20:25 zzh.txt
[root@node001 ~]# tail inittab 
# Default runlevel. The runlevels used are:
#   0 - halt (Do NOT set initdefault to this)
#   1 - Single user mode
#   2 - Multiuser, without NFS (The same as 3, if you do not have networking)
#   3 - Full multiuser mode
#   4 - unused
#   5 - X11
#   6 - reboot (Do NOT set initdefault to this)
# 
id:3:initdefault:
```

然后 将 最后一行的  3  改为 5

s 代表查找并替换，  s/被替换的字符/替换的字符      

s/ 后面是一个正则表达式，用小括号把 "id:" 和 ":initdefault:"  括住， 为了在后面使用 \1 和 \2 引用 

```bash
[root@node001 ~]# sed "s/\(id:\)[0-6]\(:initdefault:\)/\15\2/" inittab
```

结果如下 

```bash
# inittab is only used by upstart for the default runlevel.
#
# ADDING OTHER CONFIGURATION HERE WILL HAVE NO EFFECT ON YOUR SYSTEM.
#
# System initialization is started by /etc/init/rcS.conf
#
# Individual runlevels are started by /etc/init/rc.conf
#
# Ctrl-Alt-Delete is handled by /etc/init/control-alt-delete.conf
#
# Terminal gettys are handled by /etc/init/tty.conf and /etc/init/serial.conf,
# with configuration in /etc/sysconfig/init.
#
# For information on how to write upstart event handlers, or how
# upstart works, see init(5), init(8), and initctl(8).
#
# Default runlevel. The runlevels used are:
#   0 - halt (Do NOT set initdefault to this)
#   1 - Single user mode
#   2 - Multiuser, without NFS (The same as 3, if you do not have networking)
#   3 - Full multiuser mode
#   4 - unused
#   5 - X11
#   6 - reboot (Do NOT set initdefault to this)
# 
id:5:initdefault:
```

**例七**：

cp 一个网络配置文件

```bash
cp /etc/sysconfig/network-scripts/ifcfg-eth0 ./
```

 设置一个变量值

```bash
num=88
```

正则匹配ip地址然后将最后一位更改为88 

```bash
sed "s/\(IPADDR=\(\<2[0-5][0-5]\|\<2[0-4][0-9]\|\<1\?[0-9][0-9]\?\.\)\{3\}\).*/\1$num/"
```

 结果如下

```bash
ifcfg-eth0 
DEVICE=eth0
TYPE=Ethernet
ONBOOT=yes
NM_CONTROLLED=yes
BOOTPROTO=static
IPADDR=192.168.46.88
NETMASK=255.255.255.0
GATEWAY=192.168.46.2
DNS1=114.114.114.114
```

# awk

![](https://img-blog.csdn.net/20180924143213426?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**例一**：

cp 一个文件 

```bash
cp /etc/passwd ./
```

 以 “:” 分割文件中的每一行，并在读取文件之前输出  name        shell    在读取文件之后输出   hello  zzh

{} 花括号代表匿名函数，awk以行的粒度去读取文件，读出一行就会调用匿名函数，将读出的行信息传入匿名函数中，并执行代码， BEGIN 和 END  是固定的函数，一个是读取文件之前调用，一个是读取文件之后调用。

匿名函数可以有多个。BEGIN 和 END只能执行一次。

$1 代表取出第一列，$7 代表取出第七列，$0 代表取出整行

awk 所执行的函数 只能用 单引号 括住，因为 $ 对于bash来说是一个命令，用来取值。 

```bash
[root@node001 ~]# awk -F':' 'BEGIN{print "name\tshell"} {print $1 "\t" $7} {print $0} END{ print "hello zzh"}' passwd
name	shell
root	/bin/bash
root:x:0:0:root:/root:/bin/bash
bin	/sbin/nologin
bin:x:1:1:bin:/bin:/sbin/nologin
daemon	/sbin/nologin
daemon:x:2:2:daemon:/sbin:/sbin/nologin
adm	/sbin/nologin
adm:x:3:4:adm:/var/adm:/sbin/nologin
lp	/sbin/nologin
lp:x:4:7:lp:/var/spool/lpd:/sbin/nologin
sync	/bin/sync
sync:x:5:0:sync:/sbin:/bin/sync
shutdown	/sbin/shutdown
shutdown:x:6:0:shutdown:/sbin:/sbin/shutdown
halt	/sbin/halt
halt:x:7:0:halt:/sbin:/sbin/halt
mail	/sbin/nologin
mail:x:8:12:mail:/var/spool/mail:/sbin/nologin
uucp	/sbin/nologin
uucp:x:10:14:uucp:/var/spool/uucp:/sbin/nologin
operator	/sbin/nologin
operator:x:11:0:operator:/root:/sbin/nologin
games	/sbin/nologin
games:x:12:100:games:/usr/games:/sbin/nologin
gopher	/sbin/nologin
gopher:x:13:30:gopher:/var/gopher:/sbin/nologin
ftp	/sbin/nologin
ftp:x:14:50:FTP User:/var/ftp:/sbin/nologin
nobody	/sbin/nologin
nobody:x:99:99:Nobody:/:/sbin/nologin
vcsa	/sbin/nologin
vcsa:x:69:69:virtual console memory owner:/dev:/sbin/nologin
saslauth	/sbin/nologin
saslauth:x:499:76:"Saslauthd user":/var/empty/saslauth:/sbin/nologin
postfix	/sbin/nologin
postfix:x:89:89::/var/spool/postfix:/sbin/nologin
sshd	/sbin/nologin
sshd:x:74:74:Privilege-separated SSH:/var/empty/sshd:/sbin/nologin
god	/bin/bash
god:x:500:500::/home/god:/bin/bash
good	/bin/bash
good:x:501:501::/home/good:/bin/bash
zzh	/bin/bash
zzh:x:502:502::/home/zzh:/bin/bash
hello zzh
[root@node001 ~]# awk '/root/ {print $0}' passwd 
root:x:0:0:root:/root:/bin/bash
operator:x:11:0:operator:/root:/sbin/nologin
```

**例二**： 

查找文件中包含 root 字符串的行 并显示

```bash
[root@node001 ~]# awk '/root/ {print $0}' passwd 
root:x:0:0:root:/root:/bin/bash
operator:x:11:0:operator:/root:/sbin/nologin
```

**例三**：

打印所有行，并显示行数和每行的列数

```bash
[root@node001 ~]# awk -F':' '{print NR"\t"NF"\t"$0}' passwd
1	7	root:x:0:0:root:/root:/bin/bash
2	7	bin:x:1:1:bin:/bin:/sbin/nologin
3	7	daemon:x:2:2:daemon:/sbin:/sbin/nologin
4	7	adm:x:3:4:adm:/var/adm:/sbin/nologin
5	7	lp:x:4:7:lp:/var/spool/lpd:/sbin/nologin
6	7	sync:x:5:0:sync:/sbin:/bin/sync
7	7	shutdown:x:6:0:shutdown:/sbin:/sbin/shutdown
8	7	halt:x:7:0:halt:/sbin:/sbin/halt
9	7	mail:x:8:12:mail:/var/spool/mail:/sbin/nologin
10	7	uucp:x:10:14:uucp:/var/spool/uucp:/sbin/nologin
11	7	operator:x:11:0:operator:/root:/sbin/nologin
12	7	games:x:12:100:games:/usr/games:/sbin/nologin
13	7	gopher:x:13:30:gopher:/var/gopher:/sbin/nologin
14	7	ftp:x:14:50:FTP User:/var/ftp:/sbin/nologin
15	7	nobody:x:99:99:Nobody:/:/sbin/nologin
16	7	vcsa:x:69:69:virtual console memory owner:/dev:/sbin/nologin
17	7	saslauth:x:499:76:"Saslauthd user":/var/empty/saslauth:/sbin/nologin
18	7	postfix:x:89:89::/var/spool/postfix:/sbin/nologin
19	7	sshd:x:74:74:Privilege-separated SSH:/var/empty/sshd:/sbin/nologin
20	7	god:x:500:500::/home/god:/bin/bash
21	7	good:x:501:501::/home/good:/bin/bash
22	7	zzh:x:502:502::/home/zzh:/bin/bash
```

**例四**：

首先准备一个报表

```bash
[root@node001 ~]# cat awk.txt 
Tom	0	2012-12-11	car	3000
John    1	2013-01-13	bike    1000
vivi	1	2013-01-18	car	2800
Tom	0	2013-01-20	car	2500
John	1	2013-01-28	bike	3500
```

使用awk实现合计每人一月份的工资

split函数 对字符串以指定字符进行拆分，$3 对第三列进行拆分，date拆分以后存入date，以 "-" 进行拆分。

name[$1]+=$5  可以理解为  name[$1]=0  ;  name[$1] = name[$1]+$5

```bash
[root@node001 ~]# awk '{ split($3,date,"-");if(date[2]=="01"){name[$1]+=$5}} END{for(i in name){print i "\t" name[i]}}' awk.txt 
vivi	2800
Tom	2500
John	4500
```

**例五**：

根据列四的案例，再增加一列显示员工的职位，制表符中的0代表Manger，1代表Worker，以下简写成M 和 W

```bash
[root@node001 ~]# awk '{ split($3,date,"-");if(date[2]=="01"){name[$1]+=$5;if($2=="0"){role[$1]="M"}else{role[$1]="W"}}} END{for(i in name){print i "\t" name[i] "\t" role[i]}}' awk.txt 
vivi	2800	W
Tom	2500	M
John	4500	W
```

**列六**：

上面的写法当 命令过长时非常反人类，我们看起来非常不直观，其实可以将上面的内容写在一个文件当中，如下

```bash
[root@node001 ~]# cat command.awk 
{ 
	split($3,date,"-");
	if(date[2]=="01"){
		
		name[$1]+=$5;
		
		if($2=="0"){
			role[$1]="M";
		}else{
			role[$1]="W";
		}
		
	}
} 
END{
	for(i in name){
		print i "\t" name[i] "\t" role[i];
	}
}
```

然后再使用 -f 参数执行

```bash
[root@node001 ~]# awk -f command.awk awk.txt 
vivi	2800	W
Tom	2500	M
John	4500	W
```

