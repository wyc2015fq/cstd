# jdbc篇第7课：封装增删改查 - PeterBishop - CSDN博客





2018年11月27日 19:28:50[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：56
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  这节课我们来封装增删改查



实现:
package com.tool;



import com.bean.Employee;



import java.sql.*;

import java.util.*;



public class EmployeeMapper {



    Connection connection;



    public EmployeeMapper(Connection connection)

    {

        this.connection = connection;

    }



    public void insert(Employee employee) throws SQLException {

        String sql = "insert into tbl_employee (id,last_name,email,gender,d_id) values (?,?,?,?,?)";

        PreparedStatement preparedStatement = connection.prepareStatement(sql);

        preparedStatement.setInt(1,employee.getId());

        preparedStatement.setString(2,employee.getLastName());

        preparedStatement.setString(3,employee.getEmail());

        preparedStatement.setString(4,employee.getGender());

        preparedStatement.setInt(5,employee.getdId());



        preparedStatement.executeUpdate();

    }



    public void delete(int id) throws SQLException {

        String sql = "delete from tbl_employee where id = ?";

        PreparedStatement preparedStatement = connection.prepareStatement(sql);

        preparedStatement.setInt(1,id);

        preparedStatement.executeUpdate();

    }



    public void update(Employee employee) throws SQLException {

        String sql = "update tbl_employee set last_name = ?,email = ?,gender = ?, d_id = ? where id = ?";

        PreparedStatement preparedStatement = connection.prepareStatement(sql);

        preparedStatement.setString(1,employee.getLastName());

        preparedStatement.setString(2,employee.getEmail());

        preparedStatement.setString(3,employee.getGender());

        preparedStatement.setInt(4,employee.getdId());

        preparedStatement.setInt(5,employee.getId());



        preparedStatement.executeUpdate();

    }



    public Employee selectById(int id) throws SQLException {

        Employee employee = null;



        String sql = "select * from tbl_employee where id = ?";

        PreparedStatement preparedStatement = connection.prepareStatement(sql);

        preparedStatement.setInt(1,id);

        ResultSet resultSet = preparedStatement.executeQuery();

        if (resultSet.next())

        {

            employee = new Employee();

            employee.setId(resultSet.getInt(1));

            employee.setLastName(resultSet.getString(2));

            employee.setEmail(resultSet.getString(3));

            employee.setGender(resultSet.getString(4));

            employee.setdId(resultSet.getInt(5));

        }

        return employee;

    }



    */**     *     * **@param ****conditionMap **键是列名，值是要匹配的值     * **@return     ***/    *public List<Employee> selectByExample(Map<String,String> conditionMap) throws SQLException {

        List<Employee> employeeList = new ArrayList<Employee>();

        int size = conditionMap.keySet().size();

        String[] keyArr = new String[size];

        String[] valueArr = new String[size];

        conditionMap.keySet().toArray(keyArr);

        conditionMap.values().toArray(valueArr);



        String sql = "select * from tbl_employee where ";

        for (int i = 0; i < size; i++) {

            sql += keyArr[i] + "=" + valueArr[i] + " and ";

        }

        //末尾多了一个and，去掉

        sql = sql.substring(0,sql.lastIndexOf("and") - 1);



        Statement statement = connection.createStatement();

        ResultSet resultSet = statement.executeQuery(sql);

        while (resultSet.next())

        {

            Employee employee = new Employee();

            employee = new Employee();

            employee.setId(resultSet.getInt(1));

            employee.setLastName(resultSet.getString(2));

            employee.setEmail(resultSet.getString(3));

            employee.setGender(resultSet.getString(4));

            employee.setdId(resultSet.getInt(5));



            employeeList.add(employee);

        }

        return employeeList;

    }

}




测试代码:
public static void main(String[] args) throws SQLException, ClassNotFoundException, IOException {

    Connection conn = Dbc.*getConnection*("src/com/resource/dbc.properties");



    EmployeeMapper employeeMapper = new EmployeeMapper(conn);



    //测试selectById()

    Employee employee = employeeMapper.selectById(2);

    System.*out*.println(employee.toString());



    //测试insert

    employee = new Employee(100,"wangdachui","dachui@qq.com","1",1003);

    employeeMapper.insert(employee);



    //测试delete

    employeeMapper.delete(12);



    //测试update

    employee = new Employee(13,"mayun","mayun@alibaba.com","1",1004);

    employeeMapper.update(employee);//测试selectByExample

    Map<String,String> map = new HashMap<String, String>();

    map.put("gender","1");  

    map.put("d_id","1001");

    List<Employee> employees = employeeMapper.selectByExample(map);

    for (Employee employee1 : employees) {

        System.*out*.println(employee1.toString());

    }



}


原来的数据:



![](https://img-blog.csdnimg.cn/20181127192812789.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

执行后:



![](https://img-blog.csdnimg.cn/20181127192819505.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

可以看到id为12的行已经被删除了

Id为100的被插入了

第13行数据也被更改了



结果:

Employee{id=2, lastName='bb', email='bb@sina.com', gender='0', dId=1003}

Employee{id=1, lastName='xiaoye', email='xiaoye@qq.com', gender='1', dId=1001}

Employee{id=4, lastName='dd', email='dd@sina.com', gender='1', dId=1001}



第一条是selectById的结果

后两行是selectByExample的结果









