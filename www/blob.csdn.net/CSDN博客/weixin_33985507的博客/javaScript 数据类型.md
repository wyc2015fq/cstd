# javaScript 数据类型 - weixin_33985507的博客 - CSDN博客
2017年03月23日 10:12:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
```
一、Number类型
整数:
    a、十进制
        var num=510;
    b、八进制字面值第一位必须是0
        var num1=070;//八进制的56
        var num2 =079;//无效的八进制—解析为79
        var num3 =08;// 无效的八进制—解析为8 (超过当做十进制数解析)
    c、十六进制前面则必须是0x，后跟十六进制数字(0~F)，不分大小写
        var num1 = 0xA;
        var num2 = 0x1f;
    注意:虽然可以表示为八进制和十六进制，但是计算时会被转换成十进制值。
浮点数:
    var num1 = 1.1;
    var num2 =0.1;
    var num3 = .1;//有效，但不推荐
    注意:在保存整数时内存分配大小只有浮点数的1/2,所以当浮点数可以转换为整数时，javascript会自动转换为整数。
         0.1+0.2不等于0.3，而是0.3000000000000004，所以在做判断时，千万不要用浮点数相加判断等于预想中的某个值。
科学计数法计数:
    var num=123.456e10;
    注意:最小值-->Number.MIN_VALUE 最大值-->Number.MAX_VALUE不在范围自动转换成Infinity值，如果是负数，就是-Infinity,整数就是Infinity。Infinity的意思是无穷，也就是正负无穷，跟数学中的概念是一样的。但是Infinity是无法参与计算的。可以用原生函数确定是不是有穷:isFinite();只有位于数值范围内才会返回true。
常用函数:
    isNaN()函数的转换规则如下：
    在Javascript中NaN用于表示一个本来要返回数值的操作数未返回数值的情况。
    NaN(非数值 Not a Number)的特点：
    1、任何涉及NaN的操作（如：NaN/0）都会返回NaN.
    2、NaN与任何值都不相等，包括NaN本身
　　Number()函数的转换规则如下：
    　　● 如果是Boolean值，true和false将分别被替换为1和0
    　　● 如果是数字值，只是简单的传入和返回
    　　● 如果是null值，返回0
    　　● 如果是undefined，返回NaN
    　　● 如果是字符串，遵循下列规则：
    　　　○ 如果字符串中只包含数字，则将其转换为十进制数值，即”1“会变成1，”123“会变成123，而”011“会变成11（前导的0被忽略）
    　　　○ 如果字符串中包含有效的浮点格式，如”1.1“，则将其转换为对应的浮点数（同样，也会忽略前导0）
    　　　○ 如果字符串中包含有效的十六进制格式，例如”0xf“，则将其转换为相同大小的十进制整数值
    　　　○ 如果字符串是空的，则将其转换为0
    　　　○ 如果字符串中包含除了上述格式之外的字符，则将其转换为NaN
    　　● 如果是对象，则调用对象的valueOf()方法，然后依照前面的规则转换返回的值。如果转换的结果是NaN，则调用对象的toString()方法，然后再依次按照前面的规则转换返回的字符串值。
        var num1 = Number("Hello World");    //NaN
        var num2 = Number("");                //0
        var num3 = Number("000011");        //11
        var num4 = Number(true);            //1
    　　由于Number()函数在转换字符串时比较复杂而且不够合理，因此在处理整数的时候更常用的是parseInt()函数。
    parseInt()函数的转换规则如下：
        函数在转换字符串时，更多的是看其是否符合数值模式。它会忽略字符串前面的空格，直至找到第一个非空格字符。如果第一个字符串不是数字字符或者负号，parseInt()会返回NaN；也就是说，用parseInt()转换空字符串会返回NaN。如果第一个字符是数字字符，praseInt()会继续解析第二个字符，知道解析完所有后续字符或者遇到了一个非数字字符。例如，"1234blue"会被转换为1234，”22.5“会被转换为22，因为小数点并不是有效的数字字符。
    　　如果字符串中的第一个字符是数字字符，parseInt()也能够识别出各种整数格式（即十进制、八进制、十六进制）。
        var num1 = parseInt("1234blue");    //1234
        var num2 = parseInt("");            //NaN
        var num3 = parseInt("0xA");            //10（十六进制）
        var num4 = parseInt("22.5");        //22
        var num5 = parseInt("070");            //56（八进制）
        var num6 = parseInt("70");            //70
        var num7 = parseInt("10",2);        //2（按二进制解析）
        var num8 = parseInt("10",8);        //8(按八进制解析)
        var num9 = parseInt("10",10);        //10（按十进制解析）
        var num10 = parseInt("10",16);        //16（按十六进制解析）
        var num11 = parseInt("AF");            //56（八进制）
        var num12 = parseInt("AF",16);        //175
        parseFloat()函数的转换规则如下：
    　　与parseInt()函数类似，parseFloat()也是从第一个字符（位置0）开始解析每个字符。而且也是一直解析到字符串末尾，或者解析到遇见一个无效的浮点数字字符为止。也就是说，字符串中的第一个小数点是有效的，而第二个小数点就是无效的了，因此它后面的字符串将被忽略。例如，”22.34.5“将会被转换成22.34。
    　　parseFloat()和parseInt()的第二个区别在于它始终都会忽略前导的零。由于parseFloat()值解析十进制值，因此它没有用第二个参数指定基数的用法。
        var num1 = parseFloat("1234blue");    //1234
        var num2 = parseFloat("0xA");        //0
        var num3 = parseFloat("22.5");        //22.5
        var num4 = parseFloat("22.34.5");    //22.34
        var num5 = parseFloat("0908.5");    //908.5
二、Boolean类型
     这个类型只有两个值：true 和false。虽然只有两个值，但是javascript中所有类型的值都有与这两个值等价的值。要将一个值转换为对应的Boolean值，可以调用转型函数Boolean() 
    其实在 if 语句判断中，会对里面的条件自动执行Boolean变化的。
三、String类型
    var str1 = "Hello";
    var str2 = 'Hello';
　　alert(str1.length);        //输出5
    
    toString()方法:
    　　要把一个值转换为一个字符串有两种方式。第一种是使用几乎每个值都有的toString()方法。
        var age = 11;
        var ageAsString = age.toString();    //字符串"11"
        var found = true;
        var foundAsString = found.toString();    //字符串"true"
    　　数值、布尔值、对象和字符串值都有toString()方法。但null和undefined值没有这个方法。
    　　多数情况下，调用toString()方法不必传递参数。但是，在调用数值的toString()方法时，可以传递一个参数：输出数值的基数。
        var num = 10;
        alert(num.toString());      //"10"
        alert(num.toString(2));     //"1010"
        alert(num.toString(8));     //"12"
        alert(num.toString(10));    //"10"
        alert(num.toString(16));    //"a"
    String()方法:    
    　　在不知道要转换的值是不是null或undefined的情况下，还可以使用转型函数String()，这个函数能够将任何类型的值转换为字符串。String() 函数遵循下列转换规则：
    　　● 如果值有toString()方法，则调用该方法（没有参数）并返回相应的结果
    　　● 如果值是null，则返回"null"
    　　● 如果值是undefined，则返回”undefined“
        var value1 = 10;
        var value2 = true;
        var value3 = null;
        var value4;
        alert(String(value1));    //"10"
        alert(String(value2));    //"true"
        alert(String(value3));    //"null"
        alert(String(value4));    //"undefined"
四、undefined类型
    如果声明变量却没有初始化，则当前变量的值就是undefined。在较老的浏览器对象并没有undefined这个属性，所以如果使用到undefined的操作将会导致失常，故采用这样的方式，不过一开始看会有点难理解，在旧版本的浏览器中会因为没有window.undefined这个对象而返回一个undefined值,所以这样做可以兼容旧浏览器。
    不过包含undefined值的变量与未定义的变量是不一样的，如：
    var name;
    alert(name);//undefined
    alert(age);// 错误：age is not defined
    还没声明过的变量只能执行一项操作，其他全都不能做，就是使用typeof操作符检测其数据类型。
    如果不管声明过未初始化以及未声明过的变量执行typeof都是返回undefined的值。两种变来那个都不能执行真正的操作
    typeof()方法:
      由于Javascript中的变量是松散类型的，所以它提供了一种检测当前变量的数据类型的方法，也就是typeof关键字，在上面提到的ECMAScript中的5种简单数据类型中，（记住，这5种只是数据类型，代表一种数据类型，就想C#中的int，string类型一样），通过typeof关键字，对这5种数据类型会返回下面的值（以字符串形式显示
        “undefined”    ----------   如果值未定义                       Undefined
        “boolean”      ----------     如果这个值是布尔值              Boolean
        “string”        ----------     如果这个值是字符串              String
        “number”      ----------     如果这个值是数值类型           Number
        “object”        ----------     如果这个值是对象或null        Object
        “function”       ----------     如果这个值是函数                 Function
五、Null类型
    Null类型是第二个只有一个值的数据类型，这个特殊的值是null。从逻辑角度来看，null值表示一个空对象指针，而这也正是使用typeof操作符检测null时会返回"object"的原因，例如：
    var car = null;
    alert(typeof car); // "object"
　　如果定义的变量准备在将来用于保存对象，那么最好将该变量初始化为null而不是其他值。这样一来，只要直接检测null值就可以知道相应的变量是否已经保存了一个对象的引用了，例如：
    if(car != null)
    {
        //对car对象执行某些操作
    }
　　实际上，undefined值是派生自null值的，因此ECMA-262规定对它们的相等性测试要返回true。
　　alert(undefined == null); //true
　　尽管null和undefined有这样的关系，但它们的用途完全不同。无论在什么情况下都没有必要把一个变量的值显式地设置为undefined，可是同样的规则对null却不适用。换句话说，只要意在保存对象的变量还没有真正保存对象，就应该明确地让该变量保存null值。这样做不仅可以体现null作为空对象指针的惯例，而且也有助于进一步区分null和undefined。
六、Object类型
    例子1:
        对象由花括号分隔。在括号内部，对象的属性以名称和值对的形式 (name : value) 来定义。属性由逗号分隔：
        var person={firstname:"Bill", lastname:"Gates", id:5566};
        上面例子中的对象 (person) 有三个属性：firstname、lastname 以及 id。
        空格和折行无关紧要。声明可横跨多行：
        var person={
        firstname : "Bill",
        lastname  : "Gates",
        id        :  5566
        };
        对象属性有两种寻址方式：
        name=person.lastname;
        name=person["lastname"];
    例子2:    
        Object类型是Javascript引用类型的鼻祖了，（就跟在C#和Java中是一样的道理），在创建Object类型的实例后可以为其添加属性和方法，
        var o = new Object;//有效，不推荐
        var o =new Object();
    说明:    
     在Javascript中，Object类型所具有的任何属性和方法也同样存在于更具体的对象中。每个实例都有如下的属性和方法，如下：
    1、constructor ，保存着用于创建当前对象的函数。上面构造函数就是Object();
    2、hasOwnProperty，用于检查给定的属性是否在当前对象实例中，是就true，不是在实例中，而是在原型中，则是false；
    3、isPrototypeOf，用于检查传入的对象是否是另一个对象的原型
    4、propertyIsEnumerable，用于检查给定的属性是否能够使用for…in语句来枚举，
    5、toString，返回对象的字符串表示
    6、valueOf，返回对象的字符串、数值或布尔值表示，通常与toString方法返回值相同
    以上就是Object所具有的属性和方法，所有对象都会因为继承关系而继承这些属性和方法。
七、数组
    下面的代码创建名为 cars 的数组：
    var cars=new Array();
    cars[0]="Audi";
    cars[1]="BMW";
    cars[2]="Volvo";
    或者 (condensed array):
    var cars=new Array("Audi","BMW","Volvo");
    或者 (literal array):
    实例
    var cars=["Audi","BMW","Volvo"];
```
