
# java集合的相互转化(map,set,list,array) - bitcarmanlee的博客 - CSDN博客


2017年12月18日 11:30:31[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：852



## 1.应用场景
java中有丰富的集合类，日常开发中几乎时刻需要使用到各种各样的集合类，其中常用的集合类包括有Map,Set,List,Array等等。下面我们就来针对各个集合类的相互转化做一下总结。
## 2.实测代码
二话不说，直接上代码。
```python
import
```
```python
org.junit.Test;
```
```python
import
```
```python
java.util.*;
```
```python
/**
 * Created by WangLei on 17-12-18.
 */
```
```python
public
```
```python
class
```
```python
CollectionsTest
```
```python
{
```
```python
Map<String, Integer> map =
```
```python
new
```
```python
HashMap<String, Integer>() {{
        put(
```
```python
"a"
```
```python
,
```
```python
1
```
```python
);
        put(
```
```python
"b"
```
```python
,
```
```python
2
```
```python
);
        put(
```
```python
"c"
```
```python
,
```
```python
3
```
```python
);
    }};
    Set<String> set =
```
```python
new
```
```python
HashSet() {{
        add(
```
```python
"a"
```
```python
);
        add(
```
```python
"b"
```
```python
);
        add(
```
```python
"c"
```
```python
);
    }};
    List<String> list =
```
```python
new
```
```python
ArrayList() {{
        add(
```
```python
"a"
```
```python
);
        add(
```
```python
"b"
```
```python
);
        add(
```
```python
"c"
```
```python
);
    }};
    String[] arr = {
```
```python
"a"
```
```python
,
```
```python
"b"
```
```python
,
```
```python
"c"
```
```python
};
```
```python
@Test
```
```python
public
```
```python
void
```
```python
map2List
```
```python
() {
        List<String> keyList =
```
```python
new
```
```python
ArrayList(map.keySet());
        System.out.println(
```
```python
"keyList is: "
```
```python
+ keyList);
```
```python
//ArrayList有如下构造方法：public ArrayList(Collection<? extends E> c)
```
```python
List<String> valueList =
```
```python
new
```
```python
ArrayList(map.values());
        System.out.println(
```
```python
"valueList is: "
```
```python
+ valueList);
    }
```
```python
@Test
```
```python
public
```
```python
void
```
```python
map2set
```
```python
() {
        Set<String> keySet = map.keySet();
```
```python
//HashSet有如下构造方法：public HashSet(Collection<? extends E> c)
```
```python
Set<String> valueSet =
```
```python
new
```
```python
HashSet(map.values());
        System.out.println(
```
```python
"valueSet is: "
```
```python
+ valueSet);
    }
```
```python
@Test
```
```python
public
```
```python
void
```
```python
arrayset
```
```python
() {
```
```python
// array -> set
```
```python
Set<String> set =
```
```python
new
```
```python
HashSet(Arrays.asList(arr));
        System.out.println(
```
```python
"set is: "
```
```python
+ set);
```
```python
//set -> array
```
```python
String[] resultArray = set.toArray(
```
```python
new
```
```python
String[set.size()]);
        Arrays.stream(resultArray).forEach(x -> System.out.print(x +
```
```python
" "
```
```python
));
    }
```
```python
@Test
```
```python
public
```
```python
void
```
```python
listset
```
```python
() {
```
```python
// list -> set
```
```python
Set<String> resultset =
```
```python
new
```
```python
HashSet<>(list);
        System.out.println(
```
```python
"result set is: "
```
```python
+ resultset);
```
```python
// set -> list
```
```python
List<String> resultlist =
```
```python
new
```
```python
ArrayList(set);
        System.out.println(
```
```python
"result list is: "
```
```python
+ resultlist);
    }
```
```python
@Test
```
```python
public
```
```python
void
```
```python
listarray
```
```python
() {
```
```python
// list -> array
```
```python
String[] resultarray = list.toArray(
```
```python
new
```
```python
String[list.size()]);
```
```python
// array -> list
```
```python
List<String> resultlist = Arrays.asList(arr);
    }
}
```
代码中方法的名称就表示了该方法的用途，就不一一解释了。
## 3.几个注意的小点
1.ArrayList,HashSet等集合类都有`public ArrayList(Collection<? extends E> c)`这种形式的构造方法，可以传一个集合进来初始化。
2.Set，List等接口都有toArray方法，源码如下：
```python
<
```
```python
T
```
```python
>
```
```python
T
```
```python
[] toArray(
```
```python
T
```
```python
[] a);
```
2.`Arrays.asList`方法的源码如下：
```python
public
```
```python
static
```
```python
<T> List<T>
```
```python
asList
```
```python
(T... a) {
```
```python
return
```
```python
new
```
```python
ArrayList<>(a);
    }
```
可以直接将数组变为一个ArrayList

