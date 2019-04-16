# 最简单的ExtJS4的MVC实例 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年08月11日 13:45:45[gloomyfish](https://me.csdn.net/jia20003)阅读数：19032








**                            最简单的ExtJS4的MVC实例**


从ExtJS4.0开始，ExtJS支持MVC架构，让前端开发者更好的实现数据与逻辑分离，使用

View, Controller, Model组件定义来完成浏览器前端业务。提高代码可以维护性。ExtJS中

的View是被动模式，被Controller持有。

一个最简单的基于ExtJS4的MVC的App实例，有Contoller， View， Model，Store等组成

其目录结构必须遵循以下结构：

![](https://img-my.csdn.net/uploads/201208/11/1344661095_9986.png)


每个ExtJS的应用都必须从创建一个Ext.app.Application实例开始，app/app.js创建一个Application实例

并完成初始化。name: 'USERS'表示命名空间为USERS，完全源代码如下：



```java
Ext.Loader.setConfig({
 
    enabled: true
});
 
Ext.create('Ext.app.Application', {
 
    name: 'USERS',
    autoCreateViewport: false,
    controllers: ['Users'],
 
    launch: function() {
    	
    }
});
```
定义一个Controller

Controller将会绑定一个Application，Controller的真实目的是监听事件并作出合适的响应动作

通常这些事件来是View上面的用户操作。创建一个Controller为app/controller/Users.js



```java
Ext.define('USERS.controller.Users', {
 
    extend: 'Ext.app.Controller',
 
    models: ['User'],
    stores: ['Users'],
    views: ['Panel', 'Grid'],
 
    init: function() {
        Ext.create('USERS.view.Panel', {
            layout: 'fit',
            height: 300,
            width: 500,
 
            items: {
                xtype: 'userlist'
            },
 
            renderTo: document.body
        });
 
        Ext.create('USERS.view.Grid').show();
    }
});
```
定义两个个View，app/view/Grid.js是创建EXT
 Grid View来显示数据


```java
Ext.define('USERS.view.Grid' , {
 
    extend: 'Ext.grid.Panel',
    alias : 'widget.userlist',
    title : 'User Contact Info',
    store: 'Users', 
 
    columns: [{
        header: 'Name',
        dataIndex: 'name',
        flex: 1
    },{
        header: 'Email',
        dataIndex: 'email',
        flex: 1
    }]
});
```
app/view/Panel.js是创建EXT
 Panel View来构建Grid的layout



```java
Ext.define('USERS.view.Panel', {
    extend: 'Ext.panel.Panel'
});
```
定义一个数据模型(data
 Model) app/model/User.js



```java
Ext.define('USERS.model.User', {
 
    extend: 'Ext.data.Model',
    fields: ['id', 'name', 'email']
});
```
定义个数据存储(data
 store) app/store/User.js



```java
var myData = [
        [1,	'gloomyfish', 'bfnh1998@hotmail.com'],
        [2,	'Bob Denoy', 'bobwindy@yahoo.com'],
        [3, 'Rose Kate', 'kittygroup@facebook.com']
];

Ext.define('USERS.store.Users', {
 
    extend: 'Ext.data.Store',
    model: 'USERS.model.User',
    autoLoad: true,
    data: myData
});
```
位于app同一层次的HTML页面代码如下：



```
<html> 
	<head> 
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8"> 
		<title id="page-title">Simple MVC Application</title> 
	    <link rel="stylesheet" type="text/css" href="extjs/resources/css/ext-all.css">
	    <style type="text/css">
			body {
				margin-top: 30px;
				margin-right: 30px;
				margin-bottom: 30px;
				margin-left: 30px;
			}
		</style>
	    <script type="text/javascript" src="extjs/ext-all.js"></script>
		<script type="text/javascript" src="app/app.js"></script> 
	</head> 
	<body> 
	</body> 
</html>
```
运行效果如下：

![](https://img-my.csdn.net/uploads/201208/11/1344664593_1963.png)

参考引用
 - [http://www.sencha.com/learn/the-mvc-application-architecture/](http://www.sencha.com/learn/the-mvc-application-architecture/)

2012-08-11






