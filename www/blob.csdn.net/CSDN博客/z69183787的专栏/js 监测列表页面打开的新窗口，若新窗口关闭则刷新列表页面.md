# js 监测列表页面打开的新窗口，若新窗口关闭则刷新列表页面 - z69183787的专栏 - CSDN博客
2013年01月16日 13:23:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1266
思想：利用setTimeInterval 监控 窗口对象
```java
rtn = window.open(url, 'w' + task_id);
intHand=setInterval("checkWin()",3000);
```

```java
var intHand=null;
var rtn=null;	
	function checkWin(){
		if(rtn!=null && rtn.closed){
			clearInterval(intHand);
			intHand=null;
			rtn=null;
			//findDbx("");
			//getJrswDbx($("#oldDept").val());
		}
	}
```
