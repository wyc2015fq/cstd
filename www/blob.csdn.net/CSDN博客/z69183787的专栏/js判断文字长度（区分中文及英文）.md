# js判断文字长度（区分中文及英文） - z69183787的专栏 - CSDN博客
2012年10月22日 06:42:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5972
                
方法一：
```java
var len = 0;
		var v = $.trim($(this).val());
		for(i=0;i<v.length;i++)	{
			if(v.charCodeAt(i)>256)	{
				len += 3;
			} else {
				len++;
			}
		}
```
利用charCodeAt 获取该字符的unicode编码来判断，（unicode编码：例如，字符A的编码是65，字符B的编码是66，以此类推）当大于256时，说明是中文；之后通过判断服务器对1个中文的长度 来控制 前台length 长度。
```java
alert("都是a".length);
```
结果为3 可以看出js  .length  方法不区分中英文。
方法二：
```java
function isChinese(str){  //判断是不是中文
    var reCh=/[u00-uff]/;
    return !reCh.test(str);
}
function lenStat(target){
    var strlen=0; //初始定义长度为0
	var txtval = $.trim(target.val());
	for(var i=0;i<txtval.length;i++){
	 if(isChinese(txtval.charAt(i))==true){
	  strlen=strlen+2;//中文为2个字符
	 }else{
	  strlen=strlen+1;//英文一个字符
	 }
	}
	strlen=Math.ceil(strlen/2);//中英文相加除2取整数
	return strlen;
}
```
利用charAt 返回该位置的字符，利用正则表达式判断。
