
# java字符串的作业题字符串分解 - 李昆鹏的博客 - CSDN博客


2018年03月16日 11:19:18[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：250


**-------------------------java字符串的作业题字符串分解---------------**
![](//img-blog.csdn.net/20180316111815552?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
定义一个方法，传入字符串格式为(idcard:name:age:gender)的分割形式如
"120384722312901:kexin:20:1|
3219212131312:haotao:22:1|
120121121312:zhangmeiling:21:2"
将该字符串分解，并将属性赋给定义的Person类的对象上中并输出。
提示：创建的每个Person对象,每个对象有idcard、name、age和gender属性
/**
*创建一个Person类
***@****作者**李昆鹏
*
*/
**public****class**Person {
**private**Stringidcard;
**private**Stringname;
**private**Stringage;
**private**Stringgender;
**public**StringgetIdcard(){
**return**idcard;
}
**public****void**setIdcard(Stringidcard) {
**this**.idcard=idcard;
}
**public**String getName() {
**return**name;
}
**public****void**setName(Stringname) {
**this**.name=name;
}
**public**String getAge() {
**return**age;
}
**public****void**setAge(Stringage) {
**this**.age=age;
}
**public**String getGender() {
**return**gender;
}
**public****void**setGender(Stringgender) {
**this**.gender=gender;
}
@Override
**public**String toString() {
**return**"Person[idcard="+idcard+", name="+name+",age="+age+", gender="+gender+"]";
}

}
-------------------------------------------
**public****class**test12 {
**public****static****void**main(String[]args) {
//定义源字符串
//Stringstr= "120384722312901:kexin:20:1|3219212131312:haotao:22:1|120121121312:zhangmeiling:21:2";
Scannersc=**new**Scanner(System.**in**);
//获得字符串
Stringstr=sc.nextLine();
//判断输入的字符串不是空
Person[]ps=*getPersons*(str);
//打印数组
*printArr*(ps);
}
**public****static**Person[]getPersons(Stringstr) {
Person[]ps=**null**;
//判断输入的字符串不是空
**if**(str!=**null**&& !("".equals(str.trim()))){
//根据|来做字符串的分割
String[]pstrs=str.split("\\|");
//创建一个person类的数组
//注意person类数组虽然有长度但是没有地址，初始值为null
//我们必须new Person的构造器
ps=**new**Person[pstrs.length];
//遍历字符串的数组
**for**(**int**i= 0;i<pstrs.length;i++) {
//获得每一个person的字符串数据
StringpersonStr=pstrs[i];
System.**out**.println(personStr);
//把子字符串按":"来做拆分
//下面的数组中储存的是学号，姓名，年龄，性别
String[]personAttr=personStr.split(":");
//创建person数组对象
ps[i] =**new**Person();
//给学号赋值
ps[i].setIdcard(personAttr[0]);
//给姓名赋值
ps[i].setName(personAttr[1]);
//给年龄赋值
ps[i].setAge(personAttr[2]);
//给性别赋值
ps[i].setGender(personAttr[3]);
}
}
**return**ps;
}
**public****static****void**printArr(Person[]ps) {
**for**(**int**i= 0;i<ps.length;i++) {
System.**out**.println(ps[i]);
}
}
}
结果：
120384722312901:kexin:20:1|3219212131312:haotao:22:1|120121121312:zhangmeiling:21:2
120384722312901:kexin:20:1
3219212131312:haotao:22:1
120121121312:zhangmeiling:21:2
Person[idcard=120384722312901, name=kexin, age=20, gender=1]
Person[idcard=3219212131312, name=haotao, age=22, gender=1]
Person[idcard=120121121312, name=zhangmeiling, age=21, gender=2]


