# jquery源码阅读知识储备(4)关于js中if的解惑 - z69183787的专栏 - CSDN博客
2013年10月24日 15:54:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：860
**在 JavaScript 中，如果上下文需要 boolean 型的值，则引擎会自动将对象转换为 boolean 类型。转换规则为，如果该对象非空，则转换为 true,否则为 false.**
因此我们可以采取这种简写的形式。
**[javascript]**[view plain](http://blog.csdn.net/hackerhope/article/details/6174182#)[copy](http://blog.csdn.net/hackerhope/article/details/6174182#)
- if(datamodel.item){   
- //do something... 
- }else{   
-   datamodel.item = new Item();   
- }   
