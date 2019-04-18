# js对象 - weixin_33985507的博客 - CSDN博客
2017年09月04日 18:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
对象就是一种无序的数据集合，若干个“键值对”（key-value）构成。
```
var obj = {
  p: 'hello world'
};
```
key值value值之间用冒号，每个键值之间用逗号隔开。
```
{key : value}   //是JS对象字面量写法。
一般是如此生成对象的：
Object()               //  生成了一个{}
var  obj = Object()         // obj = {}
o    = {}              //    生成了一个{}
```
大括号的形式就是一种字面量的写法，简单啊。包括后面学数组时，用[]表示，也是如此，简单啊。
# 基本使用
```
var company = {
  name : 'jirengu'
}
company                      //   出现了{name : 'jirengu'},类型显示Object。
var company = {
    name : 'jirengu',
  age : 3,
  sayHello : function() {
        console.log ('hello world')
       }
}
 company.name                  // "jirengu"
company.age                        //  3
company.sayHello()                //hello world  ,这里是函数，要执行的话，后面加括号哦。
获取目标key的value值除了这种还有一种——
company['name']                   // "jirengu"
千万不要这样写：company[name],这个意思是把name当成一个变量了，
如果变量的属性是a，就是获取a了。实际上这里的变量只有company,
数据里的只有这个变量的属性也就是key的value值了。
```
var o = {
0.7 : 'hello'
}
o[0.7]       //  ' hello'     属性是数值的话，比较特殊，可以加引号，或不加。
0['0.7']        // 'hello'
```
上面的用法就是查看的用法，一共两种写法，关乎语法的意思了，记住就行。下面看看如何赋值给相应的key?
company.addr = '杭州市'
compny['business'] = '前端课程'
company                                      // 查看就有了这些新增的key-value了。
用for循环遍历对象里的每个属性，把key当成变量了。
for (var key in company) {
  console.log(key)
     // console.log(company[key])
}
先把console.log(company[key])注释掉，再运行，得到
name  age sayHello addr business
这时候，key就是变量，变量里存的就是key对应的名称的字符串，name等等。
这时候，字符串是变量的话，就可以这样：
company[key]  === company['name'] 
当然这时候company.key的意思还是获取属性是key的值，是错的写法。
```
# 键名
也就是key，加不加''都可以的，但是一些特殊字符，比如两个单词，有空格的，加。
如果键名是数值的话，自动转化成字符串，因为属性必须是字符串。
如果键名不符合标识符的命名条件（比如第一个字符是数字，或有空格或运算符），也不是数字，就必须加上引号，否则报错。
```
var o = {
  '1p' : 'hello world',
  'h b': 'hello',
  'p+q' : 'hello'
}
o ['h b']                       // 访问也要加引号了。
o .h b                              //报错了。
o.'h b'                            //报错了。
```
JS的保留字可以不加引号当key的，比如for ,class。
# 属性
属性property就是键名，每个属性的值可以是任何的类型。一个属性值是函数，把这个属性称方法。
```
var o = {
  p : function(x) {
    return 2*x;
 }
}
o.p(1)                             // 2
```
属性可以动态创建的：
```
var obj = {}
obj.foo = 123
obj.foo                    // 123       创建，然后赋值。
```
# 对象的引用
意思就是不同的变量同时引用同一个对象，指向同一个堆内存，如果操作一个变量，然后改变了堆内存数据，其他变量都同步改变了哦。
# 表达式还是语句？
```
{foo : 123}  
{foo :123}.foo           //报错了。
```
直接写这代码，可以是表达式，有foo属性的对象。也可以是语句，表示一个代码区块，foo是标签，123是表达式。
为了避免歧义，JS规定，行首大括号一律是语句，是代码块 ,也就是说上面的在浏览器看来就是123。要说表达式，也就是对象，必须用括号包裹：
```
({foo : 123}).foo              //  123，括号可以把里面的东西转化为表达式。
```
- eval
```
eval('console.log(123)')         // 123  ,eval可以把一个字符串当成JS语句去执行。
eval ('{foo:123}')        // 123。把{foo:123}当成JS执行，如果当成对象的话，怎么会是123呢？
eval('({foo；123})')     // {foo；123}。 同上，结果是对象，所以，加括号就当成对象了。
```
把字符串当成对象去执行，用eval。有风险，后续会讲，现在不用了。
# 检测某个变量是否声明
```
'abc' in window                             //false
var hello
'hello' in window                     //true
```
# 查看所有属性
```
var o = {
  key1 : 1,
  key2 : 2
}
Object.keys(o)        //['key1','key2'],数组形式哦。
```
# delete命令
删除对象的属性，成功后返回true。
```
var o = {p:1}
  Object.keys(o)              // ['p']
delete o.p                       //true
o.p                                 //undefined
Object.keys(o)                  //[  ]
```
