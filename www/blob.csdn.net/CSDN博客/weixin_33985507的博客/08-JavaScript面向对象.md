# 08-JavaScript面向对象 - weixin_33985507的博客 - CSDN博客
2018年11月09日 16:41:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
## 创建对象
- JavaScript中如何创建对象
- 通过默认的Object这个类(构造函数)来创建
- 通过字面量来创建对象(语法糖)
- 先自定义构造函数, 再通过`构造函数`来创建对象
```
// 1.通过默认的Object这个类(构造函数)来创建
    var obj = new Object();
    // 2.在JavaScript中创建好一个对象之后, 可以动态的给这个对象添加属性和方法
    obj.name = "wjh";
    obj.age = 18;
    obj.say = function () {
        console.log("我是方法");
    };
    // 3.在JavaScript中, 只要给一个对象添加了属性和方法之后, 就可以通过这个对象访问对应的属性和方法
    console.log(obj.name);
    obj.say();
    //2.通过字面量来创建对象(语法糖)
    var obj = {};
    console.log(obj);
    var obj = {}; // 相当于 var obj = new Object();
    // 2.在JavaScript中创建好一个对象之后, 可以动态的给这个对象添加属性和方法
    obj.name = "zq";
    obj.age = 13;
    obj.say = function () {
        console.log("hello");
    };
    // 3.在JavaScript中, 只要给一个对象添加了属性和方法之后, 就可以通过这个对象访问对应的属性和方法
    console.log(obj.name);
    console.log(obj.age);
    obj.say();
    // 3.通过字面量来创建对象(语法糖), 并且在创建的时候就动态的添加属性和方法
    // 注意点:
    // {}中的属性名称和值之间使用冒号隔开, 属性和属性之间使用逗号隔开
    var obj = {
        name : "wjh",
        age : 18,
        say: function () {
            console.log("我是方法");
        }
    };
    console.log(obj.age);
    obj.say();
```
## this关键字
- 在JavaScript的每一个函数中都有一个`this`关键字
- `this`关键字的值是`当前调用函数`的那个对象
谁调用了当前的函数, this就是谁
> 
注意点:
默认情况下所有的函数都是通过window调用的
```
function test() {
        // 由于test函数是window调用的
        // 所以test函数中的this就是window
        console.log("函数");
        console.log(this);//window
    }
window.test();
```
> 
注意点:
函数和方法的区别:
函数可以直接调用, 在JavaScript中函数属于window对象
方法不可以直接调用, 方法属于其他对象
```
var obj = {
        name: "lnj",
        age: 18,
        // 将一个函数和一个对象绑定在一起之后, 函数就变成了方法
        // 方法只能用所属对象的来调用
        say: function () {
            // 由于say方法是通过obj对象来调用的, 所以say方法中的this就是obj对象
            console.log(this);
            console.log("hello");
        }
};
```
## 创建对象方式
- 无论通过Object来创建对象, 还是通过字面量来创建对象都存在一个弊端, 如果创建的多个对象的属性和方法名称一样, 那么每次创建都需要重新编写一次
```
var obj1 = {
        name: "lnj",
        age: 18,
        say: function () {
            console.log("hello");
        }
    };
    var obj2 = {
        name: "zq",
        age: 333,
        say: function () {
            console.log("hello");
        }
    };
```
- 使用工厂函数创建对象
```
function createPerson(name, age) {
        // 1.通过Object创建一个空对象
        // var obj = new Object();
        var obj = {};
        // 2.动态的给空对象添加属性和方法
        obj.name = name;
        obj.age = age;
        obj.say = function () {
            console.log("hello");
        }
        // 3.将函数中创建的对象返回给调用者
        return obj;
    }
    var obj1 = createPerson("lnj", 13);
    var obj2 = createPerson("zq", 18);
    console.log(obj1);
    console.log(obj2);
    console.log(typeof obj1); // object
    console.log(obj1.constructor); // ƒ Object() { [native code] }
```
- 使用构造函数创建对象
## 构造函数
- 什么是构造函数?
构造函数也是一个函数, 只不过是专门用于创建对象的函数而已
- 构造函数和普通函数的区别?
构造函数的函数名称首字母必须大写
构造函数必须使用new 来调用
- 构造函数本质上是对工厂函数的简化
1 在构造函数中默认会创建一个空的对象, 会将创建的对象赋值给this, 会将创建的对象返回给调用者
2 过去通过Object对象, 或者通过字面量, 或者通过工厂函数创建的对象, 我们无法判断这个对象是谁创建出来的但是通过构造函数创建的对象, 我们可以判断这个对象是谁创建出来的
3 默认情况下每一个对象都有一个隐藏的属性, 叫做constructor, 这个属性指向了创建当前对象的构造函数
4.可以使用`instanceof`来判断某个对象是由哪个构造函数创建的
```
/*
    new Person("lnj", 13);做了什么事情?
    1.会在构造函数中创建一个空的对象
    2.将创建好的空对象赋值给this
    3.将创建好的对象返回给调用者
    */
    function Person(name,age) {
        this.name = name;
        this.age = age;
        this.say = function () {
            console.log("hello");
        }
    }
    var per = new Person("wjh",19);
    console.log(typeof per);//object
    console.log(per.constructor);//返回的是Person构造函数
    //在企业开发中如果想判断某个对象时候是某个构造函数创建出来的
    //可以使用 对象名称 instanceof 构造函数名称, 来判断
    console.log(per instanceof Person);
```
## 构造函数创建对象性能问题
- 默认情况下, 只要创建一个对象就会在对象中开辟一块存储空间
该存储空间中会存储对象的所有数据
- 这样子的话,每次创建一个对象都要存储所有数据,很浪费资源
```
function Person(name, age) {
        this.name = name;
        this.age = age;
        this.say = function () {
            //           obj1.name, obj1.age
            //           obj2.name, obj2.age
            console.log(this.name, this.age);
        };
    }
    /*
    默认情况下, 只要创建一个对象就会在对象中开辟一块存储空间
    该存储空间中会存储对象的所有数据
     */
    var obj1 = new Person("lnj", 13);
    obj1.say();
    var obj2 = new Person("zq", 18);
    obj2.say();
    // 这里的===是在判断两个函数的地址是否相同
    console.log(obj1.say === obj2.say);
```
## 解决构造函数性能问题方法一
- 解决方案:
将方法定义在外面, 将定义在外面函数的地址赋值给属性每次创建对象, 对象中say保存的都是函数的地址, 就不会重复保存了
弊端:
因为将函数定义在了全局作用域中, 所以如果定义了多个函数, 会导致全局作用域的名称匮乏
```
var say = function () {
        console.log(this.name, this.age);
    };
    // var say = new Function("console.log(this.name, this.age);");
    // 定义了一个构造函数
    function Person(name, age) {
        this.name = name;
        this.age = age;
        // this.say = function () {
        //     console.log(this.name, this.age);
        // };
        /*
        1.解决方案:
        将方法定义在外面, 将定义在外面函数的地址赋值给属性
        每次创建对象, 对象中say保存的都是函数的地址, 就不会重复保存了
        2.弊端:
        因为将函数定义在了全局作用域中, 所以如果定义了多个函数, 会导致全局作用域的名称匮乏
         */
        this.say = say;
    }
    var obj1 = new Person("lnj", 13);
    obj1.say();
    var obj2 = new Person("zq", 18);
    obj2.say();
    // 这里的===是在判断两个函数的地址是否相同
    console.log(obj1.say === obj2.say); // true
```
## 解决构造函数性能问题方法二
- 解决方案:
将所有函数都封装到另外一个函数中, 这样函数名称就不在全局作用域中了, 这就不会导致全局作用域命名匮乏问题了, 然后将对象中方法的地址复制给使用者即可
```
var fns = {
        say: function() {
            console.log(this.name, this.age);
        },
        eat: function () {
            console.log("eat");
        }
    };
    // 定义了一个构造函数
    function Person(name, age) {
        this.name = name;
        this.age = age;
        /*
        1.解决方案:
        将所有函数都封装到另外一个函数中, 这样函数名称就不在全局作用域中了, 这就不会导致全局作用域命名匮乏问题了
        然后将对象中方法的地址复制给使用者即可
         */
        this.say = fns.say;
        this.eat = fns.eat;
    }
    var obj1 = new Person("lnj", 13);
    obj1.say();
    var obj2 = new Person("zq", 18);
    obj2.say();
    // 这里的===是在判断两个函数的地址是否相同
    console.log(obj1.say === obj2.say); // true
```
## 解决构造函数性能问题方法三
- 这种方法是经常使用的方法
> 
私有成员（一般就是非函数成员）放到构造函数中
共享成员（一般就是函数）放到原型对象中
如果重置了 prototype 记得修正 constructor 的指向
```
/*
   1.记住当前的这种方案, 忘记前面的两种方案
   前面两种方案主要是为了让你看到问题所在
   2.在JavaScript中,每一个构造函数都有一个默认的属性, 这个属性叫做prototype
   prototype属性指向一个对象, 这个对象我们称之为构造函数的原型对象
   3.上节课为了解决全局作用域命名匮乏的问题, 我们将所有的方法都放到了一个对象中, 所以既然构造函数的prototype就对应一个对象, 所以我们就可以将方法都放到这个对象中
   4.注意点:
   所有通过同一个构造函数创建出来的对象, 都可以放到该构造函数的原型对象
   并且所有通过同一个构造函数创建出来的对象, 访问的都是同一个原型对象
    */
    //定义一个构造函数
    function Person(name, age) {
        this.name = name;
        this.age = age;
    }
    /*
    私有成员（一般就是非函数成员）放到构造函数中
    共享成员（一般就是函数）放到原型对象中
    如果重置了 prototype 记得修正 constructor 的指向
     */
    //将方法存入原型对象中
    // console.log(Person.prototype);
    Person.prototype.say = function () {
        console.log(this.name, this.age);
    };
    var per1 = new Person("wjh",19);
    per1.say();
    var per2 = new Person("lnj",20);
    console.log(per1.say === per2.say);//true
```
## 构造函数-对象-原型对象的三角关系
- 1.1每个构造函数都有一个默认的属性, 叫做`prototype`, 这个属性指向一个对象(`原型对象`)
- 1.2每个原型对象都有一个默认的属性, 叫做`constructor`, 这个属性指向原型对象对应的`构造函数`
- 1.3每个对象都有一个默认的属性, 叫做`__proto__`, 这个属性指向创建它的构造函数的`原型对象`
```
function Person(name, age) {
        this.name = name;
        this.age = age;
    }
    // 给构造函数的原型对象添加属性和方法
    Person.prototype.say = function () {
        console.log(this.name, this.age);
    };
    Person.prototype.type = "人";
    //通过构造函数创建对象
    var obj1 = new Person("wjh",19);
    //这两个都是指向Person构造函数的原型对象
    console.log(Person.prototype);
    console.log(obj1.__proto__);
    console.log(obj1.__proto__ === Person.prototype);
    console.log(obj1.__proto__.constructor);
    console.log(Person.prototype.constructor);
    console.log(obj1.__proto__.constructor === 
    Person.prototype.constructor);
```
![13631564-ab8a9396ac476a1e.png](https://upload-images.jianshu.io/upload_images/13631564-ab8a9396ac476a1e.png)
## 原型链
- 原型对象也是对象,所以原型对象是由Object构造函数创建的,原型对象的`__proto__`  指向的是`Object原型对象`,这种串联起来的链条叫做原型链
- 注意点:
Object原型对象的`__proto__` 指向的是null
```
// 定义了一个构造函数
    function Person(name, age) {
        this.name = name;
        this.age = age;
    }
    // 给构造函数的原型对象添加属性和方法
    Person.prototype.say = function () {
        console.log(this.name, this.age);
    };
    Person.prototype.type = "人";
    // 通过构造函数创建对象
    var obj1 = new Person("lnj", 13);
    console.log(obj1.__proto__.__proto__);//Object
    console.log(obj1.__proto__.__proto__.__proto__);//null
```
![13631564-8bd8e8b45c53bca2.png](https://upload-images.jianshu.io/upload_images/13631564-8bd8e8b45c53bca2.png)
## 属性和方法的查找顺序
- 属性查找顺序
- 会先在当前对象中查找有没有type属性, 如果有, 使用当前的
- 如果当前对象没有type属性, 会到原型对象中查找, 如果有, 就使用原型对象中的type属性
- 如果原型对象中没有, 会继续根据`__proto__`链条查找, 如果找到null都没有, 就输出`undefined`
- 方法查找顺序
- 会先在当前对象中查找有没有say方法, 如果有, 使用当前的
- 如果当前对象没有say方法, 会到原型对象中查找, 如果有, 就使用原型对象中的say方法
- 如果原型对象中没有, 会继续根据`__proto__`链条查找, 如果找到null都没有, 就`报错`
> 
注意点:
在直接通过对象访问属性的时, 不会修改原型对象中的属性
会在当前对象中新增一个属性
```
//这种方式是修改当前对象中的属性
obj.type = "人";
//只有使用这种方式才会修改原型对象中的属性
obj.__proto__.type = "人"
console.log(obj.__proto__.type);//人
```
```
// 定义了一个构造函数
    function Person(name, age) {
        this.name = name;
        this.age = age;
        // this.type = "人";
        // this.say = function () {
        //     console.log("自己的", this.name, this.age);
        // }
    }
    // 给构造函数的原型对象添加属性和方法
    // Person.prototype.say = function () {
    //     console.log("原型的",this.name, this.age);
    // };
    Person.prototype.type = "超人";
    //创建对象
    var obj = new Person("wjh", 19);
```
