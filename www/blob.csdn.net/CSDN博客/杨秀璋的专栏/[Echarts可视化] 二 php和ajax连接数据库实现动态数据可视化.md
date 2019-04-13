
# [Echarts可视化] 二.php和ajax连接数据库实现动态数据可视化 - 杨秀璋的专栏 - CSDN博客

2017年06月02日 19:57:23[Eastmount](https://me.csdn.net/Eastmount)阅读数：7635所属专栏：[HTML网站前端设计](https://blog.csdn.net/column/details/13444.html)[PHP基础知识及网站开发](https://blog.csdn.net/column/details/eastmount-php.html)



前一篇文章 "[[Echarts可视化] 一.入门篇之简单绘制中国地图和贵州地区](http://blog.csdn.net/eastmount/article/details/69938479)" 主要是通过Echarts可视化介绍入门知识、中国地图和贵州地区各省份的数据分析，其中贵州地图才是它的核心内容。
这篇文章主要结合PHP、MySQL、JQuery和Ajax从数据库中获取数据，动态的显示值，这是实现真正可视化的技术入，可以应用于实际的项目中。基础性文章，希望对您有所帮助，如果文章中存在不足之处，还请海涵~
参考文章：[ECharts, PHP, MySQL, Ajax, JQuery 实现前后端数据可视化](http://blog.csdn.net/marksinoberg/article/details/52875610)
[echarts+php+mysql 绘图实例 - 谦一](http://www.cnblogs.com/qjqcs/p/5852958.html)
[PHP+echarts读取地图数据](http://www.sucaihuo.com/js/720.html)
该篇文章代码下载地址：



## 一. 回顾Echarts绘制直方图
Echarts官网地址：[http://echarts.baidu.com/index.html](http://echarts.baidu.com/index.html)
注意：基础知识参考前一篇文章，这篇文章主要是代码为主，整个流程都是详细的。同时发现，准备写这篇文章的时候，“[纯臻](blog.csdn.net/marksinoberg/article/details/52875610)”大神也写了篇类似的，但依然有不同的地方可能对你有所帮助。
Echarts代码 test01.html

```python
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>ECharts</title>
    <!-- 引入 echarts.js -->
    <script src="echarts.min.js"></script>
</head>
<body>
    <!-- 为ECharts准备一个具备大小（宽高）的Dom -->
    <div id="main" style="width: 600px;height:400px;"></div>
    <script type="text/javascript">
        // 基于准备好的dom，初始化echarts实例
        var myChart = echarts.init(document.getElementById('main'));
        // 指定图表的配置项和数据
        var option = {
            title: {
                text: 'ECharts 入门示例'
            },
            tooltip: {},
            legend: {
                data:['数量']
            },
            xAxis: {
                data: ["贵阳市","遵义市","凯里市","六盘水市","都匀市","毕节市"]
            },
            yAxis: {},
            series: [{
                name: '数量',
                type: 'bar',
                data: [5, 20, 36, 10, 10, 20]
            }]
        };
        // 使用刚指定的配置项和数据显示图表。
        myChart.setOption(option);
    </script>
</body>
</html>
```
运行结果如下图所示：
![](https://img-blog.csdn.net/20170602191644181)


## 二. 创建数据库
创建数据库hcpmanage，再创建表guizhou，包括序号、地名、数量三个字段。
如下所示：

![](https://img-blog.csdn.net/20170602192002224)
SQL语句对应的代码：
```python
-- ----------------------------
-- Table structure for `guizhou`
-- ----------------------------
DROP TABLE IF EXISTS `guizhou`;
CREATE TABLE `guizhou` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `place` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `num` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
-- ----------------------------
-- Records of guizhou
-- ----------------------------
INSERT INTO `guizhou` VALUES ('1', '贵阳市', '5');
INSERT INTO `guizhou` VALUES ('2', '遵义市', '20');
INSERT INTO `guizhou` VALUES ('3', '凯里市', '36');
INSERT INTO `guizhou` VALUES ('4', '六盘水市', '10');
INSERT INTO `guizhou` VALUES ('5', '都匀市', '10');
INSERT INTO `guizhou` VALUES ('6', '毕节市', '20');
```


## 三. PHP连接数据库及Json传值
PHP连接数据库的核心代码大家非常熟悉了，下面这段代码简单包括了Json取值。
sql.php文件：

```python
<?php
	//PHP连接数据库 
	header("content-type:text/json;charset=utf-8");
	//链接数据库
	$con = mysql_connect("localhost", "root", "123456"); 
	if (!$con) 
	{ 
		die('Could not connect database: ' . mysql_error()); 
	} 
	//选择数据库
	$db_selected = mysql_select_db("hcpmanage", $con); 
	if (!$db_selected) 
	{ 
	 	die ("Can\'t use yxz : " . mysql_error()); 
	} 
	//执行MySQL查询-设置UTF8格式
	mysql_query("SET NAMES utf8"); 	
	//查询学生信息
	$sql = "SELECT * FROM `guizhou` ";
	$result = mysql_query($sql,$con); 
	//定义变量json存储值
	$data="";
	$array= array();
	class GuiZhou{
		public $id;
	    public $place;
	    public $num;
	}
	while ($row = mysql_fetch_row($result))
	{ 
		list($id,$place,$num) = $row;   
  	
  		/*
		echo $id.' ';
		echo $place.' ';
		echo $num;
		echo '<br >';
		*/
		$gz = new GuiZhou();
		$gz->id = $id;
		$gz->place = $place;
		$gz->num = $num;
		//数组赋值
		$array[] = $gz;
	}
	$data = json_encode($array);
	echo $data;
	//{"id":"1","place":"\u8d35\u9633\u5e02","num":"5"}
?>
```
核心代码，通过array数组获取Guizhou类中的值，该值是读取数据库并且赋值给实例gz的，再调用json_encode($array)进行格式转换。输出结果如下所示，也可以Json只获取两个值。

![](https://img-blog.csdn.net/20170602192729937)
注意访问地址为：http://localhost:8080/20170224YXZ/sql.php，文件放到Apache的htdocs目录下。所有文件如下图所示：

![](https://img-blog.csdn.net/20170602192928659)



## 四. Ajax动态实现Echarts可视化
test02.html代码如下：
```python
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>ECharts</title>
    <!-- 引入 echarts.js -->
    <script src="echarts.min.js"></script>
    <script src="jquery-1.9.1.min.js"></script>
</head>
<body>
    <!-- 为ECharts准备一个具备大小（宽高）的Dom -->
    <div id="main" style="width: 600px;height:400px;"></div>
    <script type="text/javascript">
        // 基于准备好的dom，初始化echarts实例
        var myChart = echarts.init(document.getElementById('main'));
        // 初始化两个数组，盛装从数据库中获取到的数据
        var places=[], nums=[];
        function TestAjax(){
            $.ajax({
                type: "post",
                async: false,     //异步执行
                url: "sql.php",   //SQL数据库文件
                data: {},         //发送给数据库的数据
                dataType: "json", //json类型
                success: function(result) {
                    if (result) {
                        for (var i = 0; i < result.length; i++) {
                            places.push(result[i].place);
                            nums.push(result[i].num);
                            console.log(result[i].place);
                            console.log(result[i].num);
                        }
                    }
                }
            })
            return places, nums;
        }
        //执行异步请求
        TestAjax();
        // 指定图表的配置项和数据
        var option = {
            title: {
                text: 'ECharts+Ajax+MySQL+PHP'
            },
            tooltip: {
                show : true
            },
            legend: {
                data:['数量']
            },
            xAxis: [{
                //data: ["贵阳市","遵义市","凯里市","六盘水市","都匀市","毕节市"]
                type : 'category',
                data : places
            }],
            yAxis: {
                type : 'value'
            },
            series: [{
                name : "数量",
                type : "bar",
                data : nums
            }]
        };
        // 使用刚指定的配置项和数据显示图表。
        myChart.setOption(option);
    </script>
    <marquee>Eastmount 璋 love 娜!!!</marquee>
</body>
</html>
```
运行结果如下图所示，这是动态调用的可视化结果。
![](https://img-blog.csdn.net/20170602193647254)
其中Ajax核心代码为：
```python
var places=[], nums=[];
        function TestAjax(){
            $.ajax({
                type: "post",
                async: false,     //异步执行
                url: "sql.php",   //SQL数据库文件
                data: {},         //发送给数据库的数据
                dataType: "json", //json类型
                success: function(result) {
                    if (result) {
                        for (var i = 0; i < result.length; i++) {
                            places.push(result[i].place);
                            nums.push(result[i].num);
                            console.log(result[i].place);
                            console.log(result[i].num);
                        }
                    }
                }
            })
            return places, nums;
        }
        //执行异步请求
        TestAjax();
```
同时，浏览器中检查元素可以对Ajax的值进行测试输出，调用console.log函数。
如下图所示：
![](https://img-blog.csdn.net/20170602194115314)
数据动态变化，数据库变了，可视化图也变化：
![](https://img-blog.csdn.net/20170603095710533)




## 五. 难点及注意事项
初学的同学可能会遇到一下几个问题：
1.其中Echarts和JQuery需要导入的，即：
<script src="echarts.min.js"></script>
<script src="jquery-1.9.1.min.js"></script>
2.Echarts使用的方法请大家学习前一篇文章和官网文章，这里不再叙述。
3.PHP文件需要header("content-type:text/json;charset=utf-8");声明Json格式，同时Ajax也需要设置dataType: "json"。
4.声明的Ajax函数需要执行，对应代码中的TestAjax()，否则只是定义未执行。
5.如果Ajax报错 [object Object]，如下图所示：
![](https://img-blog.csdn.net/20170602194519632)

这需要Json格式一致，而我的错误是因为在PHP文件中增加了个注释导致无值，百度都无法解决，头都弄大了，所以写代码需要仔细。

![](https://img-blog.csdn.net/20170602194640166)

6.最后动态数据是通过变量实现的，即ages替换以前的 [5, 20, 36, 10, 10, 20]。
series : [ {
name : "num",
type : "bar",
data : ages
}]


最后希望这篇文章对你有所帮助，尤其是对Echarts可视化感兴趣的同学，同样Java、C\#、Python相关网站的原理一样，代码也会上传。最近心情真的好低落，哎！有时间写日记总结下吧。
*稳坐珠江夜游船，翻转长隆水世界。*
*笑谈粤广风流人，宁负天下不负卿*。
哎！总是心事重重，自己真的有太多的不足和需要成长的地方，以前总会知足，现在也会。哎！为人处世真需要一辈子去学习，且写且珍惜吧，传递正能量。
(By:Eastmount 2017-06-02 中午2点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))


