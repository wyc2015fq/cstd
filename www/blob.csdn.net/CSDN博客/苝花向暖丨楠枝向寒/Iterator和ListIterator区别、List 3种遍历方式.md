# Iterator和ListIterator区别、List 3种遍历方式 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年01月17日 14:49:19[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：239


Iterator 遍历过程中 不能向集合中 删除 插入 新元素

ListIterator 遍历过程中 可以插入，通过 add()方法 可以 在 集合的最后插入元素

通过 for 循环 以及 List 下的 get() 和 add() 方法  可以 向 集合指定位置插入元素 

public class Demo03 {

  public static void main(String[] args) {

    List list01 = new ArrayList();

    list01.add("oracle");
    list01.add("java");
    list01.add("html");
    list01.add("css");
    list01.add("jsp");

    /**
     * 1 Iterator迭代器遍历
     */
    Iterator iterator = list01.iterator();
    while(iterator.hasNext()){

Object obj= iterator.next();

if(obj.equals("java")){

System.out.print("找到了:" + obj);    //找到了:java
//java.util.ConcurrentModificationException
//list01.add("javac");

}

    }
    System.out.println();     
    System.out.println("---------------------");

    /**
     * 2 ListIterator  implements Iterator 迭代器遍历
     */

    ListIterator listIterator = list01.listIterator();
    while(listIterator.hasNext()){

                 Object obj= listIterator.next();

if(obj.equals("java")){

System.out.print("找到了:" + obj);   //找到了:java
listIterator.add("javac");
break;
}
    }
    System.out.println(list01);    //[oracle, java, javac, html, css, jsp]
    System.out.println();
    System.out.println("-----------------------------");
    /**
     * 3 for循环遍历
     */
    for (int i = 0; i < list01.size(); i++) {

if(list01.get(i).equals("java")){

System.out.print("找到了:" + list01.get(i));   //找到了:java
list01.add("javac");
break;
}
}

    System.out.println(list01);      //[oracle, java, javac, html, css, jsp, javac]

       }

}


