# vim/vi  moshixiadechazhao - YZXnuaa的博客 - CSDN博客
2018年04月20日 15:51:34[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：83
我们以samba的配置文件为例，搜索一个user的字符串。
vim /etc/samba/smb.conf
打开smb.conf
命令模式下，输入/user
/要搜索的字符串或者字符
按下回车之后，可以看到vim已经把光标移动到该字符处和高亮了匹配的字符串（vi没高亮，因为它没有颜色）
怎么查看下一个匹配呢？按下n(小写n)
我们一直按n到最后，红色的字提示BOTTOM（已经到底了，尽头了），说明匹配的字符串已经到此处是最后一个。再按n会回到TOP（第一个匹配成功的字符串）
那么如何跳转到上一个匹配呢？按下N（大写N）。
你可以按下Caps Lock键切换大小写，也可以在小写状态按下Shift + n
如果我们想从文件的结尾往开始处搜索呢？
使用?user
?要搜索的字符串或者字符

