# AngularJS开发下一代Web应用笔记（一） - weixin_33985507的博客 - CSDN博客
2015年10月30日 00:46:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
**一、写在最前**
![](https://images2015.cnblogs.com/blog/541454/201510/541454-20151030002642294-1138180067.jpg)
AngularJS是Google推出的一款Web应用开发框架。它提供了一系列兼容性良好并且可扩展的服务，包括数据绑定、DOM操作、MVC设计模式和模块加载等。
现在网上JS框架茫茫多，真不知道是到底要学习什么框架，学习了这个框架，说不定还没用在项目，又有更好的js框架出来了。前端就是不断折腾新东西，生命在于折腾~
是的，这么多的前端框架，是不是真的都要学习？？答案当然不是，看所要完成的项目。这是带我的前端前辈告诉我，具体使用什么框架还是不用框架，
看项目需求，一切都以按时按质完成作为选择的标准。但是新的东西要学习，不学习就落后。
**二、AngularJS的几个重要概念有什么**
客户端模板   举个栗子：hello.html
模型/视图/控制器(MVC)
数据绑定
依赖注入
路由管理
指令（Angular的灵魂）
**示例：购物车（**源码在最后**）**
**三、对这些概念的理解　**
插个曲，Angular最适合用来做单页web应用。在百度上查了单页web应用的概念。如表：
![](https://images2015.cnblogs.com/blog/541454/201510/541454-20151030002918357-936930546.png)
1).SEO难度较高
2).前进、后退管理（ 所有的页面切换需要自己建立堆栈管理）
3).初次加载耗时多（代码合并、CDN）
像游戏开发，频繁操作DOM也不适合使用Angular
|**客户端模板**||
|----|----|
|多页面|单页面|
|通过装配和链接服务器上数据来创建HTML，然后将构建好的HTML页面发送到浏览器。|Angular将模板和数据推送到浏览器中装配他们，然后服务器角色只是为模板提供静态资源以及为模板提供数据|
|**模型/视图/控制器(MVC)**||
|----|----|
|M 代码中清晰的分离数据管理(模型)V 及给用户呈现数据(视图)C  应用程序逻辑 (控制器 )|Angular中的M  模型中的数据便是存储在对象中的属性(属性值V   文档对象模型 (DOM)C   控制器是 JavaScript类C和V之间的纽带就是$scope|
|**数据绑定**|**双向绑定**|
|----|----|
|ng-model|所谓的双向绑定，无非**是从界面的操作能实时反映到数据，数据的变更能实时展现到界面。**|
|**依赖注入**|举个栗子|
|----|----|
|程序运行过程中，如需另一个对象协作(调用它的方法、访问他的属性)时，无须在代码中创建被调用者，而是依赖于外部容器的注入|![](https://images2015.cnblogs.com/blog/541454/201510/541454-20151030001945466-1842230622.png)在HelloController控制器中，$scope对象会把数据绑定自动传递给我们；我们不需要通过调用任何函数来创建他。只是通过将他防止在控制器的构造器中来请求它。|
**控制器helloController单词首字母大写**
|**数据绑定**||
|----|----|
||该过程，没有在输入字段上注册一个改变值的事件监听器。|
|**指令**|Angular之可以可以编写模板如HTML一样，是因为框架核心包含了一个强大的DOM解析引擎。|
|----|----|
|扩展指令|例如：ng-controller。在HTML模板看到不属于HTML规范的新属性，这称为HTML扩展指令|
|自定义指令|自己定义自己需要的指令（据说指令是Angular的灵魂）|
附购物车源码：
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
<!DOCTYPE html>
<html ng-app>
<head>
    <meta charset="UTF-8">
    <title>购物车</title>
    
</head>
<body ng-controller="CartController">
    <h1>Your order</h1>
    <div ng-repeat="item in items">
        <span>{{item.title}}</span>
        <input ng-model="item.quantity">
        <span>{{item.price | currency}}</span>
        <span>{{item.price * item.quantity | currency}}</span>
        <button ng-click="remove($index)">Remove</button>
    </div>
</body>
<script src="../assets/js/angular-1.3.0.js"></script>
<script>
function CartController($scope){
    // currency是过滤器，用来转换文本
    // $index参数   包含了它在ng-repeat的索引值
    //splice()   arrayObject.splice(index,howmany,item1,.....,itemX)
    // 定义一个一维数组存放对象
    $scope.items = [
        {title: 'Paint pots',quantity:8,price:3.95},
        {title: 'Polka dots',quantity:17,price:12.95},
        {title: 'Pebbles',quantity:5,price:6.95}
    ];
    $scope.remove = function(index){
        $scope.items.splice(index,1);
    }
}
</script>
</html>
View Code
```
 运行后结果：
![](https://images2015.cnblogs.com/blog/541454/201510/541454-20151030004529935-1623905937.png)

自定义指令的图解：
![](https://images2015.cnblogs.com/blog/541454/201511/541454-20151118132719140-355392336.png)

