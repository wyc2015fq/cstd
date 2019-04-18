# JAVA Digester 简介 - z69183787的专栏 - CSDN博客
2015年07月19日 12:39:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：759
JAVA Digester 简介
Digester是apache开源项目Commons中的一个子项目，是一款解析处理XML文档的工具。
Digester处理是基本类似于XML文档树节点遍历的规则来进行处理，底层处理是采用了
SAX(官方Sun的标准)，基于事件驱动的模式。举个例子：
<foo name="The Parent" age="10">
  <bar id="123" title="The First Child"/>
  <bar id="456" title="The Second Child"/>
</foo>
在digester中，定义了一些规则(rule)，对遍历的节点path预先对应好要处理的规则，
即当解析器遍历到某个节点的时候，如果发现当前节点有对应的处理规则，调用相应
的rule进行处理。如上面这个XML例子：
   foo/bar -> ObjectCreatedRule //对象创建规则 
   foo/bar/id -> BeanPropertySetterRule //属性存取规则 
   foo/bar/title -> BeanPropertySetterRule //属性存取规则 
   ... 
Digester解析XML文档的过程，就是在解析XML文档之前，预先向Digester的容器集合
(默认就是RulesBase容器)对XML文档中的节点path注入匹配规则(rule)，然后在解析
文档的时候，遭遇到节点时时，调用SAX中相应的方法，配合操作数栈，根据定义好的
匹配模式(rule)，调用相应规则中的方法，将XML序列化成Java Object。
举个例子：
me2.xml
<address-book name="book">  
  <person id="1" category="acquaintance" try1="would be ignored">
    <name>Gonzo</name>
    <email type="business">gonzo@muppets.com</email>
    <gender result="the whole tag would be ignored">male</gender>
  </person>
  <person id="2" category="rolemodel">
    <name>Kermit</name>
    <email type="business">kermit@muppets.com</email>
    <email type="home">kermie@acme.com</email>
  </person>
</address-book>
public class AddressBook {
 private String name;
 private List<Person> list = new ArrayList<Person>();
 public void setName(String name) {
  this.name = name;
 }
 public void addPerson(Person p) {
  this.list.add(p);
 }
 public void print() {
  System.out.println("============now AddressBook:===============");
  System.out.println("name == " + name);
  System.out.println("List Size == " + list.size());
  System.out.println("============now Person:===============");
  for (int i = 0; i < list.size(); i++) {
   System.out.println("Person " + i);
   list.get(i).print();
  }
 }
}
public class Person {
 private int id;
 private String category;
 private String try1;
 private String name;
 private String gender;
 private Map<String, String> emails = new HashMap<String, String>();
 public void setGender(String gender) {
  this.gender = gender;
 }
 public void setId(int id) {
  this.id = id;
 }
 public void setCategory(String category) {
  this.category = category;
 }
 public void setTry1(String try1) {
  this.try1 = try1;
 }
 public void setName(String name) {
  this.name = name;
 }
 public void add(String key, String value) {
  this.emails.put(key, value);
 }
 public void print() {
  System.out.println("id==" + id + " category=" + category + " try1=" + try1
    +" name=" + name + " gender=" + gender);
   for(Iterator i = emails.keySet().iterator(); i.hasNext(); ) {
           String type = (String) i.next();
           String address = (String) emails.get(type);
           System.out.println("  email (type " + type + ") : " + address);
       }
 }
}
public class Main2 {
 public static void main(String[] args) throws IOException, SAXException {
  String filename = System.getProperty("user.dir")
    + "\\src\\test2\\me2.xml";
  File file = new File(filename);
  Digester d = new Digester();
  AddressBook ab = new AddressBook();
  d.push(ab);
  addRule(d);
  d.parse(file);
  ab.print();
 }
 private static void addRule(Digester d) {
  // 对AddressBook节点注入属性设置规则,下面有详解
  d.addSetProperties("address-book");
  // 当遇到<person>时，创建类Person的一个实例，并将其压入栈顶
  d.addObjectCreate("address-book/person", Person.class);
  // 对person节点注入属性设置规则，即在SAX的事件遭遇到person节点中的Attributes时，
  // 根据属性列表中的属性值对，这儿就是 id="1", category="acquaintance",try1="would be ignored"
  // 使用Java反射(reflection)机制，调用当前栈顶对象即Person实例类中id, category,try1
  // 属性的标准的JavaBean方法，setId, setCategory,setTry1
  d.addSetProperties("address-book/person");
  // 对person节点注入父节点方法调用规则，即在SAX事件遭遇到person节点的时候，调用栈中Person实例的父实例中的addPerson方法。
  d.addSetNext("address-book/person", "addPerson");
  // 对name节点注入方法调用规则,调用当前栈顶对象即Person实例中的setName方法，而此方法的参数即是当前name节点的字符内容。
  // 通常这个规则和addCallParam规则配合使用，这儿是一种特殊情况,即没有多个参数
  d.addCallMethod("address-book/person/name", "setName", 0);
  // 和下面的email一样
  d.addCallMethod("address-book/person/gender", "setGender", 1);
  d.addCallParam("address-book/person/gender", 0, "result");
  // 对email节点注入方法调用规则，调用当前栈顶对象即Person实例中的addEmail方法，此方法需要两个参数，
  // 一个是从属性值的 type属性获取，一个是从email本身的字符内容获取。
  d.addCallMethod("address-book/person/email", "add", 2);
  d.addCallParam("address-book/person/email", 0, "type");
  d.addCallParam("address-book/person/email", 1);
 }
}
