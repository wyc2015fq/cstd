# TreeSet判断重复元素 - Orisun - 博客园







# [TreeSet判断重复元素](https://www.cnblogs.com/zhangchaoyang/articles/5534284.html)





TreeSet的底层是TreeMap的keySet()，而TreeMap是基于[红黑树](http://www.cnblogs.com/zhangchaoyang/articles/2277591.html)实现的，红黑树是一种平衡二叉查找树，它能保证任何一个节点的左右子树的高度差不会超过较矮的那棵的一倍。

TreeMap是按key排序的，所以TreeSet中的元素也是排好序的。显然元素在插入TreeSet时compareTo()方法要被调用，所以TreeSet中的元素要实现Comparable接口。TreeSet作为一种Set，它不允许出现重复元素。TreeSet是用compareTo()来判断重复元素的，而非equals()，看下面代码。

```
1 import java.util.TreeSet;
 2 
 3 import org.junit.Test;
 4 
 5 public class TestTreeSet {
 6 
 7     class Combine implements Comparable<Combine> {
 8         private int p1;
 9         private int p2;
10 
11         public Combine(int p1, int p2) {
12             this.p1 = p1;
13             this.p2 = p2;
14         }
15 
16         @Override
17         public int hashCode() {
18             return p1 * 31 + p2;
19         }
20 
21         @Override
22         public boolean equals(Object obj) {
23             System.out.print("whether equal " + this + " and " + obj);
24             boolean rect = false;
25             if (obj instanceof Combine) {
26                 System.out.println("whether equal " + this + " and " + obj);
27                 Combine other = (Combine) obj;
28                 rect = (this.p1 == other.getP1() && this.p2 == other.getP2());
29             }
30             System.out.println(": " + rect);
31             return rect;
32         }
33 
34         @Override
35         public int compareTo(Combine o) {
36             System.out.print("compare " + this + " and " + o);
37             // 排序时只考虑p1
38             if (this.p1 < o.p1) {
39                 System.out.println(", return -1");
40                 return -1;
41             } else if (this.p1 > o.p1) {
42                 System.out.println(", return 1");
43                 return 1;
44             } else {
45                 System.out.println(", return 0");
46                 return 0;
47             }
48         }
49 
50         @Override
51         public String toString() {
52             return "(" + p1 + "," + p2 + ")";
53         }
54 
55         public int getP1() {
56             return p1;
57         }
58 
59         public void setP1(int p1) {
60             this.p1 = p1;
61         }
62 
63         public int getP2() {
64             return p2;
65         }
66 
67         public void setP2(int p2) {
68             this.p2 = p2;
69         }
70 
71     }
72 
73     @Test
74     public void test() {
75         Combine c1 = new Combine(1, 2);
76         Combine c2 = new Combine(1, 2);
77         Combine c3 = new Combine(1, 3);
78         Combine c4 = new Combine(5, 2);
79 
80         TreeSet<Combine> set = new TreeSet<Combine>();
81         set.add(c1);
82         set.add(c2);
83         set.add(c3);
84         set.add(c4);
85         while (!set.isEmpty()) {    //按顺序输出TreeSet中的元素
86             Combine combine = set.pollFirst();
87             System.out.println(combine.getP1() + "\t" + combine.getP2());
88         }
89     }
90 }
```

输出：

compare (1,2) and (1,2), return 0
compare (1,2) and (1,2), return 0
compare (1,3) and (1,2), return 0
compare (5,2) and (1,2), return 1
1	2
5	2

我们看到不论compareTo()返回的是不是相等，equals()方法都没有被调用。












