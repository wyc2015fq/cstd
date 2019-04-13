
# java集合中的泛型的使用 - 李昆鹏的博客 - CSDN博客


2018年03月24日 23:30:38[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：74


**-----------------------java集合中的泛型的使用------------------------**
**我们在项目中使用集合基本****99%****都是在集合中存储同一种数据类型。既然我们在集合中存储同一种数据类型，我们事先一定知道这个集合中要存储什么数据类型。我们就可以预先去指定要存储的数据类型。**
**泛型：就是提前指定要操作的数据类型。**
**语法：**
**在定义集合的时候**
**List<****数据类型****>****变量名****= new ArrayList<****数据类型****>();**
![](https://img-blog.csdn.net/20180324232533907?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
![](https://img-blog.csdn.net/20180324232541466?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
例子：
**public****class**Generic {
**public****static****void**main(String[]args) {
//集合中可以存储任何数据类型
//实际情况都是在集合中存一种数据类型
//我们可以通过泛型来指定这种数据类型
Collection<String>sc=**new**ArrayList<String>();
sc.add("曹盖");
//通过泛型指定添加类型后不能再添加其他类型
//sc.addAll(1);
//指定集合中能添加学生类的对象
Collection<Student>sc1=**new**ArrayList<Student>();
sc1.add(**new**Student(1,"宋江",1,**new**Date()));
System.**out**.println(sc1);
//在创建一个集合
Collection<Student>sc2=**new**ArrayList<Student>();
sc2.add(**new**Student(1,"宋江",1,**new**Date()));
System.**out**.println(sc2);
//把sc2加入到sc1
sc1.addAll(sc2);
System.**out**.println(sc1);
//创建一个小学生集合
Collection<ChildStudent>csc=**new**ArrayList<ChildStudent>();
ChildStudentcstu=**new**ChildStudent();
cstu.setLhq(13f);
cstu.setId(3);
cstu.setName("小明");
cstu.setGender(1);
cstu.setBirthday(**new**Date());
csc.add(cstu);
sc1.addAll(csc);
System.**out**.println(sc1);
}
}
结果：
[Student [id=1, name=宋江, gender=1, birthday=Sat Mar 24 22:46:31 CST 2018]]
[Student [id=1, name=宋江, gender=1, birthday=Sat Mar 24 22:46:31 CST 2018]]
[Student [id=1, name=宋江, gender=1, birthday=Sat Mar 24 22:46:31 CST 2018],Student [id=1, name=宋江, gender=1, birthday=Sat Mar 24 22:46:31 CST2018]]
[Student [id=1, name=宋江, gender=1, birthday=Sat Mar 24 22:46:31 CST 2018],Student [id=1, name=宋江, gender=1, birthday=Sat Mar 24 22:46:31 CST2018], ChildStudent [lhq=13.0, toString()=Student [id=3, name=小明, gender=1, birthday=Sat Mar 24 22:46:31 CST 2018]]]

迭代器也带泛型：
例子：
public class GenericDemo1 {
public static void main(String[] args) {
//集合中可以存储任何数据类型
//实际情况都是在集合中存一种数据类型
//我们可以通过泛型来指定这种数据类型
Collection<String>sc = new ArrayList<String>();
sc.add("罩盖");
sc.add("鲁智深");
sc.add("林冲");
sc.add("王伦");
//迭代器也带泛型
Iterator<String> iter =sc.iterator();
while(iter.hasNext()) {
Stringstr = iter.next();
System.out.println(str);
}
System.out.println("-------------------------");
//创建一个List集合
List<String> list = newArrayList<String>();
list.add("罩盖");
list.add("鲁智深");
list.add("林冲");
list.add("王伦");
//使用size（）方法遍历
for (int i = 0; i < list.size();i++) {
Stringstr2 = list.get(i);
System.out.println(str2);
}

}
}
结果：
罩盖
鲁智深
林冲
王伦
-------------------------
罩盖
鲁智深
林冲
王伦
**------------------------------自定义泛型---------------------------**
**在自定义泛型时**
**语法：****class/interface****类名****/****接口名****<T>{**
**}**
**T****只是泛型的一个标准，使用什么字符都可以，但是都要大写，不要使用特殊字符，建议用****T****。**
创建一个自定义泛型类：
**public****class**GenericTest<T>  {
**private**T[]arr;
**public**T[] getArr() {
**return**arr;
}
**public****void**setArr(T[]arr) {
**this**.arr=arr;
}
**public**T getByIndex(**int**index) {
**return**arr[index];
}
}
-------------------------------------
测试自定义泛型：
**public****class**GenericTestMain  {
**public****static****void**main(String[]args) {
GenericTest<String>gt=**new**GenericTest<String>();
String[]arr=**new**String[3];
arr[0] ="蔡京";
arr[1] ="高俅";
arr[2] ="高衙内";
gt.setArr(arr);
Stringstr=gt.getByIndex(2);
System.**out**.println(str);
System.**out**.println("------------------");
GenericTest<Student>gt1=**new**GenericTest<Student>();
Student[]stuArr=**new**Student[3];
stuArr[0] =**new**Student(1,"蔡京", 1,**new**Date());
stuArr[1] =**new**Student(2,"蔡九", 1,**new**Date());
stuArr[2] =**new**Student(3,"潼关", 1,**new**Date());
gt1.setArr(stuArr);
Students=gt1.getByIndex(2);
System.**out**.println(s);
}
}
结果：
高衙内
------------------
Student [id=3, name=潼关, gender=1, birthday=Sat Mar 24 23:07:21 CST 2018]

**---------------------------增强for循环---------------------------------**
**在****jdk1.5****以后出现了增强****for****循环**
**对数组、集合来做遍历**
**语法：**
**For(****数据类型****变量名****：****集合变量名****){**
**//**
**}**
**遍历时存在并发问题**
**例子：**
/**
* 增强for循环
* @作者 李昆鹏
*
*/
public class ForDemo {
public static void main(String[] args) {
Collection<String> sc = new ArrayList<String>();
sc.add("罩盖");
sc.add("鲁智深");
sc.add("林冲");
sc.add("王伦");
**//****增强****for****循环**
**/****
*******语句：**
***   for(****数据类型****变量****：****集合****){**
*****
***   }**
***/**
**//****缺点：增强的****for****循环有并发问题存在**
for(String s:sc) {
System.out.println(s);
}
Collection<Student> sc1 = newArrayList<Student>();
sc1.add(new Student(1, "宋江", 1, new Date()));
sc1.add(new Student(2, "蔡京", 1, new Date()));
sc1.add(new Student(3, "戴宗", 1, new Date()));
sc1.add(new Student(4, "张顺", 1, new Date()));
for(Student s:sc1) {
System.out.println(s);
}
}
}
结果：
罩盖
鲁智深
林冲
王伦
Student [id=1, name=宋江, gender=1, birthday=Sat Mar 24 23:19:48 CST 2018]
Student [id=2, name=蔡京, gender=1, birthday=Sat Mar 24 23:19:48 CST 2018]
Student [id=3, name=戴宗, gender=1, birthday=Sat Mar 24 23:19:48 CST 2018]
Student [id=4, name=张顺, gender=1, birthday=Sat Mar 24 23:19:48 CST 2018]


