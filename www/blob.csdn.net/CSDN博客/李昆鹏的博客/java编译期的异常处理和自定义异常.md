
# java编译期的异常处理和自定义异常 - 李昆鹏的博客 - CSDN博客


2018年03月15日 10:39:00[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：66


----------------------------**java编译期的异常处理和自定义异常**-----------------
**两种方式**
**1.****自己处理**
/**
*自己处理的方法
*编译期的异常处理不是抛出就是自己处理
*/
**public****static****void**divide(){
**try**{
ServerSocketss=**new**ServerSocket(8888);
}**catch**(IOExceptione) {
System.**out**.println("端口被占用");
e.printStackTrace();
}
}
**2.****向上抛出**
/**
*向上抛出的方法
*编译期的异常处理不是抛出就是自己处理
***@throws**IOException
*/
**public****static****void**divide()**throws**IOException {
ServerSocketss=**new**ServerSocket(8888);
}
**一般处理方式**
**编译期的异常自行处理即可**
**运行时异常要向上抛出**
**--------------****自定义异常****------------------**
**我们自己定义的异常都是运行期的异常****.**
**自定义异常的步骤**
**1.****创建一个异常类继承****RuntimeException****；**
/**
*自定义的异常直接来继承父类的构造器就可以了
***@author**Administrator
*
*/
**public****class**StockException**extends**RuntimeException {
**public**StockException() {
**super**();
}
**public**StockException(Stringmessage, Throwablecause,**boolean**enableSuppression,**boolean**writableStackTrace) {
**super**(message,cause,enableSuppression,writableStackTrace);
}
**public**StockException(Stringmessage, Throwablecause) {
**super**(message,cause);
}
**public**StockException(Stringmessage) {
**super**(message);
}
**public**StockException(Throwablecause) {
**super**(cause);
}
}
**2.****继承****RuntimeException****的构造器；**
**public****class**test11 {
**public****static****void**main(String[]args) {
*buyItem*();
}
/**
*自己处理的方法
*编译期的异常处理不是抛出就是自己处理
*/
**public****static****void**buyItem(){
**int**stock= 0;
**if**(stock== 0) {
//我们主动抛出异常
**throw****new**StockException("库存不足");
}
}
}
**运行结果**
![](//img-blog.csdn.net/20180315103822539?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

