# 基于jquery的显示插件 jquery.qtip - z69183787的专栏 - CSDN博客
2012年10月24日 16:07:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：911
所需JS：jquery.qtip-1.0.0-rc3.js
可以用来提示信息，比如某些文本框不满足条件，做出醒目标识及提示。
$(target)为某个dom元素,error为显示的内容
```java
/**/
		$(target).qtip({
			content:error,
			position: {
                corner: {
                   tooltip: "topLeft", 
                   target: "bottomLeft" 
                }
             },
             show: {
                //when: true, 
                //ready: true 
             },
             //hide: false, 
             style: {
                border: {
                   width: 2,
                   radius: 3
                },
                padding: 10, 
                textAlign: 'center',
                tip: true, 
                name: 'red' 
             }
		});
```
效果如下：
![](https://img-my.csdn.net/uploads/201210/24/1351066323_3489.jpg)
