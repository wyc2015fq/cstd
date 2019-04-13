
# JavaScript4：函数和闭包 - 尹成的技术博客 - CSDN博客

2018年11月14日 16:04:14[尹成](https://me.csdn.net/yincheng01)阅读数：38个人分类：[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)



\#函数
函数就是重复执行的代码片。
\#\#函数定义与执行
`<script type="text/javascript">
    // 函数定义
    function aa(){
        alert('hello!');
    }
    // 函数执行
    aa();
</script>`\#\#变量与函数预解析
JavaScript解析过程分为两个阶段，先是编译阶段，然后执行阶段，在编译阶段会将function定义的函数提前，并且将var定义的变量声明提前，将它赋值为undefined。
`<script type="text/javascript">    
    aa();       // 弹出 hello！
    alert(bb);  // 弹出 undefined
    function aa(){
        alert('hello!');
    }
    var bb = 123;
</script>`\#\#提取行间事件
在html行间调用的事件可以提取到javascript中调用，从而做到结构与行为分离。
`<!--行间事件调用函数   -->
<script type="text/javascript">        
    function myalert(){
        alert('ok!');
    }
</script>
......
<input type="button" name="" value="弹出" onclick="myalert()">
<!-- 提取行间事件 -->
<script type="text/javascript">
window.onload = function(){
    var oBtn = document.getElementById('btn1');
    oBtn.onclick = myalert;
    function myalert(){
        alert('ok!');
    }
}    
</script>
......
<input type="button" name="" value="弹出" id="btn1">`\#\#匿名函数
定义的函数可以不给名称，这个叫做匿名函数，可以将匿名函数直接赋值给元素绑定的事件来完成匿名函数的调用。
`<script type="text/javascript">
window.onload = function(){
    var oBtn = document.getElementById('btn1');
    /*
    oBtn.onclick = myalert;
    function myalert(){
        alert('ok!');
    }
    */
    // 直接将匿名函数赋值给绑定的事件
    oBtn.onclick = function (){
        alert('ok!');
    }
}
</script>`\#\#函数传参
`<script type="text/javascript">
    function myalert(a){
        alert(a);
    }
    myalert(12345);
</script>`\#\#函数’return’关键字
函数中’return’关键字的作用：
1、返回函数执行的结果
2、结束函数的运行
3、阻止默认行为
`<script type="text/javascript">
function add(a,b){
    var c = a + b;
    return c;
    alert('here!');
}
var d = add(3,4);
alert(d);  //弹出7
</script>`\#封闭函数
封闭函数是javascript中匿名函数的另外一种写法，创建一个一开始就执行而不用命名的函数。
一般定义的函数和执行函数：
`function changecolor(){
    var oDiv = document.getElementById('div1');
    oDiv.style.color = 'red';
}
changecolor();`封闭函数：
`(function(){
    var oDiv = document.getElementById('div1');
    oDiv.style.color = 'red';
})();`还可以在函数定义前加上“~”和“!”等符号来定义匿名函数
`!function(){
    var oDiv = document.getElementById('div1');
    oDiv.style.color = 'red';
}()`\#闭包
\#\#什么是闭包
函数嵌套函数，内部函数可以引用外部函数的参数和变量，参数和变量不会被垃圾回收机制收回
`function aaa(a){      
      var b = 5;      
      function bbb(){
           a++;
           b++;
         alert(a);
         alert(b);
      }
      return bbb;
  }
 var ccc = aaa(2);
 ccc();
 ccc();`改写成封闭函数的形式：
`var ccc = (function(a){
  var b = 5;
  function bbb(){
       a++;
       b++;
     alert(a);
     alert(b);
  }
  return bbb;
})(2);
ccc();
ccc();`\#\#用处
1、将一个变量长期驻扎在内存当中，可用于循环中存索引值
`<script type="text/javascript">
    window.onload = function(){
        var aLi = document.getElementsByTagName('li');
        for(var i=0;i<aLi.length;i++)
        {
            (function(i){
                aLi[i].onclick = function(){
                    alert(i);
                }
            })(i);
        }
    }
</script>
......
<ul>
    <li>111</li>
    <li>222</li>
    <li>333</li>
    <li>444</li>
    <li>555</li>
</ul>`2、私有变量计数器，外部无法访问，避免全局变量的污染
`<script type="text/javascript">
var count = (function(){
    var a = 0;
    function add(){
        a++;
        return a;
    }
    return add;
})()
count();
count();
var nowcount = count();
alert(nowcount);
</script>`学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114143613461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

