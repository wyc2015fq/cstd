
# Underscore简介 - Magician的博客 - CSDN博客


2017年09月13日 08:56:30[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：212标签：[underscore 简介																](https://so.csdn.net/so/search/s.do?q=underscore 简介&t=blog)个人分类：[其他语言																](https://blog.csdn.net/qq_16633405/article/category/8701162)



**5. Underscore.js**
**Underscore封装了常用的JavaScript对象操作方法，用于提高开发效率。**它本身与我们介绍的主题“Backbone”没有半毛钱的关系，因此你可以完全不理会“Backbone”的概念来学习它，或将它单独运用到任何一个页面。（另外，Underscore还可以被使用在Node.js运行环境。）
在学习Underscore之前，你应该先保存它的API地址，因为你将在以后经常访问它：
http://documentcloud.github.com/underscore/
从API中，你已经可以看出，Underscore没有任何复杂的结构和流程，它仅仅提供了一系列常用的函数。如果你将API中的方法从头至尾用一遍，你就会对它非常了解。
尽管如此，但我觉得还是有必要将一些重要的方法拿出来与大家讨论，它们十分重要，却在API中描述地还不够清楚。
## 5.1 Underscore对象封装
Underscore并没有在原生的JavaScript对象原型中进行扩展，而是像jQuery一样，将数据封装在一个自定义对象中（下文中称“Underscore对象”）。
你可以通过调用一个Underscore对象的value()方法来获取原生的JavaScript数据，例如：
[javascript][view plain](http://blog.csdn.net/likun557/article/details/53161987#)[copy](http://blog.csdn.net/likun557/article/details/53161987#)
// 定义一个JavaScript内置对象
varjsData = {
name :'data'
}
// 通过_()方法将对象创建为一个Underscore对象
// underscoreData对象的原型中包含了Underscore中定义的所有方法，你可以任意使用
varunderscoreData = _(jsData);
// 通过value方法获取原生数据, 即jsData
underscoreData.value();
## 5.2 优先调用JavaScript 1.6内置方法
Underscore中有许多方法在JavaScript1.6中已经被纳入规范，因此在Underscore对象内部，会优先调用宿主环境提供的内置方法（如果宿主环境已经实现了这些方法），以此提高函数的执行效率。
而对于不支持JavaScript 1.6的宿主环境，Underscore会通过自己的方式实现，而对开发者来说，这些完全是透明的。
这里所说的宿主环境，可能是Node.js运行环境，或客户端浏览器。
## 5.3 改变命名空间
Underscore默认使用_（下划线）来访问和创建对象，但这个名字可能不符合我们的命名规范，或容易引起命名冲突。
我们可以通过noConflict()方法来改变Underscore的命名，并恢复_（下划线）变量之前的值，例如：
[html][view plain](http://blog.csdn.net/likun557/article/details/53161987#)[copy](http://blog.csdn.net/likun557/article/details/53161987#)
<scripttype="text/javascript">
var_='自定义变量';
</script>
<scripttype="text/javascript"src="underscore/underscore-min.js"></script>
<scripttype="text/javascript">
// Underscore对象
console.dir(_);
// 将Underscore对象重命名为us, 后面都通过us来访问和创建Underscore对象
varus=_.noConflict();
// 输出"自定义变量"
console.dir(_);
</script>
**5.4 链式操作**
还记得我们在jQuery中是如何进行链接操作吗？例如：
$('a').css('position', 'relative').attr('href', '\#').show();
Underscore同样支持链式操作，但你需要先调用chain()方法进行声明：
[javascript][view plain](http://blog.csdn.net/likun557/article/details/53161987#)[copy](http://blog.csdn.net/likun557/article/details/53161987#)
vararr = [10, 20, 30];
_(arr)
.chain()
.map(function(item) {
returnitem++;
})
.first()
.value();
如果调用了chain()方法，Underscore会将所调用的方法封装在一个闭包内，并将返回值封装为一个Underscore对象并返回：
[javascript][view plain](http://blog.csdn.net/likun557/article/details/53161987#)[copy](http://blog.csdn.net/likun557/article/details/53161987#)
// 这是Underscore中实现链式操作的关键函数，它将返回值封装为一个新的Underscore对象，并再次调用chain()方法，为方法链中的下一个函数提供支持。
varresult =function(obj, chain) {
returnchain ? _(obj).chain() : obj;
}
## 5.5 扩展Underscore
我们可以通过mixin()方法轻松地向Underscore中扩展自定义方法，例如：
[javascript][view plain](http://blog.csdn.net/likun557/article/details/53161987#)[copy](http://blog.csdn.net/likun557/article/details/53161987#)
_.mixin({
method1:function(object) {
// todo
},
method2:function(arr) {
// todo
},
method3:function(fn) {
// todo
}
});
这些方法被追加到Underscore的原型对象中，所有创建的Underscore对象都可以使用这些方法，它们享有和其它方法同样的环境。
## 5.6 遍历集合
each()和map()方法是最常用用到的两个方法，它们用于迭代一个集合（数组或对象），并依次处理集合中的每一个元素，例如：
[javascript][view plain](http://blog.csdn.net/likun557/article/details/53161987#)[copy](http://blog.csdn.net/likun557/article/details/53161987#)
vararr = [1, 2, 3];
_(arr).map(function(item, i) {
arr[i] = item + 1;
});
varobj = {
first: 1,
second: 2
}
_(obj).each(function(value, key) {
returnobj[key] = value + 1;
});
map()方法与each()方法的作用、参数相同，但它会将每次迭代函数返回的结果记录到一个新的数组并返回。
## 5.7 函数节流
函数节流是指控制一个函数的执行频率或间隔（就像控制水流的闸门一样），Underscore提供了debounce()和throttle()两个方法用于函数节流。
为了更清楚地描述这两个方法，假设我们需要实现两个需求：
需求1：当用户在文本框输入搜索条件时，自动查询匹配的关键字并提示给用户（就像在Tmall输入搜索关键字时那样）
首先分析第1个需求，我们可以绑定文本框的keypress事件，当输入框内容发生变化时，查询匹配关键字并展示。假设我想查询“windows phone”，它包含13个字符，而我输入完成只花了1秒钟（好像有点快，就意思意思吧），那么在这1秒内，调用了13次查询方法。这是一件非常恐怖的事情，如果Tmall也这样实现，我担心它会不会在光棍节到来之前就挂掉了（当然，它并没有这么脆弱，但这绝对不是最好的方案）
更好的方法是，我们希望用户已经输入完成，或者正在等待提示（也许他懒得再输入后面的内容）的时候，再查询匹配关键字。
最后我们发现，在我们期望的这两种情况下，用户会暂时停止输入，于是我们决定在用户暂停输入200毫秒后再进行查询（如果用户在不断地输入内容，那么我们认为他可能很明确自己想要的关键字，所以等一等再提示他）
这时，利用Underscore中的debounce()函数，我们可以轻松实现这个需求：
[html][view plain](http://blog.csdn.net/likun557/article/details/53161987#)[copy](http://blog.csdn.net/likun557/article/details/53161987#)
<inputtype="text"id="search"name="search"/>
<scripttype="text/javascript">
varquery=_(function() {
// 在这里进行查询操作
}).debounce(200);
$('\#search').bind('keypress', query);
</script>
你能看到，我们的代码非常简洁，节流控制在debounce()方法中已经被实现，我们只告诉它当query函数在200毫秒内没有被调用过的话，就执行我们的查询操作，然后再将query函数绑定到输入框的keypress事件。
query函数是怎么来的？我们在调用debounce()方法时，会传递一个执行查询操作的函数和一个时间（毫秒数），debounce()方法会根据我们传递的时间对函数进行节流控制，并返回一个新的函数（即query函数），我们可以放心大胆地调用query函数，而debounce()方法会按要求帮我们做好控制。
需求2：当用户拖动浏览器滚动条时，调用服务器接口检查是否有新的内容
再来分析第2个需求，我们可以将查询方法绑定到window.onscroll事件，但这显然不是一个好的做法，因为用户拖动一次滚动条可能会触发几十次甚至上百次onscroll事件。
我们是否可以使用上面的debounce()方法来进行节流控制？当用户拖动滚动条完毕后，再查询新的内容？但这与需求不符，用户希望在拖动的过程中也能看到新内容的变化。
因此我们决定这样做：用户在拖动时，每两次查询的间隔不少于500毫秒，如果用户拖动了1秒钟，这可能会触发200次onscroll事件，但我们最多只进行2次查询。
利用Underscore中的throttle()方法，我们也可以轻松实现这个需求：
[html][view plain](http://blog.csdn.net/likun557/article/details/53161987#)[copy](http://blog.csdn.net/likun557/article/details/53161987#)
<scripttype="text/javascript">
varquery=_(function() {
// 在这里进行查询操作
}).throttle(500);
$(window).bind('scroll', query);
</script>
代码仍然十分简洁，因为在throttle()方法内部，已经为我们实现的所有控制。
你可能已经发现，debounce()和throttle()两个方法非常相似（包括调用方式和返回值），作用却又有不同。
它们都是用于函数节流，控制函数不被频繁地调用，节省客户端及服务器资源。
debounce()方法关注函数执行的间隔，即函数两次的调用时间不能小于指定时间。
throttle()方法更关注函数的执行频率，即在指定频率内函数只会被调用一次。
5.8 模板解析
Underscore提供了一个轻量级的模板解析函数，它可以帮助我们有效地组织页面结构和逻辑。
我将通过一个例子来介绍它：
[html][view plain](http://blog.csdn.net/likun557/article/details/53161987#)[copy](http://blog.csdn.net/likun557/article/details/53161987#)
<!-- 用于显示渲染后的标签 -->
<ulid="element"></ul>
<!-- 定义模板，将模板内容放到一个script标签中 -->
<scripttype="text/template"id="tpl">
<% for(vari=0; i<list.length; i++) { %>
<% varitem=list[i] %>
<li>
<span><%=item.firstName%><%=item.lastName%></span>
<span><%-item.city%></span>
</li>
<% } %>
</script>
<scripttype="text/javascript"src="underscore/underscore-min.js"></script>
<scripttype="text/javascript">
// 获取渲染元素和模板内容
varelement= $('\#element'),
tpl= $('\#tpl').html();
// 创建数据, 这些数据可能是你从服务器获取的
vardata= {
list: [
{firstName: '<ahref="\#">Zhang</a>', lastName: 'San', city: 'Shanghai'},
{firstName: 'Li', lastName: 'Si', city: '<ahref="\#">Beijing</a>'},
{firstName: 'Wang', lastName: 'Wu', city: 'Guangzhou'},
{firstName: 'Zhao', lastName: 'Liu', city: 'Shenzhen'}
]
}
// 解析模板, 返回解析后的内容
varhtml=_.template(tpl, data);
// 将解析后的内容填充到渲染元素
element.html(html);
</script>
在本例中，我们将模板内容放到一个<script>标签中，你可能已经注意到标签的type是text/template而不是text/javascript，因为它无法作为JavaScript脚本直接运行。
我也建议你将模板内容放在<script>中，因为如果你将它们写在一个<div>或其它标签中，它们可能会被添加到DOM树中进行解析（即使你隐藏了这个标签也无法避免）。
_.template模板函数只能解析3种模板标签（这比Smarty、JSTL要简单得多）：
<%  %>：用于包含JavaScript代码，这些代码将在渲染数据时被执行。
<%= %>：用于输出数据，可以是一个变量、某个对象的属性、或函数调用（将输出函数的返回值）。
<%- %>：用于输出数据，同时会将数据中包含的HTML字符转换为实体形式（例如它会将双引号转换为&quot;形式），用于避免XSS攻击。
当我们希望将数据中的HTML作为文本显示出来时，常常会使用<%- %>标签。
Underscore还允许你修改这3种标签的形式，如果我们想使用{% %}、{%= %}、{%- %}作为标签，可以通过修改templateSettings来实现，就像这样：
[javascript][view plain](http://blog.csdn.net/likun557/article/details/53161987#)[copy](http://blog.csdn.net/likun557/article/details/53161987#)
_.templateSettings = {
evaluate: /\{%([\s\S]+?)\%\}/g,
interpolate: /\{%=([\s\S]+?)\%\}/g,
escape: /\{%-([\s\S]+?)%\}/g
}
在本例中，我们将模板内容和需要填充的数据传递给template方法，它会按以下顺序进行处理：
将模板内容解析为可执行的JavaScript（解析模板标签）
通过with语句将解析后的JavaScript作用域修改为我们传递的数据对象，这使我们能够直接在模板中通过变量形式访问数据对象的属性
执行解析后的JavaScript（将数据填充到模板）
返回执行后的结果
我们经常会遇到一种情况：多次调用template方法将数据渲染到同一个模板。
假设我们有一个分页列表，列表中的每一条数据都通过模板渲染，当用户进入下一页，我们会获取下一页的数据并重新渲染，实际上每次渲染的模板都是同一个，但刚才描述的template所有处理过程总会被执行。
其实Underscore的template方法提供了一种更高效的调用方式，我们将上面代码中的最后两句修改为：
[javascript][view plain](http://blog.csdn.net/likun557/article/details/53161987#)[copy](http://blog.csdn.net/likun557/article/details/53161987#)
// 解析模板, 返回解析后的内容
varrender = _.template(tpl);
varhtml = render(data);
// 将解析后的内容填充到渲染元素
element.html(html);
你会发现细微的差别：我们在调用template方法时只传递了模板内容，而没有传递数据，此时template方法会解析模板内容，生成解析后的可执行JavaScript代码，并返回一个函数，而函数体就是解析后的JavaScript，因此当我们调用该函数渲染数据时，就省去了模板解析的动作。
你应该将返回的函数存储起来（就像我将它存储在render变量中一样），再通过调用该函数来渲染数据，特别是在同一个模板可能会被多次渲染的情况下，这样做能提高执行效率（具体提升多少，应该根据你的模板长度和复杂度而定，但无论如何，这都是一个良好的习惯）。

