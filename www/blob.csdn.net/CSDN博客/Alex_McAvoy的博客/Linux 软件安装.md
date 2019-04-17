# Linux 软件安装 - Alex_McAvoy的博客 - CSDN博客





2018年07月15日 13:48:48[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：61
个人分类：[Linux 学习笔记](https://blog.csdn.net/u011815404/article/category/7715914)









# 【apt 管理工具】

## 1.概述



APT 是 Advance Packaging Tool（高级包装工具）的缩写，是 Debian 及其派生发行版的软件包管理器，APT 可以自动下载、配置、安装二进制或者源代码格式的软件包，因此简化了 Linux 系统上管理软件的过程。

APT 最早被设计成 dpkg 的前端，用来处理 deb 格式的软件包，现经 APT-RPM 组织修改，APT 已经可以安装在支持 RPM 的系统管理 RPM 包。

这个包管理器包含以 apt- 开头的多个工具，如：apt-get、apt-cache、apt-cdrom 等，在 Debian 系列的发行版中均可使用。

## 2.apt-get

### 1）概述

apt-get 是用于处理 apt 包的公用程序集，我们可用它来在线安装、卸载、升级软件包等。

当执行安装操作时，apt-get 工具首先会在本地的软件源数据库中搜索关于要安装的软件的相关信息，并根据这些信息在相关的软件源镜像服务器上下载软件，进行安装。

我们需要定期从服务器上下载一个软件包列表，使用 sudo apt-get update 命令来保持本地的软件包列表是最新的，而这个表里会有软件依赖信息的记录。

**注：**

**① 软件源是一个应用程序安装库，很多的应用软件都在这个库中。**

**② 软件依赖：当安装一个软件时，有时这个软件需要别的软件包才能正常工作，这个时候，apt-get 在安装软件时会一并替我们安装，以保证要安装的软件能正常工作。**

### 2）apt-get 包含的常用工具


|install|安装软件包|
|----|----|
|update|更新本地软件源列表|
|upgrade|升级没有依赖问题的软件包（通常会在执行前执行 update ）|
|dist-upgrade|升级并解决依赖关系（存在一定危险性）|
|remove|移除已安装的软件包与有依赖关系的软件包，不包含软件包的配置文件|
|autoremove|移除之前被其他软件包依赖，但现在不再被使用的软件包|
|purge|与 remove 相同，但会完全移除软件包，包含其配置文件|
|clean|移除下载到本地的已经安装的软件包，默认保存在 /var/cache/apt/archives/|
|autoclean|移除已安装的软件的旧版本软件包|

### 3）apt-get 常用参数


|-y|自动回应是否安装软件包的选项|
|----|----|
|-s|模拟安装|
|-q|静默安装方式，指定多个 q 或者 -q=#（# 表示数字），用于设定静默级别|
|-f|修复损坏的依赖关系|
|-d|只下载不安装|
|--reinstall|重新安装已经安装但可能存在问题的软件包|
|--install-suggests|同时安装 apt 给出的建议安装的软件包|

### 4）注意事项



① 在不知道软件包完整名的时进行安装，通常使用 Tab 键补全软件包名。

② 有时需要同时安装多个软件包，可以使用正则表达式匹配软件包名进行批量安装。

## 3.软件搜索



当刚知道了一个软件，想下载使用，需要确认软件仓库里面有没有，就需要用搜索功能。

1）命令格式：**sudo apt-cache search softname1 softname2 softname3……**

2）说明：apt-cache 命令是针对本地数据进行相关操作的工具，search 在本地的数据库中寻找有关 softname1 softname2 …… 相关软件的信息。

3）实例：

![](https://img-blog.csdn.net/20180715134754250?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 【dpkg 的使用】

1.概述

dpkg 是 Debian Packager 的简写，是 Debian 专门开发的套件管理系统，用于软件的安装、更新及移除。

所有源自 Debian 的 Linux 发行版都使用 dpkg，是 Debian 软件包管理器的基础，其与 RPM 十分相似，同样被用于安装、卸载和供给和 .deb 软件包相关的信息。

不同于 apt 这种上层工具，它是一个底层的工具，因此它无法解决软件包的依赖问题。

2.常用参数


|-i|安装指定 deb 包|
|----|----|
|-R|其后加目录名，用于安装该目录下的所有 deb 安装包|
|-r|remove，移除某个已安装的软件包|
|-l|显示 deb 包的信息|
|-s|显示已安装软件的信息|
|-S|搜索已安装的软件包|
|-L|显示已安装软件包的目录信息|





