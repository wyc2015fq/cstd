# Emacs-105-spacemacs在Mac中启动最大化且留出任务栏和程序坞 - 小灰笔记 - CSDN博客





2018年03月06日 11:38:52[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：471








Emacs-105-spacemacs在Mac中启动最大化且留出任务栏和程序坞

实现相应的配置只需要修改Spacemacs的dot文件，具体如下：

```
dotspacemacs-fullscreen-at-startup nil
   ;; If non nil `spacemacs/toggle-fullscreen' will not use native fullscreen.
   ;; Use to disable fullscreen animations in OSX. (default nil)
   dotspacemacs-fullscreen-use-non-native nil
   ;; If non nil the frame is maximized when Emacs starts up.
   ;; Takes effect only if `dotspacemacs-fullscreen-at-startup' is nil.
   ;; (default nil) (Emacs 24.4+ only)
   dotspacemacs-maximized-at-startup t
```

配置完成后，重启Emacs即可看到效果。



