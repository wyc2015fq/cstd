# [译]MongoDB 在语法上的 5 大缺陷 - z69183787的专栏 - CSDN博客
2016年10月26日 18:19:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1082
> 
前几天翻译了一篇MongoDB的技术文章，作者提到了几个MongoDB应用中所存在的缺陷，并且用实例来说明了几个缺陷的由来和产生情景，`这里截取了三个` 我比较想跟大家分享的，希望对大家有帮助。有兴趣可以阅读作者原文，对文章创作者予以支持。
另外就是在文章翻译完毕，发现MongoDB还有很多要说的，在这里埋个坑，有时间写几篇关于MongoDB的文章跟大家交流下。
文章作者 ：**[Slava Kim](http://devblog.me)**
原文 : **[Top 5 syntactic weirdnesses to be aware of in MongoDB](http://devblog.me/wtf-mongo)**
这几天抱怨MongoDB的帖子相当受追捧。大多是关于在特定的数据集，可靠性和分片问题上表现不佳。其中一些博客文章可能是正确的，其他的只是说,最受欢迎的NoSQL的解决方案并没有满足他们的需求。
这篇文章不是其中之一，虽然大多数的文章关注操作部分,基准测试和性能特征，而我想谈谈MongoDB查询接口。没错——编程接口，特别是关于Node.js的，但这个在不同语言平台和Mongo-shell上都差不多。
免责声明：我努力不去恨MongoDB。事实上我每个工作日都在使用MongoDB，它已经成为我全职工作的一部分。我也参与[Minimongo](https://github.com/meteor/meteor/tree/devel/packages/minimongo)的开发，使用内存缓存用纯javascript克隆MongoDB的API。我没有任何理由嘲笑Mongo只是警告大家这些意想不到的问题。他们大多数由David
 Glasser发现。本文假定您熟悉MongoDB的API。
#### 1. 哈希对象中key的顺序
比如，你要存储一个简单的文字对象：
``> db.books.insert({ title: "Woe from Wit", meta: { author: "A. Griboyedov", year: 1823 } });``
太棒了！现在我们有了一条书籍记录。再比如，以后我们会想找所有1823年出版的作者是 A. Griboyedov 的书。这里不太可能返回多个结果，但至少应该有《 Woe from Wit 》这本书，因为我们刚刚插入了这条记录，对不对？
```cpp
```cpp
> db.books.find({ meta: { year: 1823, author: "A. Griboyedov" } });
< No results returned
```
```
发生了什么？我们不是刚刚插入了这本书的数据吗？让我们尝试调换key的顺序:
```
> db.books.find({ meta: { author: "A. Griboyedov", year: 1823 } });
< { _id: ..., title: "Woe from Wit", meta: { ... } }
```
竟然可以!
**陷阱**: 在MongoDB中key的顺序非常重要，{ a: 1, b: 2 } 和 { b: 2, a: 1 }是不匹配的。
**为什么**: MongoDB使用叫做BSON的二进制数据格式。在BSON中key的顺序非常重要。注意，JSON对象是一个无序的键/值对集合。
那么在JavaScript里是怎样的呢？ECMA-262可没有规定（JS属性顺序）这件事。在某些浏览器下（通常是旧的）对属性的顺序不会太在意，这意味着它们可以是任何顺序（只要存在就行）。值得庆幸的是大多数现代浏览器的JavaScript引擎在维护JS属性的顺序（有时甚至在数组中也维护） ，因此实际上我们可以使用node.js来控制它。
更多内容请参阅[John Resig's blog](http://ejohn.org/blog/javascript-in-chrome/).
其实我们更加期望得到的答案是：要么给出规范形式（键按字典顺序排序） ，要么就让你自己的代码保持一致的。
当然，这里有其它的解决方法。使用另一种查询方法（selector），即指定那些特定的属性项（key-path），而不是比较对象的文本信息：
```matlab
> db.books.find({ 'meta.year': 1823, 'meta.author': 'A. Griboyedov' });
```
这种特殊情况下这样的查询方式是有效地，但请注意，这个查询语句的含义是不同的。
**陷阱**: 每当你想建立一个拥有多键值索引的数据的时候这种行为是很危险的。
``> db.books.ensureIndex({ title: 1, 'meta.year': -1 });``
这样的命令会使得title的优先级会比 meta.year 的优先级高。这在MongoDB中是一个很重要的分析数据的方式。[更多内容请参阅MongoDB docs](http://docs.mongodb.org/manual/core/index-multikey/).
#### 2. undefined, null and undefined
想必很多人都还记得那个undefined, null 的关系、特性很混乱的时候吧！在JavaScript的世界中undefined、null代表着两个不同的值，严格来说它们是不一样的：undefined！== NULL。当然，在非严格的情况下他们确实相等：undefined == null。有些人很小心的使用它们，而另一部分人将两者随意交替使用。说到底我们的问题是：JavaScript确实存在两个不同但很相似的值。
MongoDB的带来了它带到一个新的水平。BSON里将未定义规定为"deprecated"。 [BSON spec](http://bsonspec.org/#/specification)规定undefined为“deprecated”.
然而Node.js中的[node-native-driver for MongoDB却没有实现它](https://github.com/mongodb/js-bson/blob/master/lib/bson/bson.js#L77)。
Node.js目前的版本（2.4.8）特性表明null和undefined是两个相同的值。
```cpp
```cpp
> db.things.insert({ a: null, b: 1 });
> db.things.insert({ b: 2 }); // the 'a' is undefined implicitly
> db.things.find({ a: null });
< { a: null, b: 1 }
< { b: 2 }
```
```
我不确定node driver for MongoDB中的实现情况，不过看起来像是node driver直接将undefined转换为null，但是这在mongo-shell里是被限制的（因为在MongoDB里undefined和null本来就是两个值--译者注）。
在下面的代码中我们定义了三个对象，我们将会得到相同的结果并打印两次。
```java
```java
// from node.js code with mongo/node-native-driver
db.things.insert({ a: null, b: 1 });
db.things.insert({ b: 2 });
db.things.insert({ a: undefined, b: 3 });
console.log(db.things.find({ a: null }).fetch())
console.log(db.things.find({ a: undefined }).fetch())
```
```
然而，在mongo-shell中你只能使用null来查询，注意，我们所使用的三个对象和上面的是一样的。
```cpp
```cpp
// from mongo-shell
> db.things.find({a: undefined});
< error: { "$err" : "can't have undefined in a query expression", "code" : 13629 }
> db.things.find({a: null});
< { "a" : null, "b" : 1, "_id" : "wMWNPm7zrYXTNJpiA" }
< { "b" : 2, "_id" : "RjrYvmZF5EukhpuAY" }
< { "a" : null, "b" : 3, "_id" : "kethQ2khbyfFjJ7Sa" }
```
```
我们可以看到，mongo/node-native-driver 显式的将undefined转换null但实际上左边隐式的那个才是我们真正想要的（我们期望的真实结果）。
当我们使用mongo-shell显式的插入undefined的时候，有趣的事情发生了:
```cpp
```cpp
// from mongo-shell
> db.things.insert({ a: undefined, b: 4 });
> db.things.find({ a: null })
< { "a" : null, "b" : 1, "_id" : "wMWNPm7zrYXTNJpiA" }
< { "b" : 2, "_id" : "RjrYvmZF5EukhpuAY" }
< { "a" : null, "b" : 3, "_id" : "kethQ2khbyfFjJ7Sa" }
```
```
我们得到相同的三个值，但并没有我们刚才在mongo-shell里插入的 b=4的对象。undefined不是和null相等吗？好吧，让我们来看看这个新的对象：
```cpp
```cpp
> db.things.find({ b: 4 });
< { "_id" : ObjectId("52ca134f3e47d3d91146f2b5"), "a" : null, "b" : 4 }
```
```
它仍然在那里，虽然a属性的值很像是null，但与我们的选择器却不匹配。
**陷阱**:有2个以上的值在MongoDB中看起来像null： null，undefined以及隐式的向mongo-shell里插入的undefined，虽然看起来像null但在实际情况下和BSON（第6版）中的undefined 相匹配。最后一个在选择器上并不和null匹配，前两者都匹配undefined和null。这也说明了没有值同样可以匹配前两者。
原始问题请参阅 [GitHub issue](https://github.com/meteor/meteor/issues/1646#issuecomment-29682964).
#### 3.数组的特殊待遇
很多人并不知道这个特性，但数组确实是经过特殊处理的。
```
```
> db.c.insert({ a: [{x: 2}, {x: 3}], _id: "aaa"})
> db.c.find({'a.x': { $gt: 1 }})
< { "_id" : "aaa", "a" : [  {  "x" : 2 },  {  "x" : 3 } ] }
> db.c.find({'a.x': { $gt: 2 }})
< { "_id" : "aaa", "a" : [  {  "x" : 2 },  {  "x" : 3 } ] }
> db.c.find({'a.x': { $gt: 3 }})
< Nothing found
```
```
因此每当有一个数组对象，选择器都会“分发”给每一个元素，这就像“如果其中一个元素匹配，那么整个文档（document)都会被匹配”。
值得注意的是，它并不适用于嵌套数组：
```
```
> db.x.insert({ _id: "bbb", b: [ [{x: 0}, {x: -1}], {x: 1} ] })
> db.x.find({ 'b.x': 1 })
< { "_id" : "bbb", "b" : [  [  {  "x" : 0 },  {  "x" : -1 } ],  {  "x" : 1 } ] }
> db.x.find({ 'b.x': 0 })
< Nothing found
> db.x.find({ 'b.x': -1 })
< Nothing found
```
```
同样也适用于预测数组中字段（field）的一些特性：
```
```
> db.z.insert({a:[[{b:1,c:2},{b:2,c:4}],{b:3,c:5},[{b:4, c:9}]]})
> db.z.find({}, {'a.b': 1})
< { "_id" : ObjectId("52ca24073e47d3d91146f2b7"), "a" : [  [  {  "b" : 1 },  {  "b" : 2 } ],  {  "b" : 3 },  [  {  "b" : 4 } ] ] }
```
```
如果我们在选择器上将以上特性与使用数字键做更多的组合，那么这个特性将变得越来越难以预测：
```
```
> db.z.insert({a: [[{x: "00"}, {x: "01"}], [{x: "10"}, {x: "11"}]], _id: "zzz"})
> db.z.find({'a.x': '00'})
< Nothing found
> db.z.find({'a.x': '01'})
< Nothing found
> db.z.find({'a.x': '10'})
< Nothing found
> db.z.find({'a.x': '11'})
< Nothing found
> db.z.find({'a.0.0.x': '00'})
< { "_id" : "zzz", "a" : [     [   {   "x" : "00" },   {   "x" : "01" } ],     [   {   "x" : "10" },   {   "x" : "11" } ] ] }
> db.z.find({'a.0.0.x': '01'})
< Nothing found
> db.z.find({'a.0.x': '00'})
< { "_id" : "zzz", "a" : [     [   {   "x" : "00" },   {   "x" : "01" } ],     [   {   "x" : "10" },   {   "x" : "11" } ] ] }
> db.z.find({'a.0.x': '01'})
< { "_id" : "zzz", "a" : [     [   {   "x" : "00" },   {   "x" : "01" } ],     [   {   "x" : "10" },   {   "x" : "11" } ] ] }
> db.z.find({'a.0.x': '10'})
< Nothing found
> db.z.find({'a.0.x': '11'})
< Nothing found
> db.z.find({'a.1.x': '00'})
< Nothing found
> db.z.find({'a.1.x': '01'})
< Nothing found
> db.z.find({'a.1.x': '10'})
< { "_id" : "zzz", "a" : [     [   {   "x" : "00" },   {   "x" : "01" } ],     [   {   "x" : "10" },   {   "x" : "11" } ] ] }
> db.z.find({'a.1.x': '11'})
< { "_id" : "zzz", "a" : [ [ { "x" : "00" }, { "x" : "01" } ], [ { "x" : "10" }, { "x" : "11" } ] ] }
```
```
好的，我们再来稍作改动。这个和上一个案例的区别仅仅是内部值的改动：在上一个案例中是一个对象，在下面的案例中将会是一个数字。这足以让数组的特性发生改变：
```
```
> db.p.insert({a: [0], _id: "xxx"})
> db.p.find({'a': 0})
< { "_id" : "xxx", "a" : [  0 ] }
> db.q.insert({a: [[0]], _id: "yyy"})
> db.q.find({a: 0})
< Nothing found
> db.q.find({'a.0': 0})
< Nothing found
> db.q.find({'a.0.0': 0})
< { "_id" : "yyy", "a" : [  [  0 ] ] }
```
```
**陷阱**: 尽可能的避免数组或者嵌套数组以及其他一对多关系的数据存在于文档之中，并且在需要查询的时候，通常我们倾向于按照一对一关系去查询。然而对于使用数字键（例如{ 'a.0.x': Y }意味着字段a的第一个元素的x字段必须为Y）的混合型文档很可能会让人感觉非常别扭，当然这也取决于数据的复杂程度。
由此，在我开始使用Javascript编程的时候这些陷阱给我提了个醒。这里有一些我们平时不容易察觉的情况，其中一些诸如：跨浏览器导致结果不一致，还有一些你几乎怎么都用不到的特性却突然要拿来使用，因此你要格外小心。有些是众所周知的JavaScript领域中的问题，但在MongoDB中也没有处理的那么好。
几乎所有怪异的特性我们都在模拟实现MongoDB的过程中发现，然后整理并列举在这里，这个模拟MongoDB的项目叫做[Minimongo](https://github.com/meteor/meteor/tree/devel/packages/minimongo), 主要由
[David Glasser](https://twitter.com/glasser)贡献.
文／Terry_Wang（简书作者）
原文链接：http://www.jianshu.com/p/7c530fae9540
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
