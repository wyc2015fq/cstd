# Java中的Comparator()与comparaTo() - Machine Learning with Peppa - CSDN博客





2018年05月09日 21:05:37[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：659
所属专栏：[JavaSE](https://blog.csdn.net/column/details/21699.html)









### 1.为什么写？
- comparator 是javase中的接口，位于java.util包下，该接口抽象度极高，有必要掌握该接口的使用
- 大多数文章告诉大家comparator是用来排序，但我想说排序是comparator能实现的功能之一，他不仅限于排序

### 2.接口功能

该接口代表一个比较器，比较器具有可比性！大多数文章都写如何用comparator排序，是因为javase数组工具类和集合工具类中提供的sort方法sort就是使用Comparator接口来处理排序的，大家见久了都认为Comparator接口是用来排序的，按照java抽象的尿性来看，该接口如果为排序而生，应该叫Sortable，Sortor之类的名字吧！下面是javase一些使用到Comparator接口的地方：

```xml
Arrays.sort(T[],Comparator<? super T> c);
Collections.sort(List<T> list,Comparator<? super T> c);
```

### 3.使用场景

什么场景需要做比较，那么什么场景就是Comparator接口的用武之地，我总结的两个场景： 
1. 排序，需要比较两个对象谁排在前谁排在后（排序也可以让类实现Comparable接口，实现后该类的实例也具有排序能力）。 
2. 分组，需要比较两个对象是否是属于同一组。 
3. 待补充

### 4.举个栗子

1.排序 
在List或数组中的对象如果没有实现Comparable接口时，那么就需要调用者为需要排序的数组或List设置一个Compartor，Compartor的compare方法用来告诉代码应该怎么去比较两个实例，然后根据比较结果进行排序

> 
talk is cheap show me the code


```java
package com.java.demo;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
/**
 * @author puyf
 */
public class SortTest {
    class Dog{
    public int age;
    public String name;
    public Dog(int age, String name) {
        super();
        this.age = age;
        this.name = name;
    }
    @Override
    public String toString() {
        return "Dog [age=" + age + ", name=" + name + "]";
    }
    }
    public static void main(String[] args) {
    List<Dog> list= new ArrayList<>();
    list.add(new SortTest().new Dog(5, "DogA"));
    list.add(new SortTest().new Dog(6, "DogB"));
    list.add(new SortTest().new Dog(7, "DogC"));
    Collections.sort(list, new Comparator<Dog>() {

        @Override
        public int compare(Dog o1, Dog o2) {
        return o2.age - o1.age;
        }
    });
    System.out.println("给狗狗按照年龄倒序："+list);
    Collections.sort(list, new Comparator<Dog>() {

        @Override
        public int compare(Dog o1, Dog o2) {
        return o1.name.compareTo(o2.name);
        }
    });
    System.out.println("给狗狗按名字字母顺序排序："+list);
    }
}
```
- 


2.分组 
使用Comparator和for循环处理列表，来进行分类；通过调用者实现Comparator接口的比较逻辑，来告诉程序应该怎么比较，通过比较之后得结果来进行分组。比如生活中的拳击比赛，会有公斤级的概念，那么程序中应该实现的处理逻辑是只要两个人的体重在同一个区间则为同一组公斤级的选手。下面例子中分别按照狗狗的颜色和体重级别两个维度来进行分组，因此分组的核心逻辑其实就是比较逻辑。相面我抽了一个工具方法：dividerList,第一个参数为需要处理的数据源，第二参数是分组时的比较逻辑。

```java
package com.java.demo;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
/**
 * @author puyf
 */
public class GroupTest {
    class Apple {
    public String color;
    public int weight;

    public Apple(String color, int weight) {
        super();
        this.color = color;
        this.weight = weight;
    }

    @Override
    public String toString() {
        return "Apple [color=" + color + ", weight=" + weight + "]";
    }
    }

    /**
     * @author puyf
     * @Description:按条件分组
     * @param datas
     * @param c
     *            是否为同一组的判断标准
     * @return
     */
    public static <T> List<List<T>> divider(Collection<T> datas, Comparator<? super T> c) {
        List<List<T>> result = new ArrayList<List<T>>();
        for (T t : datas) {
            boolean isSameGroup = false;
            for (int j = 0; j < result.size(); j++) {
                if (c.compare(t, result.get(j).get(0)) == 0) {
                    isSameGroup = true;
                    result.get(j).add(t);
                    break;
                }
            }
            if (!isSameGroup) {
                // 创建
                List<T> innerList = new ArrayList<T>();
                result.add(innerList);
                innerList.add(t);
            }
        }
        return result;
    }

    public static void main(String[] args) {
    List<Apple> list = new ArrayList<>();
    list.add(new GroupTest().new Apple("红", 205));
    list.add(new GroupTest().new Apple("红", 131));
    list.add(new GroupTest().new Apple("绿", 248));
    list.add(new GroupTest().new Apple("绿", 153));
    list.add(new GroupTest().new Apple("黄", 119));
    list.add(new GroupTest().new Apple("黄", 224));
    List<List<Apple>> byColors = divider(list, new Comparator<Apple>() {

        @Override
        public int compare(Apple o1, Apple o2) {
        // 按颜色分组
        return o1.color.compareTo(o2.color);
        }
    });
    System.out.println("按颜色分组" + byColors);
    List<List<Apple>> byWeight = divider(list, new Comparator<Apple>() {

        @Override
        public int compare(Apple o1, Apple o2) {
        // 按重量级

        return (o1.weight / 100 == o2.weight / 100) ? 0 : 1;
        }
    });
    System.out.println("按重量级分组" + byWeight);
    }
}
```

结果如下（为了方便看，手动回车换行格式化了下）： 
按颜色分组 
[ 
[ 
Apple [color=红, weight=205], 
Apple [color=红, weight=131] 
], 
[ 
Apple [color=绿, weight=248], 
Apple [color=绿, weight=153] 
], 
[ 
Apple [color=黄, weight=119], 
Apple [color=黄, weight=224] 
] 
]

按重量级分组 
[ 
[ 
Apple [color=红, weight=205], 
Apple [color=绿, weight=248], 
Apple [color=黄, weight=224] 
], 
[ 
Apple [color=红, weight=131], 
Apple [color=绿, weight=153], 
Apple [color=黄, weight=119] 
] 
]

### 5.总结

一般需要做比较的逻辑都可以使用的上Comparator，最常用的场景就是排序和分组，排序常使用Arrays和Collections的sort方法，而分组则可以使用上面提供的divider方法。

排序和分组的区别在于: 
排序时，两个对象比较的结果有三种：大于，等于，小于。 
分组时，两个对象比较的结果只有两种：等于(两个对象属于同一组)，不等于(两个对象属于不同组)



