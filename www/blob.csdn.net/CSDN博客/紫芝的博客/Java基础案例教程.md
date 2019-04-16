# Java基础案例教程 - 紫芝的博客 - CSDN博客





2018年12月18日 14:38:51[紫芝](https://me.csdn.net/qq_40507857)阅读数：1416








## 静态代码块

```java
public class Main {
    static {
        System.out.println("测试类的静态代码执行");
    }
    public static void main(String[] args){
        Person p1=new Person();
        Person p2=new Person();
    }
}

class Person{
    static{
        System.out.println("Person类中静态代码块执行");
    }
}
```

## 成员内部类

```java
public class Main {
    public static void main(String[] args) {
        outer.inner in = new outer().new inner();
        in.show();
    }
}

class outer {
    int num = 4;

    class inner {
        void show() {
            System.out.println(num);
        }
    }
}
```

## 内部类

```java
public class Solution {
    public static void main(String[] args){
        //定义一个内部类Cat实现Animal接口
        class Cat implements Animal{
            public void shout(){
                System.out.println("喵喵。。。");
            }
        }
        animalShout(new Cat());//调用方法，并传入对象
    }

    private static void animalShout(Animal an) {
        an.shout();//调用传入对象的方法
    }
}

//定义Animal接口
interface Animal{
    void shout();//定义抽象方法
}
```

## 匿名内部类

```java
public class Solution {
    public static void main(String[] args){

        //定义匿名内部类作为参数传给AnimalShout()方法
        animalShout(new Animal() {
            @Override
            public void shout() {
                System.out.println("喵喵。。。");
            }
        });
    }

    private static void animalShout(Animal an) {
        an.shout();//调用传入对象的方法
    }
}

//定义Animal接口
interface Animal{
    void shout();//定义抽象方法
}
```

## String类的初始化

```java
public class Main {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		String str1 = new String();
		String str2 = new String("abcd");
		char[] charArray = new char[] { 'D', 'E', 'F' };
		String str3 = new String(charArray);
		System.out.println("a" + str1 + "b");
		System.out.println(str2);
		System.out.println(str2);
	}

}
```

## 统计字串的个数

```java
public class Solution {
    public static void main(String[] args) {
        String str = "nbaernbatnbaynbauinbaopnba";
        String key = "nba";
        int count = getKeyStringCount(str, key);
        System.out.println(count);
    }

    private static int getKeyStringCount(String str, String key) {
        int count = 0;
        //if(!str.contains(key))//不包含key
        if (str.indexOf(key) == -1)
            return count;
        int index = 0;//记录key出现的位置
        while ((index = str.indexOf(key)) != -1) {
            str = str.substring(index + key.length());
            count++;
        }
        return count;
    }

}
```

## 定位字符串

```java
public class Main {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		String s = "ababcdedcba";
		System.out.println(s.length());
		System.out.println("first index of 'c':" + s.indexOf('c'));
		System.out.println("last index of 'c':" + s.lastIndexOf('c'));
		System.out.println("first index of \"ab\":" + s.indexOf("ab"));
		System.out.println("last index of \"ab\":" + s.lastIndexOf("ab"));
	}

}
```

## 字符串的替换和去除空格的操作

```java
import java.util.Arrays;

public class Main {
    public static void main(String[] args) {
        String s = "itcast";
        //字符串替换操作
        System.out.println("将it替换为cn.it:" + s.replace("it", "cn.it"));
        //字符串去除空格操作
        String str = "     i t c a s t     ";
        System.out.println("去除字符串两端的空格：" + str.trim());
        System.out.println("去除字符串所有的空格：" + str.replace(" ", ""));
    }
}
```

## 字符串判断操作

```java
import java.util.Arrays;

public class Main {
    public static void main(String[] args) {
        String s1 = "string", s2 = "str", s3 = "string";
        System.out.println("判断是否以str开头：" + s1.startsWith("str"));
        System.out.println("判断是否是以ng结尾：" + s1.endsWith("ng"));
        System.out.println("判断是否包含字符串abc："+s1.contains("abc"));
        System.out.println("判断两个字符串是否相等："+s2.equals(s1));
    }
}
```

## 获取系统全部属性

```java
import java.util.*;
public class Main {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		//获取当前系统属性
		Properties properties=System.getProperties();
		System.out.println(properties);
		
		System.exit(0);//停止运行
		System.exit(1);//非0标识异常退出
		
		//获取所有系统属性的key(属性名)，返回SET对象
		Set<String> propertyNames=properties.stringPropertyNames();
		for(String key:propertyNames) {
			//获取当前键key（属性名）所对应的值（属性值）
			String value =System.getProperty(key);
			System.out.println(key+"------>"+value);
		}
	}

}
```

## 获取时间和数组拷贝

```java
import java.util.*;
public class Main {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		long startTime=System.currentTimeMillis();//循环开始的当前时间
		int sum=0;
		for(int i=0;i<(int)1e9;i++) {
			sum+=i;
		}
		long endTime=System.currentTimeMillis();
		System.out.println((endTime-startTime)+"毫秒");
		int[] fromArray= {10,20,30};
		int[] toArray=new int[10];
		System.arraycopy(fromArray, 0, toArray, 1, 3);
		System.out.println(Arrays.toString(toArray));
	}
}
```

## 虚拟机运行时的状态

```java
import java.util.*;

public class Main {

    public static void main(String[] args) throws Exception {
        // TODO Auto-generated method stub
        Runtime rt = Runtime.getRuntime();//获取虚拟机运行的状态
        System.out.println("处理器的个数：" + rt.availableProcessors());
        System.out.println("空闲内存大小：" + rt.freeMemory() / 1024 / 1024 + "MB");
        System.out.println("最大可用内存数量：" + rt.maxMemory() / 1024 / 1024 + "MB");

        rt.exec("notepad.exe");//命令打开一个记事本程序

        Process process = rt.exec("notepad.exe");//得到表示进程的Process对象
        Thread.sleep(3000);//程序休眠3秒
        process.destroy();//杀掉进程
    }
}
```

## Iterator接口

```java
import java.util.*;

public class Main {
    public static void main(String[] args) throws Exception {
        ArrayList list = new ArrayList();
        list.add("data_1");
        list.add("data_2");
        list.add("data_3");

        //遍历方法1：Iterator接口
        Iterator it = list.iterator();//获取Iterator对象
        while (it.hasNext()) {
            Object obj = it.next();
            System.out.println(obj);
        }
        
        //遍历方法2:foreach循环
        /*
        foreach(容器中元素类型 临时变量：容器变量){
            执行语句
        }
         */
        for (Object obj : list) {
            System.out.println(obj);
        }
    }
}
```

## HashSet集合

```java
import java.util.*;

public class Main {
    public static void main(String[] args) throws Exception {
        HashSet set = new HashSet();
        for (int i = 1; i <= 5; i++) {
            set.add(i);
            set.add(i);//添加重复元素
        }
        Iterator it = set.iterator();
        while (it.hasNext()) {//判断是否有重复元素
            Object obj = it.next();
            System.out.println(obj);
        }
    }
}
```

## HashMap集合

```java
import java.util.*;

public class Main {
    public static void main(String[] args) throws Exception {
        Map map = new HashMap();//创建Map集合
        map.put("1", "Jack");
        map.put("2", "Rose");
        map.put("3", "Luck");
        map.put("3", "Tom");//覆盖

        Set keySet = map.keySet();//获取键的集合
        Iterator it = keySet.iterator();//迭代键的集合
        while (it.hasNext()) {
            Object key = it.next();
            Object value = map.get(key);//获取每个键所对应的值
            System.out.println(key + ": " + value);
        }
    }
}
```

## Java类型转换

```java
public class Main {
    public static void main(String[] args) {
        String s1 = "0.5", s2 = "12";
        double x = Double.parseDouble(s1);//将字符串S1变成double类型
        int y = Integer.parseInt(s2);//将字符串S2变成int类型
        System.out.println(x + y);//x+y=12.5
    }
}
```



