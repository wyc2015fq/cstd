# 使用LinkedList来模拟一个堆栈或者队列数据结构 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2016年12月05日 10:37:26[QuJack](https://me.csdn.net/A784586)阅读数：390








## 使用LinkedList来模拟一个堆栈或者队列数据结构

|||
|----|----|



import java.util.LinkedList;




public class DuiLie {

private LinkedList link;

public DuiLie(){     //构造函数

link=new LinkedList();

}




public void myadd(Object obj){

link.addFirst(obj);

}

public Object myGet(){

return link.removeFirst();

}

public boolean isnull(){

return link.isEmpty();

}

}

////////////////////////////////////////////////////////////////////////////////////////////////


public class LinkedListtest {




public static void main(String[] args) {

DuiLie dl=new DuiLie();

dl.myadd("abc1");

dl.myadd("abc2");

dl.myadd("abc3");



while(!dl.isnull()){

System.out.println(dl.myGet());

}        



}




}





