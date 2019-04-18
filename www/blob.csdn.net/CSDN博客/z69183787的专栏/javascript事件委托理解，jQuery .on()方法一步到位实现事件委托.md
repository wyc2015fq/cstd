# javascript事件委托理解，jQuery .on()方法一步到位实现事件委托 - z69183787的专栏 - CSDN博客
2017年01月04日 15:15:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：19283
# 本篇文章借鉴自：[博客园文章](http://www.cnblogs.com/liugang-vip/p/5616484.html)，只为自己巩固下事件委托方面的知识
# 概述:
# 什么叫事件委托？他还有一个名字叫做事件代理，（时间代理 === 事件委托，现在才知道这两个是一个意思）
# 高程3上讲：事件委托即是利用事件冒泡，只指定一个事件处理程序，就可以管理某一类型的所有事件。
借鉴其他大牛的一个例子，也为自己更好的理解一下：收快递例子
有三个同事预计会在周一收到快递。为签收快递，有两种办法：一是三个人在公司门口等快递；二是委托给前台MM代为签收。现实当中，我们大都采用委托的方案（公司也不会容忍那么多员工站在门口就为了等快递）。前台MM收到快递后，她会判断收件人是谁，然后按照收件人的要求签收，甚至代为付款。这种方案还有一个优势，那就是即使公司里来了新员工（不管多少），前台MM也会在收到寄给新员工的快递后核实并代为签收。
两层意思：
1.现在委托前台的同事是可以签收的，即程序中的现有的dom节点是有事件的；
2.新员工也是可以被前台mm代为签收的，即程序中新添加的dom节点也是有事件的；
**为什么要用事件委托？？？事件委托有什么好处？？？**
一般来说，dom是需要有事件处理程序的，我们会直接给他事件处理程序就好了，那么如果是很多dom元素需要添加事件处理呢？？？
比如 100个li，每个li 都有相同的click点击事件，可能我们会有for循环的方法，来遍历所有li，然后为每个li添加绑定事件。
这么做毫无疑问的是对性能有很大的影响；
在js中，添加到页面上的事件处理程序的多少将直接关系到页面运行的整体性能，因为需要不断的与dom节点进行交互，访问dom次数越多，引起浏览器重绘与重排的次数也就越多，就会延长整个页面的交互就绪时间。
这就是性能优化，减少dom操作的原因；
如果采用事件委托，就会将所有的操作放到js程序里面，与dom的操作就只交互一次，这样减少了dom交互次数，性能就会提升；
**事件委托原理：**
事件委托就是利用事件冒泡原理实现的！
事件冒泡：就是事件从最深节点开始，然后逐步向上传播事件；
例：页面上有一个节点树，div > ul  > li  >  a
比如给最里面的a 加一个click 事件，那么事件就会一层一层的往外执行，执行顺序 a > li > ul > div,  有这样一个机制，当我们给最外层的div 添加点击事件，那么里面的ul ， li  ， a  做点击事件的时候，都会冒泡到最外层的div上，所以都会触发，这就是事件委托，委托他们父集代为执行事件；
**业务需求：**实现功能，点击td ,单元格变色；
html结构
**[html]**[view
 plain](http://blog.csdn.net/orange0111/article/details/53153228#)[copy](http://blog.csdn.net/orange0111/article/details/53153228#)
- <!-- 事件绑定 -->
- <tableid="myTable"border="1">
- <tr>
- <td>1</td>
- <td>2</td>
- <td>3</td>
- </tr>
- </table>
**[javascript]**[view
 plain](http://blog.csdn.net/orange0111/article/details/53153228#)[copy](http://blog.csdn.net/orange0111/article/details/53153228#)
- window.onload = function(){  
- var oTa = document.getElementById("myTable");  
- var aTd = oTa.getElementsByTagName('td');  
- for(var i=0;i<aTd.length;i++){  
-         aTd[i].onclick = function(){  
-             aTd[i].style.background = 'red';  
-         }  
-     }  
- }  
上面的办法是最简单的办法，也是最笨的办法，我们看看执行了多少次dom操作，首先找到table 然后遍历td ，当点击td的时候，又要找一次目标的li的位置，才能执行最后的操作，每次点击都要找一次td
那我们用**事件委托**的方式怎么来写呢，？？
**[javascript]**[view
 plain](http://blog.csdn.net/orange0111/article/details/53153228#)[copy](http://blog.csdn.net/orange0111/article/details/53153228#)
- window.onload = function(){  
- var oTa= document.getElementById("myTable");  
-     oTa.onclick = function(){         //点击 table、td均可以alert(123)
-        alert(123);    
-     }}  
**[javascript]**[view
 plain](http://blog.csdn.net/orange0111/article/details/53153228#)[copy](http://blog.csdn.net/orange0111/article/details/53153228#)
- <pre></pre>  
- <pre></pre>  
这里用父集做事件处理，当td被点击时，由于冒泡原理事件就会冒泡到table上，因此table上有点击事件，所以事件就会被触发;
当然单当点击table本身的时候也是会触发的；
如果我们只想让td触发而不想让table触发，怎么办呢？？？
**Event**对象提供了一个属性叫做**target**，可以返回事件的目标节点，我们称之为事件源，也就是说，target就可以表示**当前事件操作的dom**，但可能不是真正操作的dom，
存在兼容性问题：标准浏览器：event.target，IE浏览器：event.srcElement，
此时只是获取了当前节点的位置，但并不知道节点名称，这里我们用**nodeName**来获取具体是什么标签名，这个返回值是一个大写的，判断时需要转换为小写；
**[javascript]**[view
 plain](http://blog.csdn.net/orange0111/article/details/53153228#)[copy](http://blog.csdn.net/orange0111/article/details/53153228#)
- window.onload = function(){  
- var oTa = document.getElementById("myTable");  
- 　　oTa.onclick = function(e){  
- var e = e || window.event;                    //处理兼容性
- var target = e.target || e.srcElement;　　　  
- 　　　　target.nodeName.toLowerCase() == 'td' ? alert('我点中了table') ：(target.style.background = 'red');  //三元运算符进行判断
- 　　}  
- }  
这样改一下，就只有td会触发事件啦，且每次只执行一次dom操作，如果td很多的话，将大大减小dom的操作；
上面的例子是说td点击都是产生同样的效果，要是每个td被点击的效果都不一样，那么事件委托还有用吗，？？？
**[javascript]**[view
 plain](http://blog.csdn.net/orange0111/article/details/53153228#)[copy](http://blog.csdn.net/orange0111/article/details/53153228#)
- <!-- 事件绑定 -->  
-     <table id="myTable" border="1">  
-         <tr>  
-             <td id="add">增加</td>  
-             <td id="delete">删除</td>  
-             <td id="modfiy">修改</td>  
-             <td id="select">查找</td>  
-         </tr>  
-     </table>  
非事件委托写法
**[javascript]**[view
 plain](http://blog.csdn.net/orange0111/article/details/53153228#)[copy](http://blog.csdn.net/orange0111/article/details/53153228#)
- window.onload = function(){  
- var Add = document.getElementById("add");  
- var Delete = document.getElementById("delete");  
- var Move = document.getElementById("move");  
- var Select = document.getElementById("select");  
- 
-             Add.onclick = function(){  
-                 alert('添加');  
-             };  
-             Remove.onclick = function(){  
-                 alert('删除');  
-             };  
-             Move.onclick = function(){  
-                 alert('移动');  
-             };  
-             Select.onclick = function(){  
-                 alert('选择');  
-             }  
- 
-         }  
4个按钮，点击每一个做不同的动作，，，那么至少需要4次dom操作；
如果用事件委托，能进行优化吗？
事件委托写法
**[javascript]**[view
 plain](http://blog.csdn.net/orange0111/article/details/53153228#)[copy](http://blog.csdn.net/orange0111/article/details/53153228#)
- window.onload = function(){  
- var myTable = document.getElementById("myTable");  
-             myTable.onclick = function (ev) {     
- var ev = ev || window.event;  
- var target = ev.target || ev.srcElement;  
- if(target.nodeName.toLocaleLowerCase() == 'table'){  
- switch(target.id){  
- case'add' :  
-                             alert('添加');  
- break;  
- case'remove' :  
-                             alert('删除');  
- break;  
- case'move' :  
-                             alert('移动');  
- break;  
- case'select' :  
-                             alert('选择');  
- break;  
-                     }  
-                 }  
-             }  
- 
-         }  
用事件委托，只用一次dom操作，就能完成所有的效果，性能肯定比上面的好；
前面讲的都是document**加载完成**后现有的dom节点的操作；
那么如果是新增的节点，新增的节点会有事件吗？？？也就是说，新来的一个员工，他能收到快递吗？？？？
看一下正常添加节点的方法：
**[javascript]**[view
 plain](http://blog.csdn.net/orange0111/article/details/53153228#)[copy](http://blog.csdn.net/orange0111/article/details/53153228#)
- <input type="button" name="" id="btn" value="添加" />  
-     <ul id="ul1">  
-         <li>111</li>  
-         <li>222</li>  
-         <li>333</li>  
-         <li>444</li>  
-     </ul>  
用一般的方法，循环加给li加点击事件，你会发现新增的li**没有事件**！！！
**解决办法：**
一般情况下，我们会用一个函数把那个for循环包起来，然后再在点击事件里调用这个函数，这样也能实现目的，功能虽然实现了，但无疑又增加了dom操作，
用**事件委托**的方式如何做呢？？？
**[javascript]**[view
 plain](http://blog.csdn.net/orange0111/article/details/53153228#)[copy](http://blog.csdn.net/orange0111/article/details/53153228#)
- <script type="text/javascript">  
-     window.onload = function(){  
- var ul1 = document.getElementById('ul1');  
- var li = document.getElementsByTagName('li');  
- var btn = document.getElementById('btn');  
- var num = 1;  
- // for(var i=0;i<li.length;i++){
- //  li[i].onclick = function(){
- //      alert(this.innerHTML);
- //  }
- //  num++;
- // }
-         ul1.onclick = function(e){  
- var e = e || window.event;  
- var target = e.target || event.srcElement;  
-             console.log(target.nodeName);  
-             target.nodeName.toLowerCase() == 'li' ? alert(target.innerHTML) : alert("您点中了ul标签");  
-         }  
- 
-         btn.onclick = function(){  
- var oLi = document.createElement('li');  
-             oLi.innerHTML = 111*num;  
-             ul1.appendChild(oLi);  
-         }  
-     }  
上面是用事件委托的方式，**新添加**的子元素也是有点击功能的；
我们可以发现，当用事件委托的时候，根本不需要遍历元素的子节点，只需要给父级元素添加事件就好了，
其他的代码都是在js里面执行的，这样可以大大减少dom操作，
这就是我理解的事件委托的精髓；
**jQuery事件委托：**
我直接上code了，jquery的操作是如此的简单，一个on全搞定！！！
**[html]**[view
 plain](http://blog.csdn.net/orange0111/article/details/53153228#)[copy](http://blog.csdn.net/orange0111/article/details/53153228#)
- <!-- 事件绑定 -->
- <tableid="myTable"border="1">
- <tr>
- <td>111</td>
- <td>222</td>
- <td>333</td>
- <td>444</td>
- </tr>
- </table>
**[javascript]**[view
 plain](http://blog.csdn.net/orange0111/article/details/53153228#)[copy](http://blog.csdn.net/orange0111/article/details/53153228#)
- <script src="http://libs.baidu.com/jquery/1.9.1/jquery.min.js"></script>  
- <script type="text/javascript">  
-     $(function(){         
-         $("#myTable td").click(function(){    
-             $(this).html();  //普通写法，如果有100个td元素，就要绑定100次事件，十分浪费性能
-         })  
- 
- 
- 
-         $("#myTable").click(function(e){    //这种点击方式怎么排除父元素？？？？
-             console.log(e.target);  
- var $clicked = $(e.target);    //e.target 捕捉到触发的元素
-             console.log(e.target.nodeName);  //结果为大写，用toLowerCase()  转换为小写
-             e.target.nodeName.toLowerCase() == 'table' ? alert('我点中table了')  : $clicked.html();  //排除父元素的点击效果
-         })  
- 
- 
- // jQuery1.7的on绑定方法,替代了以往的bind、live等方法，内部自动含有事件委托机制
- // $(selector).on(event,childSelector,data,function,map)   on方法的语法
- // $(selector).on(event,childSelector,function(){})        on方法语法，绑定在父元素上 事件，子选择器，回调函数
-         $("#myTable").on('click','td',function(){  
-             $(this).html();  
-         })  
-     })  
- </script>  
**总结：**
事件委托的优点：1.在页面dom元素较多的情况下，大大减少了dom操作，优化了性能；
                            2.值需要为一个父元素绑定事件，就可以管理他下面的所有子元素；
                   3。。。。。。啦啦啦啦，我也不知道啦
适用性：
            适合：click 、mousedown、mouseup、keydown 、keyup、keypress
        不适合：mouseover和mouseout虽然也有事件冒泡，但是处理他们要特别注意，因为需要经常计算他们的位置，处理起来不太容易，
                      focus、blur之类的本身就没有冒泡的特性，自然不能用事件委托；
借鉴博客园那位兄弟的文章！！！！
