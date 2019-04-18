# JavaScript里模拟sleep - z69183787的专栏 - CSDN博客
2014年03月08日 09:25:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：695

```java
```
# 其一，不断循环，直到达到指定时间
```
```java
function
```
```java
sleep(numberMillis) {
```
```java
```
```java
var
```
```java
now =
```
```java
new
```
```java
Date();
```
```java
```
```java
var
```
```java
exitTime = now.getTime() + numberMillis;
```
```java
```
```java
while
```
```java
(
```
```java
true
```
```java
) {
```
```java
```
```java
now =
```
```java
new
```
```java
Date();
```
```java
```
```java
if
```
```java
(now.getTime() > exitTime)
```
```java
```
```java
return
```
```java
;
```
```java
```
```java
}
```
```java
}
```
```
其实代码并没有让脚本sleep下来， 相反让CPU迅速上到高负荷的附作用。多数现代浏览器会在该段时间内处于假死状态
![](http://images.cnitblog.com/blog/114013/201301/24172815-cd95122c37cd48488943d52305cce984.png)
其二，用xhr同步请求后台程序，比如传2000过去，后台就sleep 2秒后再返回，这种方式也有缺点，当N多客户端都请求后台时，一直保持连接http开销很大。
此外，window对象的alert，confirm也可以中断后续代码的执行。
5、其它模拟方式[http://www.devcheater.com/](http://www.devcheater.com/)

