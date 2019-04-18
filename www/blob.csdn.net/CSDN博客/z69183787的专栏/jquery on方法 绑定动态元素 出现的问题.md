# jquery on方法 绑定动态元素 出现的问题 - z69183787的专栏 - CSDN博客
2013年12月17日 16:18:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：32611
之前使用 on 的时候一直是
```java
$("").on('click','function(){
}')
```
之后发现有些时候一直无法绑定（比如元素动态生成时），查看文档后发现正确用法应该是
```java
$(document).on("change","#pageSize_out",function(){
        	if($("#page_out").val()!=0){
	        	$("#pageSize").val($(this).val());
	        	list();
         	}
        })
```
同时，注意
As this answers receives a lot of attention, here are two supplementary advises :
1) When it's possible, try to bind the event listener to the most precise element, to avoid useless event handling.
That is, if you're adding an element of class `b` to
 an existing element of id `a`,
 then don't use
`$(document.body).on('click', '#a .b', function(){`
but use
`$('#a').on('click', '.b', function(){`
2) Be careful, when you add an element with an id, to ensure you're not adding it twice. Not only is it "illegal" in HTML to have two elements with
 the same id but it breaks a lot of things. For example a selector `"#c"` would
 retrieve only one element with this id.
**on(events,[selector],[data],fn)**
events:一个或多个用空格分隔的事件类型和可选的命名空间，如"click"或"keydown.myPlugin" 。
selector:一个选择器字符串用于过滤器的触发事件的选择器元素的后代。如果选择器为null或省略，当它到达选定的元素，事件总是触发。
data:当一个事件被触发时要传递event.data给事件处理函数。
fn:该事件被触发时执行的函数。 false 值也可以做一个函数的简写，返回false。
## 替换bind()
当第二个参数'selector'为null时，on()和bind()其实在用法上基本上没有任何区别了，所以我们可以认为on()只是比bind()多了一个可选的'selector'参数，所以on()可以非常方便的换掉bind()
## 替换live()
在1.4之前相信大家非常喜欢使用live(),因为它可以把事件绑定到当前以及以后添加的元素上面，当然在1.4之后delegate()也可以做类似的事情了。live()的原理很简单，它是通过document进行事件委派的，因此我们也可以使用on()通过将事件绑定到document来达到live()一样的效果。
**live()写法**
复制代码代码如下:
 $('#list li').live('click', '#list li', function() {
    //function code here.
}); 
on()写法
复制代码代码如下:
$(document).on('click', '#list li', function() {
    //function code here.
});
这里的关键就是第二个参数'selector'在起作用了。它是一个过滤器的作用，只有被选中元素的后代元素才会触发事件。
**替换delegate()**delegate()是1.4引入的，目的是通过祖先元素来代理委派后代元素的事件绑定问题，某种程度上和live()优点相似。只不过live()是通过document元素委派，而delegate则可以是任意的祖先节点。使用on()实现代理的写法和delegate()基本一致。
**delegate()的写法**
复制代码代码如下:
$('#list').delegate('li', 'click', function() {
    //function code here.
});
on()写法
复制代码代码如下:
$('#list').on('click', 'li', function() {
    //function code here.
});
貌似第一个和第二个参数的顺序颠倒了一下，别的基本一样。
**总结**jQuery推出on()的目的有2个，一是为了统一接口，二是为了提高性能，所以从现在开始用on()替换bind(), live(), delegate吧。尤其是不要再用live()了，因为它已经处于不推荐使用列表了，随时会被干掉。如果只绑定一次事件，那接着用one()吧，这个没有变化。
