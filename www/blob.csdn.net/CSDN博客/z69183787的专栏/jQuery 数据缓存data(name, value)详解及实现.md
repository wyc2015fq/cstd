# jQuery 数据缓存data(name, value)详解及实现 - z69183787的专栏 - CSDN博客
2015年02月03日 21:09:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1884
作为一名程序员，一提到“缓存”你很容易联想到“客户端（浏览器缓存）”和“服务器缓存”。客户端缓存是存在浏览者电脑硬盘上的，即浏览器临时文件夹，而服务器缓存是存在服务器内存中，当然在一些高级应用场合也有专门的缓存服务器，甚至有利用数据库进行缓存的实现。当然这些都不在本文的讨论范围，本文要讨论的是最流行的JavaScript框架jQuery的数据缓存实现原理，这是jQuery1.2.3版开始加入的新功能。 
一、 jQuery数据缓存的作用
jQuery数据缓存的作用在中文API中是这样描述的：“用于在一个元素上存取数据而避免了循环引用的风险”。如何理解这句话呢，看看我下面的举例，不知道合不合适，如果你有更好的例子可以告诉我。 
（1） 存在循环引用风险的例子（注意getDataByName(name)方法中的for in语句）： 
复制代码代码如下:
<a href="javascript:void(0);" onclick="showInfoByName(this.innerHTML);">Tom</a><br/> 
<a href="javascript:void(0);" onclick="showInfoByName(this.innerHTML);">Mike</a> 
<script type="text/javascript"> 
var userInfo = [ 
{ 
"name": "Tom", 
"age": 21, 
"phone": "020-12345678" 
}, 
{ 
"name": "Mike", 
"age": 23, 
"phone": "020-87654321" 
}]; 
function getDataByName(name) 
{ 
for (var i in userInfo) 
{ 
if (userInfo[i].name == name) 
{ 
return userInfo[i]; 
break; 
} 
} 
} 
function showInfoByName(name) 
{ 
var info = getDataByName(name); 
alert('name:' + info.name + '\n' + 'age:' + info.age + '\n' + 'phone:' + info.phone); 
} 
</script> 
（2） 优化循环引用风险的例子（本例子其实与jQuery缓存实现原理差不多了，本例子重点在于改写了userInfo这个JSON结构，使name与对象key直接对应）： 
复制代码代码如下:
<a href="javascript:void(0);" onclick="showInfoByName(this.innerHTML);">Tom</a><br/> 
<a href="javascript:void(0);" onclick="showInfoByName(this.innerHTML);">Mike</a> 
<script type="text/javascript"> 
var userInfo = 
{ 
"Tom": 
{ 
"name": "Tom", 
"age": 21, 
"phone": "020-12345678" 
}, 
"Mike": 
{ 
"name": "Mike", 
"age": 23, 
"phone": "020-87654321" 
} 
}; 
function showInfoByName(name) 
{ 
var info = userInfo[name]; 
alert('name:' + info.name + '\n' + 'age:' + info.age + '\n' + 'phone:' + info.phone); 
} 
</script> 
二、简单实现jQuery设置数据缓存方法 
jQuery数据缓存的实现其实是很简单的，下面我来实现jQuery设置数据缓存方法，我让代码尽量的简单，这有助于你更容易了解data的实现原理。函数与测试代码如下： 
复制代码代码如下:
<div id="div1">div1</div><br/> 
<div id="div2">div2</div> 
<script type="text/javascript"> 
//cache对象结构像这样{"uuid1":{"name1":value1,"name2":value2},"uuid2":{"name1":value1,"name2":value2}}，每个uuid对应一个elem缓存数据，每个缓存对象是可以由多个name/value对组成的，而value是可以是任何数据类型的，比如可以像这样在elem下存一个JSON片段:$(elem).data('JSON':{"name":"Tom","age":23}) 
var cache = {}; 
//expando作为elem一个新加属性，为了防止与用户自己定义的产生冲突，这里采用可变后缀 
var expando = 'jQuery' + new Date().getTime(); 
var uuid = 0; 
function data(elem, name, data) 
{ 
//至少保证要有elem和name两个参数才能进行取缓存或设置缓存操作 
if (elem && name) 
{ 
//尝试取elem标签expando属性 
var id = elem[expando]; 
if (data) 
{ 
//设置缓存数据 
if (!id) 
id = elem[expando] = ++uuid; 
//如果cache中id键对象不存在（即这个elem没有设置过数据缓存），先创建一个空对象 
if (!cache[id]) 
cache[id] = {}; 
cache[id][name] = data; 
} 
else 
{ 
//获取缓存数据 
if (!id) 
return 'Not set cache!'; 
else 
return cache[id][name]; 
} 
} 
} 
var div = document.getElementById('div1'); 
data(div, "tagName", "div"); 
data(div, "ID", "div1"); 
alert(data(div, "tagName")); //div 
alert(data(div, "ID")); //div1 
var div2 = document.getElementById('div2'); 
alert(data(div2, "tagName")); //Not set cache! 
</script> 
三、使用jQuery数据缓存注意事项
（1）因为jQuery缓存对象是全局的，在AJAX应用中，由于页面刷新很少，这个对象将一直存在，随着你对data的不断操作，很有可能因为使用不当，使得这个对象不断变大，最终影响程序性能。所以我们要及时清理这个对象，jQuery也提供了相应方法：removeData(name)，name就是你当初设置data值时使用的name参数。 
另外，根据我对jQuery代码的了解，发现下面几种情况不需要手动清除数据缓存： 
<1> 对elem执行remove()操作，jQuery会清除对象可能存在的缓存。jQuery相关源代码参考： 
复制代码代码如下:
remove:function(selector) 
{ 
if (!selector || jQuery.filter(selector, [this]).length) 
{ 
// Prevent memory leaks 
jQuery("*", this).add([this]).each(function() 
{ 
jQuery.event.remove(this); 
jQuery.removeData(this); 
}); 
if (this.parentNode) 
this.parentNode.removeChild(this); 
} 
} 
<2> 对elem执行empty()操作，如果当前elem子元素存在数据缓存，jQuery也会清除子对象可能存在的数据缓存，因为jQuery的empty()实现其实是循环调用remove()删除子元素。jQuery相关源代码参考： 
复制代码代码如下:
empty:function() 
{ 
// Remove element nodes and prevent memory leaks 
jQuery(this).children().remove(); 
// Remove any remaining nodes 
while (this.firstChild) 
this.removeChild(this.firstChild); 
} 
2、jQuery复制节点clone()方法不会复制data缓存，准确说jQuery不会在全局缓存对象中分配一个新节点存放新复制elem缓存。jQuery在clone()中把可能存在的缓存指向属性（elem的expando属性）替换成空。如果直接把这个属性复制，就会导致原先和新复制的elem都指向一个数据缓存，中间的互操作都将会影响到两个elem的缓存变量。以下jQuery代码就是把expando属性删除（jQuery1.3.2，较早版本不是这样处理，显然新版本的这个方法性能更好）。 
jQuery.clean([html.replace(/ jQuery\d+="(?:\d+|null)"/g, "").replace(/^\s*/, "")])[0]; 
把数据缓存一起复制有时候也是很有用的，比如在拖动操作中，我们点击源目标elem节点就会复制出一个半透明的elem副本开始拖动，并把data缓存复制到拖动层中，等到拖动结束，我们就可能取到当前拖动的elem相关信息。现在jQuery方法没有给我们提供这样的处理，怎么办法。第一个办法是改写jQuery代码，这个方法显然很傻，很不科学。正确做法是复制源目标的data，把这些data都重新设置到复制出来的elem中，这样在执行data(name,
 value)方法时，jQuery会在全局缓存对象中为我们开辟新空间。实现代码如下： 
复制代码代码如下:
if (typeof($.data(currentElement)) == 'number') 
{ 
var elemData = $.cache[$.data(currentElement)]; 
for (var k in elemData) 
{ 
dragingDiv.data(k, elemData[k]); 
} 
} 
在上面代码中，$.data(elem,name,data)包含三个参数，如果只有一个elem参数，这个方法返回它的缓存key（即uuid），利用这个key就可以得到整个缓存对象，然后把对象的数据都复制到新的对象。
