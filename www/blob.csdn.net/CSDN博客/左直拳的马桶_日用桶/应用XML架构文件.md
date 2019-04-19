# 应用XML架构文件 - 左直拳的马桶_日用桶 - CSDN博客
2005年10月01日 11:13:00[左直拳](https://me.csdn.net/leftfist)阅读数：2551


应用XML架构文件
工作上，有时会遇到系统间数据交互的问题。之前我做过的方法是，源系统将数据导出为文本文件（TXT后缀），然后上传到目标系统，分析、读取。现在好一点，应用web service，编写一个数据接口，在程序里直接调用，而无须再经过人工上传一道工序。并且web service传送的数据，全部是XML格式，可以很方便的读取，不必再自己写分析算法。当然，也因为是XML格式，每个字段值前后各一个标记，并且是明文，如果记录很多，可能需要传送的数据量就比较大了，这可能是不够好的地方。
由于XML文件只存储数据，并无数据类型描述，因此从XML文件读入数据可能会出现数据类型与原先不符的情况。比如，有一表employee，结构如下：ID（INT），NAME（varchar(50)），在XML里表示为<EMPLOYEE><ID>1</ID><NAME>张三</NAME></EMPLOYEE>。这样，ID的类型原本为整型，而此时系统却以为是字符型！
因为这个情况我们遇到了点问题。
数据导入的时候，往往要加以分析，将导入的数据和现有数据比较，以确定是插入、更新还是删除。比较的算法，最简单当然是将两个数据集（DataSet）来个嵌套循环，逐条记录比较。不过这种方法效率不高，可以改进一下，将两个数据集排好序，然后再来比较。
怎样排序呢？从XML文件里读取数据，存放在数据集dsXml里，而现有数据，存放在数据集dsCur里，每个数据集里都只有一个DataTable。可以用DataTable的Select方法来实现排序，方法如下：
//从XML文件里读取数据
DataSet dsXml = new DataSet(“dsXml”);
DsXml.ReadXml(“employee.xml”);
//从现有数据库中读取数据。GetData是自定义的函数,从现有数据库中提取数据
DataSet dsCur = GetData();
//按照id的顺序排序。
//其中Select第一个参数是过滤条件，这里为空表示所有记录都适合；第二个参数表示排序字段
DataRow[] drsXml = dsXml.Tables[0].Select(“”,”id”);
DataRow[] drsCur = dsCur.Tables[0].Select(“”,”id”);
然后比较这两个记录集：drsXml与drsCur。
但是运行的结果却跟原先设想大相径庭。
什么原因呢？
原来就是因为在dsXml里，字段id是字符型。字符型排序不同于数值排序，如果让系统比较，”9”肯定大于”10”。因此，dsXml的排序没有得到我们预期的效果。
看来要让系统知道id是整型。如何能做到？可以结合XML架构文件。
dsXml.ReadXmlSchema("employee.xsd");
dsXml.ReadXml(“employee.xml”,XmlReadMode.InferSchema);
这样子读出来后，数据结构就跟架构文件employee.xsd里描述的一模一样了。
补充一点，本人对架构文件语法不熟悉，是先用从数据库中拿到的数据集dsCur输出来，然后再稍作修改：
dsCur. WriteXmlSchema(“employee.xsd”); 
