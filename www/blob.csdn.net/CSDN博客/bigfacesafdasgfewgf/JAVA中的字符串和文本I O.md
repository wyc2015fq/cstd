# JAVA中的字符串和文本I/O - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 15:48:10[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：589标签：[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java](https://blog.csdn.net/puqutogether/article/category/2595127)









- 定义一个字符串，Java可以有如下几种方式：1) String newstring = new String("Welcome to Java");   2) String message = "Welcome to Java";   3) char [] charArray = {‘H’, 'e', 'l', 'l', 'o'}; String message =new String(charArray);
- 判断两个字符串是否相等，使用equals()方法。例如：string1.equals(string2)； 使用==只能检测两个字符串是否指向同一个对象，不会告诉你它们的内容是否相同；如果相等，返回true；如果不等，返回false；
- 比较两个字符串大小，可以使用compareTo方法。如果s1等于s2，那么返回0；如果s1小于s2，返回值小于0，且整数值为从左到右第一个不同字符之间的字符差值；如果s1大于s2，返回值为正数，值的大小同上。使用形式为：s1.compareTo(s2);
- String类中其余常用到的方法包括：equalsIgnoreCase, compareToIgnoreCase, regionMatches, str.startsWith(prefix), str.endsWith(suffix);
- message.length()返回字符串的长度；s.charAt(index)可以提取字符串在index处的字符，其中index的取值范围从0~s.length()-1;
- 连接两个字符串：1）String s3 = s1.concat(s2);   2) String s3 = s1 + s2; 
- 提取一个字符串的子串：message.substring(beginIndex, endIndex);  
- String类中常用的方法还包括：toLowerCase, toUpperCase, trim, replace, replaceFirst; 
- char [] chars = "Java".toCharArrays();  可以把字符串转换为字符数组；String.valueOf(5.44)可以将数值转换为字符，返回值是'5', '.', '4', '4' 这4个字符； 
-  StringBuilder和StringBuffer类是可以替代String类的另一种处理字符串的解决方案。一般来说，只要使用字符串的地方，都可以使用StringBuilder/StringBuffer类，而且它们比String类更加灵活，因为它们可以添加、插入或者追加新的内容，但是String类的对象一旦创建，它的值也就确定了，不可以修改了；
- 如果是多任务并发访问，最好使用StringBuffer；而如果是单任务访问，使用BuilderBuilder会更加有效；
- Java中的I/O文本输出。首先需要用java.io.File file = new java.io.File("scores.txt"); 创建一个文件。再用java.io.PrintWriter output = new java.io.PrintWriter(file);可以创建一个文件并向文本文件写入数据，对于file文件创建了一个PrintWriter类的对象；output.print()和output.println可以往文件里面写内容；最后还需要使用output.close();
  关闭文件，否则数据不能正确地保存在文件中去；对于输入，我们首先需要java.io.File file = new java.io.File("scores.txt");，然后Scanner input = new Scanner(file);  input.next()来读取，默认分隔符是空格；外围可以使用while(input.hasNext())来判断文件中是否还有可读内容；
- 调用PrintWriter的构造方法可能会抛出某种I/O异常。Java强制要求编写代码来处理这些异常，我们需要在方法的头声明中throws Exception.
- Scanner中的input.next()只能用在string类型的读，其余类型的需要在next中指定类型。例如：nextInt(),nextyDouble()等等；而读取一行则可以使用String line=input.nextLine();





