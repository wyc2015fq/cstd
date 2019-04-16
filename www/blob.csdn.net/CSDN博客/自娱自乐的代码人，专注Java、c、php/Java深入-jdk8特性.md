# Java深入 - jdk8特性 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2017年04月27日 14:03:42[initphp](https://me.csdn.net/initphp)阅读数：1069








### 1. 接口提供默认方法

Java 8允许我们给接口添加一个非抽象的方法实现，只需要使用 default关键字即可



```java
public interface Test {
    
    int get();

    /* JAVA8 接口默认方法*/
    default int getVal(int a) {
        return a * 2;
    }

}
```


```java
public class Test2 {

    public static void main(String[] args) {
        Test test = new Test() {

            @Override
            public int get() {
                return getVal(2); //调用接口默认方法
            }

        };

        System.out.println(test.get());
    }

}
```

#### 2. Lambda 表达式



```java
public class Test2 {

    public static void main(String[] args) {
        List<String> list = new ArrayList<String>();
        list.add("c");
        list.add("A");
        list.add("b");
        list.add("d");

        /* forEach */
        list.forEach((String e) -> {
            System.out.println(e);
        });

        /* 排序 */
        Collections.sort(list, (String a, String b) -> {
            return b.compareTo(a);
        });

        list.forEach((String e) -> {
            System.out.println("=====" + e);
        });

        /* Map遍历方式 */
        Map<String, String> map = new HashMap<String, String>();
        map.put("a", "aaaaaaaaa");
        map.put("b", "bbbbbbbbb");
        map.put("c", "ccccccccc");
        map.put("d", "ddddddddd");

        /* Map遍历 */
        map.forEach((a, b) -> {
            System.out.println("key:" + a + "|val:" + b);
        });

    }

}
```


### 3. 函数式接口



```java
@FunctionalInterface
    interface Converter<F, T> {
        T convert(F from);
    }

    public static void main(String[] args) {

        Converter<String, Integer> converter = (from) -> Integer.valueOf(from);
        Integer converted = converter.convert("123");
        System.out.println(converted); // 123

    }
```


### 4. 访问默认的接口

#### 1. Predicate 接口只有一个参数，返回boolean类型。该接口包含多种默认方法来将Predicate组合成其他复杂的逻辑（比如：与，或，非）



```java
public static void main(String[] args) {

        /* Predicate 接口只有一个参数，返回boolean类型。该接口包含多种默认方法来将Predicate组合成其他复杂的逻辑（比如：与，或，非） */
        Predicate<String> predicate = (s) -> (s.length() > 0 && s.length() < 10);

        System.out.println(predicate.test("test22222222222222222222")); // false
        System.out.println(predicate.negate().test("test2")); // false 结果反着来了

        Predicate<Boolean> nonNull = Objects::nonNull;
        Predicate<Boolean> isNull = Objects::isNull;

        Predicate<String> isEmpty = String::isEmpty;
        Predicate<String> isNotEmpty = isEmpty.negate();

    }
```

#### 2. Function 接口有一个参数并且返回一个结果，并附带了一些可以和其他函数组合的默认方法



```java
public static void main(String[] args) {

        /* Function 接口有一个参数并且返回一个结果，并附带了一些可以和其他函数组合的默认方法 */
        
        /* 入参String => 返回Integer*/
        Function<String, Integer> toInteger = (s) -> {
            return NumberUtils.toInt(s, 0);
        };
        System.out.println(toInteger.apply("213213"));
        
        /* 入参Integer => 返回Integer*/
        Function<Integer, Integer> add = (s) -> {
            return s + 100;
        };
        System.out.println(add.apply(200));

    }
```

#### 3. Supplier 接口返回一个任意范型的值，和Function接口不同的是该接口没有任何参数



```java
public static void main(String[] args) {

        /* Supplier 接口返回一个任意范型的值，和Function接口不同的是该接口没有任何参数 */
        Supplier<Person> personSupplier = Person::new;
        Person person = personSupplier.get();
        person.setAge(100);

        System.out.println(person.getAge());

    }
```

#### 4. Consumer 接口表示执行在单个参数上的操作。



```java
Consumer<Person> greeter = (p) -> System.out.println("Hello, " + p.firstName);
greeter.accept(new Person("Luke", "Skywalker"));
```

#### 5. Optional 接口防止空指针操作

```java
public static void main(String[] args) {

        List<String> list = new ArrayList<String>();
        list.add("c");
        list.add("A");
        list.add("b");
        list.add("d");
        

        /* Optional 主要用于防止传入的参数为null */
        Optional<String> optional = Optional.of("aa"); //  null 则会直接报错
        System.out.println(optional.get());
        
        Optional<List<String>> optional2 = Optional.of(list);
        System.out.println(optional2.get());
        System.out.println(optional2.isPresent()); //非空值 true

    }
```




6. 













