# Java实现mysql存储过程调用 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年02月19日 19:42:28[boonya](https://me.csdn.net/boonya)阅读数：9448







**-------------------------------开篇基础简介--------：**

存储过程通常有以下优点：[复习/更新/记忆/扩容/参考]--你的大脑有多大内存？

(1).存储过程增强了SQL语言的功能和灵活性。存储过程可以用流控制语句编写，有很强的灵活性，可以完成复杂的判断和较复杂的运算。

(2).存储过程允许标准组件是编程。存储过程被创建后，可以在程序中被多次调用，而不必重新编写该存储过程的SQL语句。而且数据库专业人员可以随时对存储过程进行修改，对应用程序源代码毫无影响。

(3).存储过程能实现较快的执行速度。如果某一操作包含大量的Transaction-SQL代码或分别被多次执行，那么存储过程要比批处理的执行速度快很多。因为存储过程是预编译的。在首次运行一个存储过程时查询，优化器对其进行分析优化，并且给出最终被存储在系统表中的执行计划。而批处理的Transaction-SQL语句在每次运行时都要进行编译和优化，速度相对要慢一些。

(4).存储过程能过减少网络流量。针对同一个数据库对象的操作（如查询、修改），如果这一操作所涉及的Transaction-SQL语句被组织程存储过程，那么当在客户计算机上调用该存储过程时，网络中传送的只是该调用语句，从而大大增加了网络流量并降低了网络负载。

(5).存储过程可被作为一种安全机制来充分利用。系统管理员通过执行某一存储过程的权限进行限制，能够实现对相应的数据的访问权限的限制，避免了非授权用户对数据的访问，保证了数据的安全。

**>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>procedure>mysql**

**1、语法：**

        删除> drop procedure if exists procedureName;

        创建> create procedure(IN/OUT/INOUT  参数名称     数据类型)  BEGIN  .... END;

     ----说明：

     IN 输入参数:表示该参数的值必须在调用存储过程时指定，在存储过程中修改该参数的值不能被返回，为默认值

     OUT 输出参数:该值可在存储过程内部被改变，并可返回

     INOUT 输入输出参数:调用时指定，并且可被改变和返回

        调用存储过程>   call procedureName()/call procedureName(params);
**2、mysql的变量声明：**

  DECLARE  variable_name[,variable_name2,variable_name3...] 数据类型  [DEFAULT variable_value];


  []的内容表示可选。来看几个示例：

    DECLARE my_date date DEFAULT '2012-12-31';  

    DECLARE my_datetime datetime DEFAULT '2012-12-31 23:59:59';  

    DECLARE my_varchar varchar(255) DEFAULT 'This will not be padded';  
**3、变量赋值**

  SET variable_name=variable_value;
**4、mysql内置函数**

 ------------------字符操作函数----------------------------

 CHARSET(str) //返回字串字符集

 CONCAT (string2 [,... ]) //连接字串

 INSTR (string ,substring ) //返回substring首次在string中出现的位置,不存在返回0

 LCASE (string2 ) //转换成小写

 LEFT (string2 ,length ) //从string2中的左边起取length个字符

 LENGTH (string ) //string长度

 LOAD_FILE (file_name ) //从文件读取内容

 LOCATE (substring , string [,start_position ] ) 同INSTR,但可指定开始位置

 LPAD (string2 ,length ,pad ) //重复用pad加在string开头,直到字串长度为length

 LTRIM (string2 ) //去除前端空格

 REPEAT (string2 ,count ) //重复count次

 REPLACE (str ,search_str ,replace_str ) //在str中用replace_str替换search_str

 RPAD (string2 ,length ,pad) //在str后用pad补充,直到长度为length

 RTRIM (string2 ) //去除后端空格

 STRCMP (string1 ,string2 ) //逐字符比较两字串大小,

 SUBSTRING (str , position [,length ]) //从str的position开始,取length个字符,

         注：mysql中处理字符串时，默认第一个字符下标为1，即参数position必须大于等于1

 TRIM([[BOTH|LEADING|TRAILING] [padding] FROM]string2) //去除指定位置的指定字符

 UCASE (string2 ) //转换成大写

 RIGHT(string2,length) //取string2最后length个字符

 SPACE(count) //生成count个空格     

