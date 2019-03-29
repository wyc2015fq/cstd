# emacs笔记



# 下载

http://mirrors.ustc.edu.cn/gnu/emacs/windows/emacs-26/

```
emacs-26-deps-mingw-w64-src.zip                    03-Jun-2018 20:52           
emacs-26-deps-mingw-w64-src.zip.sig                03-Jun-2018 20:52                
emacs-26-i686-deps.zip                             03-Jun-2018 20:54          
emacs-26-i686-deps.zip.sig                         03-Jun-2018 20:54                
emacs-26-x86_64-deps.zip                           03-Jun-2018 20:57          
emacs-26-x86_64-deps.zip.sig                       03-Jun-2018 20:57                
emacs-26.1-i686-no-deps.zip                        03-Jun-2018 20:33          
emacs-26.1-i686-no-deps.zip.sig                    03-Jun-2018 20:33                
emacs-26.1-i686.zip                                03-Jun-2018 20:38          
emacs-26.1-i686.zip.sig                            03-Jun-2018 20:38                
emacs-26.1-x86_64-no-deps.zip                      03-Jun-2018 20:41          
emacs-26.1-x86_64-no-deps.zip.sig                  03-Jun-2018 20:41                
emacs-26.1-x86_64.zip                              03-Jun-2018 20:47          
emacs-26.1-x86_64.zip.sig                          03-Jun-2018 20:47                
```

- 说明：i686 表示 32 位， x86_64 是 64 位，deps 表示带有依赖，no-deps 表示没有依赖，要是我的话就下 emacs-26.1-x86_64.zip。`mingw-w64` 是模拟 Linux 环境的一个东西， `w64` 表示是 64 位环境。 `.sig` 是签名文件，用来验证你下载的文件是否完整，是否被别人篡改。







# 将Emacs绿色便携化



在使用emacs的过程中，我们有时会在不同电脑之间切换，或重置电脑。但是Emacs默认的配置文件存放在%APPDATA%目录下，需要手动备份，很不方便。如何让Emacs绿色化从而压缩之后在新电脑解压即用呢？

## 

1.首先我们打开Emacs官网（https://www.gnu.org/software/emacs/download.html#windows），点击"nearby GNU mirror"   

2.然后会看见这样一个列表，我们选择名字为“emacs+数字”的链接，点击数字最大的那个，比如这里就是“emacs26”

3.然后我们再选择数字最大的、后缀为x86_64的文件。如果只用到org-mode的话，可以按照图片所示选择“emacs-26.1-x86_64-no-deps.zip”

4.我们将下载下来的安装包解压，然后将解压出来的文件夹重命名为emacs

5.打开这个文件夹，然后进入share/emacs/site-lisp

6.空白地方右键，新建文本文档，然后重命名为“site-start.el”

7.右键site-start.el，打开方式-记事本

8.然后复制粘贴以下文本，保存
```lisp
(defvar program-dir       
   (replace-regexp-in-string "share/emacs.*/etc/$" "home/" data-directory :from-end))   (setenv "HOME" program-dir) 
(load "~/.emacs.d/init.el")
```

9.再回到一开始的emacs文件夹，右键-新建-文件夹，命名为home

10.然后进入bin文件夹，双击runemacs.exe

11.我们在这里按Ctrl+x, Ctrl+f新建一个名为.emacs的文件

12.然后关闭，运行runemacs，就能看见emacs初始界面了


    

# spacemacs



## 安装

安装很简单，先备份自己的.emacs.d目录，然后移走。

现在clone项目到~/.emacs.d目录去

```php
git clone https://github.com/syl20bnr/spacemacs ~/.emacs.d
或者
mkdir spacemacs
git clone https://github.com/syl20bnr/spacemacs.git spacemacs/.emacs.d
HOME=spacemacs emacs
```



在`~/.emacs.d/init.el` 中添加

```lisp
(setq spacemacs-start-directory "~/.emacs.d/spacemacs/")
(load-file (concat spacemacs-start-directory "init.el"))
```



按右键选择holly control tower(emacs) editing styel

后面两个都选默认吧，都是功能最全，可能速度慢点或者系统资源消耗多点。



## 创建dean layer

现在创建private layer，用来保存自己的配置，不和github上的冲突。这样以后就可以用github放心升级spacemacs了。

M-x configuration-layer/create-layer命令，然后回答几个问题，创建dean的private layer.

需要编辑~/.spacemacs文件，添加dean layer

```lisp
   dotspacemacs-configuration-layers
   '(
     ;; ----------------------------------------------------------------
     ;; Example of useful layers you may want to use right away.
     ;; Uncomment some layer names and press <SPC f e R> (Vim style) or
     ;; <M-m f e R> (Emacs style) to install them.
     ;; ----------------------------------------------------------------
     helm
     ;; auto-completion
     ;; better-defaults
     emacs-lisp
     ;; git
     ;; markdown
     ;; org
     ;; (shell :variables
     ;;        shell-default-height 30
     ;;        shell-default-position 'bottom)
     ;; spell-checking
     ;; syntax-checking
     ;; version-control
     dean
     )
```



dean目录会出现在.emacs.d/private/dean, 里面有一个package.el文件

## 配置属于dean layer的package

现在我来添加一个chinese-pyim package来支持我在emacs里面输入中文。

所有的属于dean layer的packages都可以列在下面



