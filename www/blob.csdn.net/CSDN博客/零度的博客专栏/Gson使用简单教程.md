# Gson使用简单教程 - 零度的博客专栏 - CSDN博客
2017年01月10日 14:56:59[零度anngle](https://me.csdn.net/zmx729618)阅读数：1126
### 1、【原始类型演示】
（1）原始类型转换为JSON对象（序列化）
Gson gson = new Gson();
gson.toJson(1); // 转换后的JSON为1
gson.toJson("abcd"); // 转换后的JSON为"abcd"
gson.toJson(new Long(10)); //转换后的JSON为10
gson.toJson(new int[]{1, 2}); //转换后的JSON为[1, 2]
（2）JSON对象转换为原始类型（反序列化）
Gson gson = new Gson();
int one = gson.fromJson("1", int.class); // 转换后的Java原始类型值为1
Integer ten = gson.fromJson("10", Integer.class); // 转换后的Java原始类型值为10
Long five = gson.fromJson("5", Long.class); //转换后的Java原始类型值为5
Boolean b = gson.fromJson("false", Boolean.class); //转换后的Java原始类型值为false
String str = gson.fromJson("\"abc\"", String.class); //转换后的Java原始类型值为"abc"
String anotherStr = gons.fromJson("[\"abcdef\"]", String.class); //转换后的Java原始类型值为"abcdef"
### 2、【Java对象演示】
（1）Java对象转换为JSON对象（序列化）
public class BagOfPrimitives
{
    private int value1 = 1;
    private String value2 = "abc";
    private transient int value3 = 3;
    BagOfPrimitives()
    {
    }
    public static void main(String[] args)
    {
         Gson gson = new Gson();
         BagOfPrimitives obj = new BagOfPrimitives();
         String json = gson.toJson(obj);
         System.out.println(json); // 转换后的JSON为{"value1": 1, "value2":"abc"}
    }
}
--------------------------------------------------------------------------------
细心的读者可能已发现transient修饰的字段没有转换为JSON
--------------------------------------------------------------------------------
（2）JSON对象转换为Java对象（反序列化）
String json = "{\"value1\": 1, \"value2\": \"abc\", \"value3\": 8}";
BagOfPrimitives obj = gson.fromJson(json, BagOfPrimitives.class);
--------------------------------------------------------------------------------
JSON结构中的value3尽管被赋值为8，但转换为BagOfPrimitives对象后value3的属性值依然为定义时的值。
--------------------------------------------------------------------------------
### 3、【GSON可以把JSON转换为静态内部类，但不能转换所谓纯内部类】
例如：
public class A
{
   public String a = null;
   class B
   {
        public String b = null;
   }
   public static void main(String[] args)
   {
       Gson gson = new Gson();
       String json = "{\"b\": \"def\"}";
       A obj = gson.fromJson(json, A.class);
   }
}
在转换过程中会抛出“No-args constructor for class com.pwm.gson.A$B does not exist”异常。解析方法把内部类改为静态内部类，或者按异常信息“Register an InstanceCreator with Gson for this type to fix this problem”所示，用Gson为其提供一个内部实例创建器，但GSON强烈不建议使用此方法。
### 4、【数组演示】
（1）数据转换为JSON对象（序列化）
Gson gson = new Gson();
int[] ints = {1, 2, 3, 4, 5};
String[] strings = {"abc", "def", "ghi"};
gson.toJson(ints); // 转换后的JSON为[1,2,3,4,5]
gson.toJson(strings); // 转换后的JSON为["abc","def","ghi"]
（2）JSON对象转换为数组（反序列化）
Gson gson = new Gson();
String arrays = "[1,2,3,4,5]";
int[] ints2 = gson.fromJson(arrays, int[].class); // 转换后的Java整型数组
String[] strs2 = gson.fromJson(arrays, String[].class);  //转换后的Java字符串数组
### 5、【集合演示】
（1）集合转换为JSON对象（序列化）
Gson gson = new Gson();
Collection<Integer> ints = new ArrayList<Integer>();
ints.add(new Integer(1));
ints.add(new Integer(11));
ints.add(new Integer(111));
gson.toJson(ints); // 转换后的JSON为[1,11,111]
（2）JSON对象转换为集合（反序列化）
Gson gson = new Gson();
String colJson = "[2, 22, 222]";
Type collectionType = new TypeToken<Collection<Integer>>(){}.getType();
Collection<Integer> ints2 = gson.fromJson(colJson, collectionType); // 转换后的Java整数集合
--------------------------------------------------------------------------------
GSON官方资料中关于集合有个限制声明：
•对于任意类型的对象总能序列化为JSON对象，但不能把它反序列化过来，因为对用户来讲没有通常办法决定返回值的类型
•当反序列化为JAVA对象时，必须为集合指定类型
--------------------------------------------------------------------------------
### 6、【泛型演示】
当使用toJson(obj)时，GSON会调用obj.getClass()获取对象类型信息再进行序列化，类似地当调用fromJson(json, MyClass.class)时，GSON会根据类型信息很好地完成反序列化（把JSON转换为JAVA对象），但当对象是一个泛型时则玩不转了，因为GSON在序列化或反序列化时，JAVA的泛型类型会被擦除掉，从而GSON无法获取泛型对象信息，从而无法完成转换。
（1）举例：
class Foo<T>
{
   T value;
}
Gson gson = new Gson();
Foo<Bar> foo = new Foo<Bar>();
gson.toJson(foo); // 转换JSON对象会失败，它不能正确地序列化foo.value
gson.fromJson(json, foo.getClass()); // 转换JAVA对象会失败，它无法把foo.value作为 Bar反序列化
（2）解决办法
对于这个泛型可以通过TypeToken指定正确的参数类型来解决这个问题，如下：
Type fooType = new TypeToken<Foo<Bar>>(){}.getType();
gson.toJson(foo, fooType);
gson.fromJson(json, fooType);
### 7、【任意类型集合演示】
有时候我们会处理混合类型的JSON数组，如["hello", 5, {name:"GREETINGS", source:"guest"}]，这个数组中既有字符串"hello"，又有数值5，同时还有对象{name:"GREETINGS", source:"guest"}
（1）转换为JSON对象（序列化）
很自然地想到把对象{name:"GREETINGS", source:"guest"}封装成一个JAVA对象
class Event
{
    private String name = null;
    private String source = null;
    Event(String name, String source)
    {
        this.name = name;
        this.source = source;
    }
}
此时可以使用集合进行处理了：
Collection collection = new ArrayList();
collection.add("hello");
collection.add(5);
collection.add(new Event("GREETINGS", "guest"));
对其进行测试，可以发现能成功地转换为JSON对象：
Gson gson = new gson();
gson.toJson(collection); // 转换后的JSON为["hello", 5, {name:"GREETINGS", source:"guest"}]
（2）转换为JAVA对象（反序列化）
把上面的JSON对象使用Collection collect = gson.fromJson(json, Collection.class)反序列化，遗憾的是它失败了，抛出“The JsonDeserializer
[com.google.gson.DefaultTypeAdapter$CollectionTypeAdater@a3bcc1](mailto:com.google.gson.DefaultTypeAdapter$CollectionTypeAdater@a3bcc1) failed to deserialized json object ["hello", 5, {name:"GREETINGS", source:"guest"}] given the type interface java.util.Collection”
（3）解决办法
遇到此种问题通常有三种办法来解决，此处使用最为推荐的方法：使用GSON解析器的API解析数组元素，它可以对每个数组元素使用fromJson()来处理。
JsonParser parser = new JsonParser();
JsonArray array = parser.parse(json).getAsJsonArray();
String ss = gson.fromJson(array.get(0), String.class);
int ii = gson.fromJson(array.get(1), Integer.class);
Event ee = gson.fromJson(array.get(2), Event.class);
### 8、【空对象演示】
对于空字段GSON缺省行为会忽略掉，但当使用GsonBuilder序列化一个空字段时，Gson将增加一个JsonNull元素到JsonElement结构中，从而可实现空对象的序列化和反序列化，例如：
public class Foo
{
   private final String s;
   private final int i;
   public Foo()
   {
        this(null, 5);
   }
   public Foo(String s, int i)
   {
       this.s = s;
       this.i = i;
   }
}
使用Gson进行测试：
Gson gson = new Gson();
Foo foo = new Foo();
String json = gson.toJson(foo); //转换后的Json为{"i":5}，把字符串s忽略掉了
json = gson.toJson(null);       // 转换后的Json为""空字符串
使用GsonBuilder进行测试：
Gson gson = new GsonBuilder().serializeNulls().create();
Foo foo = new Foo();
String json = gson.toJson(foo); //转换后的Json为{"s":null, "i":5}
json = gson.toJson(null);       //转换后的JSON为null
