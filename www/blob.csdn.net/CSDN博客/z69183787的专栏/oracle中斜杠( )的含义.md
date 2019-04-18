# oracle中斜杠(/)的含义 - z69183787的专栏 - CSDN博客
2015年01月26日 10:03:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6440
**如果是语句的话，;表示结束并且运行如果是程序块或者pl块，；表示结束不运行，必须/表示运行**
斜杠就是让服务器执行前面所写的sql脚本。如果是普通的select语句，一个分号，就可以执行了。但是如果是存储过程，那么遇到分号，就不能马上执行了。这个时候，就需要通过斜杠(/)来执行。
```
`set`
```
serveroutput
```
`on``;`
`begin`
`    ``dbms_output.put_line(`
```
'Hello
 World!'
```
`);`
`end``;`
`/`
```
执行运行结果：
```
```xml
Hello
 World!
```
```xml
```
```xml
PL/SQL
 procedure successfully completed
```
```
