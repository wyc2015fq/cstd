# 基于ExtJS Grid创建Table例子 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年08月09日 10:49:51[gloomyfish](https://me.csdn.net/jia20003)阅读数：11797








**基本思路：**

基于ExtJS4.1版本开发，主要是首先创建一个Data Model组件，mockup一些JSON数据

然后将data Model与JSON数据绑定到创建的data store中，最后创建grid组件同时绑定

之前的data store完成整个程序。




**一个简单ExtJS组件结构图：**

![](https://img-my.csdn.net/uploads/201208/09/1344480856_9774.png)


**程序运行结果如下：**

![](https://img-my.csdn.net/uploads/201208/09/1344481042_9738.png)


**JavaScript部分的源代码如下：**



```java
Ext.require([
    'Ext.grid.*',
    'Ext.data.*',
    'Ext.util.*',
    'Ext.state.*'
]);

Ext.onReady(function() {
	// mock up data
	// sample static data for the store
    var myData = [
        ['gloomyfish',	31, 'M',  'software',  '09 July 1980'],
        ['Mike',	31, 'M',  'software',  '09 July 1984'],
        ['Green Mook',	31, 'M',  'software',  '09 July 1980'],
        ['Rose Kate',	25, 'F',  'software',  '09 July 1987'],
        ['Dave Wu',		28, 'M',  'software',  '24 July 1984'],
        ['Hong Naa',	31, 'M',  'software',  '09 July 1980']
    ];
    
    // create data model
    Ext.define('EmplyeeInfo', {
        extend: 'Ext.data.Model',
        fields: [
           {name: 'employee', type: 'string'},
           {name: 'age',      type: 'int'},
           {name: 'gentle',    type: 'string'},
           {name: 'department',  type: 'string'},
           {name: 'birthday', type: 'date', dateFormat: 'd F Y'}
        ],
    });
    
	// create the data store
    var store = Ext.create('Ext.data.ArrayStore', {
    	model: 'EmplyeeInfo',
        data: myData
    });
    
    // create the Grid
    var grid = Ext.create('Ext.grid.Panel', {
        store: store,
        columns: [
            {
                text     : 'Employee Name',
                flex     : 1,
                sortable : false,
                dataIndex: 'employee'
            },
            {
                text     : 'Age',
                width    : 75,
                sortable : true,
                dataIndex: 'age'
            },
            {
                text     : 'Gentle',
                width    : 75,
                sortable : true,
                dataIndex: 'gentle'
            },
            {
                text     : 'Department',
                width    : 75,
                sortable : true,
                dataIndex: 'department'
            },
            {
                text     : 'Birthday Date',
                width    : 85,
                sortable : true,
                renderer : Ext.util.Format.dateRenderer('m/d/Y'),
                dataIndex: 'birthday'
            }
        ],
        height: 350,
        width: 600,
        title: 'Employee Information Table',
        renderTo: 'grid-example',
        viewConfig: {
            stripeRows: true
        }
    });
});
```
**HTML部分的源代码如下：**


```
<html>
<head>
    <title>Hello Ext</title>
    <link rel="stylesheet" type="text/css" href="extjs/resources/css/ext-all.css">
    <script type="text/javascript" src="extjs/ext-all.js"></script>
    <!-- <script type="text/javascript" src="app.js"></script> -->
    <script type="text/javascript" src="app/grid-demo.js"></script>
    <style type="text/css">
		body {
			margin-top: 30px;
			margin-right: 30px;
			margin-bottom: 30px;
			margin-left: 30px;
		}
	</style>
</head>
<body>
<h1>My Fist Grid Table - ExtJS 4.1</h1>
<p>This example shows how to create a grid from Array data.</p><br>
<div id="grid-example"></div> 
</body>
</html>
```
**在google chrome上测试通过。运行本程序前请下载ExtJS 4.1开发包。**


