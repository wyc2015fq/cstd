# thymeleaf里设置路径 - 左直拳的马桶_日用桶 - CSDN博客
2019年01月30日 17:20:48[左直拳](https://me.csdn.net/leftfist)阅读数：232
个人分类：[Java](https://blog.csdn.net/leftfist/article/category/2412493)
百里香叶子中，设置路径是一个常用的动作。
一、用统一的相对根目录路径来代替页面自己自以为是的相对路径
比如，页面上有路径：
```
<img src="../images/hi.png" />
```
这种路径，真正运行起来，不一定正确；即使当时正确，难保以后随着文件夹的变动，变得不正确了。在百里香叶子中，可以用相对根目录的路径来定义：
```
<!-- 真实路径是 /static/images/hi.png -->
<img th:src="@{/images/hi.png}" />
```
二、JS中使用相对根目录路径
```
<script type="text/javascript" th:inline="javascript">
	let configJSON = (/*[[@{/config.json}]]*/'') + '?t=' + new Date().getTime();
</script>
```
其中，`@{/config.json}`就是我们耳熟能详的相对根目录路径，但在JS中，要包在一个类似XML的注释符里：
```xml
/*[[@{/config.json}]]*/
```
如果它还要参与运算，那么还需用大括号括起来：
```
let configJSON = (/*[[@{/config.json}]]*/'') + '?t=' + new Date().getTime();
```
三、后台变量是路径的一部分
```
<link rel="stylesheet" type="text/css" th:href="${pubzy} + '/arcgis_js_api/4.10/esri/css/main.css'" />
```
后台变量，pubzy。在后台里：
```java
@RequestMapping(value = {"/","/index"}, method = RequestMethod.GET)
    public String getAisBase(Model model) {//返回页面
        model.addAttribute("pubzy", sysConfig.getPubzy());
        return "map/index";
    }
```
