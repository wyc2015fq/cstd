# js页面加载完后执行（document.onreadystatechange 和 document.readyState） - weixin_33985507的博客 - CSDN博客
2017年09月19日 22:05:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：23
js页面加载完后执行javascript（document.onreadystatechange 和 document.readyState） 
```java
document.onreadystatechange 页面加载状态改变时的事件
document.readyState         页面加载状态值,下面4个状态值。
uninitialized  - 还未开始载入
loading        - 载入中
interactive    - 已加载，文档与用户可以开始交互
complete       - 载入完成       
//页面加载完隐藏加载滚动条
document.onreadystatechange = function(){
    if(document.readyState=='complete'){
         $(".loading").fadeOut();
    }
}
```
