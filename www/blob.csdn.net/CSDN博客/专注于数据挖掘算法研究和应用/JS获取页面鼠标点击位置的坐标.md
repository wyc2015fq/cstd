# JS获取页面鼠标点击位置的坐标 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年06月13日 15:34:49[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5654








本来想通过JS实现当前页面对其他页面的操作，在网上发现了这段js代码，先保存下来，可以获取页面鼠标点击位置的坐标。



```java
<html>
<body>
<script>
function imitateClick(oElement,iClientX,iClientY){
	var oEvent;
	if (document.createEventObject) {
		oEvent=document.createEventObject();
		oEvent.clientX=iClientX;
		oEvent.clientY=iClientY;
		oElement.fireEvent("onclick",oEvent);
	}else{
		oEvent=document.createEvent("MouseEvents");
		oEvent.initMouseEvent("click",true,true,document.defaultView,0,0,0,iClientX,iClientY);
		oElement.dispatchEvent(oEvent);
	}
}
var body=document.body;
body.onclick=function(event){
	alert("clicked at("+event.clientX+","+event.clientY+")");
};
imitateClick(body,100,100);
</script>
</body>
</html>
```





