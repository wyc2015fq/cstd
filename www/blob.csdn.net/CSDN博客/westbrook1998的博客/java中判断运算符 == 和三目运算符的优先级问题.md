# java中判断运算符 == 和三目运算符的优先级问题 - westbrook1998的博客 - CSDN博客





2018年03月21日 22:54:41[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：224








```
public class Test {
    public static void main(String[] args) {
        System.out.println(true?false:true==true?false:true);
    }
}
//输出：false
```

== 的优先级是**大于** 三目运算符 ？ ： 的



