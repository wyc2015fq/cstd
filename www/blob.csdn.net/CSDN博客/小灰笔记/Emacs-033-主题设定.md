# Emacs-033-主题设定 - 小灰笔记 - CSDN博客





2018年01月08日 23:48:59[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：517








                
Emacs-033-主题设定

没想到Emacs的主题设定这么简单，不知道之前的版本是不是这样呢？

![](https://img-blog.csdn.net/20180108234938012?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


上面这个就是进入主题设定的方法，自然，GUI的配置操作也有对应的elisp描述。 不过，在主题上还是GUI设置来的干脆些，毕竟是感官上的东西。上面操作后，会进入到下面的界面：



![](https://img-blog.csdn.net/20180108235000390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

小方格用于选择主题，并且生效，但是如果想后期Emacs启动的时候就生效需要点击保存主题设置的按钮。点击之后，Emacs的配置文件中会增加相应的配置。到现在为止，我个人的Emacs配置文件内容如下：

;; Added by Package.el.  This must come before configurations of

;; installed packages.  Don't delete this line.  If you don't want it,

;; just comment it out by adding a semicolon to the start of the line.

;; You may delete these explanatory comments.

(package-initialize)



(custom-set-variables

 ;; custom-set-variables was added by Custom.

 ;; If you edit it by hand, you could mess it up, so be careful.

 ;; Your init file should contain only one such instance.

 ;; If there is more than one, they won't work right.

 '(ansi-color-names-vector

   ["#073642" "#dc322f" "#859900" "#b58900" "#268bd2" "#d33682" "#2aa198" "#657b83"])

 '(company-abort-manual-when-too-short t)

 '(company-auto-complete t)

 '(company-auto-complete-chars (quote ignore))

 '(company-idle-delay 0)

 '(company-minimum-prefix-length 1)

 '(company-selection-wrap-around t)

 '(company-show-numbers t)

 '(company-tooltip-align-annotations t)

 '(company-tooltip-flip-when-above t)

 '(company-tooltip-idle-delay 0)

 '(compilation-message-face (quote default))

 '(cua-global-mark-cursor-color "#2aa198")

 '(cua-normal-cursor-color "#839496")

 '(cua-overwrite-cursor-color "#b58900")

 '(cua-read-only-cursor-color "#859900")

 '(custom-enabled-themes nil)

 '(custom-safe-themes

   (quote
("d677ef584c6dfc0697901a44b885cc18e206f05114c8a3b7fde674fce6180879" "8aebf25556399b58091e533e455dd50a6a9cba958cc4ebb0aab175863c25b9a4" default)))

 '(fci-rule-color "#073642")

 '(highlight-changes-colors (quote ("#d33682" "#6c71c4")))

 '(highlight-symbol-colors

   (--map
(solarized-color-blend it "#002b36" 0.25)
(quote
("#b58900" "#2aa198" "#dc322f" "#6c71c4" "#859900" "#cb4b16" "#268bd2"))))

 '(highlight-symbol-foreground-color "#93a1a1")

 '(highlight-tail-colors

   (quote
(("#073642" . 0)
("#546E00" . 20)
("#00736F" . 30)
("#00629D" . 50)
("#7B6000" . 60)
("#8B2C02" . 70)
("#93115C" . 85)
("#073642" . 100))))

 '(hl-bg-colors

   (quote
("#7B6000" "#8B2C02" "#990A1B" "#93115C" "#3F4D91" "#00629D" "#00736F" "#546E00")))

 '(hl-fg-colors

   (quote
("#002b36" "#002b36" "#002b36" "#002b36" "#002b36" "#002b36" "#002b36" "#002b36")))

 '(magit-diff-use-overlays nil)

 '(nrepl-message-colors

   (quote
("#dc322f" "#cb4b16" "#b58900" "#546E00" "#B4C342" "#00629D" "#2aa198" "#d33682" "#6c71c4")))

 '(package-selected-packages

   (quote
(company-statistics counsel-ebdb math-symbol-lists company-math company-ebdb company auto-overlays auto-correct dash)))

 '(pos-tip-background-color "#073642")

 '(pos-tip-foreground-color "#93a1a1")

 '(smartrep-mode-line-active-bg (solarized-color-blend "#859900" "#073642" 0.2))

 '(term-default-bg-color "#002b36")

 '(term-default-fg-color "#839496")

 '(vc-annotate-background nil)

 '(vc-annotate-background-mode nil)

 '(vc-annotate-color-map

   (quote
((20 . "#dc322f")
(40 . "#c85d17")
(60 . "#be730b")
(80 . "#b58900")
(100 . "#a58e00")
(120 . "#9d9100")
(140 . "#959300")
(160 . "#8d9600")
(180 . "#859900")
(200 . "#669b32")
(220 . "#579d4c")
(240 . "#489e65")
(260 . "#399f7e")
(280 . "#2aa198")
(300 . "#2898af")
(320 . "#2793ba")
(340 . "#268fc6")
(360 . "#268bd2"))))

 '(vc-annotate-very-old-color nil)

 '(weechat-color-list

   (quote
(unspecified "#002b36" "#073642" "#990A1B" "#dc322f" "#546E00" "#859900" "#7B6000" "#b58900" "#00629D" "#268bd2" "#93115C" "#d33682" "#00736F" "#2aa198" "#839496" "#657b83")))

 '(xterm-color-names

   ["#073642" "#dc322f" "#859900" "#b58900" "#268bd2" "#d33682" "#2aa198" "#eee8d5"])

 '(xterm-color-names-bright

   ["#002b36" "#cb4b16" "#586e75" "#657b83" "#839496" "#6c71c4" "#93a1a1" "#fdf6e3"]))

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

(setq default-tab-width 4)

(setq backward-delete-char-untabify-method nil)

(defun my-c-mode-hook ()

  (setq c-basic-offset 4

        c-indent-level 4

        c-default-style "bsd"))

(add-hook 'c-mode-common-hook 'my-c-mode-hook)

;; config load-path

(add-to-list 'load-path "C:/Grey/.emacs.d/lisp")

(add-to-list 'custom-theme-load-path "C:/Grey/.emacs.d/themes")

;; Modeify the cursor-style

(setq-default cursor-type 'bar)

;; config for company

(add-hook 'after-init-hook 'global-company-mode)

;; try to config company for clang.

(add-hook 'c-mode-hook 'company-mode)




