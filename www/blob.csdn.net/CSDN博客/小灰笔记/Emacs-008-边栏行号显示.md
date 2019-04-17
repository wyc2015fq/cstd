# Emacs-008-边栏行号显示 - 小灰笔记 - CSDN博客





2017年12月26日 20:35:48[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：313








                
Emacs-008-边栏行号显示

Emacs中，边栏行号显示比行号模式要直观多了。开启可以通过命令输入的方式开启，用到的命：linum-mode。这个命令也有点像开关的行为，重复输入会不断进行设置状态的翻转。

通过linum-mode开启的效果：



![](https://img-blog.csdn.net/20171226203514469?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


再次输入，出现的效果：

![](https://img-blog.csdn.net/20171226203523686?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


如果希望Emacs启动的时候就开启这个功能，需要在.emacs配置文件中加入以下配置：

(global-linum-mode t)
目前我个人的Emacs配置文件积累到如下状态：

(custom-set-variables

 ;; custom-set-variables was added by Custom.

 ;; If you edit it by hand, you could mess it up, so be careful.

 ;; Your init file should contain only one such instance.

 ;; If there is more than one, they won't work right.

 )

(custom-set-faces

 ;; custom-set-faces was added by Custom.

 ;; If you edit it by hand, you could mess it up, so be careful.

 ;; Your init file should contain only one such instance.

 ;; If there is more than one, they won't work right.

 '(default ((t (:family "Courier New" :foundry "outline" :slant normal :weight normal :height 98 :width normal)))))

;; Disable files backup function

(setq make-backup-files nil)

(setq line-number-mode t)

(global-linum-mode t)
            


