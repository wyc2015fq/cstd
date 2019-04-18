# EXCEPTION-JS - weixin_33985507的博客 - CSDN博客
2017年03月09日 11:16:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5

CreateTime--2016年11月22日13:00:55
Author:Marydon
声明：异常类文章主要是记录了我遇到的异常信息及解决方案，解决方案大部分都是百度解决的，（这里只是针对我遇到的做个汇总），特此声明！
js异常总结
异常一
　　onload为null
	　　window.onload
		　　一个页面只能有一个window.onload，当页面中有多个onload事件时，最后一个onload事件会执行，前面的多个onload事件都不会执行，js断点调试显示结果为：onload为null
		　　表现形式一：
			　　onload事件写在js里
```
<script>
    window.onload = function () {
        "页面所有元素加载完毕要执行的内容"
    }    
</script>
```
 　　表现形式二：
			　　　　onload事件写在body标签上
```
<script>
    function demo () {
        "页面所有元素加载完毕要执行的内容"
    }
</script>
<body onload="demo()">
</body>
```
　　举例:
```
<body>
    <script>
        num = 1;
        window.onload = function ()    {
            num = 2;//函数内部变量不声明var时，num代表的是全局变量
        }
        window.onload = function ()    {
            num = 3;
        }
    </script>
</body>
```
 　　测试结果：
				　　　　js断点调试显示结果为：第一个onload的值为null，最终num = 3
		　　误区：
有且只有这两种表现形式，其他的HTML标签上添加onload事件无效（比如：div）
	　　　　jQuery的页面加载完毕事件可以有多个
		　　　　　　$(function(){
			　　　　　　　　"页面加载完毕要执行的内容"（执行时间优先于图片信息等）
		　　　　　　});
异常二
　　"loadData"未定义
```
function PersonmultiQuery() {
    var object = this;    
    this.search=function search() {
        var fmcard=$get("FMCARD").value.trim();
        var fjcmyear=$get("FJCMYEAR").value;
        if(fmcard==""){
        Dialog.Alert("消息提示",'请输入参保人员身份证号！',function(){
            $get("FMCARD").focus();
        },null,50);
        return false;
        }    
        if(fjcmyear==""){
        Dialog.Alert("消息提示",'参保年度不能为空！',function(){
            $get("FJCMYEAR").focus();
        },null,50);
        return false;
        }
        loadData(1);//改为object.loadData(1);
        $get("SearchButton").disabled=true;
    };
        
    this.loadData=function loadData(pageIndex) {        
        $get("PageIndex").value=pageIndex;                
        Form_JsonSubmit("IndexForm");
    };
}
```
 　　解释：
从属于对象的函数之间的相互调用，需加上"this",由于this会因为使用情景的不同而随着调用的对象不同而变化，所以为防止this所指的对象发生变化，在函数第一行将this赋值给另一个变量var object = this;
