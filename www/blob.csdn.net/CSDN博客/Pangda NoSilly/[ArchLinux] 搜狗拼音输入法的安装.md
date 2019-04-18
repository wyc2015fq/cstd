# [ArchLinux] 搜狗拼音输入法的安装 - Pangda NoSilly - CSDN博客

2017年06月26日 11:33:56[hepangda](https://me.csdn.net/hepangda)阅读数：3881
个人分类：[Linux](https://blog.csdn.net/hepangda/article/category/6991396)



## 配置源

在ArchlinuxCN源中有很多方便中国用户使用的包，其中也包含了经常使用的搜狗拼音输入法，于是我们需要先配置ArchlinuxCN源，这样我们就可以使用自带的包管理器Pacman直接安装搜狗拼音输入法。

首先使用文本编辑器打开`/etc/pacman.conf`，末尾增加以下两句：

```
[archlinuxcn]
Server = https://mirrors.ustc.edu.cn/archlinuxcn/$arch
```

然后我们更新一下软件仓库：

```bash
sudo pacman -Sy
```

这里我们就完成了添加源的操作。

## 安装

### 安装Fcitx

由于搜狗拼音输入法依赖于Fcitx，在安装搜狗拼音输入法之前，需要先行安装Fcitx，在终端窗口下直接输入：

```bash
sudo pacman -S fcitx
```

即可完成安装，需要注意的是，仅仅安装这一项是不够的，这样在安装完成之后，Fcitx基本上是处于不可用的状态，我们还需要安装以下几个包：

```
sudo pacman -S fcitx-configtool
sudo pacman -S fcitx-gtk2 fcitx-gtk3 fcitx-qt4 fcitx-qt5
```

### 安装搜狗拼音

在前一步中我们已经正确的配置了源，这里直接输入：
`sudo pacman -S fcitx-sogoupinyin`
即可安装。

## 配置

安装完之后我们还不可以直接使用，还需要进行一定的配置，用文本编辑器打开`~/.xprofile`，在其末尾添加以下几行：

```bash
export GTK_IM_MODULE=fcitx
export QT_IM_MODULE=fcitx
export XMODIFIERS="@im=fcitx"
# fcitx &后请替换为你的桌面环境的启动命令
fcitx & cinnamon-session
```

保存退出，若你使用的桌面环境比较特殊，可能需要在`/etc/environmenet`后方也加入：

```bash
export GTK_IM_MODULE=fcitx
export QT_IM_MODULE=fcitx
export XMODIFIERS="@im=fcitx"
```

至此，搜狗拼音输入法已经安装完毕，重启你的桌面环境，就可以在系统中使用搜狗拼音了。

