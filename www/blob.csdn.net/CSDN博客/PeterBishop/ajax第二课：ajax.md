# ajax第二课：ajax - PeterBishop - CSDN博客





2019年01月09日 20:17:39[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：32
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









大纲：
- 简介
- 使用



这节课我们来学ajax，讲的东西有点少啊
- 简介：

AJAX = 异步 JavaScript 和 XML（Asynchronous JavaScript and XML）。

简短地说，在不重载整个网页的情况下，AJAX 通过后台加载数据，并在网页上进行显示。

本质：发http请求用的


- 使用

分为原生版和jquery版，原生版没人用，不讲，直接将jquery版

jquery版ajax使用特别简单，就是一个函数:

$.ajax();



ajax.js
document.write("<script src='jquery.js'></script>");



//jquery的ajax函数，作用是发起一个http请求

//参数是一个json对象

$.ajax({

    url:"/test",

    type:"get"

});






可以看到发起了一个名为test的请求，第三列写着xhr

注意: 凡是第三列写着xhr的全都是ajax发起的请求





可以看到请求方式是get

因为我们刚刚在$.ajax()里写了 type:”get”

看菜鸟教程属性挺多的，说几个重点的：













ajax.js
document.write("<script src='jquery.js'></script>");



//jquery的ajax函数，作用是发起一个http请求

//参数是一个json对象

$.ajax({

    url:"/test",    //请求路径

    type:"post",     //请求方式

    async:**true**, //异步

    data:{

        id:1001,

        name:"xiaoye"

    },

    //如果请求响应成功，则执行success后面跟着的函数

    //不过显然是无法响应成功的，因为我并没有开启tomcat服务器

    success:**function **(data) {



    }



});


Demo：







**如果请求响应成功，响应的信息会被保存在****success****后那个函数的第一个参数里。**