---------------------------数学函数---------------------------------

 ABS (number2 ) //绝对值

 BIN (decimal_number ) //十进制转二进制

 CEILING (number2 ) //向上取整

 CONV(number2,from_base,to_base) //进制转换

 FLOOR (number2 ) //向下取整

 FORMAT (number,decimal_places ) //保留小数位数

 HEX (DecimalNumber ) //转十六进制

        注：HEX()中可传入字符串，则返回其ASC-11码，如HEX('DEF')返回4142143

       也可以传入十进制整数，返回其十六进制编码，如HEX(25)返回19

 LEAST (number , number2 [,..]) //求最小值

 MOD (numerator ,denominator ) //求余

 POWER (number ,power ) //求指数

 RAND([seed]) //随机数

 ROUND (number [,decimals ]) //四舍五入,decimals为小数位数]

           注：返回类型并非均为整数，如：

    (1)默认变为整形值

         select round(1.23);   //打印输出1

         select round(1.56);   //打印输出2

     (2)可以设定小数位数，返回浮点型数据

        select round(1.567,2); //打印输出1.57

 SIGN (number2 ) //在进行SQL查询时，我们通常会有这样一种需求：

        对一个自然数进行判断，如果为零，返回0，如果为负数，统一返回-1， 如果为正数，统一返回1。

-----------------------------日期函数-------------------------------------

 ADDTIME (date2 ,time_interval ) //将time_interval加到date2

 CONVERT_TZ (datetime2 ,fromTZ ,toTZ ) //转换时区

 CURRENT_DATE ( ) //当前日期

 CURRENT_TIME ( ) //当前时间

 CURRENT_TIMESTAMP ( ) //当前时间戳

 DATE (datetime ) //返回datetime的日期部分

 DATE_ADD (date2 , INTERVAL d_value d_type ) //在date2中加上日期或时间

 DATE_FORMAT (datetime ,FormatCodes ) //使用formatcodes格式显示datetime

 DATE_SUB (date2 , INTERVAL d_value d_type ) //在date2上减去一个时间

 DATEDIFF (date1 ,date2 ) //两个日期差

 DAY (date ) //返回日期的天

 DAYNAME (date ) //英文星期

 DAYOFWEEK (date ) //星期(1-7) ,1为星期天

 DAYOFYEAR (date ) //一年中的第几天

 EXTRACT (interval_name FROM date ) //从date中提取日期的指定部分

 MAKEDATE (year ,day ) //给出年及年中的第几天,生成日期串

 MAKETIME (hour ,minute ,second ) //生成时间串

 MONTHNAME (date ) //英文月份名

 NOW ( ) //当前时间

 SEC_TO_TIME (seconds ) //秒数转成时间

 STR_TO_DATE (string ,format ) //字串转成时间,以format格式显示

 TIMEDIFF (datetime1 ,datetime2 ) //两个时间差

 TIME_TO_SEC (time ) //时间转秒数]

 WEEK (date_time [,start_of_week ]) //第几周

 YEAR (datetime ) //年份

 DAYOFMONTH(datetime) //月的第几天

 HOUR(datetime) //小时

 LAST_DAY(date) //date的月的最后日期

 MICROSECOND(datetime) //微秒

 MONTH(datetime) //月

 MINUTE(datetime) //分返回符号,正负或0

 SQRT(number2) //开平方
**5、在存储过程中使用逻辑判断**

        还是从实例中看比较直观：

   (1)、if then else  end if

     CREATE PROCEDURE proc2(IN parameter int)  

      begin 

       declare var int;  

       set var=parameter+1;  

       if var=0 then 

          insert into t values(17);  

       end if;  

       if parameter=0 then 

          update t set s1=s1+1;  

       else 

         update t set s1=s1+2;  

       end if;  

     end;  

   (2)、case语句

     CREATE PROCEDURE proc3 (in parameter int)  

      begin 

       declare var int;  

       set var=parameter+1;  

       case var  

         when 0 then   

            insert into t values(17);  

         when 1 then   

            insert into t values(18);  

         else   

            insert into t values(19);  

       end case;  

      end;  

   (3)、while...  end while

    CREATE PROCEDURE proc4()  

      begin 

        declare var int;  

        set var=0;  

        while var<6 do  

           insert into t values(var);  

           set var=var+1;  

        end while;  

      end;  

   (4)、loop ... end loop

     CREATE PROCEDURE proc5 ()  

      begin 

        declare v int;  

        set v=0;  

        LOOP_LABLE:loop  

          insert into t values(v);  

          set v=v+1;  

          if v >=5 then 

             leave LOOP_LABLE;  

          end if;  

        end loop;  

      end;  

    (5)、迭代Iterate

      CREATE PROCEDURE proc6 ()  

        begin 

         declare v int;  

         set v=0;  

         LOOP_LABLE:loop  

          if v=3 then   

             set v=v+1;  

             ITERATE LOOP_LABLE;  

           end if;  

           insert into t values(v);  

           set v=v+1;  

           if v>=5 then 

             leave LOOP_LABLE;  

           end if;  

        end loop;  

      end;  
