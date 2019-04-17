# js的一个使用方法记录 - westbrook1998的博客 - CSDN博客





2018年03月04日 20:43:46[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：55








背景，主要要实现一个点赞功能，点击之后ajax提交，然后回调应该修改这个节点的值，但在ajax回调函数里面使用 
`$(this)`得到的是ajax的这个对象，所以解决方法是现在ajax之前用`var xxx=this;` 将这个节点保存，然后之后就可以用`$(xxx)`使用这个节点了

代码

```java
$(".zan").click(function () {
            //保存当前对象
            var _this=this;
            $.ajax({
                url:"/user/zan?id="+$(this).attr('type'),
                type:"post",
                success:function (data) {
                    alert($(_this).attr('type'))
                }
            })
        })
```



