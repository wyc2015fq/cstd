# jdbc篇第3课：使用jdbc做查询操作 - PeterBishop - CSDN博客





2018年11月27日 19:08:52[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：19
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  这节课我们来讲如何用jdbc做查询操作:



Demo 01:
package jdbc;



import jdbc.bean.Employee;



import java.sql.*;

import java.util.ArrayList;

import java.util.List;



//Jdbc查询教学

public class JdbcQueryTeach {



    public static void main(String[] args) throws SQLException {

        */**         * jdbc**查询操作:         * 1. 建立好表对应的类(我们称为bean)，要求每一列对应一个属性，且数据类型对应上         * 2. 建立连接，获取Connection对象         * 3. 使用Connection对象获取Statement或PreparedStatement对象         * 4. 写好sql         * 5. 使用Statement或PreparedStatement执行查询语句，获取ResultSet对象         * 6. 建一个或多个bean对象         * 7. 获取ResultSet里存储的值，并封装到bean对象中         * 8. 关闭连接         */        *Connection connection = null;

        try {



            //2. 获取Connection连接

            Class.*forName*("com.mysql.jdbc.Driver");

            String url = "jdbc:mysql://localhost:3306/teach";



            String user = "root";

            String password = "root";

            connection = DriverManager.*getConnection*(url, user, password);



            //3. 使用Connection对象获取Statement或PreparedStatement对象

            Statement statement = connection.createStatement();



            //4. 写好sql

            //暂时取5条数据

            String sql = "select * from tbl_employee where id <= 5";



            //5. 使用Statement或PreparedStatement对象执行sql,获得ResultSet对象

            ResultSet resultSet = statement.executeQuery(sql);



            //6. 建立一个或多个bean对象

            List<Employee> employeeList = new ArrayList<>();

            //看来没有直接获取记录数的方法

            //next()方法就是获取下一条记录，如果下一条记录存在就返回true，并且自己执行该条记录

            //如果没有下一条记录就返回false

            while (resultSet.next())

            {

                Employee employee = new Employee();

                */**                 *  ResultSet.getXxx()                 *                 *  **可接受参数:                 *  columnIndex: 表示第几列，从1开始                 *  columnLabel: 表示列名                 *                 *  返回值:                 *  根据列索引或者列名返回Xxx类型的对象                 */                *//获取ResultSet里存储的值，并封装到bean对象中

                employee.setId(resultSet.getInt(1));

                employee.setLastName(resultSet.getString(2));

                employee.setEmail(resultSet.getString(3));

                employee.setGender(resultSet.getString(4));

                employee.setdId(resultSet.getInt(5));



                employeeList.add(employee);

            }



            for (Employee employee : employeeList) {

                System.*out*.println(employee.toString());

            }

        } catch (ClassNotFoundException e) {

            e.printStackTrace();

        } catch (SQLException e) {

            e.printStackTrace();

        }finally {

            if (connection != null)

                connection.close();//这个异常没必要捕获，有需要的时候再捕获

        }



    }

}


结果:

Employee{id=1, lastName='aa', email='aa@sina.com', gender='1', dId=1001}

Employee{id=2, lastName='bb', email='bb@sina.com', gender='0', dId=1003}

Employee{id=3, lastName='cc', email='cc@sina.com', gender='0', dId=1002}

Employee{id=4, lastName='dd', email='dd@sina.com', gender='1', dId=1001}

Employee{id=5, lastName='ee', email='ee@sina.com', gender='0', dId=1003}





Demo 02:
package jdbc;



import com.sun.xml.internal.bind.v2.model.core.ID;

import jdbc.bean.Employee;



import java.sql.*;

import java.util.ArrayList;

import java.util.List;



//这个专门讲PreparedStatement

public class JdbcPreparedStatementTeach {


public static void main(String[] args) throws Exception{

        */**         * 1. public interface PreparedStatement extends Statement         *      PreparedStatement**是Statement的子接口         * 2. 所以PreparedStatement和Statement使用方法及其相似         * 3. PreparedStatement和Statement的区别在于         *      PreparedStatement支持预处理的sql，即支持用?当做占位符的sql         *      Statement对象是使用字符串拼接的方式来构造sql语句         */        *Employee employee1 = *selectByIdUseStatement*(1);

        Employee employee2 = *selectByIdUsePreparedStatement*(2);



        System.*out*.println(employee1);

        System.*out*.println(employee2);

    }



    public static Employee selectByIdUseStatement(int id) throws SQLException {

        Connection connection = null;

        try {



            //2. 获取Connection连接

            Class.*forName*("com.mysql.jdbc.Driver");

            String url = "jdbc:mysql://localhost:3306/teach";



            String user = "root";

            String password = "root";

            connection = DriverManager.*getConnection*(url, user, password);



            //3. 使用Connection对象获取Statement或PreparedStatement对象

            Statement statement = connection.createStatement();



            //4. 写好sql

            //暂时取5条数据

            //Statement对象是使用字符串拼接的方式来构造sql语句

            String sql = "select * from tbl_employee where id = " + id;

            

            //5. 使用Statement或PreparedStatement对象执行sql,获得ResultSet对象

            //ResultSet对象一开始指向的是第0行(其实就是没有数据)

            ResultSet resultSet = statement.executeQuery(sql);

            resultSet.next();



            //6. 建立一个或多个bean对象

            Employee employee = new Employee();



            employee.setId(resultSet.getInt(1));

            employee.setLastName(resultSet.getString(2));

            employee.setEmail(resultSet.getString(3));

            employee.setGender(resultSet.getString(4));

            employee.setdId(resultSet.getInt(5));



            return  employee;

        } catch (ClassNotFoundException e) {

            e.printStackTrace();

        } catch (SQLException e) {

            e.printStackTrace();

        }finally {

            if (connection != null)

                connection.close();//这个异常没必要捕获，有需要的时候再捕获

        }

        return null;

    }



    public static Employee selectByIdUsePreparedStatement(int id) throws SQLException {

        Connection connection = null;

        try {



            //2. 获取Connection连接

            Class.*forName*("com.mysql.jdbc.Driver");

            String url = "jdbc:mysql://localhost:3306/teach";



            String user = "root";

            String password = "root";

            connection = DriverManager.*getConnection*(url, user, password);



            //4. 写好sql

            //暂时取5条数据

            //PreparedStatement使用方法和Statement基本相似

            //但是PreparedStatement支持使用？当做占位符

            //等下，再暂停下

            String sql = "select * from tbl_employee where id = ?";



            //获取PreparedStatement时强制要求传入一个sql语句

            PreparedStatement preparedStatement = connection.prepareStatement(sql);



            //刚才写好的sql中有一个占位符

            */**             * PreparedStatement**的setXxx(int parameterIndex, Xxx x)方法             * 参数:             *  1. parameterIndex 参数索引，表示要设置第几个参数，从1开始计数             *  2. x ，Xxx类型，要传入的对象             */            *preparedStatement.setInt(1,id);



            //5. 使用Statement或PreparedStatement对象执行sql,获得ResultSet对象

            //发现问题了， PreparedStatement接口重载了executeQuery()方法

            //PreparedStatement提供了一个无参的executeQuery()方法

            //如果传参数，调用的是父接口的那个executeQuery

            //sql是在获取PreparedStatement对象时传入的

            //因此在调用PreparedStatement对象的executeQuery方法时不需要传参数

            ResultSet resultSet = preparedStatement.executeQuery();



            resultSet.next();

            //6. 建立一个或多个bean对象

            Employee employee = new Employee();

            employee.setId(resultSet.getInt(1));

            employee.setLastName(resultSet.getString(2));

            employee.setEmail(resultSet.getString(3));

            employee.setGender(resultSet.getString(4));

            employee.setdId(resultSet.getInt(5));



            return  employee;

        } catch (ClassNotFoundException e) {

            e.printStackTrace();

        } catch (SQLException e) {

            e.printStackTrace();

        }finally {

            if (connection != null)

                connection.close();//这个异常没必要捕获，有需要的时候再捕获

        }

        return null;

    }

}


结果:

Employee{id=1, lastName='aa', email='aa@sina.com', gender='1', dId=1001}

Employee{id=2, lastName='bb', email='bb@sina.com', gender='0', dId=1003}





Demo 03:
public static void main(String[] args) throws Exception{

    */**     * 1. public interface PreparedStatement extends Statement     *      PreparedStatement**是Statement的子接口     * 2. 所以PreparedStatement和Statement使用方法及其相似     * 3. PreparedStatement和Statement的区别在于     *      PreparedStatement支持预处理的sql，即支持用?当做占位符的sql     *      Statement对象是使用字符串拼接的方式来构造sql语句     */    *Employee employee = new Employee(1,"xiaoye","xiaoye@qq.com","1",1001);

    *updateEmployeeUsePreparedStatement*(employee);

}



public static void updateEmployeeUsePreparedStatement(Employee employee) throws SQLException {

    //?占位符不仅可以用在select语句的where里，还可以用在update的set等地方，这里就拿update语句举例

    Connection connection = null;

    try {



        //2. 获取Connection连接

        Class.*forName*("com.mysql.jdbc.Driver");

        String url = "jdbc:mysql://localhost:3306/teach";



        String user = "root";

        String password = "root";

        connection = DriverManager.*getConnection*(url, user, password);



        //4. 写好sql

        //暂时取5条数据

        //PreparedStatement使用方法和Statement基本相似

        //但是PreparedStatement支持使用？当做占位符


        String sql = "update tbl_employee set last_name=?,email=?,gender=?,d_id=? where id = ?";



        //获取PreparedStatement时强制要求传入一个sql语句

        PreparedStatement preparedStatement = connection.prepareStatement(sql);



        //刚才写好的sql中有一个占位符

        */**         * PreparedStatement**的setXxx(int parameterIndex, Xxx x)方法         * 参数:         *  1. parameterIndex 参数索引，表示要设置第几个参数，从1开始计数         *  2. x ，Xxx类型，要传入的对象         */        *preparedStatement.setString(1,employee.getLastName());

        preparedStatement.setString(2,employee.getEmail());

        preparedStatement.setString(3,employee.getGender());

        preparedStatement.setInt(4,employee.getdId());

        preparedStatement.setInt(5,employee.getId());  //这里方法调错了



        //5. 使用Statement或PreparedStatement对象执行sql,获得ResultSet对象

        //发现问题了， PreparedStatement接口重载了executeQuery()方法

        //PreparedStatement提供了一个无参的executeQuery()方法

        //如果传参数，调用的是父接口的那个executeQuery

        //sql是在获取PreparedStatement对象时传入的

        //因此在调用PreparedStatement对象的executeQuery方法时不需要传参数

        preparedStatement.execute();

        //暂停下



    } catch (ClassNotFoundException e) {

        e.printStackTrace();

    } catch (SQLException e) {

        e.printStackTrace();

    }finally {

        if (connection != null)

            connection.close();//这个异常没必要捕获，有需要的时候再捕获

    }

}


原来的数据:



![](https://img-blog.csdnimg.cn/20181127184424683.png)

执行后:



![](https://img-blog.csdnimg.cn/20181127184430135.png)

可以看到id为1的这行数据已经被更改了



总结:

 jdbc查询操作:

     1. 建立好表对应的类(我们称为bean)，要求每一列对应一个属性，且数据类型对应上

     2. 建立连接，获取Connection对象

     3. 使用Connection对象获取Statement或PreparedStatement对象

     4. 写好sql

     5. 使用Statement或PreparedStatement执行查询语句，获取ResultSet对象

     6. 建一个或多个bean对象

     7. 获取ResultSet里存储的值，并封装到bean对象中

     8. 关闭连接


- public interface PreparedStatement extends Statement

	     PreparedStatement是Statement的子接口

	2. 所以PreparedStatement和Statement使用方法及其相似

	3. PreparedStatement和Statement的区别在于

	      PreparedStatement支持预处理的sql，即支持用?当做占位符的sql

	      Statement对象是使用字符串拼接的方式来构造sql语句



PreparedStatement的setXxx(int parameterIndex, Xxx x)方法

 参数:

  1. parameterIndex 参数索引，表示要设置第几个参数，从1开始计数

  2. x ，Xxx类型，要传入的对象





PreparedStatement接口重载了executeQuery()方法

         PreparedStatement提供了一个无参的executeQuery()方法

         如果传参数，调用的是父接口的那个executeQuery

         sql是在获取PreparedStatement对象时传入的

         因此在调用PreparedStatement对象的executeQuery方法时不需要传参数



ResultSet对象一开始指向的是第0行(其实就是没有数据)







