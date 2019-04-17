# Emacs-044-增加插件源并安装插件 - 小灰笔记 - CSDN博客





2018年01月13日 18:42:45[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：614








Emacs-044-增加插件源

              插件管理是最终便捷使用Emacs的必然选择，其实不仅仅是Emacs，其他的也是。现在Emacs内置了一个插件管理，但是里面的插件并不是很多。如果要使用更多的插件，可以选择MELPA源等其他方式。

              配置起来也比较简单，我直接从网上找来了一个其他人的配置，还附加了自动安装等功能，具体如下：

(when (>= emacs-major-version 24) ;;判断Emacs的版本，必须高于24

     (require'package)

    (package-initialize) ;;这部分用来取代原来配置文件中最上面的自动生成配置，原来的也就删除

     (setqpackage-archives '(("gnu"   ."http://elpa.emacs-china.org/gnu/")

                     ("melpa" . "http://elpa.emacs-china.org/melpa/"))));;这里增加了两个中国镜像的源站。



 ;; cl - Common LispExtension

 (require 'cl)



 ;; Add Packages

 (defvar my/packages'(

                                                          ;;下面这部分用来说明需要安装的插件包，所有的信息存储到一个变量中，后面使用。

                ;; ---Auto-completion ---

               company

                ;; ---Better Editor ---

               hungry-delete

                swiper

               counsel

               smartparens

                ;; ---Major Mode ---

               js2-mode

                ;; ---Minor Mode ---

               nodejs-repl

               exec-path-from-shell

                ;; ---Themes ---

               monokai-theme

                ;;solarized-theme

                )"Default packages")

;; 下面这部分用到了上面定义的软件包选择变量，然后设定选中插件的值。

 (setqpackage-selected-packages my/packages)

;; 定义一个函数，查看软件插件包是否安装，如果没安装返回nil

 (defunmy/packages-installed-p ()

     (loop for pkg inmy/packages

           when (not(package-installed-p pkg)) do (return nil)

           finally(return t)))

;; 有安装包没安装的话，进行处理

 (unless(my/packages-installed-p)

     (message"%s" "Refreshing package database...")

    (package-refresh-contents)

     (dolist (pkgmy/packages)

       (when (not(package-installed-p pkg))

;; 这里应该是安装动作了

        (package-install pkg))))

;; 这部分用于Mac，暂且也就不去理解了

 ;; Find ExecutablePath on OS X

 (when (memqwindow-system '(mac ns))

  (exec-path-from-shell-initialize))



              这样，Emacs在重启的时候就会检查上面的配置。如果相应的软件包没有安装则会自动安装。



