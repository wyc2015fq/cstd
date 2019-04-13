
# js处理json和字符串示例 - 阳光岛主 - CSDN博客

2013年09月07日 00:33:20[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：5275


js处理json格式的插入、修改、删除，以及字符串的比较等常用操作

**demo 1：**
json格式的插入、删除
```python
<html>
	<head>
		<title></title>
		<script language="javascript">
			function change(){
				var obj=document.getElementById("floor");
				if (document.getElementById("qu").value=="1"){
					var t=document.createElement("OPTION");
					t.text="第五楼";
					t.value="5";
					obj.add(t);
				}else if(document.getElementById("qu").value=="2"){
					for(var i=0; i<obj.length;i++){
						if(obj.options[i].value=="5"){
							obj.remove(i);
							return;
						}
					}
				}
			}
		</script>
	</head>
	<body>
	<form id="form1" name="form1" method="post" action="">
	
	  <select multiple="multiple" name="qu" id="qu" onchange="change()" style="width: 150px; height: 300px;">
	    <option value="请选择所在区">--请选择所在区--</option>
	    <option value="1">南开区</option>
	    <option value="2">红桥区</option>
      </select>
      
	  <select multiple="multiple" name="floor" id="floor" style="width: 150px; height: 300px;">
          <option value="请选择楼层">--请选择楼层--</option>
          <option value="1">第一楼</option>
          <option value="2">第二楼</option>
          <option value="3">第三楼</option>
          <option value="4">第四楼</option>
        </select>
    </form>
	
	</body>
</html>
```
**运行结果：**
![](https://img-blog.csdn.net/20130827091254281)


**demo2：**

```python
<!DOCTYPE html>
<html>
	<script language="javascript">
		function toLeft() {
			var list1 = document.getElementById("list1");
			var list2 = document.getElementById("list2");
  
			for (var i = list2.options.length-1; i >= 0 ; i--) {
				if(list2.options[i].selected ==true){
					var op = document.createElement("option");
					op.text = list2.options[i].text;
					op.value = list2.options[i].value;
					list1.add(op);
					list2.remove(i);
				}
			}
		}
		function toRight() {
			var list1 = document.getElementById("list1");
			var list2 = document.getElementById("list2");
  
			for (var i = list1.options.length-1; i >= 0 ; i--) {
				if(list1.options[i].selected ==true){
					var op = document.createElement("option");
					op.text = list1.options[i].text;
					op.value = list1.options[i].value;
					list2.add(op);
					list1.remove(i);
				}
			}
		}
		function submit() {
			var list2 = document.getElementById("list2");
			var innerStr = "";
			for (var i = 0; i < list2.options.length; i++) {
				innerStr += i + " - " + list2.options[i].text + " : " + list2.options[i].value + "</br>";
			}
			var wt = document.getElementById("wt");
			wt.innerHTML = innerStr;
		}
	</script>
</head>
<body>
<div>
<select multiple="multiple" id="list1" style="width: 500px; height: 100px;>
  <option value="volvo">Volvo</option>
  <option value="saab">Saab</option>
  <option value="opel">Opel</option>
  <option value="audi">Audi</option>
</select>
<input type="button" value=">" onclick="toRight();submit()"/>  
<input type="button" value="<" onclick="toLeft();submit()"/>
<select multiple="multiple" id="list2" style="width: 500px; height: 100px;>
  <option value="volvo">Volvo</option>
  <option value="saab">Saab</option>
  <option value="opel">Opel</option>
  <option value="audi">Audi</option>
</select>
<input type="button" value="submit" onclick="submit()"/>
<div id="wt"/></div>
</body>
</html>
```
**运行结果：**![](https://img-blog.csdn.net/20130827091309203)


**Demo 3：**
json元素的插入、修改、删除，以及与字符串的转化格式
```python
<html>
<head>
<title>IT-Homer demo</title>
</head>
<body>
	<input type="button" id="parse" value="parse json" onclick="parseJson()" />
	<div id="txt"  /> 
	
	<script type="text/javascript">
		function parseJson(){
			var txt = document.getElementById("txt");
		
			var jsonData = '{"name":"it-homer","age":25}';
			var field = "";
			
			if(jsonData.length <= 0){
				jsonData = '{}';
			}
				
			var jsonObj = JSON.parse(jsonData);				// ok
		//	var jsonObj = eval('(' + jsonData + ')');		// ok
		//	var jsonObj = jsonData.parseJSON();				// error
			field += "name = " + jsonObj.name;
			field += ", age = " + jsonObj.age;
			
			jsonObj["sex"] = "boy";		// add json, {"name":"it-homer","age":25,"sex":"boy"}
		//	createJson(jsonObj, "sex", "boy");
		
			jsonData = JSON.stringify(jsonObj);		// ok 
		//	jsonData = jsonObj.toJSONString();		// error
				
			var sex = "";
			if(jsonObj.length > 0) {
				sex = jsonObj.sex;
			}
				
			txt.innerHTML = field + "</br>" + jsonData;
		}
		
		function createJson(jsonObj, key, value){
			if(typeof value === "undefined"){
				delete jsonObj[key];
			} else {
				jsonObj[key] = value;
			}			
		}
	</script>
</body>
</html>
```
**运行结果：**
![](https://img-blog.csdn.net/20130827102209046)


**Demo4：**
比较两个字符串不同的元素并打印出来，其中两个字符串是包含关系，即一个字符串一定是另一个字符串的子串，如{1,2,4}是{1,2,3,4,5}的子串

```python
<html>
<head>
<title>IT-Homer demo</title>
</head>
<body>
	<input type="button" id="parse" value="parse json" onclick="parseJson222()" />
	<div id="txt"  /> 
	
	<script type="text/javascript">		
		function parseJson222(){
			var txt = document.getElementById("txt");
		
	//		var jsonData_old = '_2,4,_4,21,_5,22,8,_7,23,_9,11,12,13,61';
	//		var jsonData_new = '_2,_3,1,2,3,4,_4,21,_5,22,_6,5,6,7,8,_7,23,_9,11,12,13,14,-1,61';		
			
			var jsonData_new = '_2,4,_4,21,_5,22,8,_7,23,_9,11,12,13,61';
			var jsonData_old = '_2,_3,1,2,3,4,_4,21,_5,22,_6,5,6,7,8,_7,23,_9,11,12,13,14,-1,61';
			
			var diff = diffJson(jsonData_old, jsonData_new);
			txt.innerHTML = diff;
			
			printArray("diff", diff);
		}
		
		function diffJson(jsonData_old, jsonData_new){
			var diff = "";
			
			var oldArray = jsonData_old.split(",");
			var newArray = jsonData_new.split(",");
			var oldLen = oldArray.length;
			var newLen = newArray.length;
			
			var minLen = Math.min(oldLen, newLen);
			if(minLen == newLen){
				tmpArray = newArray;	// swap array
				newArray = oldArray;
				oldArray = tmpArray;
				
				newLen = oldLen;		// swap array length
				oldLen = minLen;
			}
			
			printArray("newArray", newArray);
			printArray("oldArray", oldArray);
			
			var arr = [];
			for(i=0; i<newLen; i++){
				var j=0;
				for(j=0; j<oldLen; j++){
					if(newArray[i] == oldArray[j]){
						break;
					}
				}
				if(j == oldLen){
					arr.push(newArray[i]);
				}
			}
			return arr;
		}
		
		function printArray(tag, arr){
			var len = arr.length;
			document.write("<br>");
			document.write(tag + " : " + arr.toString());
		}
	</script>
</body>
</html>
```
**运行结果：**
![](https://img-blog.csdn.net/20130901162123765)


Demo5：


```python
<html>
<head>
<title>hello</title>
<style type="text/css">
#adddelTextId{
	float: clean;
}
normal {
	font-style: normal;
	color: #000000;
}
add {
	font-style: normal;
	color: #cc0000;
}
del {
	font-style: normal;
	color: #0000ff;
	text-decoration: line-through;
}
</style>
</head>
<body onload="initLoad()">
<textarea id="textareaId" name="aaa" cols="50" rows="5" >
</textarea><br />
<div>
	<div id="normalTextId" style="float:left">我,喜,欢</div>
	<add><div id="addTextId" style="float:left">,你</div></add>
	<del><div id="delTextId">,做,朋,友</div></del>
</div>
<input type="button" value="click me" onclick="hh()">
<script language="javascript">
	var textareaId = document.getElementById("textareaId");
		
	var adddelTextId = document.getElementById("adddelTextId");
	var normalTextId = document.getElementById("normalTextId");
	var addTextId = document.getElementById("addTextId");
	var delTextId = document.getElementById("delTextId");
		
	function initLoad(){
		adddelText = normalTextId.innerHTML + addTextId.innerHTML;
		textareaId.innerHTML = adddelText;
	}
	function hh(){
		adddelTextFunc(true, ",IT-Homer");
		adddelTextFunc(false, ",Sunboy_2050");
	}
	
	function adddelTextFunc(isAdd, txt){
		if(isAdd){								// add
			addTextId.innerHTML = txt;
		} else {								// del
			delTextId.innerHTML = txt;		
		}
		
		adddelText = normalTextId.innerHTML + addTextId.innerHTML;		
		textareaId.innerHTML = adddelText;
	}
</script>
</body>
</html>
```
**运行结果：**
![](https://img-blog.csdn.net/20130901174149343)


**参考推荐：**
[js 数组Array用法](http://blog.csdn.net/ithomer/article/details/8178257)
[26 个 jQuery使用技巧](http://blog.csdn.net/ithomer/article/details/8179162)

[Dynatree - Example Browser](http://wwwendt.de/tech/dynatree/doc/samples.html)
[jquery-fileTree](http://labs.abeautifulsite.net/archived/jquery-fileTree/demo/)
[zTree](http://www.ztree.me/v3/demo.php#_101)
[js 将json与String互转](http://www.cnblogs.com/mizzle/archive/2012/02/10/2345891.html)



