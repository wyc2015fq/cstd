# 如何将Emacs打造成现代的C/C++ IDE(一 把基本环境搭起来） - alw2009的博客 - CSDN博客





2018年12月24日 22:14:42[traveler_zero](https://me.csdn.net/alw2009)阅读数：307








为什么选择EMacs? 网络上已经有很多文章说明这个问题， 对我而言Just for fun.



接下来我会演示如何在一个Centos 7.6 上安装最新版本的GNU Emacs. 经过以下步骤， 你就可以得到一个可以在SSH终端上使用起来还不错的C/C++编辑器。

**目录**

[0. 环境准备](#0.%20%E7%8E%AF%E5%A2%83%E5%87%86%E5%A4%87)

[1. 下载和编译emacs.](#1.%20%E4%B8%8B%E8%BD%BD%E5%92%8C%E7%BC%96%E8%AF%91emacs.%C2%A0)

[2. 下载spacemacs配置](#2.%20%E4%B8%8B%E8%BD%BDspacemacs%E9%85%8D%E7%BD%AE)

[3. 设置c-c++-layer](#3.%20%E8%AE%BE%E7%BD%AEc-c%2B%2B-layer)

[4. 设置终端色彩](#4.%20%E8%AE%BE%E7%BD%AE%E7%BB%88%E7%AB%AF%E8%89%B2%E5%BD%A9)

[5. 效果演示](#5.%20%E6%95%88%E6%9E%9C%E6%BC%94%E7%A4%BA)



### 0. 环境准备
`sudo yum install -y gcc gcc-c++ ncurses ncurses-devel gnutls gnutls-utils`


### 1. 下载和编译emacs. 

[去官网拿到下载地址](https://www.gnu.org/software/emacs/)

```
$ mkdir downloads && cd downloads
$ wget http://mirrors.nju.edu.cn/gnu/emacs/emacs-26.1.tar.gz
$ tar xf emacs-26.1.tar.gz
$ cd emacs-26.1

# 这里创建一个目录用于安装自己编译的一些程序
$ mkdir ~/local
$ ./configure --prefix=/home/sujinpei/local --with-gnutls=no
$ make && make install

$ export PATH=$HOME/local/bin:$PATH

# 查看一下emacs的版本是否正确
$ emacs --version

# 将路径添加到PATH环境变量
echo -en "export PATH=$HOME/local/bin:$PATH\n" >> ~/.bashrc
```

2. 下载spacemacs配置

```
# 这个步骤需要git支持， 如果没安装先运行: sudo yum install -y git
git clone https://github.com/syl20bnr/spacemacs ~/.emacs.d

# 如果~/.emacs.d已经存在， 请先删除或者备份

# 切换到develop分支

cd ~/.emacs.d/

git checkout develop
```

 启动emacs根据提示完成spacemacs的安装。在Installing packages的时候可能会很慢甚至报错。C-g(ctrl+g)取消安装， 然后C-x C-c退出。

打开~/.spacemacs文件， 将user-init部分修改如下

```
(defun dotspacemacs/user-init ()
  "Initialization for user code:
This function is called immediately after `dotspacemacs/init', before layer
configuration.
It is mostly for variables that should be set before packages are loaded.
If you are unsure, try setting them in `dotspacemacs/user-config' first."
  (setq configuration-layer-elpa-archives
      '(("melpa-cn" . "http://elpa.emacs-china.org/melpa/")
        ("org-cn"   . "http://elpa.emacs-china.org/org/")
        ("gnu-cn"   . "http://elpa.emacs-china.org/gnu/")))
  )
```

然后重启emacs, emacs会继续完成spacemacs的安装， 这时候安装的速度应该会提升很多。如果安装依然失败， 就需要用emacs --debug-init命令来启动， 排查一下问题， 一般是网络问题。



### 3. 设置c-c++-layer

打开.spacemacs找到dotspacemacs-configuration-layers

```
dotspacemacs-configuration-layers
    '(
      ;; ----------------------------------------------------------------
      ;; Example of useful layers you may want to use right away.
      ;; Uncomment some layer names and press `SPC f e R' (Vim style) or
      ;; `M-m f e R' (Emacs style) to install them.
      ;; ----------------------------------------------------------------[<0;81;16M]
      c-c++
      helm
      auto-completion
      better-defaults
      emacs-lisp
      ;; git
      ;; markdown
      multiple-cursors
      neotree
      ;; org
      ;; (shell :variables
      ;;        shell-default-height 30
      ;;        shell-default-position 'bottom)
      ;; spell-checking
      ;; syntax-checking
      ;; version-control
      )
```

重启emacs, 等待相关的包安装完成。C-x C-f创建一个C/C++的文件编辑一下， 语法高亮， 一些自动补全功能， 但是颜色可能有一些问题， 如何解决呢？ 请看下面的步骤。



### 4. 设置终端色彩

```
export TERM=xterm-256color
```



### 5. 效果演示

![](https://img-blog.csdnimg.cn/20181224221200874.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=,size_16,color_FFFFFF,t_70)