**6、Java调用存储过程访问mysql示例**

(1)、定义不同数据库实现接口（适用于mysql，sqlserver，oracle等数据库存储过程）

```java
package com.boonya.procedure.service;
/**
 * 文件：ProcedureService.java
 * 功能: 实现mysql数据库存储过程的操作
 * 注释：对于功能相同而实现类型不同而言最好先定义接口
 * @author PengJunlin
 * @date   2013-02-19
 */
public interface ProcedureService {
     void createProcedure(String myprocedure,String procedureName);
     void usingProcedure(String myprocedure,String param);
     void alertProcedure(String myprocedure,String procedureName);
     void dropProcedure(String procedureName);
}
```



（2）、实现接口和测试

```java
package com.boonya.procedure.mysql;
import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import com.boonya.procedure.service.ProcedureService;
import com.boonya.procedure.utils.JDBCConnection;
/**
 * 文件：MysqlProcedure.java
 * 功能: 实现mysql数据库存储过程的操作
 * 注释：对于功能相同而实现类型不同而言最好先定义接口，已实现接口
 * @author PengJunlin
 * @date   2013-02-19
 */
public class MysqlProcedure implements ProcedureService{
    
    static CallableStatement cstmt=null;
    static Connection conn=null;
    static Statement stmt=null;
    static ResultSet rs=null;
    
    static{
        JDBCConnection.setConn("mysql");
        conn=JDBCConnection.getConn();
    }
    /**
     * 创建数据库表///温故而知新
     */
    public  void createTable(String createSql,String tableName){
        try {
            stmt=conn.createStatement();
            stmt.execute("drop table if exists "+tableName+" ;");
            stmt.executeUpdate(createSql);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
    
    public  void createProcedure(String myprocedure,String procedureName){
        try {
            stmt=conn.createStatement();
            stmt.execute("drop procedure if exists "+procedureName+" ;");
            stmt.executeUpdate(myprocedure);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
    
    public  void alertProcedure(String myprocedure,String procedureName){
        try {
            stmt=conn.createStatement();
            stmt.execute("drop procedure if exists "+procedureName+" ;");
            stmt.executeUpdate(myprocedure);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
    
    public  void usingProcedure(String procedureName,String param){
        try {
            cstmt=conn.prepareCall("call "+procedureName+"("+param+")");
            rs=cstmt.executeQuery();
            System.out.println("存储过程查询结果：");
            while (rs.next()) {
                System.out.println(rs.getInt("sno")+" "+rs.getString("sname")
                    +" "+rs.getInt("sage")+" "+rs.getString("sdept"));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
    
    public  void dropProcedure(String procedureName){
        try {
            String dorpProcedure="drop procedure if exists "+procedureName+" ;";
            stmt=conn.createStatement();
            stmt.executeUpdate(dorpProcedure);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

}

class MysqlProcedureTest{
    public static void main(String[] args) {
                MysqlProcedure mysqlProcedure=new MysqlProcedure();
                String mytableString=" create table mytable_mysql "        //建表语句
                +"("
                     +" uid int(11) primary key ,"
                     +" uname varchar(30) ,"
                     +" uage int(11) , "
                     +" ubackground varchar(100)"
                 +");";                                
                mysqlProcedure.createTable(mytableString,"mytable_mysql"); //创建数据表
                String myprocedure=" create procedure snoquery(IN no int) "//存储过程语句
                       +" BEGIN"
                       +" declare s_no int;"
                       +" set s_no=no;"
                       +" select sno,sname,sage,ssex,sdept "
                       +" from student where sno=s_no ;"
                       +" END;";
                mysqlProcedure.createProcedure(myprocedure,"snoquery");    //创建存储过程
                mysqlProcedure.usingProcedure("snoquery","12");            //调用存储过程
                String alertProcedure="create procedure snoquery(IN name varchar(30))"
                       +" BEGIN"
                       +" declare s_name varchar(30);"
                       +" set s_name=name;"
                       +" select sno,sname,sage,ssex,sdept "
                       +" from student  where sname=s_name;"
                       +" END;";
                mysqlProcedure.alertProcedure(alertProcedure,"snoquery");  //修改存储过程
                mysqlProcedure.usingProcedure("snoquery","'boonya007'");   //调用存储过程
                mysqlProcedure.dropProcedure("snoquery");                  //删除存储过程
    }
}
```



      注意：(1) 删除和创建数据库表或存储过程是两个操作，不能在同一个数据库声明语句中执行，否则报SQL语法错误。

        (2)java中调用存储过程时，如果参数是字符串一定要加单引号,如："'sds'"；否则抛出异常：Unknown column '字符串参数' in 'field list'
            


