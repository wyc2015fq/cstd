# Rainmeter新手教程 - 三少GG - CSDN博客
2014年09月11日 16:55:05[三少GG](https://me.csdn.net/scut1135)阅读数：1593
# Rainmeter新手教程(原创，绝对好懂！)
![墨绯](http://img3.douban.com/icon/u48431313-4.jpg)
### 来自: [墨绯](http://www.douban.com/group/people/48431313/)(吾墨子绯，携手相看天涯。)2012-09-28 15:22:29
Rainmeter作为一款桌面美化软件，自定义行极强。可以为你制作出属于你自己风格的电脑桌面。这篇新手教程实际上是献给我自己这个新手的，是我对着半个月学习rainmeter的一个总结。所以会以最简单的方式为你呈现这个软件的基本运用。 
1.Rainmeter的下载安装 
(1)首先进入rainmeter的官网 ，[http://rainmeter.cn/cms/](http://rainmeter.cn/cms/)
下载32bty版本的rainmeter，可以选择rainmeter的安装位置，无论是安装在那个位置都不影响rainmeter的使用 
(2)rainmeter安装后，在安装位置会出现一个“Rainmeter”文件夹，里面会有一些rainmeter的默认文件，可以不必理会。安装成功后，rainmeter会在“我的文档”下新建“Rainmeter”文件夹，打开“Rainmeter”会有“Skins”的子文件夹，该文件夹即为rainmeter的皮肤文件夹（这些不用自己新建，在安装成功后，会自动创建的。） 
(3)安装成功后，桌面上会有rainmeter默认的皮肤，可以将鼠标移至该处，右击--“关闭皮肤”，便可将那些奇丑无比的默认皮肤关掉。 
(4)安装成功后，会在任务栏上出现小水滴图标（win7安装成功后，可能不在任务栏，而是在“▲”里“显示影藏的图标”，可以直接长按鼠标左键，将rainmeter拉到任务栏上即可。） 
2.关于rainmeter皮肤的下载使用 
(1)那些很炫很漂亮的皮肤不必会自己制作，只要到相关网站下载即可。推荐三个网址： 
[http://customize.org/rainmeter/skins/popular](http://customize.org/rainmeter/skins/popular)
（这是一个英文网站，找到喜欢的皮肤， 点击download，即可下载，保存路径为“在我文档”下的“rainmeter”下的“skins” ） 
[http://tochpc.ru/](http://tochpc.ru/)
（这是一个俄文网站，点击Rainmeter，可以找到喜欢的皮肤，点击download，保存路径与上面的方式一样） 
[http://bbs.rainmeter.cn/](http://bbs.rainmeter.cn/)
（中文社区，里面有许多关于使用rainmeter的心得什么的，要下载里面的资源需要注册。） 
(2) 右击任务栏上的小水滴，找到“刷新全部”，这样才能载入你所下载的新皮肤。（每次下载了新皮肤都一定要“刷新全部”一次。） 
有两种方式可以安装新皮肤： 
A 右击小水滴，鼠标滑过“皮肤”选项，自然会出现你所安装的皮肤，找到你想要的，按顺序直到出现以.ini的文件，左击，即可。此时桌面上就会出现你所想要的皮肤。你发现这是皮肤是可以随意拖动的，不喜欢的可以直接在皮肤表面右击“关闭皮肤”即可。 
B 右击小水滴，左击“管理”会出现“Rainmeter管理器”，皮肤部分会有你安装的所有皮肤文件，找到喜欢的不断打开，直到出现.ini文件，左击就会在桌面上出现，不喜欢的可以与上面用同样的方法关闭皮肤。 
(3)关于皮肤上的图标的链接 
右击皮肤，在“编辑皮肤”里 找到 下面的一段 代码 
LeftMouseDownAction=!Execute ["#Path1#"] 
或者是 LeftMouseUpAction=!Execute ["#Path1#"] 
注意看 括号里的 #Path1# 
把这个改成你所要定位的软件或者文件夹的路径 比如 G:\电影 或者 D:\常用软件\QQ\Bin\QQ.exe 就可以了 
(4)关于天气的链接 
有以下几种可能： 
A 有的是填写天气代码，如： 
Please replace your location code of Weather.com below, e.g: Taipei is TWXX0021 
那只要在有[Variables] 
Location=CHXX0116 
;Location=CHXX0116或者是Code=CHXX0116 
把Location或是code等号后天气代码改成你所在地就可以了，天气代码可以去雅虎天气查找。 
B 有的是给出刷天气的网址，如： 
Url=[http://xml.weather.com/weather/local/CHXX0062?cc=*&unit=#Type#&dayf=6](http://xml.weather.com/weather/local/CHXX0062?cc=*&unit=#Type%23&dayf=6)
只要在有天气代码的地方改成你所在城市的代码即可了。 
这里要注意到“unit=”，m表示摄氏温度，f表示华氏温度，中国人的话还是用m好了。 
(5) 关于音乐播放器的链接 
国内研究的皮肤很少，所以国内音乐播放器的皮肤非常少，之前有在一些论坛上看到的都是“千千静听”的皮肤，很少有“酷狗”很“QQ音乐”的。所以如果习惯用这些的，可以直接无视这一段。 
国外多使用WMP（windows music player）或是Winamp（国外非常流行的播放器，自己百度好了），或是iTunes。很多皮肤都是特制的，也就是说它若是标了iTunes skin 那么就只能用于iTunes了。 
但是还有一部分是却是可以自定义的，如 
[Player] 
Measure=Plugin 
Plugin=NowPlaying.dll 
PlayerName=WMP 
PlayerType=TITLE 
在PlayerName后填上你用的那个播放器即可，但是貌似也只适合上面的三个，你们自己试试其他的可以不。 
注意：你在编辑修改完以上（3）（4）（5）的程序后到记得的先点击“保存”，然后关闭，再在皮肤表面右击“刷新皮肤”才能出现效果。 
3 关于设计桌面 
(1) 皮肤的选配 
你下到的皮肤可能是一套的，也可能只是某一部分的，所以就根据自己喜欢的方式选择即可。自定义你的桌面。 
(2) 壁纸的选择 
壁纸人人都会下载，这里只给几个我觉得挺不错的网站 
[http://wallbase.cc/home](http://wallbase.cc/home)
[http://www.socwall.com/](http://www.socwall.com/)
[http://www.mistwall.com/](http://www.mistwall.com/)
[http://www.themebin.com/category/hd-wallpapers/page/2/](http://www.themebin.com/category/hd-wallpapers/page/2/)
[http://wallpaper.desktx.com/vector/index_2.html](http://wallpaper.desktx.com/vector/index_2.html)
(3) 桌面图标的隐藏 
在桌面右击——“查看”——将显示桌面图标小勾去掉即可。 
这样你就可以用自定义你所喜欢的桌面啦！ 

