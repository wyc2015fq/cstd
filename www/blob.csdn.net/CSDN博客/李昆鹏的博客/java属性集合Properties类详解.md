
# java属性集合Properties类详解 - 李昆鹏的博客 - CSDN博客


2018年04月12日 22:28:14[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：863


---------------------------java属性集合Properties类详解--------------------------------

**Properties是Hashtable集合的子类，也是线程安全的。**
`Properties`**类表示了一个持久的属性集。**`Properties`**可保存在流中或从流中加载。属性列表中每个键及其对应值都是一个字符串。**

**特点：**
**1.    继承于Hashtable，是线程安全的键值对存储结构。**
**2.    Properties可保存在流中或从流中加载。**
**3.    只能保存字符串的键值对。**
构造器
|Properties|()
|创建一个无默认值的空属性列表。
|
|Properties|(|Properties|defaults)
|创建一个带有指定默认值的空属性列表。
|

常用方法
|String
|getProperty|(|String|key)
|用指定的键在此属性列表中搜索属性。
|
|String
|getProperty|(|String|key,|String|defaultValue)
|用指定的键在属性列表中搜索属性。
|
|Object
|setProperty|(|String|key,|String|value)
|调用 Hashtable 的方法 put。
|

**例子：创建Properties集合添加键值对，并且打印**
**public****class**PropTest {
**public****static****void**main(String[]args) {
//属性集合类不支持泛型
Propertiesprop=**new**Properties();
//添加键值对
prop.setProperty("name","zhangsan");
prop.setProperty("age","10");
prop.setProperty("gender","male");
//打印prop
System.**out**.println(prop);
//输出{age=10, name=zhangsan, gender=male}
//获取键值对
Stringname=prop.getProperty("name");
Stringage=prop.getProperty("age");
Stringgender=prop.getProperty("gender");
System.**out**.println(name+age+gender);
//输出zhangsan10male
}
}


**范例：将Properties集合中的键值对写入到文件中**
方法一：使用遍历方式写入
**public****class**PropTest2 {
**public****static****void**main(String[]args) {
//属性集合类不支持泛型
Propertiesprop=**new**Properties();
//添加键值对
prop.setProperty("name","zhangsan");
prop.setProperty("age","10");
prop.setProperty("gender","male");
PrintWriterpw=**null**;
**try**{
//创建自动刷新字符打印流对象
pw=**new**PrintWriter(**new**FileWriter("prop.txt"),**true**);
//拿到prop中所有的key
Setkeys=prop.keySet();
//遍历所有的key
**for**(Objectobj:keys) {
//将obj强转成String
Stringkey= (String)obj;
//通过key获取到相对应的值
Stringvalue=prop.getProperty(key);
//key和值写入到文件中
pw.println(key+"="+value);
}
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**if**(pw!=**null**)
pw.close();
}
}
}

|void
|list|(|PrintStream|out)
|将属性列表输出到指定的输出流。
|
|void
|list|(|PrintWriter|out)
|将属性列表输出到指定的输出流。
|

方法二：使用Properties中的list()方法写入文件
**public****class**PropTest3 {
**public****static****void**main(String[]args) {
//属性集合类不支持泛型
Propertiesprop=**new**Properties();
//添加键值对
prop.setProperty("name","zhangsan");
prop.setProperty("age","10");
prop.setProperty("gender","male");
PrintWriterpw=**null**;
**try**{
//创建自动刷新字符打印流对象
pw=**new**PrintWriter(**new**FileWriter("prop1.txt"),**true**);
//使用list()方法，把prop中的键值对写入到文件中
prop.list(pw);
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**if**(pw!=**null**)
pw.close();
}
}
}

========================================================================
|void
|load|(|InputStream|inStream)
|从输入流中读取属性列表（键和元素对）。
|
|void
|load|(|Reader|reader)
|按简单的面向行的格式从输入字符流中读取属性列表（键和元素对）。
|

**范例：把文件中的数据传入到Properties集合中，使用Properties中的load()方法**
**注意：****实际项目中使用Properties的文件来存储数据，不使用txt**
**fr= new FileReader("src/prop.properties");**
**public****class**PropTest4 {
**public****static****void**main(String[]args) {
//属性集合类不支持泛型
Propertiesprop=**new**Properties();
FileReaderfr=**null**;
**try**{
fr=**new**FileReader("prop.txt");
//把文件中的字符读取到prop集合中
prop.load(fr);
//获取prop中的属性
Stringname=prop.getProperty("name");
Stringage=prop.getProperty("age");
Stringgender=prop.getProperty("gender");
//打印
System.**out**.println(name+age+gender);
//打印zhangsan10male
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**try**{
**if**(fr!=**null**)
fr.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}

=====================================================================
|void
|store|(|OutputStream|out,|String|comments)
|以适合使用|load(InputStream)|方法加载到 Properties 表中的格式，将此 Properties  表中的属性列表（键和元素对）写入输出流。
|
|void
|store|(|Writer|writer,|String|comments)
|以适合使用|load(Reader)|方法的格式，将此 Properties 表中的属性列表（键和元素对）写入输出字符。
|

**范例：把properties集合按属性格式存储到输出流文件中，使用properties中的store()方法**

**public****class**PropTest6{
**public****static****void**main(String[]args) {
//属性集合类不支持泛型
Propertiesprop=**new**Properties();
//添加键值对
prop.setProperty("name","zhangsan");
prop.setProperty("age","10");
prop.setProperty("gender","male");
FileWriterfw=**null**;
**try**{
fw=**new**FileWriter("prop2.txt");
//把prop存储到输出流中，第二个参数是注释
prop.store(fw,"---");
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**try**{
**if**(fw!=**null**)
fw.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}


