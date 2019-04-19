# JavaScript—— Json串值的获取及传递 - Big Smile - CSDN博客
2016年11月14日 00:52:19[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1139
        今天弄了一天，解决了一个非常简单的问题，可能有人会说这么简单的问题有什么好说的啊，但是不知道的人真的是不知道啊，我就是一直在换着法子在百度和CSDN上面游荡，最后终于找到了想要的结果，一起来分享一下。
        首先就是关于Ajax返回值的取出的问题，我们都知道用Ajax的一套语句去底层查询之后，返回到前端的都是JSON格式的字符串，获取其中属性对应的值是需要有所转换的，那么要怎么办呢，还好JS提供了一个对象化的方法，可以使字符串变为一个由属性和值所对应的对象，这样只需要点出属性就可以得到想要的值了。下面是代码：
```java
$.ajax({
     url: 'LoginMobile.aspx/getDate',
     data: params1,
     type: "post",
     dataType: 'text',
     contentType: "application/json; charset=utf-8",
     success: function (data) {          //data为底层返回的Json的数据
         var ev = eval('(' + data + ')');
         //用eval把data实例化为JS对象之后访问
         loginTime = ev.d;       //取得对象ev中的属性d的值                     
         alert(loginTime);
      }
});
```
        这个eval转换对象的函数还可以用作其他的地方，例如，声明一个变量为var     str=“alert(‘hello’)”   则如果我们用eval函数来建立这个str的对象，则会自动执行里面的JS代码，这样也是很方便的。
        第二个问题是对于Ajax的传递外部的值，一开始的时候用的是先给一个页面的HTML元素传递值，然后再用getelement调取值，实际发现在页面刚加载的时候这样做会导致一种不明的错误，就是在页面元素中获取不到值，而且getelement这句话要先于AJAX执行，所以没有办法了，只能上网求助，最后给出一个答案让我很是惊奇，因为之前这种做法都会返回undefined类型的值所以被抛弃不用了，现在一试竟然可以使用，不免让我有些怀疑是否是之前有别的原因导致错误。代码如上图的loginTime值，在后面可以被调用且有值。
