# Web 设计与开发终极资源大全 - 深之JohnChen的专栏 - CSDN博客

2012年06月23日 11:04:57[byxdaz](https://me.csdn.net/byxdaz)阅读数：2886标签：[web																[javascript																[ajax																[calendar																[php																[flv](https://so.csdn.net/so/search/s.do?q=flv&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)](https://so.csdn.net/so/search/s.do?q=calendar&t=blog)](https://so.csdn.net/so/search/s.do?q=ajax&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)](https://so.csdn.net/so/search/s.do?q=web&t=blog)
个人分类：[WEB编程																[web前端](https://blog.csdn.net/byxdaz/article/category/5719017)](https://blog.csdn.net/byxdaz/article/category/479812)


Web 技术突飞猛进，Web 设计与开发者们可以选择的工具越来越多，Web 开发者的技巧不再只限于 HTML 和 服务器端编程，还需要精通各种第三方资源，这些第三方资源有时候比你的项目更复杂，更专业，你无法自己实现一切，借助一些 Web API，你可以很方便地将大量优秀的第三方资源集成到自己的站点。本文全面搜集 Web 开发中可能用到的各种第三方资源。

**1. 函数与类库**

**A. CAPTCHA**

CAPTCHA 用来防止恶意表单发布，以下 CAPTCHA 系统可以轻松集成到你的程序中：

[reCAPTCHA](http://recaptcha.net/)

这是目前最流行的 CAPTCHA 机制，该机制同时是一个古籍数字化工程的一部分，用户在验证的同时，也帮助辨认一些不够清晰的估计扫描。reCAPTCHA 还有一个 [Perl 模块](http://search.cpan.org/dist/Captcha-reCAPTCHA/lib/Captcha/reCAPTCHA.pm) 实现该功能。

![clip_image001](http://pic003.cnblogs.com/2010/itwriter/20100318125919969.gif)

[Securimage](http://www.phpcaptcha.org/)

这个一个免费的，开源 PHP CAPTCHA 脚本。

![clip_image002](http://pic003.cnblogs.com/2010/itwriter/20100318125919663.gif)

[freeCap](http://www.puremango.co.uk/2005/04/php_captcha_script_113/)

基于 GPL 协议的 CAPTCHA 脚本

[HN CAPTCHA](http://www.phpclasses.org/browse/package/1569.html)

PHP CAPTCHA 脚本，基于 LGPL 协议

**B. 日期处理**

日期操作并不轻松，尽管 PHP 和 Perl 内置了大量此类函数，但未必满足你的需要，以下是几个很好用的日期函数：
- [PEAR Date](http://pear.php.net/package/Date)
通用 PHP 日期类库
- [Date Class](http://www.phpclasses.org/browse/package/1649.html)
PHP 类库，计算及其增减与日期差异
- [Date manipulation in PHP](http://www.phpbuilder.com/columns/akent20000610.php3)
PHPBuilder.com 上的一个教程，讲解如何进行日期处理
- [Date::Calc Perl Module](http://search.cpan.org/dist/Date-Calc/lib/Date/Calc.pod)
Perl模块，用于格列高利历法的日期计算

**C. 图形处理**

图片缩放，添加水印等：
- [Image Manipulation Class](http://www.phpclasses.org/browse/package/3294.html)
在 PHP 中缩放，反转，旋转图片
- [PHP Thumbnailer](http://phpthumb.gxdlabs.com/)
一个轻量级图片缩略图工具
- [PHP GD](http://uk.php.net/manual/en/book.image.php)
一个强大的图片处理类库
- [Perl GD](http://search.cpan.org/dist/GD/)
Perl 图片处理模块

**D. 表单验证**

表单验证不仅保证用户填写的准确，还可以防止攻击：
- [validaForms](http://www.phpclasses.org/browse/package/1698.html)
一个 PHP 表单验证类库
- [User Input Validation Class](http://www.phpclasses.org/browse/package/3632.html)
一个 PHP 表单验证基础库
- [Validate Form Fields Script](http://www.finalwebsites.com/snippets.php?id=15)
另一个 PHP 表单验证基础库
- [WWW::FieldValidator](http://search.cpan.org/~shlomif/WWW-Form-1.18/lib/WWW/FieldValidator.pm)
一个简单的 Perl 表单验证模块

**E. 密码验证**

验证密码的复杂度
- [Strength Test](http://rumkin.com/tools/password/passchk.php)
一个 PHP 密码强度验证函数

![clip_image003](http://pic003.cnblogs.com/2010/itwriter/20100318125920823.gif)
- [Password Checker](http://www.phpclasses.org/browse/package/2966.html)
一个 PHP 类，用来验证密码强度
- [Data::Password::Check](http://search.cpan.org/~chisel/Data-Password-Check-0.08/lib/Data/Password/Check.pm)
Perl 模块，用来验证密码强度

**2. Ajax 与 JavaScript**

**A. 自动输入建议**

最早最成熟的自动输入建议应该是 Google 搜索条。
- [Facebook-Style Autosuggest](http://devthought.com/blog/projects-news/2008/01/textboxlist-meets-autocompletion/)
Facebook 风格的自动输入建议

![clip_image004](http://pic003.cnblogs.com/2010/itwriter/20100318125920875.gif)
- [Autosuggest / Autocomplete with Ajax](http://www.brandspankingnew.net/specials/ajax_autosuggest/ajax_autosuggest_autocomplete.html)
基于 XML 和 JSON 数据源的自动输入建议工具

![clip_image005](http://pic003.cnblogs.com/2010/itwriter/20100318125920374.gif)
- [Ajax.Autocompleter](http://wiki.github.com/madrobby/scriptaculous/ajax-autocompleter)
基于 script.aculo.us 的输入自动完成工具
- [Adobe Labs Spry Auto Suggest](http://labs.adobe.com/technologies/spry/articles/data_api/apis/autosuggest.html)
由 Adobe 实验室提供的自动输入建议工具
- [Google Suggest Style Filter](http://mattberseth.com/blog/2007/12/creating_a_google_suggest_styl.html)
Google Suggest 风格的自动输入建议

**B. 日历**

在输入日期的地方，使用非常直观的日历面板
- [Vista-Like Ajax Calendar](http://dev.base86.com/scripts/vista-like_ajax_calendar_version_2.html)
基于 Mootools，Vista 风格

![clip_image006](http://pic003.cnblogs.com/2010/itwriter/20100318125920231.gif)
- [JS Calendar in DHTML Suite](http://www.dhtmlgoodies.com/index.html?page=calendarScripts)
基于 JavaScript ，支持多日历连接

![clip_image007](http://pic003.cnblogs.com/2010/itwriter/20100318125920252.gif)
- [AJAX Calendar with PHP and MySQL](http://www.opencrypt.com/blog.php?a=29)
一个开源的 AJAX 日历控件
- [Super AJAX Calendar](http://www.bmgadg.com/super_ajax_calendar.php)
一个基于 PHP 和 MySQL 的日历控件
- [dhtmlxScheduler](http://www.dhtmlx.com/docs/products/dhtmlxScheduler/)
一个强大的事务日历

**C. 滑动条**

滑动条可以更直观地进行数值的输入和选择
- [Accessible Slider](http://www.filamentgroup.com/lab/developing_an_accessible_slider/)
一个设计出众的滑动条控件

![clip_image008](http://pic003.cnblogs.com/2010/itwriter/20100318125921391.gif)
- [Phatfusion Slider](http://www.phatfusion.net/slider/)
一个基于 AJAX 的滑动条

![clip_image009](http://pic003.cnblogs.com/2010/itwriter/20100318125921858.gif)
- [AJAXEngine Sliders](http://www.mathertel.de/AJAXEngine/S03_AJAXControls/ConnectionsTestPage.aspx)
AJAXEngine 项目中的滑动条
- [Slider Using PHP, Ajax And Javascript](http://roshanbh.com.np/2008/01/slider-using-php-ajax-and-javascript.html)
一个简单实用的 AJAX 滑动条
- [WebFX Slider](http://webfx.eae.net/dhtml/slider/slider.html)
基于 Apache 软件许可 2.0 的滑动条控件

**D. 表格**

Ajax 表格，支持排序等功能
- [Grid3](http://www.extjs.com/playpen/ext-2.0/examples/grid/grid3.html)
作为 Ext JS 库一部分的表格控件

![clip_image010](http://pic003.cnblogs.com/2010/itwriter/20100318125921356.gif)
- [dhtmlxGrid](http://dhtmlx.com/docs/products/dhtmlxGrid/)
支持列排序和搜索

![clip_image011](http://pic003.cnblogs.com/2010/itwriter/20100318125921214.gif)
- [Unobtrusive Table Sort Script](http://www.frequency-decoder.com/2006/09/16/unobtrusive-table-sort-script-revisited)
支持多列排序
- [AJAX Data Listings Engine](http://www.opencrypt.com/blog.php?a=22)
基于 PHP 和 MySQL 
- [TableKit](http://www.millstream.com.au/upload/code/tablekit/)
基于 Prototype JavaScript 框架

**E. 可拖动内容**

Ajax-based draggable content features can be very useful when used appropriately. Such tools enable users to reorganize a page to suit their needs, and can also serve as a basis for an interactive feature like a shopping basket.
- [Draggable Content Scripts](http://www.dhtmlgoodies.com/index.html?page=dragDrop)
dhtmlgoodies.com 推出多种可拖放内容脚本，以及图片裁剪工具

![clip_image012](http://pic003.cnblogs.com/2010/itwriter/20100318125921907.gif)
- [DragResize](http://www.twinhelix.com/javascript/dragresize/)
支持尺寸修改功能

![clip_image013](http://pic003.cnblogs.com/2010/itwriter/20100318125921210.gif)
- [Drag & Drop Sortable Lists](http://tool-man.org/examples/sorting.html)
位置可调列表控件
- [Scriptaculous Draggable](http://wiki.github.com/madrobby/scriptaculous/draggable)
script.aculo.us 框架的一部分

**F. 图片放大**

类似 JavaScript 灯箱的图片放大显示控件
- [Highslide JS](http://highslide.com/)
在 Modal 窗口显示放大尺寸的图片

![clip_image014](http://pic003.cnblogs.com/2010/itwriter/20100318125922587.gif)
- [FancyZoom](http://cabel.name/2008/02/fancyzoom-10.html)
图片缩放显示

![clip_image015](http://pic003.cnblogs.com/2010/itwriter/20100318125922639.gif)
- [TJPzoom 3](http://valid.tjp.hu/tjpzoom/)
图片放大工具
- [Ajax Script To Scale Images](http://www.xul.fr/ajax/ajax-image-gallery.html)
显示缩略图的放大尺寸版

**G. 相册和幻灯**

用相册或幻灯方式显示系列图片
- [minishowcase](http://minishowcase.net/)
基于 AJAX 和 JSON 的相册控件

![clip_image016](http://pic003.cnblogs.com/2010/itwriter/20100318125922332.gif)
- [Animated JavaScript Slideshow](http://www.leigeber.com/2008/12/javascript-slideshow/)
轻量级 JavaScript 图片幻灯效果，支持运动字幕

![clip_image017](http://pic003.cnblogs.com/2010/itwriter/20100318125922744.gif)
- [Hoverbox Image Gallery](http://sonspring.com/journal/hoverbox-image-gallery)
简单的相册，鼠标在缩略图上移动时显示原图
- [TripTracker](http://slideshow.triptracker.net/)
以幻灯方式显示图片
- [Agile Gallery (Ajax Version)](http://www.agilegallery.com/ajax-photo-gallery.html)
AJAX 相册工具

**H. 打分工具**

非常直观的打分控件
- [Starbox](http://www.nickstakenburg.com/projects/starbox/)
基于 Prototype JavaScript 框架

![clip_image018](http://pic003.cnblogs.com/2010/itwriter/20100318125923601.gif)
- [Unobtrusive AJAX Star Rating Bar](http://masugadesign.com/the-lab/scripts/unobtrusive-ajax-star-rating-bar/)
基于 PHP 和 AJAX

![clip_image019](http://pic003.cnblogs.com/2010/itwriter/2010031812592399.gif)
- [CSS: Star Rater Ajax Version](http://www.yvoschaap.com/index.php/weblog/css_star_rater_ajax_version/)
基于 AJAX

**I.取色板**

用于取色
- [jscolor](http://jscolor.com/)
简单的弹出式取色板

![clip_image020](http://pic003.cnblogs.com/2010/itwriter/20100318125923597.gif)
- [JavaScript Color Picker](http://www.nogray.com/color_picker.php)
支持 RGB，HSL，以及16进制图片色值

![clip_image021](http://pic003.cnblogs.com/2010/itwriter/20100318125923455.gif)
- [Tigra Color Picker](http://www.softcomplex.com/products/tigra_color_picker/demo1.html)
非常简单的取色板
- [Photoshop-like JavaScript Color Picker](http://johndyer.name/post/2007/09/26/PhotoShop-like-JavaScript-Color-Picker.aspx)
支持色调和饱和度选项
- [ColorPicker – jQuery Plugin](http://www.eyecon.ro/colorpicker/)
基于 jQuery
- [DHTML Color Picker](http://nofunc.org/DHTML_Color_Picker/)
基于 DHTML

**J. 进度条**
- [jsProgressBarHandler](http://www.bram.us/projects/js_bramus/jsprogressbarhandler/)
非常灵活

![clip_image022](http://pic003.cnblogs.com/2010/itwriter/20100318125923116.gif)
- [YUI Loading Panel Widget](http://thecodecentral.com/2008/01/17/a-yui-loading-panel-widget)
非常漂亮.

![clip_image023](http://pic003.cnblogs.com/2010/itwriter/20100318125923810.gif)
- [Simple Javascript Progress Bar with CSS](http://www.webappers.com/2007/07/31/webappers-simple-ajax-progress-bar-with-css/)
简单实用.
- [Uploadify](http://www.uploadify.com/)
基于 jQuery.

**3. APIs**

以下 API 可以将很多现成的功能集成到你的站点：
- [Alexa Top Sites](http://aws.amazon.com/alexatopsites/)
这个 API 按 Alexa 排名返回网站列表
- [bit.ly API](http://code.google.com/p/bitly-api/wiki/ApiDocumentation)
bit.ly URL 缩短 API
- [Blinksale API](http://www.blinksale.com/api)
访问 Blinksale 数据。
- [FoXRate](http://foxrate.org/)
货币汇率转换 API
- [eBay API](http://developer.ebay.com/common/api/)
eBay 的 API，可以向 eBay 提交货品。
- [Twitter API](http://apiwiki.twitter.com/)
Twitter API，用来访问 Twitter 数据，包括用户状态和信息。

**4. IP 定位**

用户来自什么地方。
- [OpenCrypt IP Location API](http://www.opencrypt.com/ip-location.php)
根据 IP 探测用户来自哪个国家

![clip_image001](http://pic003.cnblogs.com/2010/itwriter/2010031814024034.gif)
- [IP Details](http://www.phpclasses.org/browse/package/5680.html)
一个 PHP 类库，根据 IP 地址获取用户地理位置
- [GEO-IP](http://software77.net/geo-ip/)
IP 到国家对应数据库

**5. 图表**
- [amCharts](http://www.amcharts.com/)
基于 Flash，支持 3D 图表

![clip_image002](http://pic003.cnblogs.com/2010/itwriter/20100318140240641.gif)
- [FusionCharts](http://www.fusioncharts.com/)
漂亮的 3D 图表

![clip_image003](http://pic003.cnblogs.com/2010/itwriter/20100318140241442.gif)
- [XML/SWF Charts](http://www.maani.us/xml_charts/index.php?menu=upgrade)
可高度定制的 Flash 图表工具
- [JFreeChart](http://www.jfree.org/jfreechart/)
基于 Java 的免费图表工具
- [pChart](http://pchart.sourceforge.net/index.php)
基于 PHP

**6. 地图**
- [amMap](http://www.ammap.com/)
交互式地图工具，支持下钻式挖掘

![clip_image004](http://pic003.cnblogs.com/2010/itwriter/20100318140241495.gif)
- [Google Charts API](http://code.google.com/apis/chart/)
来自 Google 地图工具，只是支持的地图尺寸有限

![clip_image004[1]](http://pic003.cnblogs.com/2010/itwriter/20100318140241711.gif)
- [Flashmaps](http://www.flashmaps.com/)
多个基于 Flash 的地图工具

**7. 音频播放器**
- [XSPF Web Music Player](http://musicplayer.sourceforge.net/)
基于 Flash 的开源音频播放器

![clip_image005](http://pic003.cnblogs.com/2010/itwriter/20100318140241209.gif)
- [Flash MP3 Player](http://www.flashmp3player.org/)
免费 PHP + Flash MP3 播放器

![clip_image006](http://pic003.cnblogs.com/2010/itwriter/20100318140242760.gif)
- [TSPlayer](http://components.developers4web.com/mp3-flash-player)
基于 Flash，支持换皮肤
- [E-Phonic MP3 Player](http://www.e-phonic.com/mp3player/)
轻量级 MP3 播放器，支持换肤，支持实时视效

**8. 视频播放器**
- [OS FLV](http://www.osflv.com/)
开源 Flash 视频播放器

![clip_image007](http://pic003.cnblogs.com/2010/itwriter/20100318140242279.gif)
- [Flowplayer](http://flowplayer.org/)
很好用，高度可定制。

![clip_image008](http://pic003.cnblogs.com/2010/itwriter/20100318140242526.gif)
- [JW FLV Media Player](http://www.longtailvideo.com/players/jw-flv-player/)
强大的 Flash 视频播放器
- [FLV Player](http://flvplayer.com/)
漂亮的 FLV 播放器

**9. 视频转换**
- [OpenCrypt Video Conversion API](http://www.opencrypt.com/video-conversion.php)
强大的视频转换 API，可生成 FLV 视频
- [Hey!Watch](http://heywatch.com/page/home)
一个在线视频编码平台
- [Online FLV Converter](http://vixy.net/)
在线 FLV 视频转换
- [FlashVideo Module API](http://www.travistidwell.com/flashvideo_api)
面向 Drupal 社区的 Flash 视频编码 API

**10. 所见即所得编辑器**
- [TinyMCE](http://tinymce.moxiecode.com/)
一个轻量的，基于 JavaScript 的所见即所得编辑器

![clip_image009](http://pic003.cnblogs.com/2010/itwriter/20100318140242742.gif)
- [XINHA](http://trac.xinha.org/)
功能完整的开源 WYSIWYG 编辑器

![clip_image010](http://pic003.cnblogs.com/2010/itwriter/20100318140243436.gif)
- [NicEdit](http://nicedit.com/)
一个简单的轻量级 WYSIWYG 编辑器
- [openWYSIWYG](http://www.openwebware.com/)
开源，跨浏览器 WYSIWYG 编辑器
- [CKEditor](http://ckeditor.com/)
设计出众，功能丰富的 WYSIWYG 编辑器

本文国际来源：[100 Essential Web Development Tools](http://www.noupe.com/tools/100-essential-web-development-tools.html)