```lisp
(defconst dean-packages
  '(chinese-pyim))
```





专门为chinese-pyim定义了一个初始化的函数, 这个必须有，否则重启emacs不会自动安装package

```lisp
(defun dean/init-chinese-pyim()
    (use-package chinese-pyim)
    )
```



设置了默认输入法和词典文件路径

```lisp
(custom-set-variables
 '(default-input-method "chinese-pyim")
 '(pyim-dicts (quote((:name "BigDict-01" :file "/home/dean/.emacs.d/private/dean/pyim/dicts/pyim-bigdict.pyim" :coding utf-8-unix :dict-type pinyin-dict)))
   )
 )
```

重新启动emacs，一会儿看到下载安装，然后检查*Message*，会看到dean/package.el成功loading

到~/.emacs.d/elpa/目录下可以找到chinese-pyim package的程序，证明安装成功。



现在按C-\, 可以输入中文了。



## 目录树功能

M-m f t

M-m 其实就是SPC（vim用户习惯用SPC，emacs用户就是M-m了）

现在很漂亮的目录树出现了。

其实这里用的是neotree package。具体快捷键如下：



```lisp
Shortcut (Only in Neotree Buffer)

n next line ， p previous line。
SPC or RET or TAB Open current item if it is a file. Fold/Unfold current item if it is a directory.
g Refresh
A Maximize/Minimize the NeoTree Window
H Toggle display hidden files
C-c C-n Create a file or create a directory if filename ends with a ‘/’
C-c C-d Delete a file or a directory.
C-c C-r Rename a file or a directory.
C-c C-c Change the root directory.
```

## 复制当前buffer的路径

M-m f y

非常实用的快捷键。



## 启用git layer

参考 https://github.com/syl20bnr/spacemacs/tree/master/layers/%2Bsource-control/git



## 使用smart search搜索

M-m / 然后输入想要搜索的文字，就可以立刻搜索当前目录树里面的文件，非常好用。

不再需要进入eshell再grep了。



## org相关

## 导出为odt文件

C-c C-e看不到菜单，无所谓了，命令行依旧可以工作

M-x org-odt-export-to-odt就好。

## odt/html中文优化

github上有一个中文优化工具，可以这样安装。我在自己的layer里面安装不成功。所以用全局的方法。

在.spacemacs文件中

```lisp
   dotspacemacs-additional-packages '(org-chinese-utils)
```

同时在这里加上初始化代码：





```lisp
(defun dotspacemacs/user-config ()
  "Configuration function for user code.
This function is called at the very end of Spacemacs initialization after
layers configuration.
This is the place where most of your configurations should be done. Unless it is
explicitly specified that a variable should be set before a package is loaded,
you should place your code here."
  (require 'org)
  (require 'org-chinese-utils)
  (ocus-enable)
  )
```

具体优化功能参考网站上的说明：



https://github.com/tumashu/org-chinese-utils

我也是装了第一次用，还没有验证功能是否很好。



# 编程相关

## 代码折叠

这里需用命令，快捷键我还没有找到。自己也懒得定制。

M-x evil-open-fold(s) 展开代码

M-x evil-close-fold(s) 折叠代码



## 括号匹配

什么都不用作，自带。最牛的一点是居然将当前匹配的一对括号用下划线和高亮显示。比之前我用的任何方法都好。

写lisp最头疼的是什么？就是难以在一堆括号里面迅速找到匹配的那一对。现在福音来了。



## 自动格式化

在~/.spacemacs中启用c-c++ layer后，然后在dean layer的package.el中添加下面一段



```lisp
;; clang
(defun dean/c-c++()
  (use-package c-c++)
  )

(setq-default dotspacemacs-configuration-layers
              '((c-c++ :variables
                       c-c++-default-mode-for-headers 'c++-mode)))
(setq-default dotspacemacs-configuration-layers
              '((c-c++ :variables c-c++-enable-clang-support t)))

;; Bind clang-format-region to C-M-tab in all modes:
(global-set-key [C-M-tab] 'clang-format-region)
;; Bind clang-format-buffer to tab on the c++-mode only:
(add-hook 'c++-mode-hook 'clang-format-bindings)
(defun clang-format-bindings ()
  (define-key c++-mode-map [tab] 'clang-format-buffer))
```



现在只需要按下tab键，就能够自动格式化代码，效果很好。



# dired mode

## 用外部程序打开文件

在.spacemacs中添加openwith package



```vbnelispt
dotspacemacs-additional-packages '(org-chinese-utils openwith)
```

并初始化，最后三行

```lisp
(defun dotspacemacs/user-config ()
  "Configuration function for user code.
This function is called at the very end of Spacemacs initialization after
layers configuration.
This is the place where most of your configurations should be done. Unless it is
explicitly specified that a variable should be set before a package is loaded,
you should place your code here."
  (require 'org)
  (require 'org-chinese-utils)
  (ocus-enable)
  (require 'openwith)
  (setq openwith-associations '(("\\.odt\\'" "libreoffice" (file))))
  (openwith-mode t)
  )
```

重启emacs后安装。配在自己的layer里面不成功。所以暂时也放在.spacemacs中了。



现在在dired 模式下，选中一个文件按Enter，libreoffice就立刻弹出来了。

默认dired mode会在emacs里面打开pdf, pptx, docx和图片。odt不能打开，所以我就处理了一下odt文件。













