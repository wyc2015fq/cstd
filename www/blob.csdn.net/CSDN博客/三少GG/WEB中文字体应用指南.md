# WEB中文字体应用指南 - 三少GG - CSDN博客
2014年04月14日 22:56:20[三少GG](https://me.csdn.net/scut1135)阅读数：991

http://www.fontsquirrel.com/fontface
**@font-face**是[CSS3](http://www.w3.org/TR/CSS/#css3)中的一个模块，他主要是把自己定义的Web字体嵌入到你的网页中，随着**[@font-face](http://www.w3.org/TR/css3-fonts/)**模块的出现，我们在Web的开发中使用字体不怕只能使用Web安全字体，你们当中或许有许多人会不自然的问，这样的东西IE能支持吗？当我告诉大家**[@font-face](http://www.w3.org/TR/css3-fonts/)**这个功能早在IE4就支持了你肯定会感到惊讶。我的Blog就使用了许多这样的自定义Web字体，比如说首页的Logo，Tags以及页面中的手写英文体，很多朋友问我如何使用，能让自己的页面也支持这样的自定义字体，一句话这些都是**[@font-face](http://www.w3.org/TR/css3-fonts/)**实现的，为了能让更多的朋友知道如何使用他，今天我主要把自己的一点学习过程贴上来和大家分享。
首先我们一起来看看@font-face的语法规则：
```
@font-face {
      font-family: <YourWebFontName>;
      src: <source> [<format>][,<source> [<format>]]*;
      [font-weight: <weight>];
      [font-style: <style>];
    }
```
**取值说明**
- YourWebFontName:此值指的就是你自定义的字体名称，最好是使用你下载的默认字体，他将被引用到你的Web元素中的font-family。如“font-family:"YourWebFontName";”
- source:此值指的是你自定义的字体的存放路径，可以是相对路径也可以是绝路径；
- format：此值指的是你自定义的字体的格式，主要用来帮助浏览器识别，其值主要有以下几种类型：truetype,opentype,truetype-aat,embedded-opentype,avg等；
- weight和style:这两个值大家一定很熟悉，weight定义字体是否为粗体，style主要定义字体样式，如斜体。
**兼容浏览器**
![](http://images.cnblogs.com/cnblogs_com/rubylouvre/199042/o_font-face-browers.jpg)
说到浏览器对@font-face的兼容问题，这里涉及到一个字体format的问题，因为不同的浏览器对字体格式支持是不一致的，这样大家有必要了解一下，各种版本的浏览器支持什么样的字体，前面也简单带到了有关字体的几种格式，下面我就分别说一下这个问题，让大家心里有一个概念：
**一、TureTpe(.ttf)格式：**
.ttf字体是Windows和Mac的最常见的字体，是一种RAW格式，因此他不为网站优化,支持这种字体的浏览器有【IE9+,Firefox3.5+,Chrome4+,Safari3+,Opera10+,iOS Mobile Safari4.2+】；
**二、OpenType(.otf)格式：**
.otf字体被认为是一种原始的字体格式，其内置在TureType的基础上，所以也提供了更多的功能,支持这种字体的浏览器有【Firefox3.5+,Chrome4.0+,Safari3.1+,Opera10.0+,iOS Mobile Safari4.2+】；
**三、Web Open Font Format(.woff)格式：**
.woff字体是Web字体中最佳格式，他是一个开放的TrueType/OpenType的压缩版本，同时也支持元数据包的分离,支持这种字体的浏览器有【IE9+,Firefox3.5+,Chrome6+,Safari3.6+,Opera11.1+】；
**四、Embedded Open Type(.eot)格式：**
.eot字体是IE专用字体，可以从TrueType创建此格式字体,支持这种字体的浏览器有【IE4+】；
**五、SVG(.svg)格式：**
.svg字体是基于SVG字体渲染的一种格式,支持这种字体的浏览器有【Chrome4+,Safari3.1+,Opera10.0+,iOS Mobile Safari3.2+】。
这就意味着在@font-face中我们至少需要.woff,.eot两种格式字体，甚至还需要.svg等字体达到更多种浏览版本的支持。
为了使@font-face达到更多的浏览器支持，[Paul Irish](http://paulirish.com/)写了一个独特的@font-face语法叫[Bulletproof
 @font-face](http://paulirish.com/2009/bulletproof-font-face-implementation-syntax/):
   @font-face {
	font-family: 'YourWebFontName';
	src: url('YourWebFontName.eot?') format('eot');/*IE*/
	src:url('YourWebFontName.woff') format('woff'), url('YourWebFontName.ttf') format('truetype');/*non-IE*/
   }
但为了让各多的浏览器支持，你也可以写成：
   @font-face {
	font-family: 'YourWebFontName';
	src: url('YourWebFontName.eot'); /* IE9 Compat Modes */
	src: url('YourWebFontName.eot?#iefix') format('embedded-opentype'), /* IE6-IE8 */
             url('YourWebFontName.woff') format('woff'), /* Modern Browsers */
             url('YourWebFontName.ttf')  format('truetype'), /* Safari, Android, iOS */
             url('YourWebFontName.svg#YourWebFontName') format('svg'); /* Legacy iOS */
   }
说了这么多空洞的理论知识，大家一定有点心痒痒了，那么我们先来看看W3CPLUS首页中导航部分的兰色字体是如何实现的，假如我们有一个这样的DOM标签，需要应用自定义字体：
**HTML Code:**
```
<h2 class="neuesDemo">Neues Bauen Demo</h2>
```
通过@font-face来定义自己的Web Font:
```
@font-face {
    font-family: 'NeuesBauenDemo';
    src: url('../fonts/neues_bauen_demo-webfont.eot');
    src: url('../fonts/neues_bauen_demo-webfont.eot?#iefix') format('embedded-opentype'),
     url('../fonts/neues_bauen_demo-webfont.woff') format('woff'),
     url('../fonts/neues_bauen_demo-webfont.ttf') format('truetype'),
     url('../fonts/neues_bauen_demo-webfont.svg#NeuesBauenDemo') format('svg');
    font-weight: normal;
    font-style: normal;
  }
```
我在这里采用的是相对路径，当然大家也可以使用绝路径。到这里我们就需要把定义好的字体应用到我们实际页面中去：
```
h2.neuesDemo {
      font-family: 'NeuesBauenDemo'
   }
```
**效果：**
![](http://images.cnblogs.com/cnblogs_com/rubylouvre/199042/o_neues-bauen-demo.jpg)
看到上面的效果，我想大家会感到@font-face很神奇，同时也想争着做做看，可是一动手才发现，特殊字体我要怎样才能得到，那些.eot,.woff,.ttf,.svg这些字体格式又怎么获取呢？有些朋友可能就不知道如何运手了，那么我们就带着这些问题来一个全程完成的实例吧：
**一、获取特殊字体：**
我们拿下面这种single Malta字体来说吧：
![](http://images.cnblogs.com/cnblogs_com/rubylouvre/199042/o_font-face-single-malta-demo.jpg)
要得到single Malta字体，不外乎两种途径，其一找到付费网站购买字体，其二就是到免费网站DownLoad字体。当然要给钱的这种傻事我想大家都不会做的，那我们就得到免费的地方下载，在哪有呢？我平时都是到[Google Web Fonts](http://www.google.com/webfonts)和[Dafont.com](http://www.dafont.com/)寻找自己需要的字体，当然网上也还有别的下载字体的地方，这个Demo使用的是[Dafont.com](http://www.dafont.com/)的Single
 Malta字体，这样就可以到这里下载[Single Malta](http://www.dafont.com/single-malta.font)：
![](http://images.cnblogs.com/cnblogs_com/rubylouvre/199042/o_single-malta-font.jpg)
Single Malta下载下来后，需要把它解压缩出来：
![](http://images.cnblogs.com/cnblogs_com/rubylouvre/199042/o_single-malta-font-out.jpg)
**二、获取@font-face所需字体格式：**
特殊字体已经在你的电脑中了，现在我们需要想办法获得@font-face所需的.eot,.woff,.ttf,.svg字体格式。要获取这些字体格式，我们同样是需要第三方工具或者软件来实现，下面我给大家推荐一款我常用的一个工具[fontsquirrel](http://www.fontsquirrel.com/fontface/generator),别的先不多说，首跟我点[这里](http://www.fontsquirrel.com/fontface/generator)进入到下面这个界面吧。
![](http://images.cnblogs.com/cnblogs_com/rubylouvre/199042/o_font-face-kit-generator.png)
如果你进入页面没有看到上图，你可以直接点击导航:
![](http://images.cnblogs.com/cnblogs_com/rubylouvre/199042/o_fontsquirrel-menu.jpg)
如果你看到了上面的界面，那就好办了，我们来看如何应用这个工具生成@font-face需要的各种字体，先把我们刚才下载的字体上传上去：
![](http://images.cnblogs.com/cnblogs_com/rubylouvre/199042/o_add-fonts.jpg)
上传后按下图所示操作：
![](http://images.cnblogs.com/cnblogs_com/rubylouvre/199042/o_font-download.png%22)
现在从Font Squirrel下载下来的文件已经保存在你本地的电脑上了，接着只要对他进行解压缩，你就能看到文件列表如下所示：
![](http://images.cnblogs.com/cnblogs_com/rubylouvre/199042/o_font-format.jpg)
大家可以看到，解压缩出来的文件格式，里面除了@font-face所需要的字体格式外，还带有一个DEMO文件，如果你不清楚的也可以参考下载下来的DEMO文件，我在这里不对DEMO说明问题，我主要是给大家介绍如何把下载下来的文件有价值的运用到我们的项目中。
例如在自己的本地创建了一个fontface项目：
![](http://images.cnblogs.com/cnblogs_com/rubylouvre/199042/o_font-face-project.jpg)
为了让项目结构更清晰，我们在项目中单独创建一个fonts目录，用来放置解压缩出来@font-face所需的字体格式：
![](http://images.cnblogs.com/cnblogs_com/rubylouvre/199042/o_font-face-project-demo.jpg)
现在@font-face所需字体已经加载到本地项目，现在本地项目中的style.css中附上我们需要的@font-face样式
```
@font-face {
      font-family: 'SingleMaltaRegular';
      src: url('../fonts/singlemalta-webfont.eot');
      src: url('../fonts/singlemalta-webfont.eot?#iefix') format('embedded-opentype'),
           url('../fonts/singlemalta-webfont.woff') format('woff'),
       url('../fonts/singlemalta-webfont.ttf') format('truetype'),
       url('../fonts/singlemalta-webfont.svg#SingleMaltaRegular') format('svg');
      font-weight: normal;
      font-style: normal;
   }
```
到这里为止，我们已经通过@font-face自定义好所需的SingleMalta字体，离最后效果只差一步了，就是把自己定义的字体应用到你的Web中的DOM元素上：
```
h2.singleMalta {
     font-family: 'SingleMaltaRegular'
   }
```
**效果：**
![](http://images.cnblogs.com/cnblogs_com/rubylouvre/199042/o_single-malta-font-demo.jpg)
看到上面的效果，那大家就知道我们实现成功了。那么关于@font-face帮你打造特殊效果的字体，到这里基本上就完成了，我在这里需要提醒使用者：
1、如果你的项目中是英文网站，而且项目中的Logo，Tags等应用到较多的这种特殊字体效果，我建议你不要使用图片效果，而使用@font-face，但是你是中文网站，我觉得还是使用图片比较合适，因为加载英文字体和图片没有多大区别，但是你加载中文字体，那就不一样了，因为中文字体太大了，这样会影响到项目的某些性能的优化；
2、致命的错误，你在@font-face中定义时，文件路径没有载对；
3、你只定义了@font-face，但并没有应用到你的项目中的DOM元素上；
以上几点都是在平时制作中常出现的问题，希望大家能小意一些，另外我们没有办法在购买所有字体，就算你实力雄厚，那也没有办法在一台服务器主机上放置你所有项目需要的字体。因此我给大家提供几个免费字体下载的网址：[Webfonts](http://webfonts.fonts.com/),[Typekit](http://typekit.com/),[Kernest](http://kernest.com/),[Google
 Web Fonts](http://www.google.com/webfonts),[Kernest](http://kernest.com/licenses),[Dafont](http://www.dafont.com/),[Niec
 Web Type](http://nicewebtype.com/fonts/),不然你点[这里](http://www.google.com/search?q=webfonts)将有更多的免费字体。前面几个链接是帮助你获取一些优美的怪异的特殊字体，但下面这个工具作用更是无穷的大，他能帮你生成@font-face所需要的各种字体，这工具就是[Font
 Squirrel](http://www.fontsquirrel.com/fontface/generator)。
**最后在提醒一下，使用@font-face别的可以忘了，但[Font Squirrel](http://www.fontsquirrel.com/fontface/generator)千万不能忘，因为他能帮你生成@font-face所需的各种字体格式。**
到此关于@font-face就介绍完了，不知道大家喜欢不喜欢，如果喜欢的话赶快动手实践一下，有Blog的可以马上运用上去，也可以炫一下。
[](http://blog.csdn.net/xiaolongtotop/article/details/8316554)
# web字体格式及几种在线格式转换工具介绍
[@余五四](http://www.weibo.com/nightire) ：在 Web 上应用字体是一项基本技术，同时也是一门艺术。对于英文字体来说可选择的范围实在是太广泛了，合理的使用它们将会为你的网站增色不少。关于英文字体的使用和搭配技巧，在这里不做赘述，只推荐一套非常好的视频：[Fundamentals
 of Design *by CodeSchool*](http://www.codeschool.com/courses/fundamentals-of-design)
> 
**相关好文：**
[新手入门：选择字体的五个原则](http://www.uisdc.com/select-the-font-five-principles)
[浅析中英文基本字形的演变和发展](http://www.uisdc.com/163-uedc-1)
[2012年度最佳移动网页字体设计](http://www.uisdc.com/mobile-font-design-2012)
[推荐！跨平台字体效果浅析](http://www.uisdc.com/web-font-style)
而真正的挑战在于中文字体，**由于中文字体组成的特殊性导致其体积过于庞大，除了操作系统内置的字体之外，我们很难在网站上应用其他的字体。**在可选性很差的前提之下，如何正确的使用中文字体呢？
首先，以下的字体声明都是很糟糕的，切忌使用：
> 
Font-family: “宋体”
Font-family: “宋体”，Arial；
Font-family: Arial，”宋体”，”微软雅黑”；
Font-family: Helvetica，Arial，”华文细黑”，”微软雅黑”；
接下来，我们一步一步来说明如何定义好的字体声明。
#### 中文字体也有英文名称
很多开发者忽略了这一点：尽管我们在操作系统中常常看到**宋体、微软雅黑、华文细黑**这样的字体名称，但实际上这只是字体的显示名称，而不是字体文件的名称。虽然说在大多数情况下直接使用显示名称也有效，但有些用户却工作在一些很极端的情况下，这会导致你的字体声明无效。
比如说，用户安装了中文版的操作系统（这意味着系统有中文字体），但是却切换到了以英文为主要语言——这种情况在那些希望加强英语锻炼的中文用户当中是很常见的。这时候，操作系统很有可能无法按照显示名称找到正确的字体，所以我们要记住的第一件事情就是： **同时声明中文字体的字体名称（英文）和显示名称（中文）**，就像这样：
> 
Font-family: SimSun, “宋体”
Font-family: “Microsoft YaHei”, “微软雅黑”
Font-family: STXihei, “华文细黑”, “Microsoft YaHei”, “微软雅黑”
#### 永远不要忘记声明英文字体，并且英文字体应该在中文字体之前
记住这个事实：绝大部分中文字体里包含英文字母（但是基本上都很丑），而英文字体里不包含中文字符。
在网页里中/英文混排是很常见的，你绝对不会喜欢用中文字体显示英文的效果，所以一定不要忘了先声明英文字体：
> 
Font-family: Georgia, SimSun, “宋体”
Font-family: Arial, “Microsoft YaHei”, “微软雅黑”
另外还有一个好习惯，就是在最后补充英文字体族的名称。字体族大体上分为两类：非衬线和衬线，它们之间的区别和使用规则请见本文开始介绍的视频。一般来说，你应该这么做：
> 
Font-family: Georgia, SimSun, “宋体”, serif
Font-family: Arial, “Microsoft YaHei”, “微软雅黑”, sans-serif
请注意：以上两句声明中的**宋体**和**微软雅黑**不应该调换（尽管调换了也不会发生错误），这是因为从字体的式样来看，微软雅黑是非衬线的，而宋体才是衬线的。然而中文并不像英文那样严格区分字体族，所以这一点在实际应用当中并不那么重要。
#### 别忘了照顾不同的操作系统
作为一个 Web 开发者，你理应对 Windows, Mac OS, Linux 家族等常用操作系统里的系统字体有足够的了解，特别是中文。在这里，我们假设目标网站要同时给予 windows 用户和 mac 用户最好的字体体验，于是我们可以这样声明：
> 
Font-family: Helvetica, Tahoma, Arial, STXihei, “华文细黑”, “Microsoft YaHei”, “微软雅黑”, sans-serif
这句声明都做到哪些事情呢？让我们一一说明（括号内代表其对应的目标操作系统）：
- 对于英文字符，首先查找Helvetica(Mac)，然后查找Tahoma(Win)，都找不到就用Arial(Mac&Win)；若是以上三者都缺失，则使用当前默认的sans-serif字体(操作系统或浏览器指定)；
- 对于中文字体，我们已经了解其规则了。华文细黑(Mac)，微软雅黑(Win)是这两个平台的默认中文字体。
#### 注意向下兼容
到此为止，我们的字体声明已经很不错了——如果你不必考虑还在使用旧版本操作系统的用户的话。遗憾地是，中文市场还有大量的用户在使用 Windows XP，宋体才是他们的主要中文字体。为了照顾到这些用户，你可以为微软雅黑增加一个 fallback：
> 
font-family: Helvetica, Tahoma, Arial, STXihei, “华文细黑”, Heiti, “黑体”, “Microsoft YaHei”, “微软雅黑”, SimSun, “宋体”, sans-serif
同样地，你看到我们也为 Mac 系统使用了黑体作为 fallback。
#### 其他
#### 不加双引号可以吗？
可以。有些英文字体的名称多于两个单词，因为单词中间有空格所以需要用 `""` 包裹起来。中文字体很特别，按照英文的角度来看，像微软雅黑究竟算是一个词还是四个词呢？没关系，好在中文字体的名称里没有空格，所以 `""` 不加也没什么大碍。
不过，谁都不能保证在任何操作系统/浏览器环境下都是如此，若是发生了奇怪的事情，不妨加上双引号试试看。
#### 可以默认显示某种字体吗？比如微软雅黑
你可能注意到了，在我们最后的字体声明里，华文细黑是默认字体（如果你的系统上安装了声明里所有的中文字体的话），为什么我要先声明 Mac 系统的字体呢？
按理来说，大多数网站的主要目标市场还是 Windows 用户的，所以理论上这个才是合理的声明：
> 
Font-family: Helvetica, Tahoma, Arial, “Microsoft YaHei”, “微软雅黑”, SimSun, “宋体”, STXihei, “华文细黑”, Heiti, “黑体”, sans-serif
但实际上却并非如此。在中文字体的用户群体里，很大一部分拥有 Mac 的人都同时安装了 Win 下常用的中文字体（这得归功于 Office for Mac）；但极少有 Win 用户去安装 Mac 下的中文字体.
因此，把 Mac 用字体声明在前面几乎不会对 Win 用户产生什么影响（因为他们压根没有！），倒是用来做 fallback 的黑体可能会取代微软雅黑的位置，所以更保险的做法或许是这样：
> 
Font-family: Helvetica, Tahoma, Arial, STXihei, “华文细黑”, “Microsoft YaHei”, “微软雅黑”, SimSun, “宋体”, Heiti, “黑体”, sans-serif
但无论如何请不要把微软雅黑放在中文字体的最前面，作为史上最丑陋的中文字体之一，微软雅黑实在不是什么好的选择，请照顾一下被 Mac 宠坏的用户吧，谢谢！（仅代表个人观点）
BTW，如果你也像我一样不喜欢呆头呆脑的微软雅黑，那就干脆把它删了吧~
到此为止，虽然在我们的示例代码里没有包含 Linux 家族的例子，不过相信你也明白该怎么做了吧。
#### 一点补充
鉴于一些人对微软雅黑的排位产生异议，我不妨把上文的解决方案再延伸一步。事实是这样子的：
- 把微软雅黑放前面，会导致安装了微软雅黑字体的 Mac 用户不得不面对微软雅黑，而在 Mac 下比微软雅黑优雅得多的中文字体比比皆是；
- 把 Mac 下的字体放前面，也会对 Windows 用户造成差不多的困惑，毕竟微软雅黑是 Windows 平台下显示效果最好的字体（目前为止）；
1 和 2，哪一种出现的概率更大一些？我想这是一个不需要计算就能知道的答案吧？
但是——的确还有另外两个因素在纠结着：
- 不少 Windows 用户因为各种原因关闭了 ClearType，在此情形下微软雅黑将会惨不忍睹！但是 Mac 的字体也不是好的选择，真正的胜出者？猜对了，宋体。
- 绝大部分 Mac 下的黑体在 Windows 下模糊不清，而微软雅黑虽然丑但在 Mac 下至少能看。（间接体现了两个平台的字体渲染技术的差距）
所以在实践中，真正接近”万无一失”的方案需要考虑以下几点：
- 利用 UA 判断为不同的平台加载不一样的字体声明；
- 除非有特别的原因，否则尽量保持正文用宋体，标题和其他可以放大些的地方用微软雅黑（针对 Windows）；
- Mac 下的冬青体效果极佳，但是该字体在 Mac OS X 10.6 以前是没有的，所以谨慎考虑你的用户群体，或者使用华文黑体系列做 fallback；
最后，我不想再和任何人争论字体的优劣，本文的目的是介绍使用方法而不是字体选择。”美”或”丑”向来都是很主观的事情，只因为我是作者，所以我免不了会有倾向性，然而我也相信你自己会有正确的判断，和我较真没有任何实际意义。
原文地址：[ruby-china.org](http://ruby-china.org/topics/14005)
作者：[@余五四](http://www.weibo.com/nightire)
