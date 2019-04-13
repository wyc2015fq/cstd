
# java文件纵横转换作业题 - 李昆鹏的博客 - CSDN博客


2018年04月21日 17:50:01[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：139


--------------------------------------------java文件纵横转换作业题--------------------------------------
![](https://img-blog.csdn.net/20180421174558477?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**作业：把左边的文件转换成右边的样子**
步骤分析
1.    抽象Student类
2.    解析文件成List<Student>
a)      获得每一行，然后按着空格来做切分
3.    把集合写入文件
首先创建Student类
**public****class**Student**implements**Comparable<Student>{
/**
*学号
*/
**private**StringstuNo;
/**
*姓名
*/
**private**Stringname;
/**
*性别
*/
**private**Stringgender;
/**
*年龄
*/
**private**Stringage;
/**
*身高
*/
**private**Stringtall;
/**
*体重
*/
**private**Stringweight;
/**
*电话
*/
**private**Stringtel;
**public**String getStuNo() {
**return**stuNo;
}
**public****void**setStuNo(StringstuNo) {
**this**.stuNo=stuNo;
}
**public**String getName() {
**return**name;
}
**public****void**setName(Stringname) {
**this**.name=name;
}
**public**String getGender() {
**return**gender;
}
**public****void**setGender(Stringgender) {
**this**.gender=gender;
}
**public**String getAge() {
**return**age;
}
**public****void**setAge(Stringage) {
**this**.age=age;
}
**public**String getTall() {
**return**tall;
}
**public****void**setTall(Stringtall) {
**this**.tall=tall;
}
**public**String getWeight() {
**return**weight;
}
**public****void**setWeight(Stringweight) {
**this**.weight=weight;
}
**public**String getTel() {
**return**tel;
}
**public****void**setTel(Stringtel) {
**this**.tel=tel;
}
@Override
**public**String toString() {
**return**"Student [stuNo="+stuNo+",name="+name+", gender="+gender+",age="+age+", tall="+tall
+", weight="+weight+", tel="+tel+"\n]";
}
@Override
**public****int**compareTo(Studento) {
**int**val=**this**.stuNo.compareTo(o.stuNo);
**return**val;
}
}

---------------------------------------------------
再对文件进行解释
**public****class**FileUtils {
**public****static****void**main(String[]args) {
*parseFile*();
/*[Student [stuNo=27s1, name=李四, gender=男, age=20, tall=173cm, weight=70kg,tel=13812653051
], Student [stuNo=36s2, name=张三, gender=男, age=21, tall=179cm, weight=76kg,tel=0432-253111
], Student [stuNo=151s1, name=李玲, gender=女, age=19, tall=166cm, weight=50kg,tel=13921256372
], Student [stuNo=26s2, name=王五, gender=男, age=22, tall=182cm, weight=80kg,tel=13811527635
], Student [stuNo=13s3, name=李强, gender=男, age=24, tall=188cm, weight=79kg,tel=13847473232
]]*/
*writeFile*();
/*学号姓名性别年龄身高体重电话
李强李强男24  188cm   79kg    13847473232
李玲李玲女19  166cm   50kg    13921256372
王五王五男22  182cm   80kg    13811527635
李四李四男20  173cm   70kg    13812653051
张三张三男21  179cm   76kg    0432-253111*/
}
/**
*定义存储学生对象的集合
*/
**static**List<Student>*sList*=**new**ArrayList<Student>();
/**
*解析文件
*/
**public****static****void**parseFile() {
//定义字符缓冲区
BufferedReaderbr=**null**;
**try**{
br=**new**BufferedReader(**new**FileReader("student.txt"));
//定义是否是第一行的变量
**boolean**isFirstRow=**true**;
//定义存储行的变量
Stringline=**null**;
**while**((line=br.readLine()) !=**null**) {
//把每一个行通过空格来切分
String[]arr=line.split(" ");
//如果是第一行
**if**(isFirstRow) {
//遍历第一行的学号
**for**(**int**i= 1;i<arr.length;i++) {
//创建学生对象
Students=**new**Student();
//把学生加入到集合
*sList*.add(s);
}
isFirstRow=**false**;
}
*setAttr*(arr);
}
System.**out**.println(*sList*);
}**catch**(FileNotFoundExceptione) {
e.printStackTrace();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**try**{
**if**(br!=**null**)
br.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
/**
*给Student属性赋值
***@param**arr
*/
**public****static****void**setAttr(String[]arr) {
//获得每一行的头
Stringhead=arr[0];
//循环行的每一个拆分项，从第二项开始
**for**(**int**i= 1;i<arr.length;i++) {
//在集合中根据索引来获得学生对象
Students=*sList*.get(i-1);
**switch**(head) {
**case**"学号":
s.setStuNo(arr[i]);
**break**;
**case**"姓名":
s.setName(arr[i]);
**break**;
**case**"性别":
s.setGender(arr[i]);
**break**;
**case**"年龄":
s.setAge(arr[i]);
**break**;
**case**"身高":
s.setTall(arr[i]);
**break**;
**case**"体重":
s.setWeight(arr[i]);
**break**;
**case**"电话":
s.setTel(arr[i]);
**break**;
}
}
}
/**
*把集合写入文件
*/
**public****static****void**writeFile() {
//对集合进行排序
Collections.*sort*(*sList*);
PrintWriterpw=**null**;
**try**{
pw=**new**PrintWriter("studenResult.txt");
pw.println("学号\t姓名\t性别\t年龄\t身高\t体重\t电话");
**for**(Students:*sList*) {
pw.println(s.getName() +"\t"+s.getName()+"\t"+s.getGender()+"\t"
+s.getAge()+"\t"+s.getTall()+"\t"
+s.getWeight()+"\t"+s.getTel());
pw.flush();
}
}**catch**(FileNotFoundExceptione) {
e.printStackTrace();
}**finally**{
**if**(pw!=**null**)
pw.close();
}
}
}

