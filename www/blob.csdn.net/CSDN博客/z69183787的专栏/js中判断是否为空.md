# js中判断是否为空 - z69183787的专栏 - CSDN博客
2014年03月02日 10:22:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4049
有三种方法：
<script type="text/javascript">
       var x;
**if(null == x){}**
**if(typeof(x) == "undefined"){}**
**if(!x){}**
</script>
推荐第三种。
```java
$.getSN = function(options){
					if (typeof(options) == "undefined" || options== null){
						return "";
					}else if(options.length > 50){
						return options.substr(0,50);
					}else{
						return options;
					}
				}
```
