# Java重要知识点（继承、多态、接口，异常，工具，测试） - Soul Joy Hub - CSDN博客

2016年11月13日 17:27:39[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：753


# 重载、多态、接口

## [多态调用的顺序](http://blog.csdn.net/zhanghaotian2011/article/details/9040935)

当父类，和子类有Static时，先初始化Static，再初始化子类的Static，再初始化父类的其他成员变量－>父类构造方法－>子类其他成员变量－>子类的构造方法。

## 重写 hashcode equals compareto

### Employee.java

```java
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.Objects;

public class Employee implements Comparable<Employee> {
    private String name;
    private double salary;
    private Date hireDay;

    public Employee(String n, double s, int year, int month, int day) {
        name = n;
        salary = s;
        GregorianCalendar calendar = new GregorianCalendar(year, month - 1, day);
        hireDay = calendar.getTime();
    }

    public String getName() {
        return name;
    }

    public double getSalary() {
        return salary;
    }

    public Date getHireDate() {
        return hireDay;
    }

    public boolean equals(Object Otherobject) {
        if (this == Otherobject)
            return true;
        if (Otherobject == null)
            return false;
        // if(! Otherobject instanceof Employee) return false;
        if (getClass() != Otherobject.getClass())
            return false;
        Employee other = (Employee) Otherobject;
        return Objects.equals(name, other.name) && salary == other.salary && Objects.equals(hireDay, other.hireDay);
    }

    public int hashCode() {
        return Objects.hash(name, salary);
    }

    public String toString() {
        return getClass().getName() + "name=" + name + ",salary=" + salary + ",hireDay=" + hireDay;
    }

    @Override
    public int compareTo(Employee other) {
        // if(getClass() != other.getClass()) throw new ClassCastException();
        return Double.compare(salary, other.salary);
    }
}
```

### Manager.java

```java
public class Manager extends Employee {

    private double bonus;

    public Manager(String n, double s, int year, int month, int day) {
        super(n, s, year, month, day);
        bonus = 0;
    }

    public void setBonus(double b) {
        bonus = b;
    }

    @Override
    public double getSalary() {
        double baseSalary = super.getSalary();
        return bonus + baseSalary;
    }

    @Override
    public boolean equals(Object Otherobject) {
        if (!super.equals(Otherobject))
            return false;
        Manager other = (Manager) Otherobject;
        return bonus == other.bonus;
    }

    @Override
    public int hashCode() {
        return super.hashCode() + 17 * new Double(bonus).hashCode();
    }

    @Override
    public String toString() {
        return super.toString() + ",bonus=" + bonus;
    }
}
```

# 异常

## 涉及return finally 以及异常抛出的问题

![这里写图片描述](https://img-blog.csdn.net/20161113172343059)
![这里写图片描述](https://img-blog.csdn.net/20161113171756147)

## 各个异常出现的情况

```java
package exp;

import java.awt.Font;
import java.awt.FontFormatException;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;
import java.beans.Statement;
import java.io.EOFException;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.sql.Connection;
import java.sql.DriverManager;
import java.util.Stack;

public class AllExcept {

    public static void main(String[] args) throws Exception {
        // 01 空指针异常 java.lang.NullPointerException
        try {
            int[] s = null;
            int i = s.length;
        } catch (Exception e) {
            System.out.println("1、" + e.getClass().getName() + "异常");
        }
        // 02数组越界 java.lang.ArrayIndexOutOfBoundsException异常
        try {
            int[] s1 = new int[2];
            s1[2] = 5;
        } catch (Exception e) {
            System.out.println("2、" + e.getClass().getName() + "异常");
        }
        // 03 错误类型的对象存储到一个对象数组时抛出的异常 java.lang.ArrayStoreException异常
        try {
            Object x[] = new String[3];
            x[0] = new Integer(0);

        } catch (Exception e) {
            System.out.println("3、" + e.getClass().getName() + "异常");
        }
        // 04 除数为0异常 java.lang.ArithmeticException异常
        try {
            int a = 0;
            int b = 10;
            b /= a;
        } catch (Exception e) {
            System.out.println("4、" + e.getClass().getName() + "异常");
        }
        // 05 数据类型转换异常 java.lang.NumberFormatException异常
        try {
            String s2 = "19k";
            int n = Integer.parseInt(s2);
        } catch (Exception e) {
            System.out.println("5、" + e.getClass().getName() + "异常");
        }
        // 06 超过字符串添加长度异常 java.lang.StringIndexOutOfBoundsException异常
        try {
            StringBuffer buffer = new StringBuffer();
            buffer.charAt(5);
        } catch (Exception e) {
            System.out.println("6、" + e.getClass().getName() + "异常");
        }
        // 07 数组大小为负值异常。 java.lang.NegativeArraySizeException异常
        try {
            int i = -1;
            int[] s = new int[i];
        } catch (Exception e) {
            System.out.println("7、" + e.getClass().getName() + "异常");
        }
        // 08 正则表达式异常 java.util.regex.PatternSyntaxException异常
        try {
            String a = "\\";// 错误的正则表达式
            String a1 = "\\.";// 正确的正则表达式
            String[] x5 = "abc.cde".split(a1);
            x5 = "abc.cde".split(a);

        } catch (Exception e) {
            System.out.println("8、" + e.getClass().getName() + "异常");
        }
        // 09 空栈异常 java.util.EmptyStackException异常
        try {
            Stack s = new Stack();
            s.pop();
        } catch (Exception e) {
            System.out.println("9、" + e.getClass().getName() + "异常");
        }
        // 10 找不到类异常 java.lang.ClassNotFoundException异常
        try {
            Object s = Class.forName("ClassName");
        } catch (Exception e) {
            System.out.println("10、" + e.getClass().getName() + "异常");
        }
        // 11非法参数异常
        // 12 字体格式错误异常
        try {
            Font f = new Font("楷体", Font.BOLD, 7);
            f.createFont(2, new File("1.txt"));
        } catch (FontFormatException e) {
            System.out.println("11、" + e.getClass().getName());
        } catch (IllegalArgumentException e) {
            System.out.println("12、" + e.getClass().getName() + "异常");
        }
        // 13不支持画图异常
        try {
            Image img = Toolkit.getDefaultToolkit().getImage("Images\1.jpg");
            Graphics g = img.getGraphics();
            g.drawRect(-1, -1, -1, -1);
        } catch (Exception e) {
            System.out.println("13、" + e.getClass().getName() + "异常");
        }

        // 14 BufferOverflowException
        try {
            int cap = 2;
            ByteBuffer bf = ByteBuffer.allocate(cap);
            for (int i = 0; i < cap; i++) {
                bf.put((byte) i);
            }
            bf.put((byte) 10);
            bf.getInt(9);
        } catch (Exception e) {
            System.out.println("14、" + e.getClass().getName() + "异常");
        }
        // 15文件找不到异常
        // 16.当发生某种 I/O 异常时，抛出IOException
        // 17.文件已结束异常EOFException
        try {
            File f = new File("E:\\1.txt");
            byte[] buff = new byte[2];
            FileInputStream file = new FileInputStream(f);
            int bytes = file.read(buff, 0, 2);
            System.out.println(new String(buff, 0, 0, bytes));
            file.close();

        } catch (FileNotFoundException e) {
            System.out.println("15、" + e.getClass().getName() + "异常");
        } catch (EOFException e) {
            System.out.println("16、" + e.getClass().getName() + "异常");
        } catch (IOException e) {
            System.out.println("17、" + e.getClass().getName() + "异常");
        }
        // 18 ClassCastException异常
        try {
            Object s = new Integer(0);
            System.out.println((String) s);
        } catch (Exception e) {
            System.out.println("18、" + e.getClass().getName() + "异常");
        }
        // 19 BufferUnderflowException异常
        try {
            ByteBuffer buf = ByteBuffer.allocate(50);
            buf.flip();
            byte[] byt = new byte[100];
            buf.get(byt);

            new RuntimeException();
        } catch (Exception e) {
            System.out.println("19、" + e.getClass().getName() + "异常");
        }
        // 20 java.sql.SQLException异常
        try {

            String sql = "select * from Table";// sql语句错误
            Connection conn = DriverManager.getConnection("com.mysql.jdbc.Driver");

            Statement smt = (Statement) conn.createStatement();
            smt.execute();

        } catch (Exception e) {
            System.out.println("20、" + e.getClass().getName() + "异常");
        }
    }
}
```

# 工具

## [git svn 的 区别 和 常用命令](http://blog.csdn.net/u011239443/article/details/53106445)

## [Maven依赖中的scope](http://blog.csdn.net/u011239443/article/details/53005079)

# 测试

## JUnit

### Die.java

```java
public class Die
{
    private int faceValue;
    private int numofsides;
    public Die(){
        faceValue = 0;
        numofsides = 6;
    }

    public Die(int v){
        numofsides = v;
    }

    public int numOfSides(){
        return numofsides;
    }

    public int  topFace(){
        return faceValue;
    }

    public void roll() {
        faceValue = (int) (1 + Math.random() * numofsides );
    }
}
```

### TestDie.java

```java
import org.junit.After;
import org.junit.Assert;
import org.junit.Test;
import org.junit.Before;

/**
 * Unit test for simple App.
 */

public class TestDie {

    private Die die1,die2;

    @Before
    public void setUp() {

        die1 = new Die();
        die2 = new Die(10);
        die1.roll();
        die2.roll();
    }

    @Test
    public void testdie1numofsides() {
        Assert.assertEquals(6, die1.numOfSides());
    }

    @Test
    public void testdie1Face() {
        Assert.assertTrue(die1.topFace() >= 1 && die1.topFace() <= 6);
    }

    @Test
    public void testdie2numofsides() {
        Assert.assertEquals(10, die2.numOfSides());
    }

    @Test
    public void testdie2Face() {
        Assert.assertTrue(die2.topFace() >= 1 && die2.topFace() <= 10);
    }

    @After
    public void tearDown() {
    }

}
```

## [实现 assertEquals aseertSame](http://blog.csdn.net/u011239443/article/details/52999946)

### assertEquals.java

当expected和actual都是String类型时，ComparisonFailure还会找出是前后相同的串，并用[Different String]标明那些不相同的字符串，也就是expectedToString和actualToString的格式将会变成：…${sameString}[${differentString}]${sameString}…。其中“…”只会在相同的字符串太长的情况下才会出现，这个长度标准目前（JUnit4.10）是20个字符。具体实现参考ComparisonFailure类，它继承自AssertionError，这里不再展开。

```java
/**
     * Asserts that two objects are equal. If they are not, an
     * {@link AssertionError} is thrown with the given message. If
     * <code>expected</code> and <code>actual</code> are <code>null</code>,
     * they are considered equal.
     * 
     * @param message
     *            the identifying message for the {@link AssertionError} (<code>null</code>
     *            okay)
     * @param expected
     *            expected value
     * @param actual
     *            actual value
     */
        static public void assertEquals(String message, Object expected,
            Object actual) {
        if (expected == null && actual == null)
            return;
        if (expected != null && isEquals(expected, actual))
            return;
        else if (expected instanceof String && actual instanceof String) {
            String cleanMessage= message == null ? "" : message;
            throw new ComparisonFailure(cleanMessage, (String) expected,
                    (String) actual);
        } else
            failNotEquals(message, expected, actual);
    }

    private static boolean isEquals(Object expected, Object actual) {
        return expected.equals(actual);
    }

    /**
     * Asserts that two objects are equal. If they are not, an
     * {@link AssertionError} without a message is thrown. If
     * <code>expected</code> and <code>actual</code> are <code>null</code>,
     * they are considered equal.
     * 
     * @param expected
     *            expected value
     * @param actual
     *            the value to check against <code>expected</code>
     */
    static public void assertEquals(Object expected, Object actual) {
        assertEquals(null, expected, actual);
    }
```

### aseertSame.java

```java
/**
     * Asserts that two objects refer to the same object. If they are not, an
     * {@link AssertionError} is thrown with the given message.
     * 
     * @param message
     *            the identifying message for the {@link AssertionError} (<code>null</code>
     *            okay)
     * @param expected
     *            the expected object
     * @param actual
     *            the object to compare to <code>expected</code>
     */
    static public void assertSame(String message, Object expected, Object actual) {
        if (expected == actual)
            return;
        failNotSame(message, expected, actual);
    }

    /**
     * Asserts that two objects refer to the same object. If they are not the
     * same, an {@link AssertionError} without a message is thrown.
     * 
     * @param expected
     *            the expected object
     * @param actual
     *            the object to compare to <code>expected</code>
     */
    static public void assertSame(Object expected, Object actual) {
        assertSame(null, expected, actual);
    }
```

## 其他
- hamcrest 可以用来增强 JUnit 中的 assert 功能
- Cobertura is a free Java tool that calculates the percentage of code accessed by tests.
- stub & mock. EasyMock has been the first dynamic Mock Object generator, relieving users of hand-writing Mock Objects, or generating code for them
- DbUnit is a JUnit extension (also usable with Ant) targeted at database-driven projects that,among other things, puts your database into a known state between test runs

