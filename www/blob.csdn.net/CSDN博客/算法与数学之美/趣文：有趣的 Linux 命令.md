# 趣文：有趣的 Linux 命令 - 算法与数学之美 - CSDN博客
2018年09月05日 21:34:51[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：39
1. sl 命令
sl是指“steam locomotive（蒸汽机车）”，你会看到一辆火车从屏幕右边开往左边。。。。。。
安装  $ sudo apt-get install sl 
运行  $ sl
命令有 -alFe几个选项，
-a An accident seems to happen. You'll feel pity for people who cry for help.
-l shows little one.
-F It flies.
-e Allow interrupt by Ctrl+C.
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkxAYzj7ZYLtqSEq1meRJjMtBaFfpqTvstFI5nBe0OvFiaQBD5gYGxV0KVRUyVVkA3BVQhmHfhp2ia4g/640?wx_fmt=png)
可以给别人来个恶作剧，搞个关联，他一敲ls，不知道的肯定很有效果
$alias ls=sl
2. fortune  命令
输出一句话，有笑话，名言什么的 (还有唐诗宋词sudo apt-get install fortune-zh)
安装 $sudo apt-get install fortune
运行 $fortune
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkxAYzj7ZYLtqSEq1meRJjMtgwJTJXpL5OMuNQoJrriazyrkWxibP0LlPyE9TgSoRuDxqGgd5sHsyBZQ/640?wx_fmt=png)
3. cowsay 命令
用ASCII字符打印牛，羊等动物，还有个cowthink，这个是奶牛想，那个是奶牛说，哈哈，差不多
安装  $sudo apt-get install cowsay
运行  $cowsay "I am not a cow, hahaha"
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkxAYzj7ZYLtqSEq1meRJjMtT3qqdiaCYmTiaI28caomhzefDBic4TeK1Yb3wy78icowbHEIkh2hABpZUg/640?wx_fmt=png)
$cowsay -l  查看其它动物的名字，然后-f跟上动物名，如
$cowsay -f tux "坑爹啊"
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkxAYzj7ZYLtqSEq1meRJjMt9HEpwGCb5MM3L57Wu1pgLb4d9KSP9pyqFbVmxIzgXOe9dR0icQWLjGQ/640?wx_fmt=png)
还可以让cowsay说出fortune的内容，就像这样：
$fortune | cowsay
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkxAYzj7ZYLtqSEq1meRJjMtax9IUcqeYLqIZq0ibGdxvtEZ80uneogicFlkFW44ktL8w2L7qvHhkQXw/640?wx_fmt=png)
4. cmatrix 命令
这个很酷！《黑客帝国》那种矩阵风格的动画效果
安装   $ sudo apt-get install cmatrix
运行   $cmatrix
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkxAYzj7ZYLtqSEq1meRJjMtQicC4hsL4wlTUJcnQPCUOCkiamk2wENCnXV0ibdvWfofBUo8ib4xCjAiblQ/640?wx_fmt=png)
5. figlet 、toilet命令
艺术字生成器，由ASCII字符组成，把文本显示成标题栏。此外还有banner这个命令
安装  $sudo apt-get install figlet
$sudo apt-get install toilet
运行  $figlet fuck you !
$toilet i love you
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkxAYzj7ZYLtqSEq1meRJjMtdIQibJn6XPubqT6vy64WC3DQ4DuWE2BjkUeQ2TK3JlZaZoy2WUrDK2w/640?wx_fmt=png)
toilet还可以添加颜色，里面的选项请自己man一下
$ toilet -f mono12 -F gay a girl
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkxAYzj7ZYLtqSEq1meRJjMtEiccgEhOgcgyguWS4uvTYKIUu06Ddq0NcIIHUd2HFL9xqthLicPAJscQ/640?wx_fmt=png)
6. oneko 命令
桌面上出现一直喵星人，跟着你的鼠标跑，你不动了它就睡觉。哈哈，这个挺不错
安装 $sudo apt-get install oneko
运行 $oneko               要关掉这家伙，按ctrl+c 结束
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkxAYzj7ZYLtqSEq1meRJjMt7mud2lRricmJsUHoxaQxtFwzaJsfzklbPxBeMyM3YKkVngtrrG2C7PA/640?wx_fmt=png)
7. xeyes 命令
在屏幕上出现一双眼睛，盯着你的鼠标指针
安装运行同上
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkxAYzj7ZYLtqSEq1meRJjMtmK9sKVKa85qYRbpZdMQ9LeHjdS9j0GVro028jxoqNeubnl7CuLnWlw/640?wx_fmt=png)
8. yes 命令
输出无穷无尽的字符,按ctrl+c结束，如
$yes  我很NB
9. cal 9 1752
cal是打印日历，不过这个是很奇葩的一个月，
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkxAYzj7ZYLtqSEq1meRJjMtiaWUuMP9TRyjh0RgwtyymlvKuryRdh4JHk45tZ0yG8kO8ZTGu5GmkMQ/640?wx_fmt=png)
10. shred
覆盖搞乱文件，就是文档粉碎，哈哈，要把你私藏的大片种子和电影销毁，不被恢复出来，就靠它了
11. factor
分解因数，这个。。。小学生可以用一下
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkxAYzj7ZYLtqSEq1meRJjMt1d2hrcicYiasvQyfcIt6nfSuWfOglKib0uib48cVRLJSV2IFjXJznckuaQ/640?wx_fmt=png)
12. 挨个敲下面这一堆, aptitude没有的可以安装上
aptitude moo
aptitude -v moo
aptitude -vv moo
aptitude -vvv moo
aptitude -vvvv moo
aptitude -vvvvv moo
aptitude -vvvvvv moo
aptitude -vvvvvvv moo
Linux还有很多你意想不到的有趣的命令和功能哦，不得不感叹开源系统的强大，有兴趣你可以研究这些命令的源码从而修改或开发自己有趣的Linux命令供大家分享娱乐！！！
∑编辑 | Gemini
来源 | mylinuxbook
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabky7x6u1VxMVMia4MLibNzC2nrumY3zDflTsCeoM04M1BrkvPny8tsw6hYkIicUr42iarLmadL2x6JwV6A/640?wx_fmt=png)
算法数学之美微信公众号欢迎赐稿
稿件涉及数学、物理、算法、计算机、编程等相关领域，经采用我们将奉上稿酬。
投稿邮箱：math_alg@163.com
