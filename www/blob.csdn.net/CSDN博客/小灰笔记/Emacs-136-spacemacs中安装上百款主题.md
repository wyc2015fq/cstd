# Emacs-136-spacemacs中安装上百款主题 - 小灰笔记 - CSDN博客





2018年03月13日 20:04:20[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：547








            最初在dot文件中的如下内容中修改我想要的主题：

   ;; List of themes,the first of the list is loaded when spacemacs starts.

   ;; Press<SPC> T n to cycle to the next theme in the list (works great

   ;; with 2 themesvariants, one dark and one light)

   dotspacemacs-themes'(solarized-light

                        solarized-dark

                        )

            如果我需要的主题还没有安装，那么会自动联网安装。但是有一个问题就是我后期安装的主题如果不用会被自动删除。

            找到的解决方案是增加一个可以安装上百款theme的layer，具体修改配置：


![](https://img-blog.csdn.net/20180313200324847?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



            增加66行的配置，然后联网重启。现在，我的spacemacs中已经安装了上百款的主题，且主题在不用的时候依然存在。
![](https://img-blog.csdn.net/20180313200406308?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




