# js数组对象的学习 - z69183787的专栏 - CSDN博客
2013年10月24日 17:20:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：789
个人分类：[Javascript-概述](https://blog.csdn.net/z69183787/article/category/2175033)
 最近实习，一直就画画页面，他们说想成为好的程序员就要脚踏实地的做事。所以我克制自己尽量少埋怨，环境不能适应我，只能我去适应环境的。
各个击破是学习的一个好方法！
|**方法**|**描述**|
|----|----|
|**concat()  **|连接两个或更多的数组，并返回结果。|
|**join()**|把数组的所有元素放入一个字符串。元素通过指定的分隔符进行分隔。|
|**pop()**|删除并返回数组的最后一个元素。|
|**push()**|向数组的末尾添加一个或更多元素，并返回新的长度。|
|**reverse()**|颠倒数组中元素的顺序。|
|**shift()**|删除并返回数组的第一个元素。|
|**slice()**|从某个已有的数组返回选定的元素。|
|**sort()**|对数组的元素进行排序。|
|**splice()**|删除元素，并向数组添加新元素。|
|**unshift() **|向数组的开头添加一个或更多元素，并返回新的长度。|
|**valueOf()**|返回数组对象的原始值。|
**可用属性：length**
**[javascript]**[view plain](http://blog.csdn.net/hackerhope/article/details/6142752#)[copy](http://blog.csdn.net/hackerhope/article/details/6142752#)
- var array = new Array("red","green","black");  
- alert(array.length);//output:3
**toString(),和join()方法使用**
**[javascript]**[view plain](http://blog.csdn.net/hackerhope/article/details/6142752#)[copy](http://blog.csdn.net/hackerhope/article/details/6142752#)
- var array = new Array("red","green","black");  
- alert(array.toString());//outputs:"red","green","black"
- alert(array.join("/"))//outputs:"red"/"green"/"black"
**String的split()方法可以吧字符串转化成数组的方法。**
**[c-sharp]**[view plain](http://blog.csdn.net/hackerhope/article/details/6142752#)[copy](http://blog.csdn.net/hackerhope/article/details/6142752#)
- var strColors = "red,green,bule";  
- var arrColors = strColors.split(",");//chenge to a Array
- 
- var strColors1 = "green";  
- var arrColors1 = sColors1.split("");  
- alert(arrColors1.toString());//outputs:"g,r,e,e,n"
**Array对象具有String类具有的两个方法：concat(),slice().**
**[c-sharp]**[view plain](http://blog.csdn.net/hackerhope/article/details/6142752#)[copy](http://blog.csdn.net/hackerhope/article/details/6142752#)
- var array = new Array("red","green","black");  
- alert(array.concat("yellow"));//outputs:"red","green","black","yellow"
- var array1 = new Array("red","green","black");  
- alert(array1.slice(1));//outputs:"green"
- alert(array1.slice(0,2));//outputs:"green,black"
**Array对象的动作就像一个栈！（先进后出 类似子弹匣）。pop()和push()方法。**
****[javascript]**[view plain](http://blog.csdn.net/hackerhope/article/details/6142752#)[copy](http://blog.csdn.net/hackerhope/article/details/6142752#)**
- **var stack = new Array();  **
- **stack.push("red");  **
- **stack.push("green");  **
- **alert(stack.toString());//outputs:"red,green"**
- **var v = stack.pop();  **
- **alert(v);//outputs:"green"**
- **alert(stack.toString());//outputs:"red"**
**通过调用shift()和push()方法可以实现队列一样的行为（后进后出 类似排队买票）。**
**还有sort()方法进行排序，以后要进一步了解他。**
