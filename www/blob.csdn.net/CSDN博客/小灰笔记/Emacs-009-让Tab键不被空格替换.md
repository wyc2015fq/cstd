# Emacs-009-让Tab键不被空格替换 - 小灰笔记 - CSDN博客





2017年12月27日 00:48:41[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：380








                
Emacs-009-让Tab键不被空格替换
Emacs自带的缩进太让人苦恼了！动不动就把Tab替换成空格，虽说有时候会让代码看上去好看一点，但是对我来说绝大多数的时候是给我增加苦恼。我时长写个Python脚本，但是偶尔退格的时候要么需要按4次退格键要么需要按8次退格键，太苦恼了！我的主业还是C程序员，可是默认情况下又把Tab键替换成了2个空格。过去将近十年的经历中，我都习惯了缩进是4个字符宽度，可现在居然变成了2个字符宽度！2个字符也就罢了，退格的时候依然不能够一键退格！
如果，非得让一步，我宁愿选择只用Tab按键，显示为8个字符宽度。当然，如果能像VIM一样设置为显示4个字符宽度那就最好了。
各种网络帖子搜索遍了，最终还是在老外的一个帖子中找到了一点有用的解释。那就是可以让Tab永远为Tab而不被空格替换。实现方式如下：

(global-set-key (kbd "TAB") 'self-insert-command);           # in every mode

OR

(define-key text-mode-map (kbd "TAB") 'self-insert-command); # only in text-mode
上面两个分别是实现所有模式中设置和文本模式中设置两种配置，对我来说，我期待的应该就是全部了！因为我宁愿全部，哪怕后续在进行替换处理。如此，我的Emacs配置文件变成了下面的状态：

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

;; Config Tab

;;;; This will force Tab to be just one Tab.

(global-set-key (kbd "TAB") 'self-insert-command)
编辑一个简单的C代码，效果如下：

![](https://img-blog.csdn.net/20171227004824443?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

现在，编辑的时候已经可以实现Tab插入了。虽然，现在输入的Tab键在整个句子编辑完成之后还是会被Emacs修改，但是已经有进步了！完不成其他的功能，暂时不去学习其他的Emacs功能了，一直啃下去，相信迟早我会把这个Tab问题搞定！
            


