
# CSS+JS 悬浮固定菜单效果 - 阳光岛主 - CSDN博客

2015年08月22日 13:28:34[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：12183


首先，先看实例，**米扑代理**：[http://proxy.mimvp.com/price.php](http://proxy.mimvp.com/price.php)
效果如下图，兼容chrome，firefox，windows，360，搜狗等主流浏览器
![cssjs-suspension-fixed-menu](http://cdn-blog.mimvp.com/wp-content/uploads/2015/08/cssjs-suspension-fixed-menu.png)
米扑代理，是由百度，小米出来的工程师创建，做得非常棒，功能很全很贴心。
**JS + CSS 经典实现**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|52
|53
|54
|55
|56
|57
|58
|59
|60
|61
|62
|63
|64
|65
|66
|67
|68
|69
|70
|71
|72
|73
|74
|75
|76
|77
|78
|79
|80
|81
|82
|83
|84
|85
|86
|87
|88
|89
|90
|91
|92
|93
|94
|95
|96
|97
|98
|<!DOCTYPE HTML>
|<|html|lang|=|"en-US"|>
|<|head|>
|<|title|>悬浮固定菜单</|title|>
|<|style|type|=|"text/css"|>
|.wrapper {
|width:1000px;
|height:2000px;
|margin-left:auto;
|margin-right:auto;
|text-align: center;
|}
|.header {
|height:150px;
|}
|\#nav {
|padding: 10px;
|position: relative;
|top: 0;
|background: \#375069;
|width: 1000px;
|}
|\#nav a {
|display:inline-block;
|margin:0 10px;
|*display:inline;
|color:white;
|width: 15%;
|}
|p a {
|margin:0 10px;
|width: 15%;
|}
|</|style|>
|<|script|type|=|"text/javascript"|>
|window.onload = function(){
|menuFixed('nav');
|}
|function menuFixed(id){
|var obj = document.getElementById(id);
|var objHeight = obj.offsetTop;
|window.onscroll = function(){
|var obj = document.getElementById(id);
|var scrollTop = document.documentElement.scrollTop || document.body.scrollTop;
|if(scrollTop <|objHeight|){
|obj.style.position|=|'relative'|;
|}else{
|obj.style.position|=|'fixed'|;
|}
|}
|}
|</script>
|</|head|>
|<|body|>
|<|div|class|=|"wrapper"|>
|<|div|class|=|"header"|>
|</|div|>
|<|div|id|=|"nav"|>
|<|a|href|=|"http://proxy.mimvp.com"|>米扑代理</|a|>
|<|a|href|=|"http://apptop.mimvp.com"|>米扑排名</|a|>
|<|a|href|=|"http://domain.mimvp.com"|>米扑域名</|a|>
|<|a|href|=|"http://blog.mimvp.com"|>米扑博客</|a|>
|<|a|href|=|"http://forum.mimvp.com"|>米扑论坛</|a|>
|</|div|>
|<|div|id|=|"content"|style|=|"text-align: left;"|>
|<|p|>悬浮经典实例请参考：米扑代理</|p|>
|<|p|>米扑代理价格表实例： </|p|>
|<|p|><|a|href|=|"http://proxy.mimvp.com/price.php"|>http://proxy.mimvp.com/price.php</|a|></|p|>
|<|p|>悬浮经典实例请参考： 米扑代理</|p|>
|<|p|>米扑代理价格表实例： </|p|>
|<|p|><|a|href|=|"http://proxy.mimvp.com/price.php"|>http://proxy.mimvp.com/price.php</|a|></|p|>
|<|p|>悬浮经典实例请参考： 米扑代理</|p|>
|<|p|>米扑代理价格表实例： </|p|>
|<|p|><|a|href|=|"http://proxy.mimvp.com/price.php"|>http://proxy.mimvp.com/price.php</|a|></|p|>
|</|div|>
|</|div|>
|</|body|>
|</|html|>
|

**运行效果：**
![cssjs-suspension-fixed-menu-01](http://cdn-blog.mimvp.com/wp-content/uploads/2015/08/cssjs-suspension-fixed-menu-01.png)
**示例代码下载**
[mimvp_navi_demo.tar](http://cdn-blog.mimvp.com/wp-content/uploads/2015/08/mimvp_navi_demo.tar.gz)

