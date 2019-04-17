# Linux系统备份 - xiahouzuoxin - CSDN博客





2014年05月08日 17:36:55[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：2092









Linux相对于Windows有一个最大的好处就是：Linux安装程序的过程只有复制文件的过程，没有写注册表的过程，因此，Linux系统重装之后只要将之前系统的程序文件备份好，重新放到新系统下即可使用。


如何备份取决于如何使用的Linux以及你备份的目的！

#### [](https://github.com/xiahouzuoxin/notes/blob/master/essays/Linux%E7%B3%BB%E7%BB%9F%E5%A4%87%E4%BB%BD.md#%E6%88%91%E6%98%AF%E8%BF%99%E6%A0%B7%E4%BD%BF%E7%94%A8linux%E7%9A%84)我是这样使用Linux的


大部分用户应用软件都会默认安装在/usr/local下，另外，我自己还会将部分软件安装在/opt/下，因此，这两个目录我都需要备份。


/etc目录下保留了几乎Linux系统所有的配置文件，也需要备份。


其它的东西我都会放在home用户目录中，因此，将/home目录备份好就OK了！


如果你还使用数据库，那么var的某些目录如lib也可能要备份，如果更新过内核，则/boot目录也可以备份，不过影响不大。

#### [](https://github.com/xiahouzuoxin/notes/blob/master/essays/Linux%E7%B3%BB%E7%BB%9F%E5%A4%87%E4%BB%BD.md#%E6%88%91%E7%9A%84%E7%9B%AE%E7%9A%84)我的目的


在重新安装系统后可以免去配置vim以及安装软件的麻烦！只需要进行下系统更新就可以使用！我不是专业人士，因此，如果你是做Linux数据库维护的，那你的备份将远不止于这些。

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/Linux%E7%B3%BB%E7%BB%9F%E5%A4%87%E4%BB%BD.md#1-%E5%A4%87%E4%BB%BD%E7%94%A8%E6%88%B7%E7%A8%8B%E5%BA%8F)1. 备份用户程序


/usr/local目录下为一般用户程序的安装位置，

```
tar -zcpPvf local.tar.gz /usr/local/
```


/opt也常用于存放用户程序，

```
tar -zcpPvf opt.tar.gz /opt/
```

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/Linux%E7%B3%BB%E7%BB%9F%E5%A4%87%E4%BB%BD.md#2-%E5%A4%87%E4%BB%BD%E5%85%A8%E5%B1%80%E5%8F%82%E6%95%B0%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6)2. 备份全局参数配置文件


Linux下大部分配置都保存在/etc/文件夹下，

```
tar -zcpPvf etc.tar.gz /etc/
```

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/Linux%E7%B3%BB%E7%BB%9F%E5%A4%87%E4%BB%BD.md#3-%E5%A4%87%E4%BB%BD%E7%94%A8%E6%88%B7%E7%9B%AE%E5%BD%95)3. 备份用户目录

```
tar -zcpPvf monkeyzx.tar.gz /home/monkeyzx/
```


其中monkeyzx为你的用户名。

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/Linux%E7%B3%BB%E7%BB%9F%E5%A4%87%E4%BB%BD.md#4-%E6%81%A2%E5%A4%8D%E5%A4%87%E4%BB%BD)

## 4. 恢复备份


恢复备份的方法很简单，只要将压缩包解压到新的系统中即可。因为压缩时使用的是绝对路径，解压可以在任意目录下使用下面的命令，

```
su root
tar -zxpPvf local.tar.gz
tar -zxpPvf opt.tar.gz 
tar -zxpPvf etc.tar.gz
tar -zxpPvf monkeyzx.tar.gz
```

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/Linux%E7%B3%BB%E7%BB%9F%E5%A4%87%E4%BB%BD.md#5-vim%E9%85%8D%E7%BD%AE)5. vim配置


我将自己的vim配置打了个包，用于系统更新后方便的将之前的配置移植，配置文件放在了Github上:

[https://github.com/xiahouzuoxin/vim_config](https://github.com/xiahouzuoxin/vim_config)

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/Linux%E7%B3%BB%E7%BB%9F%E5%A4%87%E4%BB%BD.md#tar%E5%91%BD%E4%BB%A4%E7%9A%84%E8%A7%A3%E9%87%8A)tar命令的解释

> 
> 
-z gzip压缩格式

-x tar解包

-c tar打包，创建文件

-p 保留文件权限属性（小写字符）

-P 保留绝对路径。在Linux中，防止解压tar包时无意覆盖'/'中文件, 默认情况下绝对路径中的根"/"将会被去除，使用-P可保留。

-v 显示压缩信息

-f 指定打包文件名



> 
> 






