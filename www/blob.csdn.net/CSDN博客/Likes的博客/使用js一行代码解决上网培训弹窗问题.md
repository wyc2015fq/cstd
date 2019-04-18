# 使用js一行代码解决上网培训弹窗问题 - Likes的博客 - CSDN博客
2018年11月13日 14:36:37[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：71
经常遇到在线网络学习，在一段时间后会弹出对话框，要求确认后才能计入学习时间，这很麻烦。这里介绍一种比较通用的方法，通过chrome修改js代码，自动确认的目的。
1、在浏览器中按F12，进入Element选框
2、搜索弹框中的关键字“您已经在此页面5分钟了，是否继续？”，定位代码，如下所示（注意注释的代码）
```java
var secondsInit = 0;
var hdflag = 0;
var login_flag = "1";
function _fresh()
{
	secondsInit ++;
	var seconds = secondsInit;
	var result = '';
	if (seconds >= 3600)
	{
		var h = Math.floor(seconds / 3600);
		result += h + "小时";
		seconds -= 3600 * h;
	}
	if (seconds >= 60)
	{
		var m = Math.floor(seconds / 60);
		result += m + "分";
		seconds -= 60 * m;
	}
	result += seconds + "秒";
	document.getElementById('_lefttime').innerHTML = result;
	
	if((hdflag%60)==0){//每60s发送给服务器一个心跳包，待会需要自己构造发送
		$.post("exam_xuexi_online.php", {
			"cmd":'xuexi_online'
		}, function (data) {
			data = eval('('+data+')');
			if(data.status==1){
				$('#xuexi_online').html(data.shichang);
			}
		});
	}
	
	if(hdflag>300){//当计数器大于300时会弹出对话框，点击确认后会将计数器hdflag置零
		if(confirm("您已经在此页面5分钟了，是否继续？")){
			hdflag = 0;
		}
	}
	hdflag ++;
}
if(login_flag){
	_fresh()
	setInterval(_fresh,1000);
}
```
4、理清弹窗和心跳报文的基本逻辑后就可以开始修改代码。需要说明的是，上面的代码都是加载如浏览器执行的，不能够直接修改，我们所能做的只是增加新的回调函数，将hdflag更频繁地置零，这样就不会跳出对话窗口了。
点击浏览器console选框，加入如下代码（注意注释代码）：
```java
setInterval(function(){hdflag=0},60000) //防止弹窗的回调函数，每隔60000ms执行函数function，也就是将hdflag置零
var body = new FormData();//构造心跳报文
    body.append('cmd', 'xuexi_online');
var timer = setInterval(function () {//发送心跳报文的回调函数，30000ms执行一次
  fetch('http://222.197.182.137/exam_xuexi_online.php', {
    credentials: 'include',
    method: 'POST',
    body: body
  })
  //.then()表示异步执行，在当前的业务执行完成后，最后调用then中的函数，类似于c++中的asyn中的deferred方法
  //=>是一种函数简写方法，x=>x+1 表示传入参数x，在函数体中执行return x+1;
  .then(res => res.json())//res是js中固有对象，表示response，res.json()是收到消息后发送给服务器确信息（这里是空数据）
  .then(res => {//这里是执行打日志功能
    if (res.status === 'error') {
      console.error(res.msg);
    } else {
      console.log('%c' + JSON.stringify(res), 'font-size: 20px; color: green');
    }
  })}, 30000)
```
5、上面的代码，重发报文和记录日志占了很大一部分内容，最核心的代码只有一行setInterval(function(){hdflag=0},60000)
