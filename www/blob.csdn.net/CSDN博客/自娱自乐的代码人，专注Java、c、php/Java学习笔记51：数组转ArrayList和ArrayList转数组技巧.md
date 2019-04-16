# Java学习笔记51：数组转ArrayList和ArrayList转数组技巧 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年09月12日 16:52:55[initphp](https://me.csdn.net/initphp)阅读数：2765
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









ArrayList转数组：



```java
public class Test {

            public static void main(String[] args) {
                List<String> list = new ArrayList<String>();
                list.add("王利虎");
                list.add("张三");
                list.add("李四");
                int size = list.size();
                String[] array = (String[]) list.toArray(new String[size]);
                for (int i = 0; i < array.length; i++) {
                    System.out.println(array[i]);
                }
            }
        }
```


数组转ArrayList：





```java
public class Test {

            public static void main(String[] args) {
                String[] array = new String[3];
                array[0] = "王利虎";
                array[1] = "张三";
                array[2] = "李四";
                List<String> list = Arrays.asList(array);
                for (int i = 0; i < list.size(); i++) {
                    System.out.println(list.get(i));
                }

            }
        }
```







