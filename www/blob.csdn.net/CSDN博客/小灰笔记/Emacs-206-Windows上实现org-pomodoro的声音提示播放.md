# Emacs-206-Windows上实现org-pomodoro的声音提示播放 - 小灰笔记 - CSDN博客





2018年03月27日 18:55:08[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：129








       声音提示功能最开始就知道有，但是不知道为什么一直不能用。我以为还需要比较细致的设置，也就没有在意。

       切换到Mac上之后发现这个功能好了，看起来是Windows上的配置有点小问题。简单查看了代码，问题定位初步为播放器原因，org-pomodoro使用的播放器Windows不存在。

       尝试修改源代码，使用Windows Media Player，失败；再次尝试Mplayer，又是失败。网络上找了一下，在一个国外的网站上找到了相应的说明——Windows上需要继续安装插件。

```
powershell
                      sound-wav
```

       安装上面的package之后，声音提示功能正常了。但是提示的时候会出现一个多余的window，倒也不是很大的问题。后续继续修复窗口分割的这个小问题吧！




