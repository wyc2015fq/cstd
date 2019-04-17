# java泛型一点零碎笔记 - westbrook1998的博客 - CSDN博客





2018年03月21日 20:59:13[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：41标签：[java																[泛型](https://so.csdn.net/so/search/s.do?q=泛型&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[杂七杂八](https://blog.csdn.net/westbrook1998/article/category/7458454)








**泛型为`<? extends 父类>`的数组列表只能get，不能add，原因就是父类的子类有可能有多个，这样无法确定是哪一个类，有不确定性，所以无法通过编译而相反的**

```java
package com.zxc.generics;

import java.util.ArrayList;

/**
 * @author russell
 */
public class SonGenericsTest {

    public static void main(String[] args){
        /*
        泛型为<? extends 父类>的数组列表只能get，不能add，原因就是父类的子类有可能有多个，
        这样无法确定是哪一个类，有不确定性，所以无法通过编译
        而相反的<? super 子类>的数组列表只能add，不能get
        ArrayList<? extends Father> sonList=new ArrayList<>();
        sonList.add(new Son("myson",15));
        */
        ArrayList<Father> fatherList=new ArrayList<>();
        fatherList.add(new Father("one",50));
        fatherList.add(new Father("two",30));
        System.out.println(fatherList.get(1).toString());
        ArrayList<? extends Father> sonList=fatherList;
        System.out.println(sonList.get(1).toString());
    }
}

class Father{
    private String name;
    private int age;

    Father(){}

    Father(String name,int age){
        this.name=name;
        this.age=age;
    }

    @Override
    public String toString() {
        return name+" "+age;
    }
}

class Son extends Father{
    private String name;
    private int age;

    Son(String name,int age){
        super();
        this.name=name;
        this.age=age;
    }

    @Override
    public String toString() {
        return super.toString();
    }
}
```




