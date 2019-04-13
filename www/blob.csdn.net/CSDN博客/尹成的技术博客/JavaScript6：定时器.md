
# JavaScript6：定时器 - 尹成的技术博客 - CSDN博客

2018年11月14日 16:02:55[尹成](https://me.csdn.net/yincheng01)阅读数：36个人分类：[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)



\#定时器
\#\#定时器在javascript中的作用
1、制作动画
2、异步操作
3、函数缓冲与节流
定时器类型及语法
`/*
    定时器：
    setTimeout  只执行一次的定时器 
    clearTimeout 关闭只执行一次的定时器
    setInterval  反复执行的定时器
    clearInterval 关闭反复执行的定时器
*/
var time1 = setTimeout(myalert,2000);
var time2 = setInterval(myalert,2000);
/*
clearTimeout(time1);
clearInterval(time2);
*/
function myalert(){
    alert('ok!');
}`\#\#定时器制作时钟
`<script type="text/javascript">
    window.onload = function(){    
        var oDiv = document.getElementById('div1');
        function timego(){
            var now = new Date();
            var year = now.getFullYear();
            var month = now.getMonth()+1;
            var date = now.getDate();
            var week = now.getDay();
            var hour = now.getHours();
            var minute = now.getMinutes();
            var second = now.getSeconds();
            var str = '当前时间是：'+ year + '年'+month+'月'+date+'日 '+toweek(week)+' '+todou(hour)+':'+todou(minute)+':'+todou(second);
            oDiv.innerHTML = str;
        }
        timego();
        setInterval(timego,1000);
    }
    function toweek(n){
        if(n==0)
        {
            return '星期日';
        }
        else if(n==1)
        {
            return '星期一';
        }
        else if(n==2)
        {
            return '星期二';
        }
        else if(n==3)
        {
            return '星期三';
        }
        else if(n==4)
        {
            return '星期四';
        }
        else if(n==5)
        {
            return '星期五';
        }
        else
        {
            return '星期六';
        }
    }

    function todou(n){
        if(n<10)
        {
            return '0'+n;
        }
        else
        {
            return n;
        }
    }
</script>
......
<div id="div1"></div>`\#\#定时器制作倒计时
`<script type="text/javascript">
    window.onload = function(){
        var oDiv = document.getElementById('div1');
        function timeleft(){
            var now = new Date();
            var future = new Date(2016,8,12,24,0,0);
            var lefts = parseInt((future-now)/1000);
            var day = parseInt(lefts/86400);
            var hour = parseInt(lefts%86400/3600);
            var min = parseInt(lefts%86400%3600/60);
            var sec = lefts%60;
            str = '距离2016年9月12日晚24点还剩下'+day+'天'+hour+'时'+min+'分'+sec+'秒';
            oDiv.innerHTML = str; 
        }
        timeleft();
        setInterval(timeleft,1000);        
    }
</script>
......
<div id="div1"></div>`学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114143613461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

