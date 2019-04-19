# js jQuery 右键菜单 清屏 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年10月17日 14:06:30[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：40
所属专栏：[前端技术](https://blog.csdn.net/column/details/31053.html)
主要用到了oncontextmenu事件，在oncontextmenu事件中使用return false 屏蔽掉原生右键菜单，再使用event获取鼠标的坐标位置，设置自定义菜单的位置。
[http://swisnl.github.io/jQuery-contextMenu/demo.html](http://swisnl.github.io/jQuery-contextMenu/demo.html)
[https://swisnl.github.io/jQuery-contextMenu/docs.html](https://swisnl.github.io/jQuery-contextMenu/docs.html)
[http://www.hangge.com/blog/cache/detail_1821.html](http://www.hangge.com/blog/cache/detail_1821.html)
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title></title>
    <script src="jquery-3.1.1.js" charset="utf-8"></script>
    <script src="contextMenu/jquery.ui.position.min.js" type="text/javascript"></script>
    <script src="contextMenu/jquery.contextMenu.js" type="text/javascript"></script>
    <link href="contextMenu/jquery.contextMenu.css" rel="stylesheet" type="text/css" />
  </head>
  <body>
    <button class="context-menu-one">按钮1</button>
    <script type="text/javascript">
      $(function() {
         //初始化菜单
         $.contextMenu({
             selector: '.context-menu-one',
             callback: function(key, options) {
                 console.log("点击了：" + key);
             },
             items: {
                 "edit": {name: "编辑", icon: "edit"},
                 "cut": {name: "剪切", icon: "cut"},
                 "copy": {name: "复制", icon: "copy"},
                 "paste": {name: "粘贴", icon: "paste"},
                 "delete": {name: "删除", icon: "delete"},
                 "sep1": "---------",
                 "quit": {name: "退出", icon: function(){
                     return 'context-menu-icon context-menu-icon-quit';
                 }}
             }
         });
      });
    </script>
  </body>
</html>
icon: "paste" // Class context-menu-icon-paste is used on the menu item.
使用自带的图标需要font文件夹的内容
[http://www.111cn.net/wy/jquery/141609.htm](http://www.111cn.net/wy/jquery/141609.htm)
