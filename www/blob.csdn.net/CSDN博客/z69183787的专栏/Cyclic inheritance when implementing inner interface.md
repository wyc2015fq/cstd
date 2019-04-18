# Cyclic inheritance when implementing inner interface - z69183787的专栏 - CSDN博客
2016年12月29日 16:17:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：497
This would be because you are implementing (coding) the interface you are implementing (inheriting) inside of the class that is inheriting from that class.
I wish I could make that sentence better...
But here is a visual example.
```java
```
Class A implements Interface B {
    Interface B {
    }
}
```
```
As far as I know, this is not allowed. You need to define the interface outside of the class (in this case, an Enum).
Like so:
```java
```
Interface B {
}
Class A implements Interface B {
}
```
```
Best practice is probably to break them up into different files.
