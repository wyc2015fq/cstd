# JavaScript面向对象程序设计—创建对象的模式 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年09月13日 08:50:35[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：427
JS本身为我们提供了`Array`、`Date`、`Math`等不少对象（见[《浅析JavaScript的对象系统》](http://www.jianshu.com/p/d0930dc0f95d)），但在实际开发中我们使用最多的还是自定义对象。自定义对象是一门值得研究的学问。往浅了讲，它关系到代码量、封装性、代码是否优雅；往深了讲，它又涉及到内存开销、设计模式乃至JavaScript语言的核心。下面就一起循序渐进地看看如何更好地创建一个对象。
##### 1. 基本模式
```java
```java
var person = new Object();    //或 var person = {};
person.name = "Chuck"; 
person.age = 25;
person.job = "Software Engineer";  
person.sayName = function(){     
    alert(this.name); 
};
```
```
这是我们创建一个对象常用的方式，写起来很自然也很顺手，但是你不觉得连着写5个`person`有些麻烦吗？所以我们想到可以使用 *对象字面量* 语法进行改进：
```java
```java
var person = {
    name: 'Chuck',
    age: '25',
    job: 'Software Engineer',
    sayName: function(){
        alert(this.name);
    }
};
```
```
这样是不是好多了？是的，字面量语法真是个好东西！的确，使用对象字面量形式创建对象是非常常用也是我们很喜欢的一种方式，它在只需要单独定义几个对象的时候会很方便。可问题在于，当我想要创建很多很多和上面的`person`拥有相同结构的对象时，这种方案就不太好了，难道要让我每创建一个对象就要敲一遍`name``age``job``sayName`吗？这显然不现实，会产生大量的重复代码。
##### 2. 工厂模式
上面的问题是说，我们不希望完全手动组装每一个对象，而是希望有一个模具一样的东西，我们只需把不同的材料丢进去，出来的就是我们想要的对象，它们结构相同而内容不同。这个模具就如同一个工厂，我们使用函数来建造这个“工厂”：
```java
```java
function createPerson(name, age, job){
    var o = new Object();
    o.name = name;
    o.age = age;
    o.job = job;
    o.sayName = function(){
        alert(this.name);
    };
    return o;
}
var person1 = createPerson('Chuck', 25, 'Software Engineer');
var person2 = createPerson('leo', 26, 'Web Front Engineer');
console.log(person1.constructor);    //Object() { [native code] }
console.log(person1 instanceof createPerson);    //false
```
```
这个`createPerson()`函数就是一个“工厂”，我们可以通过它方便地创建任意多个我们想要的对象，只要往里“丢材料”就行了。
可以看到，一旦我们建好了“工厂”，创建对象就变得很简洁，只不过一行语句的事。这非常不错，但是这种模式仍然不完美，它没办法解决对象识别的问题。也就是说，我们无法通过任何手段识别`createPerson()`创建出来的对象的类型，因为它始终都只是个`Object`的实例。正是因为这个原因，我们很少会使用工厂模式，因为我们不希望也不应该花力气去建造这样一个工厂——这个工厂制造出的东西有模也有样，但是我们却不知道它是个什么玩意儿。
##### 3. 构造函数模式
何为构造函数？我们知道，在JavaScript中，`var o = new Object()`中的`Object()`就是一个原生的构造函数，它可以构造出`Object`类型的对象。类似地，`Array()`、`Date()`同样是JS提供的原生构造函数，它们分别能构造出数组对象和日期对象。除了这些原生构造函数，我们当然也能自定义构造函数供我们使用：
```java
```java
function Person(name, age, job){
    this.name = name;
    this.age = age;
    this.job = job;
    this.sayName = function(){
        alert(this.name);
    }
}
var person1 = new Person('Chuck', 25, 'Software Engineer');
var person2 = new Person('leo', 26, 'Web Front Engineer');
```
```
这就是构造函数模式。乍一看是不是和上面的工厂模式蛮像的？其实，差别大了去了！我们来看看这个构造函数：
- 函数名为名词&首字母大写
- 没有在内部`new Object()`而是使用了`this`关键字
- 没有`return`任何对象
- 在调用时使用`new`操作符
构造函数为什么要写成这个样子？
首先，函数名使用名词以及首字母大写的原因在于我们把构造函数视作“类”的声明，保持和`Object()`、`Array()`、`Date()`等原生构造函数统一，毕竟我们不能将构造函数当作普通函数一般对待。其次，我们在构造函数内部使用了`this`这个有意思的关键字（关于`this`，请看[《JavaScript函数与方法的那些事》](http://www.jianshu.com/p/14cfb4d69e28)一文中的相关内容），我们知道，`this`会指向调用该函数的那个对象，那么对于
```
var
 person1 = new Person('Chuck', 25, 'Software Engineer');
```
这句代码，是谁在调用`Person()`？`Person()`中的`this`会指向谁？很自然地，你可能会认为`this`表示`window`对象，`name``age``job``sayName`都会被添加到`window`对象中作为全局属性，但事实却不是！事实是它们都被添加到了`person1`当中！为什么会这样？原因就是在`Person()`之前有一个`new`操作符。当你在一个函数之前使用`new`，解析器就知道你是想创建对象，内部就会自动执行以下操作：
1> 创建一个新对象；
2> 将构造函数的作用域赋给这个新对象（因此构造函数中的`this`会指向这个新对象而不是你以为的`window`对象）；
3> 执行构造函数中的代码（为这个新对象添加属性）；
4> 返回这个新对象；
因此，`var person1 = new Person('Chuck', 25, 'Software Engineer');`这句代码确确实实为我们创建了一个我们想要的对象。
构造函数模式的这一原理需要稍加理解。不过说白了，关键和核心就在于`new`这个操作符，`new`就是为了创建对象而生的。
> 
构造函数与其他函数的唯一区别，就在于调用它们的方式不同。不过，构造函数毕竟也是函数，不存在定义构造函数的特殊语法。任何函数，只要通过 new 操作符来调用，那它就可以作为构造函数；而任何函数，如果不通过 new 操作符来调用，那它跟普通函数也不会有什么两样。——《JavaScript高级程序设计（第3版）》
回过头来，还记得工厂模式中无法进行对象识别的那个缺陷吗？没错，通过构造函数模式，我们已经完美地解决了它。来做个测试：
```java
```java
//工厂模式 createPerson()
var p1 = createPerson('Chuck', 25, 'Software Engineer');
console.log(p1.constructor);    //function Object() { [native code] }
console.log(p1 instanceof createPerson);    //false
//构造函数模式 Person()
var p2 = new Person('Chuck', 25, 'Software Engineer');
console.log(p2.constructor);    //function Person(){}
console.log(p2 instanceof Person);    //true
```
```
没问题，无论是查看`constructor`属性还是使用`instanceof`操作符，我们现在都能识别出对象的类型了。
到了这一步，你可能会觉得构造函数模式已经很完美了。确实，和工厂模式比起来，构造函数模式要优秀不少，它最棒的地方就是解决了前者无法进行对象识别这个严重的问题。但是，我们总是想要精益求精。让我们再来仔细地看一遍构造函数模式：
```java
```java
function Person(name, age, job){
    this.name = name;
    this.age = age;
    this.job = job;
    this.sayName = function(){
        alert(this.name);
    }
}
var person1 = new Person('Chuck', 25, 'Software Engineer');
var person2 = new Person('leo', 26, 'Web Front Engineer');
```
```
哪里还有问题？
> 
使用构造函数的主要问题，就是每个方法都要在每个实例上重新创建一遍。——《JavaScript高级程序设计（第3版）》
什么意思呢？我们知道，`person1`和`person2`都有一个名为`sayName()`的方法，问题在于执行构造函数后，这两个方法会分别引用各自的`sayName`实例，这就造成了额外的或者说根本就不应该的内存资源开销。请看测试：
```java
```java
console.log(person1.sayName === person2.sayName);    //false
```
```
要知道，不管在哪个对象中，这个`sayName()`方法做的事其实都是一样的，都是打印该对象的`name`。既然一个方法要干的是同一件事情，为什么要我每创建一个对象就必须同时创建一个方法的实例呢？这不是浪费资源吗？只创建一个方法的实例实现引用共享不就行了吗？所以我们对构造函数模式做如下改进：
```java
```java
function Person(name, age, job){
    this.name = name;
    this.age = age;
    this.job = job;
    this.sayName = sayName; 
}  
function sayName(){
    alert(this.name); 
} 
var person1 = new Person("Nicholas", 29, "Software Engineer");
var person2 = new Person("Greg", 27, "Doctor"); 
alert(person1.sayName === person2.sayName);  //true
```
```
我们把`sayName()`方法移到构造函数之外的全局作用域中，在构造函数内部使用`sayName`来引用它。这样一来，无论我们创建多少个对象，它们的`sayName`都指向了同一个函数，实现了引用共享。
但是，这样一来似乎又产生了更加直观的问题。首先，我们原本好好的一个构造函数被硬生生拆开，这破坏了原本良好的封装性，可以想象，当我们按照这种方式定义多个构造函数时，代码就会杂乱不堪，简直毫无封装性可言；其次，单独拎出来的`sayName()`作为一个全局函数，其作用仅仅是为了给`Person()`构造函数创建出的对象去调用，这样的全局函数多少显得有些名不副实了吧。因为这些缺陷，我们宁愿使用构造函数模式也不愿意使用这一略显尴尬的“改进版”。
##### 4. 原型模式
*原型* 是JavaScript中特有的概念。在试图掌握原型模式之前，你需要先对原型及原型链有一定程度的理解。考虑到文章主题以及篇幅问题，这里对原型的知识不做详述，只介绍如何通过原型模式进行对象的创建。
直接上代码（请结合注释进行理解）：
```java
```java
//先声明一个无参数、无内容的“空”构造函数
function Person() {
}
//使用对象字面量语法重写Person的原型对象
Person.prototype = {
    name: 'Chuck',
    age: '25',
    job: 'Software Engineer',
    sayName: function () {
        return this.name;
    }
};
//因为上面使用对象字面量的方式完全重写了原型对象，
//导致初始原型对象(Person.prototype)与构造函数(Person)之间的联系(constructor)被切断，
//因此需要手动进行连接
Object.defineProperty(Person.prototype, 'constructor', {
    enumerable: false,
    value: Person
});
//测试
var person1 = new Person();
var person2 = new Person();
person2.name = 'leo';
console.log(person1.name);    //'Chuck'
console.log(person2.name);    //'leo'
console.log(person1.sayName());    //'Chuck'
console.log(person2.sayName());    //'leo'
console.log(person1.sayName === person2.sayName);    //true
```
```
这就是纯粹的原型模式。原型对象`Person.prototype`与对象实例之间存在一个天然的连接（`_proto_`），正是这一连接赋予了JS对象属性的动态搜索特性：如果在对象本身找不到某个属性，那么就会通过这个连接到其原型对象中去找。
可以看到，通过原型模式，我们同样可以轻松地创建对象，而且可以像“继承”一般得到我们在原型对象中定义的默认属性，在此基础上，我们也可以对该对象随意地添加或修改属性及值。此外，通过上面最后一句测试代码还可以看出，其函数实现了完美的引用共享，从这一点上来说，原型模式真正解决了构造函数模式不能共享内部方法引用的问题。
原型模式看起来不错，不过它也不是没有缺点。第一，它不像构造函数模式那样，初始化时即提供参数，这使得所有新创建的实例在一开始时长得一模一样；第二，封装性欠佳；第三，对于包含引用类型值的属性，会导致不应该出现的属性共享。
对于第三个缺点，用代码更能说明问题：
```java
```java
function Person() {
}
Person.prototype = {
    constructor: Person,    //这样恢复连接会导致该属性的[[Enumerable]]特性变为true。上面的Object.defineProperty()才是完美写法。
    name: 'Chuck',
    age: '25',
    job: 'Software Engineer',
    friends: ['Frank', 'Lambert'],
    sayName: function () {
        return this.name;
    }
};
var person1 = new Person();
var person2 = new Person();
person1.friends.push('Lily');
console.log(person1.friends);    //["Frank", "Lambert", "Lily"]
console.log(person2.friends);    //["Frank", "Lambert", "Lily"]
```
```
一般而言，我们都希望各个对象各有各的属性和值，相互没有影响。可像上面示例一样，原型模式共享了不应该共享的属性，这绝对不会是我们想要的结果。
##### 5. 组合使用构造函数模式和原型模式
在进行组合之前，我们来理一理先。
不得不说，构造函数模式其实很棒，又可以传参，封装性又好，代码又简洁、容易理解，唯一的缺点就在于未能实现函数引用共享而造成不必要的内存开销。而原型模式可以完美地实现函数引用共享，但问题是它的共享很过分，它把包括函数在内的所有属性都给共享了。
很自然地，我们想到对构造函数模式和原型模式去粗取精，取各家之所长，然后进行组合，岂不完美？
的确如此。下面就是目前使用最广泛、认同度最高的构造函数模式和原型模式结合起来的“组合模式”：
```java
```java
function Person(name, age, job){
    this.name = name;
    this.age = age;
    this.job = job;
    this.friends = ['Frank', 'Lambert'];
}
Person.prototype = {
    constructor: Person,
    sayName: function(){
        return this.name;
    }
};
var person1 = new Person('Chuck', 25, 'Software Engineer');
var person2 = new Person('Natsu', 18, 'Slayers');    //魔导士纳兹 - -
person1.friends.push('Lily');
console.log(person1.friends);    //["Frank", "Lambert", "Lily"]
console.log(person2.friends);    //["Frank", "Lambert"]
console.log(person1.sayName === person2.sayName);    //true
```
```
> 
这种构造函数与原型混成的模式，是目前在 ECMAScript 中使用广泛、认同度高的一种创建自定义类型的方法。可以说，这是用来定义引用类型的一种默认模式。——《JavaScript高级程序设计（第3版）》
通过测试可以看到，通过组合使用构造函数模式和原型模式，我们终于实现了使得对象实例拥有自己可完全支配的全部属性，同时还共享了方法引用以节省内存开销。
##### 6. 动态原型模式
到上一步的“组合模式”为止，就功能和性能上而言可以说已经达到我们的要求了，现在我们考虑是否可以对代码进一步优化，毕竟“组合模式”有两段代码，起码封装性看起来不够好。
我们把需要共享的函数引用通过原型封装在构造函数中，在调用构造函数初始化对象实例的同时将该函数追加到原型对象中。当然，为了避免重复定义，需要加一个`if`判断。代码如下：
```java
```java
function Person(name, age, job, friends){
    this.name = name;
    this.age =  age;
    this.job = job;
    this.friends = friends;
    if (typeof Person.prototype.sayName !== 'function') {
        Person.prototype.sayName = function(){
            return this.name;
        }
    }
}
var person1 = new Person('chuck', 25, 'Software Engineer', ['A','B','C']);
console.log(person1.sayName());    //'chuck'
```
```
### 结语
上述6种创建对象的模式虽说是由劣到优、循序渐进的，但一般而言，在某些使用场景下的某些缺陷纯属鸡蛋里挑骨头。比如当你只是想定义一个单纯的、唯一的对象用于保存一组数据，你完全没有必要一上来就祭出一记动态原型模式。也就是说，选取何种模式来创建对象要看具体的使用场景，否则前面5种模式存在的意义是什么？此外，也希望这种鸡蛋里挑骨头的思想，这种寻找最优解的思路能带给我们一些额外的启发。
文／查查查查查查克（简书作者）
原文链接：http://www.jianshu.com/p/a9b40e6f306a
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
