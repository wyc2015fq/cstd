# Java中treeMap的使用。。 - z69183787的专栏 - CSDN博客
2012年11月19日 09:37:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4655
众所周知，TreeMap是一种可以用key来排序的map，下面说一下如何使用。
第一步 定义1个  实现Comparator接口，重载 public int compare(Step o1, Step o2) 方法 或静态类 
如下：
```java
static Comparator comparator1 = new Comparator() {  
	
		  public int compare(Object o1, Object o2) {  
		  return Integer.parseInt(o2.toString()) - Integer.parseInt(o1.toString());  
		  }  
	
		  };
```
或
```java
class comparator1 implements Comparator
{
    public int compare(Object o1,Object o2)
    {
        return Integer.parseInt(o2.toString()) - Integer.parseInt(o1.toString());  
    }
}
```
第二步 初始化treemap时，构造函数中传入 该Comparator的实现类
```java
Map<String,Integer> map=new TreeMap<String,Integer>(comparator);
```
这样，遍历treemap时 就是按照key的倒叙排列的了。
