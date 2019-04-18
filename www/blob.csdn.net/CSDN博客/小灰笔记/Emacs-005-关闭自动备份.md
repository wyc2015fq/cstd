# Emacs-005-关闭自动备份 - 小灰笔记 - CSDN博客





2017年12月26日 00:06:54[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1300








Emacs-005-关闭自动备份

       默认情况下，每次编辑后编辑文件所在的目录都会生成一个备份文件。时间久了看着真是难受！在读入门文档的时候看到了下面的描述，感觉找到了解决的出路。

![](https://img-blog.csdn.net/20171226000607758?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       尝试修改配置文件，增加以下内容：

![](https://img-blog.csdn.net/20171226000616920?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       之后重启Emacs，不断备份的功能已经被禁用。




目前的Emacs配置：



```
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
```





