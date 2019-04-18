# java中PriorityQueue优先级队列使用方法 - z69183787的专栏 - CSDN博客
2015年07月21日 14:03:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2539
    优先级队列是不同于先进先出队列的另一种队列。每次从队列中取出的是具有最高优先权的元素。
　　PriorityQueue是从JDK1.5开始提供的新的数据结构接口。
　　如果不提供Comparator的话，优先队列中元素默认按自然顺序排列，也就是数字默认是小的在队列头，字符串则按字典序排列。
       由于网上的资料大多将优先级队列各个方法属性，很少有实例讲解的，为方便大家以后使用，我就写了个demo~
      如果想实现按照自己的意愿进行优先级排列的队列的话，需要实现Comparator接口。下面的方法，实现了根据某个变量，来进行优先级队列的建立。
**[java]**[view
 plain](http://blog.csdn.net/hiphopmattshi/article/details/7334487#)[copy](http://blog.csdn.net/hiphopmattshi/article/details/7334487#)
- import java.util.Comparator;  
- import java.util.PriorityQueue;  
- import java.util.Queue;  
- 
- publicclass test {  
- private String name;  
- privateint population;  
- public test(String name, int population)  
-     {  
- this.name = name;  
- this.population = population;  
-     }  
- public String getName()  
-     {  
- returnthis.name;  
-     }  
- 
- publicint getPopulation()  
-     {  
- returnthis.population;  
-     }  
- public String toString()  
-     {  
- return getName() + " - " + getPopulation();  
-     }  
- publicstaticvoid main(String args[])  
-     {  
-         Comparator<test> OrderIsdn =  new Comparator<test>(){  
- publicint compare(test o1, test o2) {  
- // TODO Auto-generated method stub
- int numbera = o1.getPopulation();  
- int numberb = o2.getPopulation();  
- if(numberb > numbera)  
-                 {  
- return1;  
-                 }  
- elseif(numberb<numbera)  
-                 {  
- return -1;  
-                 }  
- else
-                 {  
- return0;  
-                 }  
- 
-             }  
- 
- 
- 
-         };  
-         Queue<test> priorityQueue =  new PriorityQueue<test>(11,OrderIsdn);  
- 
- 
- 
-         test t1 = new test("t1",1);  
-         test t3 = new test("t3",3);  
-         test t2 = new test("t2",2);  
-         test t4 = new test("t4",0);  
-         priorityQueue.add(t1);  
-         priorityQueue.add(t3);  
-         priorityQueue.add(t2);  
-         priorityQueue.add(t4);  
-         System.out.println(priorityQueue.poll().toString());  
-     }  
- }  
