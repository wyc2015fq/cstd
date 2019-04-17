# jdbc篇第8课：进一步封装jdbc - PeterBishop - CSDN博客





2018年11月27日 19:53:59[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：43
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  这节课我们来进一步封装jdbc



  首先来分析下，jdbc的使用:

jdbc使用:

     1. 建立好表对应的类(我们称为bean)，要求每一列对应一个属性，且数据类型对应上

     2. 建立连接，获取Connection对象

     3. 使用Connection对象获取Statement或PreparedStatement对象

     4. 写好sql

     5. 使用Statement或PreparedStatement执行查询语句，如果是select语句,获取ResultSet对象,并执行6和7

     6. 建一个或多个bean对象

     7. 获取ResultSet里存储的值，并封装到bean对象中

     8. 关闭连接



第1步的封装以后讲，这个可以算是另一个小项目了

第2、3步我们已经封装好了，就是dbc



接下来就是封装4、5、6、7



代码:
package com.tool;



import com.bean.Employee;

import com.util.ClassUtil;

import com.util.ObjectUtil;

import com.util.StringUtil;



import java.io.IOException;

import java.lang.reflect.Field;

import java.lang.reflect.InvocationTargetException;

import java.sql.*;

import java.util.ArrayList;

import java.util.List;

import java.util.Map;



*/** * * **@param ***<*E*> *bean**的类型 * 一个bean对应一个表，且bean的类名用驼峰式命名，表的命名用下划线全小写命名 * 这个类是一个抽象类，必须有子类继承并且给定E的类型 */*public abstract class Mapper<E> {



    private Connection connection;



    public Mapper(Connection connection)

    {

        this.connection = connection;

    }



    public Mapper(String dbcPropertiesPath) throws SQLException, IOException, ClassNotFoundException {

        this.connection = Dbc.*getConnection*(dbcPropertiesPath);

    }



    */**     * **将value转换成合适的类型插入到合适的位置     */    *private void castToSuitType(PreparedStatement preparedStatement,int index, Object value) throws SQLException {

        Class<?> valueClass = value.getClass();



        if (valueClass == String.class)

        {

            preparedStatement.setString(index, String.*valueOf*(value));

        }

        else if(valueClass == Integer.class || valueClass == int.class)

        {

            preparedStatement.setInt(index,(Integer)value);

        }

        else if (valueClass == Float.class || valueClass == float.class)

        {

            preparedStatement.setFloat(index,(Float) value);

        }

        else if (valueClass == Double.class || valueClass == double.class)

        {

            preparedStatement.setDouble(index,(Double) value);

        }

        else if (valueClass == Short.class || valueClass == short.class)

        {

            preparedStatement.setShort(index,(Short)value);

        }

        else

        {

            preparedStatement.setString(index, String.*valueOf*(value));

        }

    }



    private <T> T castToSuitType(ResultSet resultSet,int index, Class<T> fieldClass) throws SQLException

    {

        Object value;

        if (fieldClass == String.class)

        {

            value = resultSet.getString(index);

        }

        else if(fieldClass == Integer.class || fieldClass == int.class)

        {

            value = resultSet.getInt(index);

        }

        else if (fieldClass == Float.class || fieldClass == float.class)

        {

            value = resultSet.getFloat(index);

        }

        else if (fieldClass == Double.class || fieldClass == double.class)

        {

            value = resultSet.getDouble(index);

        }

        else if (fieldClass == Short.class || fieldClass == short.class)

        {

            value = resultSet.getShort(index);

        }

        else

        {

            value = resultSet.getString(index);

        }

        return (T)value;

    }



    */**     * **首先，我们要获取所有的类列     * 每一列对应bean的类的一个属性     * 列名用全小写下划线式     * 属性名用首字母小写的驼峰式     *     * 接下来我们需要几个一个工具类来处理字符串     * 当然，要用的工具类我都提前写好了     */    /**     * 获取泛型E的Class对象，即bean的类型的Class对象     * **@return     ***/    *public Class<E> getBeanClass()

    {

        return ClassUtil.*getSuperClassGenricType*(this.getClass());

    }



    */**     * **获取表名     * **@param     ***/    *public String getTableName()

    {

        Class<E> beanClass = getBeanClass();

        String className = beanClass.getName();

        className = className.substring(className.lastIndexOf(".") +1);

        String tableName = StringUtil.*castCamelCaseToUnderline*(className);



        //保险起见首字母转下小写

        tableName = StringUtil.*firstLetterToLowerCase*(tableName);

        return tableName;

    }



    */**     * **获取列名     * **@return     ***/    *public List<String>  getColumnNames()

    {

        Class<E> beanClass = getBeanClass();

        Field[] fields = beanClass.getDeclaredFields();

        List<String> columnNames = new ArrayList<String>(fields.length);

        for (Field field : fields) {

            String columnName = StringUtil.*castCamelCaseToUnderline*(field.getName());

            //保险起见首字母转下小写

            columnName = StringUtil.*firstLetterToLowerCase*(columnName);

            columnNames.add(columnName);

        }

        return columnNames;

    }





    //接下来封装增删改查操作

    public void insert(E e) throws SQLException, NoSuchMethodException, IllegalAccessException, InvocationTargetException {

        //String sql = "insert into tbl_employee (id,last_name,email,gender,d_id) values (?,?,?,?,?)";

        String sql = "insert into ";

        sql += getTableName() + " (";

        List<String> columnNames = getColumnNames();

        for (String columnName : columnNames) {

            sql += columnName + ",";

        }



        //删除最后一个逗号

        sql = StringUtil.*deleteLastChildString*(sql,1);

        sql += ") values (";



        //说了含金量高的，索性写复杂点

        List<Object> valueList = ObjectUtil.*getAllFieldValue*(e);

        for (Object value : valueList) {

            //用？当占位符

            sql += "?,";

        }

        //删除最后一个逗号

        sql = StringUtil.*deleteLastChildString*(sql,1);

        sql += ");";





        PreparedStatement preparedStatement = connection.prepareStatement(sql);

        for (int i = 0; i < valueList.size(); i++) {

            //这个也是从1开始

            castToSuitType(preparedStatement,i + 1,valueList.get(i));

        }

        preparedStatement.executeUpdate();

        //总算把insert写完了，后面的都差不多，我快点写，就不写注释了

    }



    public void delete(int id) throws SQLException {

        //String sql = "delete from tbl_employee where id = ?";

        String sql = "delete from ";

        sql += getTableName() + " ";

        sql += "where ";

        Class<E> beanClass = getBeanClass();



        //注意，在写bean类的时候必须将主键写在最前面，否则，这个代码要写的更加复杂，因为涉及到注解

        String primaryKeyName = beanClass.getDeclaredFields()[0].getName();

        sql += primaryKeyName + "= ?;";



        PreparedStatement preparedStatement = connection.prepareStatement(sql);

        preparedStatement.setInt(1,id);

        preparedStatement.executeUpdate();

    }



    public void update(E e) throws SQLException, NoSuchMethodException, IllegalAccessException, InvocationTargetException {

        //String sql = "update tbl_employee set last_name = ?,email = ?,gender = ?, d_id = ? where id = ?";

        String sql = "update " + getTableName() + " set ";

        List<String> columnNames = getColumnNames();

        String primaryKeyName = columnNames.remove(0);

        for (String columnName : columnNames) {

            sql += columnName + "=?,";

        }

        sql = StringUtil.*deleteLastChildString*(sql,1);



        sql += " where " + primaryKeyName + "=?";



        PreparedStatement preparedStatement = connection.prepareStatement(sql);

        List<Object> valueList = ObjectUtil.*getAllFieldValue*(e);

        Object primaryKeyValue = valueList.remove(0);

        //主键被挪到了最后，在where那里

        valueList.add(primaryKeyValue);

        for (int i = 0; i < valueList.size(); i++) {

            castToSuitType(preparedStatement,i + 1,valueList.get(i));

        }

        preparedStatement.executeUpdate();

    }



    public E selectByPrimaryKey(Object primaryKey) throws SQLException, IllegalAccessException, InstantiationException, NoSuchMethodException, NoSuchFieldException, InvocationTargetException {

        Class<E> beanClass = getBeanClass();

        //以前说过，java bean必须有空构造函数，其实就是为了方便通过反射构造对象

        E e = null;



        //String sql = "select * from tbl_employee where id = ?";

        String sql = "select ";

        List<String> columnNames = getColumnNames();

        for (String columnName : columnNames) {

            sql += columnName + ",";

        }

        sql = StringUtil.*deleteLastChildString*(sql,1);

        sql += " from " + getTableName() + " where ";

        

        //获取主键名称

        String primaryKeyName = beanClass.getDeclaredFields()[0].getName();

        sql += primaryKeyName + "= ?;";



        PreparedStatement preparedStatement = connection.prepareStatement(sql);

        castToSuitType(preparedStatement,1,primaryKey);

        ResultSet resultSet = preparedStatement.executeQuery();

        if (resultSet.next())

        {

            //查得到数据才new，否则返回null

            e = getBeanFromResultSet(resultSet, beanClass);

        }



        return e;

    }



    */**     *     * **@param ****conditionMap **键是列名，值是要匹配的值     * **@return     ***/    *public List<E> selectByExample(Map<String,Object> conditionMap) throws SQLException, IllegalAccessException, InstantiationException, NoSuchMethodException, NoSuchFieldException, InvocationTargetException {

        List<E> resultList = new ArrayList<E>();

        int size = conditionMap.keySet().size();

        String[] keyArr = new String[size];

        Object[] valueArr = conditionMap.values().toArray();

        conditionMap.keySet().toArray(keyArr);



        //String sql = "select * from tbl_employee where ";

        String sql = "select ";

        List<String> columnNames = getColumnNames();

        for (String columnName : columnNames) {

            sql += columnName + ",";

        }

        sql = StringUtil.*deleteLastChildString*(sql,1);

        sql += " from " + getTableName() + " where ";



        for (int i = 0; i < size; i++) {

            sql += keyArr[i] + "=? and "; //and前后要加空格

        }

        //末尾多了一个and，去掉

        sql = sql.substring(0,sql.lastIndexOf("and") - 1);



        PreparedStatement preparedStatement = connection.prepareStatement(sql);

        for (int i = 0; i < valueArr.length; i++) {

            castToSuitType(preparedStatement,i + 1,valueArr[i]);

        }

        ResultSet resultSet = preparedStatement.executeQuery();



        Class<E> beanClass = getBeanClass();

        while (resultSet.next())

        {

            E e = getBeanFromResultSet(resultSet, beanClass);

            resultList.add(e);

        }

        return resultList;

    }



    //提取点公共代码



    public E getBeanFromResultSet(ResultSet resultSet,Class<E> beanClass) throws IllegalAccessException, InstantiationException, SQLException, NoSuchMethodException, NoSuchFieldException, InvocationTargetException {

        E e = beanClass.newInstance();

        Field[] fields = beanClass.getDeclaredFields();

        for (int i = 0; i < fields.length; i++) {

            //列数从1开始

            Object value = castToSuitType(resultSet, i + 1, fields[i].getType());

            ObjectUtil.*setFieldValue*(e,fields[i],value);

        }

        return e;

    }

}


使用:
public class EmployeeMapper extends Mapper<Employee> {





    public EmployeeMapper(Connection connection) {

        super(connection);

    }



    public EmployeeMapper(String dbcPropertiesPath) throws SQLException, IOException, ClassNotFoundException {

        super(dbcPropertiesPath);

    }

}


测试:
public static void main(String[] args) throws Exception {



    //测一下，我也不确定我能不能一次写对

    EmployeeMapper mapper = new EmployeeMapper("src/com/resource/dbc.properties");



    //通过

    Employee employee = mapper.selectByPrimaryKey(3);

    System.*out*.println(employee);



    //通过

    mapper.delete(100);



    //通过

    employee = new Employee(13,"mahuateng","mahuateng@qq.com","1",1004);

    mapper.update(employee);



    //通过

    employee = new Employee(100,"xiaowang","xiaowang@163.com","0",1002);

    mapper.insert(employee);



    //通过

    Map<String,Object > map = new HashMap<String, Object>();

    map.put("gender","1");

    map.put("d_id",1002);

    List<Employee> employees = mapper.selectByExample(map);



    for (Employee employee1 : employees) {

        System.*out*.println(employee1);

    }

}


工具类:
package com.util;



//字符串工具类

//这个工具类里的所有方法要求看懂并且可以自己写出来

//这个里的所有方法全是我自己手写出来的，要求你们也要可以手写出来

public class StringUtil {



    */**     * **将字符串首字母转换成大写     *     * **@param ****str **要转换的字符串     * **@return **返回str首字母大写后的字符串     */    *public static String firstLetterToUpperCase(String str)

    {

        //这里只替换第一个

        return str.replaceFirst(String.*valueOf*(str.charAt(0)),String.*valueOf*(str.charAt(0)).toUpperCase());

    }



    */**     * **将字符串首字母转换成小写     *     * **@param ****str **要转换的字符串     * **@return **返回str首字母瞎写写后的字符串     */    *public static String firstLetterToLowerCase(String str)

    {

        //这里只替换第一个

        return str.replaceFirst(String.*valueOf*(str.charAt(0)),String.*valueOf*(str.charAt(0)).toLowerCase());

    }



    */**     * **驼峰转下划线     *     解释下什么叫驼峰式，就是每个单词的首字母大写，其余字母都小写     *     java推荐所有变量、方法、类都使用驼峰式命名     *     类名一般第一个单词的首字母也要大写     *     变量、方法第一个单词的首字母小写     *     下划线式是指全字母小写或全字母大写，单词之间用下划线分割，如cast_camel_case_to_underline     *     常量采用全字母大写加下划线式     */    /**     * 将驼峰式字符串转换成下划线式     *     * **@param ****str **驼峰式字符串     * **@return **下划线式字符串     */    *public static String castCamelCaseToUnderline(String str)

    {

        char[] chars = str.toCharArray();

        String result = "";

        for (char i : chars)

        {

            if (i >= 'A' && i <= 'Z')

            {

                result += "_" + String.*valueOf*(i).toLowerCase();

            }

            else

            {

                result += i;

            }

        }

        if (result.startsWith("_"))

        {

            result = result.substring(1);

        }

        return result;

    }



    */**     * **将下划线字符串转换成驼峰式     *     * **@param ****str **下划线式字符串     * **@return **驼峰式字符串     */    *public static String castUnderlineToCamelCase(String str)

    {

        String[] split = str.split("_");

        String result = "";

        for (String s : split)

        {

            result += *firstLetterToUpperCase*(s);

        }

        return *firstLetterToLowerCase*(result);

    }



    */**     * **删除下划线最后length位字符     *     * **@param ****str **要处理的字符串     * **@param ****length **要删除字符串最后多少位字符     * **@return **删除str字符串最后length位字符后的新字符串     */    *public static String deleteLastChildString(String str,int length)

    {

        return str.substring(0,str.length() - length);

    }

}

package com.util;



import java.io.File;

import java.lang.reflect.Field;

import java.lang.reflect.InvocationTargetException;

import java.lang.reflect.Method;

import java.util.ArrayList;

import java.util.List;



public class ObjectUtil {



    */**     * **获取object的属性值     * **@param ****object **要获取属性值的镀锡     * **@param ****fieldName **属性名     * **@return **object的filedName对应的属性值     */    *public static Object getFieldValue(Object object,String fieldName) throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {

        String getterMethodName = "get" + StringUtil.*firstLetterToUpperCase*(fieldName);



        //只要public的getter方法

        Method getterMethod = object.getClass().getMethod(getterMethodName);

        if (getterMethod != null)

        {

            Object value = getterMethod.invoke(object);

            return value;

        }

        return null;

    }



    */**     * **获取object的属性值     * **@param ****object **要获取属性值的镀锡     * **@param ****field **属性     * **@return **object的filed对应的属性值     */    *public static Object getFieldValue(Object object,Field field) throws NoSuchMethodException, IllegalAccessException, InvocationTargetException {

        return *getFieldValue*(object,field.getName());

    }



    */**     * **获取object的所有属性值     * **@param ****object **要获取属性值的镀锡     * **@return **object的所有属性值组成的List集合     */    *public static List<Object> getAllFieldValue(Object object) throws NoSuchMethodException, IllegalAccessException, InvocationTargetException {

        Field[] declaredFields = object.getClass().getDeclaredFields();

        List<Object> valueList = new ArrayList<Object>(declaredFields.length);

        for (Field field : declaredFields) {

            valueList.add(*getFieldValue*(object,field));

        }

        return valueList;

    }



    */**     * **设置object的属性值     * **@param ****object **要获取属性值的镀锡     * **@param ****fieldName **属性名     * **@param ****value **要设置的属性值     * **@return **object的filedName对应的属性值     */    *public static void setFieldValue(Object object,String fieldName,Object value) throws NoSuchMethodException, InvocationTargetException, IllegalAccessException, NoSuchFieldException {

        String setterMethodName = "set" + StringUtil.*firstLetterToUpperCase*(fieldName);

        Class<?> fieldClass = object.getClass().getDeclaredField(fieldName).getType();



        //只要public的getter方法

        Method setterMethod = object.getClass().getMethod(setterMethodName,fieldClass);

        if (setterMethod != null)

        {

            setterMethod.invoke(object,value);

        }

    }



    */**     * **设置object的属性值     * **@param ****object **要获取属性值的镀锡     * **@param ****field **属性     * **@param ****value **要设置的属性值     * **@return **object的filedName对应的属性值     */    *public static void setFieldValue(Object object, Field field, Object value) throws NoSuchMethodException, InvocationTargetException, IllegalAccessException, NoSuchFieldException {

        *setFieldValue*(object,field.getName(),value);

    }

}

package com.util;



import java.lang.reflect.ParameterizedType;

import java.lang.reflect.Type;



//这个工具类的所有方法会用就行，以后需要就直接复制粘贴

//反正这个玩意我也是百度的

public class ClassUtil {



    */**     * **获取父类的泛型的类型     * **@param ****clazz **子类     * **@return **父类的泛型的Class对象     */    *public static Class getSuperClassGenricType(Class clazz) {

        return *getSuperClassGenricType*(clazz, 0);

    }



    */**     * **通过反射,获得定义Class时声明的父类的范型参数的类型. 如public BookManager extends GenricManager**<Book>     ***     * **@param ****clazz **clazz The class to introspect     * **@param ****index **the Index of the generic ddeclaration,start from 0.     */    *public static Class getSuperClassGenricType(Class clazz, int index)

            throws IndexOutOfBoundsException {

        Type genType = clazz.getGenericSuperclass();

        if (!(genType instanceof ParameterizedType)) {

            return Object.class;

        }

        Type[] params = ((ParameterizedType) genType).getActualTypeArguments();

        if (index >= params.length || index < 0) {

            return Object.class;

        }

        if (!(params[index] instanceof Class)) {

            return Object.class;

        }

        return (Class) params[index];

    }





}




