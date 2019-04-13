
# java集合序列化 - 李昆鹏的博客 - CSDN博客


2018年04月13日 08:39:07[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：272


---------------------------java集合序列化-----------------------------

**当我们要序列化多个对象时，一个一个的写和读太麻烦，**
**如果我们使用遍历来读取时，当对象没有对象可读取的时候，会出现EOFException异常。**
**这个时候我们就可以使用集合存储多个对象，然后再序列化集合，最后我们只需要对集合做读取就可以，**
**在对集合进行遍历，获取到具体的每一个对象。这就是集合序列化**

例子：

**public****class**SerilDemo2 {
**public****static****void**main(String[]args) {
//writerObject();
//readerObject();
//输出
//Person [name=李昆鹏, age=22]
//Person [name=李昆鹏1, age=23]
//Person [name=李昆鹏2, age=24]
//通过集合来写入多个对象，并且读取
*writerObject1*();
*readerObject1*();
//输出结果是一样的，当对象数量足够多的时候，使用集合会非常方便
//Person [name=李昆鹏, age=22]
//Person [name=李昆鹏1, age=23]
//Person [name=李昆鹏2, age=24]
}
/**
*序列化流
*/
**public****static****void**writerObject() {
ObjectOutputStreamoos=**null**;
**try**{
//创建序列化流对象
oos=**new**ObjectOutputStream(**new**FileOutputStream("person.txt"));
//创建多个person类对象，并且给属性赋值
Personp=**new**Person();
p.setName("李昆鹏");
p.setAge(22);
Personp1=**new**Person();
p1.setName("李昆鹏1");
p1.setAge(23);
Personp2=**new**Person();
p2.setName("李昆鹏2");
p2.setAge(24);
//调用序列化流的写入方法
//字节流是不需要刷新的
//写入多个对象
oos.writeObject(p);
oos.writeObject(p1);
oos.writeObject(p2);
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**try**{
**if**(oos!=**null**)
oos.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
/**
*反序列化
*/
**public****static****void**readerObject() {
ObjectInputStreamoos=**null**;
**try**{
//创建序列化流对象
oos=**new**ObjectInputStream(**new**FileInputStream("person.txt"));
//从反序列化的流中来读取数据,返回的是Object类型，需要处理异常
//读取多个对象
Objectobj=oos.readObject();
Objectobj1=oos.readObject();
Objectobj2=oos.readObject();
//当对象已经读取完毕后再读取时就会出现EOFException异常
//所以我们使用循环来读取多个对象时很不方便
//Object obj3 = oos.readObject();
Personp1= (Person)obj;
System.**out**.println(p1);
System.**out**.println(obj1);
System.**out**.println(obj2);
//当对象已经读取完毕后再读取时就会出现EOFException异常
//System.out.println(obj3);
}**catch**(IOExceptione) {
e.printStackTrace();
}**catch**(ClassNotFoundExceptione) {
e.printStackTrace();
}**finally**{
**try**{
**if**(oos!=**null**)
oos.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}

/**
*反序列化
*通过循环来读取多个对象
*/
**public****static****void**readerObject1() {
ObjectInputStreamoos=**null**;
**try**{
//创建序列化流对象
oos=**new**ObjectInputStream(**new**FileInputStream("person.txt"));
//从反序列化的流中来读取数据,返回的是Object类型，需要处理异常
//当通过集合来写入多个对象时，读取时返回的是存储多个对象的集合
//通过集合来读取多个对象
Objectobj=oos.readObject();
//目前的obj是多个对象的集合，我们将它强转并用集合接收
ArrayList<Person>al=(ArrayList<Person>)obj;
//遍历集合
**for**(Personp:al) {
//打印单个对象
System.**out**.println(p);
}

}**catch**(IOExceptione) {
e.printStackTrace();
}**catch**(ClassNotFoundExceptione) {
e.printStackTrace();
}**finally**{
**try**{
**if**(oos!=**null**)
oos.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}

/**
*序列化流
*写入多个对象
*通过集合存储多个对象
*/
**public****static****void**writerObject1() {
ObjectOutputStreamoos=**null**;
**try**{
//创建序列化流对象
oos=**new**ObjectOutputStream(**new**FileOutputStream("person.txt"));
//创建存储Person对象的集合
ArrayList<Person>al=**new**ArrayList<Person>();
//创建多个person类对象，并且给属性赋值
Personp=**new**Person();
p.setName("李昆鹏");
p.setAge(22);
Personp1=**new**Person();
p1.setName("李昆鹏1");
p1.setAge(23);
Personp2=**new**Person();
p2.setName("李昆鹏2");
p2.setAge(24);
//将对象写入集合中
al.add(p);
al.add(p1);
al.add(p2);
//调用序列化流的写入方法
//字节流是不需要刷新的
//写入多个对象对象的集合，通过集合我们可以读取到多个对象
oos.writeObject(al);
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**try**{
**if**(oos!=**null**)
oos.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}


