# Java中ArrayList remove会遇到的坑 - z69183787的专栏 - CSDN博客
2018年09月10日 10:00:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：263
# 前言
平时最常用的莫过于`ArrayList`和`HashMap`了，面试的时候也是问答的常客。先不去管容量、负载因子什么的，就是简单的使用也会遇到坑。
## Remove 元素
经常遇到的一个场景是：遍历list, 然后找到合适条件的给删除掉，比如删除所有的偶数。
```
@Test
public void testRemove2(){
    List<Integer> integers = new ArrayList<>(5);
    integers.add(1);
    integers.add(2);
    integers.add(2);
    integers.add(4);
    integers.add(5);
    for (int i = 0; i < integers.size(); i++) {
        if (integers.get(i)%2==0){
            integers.remove(i);
        }
    }
    System.out.println(integers);
}
```
看起来好像没问题，加入面试的时候当面问：输出结果是什么？再问真不会报错吗？再问结果是什么？
- 报错
- 结果是空list
- 结果是[1, 2, 5]
### List.remove()有两个，一个` public E remove(int index)`，一个是`public boolean remove(Object o) `，那下面的结果是什么：
```
@Test
public void testRemove(){
    ArrayList<Integer> integers = Lists.newArrayList(1, 2, 3, 4);
    System.out.println(integers);
    integers.remove(1);
    System.out.println(integers);
}
```
- [1, 3, 4]
### 经常会使用一个Arrays.asList的API, 那么下面的结果是什么：
```
@Test
public void testRemove3(){
    List<String> list = Arrays.asList("a","b");
    list.add("c");
    System.out.println(list);
}
```
- 报错： java.lang.UnsupportedOperationException
### 使用foreach是否可以实现刚开始的问题
```
@Test
public void testRemove4(){
    List<String> strings = new ArrayList<>();
    strings.add("a");
    strings.add("b");
    strings.add("c");
    strings.add("d");
    for (String string : strings) {
        strings.remove(string);
    }
}
```
- 否，报错java.util.ConcurrentModificationException
### 为了性能问题，我们推荐把list.size的计算提取出来
```
@Test
public void testRemove5(){
    List<String> strings = new ArrayList<>();
    strings.add("a");
    strings.add("b");
    strings.add("c");
    strings.add("d");
    int size = strings.size();
    for (int i = 0; i < size; i++) {
        strings.remove(i);
    }
}
```
- 报错： java.lang.IndexOutOfBoundsException: Index: 2, Size: 2
- 这是很好的习惯， 不像开头那样每次循环都计算一次size，而且按这种情况还可以再运行的时候报错。文初的做法不报错，但结果并不是我们想要的。
### 使用Iterator是不是就可以remove了
```
@Test
public void testRemove6(){
    List<String> strings = new ArrayList<>();
    strings.add("a");
    strings.add("b");
    strings.add("c");
    strings.add("d");
    Iterator<String> iterator = strings.iterator();
    while (iterator.hasNext()){
        String next = iterator.next();
        strings.remove(next);
    }
    System.out.println(strings);
}
```
- 报错： java.util.ConcurrentModificationException
### 正确的remove做法是什么
```
@Test
public void testRemove7(){
    List<String> strings = new ArrayList<>();
    strings.add("a");
    strings.add("b");
    strings.add("c");
    strings.add("d");
    Iterator<String> iterator = strings.iterator();
    while (iterator.hasNext()){
        String next = iterator.next();
        iterator.remove();
    }
    System.out.println(strings);
}
```
