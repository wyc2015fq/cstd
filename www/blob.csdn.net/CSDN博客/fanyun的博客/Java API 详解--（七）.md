# Java  API 详解--（七） - fanyun的博客 - CSDN博客
2016年12月25日 21:56:51[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1398
所属专栏：[Java框架精要](https://blog.csdn.net/column/details/java-01.html)
**Java API概念：**
API (Application Program Interface)：[应用程序接口](http://www.so.com/s?q=%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F%E6%8E%A5%E5%8F%A3&ie=utf-8&src=wenda_link)；Java API是[Java](http://www.so.com/s?q=Java&ie=utf-8&src=wenda_link)提供很多类和应用程序接口，主要用来帮助我们编程；
**java API的使用：**
比如想查找java的随机函数的用法
1、  单击类目录，找到 math类，因为随机函数就在这个类中。
2、  右侧解释说明部分会随之转到math类的解释内容。
3、  单击右侧滚动条，找到random方法，如下图说所示。
![](https://img-blog.csdn.net/20161225215550145?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4、  单击表格中蓝色的“random()”字符部分，可以转到详细的解释说明部分，如下图所示：
**常用Java API总结：**
一、 java.io.BufferedReader类（用于从文件中读入一段字符；所属套件：java.io）
1、 构造函数BufferedReader(java.io.FileReaderFileReader变量) 说明：新建一个BufferReader对象。
2、 close方法 voidclose() 说明：关闭BufferReader对象。
3、 readLine方法java.lang.string readLine() 说明：从文件中读取一行字符。若为NULL，代表读取至文件结尾。
二、 java.io.BufferedWriter(向文件写入一段字符。所属套件：java.io)
1、 构造函数：BufferedWriter(java.io.FileWriteFileWrite变量) 说明：新建一个BufferWriter对象。
2、 close方法：voidclose() 说明：关闭BufferedWriter对象。
3、 newLine方法：java.lang.stringnewLine() 说明：向文件写入分行符。
4、 write方法：voidwrite(char字符)；void write(char[]字符数组)；void write(string 字符串)；void write(char[]字符数组，int数组中起始位置，int输出长度) 说明：将数据写入文件。
三、 Java.io.File（File用于管理文件或目录；所属套件：java.io）
1、 构造函数：File(java.lang.string路径，java.lang.string文件名) 说明：建立代表文件或目录的File对象。
2、 canRead方法：（booleancanRead()）判断是否可读取
3、 canWrite方法：（booleancanWrite）判断是否可以写入
4、 createNewFile方法：（booleancreateNewFile()）说明：创建文件，成功返回TRUE，否则返回false
5、 delete方法：（booleandelete()）说明：删除文件或目录，成功返回TRUE，否则返回FALSE。
6、 exist方法：（booleanexist()）说明：检查文件或目录是否存在。
7、 getName方法：（java.lang.stringgetName()）说明：取得文件或目录的名称。
8、 lastModified方法：（longlastModified()）取出最后一次修改的时间。
9、 length方法：（intlength()）说明：取得文件的长度
10、 listfiles方法：（File[]listFiles()）说明：取出该目录下的所有文件。
11、 isDirectory方法：（booleanisDirectory()）说明：判断是否为文件。
12、 isFile方法：（booleanisFile）说明：判断是否为文件。
13、 isHidden 方法：（ BooleanisHidden()）说明：判断是否为隐藏文件。
14、 mk dir方法：（booleanmkdir）说明：创建目录，成功返回TRUE，否则返回FALSE
四、 java.io.FileReader（FileReader 用于从文件中读取字符；所属套件：java.io）
1、 构造函数（FileReader(Java.lang.strig文件路径和名称) 说明：为要读取的文件建立一个FileReader对象）
2、 read方法：(1)（int read()）说明：从文件读取一个字符，返回值为一整数，为读取得字符串的字符码，若返回值为-1，则代表已经读到文件的结尾。(2) int read（char[] chararray,int startpos,int length）说明：将从startpos开始，读取length字符，保存到chararray数组中，返回值是读取到的字数，如果返回-1，则代表已经读到文件的结尾。
3、 skip方法：（longskip(int 要略过的字符数) 说明：直接将文件读取指针向后移动数个字符，从而略过字符不读取）；
五、 java.io.FileWrite类（用于向文件中写入读取字符；所属套件java.io）
1、 FileWrite（java.lang.string文件路径与名称）Filwrite（java.lang.string文件路径与名称,Boolean append）  说明：为要写入的文件建立一个FileReader对象，参数append表示是否在原文件后添加。
2、 Write方法：（voidwrite(char字符)，char[]字符数组，string字符串，char[]字符数组、int数组中起始位置，int输出长度）说明：将数据写入文件。
六、 java.lang.string类（string类用于处理字符串这种数据类型；所属套件：java.lang）
1、 charAt方法。（语法：charcharAt(int index) 说明：返回指定位置的字符
2、 equals方法：（语法：booleanequals(object anobject) 说明：判断字符串是否与指定的对象相等）事例：if (str.equals(“abc”)){out.print(“str的值为abc”);}
3、 index of方法：（语法：intindexOf(char ch);int indexOf(string str) 说明：返回要查找的字符或子串第一次出现在字符串中的位置，如果找不到该字符／子字符串，将返回－１）。
4、 lastindexOf方法（语法：intlastindexOf(char ch);int lastindexOf(string ch) 说明：返回要查找的字符或子串最后一次出现在字符中的位置。如果找不到该字符/子字符串，将返回-1）
5、 replace方法：（语法：stringreplace(char oldChar,char newChar) 说明：替换字符串中的某个字符，返回值为替换后的结果。）
6、 substring方法：（方法：stringsubstring(int beginIndex);string substring(int beginIndex,int endIndex) 说明：取得字符串中的子串。示例： string a=”AABB”；b=a.substring(1) 最后结果是“ABB”c=a,subsirng(1,2);最后结果是“AB”
7、 大小写转换的方法（语法：stringtoLowerCase();string toUpperCase() 说明：对字符串进行大小写转换，返回值为转换后的结果）
8、 valueOf方法（语法：stringvalueOf(Object obj) 说明：将对象转换为字符串，返回值为转换后的结果）
七、 java.net.URL Encoder（说明：URLEncoder类用于URL的编码。所属套件：JAVA .net）
1、 encode方法（语法：java.lang.stringencode(java.lang.sring要编码的字符串) 说明：返回编码后的字符串。）
八、 java.text.simpleDateFormat类（说明：simpleDateFormat类用于处理Date数据的格式化输出和输入。所属套件：java.text）
1、 构造函数：（语法：simpleDateFormat(java.lang.strng格式化字符串) 说明：根据指定格式化字符串，产生simpleDateFormat对象）
2、 format方法：（语法：java.lang.stringformat(java.util.Date日期数据) 说明：将Date数据转化为指定格式的字符串。）
3、 parse方法：（语法：java.util.Dateparse(java.lang.sring 字符串) 说明：将字符串转化为Date 数据。如果格式错误，将会产生异常
九、 java.spl.DriverManager类（说明：当数据库驱动程序加载到内存中后，系统将会自动利用DriverManager类注册加载的驱动程序，使其状态为可使用（available）所属套件：java.spl
1、 getConnection方法：（语法：getConnection(string数据源名称，string用户名称，string用户密码) 说明：打开数据库链接。）
十、 java.sql.connection类（说明：Connection类用于处理一条与数据库的链接。所属套件：Java.sql）
1、 createStatement方法：（语法：java.spl.statementcreateStatement(int 指针类型参,int数据一致性参数) 说明：建立Statement对象。
十一、 java.sql.ResultSetMetaData类（说明：ResultSetMetaData类用于获取记录集字段的信息。所属套件：java.sql.
1、 getColumnCount方法：（语法：intgetColumnCount() 说明：取得记录集中的字段数目）
2、 getColumnName方法：（语法：java.lang.StringgetColumnName(int column) 说明：取得字段的字称。其中字段索引值是记录集中字段的索引值，第一字段的索引值1，最后一个字段的索引值即为总字段数。
十二、java.sql.Statement类（说明：Statement 类可以执行ＳＱＬ动作和查询语句，实现对数据库的操作。所属套件：java.sql）。
1、 execute方法：（语法：booleanexecute(string sql) 说明：执行ＳＱＬ动作语句，如插入、删除、修改等。）示例：st.execute(“delect from table”)  //从表table中删除数据。
2、 executeQuery方法：（语法：java.sql.ResultSetexecuteQuery(string sql) 说明：执行ＳＱＬ查询语句，得到相应的记录集。）示例：rs=st.execute(“select * from table”)  //从表中取出数据。
十三、java.sql.ResultSet类（说明：ResultSet类用于处理一个记录集。所属套件：java.sql）
1、 absolute方法（语法：booleanabsolute(int row) 说明：设置记录指针指向记录的位置。）
2、 afterLast 方法：（语法：voidafterLast() 说明：将指针移动到最后一条记录之后。）
3、 beforeFirst()方法：（语法：voidbeforeFirst() 说明：将指针移动到第一条记录之前。）
4、 first方法：（语法：booleanfirst() 说明：将记录指针移至第一条记录。）
5、 isAfterlast 方法：（语法：BooleanisAfterLast() 说明：判断记录指针是否指向最后一条记录之后）
6、 isBeforeFirst方法：（语法：BooleanisBeforeFirst() 说明：判断记录指针是否指向第一条记录之前。
7、 isFirst方法：（语法：BooleanisFirst() 说明：判断记指针是否指向第一条记录）
8、 isLast 方法：（语法：BooleanisLast() 说明：判断记录指针是否指向最后一条记录。）
9、 字段值获取方法（语法：object get…(stringcolumnName);object get…(int columnIndex) 说明：从ResultSet对象中取出字段的值）示例：string name=rs.getstring(“use”);
10、 getMetaData 方法：（语法：java.sql.ResultSetMetaDataResultSetMetaData getMetaData() 说明：取得该ResultSet对象对应的ResultSetMetaData对象。）
11、 getＲow方法：（语法：intgetRow()  说明：取得目前记录指针指向记录的位置）
12、 next方法：（语法：booleannext()  说明：将指针移动到下一条）
13、 previous方法：（语法：booleanprevious() 说明：将指针移到上一条）
十四、java.util.Date类（说明：Date类用于处理日期这种数据类型。所属套件：java.util）
1、 构造函数：（语法：Date();Date(long毫秒数)；Date(int年，int月，int日，int时，int分，int秒) 说明：产生一个Date对象。此对象为给定的时间。如果没有输入参数的话，则此Date对象的值为目前的系统时间。）
2、 getTime方法（语法：longgetTime() 说明：该对象代表的时间，距离1970年1月1日午夜的毫秒数。）
3、 get Year方法：（语法：intgetYear() 说明：获得Date对象的年值，输出的数值以1900为基础。）
4、 getMonth方法：（语法：获得Date对象的月值，输出的数值以0代表1，1代表2，其余类推）
5、 getMonth方法：（语法：intgetDate() 说明：获得Date对象的日值）
6、 getDay方法（语法：intDay()说明：获得Date对象的星期值。）
7、 getHours方法：（语法：intgetHours() 说明：获得Date对象的小时值）
8、 getMinutes方法：（语法：intgetMinutes() 说明：获得Date对象的分值。）
9、 getSeconds方法：（语法：intgetSeconds() 说明：获得Date对象的秒值。）
10、 setYear方法：（语法：voidsetYear(int year) 说明：设置Date 对象年值，输入的数值以1900为基准。）设置：Month\day\hours\seconds都是用这种方法。
十五、javax.servlet.http.cookie（说明：Cookie类主要用于处理对cookie的使用。所属套件：javax.serlet.http）
1、 构造函数（语法：Cookie(“数据名称”，值) 说明：新建一个Cookie象。需要注意的是，保存到cookie的仅能为字符串，因此，对其他类型的数据需要进行转换。）
2、 getDomain 方法：（java.lang.stringgetDomain() 说明：取得可存取该cookie变量的网站地址。）
3、 getName方法：（语法：java.lang.stringgetName() 说明：取得Cookie变量中数据的名称。）
4、 getMaxAge方法：（语法：longsetMaxAge() 说明：取得Cookie的有效时间。输入的有效时间单位为秒）
5、 getPath方法：（语法：java.lang.stringgetPath() 说明：取得COOKIE变量的存取范围）
6、 setDomain方法：（语法voidsetDomain(java.lang.string pattern)　说明：设置可存取Ｃookie变量的网站。）
十六、java.servlet.http.httpservletRequest类（说明：ＪＳＰ中隐含对象Ｒequest即为该类的对象。用于处理与浏览器请求有关的信息。所属的套件：javax.servlet.http）
1、 getattribute方法：（语法：ＯbjectgetAttribute(java.lang.string变量名称)　说明：取出保存request对象内的数据。该数据是使用setＡttribute方法存放在request对象中的。
2、 getBufferSize方法：（语法：intgetBufferSize() 说明：取得目前缓冲区的大小。）
3、 getCookies 方法：（语法：javax.servlet.http.Cookie[]getCookies() 说明：取得与请求有关的Cookies.）
4、 getDateHeader方法：（语法：getDateHeader(java.lang.stringname) 说明：取得指定名称的、日期类型的标头。）
5、 getIntHeader方法：（语法：取得指定名称的、整数类型的标头）
6、 getHeader方法：（语法：java.lang.stringgetHeader(java.lang.string name) 说明：取得指定名称的标头。）
7、 getMethod方法：（语法：java.lang.string.getMethod()说明：取得数据发送的方法(GET或POST)）
8、 getParameter方法：（语法：java.lang.stringgetParameter(java.lang.string name) 说明：取得参数名称为name的值。返回值为string.如果参数不存在，则返回NULL。 ）
9、  getParameterNames方法：（语法：java.util.EnumerationgetParameterNames() 说明：取得所有的参数名称。
10、 getParameterValue方法：（语法：java.lang.stirng[] getParameterValues(string Name) 说明：取得所有参数名称为name的参数值。
11、 getProtocol方法：（语法：java.lang.stringgetProtocol() 说明：取得使用的协议(HTTP/1.1或HTTP/1.0)
12、 getQuerystring方法：（语法：java.lang.stringgetQuerystring()   说明：取得请求的参数字符串(数据发送的方法为get)）.
13、 GetRealPath 方法：（java.lang.stringgetRealPath(java.lang.string虚拟目录地址) 说明：取得虚拟目录对应的磁盘地址。）
14、  GetRequestURL方法：（语法：java.lang.stirng getRequestURI() 说明：取得请求的URI(不包括请求字符串)）
15、 getRemoteAddr方法：（语法：java.lang.string.getRemoteAddr() 说明：取得发出请求的用户的IP地址/）
16、 getRemoteHost方法：（语法：java.lang.stringgetRemoteHost()  说明：取得发出请求的用户的主机名称，如果不能确定主机名则返回用户的IP地址）
17、 getRemoteUser方法：（语法：java.lang.stringgetRemoteUser() 说明：取得发出请求的用户名称。）
18、  setAttribute方法：（语法： voidsetAttribute( java.lang.string 变量名称，object变量值) 说明：将数据保存到request对象内。）
19、 setBufferSize方法：（ 语法：voidsetBufferSize(int  缓冲区大小) 说明：设置缓冲区大小。
20、 removeAttribute方法：（语法：voidremoveAttribute(java.lang.stirng变量 ) 说明：删除保存在request 对象内的数据。
十七、javax.servlet.HttpSession类（说明：JSP中隐含对象Session即为该类的对象，用于处理与当前客户联机会话有关的信息。所属套件：javax.servlet.http）
1、 getAttribute方法：（语法：ObjectgetAttribute(java.lang.stirng变量) 说明：取出保存在Sesson内的变量）。
2、 getCreationTime方法：（语法：longgetCreation Time() 说明：取出此Sesson对象的建立的时间）
3、 getid方法：（语法：java.lang.stringgetID() 说明：取出此Sesson对象的编号）
4、getLastAccesssedTime方法：（语法：long getLastAccessedTime() 说明：取出此Sesson对象的最后存取时间）
5、getMaxInactiovInterval方法：（语法：int getMaxInactiveInterval()  说明：取得目前设置的Sesson对象最大时间间隔，单位为秒。如果用户超过此时间间隔无动作，则Sesson对象失效）
6、 invalidate方法：（语法：voidinvalidate() 说明：强制结束服务端代表此联机的Sesson对象）
7、 isNew方法（语法：booleanisNew() 说明：判断sesson对象是否为刚刚建立的对象）
8、 setAttribute方法：（语法：voidsetAttribute(java.lang.string变量名称，Object变量值) 说明：将数据保存到sesson对象内）
9、 removeAttribute方法：（语法：voidremoveAttribute(java.lang.string变量) 说明：删除保存在sesson对象中的数据）
十八、javax.servlet.jsp.jspWrite类（说明：jsp中隐含对象out即为该 类的对象。所属套件：javax.servlet.jsp）
1、 flush()方法：（语法：voidflush() 说明：强制输出缓冲区中的数据。该方法与HttpServletResponse类的flushBuffer 具有相似的效果）
2、 getBufferSize方法：（语法：intgetBufferSize()  说明：取得目前缓冲区的大小）
3、 getRemaining文法：（语法： intRemaining() 说明：取得目前缓冲区的剩余空间。）
4、 println 方法：（语法：voidprintln(java.lang.string 输出字符串)   说明：输出字符串。）
