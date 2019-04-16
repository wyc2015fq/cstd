# XBMC源代码分析 2：Addons（皮肤Skin） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年01月01日 00:28:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：30









前文已经对XBMC源代码的整体架构进行了分析：

[XBMC源代码分析 1：整体结构以及编译方法](http://blog.csdn.net/leixiaohua1020/article/details/17454977)

从这篇文章开始，就要对XBMC源代码进行具体分析了。首先先不分析其C++代码，分析一下和其皮肤相关的代码。

XBMC 的和皮肤相关的代码位于 "根目录/addons" 里面。可以从官方网站上下载皮肤文件的压缩包，然后解压到该目录下面即可。皮肤文件夹名称一般是“skin.XXXX”形式的，即以“skin.”开头。

XBMC自带的皮肤存储在文件夹“skin.confluence”中。我从网上下载了4个皮肤，解压后，如下图所示。

![](https://img-blog.csdn.net/20131224200247968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

系统默认的皮肤：confluence如图所示。

![](https://img-blog.csdn.net/20131224200619250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可以在“skin”选项里面选择皮肤，如图所示。

![](https://img-blog.csdn.net/20131224200622281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

皮肤“simplicity”如图所示。

![](https://img-blog.csdn.net/20131224200625843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

皮肤“SiO2”如图所示。

![](https://img-blog.csdn.net/20131224200628875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可以看出。不同皮肤之间差距非常的大。皮肤囊括了XBMC所有可以看见的界面元素。可以说不修改源代码，只制作皮肤，也可以完全定制出一套非常个性化的系统。

下面我们以系统自带的皮肤“confluence”为例，分析一下皮肤的构成。

skin.confluence文件夹中，目录结构如图所示：

![](https://img-blog.csdn.net/20131224202017546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

每个文件夹的作用：

> 
720p：界面存放于文件夹里

background：背景图片

font：字体

language：各种语言支持

media：各种图标

sound：声音


例如，background文件夹内容：

![](https://img-blog.csdn.net/20131224202804781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

media文件夹内容：

![](https://img-blog.csdn.net/20131224202808921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下面重点研究720p文件夹中的内容。这个文件夹中存储了界面的布局信息。系统会根据这个文件夹中的布局信息（xml形式）设置窗口的大小，并去其他文件夹中查找相关的素材。

我们以系统的“设置”页面为例研究一下布局信息。系统的布局页面如下图所示。

![](https://img-blog.csdn.net/20131224204004843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

“设置”页面对应的布局信息文件为Settings.xml。

时间所限，就不逐行注释了。语法理解起来还是比较容易的。总结以下几点：

1.语法与HTML类似。最外层的<window>相当于<html>。<controls>相当于<body>。<control>类似于<div>，是可以嵌套的。<content>相当于<ul>，<item>相当于<li>。当然，这只是打个比方，方便理解。

2.各种组件都是<control>，就是属性“type”不一样。例如“image”，“group”等等。<control>中

<left>，<top>，<width>，<height>表示窗口位置；

<animation>表示其动画效果；

<onleft>2</onleft>表示遥控器按向左键时如果焦点还在控件里面，并且己经是最左边一个元素时，将焦点切换到ID为2的控件；<onright>，<onup>，<ondown>与此类似。



```
<!--
雷霄骅
leixiaohua1020@126.com
中国传媒大学/数字电视技术
-->
<?xml version="1.0" encoding="UTF-8"?>
<window>
	<defaultcontrol always="true">9000</defaultcontrol>
	<allowoverlay>no</allowoverlay>
	<controls>
		<include>CommonBackground</include>
		<control type="image">
			<left>0</left>
			<top>100r</top>
			<width>1280</width>
			<height>100</height>
			<texture>floor.png</texture>
			<animation effect="slide" start="0,10" end="0,0" time="200" condition="Window.Previous(Home)">WindowOpen</animation>
			<animation effect="slide" start="0,0" end="0,10" time="200" condition="Window.Next(Home)">WindowClose</animation>
		</control>
		<control type="group">
			<left>90</left>
			<top>30</top>
			<animation type="WindowOpen" reversible="false">
				<effect type="zoom" start="80" end="100" center="640,360" easing="out" tween="back" time="300"/>
				<effect type="fade" start="0" end="100" time="300"/>
			</animation>
			<animation type="WindowClose" reversible="false">
				<effect type="zoom" start="100" end="80" center="640,360" easing="in" tween="back" time="300"/>
				<effect type="fade" start="100" end="0" time="300"/>
			</animation>
			<control type="image">
				<left>5</left>
				<top>5</top>
				<width>1090</width>
				<height>630</height>
				<texture border="15">ContentPanel.png</texture>
			</control>
			<control type="image">
				<left>5</left>
				<top>625</top>
				<width>1090</width>
				<height>64</height>
				<texture border="15">ContentPanelMirror.png</texture>
			</control>
			<control type="button">
				<description>Close Window button</description>
				<left>980</left>
				<top>11</top>
				<width>64</width>
				<height>32</height>
				<label>-</label>
				<font>-</font>
				<onclick>PreviousMenu</onclick>
				<texturefocus>DialogCloseButton-focus.png</texturefocus>
				<texturenofocus>DialogCloseButton.png</texturenofocus>
				<onleft>1</onleft>
				<onright>1</onright>
				<onup>1</onup>
				<ondown>1</ondown>
				<visible>system.getbool(input.enablemouse)</visible>
			</control>
			<control type="image">
				<description>LOGO</description>
				<left>30</left>
				<top>15</top>
				<width>220</width>
				<height>80</height>
				<aspectratio>keep</aspectratio>
				<texture>Confluence_Logo.png</texture>
			</control>
			<control type="list" id="9000">
				<left>10</left>
				<top>82</top>
				<width>260</width>
				<height>541</height>
				<onleft>9000</onleft>
				<onright>9001</onright>
				<onup>9000</onup>
				<ondown>9000</ondown>
				<pagecontrol>-</pagecontrol>
				<scrolltime>300</scrolltime>
				<itemlayout height="54" width="260">
					<control type="image">
						<left>0</left>
						<top>0</top>
						<width>260</width>
						<height>55</height>
						<texture border="5">MenuItemNF.png</texture>
					</control>
					<control type="label">
						<left>250</left>
						<top>0</top>
						<width>380</width>
						<height>55</height>
						<font>font24_title</font>
						<textcolor>grey3</textcolor>
						<align>right</align>
						<aligny>center</aligny>
						<label>$INFO[ListItem.Label]</label>
					</control>
				</itemlayout>
				<focusedlayout height="54" width="260">
					<control type="image">
						<left>0</left>
						<top>0</top>
						<width>260</width>
						<height>55</height>
						<texture border="5">MenuItemFO.png</texture>
					</control>
					<control type="label">
						<left>250</left>
						<top>0</top>
						<width>380</width>
						<height>55</height>
						<font>font24_title</font>
						<textcolor>white</textcolor>
						<align>right</align>
						<aligny>center</aligny>
						<label>$INFO[ListItem.Label]</label>
					</control>
				</focusedlayout>
				<content>
					<item id="1">
						<label>480</label>
						<label2>31400</label2>
						<onclick>ActivateWindow(AppearanceSettings)</onclick>
						<icon>-</icon>
					</item>
					<item id="2">
						<label>157</label>
						<label2>31401</label2>
						<onclick>ActivateWindow(VideosSettings)</onclick>
						<icon>-</icon>
					</item>
					<item id="3">
						<label>31502</label>
						<label2>31409</label2>
						<onclick>ActivateWindow(PVRSettings)</onclick>
						<icon>special://skin/backgrounds/tv.jpg</icon>
					</item>
					<item id="4">
						<label>2</label>
						<label2>31402</label2>
						<onclick>ActivateWindow(MusicSettings)</onclick>
						<icon>-</icon>
					</item>
					<item id="5">
						<label>1</label>
						<label2>31403</label2>
						<onclick>ActivateWindow(PicturesSettings)</onclick>
						<icon>-</icon>
					</item>
					<item id="6">
						<label>8</label>
						<label2>31404</label2>
						<onclick>ActivateWindow(WeatherSettings)</onclick>
						<icon>-</icon>
					</item>
					<item id="7">
						<label>24001</label>
						<label2>31408</label2>
						<onclick>ActivateWindow(AddonBrowser)</onclick>
						<icon>-</icon>
					</item>
					<item id="8">
						<label>14036</label>
						<label2>31410</label2>
						<onclick>ActivateWindow(ServiceSettings)</onclick>
						<icon>-</icon>
					</item>
					<item id="9">
						<label>13000</label>
						<label2>31406</label2>
						<onclick>ActivateWindow(SystemSettings)</onclick>
						<icon>-</icon>
					</item>
				</content>
			</control>
			<control type="image">
				<left>268</left>
				<top>10</top>
				<width>804</width>
				<height>50</height>
				<texture border="5">black-back2.png</texture>
			</control>
			<control type="image">
				<left>268</left>
				<top>10</top>
				<width>804</width>
				<height>70</height>
				<aspectratio>stretch</aspectratio>
				<texture>GlassTitleBar.png</texture>
			</control>
			<control type="label">
				<description>header label</description>
				<left>300</left>
				<top>20</top>
				<width>740</width>
				<height>30</height>
				<font>font16</font>
				<label>$LOCALIZE[31000] $LOCALIZE[5]</label>
				<align>left</align>
				<aligny>center</aligny>
				<textcolor>white</textcolor>
				<shadowcolor>black</shadowcolor>
			</control>
			<control type="image">
				<left>270</left>
				<top>60</top>
				<width>800</width>
				<height>450</height>
				<texture border="5">button-nofocus.png</texture>
			</control>
			<control type="image">
				<left>272</left>
				<top>62</top>
				<width>796</width>
				<height>446</height>
				<aspectratio>stretch</aspectratio>
				<fadetime>600</fadetime>
				<texture background="true">special://skin/backgrounds/settings.jpg</texture>
			</control>
			<control type="image">
				<left>272</left>
				<top>62</top>
				<width>600</width>
				<height>340</height>
				<aspectratio>stretch</aspectratio>
				<texture>GlassOverlay.png</texture>
				<colordiffuse>AAFFFFFF</colordiffuse>
			</control>
			<control type="image">
				<left>268</left>
				<top>510</top>
				<width>804</width>
				<height>118</height>
				<texture border="5">black-back2.png</texture>
			</control>
			<control type="textbox">
				<description>Appearance Description</description>
				<left>300</left>
				<top>520</top>
				<width>740</width>
				<height>100</height>
				<font>font12</font>
				<label>$INFO[Container(9000).ListItem.Label2]</label>
				<align>left</align>
				<textcolor>white</textcolor>
				<shadowcolor>black</shadowcolor>
			</control>
		</control>
		<include>CommonNowPlaying</include>
		<include>MainWindowMouseButtons</include>
		<include>BehindDialogFadeOut</include>
		<control type="image">
			<description>Section header image</description>
			<left>20</left>
			<top>3</top>
			<width>35</width>
			<height>35</height>
			<aspectratio>keep</aspectratio>
			<texture>icon_system.png</texture>
		</control>
		<control type="grouplist">
			<left>65</left>
			<top>5</top>
			<width>1000</width>
			<height>30</height>
			<orientation>horizontal</orientation>
			<align>left</align>
			<itemgap>5</itemgap>
			<control type="label">
				<include>WindowTitleCommons</include>
				<label>$LOCALIZE[5]</label>
			</control>
		</control>
		<include>Clock</include>
	</controls>
</window>
```










