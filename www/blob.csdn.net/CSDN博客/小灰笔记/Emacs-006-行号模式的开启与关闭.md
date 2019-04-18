# Emacs-006-行号模式的开启与关闭 - 小灰笔记 - CSDN博客





2017年12月26日 19:58:09[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：258








Emacs-006-行号模式的开启与关闭

       Emacs内置了一个行号模式，可以显示光标所在的行的行号。可以通过line-number-mode命令进行开关的切换。

![](https://img-blog.csdn.net/20171226195646938?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       以上是行号模式没开启的时候。

![](https://img-blog.csdn.net/20171226195734054?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       上面是通过line-number-mode开启后出现的提示与效果。

![](https://img-blog.csdn.net/20171226195747797?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       上面是从开启到关闭切换你的时候出现的提示。

       如果要在启动的时候开启或者关闭，可以使用.emacs文件进行指定配置。

              开启方式：(setq line-number-mode t)

              关闭方式：(setq line-number-mode nil)

       我自己的配置中设置了功能的常开，目前的emacs配置文件内容如下：

(custom-set-variables

 ;;custom-set-variables was added by Custom.

 ;;If you edit it by hand, you could mess it up, so be careful.

 ;;Your init file should contain only one such instance.

 ;;If there is more than one, they won't work right.

 )

(custom-set-faces

 ;;custom-set-faces was added by Custom.

 ;;If you edit it by hand, you could mess it up, so be careful.

 ;;Your init file should contain only one such instance.

 ;;If there is more than one, they won't work right.

 '(default ((t (:family "Courier New":foundry "outline" :slant normal :weight normal :height 98 :widthnormal)))))

;; Disable files backup function

(setq make-backup-files nil)

(setq line-number-mode t)



