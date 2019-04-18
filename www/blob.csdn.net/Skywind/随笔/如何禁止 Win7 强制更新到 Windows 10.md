# Skywind Inside » 如何禁止 Win7 强制更新到 Windows 10 ?
## 如何禁止 Win7 强制更新到 Windows 10 ?
July 4th, 2016[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
岳父岳母，爹妈的 Windows 7 纷纷被自动升级成 Windows 10 各种问题（一台画面变成非全屏，解析度被莫名奇妙降低，占不满显示器、运行卡，另一台台式机无线网卡不能用，还有一台某炒股软件跑不起来，按win键弹出菜单时会顿一下）。
花费了我两个整天的时间帮他们降级，找了很多方法，网上大部分都不能对新版 Gwx起作用
终于，找到了并且永久性锁定自动升级的最直接方式了：
```text
regedit：
HKEY_LOCAL_MACHINE 下面
SOFTWARE\Policies\Microsoft\Windows 下面新建项目（目录）Gwx
然后在
SOFTWARE\Policies\Microsoft\Windows\Gwx 下面新建 DWORD 值：
DisableGwx=1
```
好了，Windows 结扎手术完成，不用当心一不留神弄出个 Windows 10 来给大伙惹麻烦了。
补充：本来上次回家时间就短，好些事情没办，好些朋友没见，好了，微软一个强制升级，就把你锁家里了。
自己的事情也算小事了，爹妈平常和岳父母平日和我们聚少离多，也就爱炒个股看个基金寻找下乐趣，你把人家弄的用不了了，不好用了，老人家自己不会弄，又舍不得花钱请人修，只能眼巴巴花好几个月的时间等着你回去帮他们解决。这样的感受是很不好的，就像公园里两个大爷正高兴的下着棋，过来个顽皮小孩一把把老人的棋盘掀湖里去了，周围的人都会觉得小孩有些缺乏教养，没有公德心一般。
很多喜欢宠物的人为了不给大家添麻烦都倡导给自己的狗或猫做节育手术，真爱宠物就要对它负责，所以我们也本着对windows负责的态度对它做个结扎，防止它趁你不留神在外面闯祸，避免给大家添麻烦，对，公德心嘛。
