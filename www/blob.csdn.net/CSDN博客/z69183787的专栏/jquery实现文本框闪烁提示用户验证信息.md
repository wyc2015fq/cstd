# jquery实现文本框闪烁提示用户验证信息 - z69183787的专栏 - CSDN博客
2012年10月24日 16:35:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4553
```java
function normal(id,times)  
		{  
		    var obj=$("#"+id);  
		    obj.css("background-color","#FFF");  
		    if(times<0)  
		    {  
		        return;  
		    }  
		    times=times-1;  
		    setTimeout("error('"+id+"',"+times+")",150);  
		}  
	function error(id,times)  
		{  
		    var obj=$("#"+id);  
		    obj.css("background-color","#F6CECE");  
		    times=times-1;  
		    setTimeout("normal('"+id+"',"+times+")",150);  
		}
```
其中id为dom元素id，times为闪烁次数
