# 编译出错：对于常量池来说，字符串表示的UTF过长，那我想知道，JVM的常量池到底有多大？ - z69183787的专栏 - CSDN博客
2017年01月10日 12:16:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4758
输入缓冲说是8000个字符，和这有关吗？
 String内部是以char数组的形式存储，数组的长度是int类型，那么String允许的最大长度就是Integer.MAX_VALUE了,2147483647;
又由于java中的字符是以16位存储的，因此大概需要4GB的内存才能存储最大长度的字符串。
所以，发送xml批量的需要在oracle数据库中用clob类型，而在java 端可以用String；
 ResultSet rs = st.executeQuery("select CLOBATTR from TESTCLOB where ID=1");
if (rs.next())
  {
  java.sql.Clob clob = rs.getClob("CLOBATTR");
  inStream = clob.getCharacterStream();
  char[] c = new char[(int) clob.length()];
  inStream.read(c);
  //data是读出并需要返回的数据，类型是String
  data = new String(c);
  inStream.close();
  }
  inStream.close();
  con.commit();
#### 解决方案 »
- 
虚拟机必须为每个被装载的类型维护一个常量池。常量池就是该类型所用到常量的一个有序集和，包括直接常量（string,integer和floating point常量）和对其他类型，字段和方法的符号引用。对于String常量，它的值是在常量池中的。而JVM中的常量池在内存当中是以表的形式存在的，对于String类型，有一张固定长度的CONSTANT_String_info表用来存储文字字符串值，注意：该表只存储文字字符串值，不存储符号引用。
- 
- 
答：JVM的常量池最多可放65535个项。第0项不用。最后一项最多只能是65534(下标值)。而每一项中，若是放一个UTF-8的常量串，其长度最长是：65535个字节(不是字符)。
