# Emacs-057-hippie-expand使用 - 小灰笔记 - CSDN博客





2018年01月16日 00:10:40[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：178








                
Emacs-057-hippie-expand使用
hippie-expand通常是在company不是很好用的时候作为一个补全的方式使用，在很多company不能够使用的时候可以提供补全功能。
这里从网上找来一个增强的配置功能，可以把所有Buffer中的label搜集起来用于补全。同时，这个还能够实现一部分目录等功能的补全。这个功能跟VIM自带的那个补全功能十分相似，通过使用的相应速度也是十分快的。
找到的网络上的配置如下：

;; hippie-expand config

(setq hippie-expand-try-function-list '(try-expand-debbrev

                                        try-expand-debbrev-all-buffers

                                        try-expand-debbrev-from-kill

                                        try-complete-file-name-partially

                                        try-complete-file-name

                                        try-expand-all-abbrevs

                                        try-expand-list

                                        try-expand-line

                                        try-complete-lisp-symbol-partially

                                        try-complete-lisp-symbol))

;; This is not a must configuration on Windows, "s" is the command key on Mac. When on Windows, just use M-/

(global-set-key (kbd "s-/") 'hippie-expand)
实现上面的配置之后，可以使用M-/来实现补全。
上面的配置中抄来了一个s-/，我自己加了一个注释。其实在Windows下面直接使用默认的这种方式也就可以。
            


