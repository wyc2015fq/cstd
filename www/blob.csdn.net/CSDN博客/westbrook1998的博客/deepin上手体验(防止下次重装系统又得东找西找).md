# deepin上手体验(防止下次重装系统又得东找西找) - westbrook1998的博客 - CSDN博客





2019年02月09日 11:05:20[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：87








第二次装deepin，感觉更舒服了，输入法字体也比ubuntu好看，命令行也比ubuntu流畅，更别说比win10了，win10的命令行简直了
[新博客](https://zeng1998.github.io/2019/02/13/deepin%E4%B8%8A%E6%89%8B%E4%BD%93%E9%AA%8C-%E9%98%B2%E6%AD%A2%E4%B8%8B%E6%AC%A1%E9%87%8D%E8%A3%85%E7%B3%BB%E7%BB%9F%E5%8F%88%E5%BE%97%E4%B8%9C%E6%89%BE%E8%A5%BF%E6%89%BE/)

# chrome

发现自带了chrome，想登录google账git号同步一下win10上的chrome，发现没翻墙，想翻墙又得挂个chrome插件，想装这个插件又得同步账号，当然也可以下载个离线的crx文件然后安装，不过新版chrome默认是不能离线安装的，所以要在命令行启动chrome加个参数

```bash
google-chrome --enable-easy-off-store-extension-install
```

然后就可以登录google账号，同步书签，同步插件，然后就可以使(gu)用(ge)linux了

# git

第二步得装个git，关联个github，把win10里在写的代码给拉过来

```bash
//安装git 和ubuntu一样 
sudo apt-get install git
//配置用户名和邮箱
git config --global user.name "xxxxxx"
git config --global user.email "xxxxxx@qq.com"
//生成秘钥
ssh-keygen -t rsa
```

然后把公钥`id_rsa.pub`的内容复制到github里，就关联上了，然后就可以用`git init`和`git remote add origin https://github.com/xxxxxx/xxxx.git`来关联远程仓库了（我也不知道是不是这样表达。。。）

（补充下最好用ssh协议，用https协议的话每次push都要输入用户名和密码

附上git素质三连
```bash
git add .
git commit -m"xxxx"
git push origin master
```

# VScode

装个vscode是必须的，然后`ctrl+shift+p`，`configure display language`，把`"locale"`改成`"zh-CN"，然后再搜索chinese的第一个插件装上就有中文了，`然后再搜索c++装上第一个插件

# g++/gcc

既然要写c++，就得装这个东西

```bash
sudo apt-get install g++
```

# Navicat

按应用商店里的那个装，然后替换了一下navicat.exe

虽然感觉wine的应用还是不太好看，不过偶尔用一用还是可以

# MySQL

这个密码那部分还是挺麻烦的

```bash
sudo apt-get install mysql-server mysql-client
```

这个时候登录不了，账号都不知道是什么

```bash
//打开这个配置文件
vi /etc/mysql/debian.cnf
```

用里面的user和password去登录mysql，也就是

```bash
mysql -u xxxx -p
xxxxxx
```

然后修改root用户的密码（记得flush一下）

```bash
mysql> use mysql;
mysql> select host,user,plugin,authentication_string from user;  
mysql> update user set plugin="mysql_native_password",authentication_string=password("xxxxxx") where user="root";
mysql> flush privileges;
```

# Java Python Nodejs

三大神器，其他乱七八糟的环境就先不装了

python是系统自带的，py2和py3都有，而且py2最好不要乱动，上次ubuntu卸载了py2结果凉了

装下pip和升级
```bash
sudo apt-get install python3-pip
sudo pip3 install --upgrade pip
```

java的先到[官网](https://www.oracle.com/technetwork/java/javase/downloads/jdk11-downloads-5066655.html)下载，我下载了jdk11，然后解压

```bash
tar -xvzf jdk-11.0.2_linux-x64_bin.tar.gz
```

创建一个目录/usr/local/bin/java，将解压后的文件夹移过去

```bash
sudo mkdir /usr/local/bin/java
sudo mv jdk-11.0.2 /usr/local/bin/java/
```

然后配置环境变量

```bash
//打开修改profile文件
vi /etc/profile
```

在后面加入

```bash
export JAVA_HOME=/usr/local/bin/java/jdk-11.0.2
export CLASSPATH=/usr/local/bin/java/jdk-11.0.2/lib
export PATH=$PATH:$JAVA_HOME/bin:$CLASSPATH
```

然后使配置生效

```bash
source /etc/profile
```

然后特么还要重启一下才是真正生效，所以有时候不是你操作错误了，而是你没重启。。。

然后是nodejs，前端瞎搞利器，npm素质三连

```bash
npm install
npm run serve
npm run build
```

先到[官网](https://nodejs.org/en/)下载源码，然后解压，进入文件夹，make素质三连

```bash
sudo ./configure
sudo make
sudo make install
```

make等挺久的，万幸顺利安装完成

# JetBrain全家桶+Android Studio

直接官网下载然后解压里面bin/xxx.sh打开即可，我是解压在/home/keane/App里

(破解)
- 将“0.0.0.0 [account.jetbrains.com](http://account.jetbrains.com)”添加到hosts文件中  `/etc/hosts`
- 芝麻开门

配置快捷方式

```bash
cd /usr/share/applications
//创建desktop文件  
sudo vi Idea.desktop
```

输入

```bash
[Desktop Entry]
Type=Application
Name=IDEA
Exec=/....../bin/idea.sh
Icon=/....../bin/idea.png
Terminal=false
```

其他三个也是一样的

记得Idea要装那个默认的，不要选那个without jdk的，有点小坑

至此就可以愉快的写代码了

# Idea配置Springboot热部署
- setting->build…->compiler->build project automatically 打钩
- ctrl+shift+A 搜索Registry 选中compiler.automake.allow.when.app.running
- pom.xml里加个maven依赖并开启热部署

```xml
...
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-devtools</artifactId>
    <optional>true</optional>
</dependency>
...
<build>
    <plugins>
        <plugin>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-maven-plugin</artifactId>
            <configuration>
                <fork>true</fork>//该配置必须
            </configuration>
        </plugin>
    </plugins>
</build>
```
- Chrome禁用缓存

F12 --> NetWork --> Disable Cache(while DevTools is open)

# 增加swap空间

以为8g的内存是够的，结果开个idea,webstorm,as,chrome几个页面就卡死两次了，随便开一个内存就一两g，所以还是加个swap分区试试
- 先从win10的磁盘管理里面压缩卷2个g出来
- 在deepin的分区管理里面，右键那个分出来的2g，新建，直接确定
- `sudo fdisk -l`会发现这个两个已经是`linux filesystem`了
- 创建交换文件 不一定是sda8，是2个g的位置，后面是大小

```bash
sudo dd if=/dev/sda8 of=/swapfile bs=2048 count=2048000
sudo mkswap /swapfile
```
- 启动交换空间

```bash
sudo swapon /swapfile
```
- 修改配置文件使永久生效

```bash
sudo vi /etc/fstab 
...
//在最后加入这一行
/swapfile       swap    swap defaults   0 0
```

# linux下的android虚拟机

有个kvm的东西，也不知道是干什么的，反正要加个权限

```bash
sudo chown xxxxx(xxxxx是用户名) -R /dev/kvm
```

# Android Studio和小米手机

也是大坑一个，要把开发者选项里的miui优化给关了才能真机调试，关了的话手机又变样了

# Hexo+Github Page 博客

```bash
//先全局安装hexo
sudo npm install -g hexo-cli
//创建文件夹blog
mkdir blog
cd blog
//hexo初始化
hexo init
```

可以装个hexo-admin方便写文章





