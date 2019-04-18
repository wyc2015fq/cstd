# JDBC查询操作的封装的两种方法（匿名内部类和反射） - 苝花向暖丨楠枝向寒 - CSDN博客

2018年02月26日 14:48:03[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：148


JDBC 查询操作的封装 可以分为两种

一：创建 一个接口， 通过创建匿名内部类，重写接口的方法，实现 的效果相当于 方法的传递。

二：通过反射。

代码如下：

方法一：

dao层

   给出sql语句，以及给出要查询的实体，同时重写接口中的方法(该方法针对某一个实体解析查询的结果并放入集合当中)

return jdbcUtil.executeQuery("Select id,name,b_id,gerden from student", new RowMap<Student>() {

@Override
public Student rowMapping(ResultSet rs) {

Student student = new Student();

try {

student.setId(rs.getInt("id"));
student.setName(rs.getString("name"));
student.setB_id(rs.getInt("b_id"));
student.setGerden(rs.getInt("gerden"));
} catch (SQLException e) {

e.printStackTrace();
}

return student;
}

}, null);

jdbcUtil

    public static<T> List<T> executeQuery(String sql,RowMap<T> rowMap,Object...params){

List<T> list = new ArrayList<>();

Connection con = getConnection();

PreparedStatement pstmt = null;

ResultSet rs = null;

try {

pstmt=con.prepareStatement(sql);

if(params!=null) {

for(int i=0;i<params.length;i++) {

pstmt.setObject(i+1,params[i]);
}
}

rs = pstmt.executeQuery();

while(rs.next()) {
T t = rowMap.rowMapping(rs);     //此处调用Dao层重写的rowMapping方法。并且此时T类型已经被确定
list.add(t);
}

} catch (SQLException e) {

e.printStackTrace();

}finally {

close(con, rs, pstmt);
}
return list;

    }

RowMap接口

目的是通过接口，将接口作为一个参数传入到jdbcUtil中封装的方法，在调用该方法的时候指明要查询的表，即对应的实体类，这样就可以根据不同的实体类来解析不同的rs。

public interface RowMap<T> {

public T rowMapping(ResultSet rs);

}


方法二：

dao层

return jdbcUtil.executeQuery1("Select id,name,b_id,gerden from student", Student.class, null);

jdbcUtil

  public static<T> List<T> executeQuery1(String sql,Class<T> clz,Object...params){

List<T> list = new ArrayList<>();

Connection con = getConnection();

PreparedStatement pstmt = null;

ResultSet rs = null;

try {

pstmt=con.prepareStatement(sql);

if(params!=null) {

for(int i=0;i<params.length;i++) {

pstmt.setObject(i+1,params[i]);
}
}

rs = pstmt.executeQuery();

while(rs.next()) {

//创建该类的对象
T t = clz.newInstance();

//获取属性
Field[] fields = clz.getDeclaredFields();

//遍历属性
for (Field field : fields) {

//获取注解
Colum col = field.getDeclaredAnnotation(Colum.class);

//判断注解是否为空，为空就用实体中的属性名，不为空的用注释给出的属性名
String colum=col==null?field.getName():col.value();

//将rs 返回结果中所包含的字段 赋值给 Student 属性， 没有的则不赋值
if(check(rs, colum)) {

//给每一个属性 赋予 被更改的权限
field.setAccessible(true);

//哪个属性？  哪个对象？  赋予什么值？ 
field.set(t,rs.getObject(colum));
}

}

list.add(t);
}

} catch (SQLException e) {

e.printStackTrace();

} catch (InstantiationException e) {
// TODO Auto-generated catch block
e.printStackTrace();
} catch (IllegalAccessException e) {
// TODO Auto-generated catch block
e.printStackTrace();
}finally {

close(con, rs, pstmt);
}
return list;

    }

//判断rs中 是否有某一个 数据库中对应的字段，如果有则返回true，然后在executeQuery1中给实体的属性赋值。

public static boolean check(ResultSet rs,String colum) {

boolean result = true;

try {

rs.findColumn(colum);

} catch (SQLException e) {

result=false;
e.printStackTrace();
}

return result;
    } 

自定义注释

@Target(value =ElementType.FIELD)       //指出注解位置
@Retention(RetentionPolicy.RUNTIME)  //指出注解作用域
public @interface Colum {

String value();

}

实体

import lombok.Data;
import util.Colum;
@Data         //直接可以用get、set 方法，并且通过foreach 遍历 集合然后 输出的时候不会打印出地址
public class Student {
private Integer id;
private String name;
@Colum(value = "b_id")      
private Integer b_id;        //此处的属性，如果和数据库里的字段名字不一样，则可以通过注解来标明
private Integer gerden;

public void fun(String a) {

System.out.println(a);
}
}

