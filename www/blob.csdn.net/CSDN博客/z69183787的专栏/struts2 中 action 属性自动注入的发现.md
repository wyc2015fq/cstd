# struts2 中 action 属性自动注入的发现 - z69183787的专栏 - CSDN博客
2015年12月18日 21:20:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2754
**以前一直认为 属性设置为 private  有 public 的 getset 方法才可以注入，**
**今天发现 变量声明为 public  无需getset方法 struts2 也会自动注入 值**
