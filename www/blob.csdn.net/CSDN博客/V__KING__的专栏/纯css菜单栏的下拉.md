# 纯css菜单栏的下拉 - V__KING__的专栏 - CSDN博客





2014年11月14日 10:42:51[v__king__](https://me.csdn.net/V__KING__)阅读数：446标签：[web																[菜单下拉](https://so.csdn.net/so/search/s.do?q=菜单下拉&t=blog)](https://so.csdn.net/so/search/s.do?q=web&t=blog)







```
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>纯CSS下拉菜单，兼容IE和FF</title>
<meta name="keywords" content="jquery,jquery吧,jquery插件,jquery 特效,jquery 教程,jquery 效果,jquery ui,前端开发,web前端,web前端开发,,前端资源,html+CSS,JavaScript,html5,css3,css,css学习,css教程,浏览器兼容,php" />
<meta name="description" content="Write Less, Do More,重注前端开发,web前端开发" />
<link rel="shortcut icon" href="http://www.jqueryba.com/favicon.ico" type="image/x-icon"/>
<link rel="profile" href="http://gmpg.org/xfn/11" />
<link rel="alternate" type="application/rss+xml" title="RSS 2.0" href="http://www.jqueryba.com/rss/" />
</head>
<body>
<style type="text/css">
body {color:#fff;}
#wrapper {color:#000;}
.red {color:#c00;}
#info {margin-top:20px;}
#info h1 {font-size:3em; text-align:center; font-family: georgia, "times new roman", serif;}
#head {height:145px; border:0;}
#positioner {clear:both; position:relative; left:1px; z-index:100;}
#ads {position:relative; z-index:10;}
.menu {display:none;}
#noniemenu {position:absolute;}
#noniemenu .holder ul {padding:0; margin:0;}
#noniemenu .holder ul li {list-style-type: none;}
#noniemenu .holder li {}
#noniemenu .holder li ul {display: none;}
#noniemenu .holder li:hover > ul#a3 {display:block; position:absolute; left:105px; margin-top:-20px; border:1px solid #000;}
#noniemenu .holder .bold {font-weight:bold;}
#noniemenu .holder {
  color:#fff;
  width:104px;
  height:18px;
  display:block;
  overflow:hidden;
  float:left;
  border:1px solid #000;
  margin-right:1px;
  font-size:10px;
  }
#noniemenu .holder:hover {
  height:auto;
  }
#noniemenu a.outer, #noniemenu a.outer:visited {
   color:#fff;
   width:104px;
   line-height:18px;
   display:block;
   background:#e09222;
   text-align:center;
   text-decoration:none;
   font-family: verdana, arial, sans-serif;
   }
#noniemenu a.outer:hover {
  background:#697210;
  overflow:visible;
  }
#noniemenu div.open {display:none;}
#noniemenu a.inner, #noniemenu a.inner:visited {
  display:block;
  width:104px;
  height:18px;
  line-height:18px;
  border-bottom:1px solid #000;
  text-decoration:none;
  color:#000;
  background:#eee;
  text-align:center;
  }
#noniemenu a.second {font-weight:bold;}
#noniemenu a.inner:hover {
  background:#add;
  }
</style>
<!--[if lte IE 6]>
<style type="text/css">
body {margin-top:-8px;}
#head {height:147px;}
#noniemenu {display:none;}
.menu {display:block; position:absolute;}
.menu a.outer, .menu a.outer:visited {
   color:#fff;
   width:104px;
   height:18px;
   display:block;
   background:#e09222;
   border:1px solid #000;
   margin-right:1px;
   text-align:center;
   float:left;
   text-decoration:none;
   font-family: verdana, arial, sans-serif;
   font-size:10px;
   line-height:18px;
   overflow:hidden;
   }
.menu a.outer:hover {
  background:#697210;
  overflow:visible;
  }
.menu a.outer:hover table.first {
  display:block;
  background:#eee;
  border-collapse:collapse;
  }
.menu a.inner, .menu a.inner:visited {
  display:block;
  width:102px;
  height:18px;
  border-bottom:1px solid #000;
  text-decoration:none;
  color:#000;
  font-family: verdana, arial, sans-serif;
  font-size:10px;
  text-align:center;
  }
.menu a.inner:hover {
  background:#add;
  }
.menu a.outer table.first a.second {
  height:18px;
  line-height:18px;
  overflow:hidden;
  font-weight:bold;
  }
.menu a.outer table.first a.second:hover {
  position:relative;
  overflow:visible;
  }
.menu a.outer table.first a.second:hover table {
  position:absolute;
  top:-2px;
  left:102px;
  border-collapse:collapse;
  background:#eee;
  border:1px solid #000;
  font-weight:normal
}
</style>
<![endif]-->
<!--[if lte IE 6]>
<style>
#ads {display:none;}
#adsie {clear:both; text-align:center; width:750px; margin-top:10px;}
</style>
<![endif]-->
</head>
<body>
<div id="wrapper">
<div id="head">
<div id="positioner">
<div class="menu">
<a class="outer" href="">DEMOS
<table class="first"><tr><td>
<a class="inner" href="" title="The zero dollar ads page">zero dollars</a>
<a class="inner" href="" title="Wrapping text around images">wrapping text</a>
<a class="inner" href="" title="Styling forms">styled form</a>
<a class="inner" href="" title="Removing active/focus borders">active focus</a>
<a class="inner second" href="" title="Hover/click with no active/focus borders">HOVER/CLICK >
<table><tr><td>
<a class="inner" href="" title="Styling forms">styled form</a>
<a class="inner" href="" title="Removing active/focus borders">active focus</a>
<a class="inner" href="" title="Hover/click with no active/focus borders">hover/click</a>
</td></tr></table>
</a>
<a class="inner" href="" title="Multi-position drop shadow">shadow boxing</a>
<a class="inner" href="" title="Image Map for detailed information">image map</a>
<a class="inner" href="" title="fun with background images">fun backgrounds</a>
<a class="inner" href="" title="fade-out scrolling">fade scrolling</a>
<a class="inner" href="" title="em size images compared">em sized images</a>
</td></tr></table>
</a>
<a class="outer" href="">MENUS
<table class="first"><tr><td>
<a class="inner" href="" title="a coded list of spies">spies menu</a>
<a class="inner" href="" title="a horizontal vertical menu">vertical menu</a>
<a class="inner" href="" title="an enlarging unordered list">enlarging list</a>
<a class="inner" href="" title="an unordered list with link images">link images</a>
<a class="inner" href="" title="non-rectangular links">non-rectangular</a>
<a class="inner" href="" title="jigsaw links">jigsaw links</a>
<a class="inner" href="" title="circular links">circular links</a>
</td></tr></table>
</a>
<a class="outer" href="">LAYOUTS
<table class="first"><tr><td>
<a class="inner" href="" title="Cross browser fixed layout">Fixed 1</a>
<a class="inner" href="" title="Cross browser fixed layout">Fixed 2</a>
<a class="inner" href="" title="Cross browser fixed layout">Fixed 3</a>
<a class="inner" href="" title="Cross browser fixed layout">Fixed 4</a>
<a class="inner" href="" title="A simple minimum width layout">minimum width</a>
</td></tr></table>
</a>
<a class="outer" href="">BOXES
<table class="first"><tr><td>
<a class="inner" href="" title="Left scroll bars">left scroll</a>
<a class="inner" href="" title="IE6 3px float fix">IE6 3px fix</a>
<a class="inner" href="" title="Snazzy borders">snazzy borders</a>
<a class="inner" href="" title="Krazy Korners">krazy korners</a>
<a class="inner" href="" title="Percentage PLUS pixels">% PLUS pixels</a>
<a class="inner" href="" title="min-width for IE">IE min-width</a>
<a class="inner" href="" title="min-height for IE">IE min-height</a>
</td></tr></table>
</a>
<a class="outer" href="../mozilla/index.html">MOZILLA
<table class="first"><tr><td>
<a class="inner" href="" title="A drop down menu">drop down menu</a>
<a class="inner" href="" title="A cascading menu">cascading menu</a>
<a class="inner" href="" title="Using content:">content:</a>
<a class="inner" href="" title=":hover applied to a div">mozzie box</a>
<a class="inner" href="" title="I can build a rainbow">rainbow box</a>
<a class="inner" href="" title="Snooker cue">snooker cue</a>
<a class="inner" href="" title="Target Practise">target practise</a>
<a class="inner" href="" title="Two tone headings">two tone headings</a>
<a class="inner" href="" title="Shadow text">shadow text</a>
</td></tr></table>
</a>
<a class="outer" href="">EXPLORER
<table class="first"><tr><td>
<a class="inner" href="" title="Example one">example one</a>
<a class="inner" href="" title="Weft fonts">weft fonts</a>
<a class="inner" href="" title="Vertical align">vertical align</a>
</td></tr></table>
</a>
<a class="outer" href="">OPACITY
<table class="first"><tr><td>
<a class="inner" href="" title="colour wheel">opaque colours</a>
<a class="inner" href="" title="a menu using opacity">opaque menu</a>
<a class="inner" href="" title="partial opacity">partial opacity</a>
<a class="inner" href="" title="partial opacity II">partial opacity II</a>
</td></tr></table>
</a>
</div>
<div id="noniemenu">
<div class="holder">
<ul>
<li><a class="outer" href="">DEMOS</a></li>
<li><a class="inner" href="" title="The zero dollar ads page">zero dollars</a></li>
<li><a class="inner" href="" title="Wrapping text around images">wrapping text</a></li>
<li><a class="inner" href="" title="Styling forms">styled form</a></li>
<li><a class="inner" href="" title="Removing active/focus borders">active focus</a></li>
<li><a class="inner second" href="" title="Hover/click with no active/focus borders">HOVER/CLICK ></a>
<ul id="a3">
<li><a class="inner" href="" title="Styling forms">styled form</a></li>
<li><a class="inner" href="" title="Removing active/focus borders">active focus</a></li>
<li><a class="inner" href="" title="Hover/click with no active/focus borders">hover/click</a></li>
</ul>
</li>
<li><a class="inner" href="" title="Multi-position drop shadow">shadow boxing</a></li>
<li><a class="inner" href="" title="Image Map for detailed information">image map</a></li>
<li><a class="inner" href="" title="fun with background images">fun backgrounds</a></li>
<li><a class="inner" href="" title="fade-out scrolling">fade scrolling</a></li>
<li><a class="inner" href="" title="em size images compared">em sized images</a></li>
</ul>
</div>
<div class="holder">
<ul>
<li><a class="outer" href="">MENUS</a></li>
<li><a class="inner" href="" title="a coded list of spies">spies menu</a></li>
<li><a class="inner" href="" title="a horizontal vertical menu">vertical menu</a></li>
<li><a class="inner" href="" title="an enlarging unordered list">enlarging list</a></li>
<li><a class="inner" href="" title="an unordered list with link images">link images</a></li>
<li><a class="inner" href="" title="non-rectangular links">non-rectangular</a></li>
<li><a class="inner" href="" title="jigsaw links">jigsaw links</a></li>
<li><a class="inner" href="" title="circular links">circular links</a></li>
</ul>
</div>
<div class="holder">
<ul>
<li><a class="outer" href="">LAYOUTS</a></li>
<li><a class="inner" href="" title="Cross browser fixed layout">Fixed 1</a></li>
<li><a class="inner" href="" title="Cross browser fixed layout">Fixed 2</a></li>
<li><a class="inner" href="" title="Cross browser fixed layout">Fixed 3</a></li>
<li><a class="inner" href="" title="Cross browser fixed layout">Fixed 4</a></li>
<li><a class="inner" href="" title="A simple minimum width layout">minimum width</a></li>
</ul>
</div>
<div class="holder">
<ul>
<li><a class="outer" href="">BOXES</a></li>
<li><a class="inner" href="" title="Left scroll bars">left scroll</a></li>
<li><a class="inner" href="" title="IE6 3px float fix">IE6 3px fix</a></li>
<li><a class="inner" href="" title="Snazzy borders">snazzy borders</a></li>
<li><a class="inner" href="" title="Krazy Korners">krazy korners</a></li>
<li><a class="inner" href="" title="Percentage PLUS pixels">% PLUS pixels</a></li>
<li><a class="inner" href="" title="min-width for IE">IE min-width</a></li>
<li><a class="inner" href="" title="min-height for IE">IE min-height</a></li>
</ul>
</div>
<div class="holder">
<ul>
<li><a class="outer" href="">MOZILLA</a></li>
<li><a class="inner" href="" title="A drop down menu">drop down menu</a></li>
<li><a class="inner" href="" title="A cascading menu">cascading menu</a></li>
<li><a class="inner" href="" title="Using content:">content:</a></li>
<li><a class="inner" href="" title=":hover applied to a div">mozzie box</a></li>
<li><a class="inner" href="" title="I can build a rainbow">rainbow box</a></li>
<li><a class="inner" href="" title="Snooker cue">snooker cue</a></li>
<li><a class="inner" href="" title="Target Practise">target practise</a></li>
<li><a class="inner" href="" title="Two tone headings">two tone headings</a></li>
<li><a class="inner" href="" title="Shadow text">shadow text</a></li>
</ul>
</div>
<div class="holder">
<ul>
<li><a class="outer" href="">EXPLORER</a></li>
<li><a class="inner" href="" title="Example one">example one</a></li>
<li><a class="inner" href="" title="Weft fonts">weft fonts</a></li>
<li><a class="inner" href="" title="Vertical align">vertical align</a></li>
</ul>
</div>
<div class="holder">
<ul>
<li><a class="outer" href="">test</a></li>
<li><a class="inner" href="" title="">DEMO</a></li>
<li><a class="inner" href="" title="">DEMO</a></li>
<li><a class="inner" href="" title="">DEMO</a></li>
<li><a class="inner" href="" title="">DEMO</a></li>
</ul>
</div>
</div>
</div>
</div> 

</div>
</body>
</html>
```





