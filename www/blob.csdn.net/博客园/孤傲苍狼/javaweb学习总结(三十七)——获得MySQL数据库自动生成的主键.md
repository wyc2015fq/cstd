# javaweb学习总结(三十七)——获得MySQL数据库自动生成的主键 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [javaweb学习总结(三十七)——获得MySQL数据库自动生成的主键](https://www.cnblogs.com/xdp-gacl/p/3983922.html)



　　测试脚本如下：

```
1  create table test1
2 (
3      id int primary key auto_increment,
4      name varchar(20)
5 );
```

　　测试代码：

```
1 package me.gacl.demo;
 2 
 3 import java.sql.Connection;
 4 import java.sql.PreparedStatement;
 5 import java.sql.ResultSet;
 6 import me.gacl.utils.JdbcUtils;
 7 
 8 public class Test {
 9     public static void main(String[] args) {
10         Connection conn = null;
11         PreparedStatement st = null;
12         ResultSet rs = null;
13         try{
14             conn = JdbcUtils.getConnection();
15             String sql = "insert into test1(name) values(?)";
16             st = conn.prepareStatement(sql);
17             st.setString(1, "aaa");
18             st.executeUpdate();
19             //获取数据库自动生成的主键
20             rs = st.getGeneratedKeys();
21             if(rs.next()){
22                 System.out.println(rs.getInt(1));
23             }
24         }catch (Exception e) {
25             e.printStackTrace();
26         }finally{
27             JdbcUtils.release(conn, st, rs);
28         }
29     }
30 }
```











