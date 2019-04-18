# 代码详解Java猴子选王问题（约瑟夫环） - z69183787的专栏 - CSDN博客
2018年03月30日 18:15:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：309
[https://m.jb51.net/article/135476.htm](https://m.jb51.net/article/135476.htm)
关于约瑟夫环的基本知识：
罗马人攻占了乔塔帕特，41人藏在一个山洞中躲过了这场浩劫。这41个人中，包括历史学家josephus和他的一个朋友。剩余的39个人为了表示不向罗马人屈服，决定集体自杀。大家决定了一个自杀方案，所有这41人围城一个圆圈，由第一个人开始顺时针报数，没报数为3的人就立刻自杀，然后由下一个人重新开始报数仍然是每报数为3的人就立刻自杀,......,知道所有人都自杀死亡为止.约瑟夫和他的朋友并不想自杀，于是约瑟夫想到了一个计策，他们两个同样参数到自杀方案中，但是最后却躲过了自杀。请问是怎么做到的
代码分享：
```java
import java.util.HashMap; 
import java.util.Map; 
public class MonkeyKing { 
  public static void main(String args[]) { 
    int n = 100; // 猴子总数 
    int m = 3; // 报数出局数 
    @SuppressWarnings("rawtypes") 
    Map map = new HashMap(); 
    int nn = 1; // 报数序号 
    int mm = 1; // 报数号 
    System.out.println("-----------------------" + n + "只猴子选大王开始-----------------------"); 
    for (int i = 1; i < n + 1; i++) { 
      map.put(i, i); 
    } 
    while (map.size() > 1) { 
      if (mm == 3) { 
        map.remove(nn); 
      } 
      nn++; 
      if (nn == n + 1) { 
        nn = 1; 
      } 
      if (map.get(nn) != null) { 
        mm++; 
      } 
      if (mm == m + 1) { 
        mm = 1; 
      } 
    } 
    String result = map.values().toString(); 
    System.out.println("第" + result.substring(1, result.length() - 1) + "只猴子当选猴王"); 
  } 
}
```
