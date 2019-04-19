# shtml - 左直拳的马桶_日用桶 - CSDN博客
2013年01月19日 22:30:52[左直拳](https://me.csdn.net/leftfist)阅读数：13548
shtml接近静态页，比html慢，但比asp快。静中带动，静若处仔，动如脱裤。
一、为什么要用shtml
首先是因为它很接近静态页，速度比asp\aspx页面快得多；其次是它可以通过外链公用页面，以实现“动态”的功能。最重要一个原因，我觉得就是，这是网站将动态页面静态化的一个很好的途径。这个途径的步骤如下：
1、用户访问一个shtml页面
2、
1）如果这个shtml页面已经存在，系统返回该页面，十分自然
2）如果这个shtml页面并不存在，系统会报404错误，找不到该页面；这时IIS里自定义404错误会转向一个动态页，比如pagenotfound.aspx。在这个动态页里，组织相关内容，生成该shtml页面，然后再转向新生成的shtml页。
这样就通过用户的访问，逐渐将一些类似文章之类的页面静态化了。
二、设置IIS以支持shtml
在IIS里配置。
IIS7
如果安装IIS的时候没有装上的话，需要补装上这一块
打开控制面板 - 程序与功能 - 打开或关闭windows功能 - internet信息服务（就是IIS) ，在万维网服务里，勾上“服务器端包含”，安装即可
![](https://img-my.csdn.net/uploads/201302/05/1360055036_8323.png)
IIS6
![](https://img-my.csdn.net/uploads/201301/19/1358604885_1847.png)
三、如何链接公用文件
如果shtml没有外链页面，那它跟html没有多大区别；使用了外链文件，就可以部分地实现动态的功能。比如，将导航条做成公用的外链文件，如果有所变化，那么所有链接它的shtml文件都会反映出来，而无须重新这些shtml页面。
链接页面的话，可以链接物理路径文件，也可以链接虚拟路径的。我觉得虚拟路径更常用些。
类似 <!--#include virtual="/template/css.htm"-->
示例：
shtml页面部分代码
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title>左直拳的马桶</title>
    <meta name="description" content="左直拳的工作博客，技术心得" />
    <meta name="keywords" content=".NET Web Java Android Sql Server" />
    <link rel="shortcut icon" href="/img/favicon.ico" />
    <!--#include virtual="/template/css.htm"-->
    <script type="text/javascript" src="/js/MergerJsHandler.ashx?href=/js/jquery.js,/js/jquery.scrollLoading-min.js,/js/page.js,/js/_login.js,/public/_pub_show.js,/master/_base.js&v=20130114"></script>
</head>
<body>
            <div id="mb_Navi">
                <div id="mb_divAdd" style="float:right;margin-right:10px;font-size:12px;">
                    <a href="/sc.aspx">+添加</a>
                </div>
                <div>
                    <!--#include virtual="/template/navi.htm"--> 
                </div>
            </div>
```
被链接页 /template/css.htm 代码：
```
<style type="text/css">
    #divRelation{vertical-align:top;padding-top:5px; padding-left:5px; line-height:23px; font-size:12px;}
    #divRelation li { list-style-position:inside }
    #divPannel table{width:100%;font-size:12px;empty-cells:show;}
    #divPannel td{line-height:30px;text-align:center;}
    #divPannel img{vertical-align:text-bottom;}
    #divPannel span{margin-right:2px;}
    #divPannel a:link, #divPannel a:active, #divPannel a:visited {color:#777;text-decoration:underline;}
    #divPannel a:hover {color:red;text-decoration:underline;}
    #divPannel2 table{width:100%;font-size:12px;border:none;}
    #divPannel2 td{line-height:20px;text-align:center;}
    #divPannel2 img{vertical-align:text-bottom;}
    #divPannel2 a:link, #divPannel2 a:active, #divPannel2 a:visited {color:#777;text-decoration:none;}
    #divPannel2 a:hover {color:red;text-decoration:underline;}
    .bottom{vertical-align:bottom;}
    .txtad{margin:15px auto;}
    
    #st_btn {background-color:#0096E3;font-weight:bold;width:50px;text-align:center;}
    #st_btn a:link, #st_btn a:active, #st_btn a:visited {color:#fff;text-decoration:none;}
    #st_btn a:hover {color:#ff0;text-decoration:underline;} 
    #st_txt {border:1px solid #9CBBE0;width:160px;height:25px;padding-left:5px;}        
</style>
<link href="/css/default.css?ver=20130113" rel="stylesheet" type="text/css"/>
```
【注意事项】
如果页面编码为utf-8，则需要
1、shtml或模板页需要保存为 “utf-8 带签名”
2、被链接页保存为“utf-8 无签名”
否则页面顶部会莫名其妙地出现一个空行。究其原因，是链接页面以后，连接处会出现不可见的奇怪字符。
