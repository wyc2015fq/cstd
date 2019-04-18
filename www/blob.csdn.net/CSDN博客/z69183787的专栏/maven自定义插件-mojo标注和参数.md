# maven自定义插件-mojo标注和参数 - z69183787的专栏 - CSDN博客
2016年10月31日 18:07:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4073
每个mojo都必须使用@Goal标注来表明其目标名称，否则maven将无法识别该目标。还有其他很多标注，列举如下：
@goal <name>:唯一必须声明的标注，当用户命令行调用或在pom中配置插件是，需使用该目标名称
@phase <phase>:默认将该目标绑定至default声明周期的某个阶段，这样在配置使用插件目标时，就无需声明phase，如maven-surefire-plugin的test目标带有@phase tes标注
@requiresDependencyResolution <scope>:在运行mojo之前必须解析所有指定范围的依赖，如maven-surefire-plugin的test目标带有requiresDependencyResolution test标注，表示执行测试前，所有测试范围的依赖必须得到解析
@requiresProject <true/false>:该目标是否必须在一个maven项目中运行（如测试插件用于测试其他项目），默认为true。大部分插件目标需依赖一个项目才能运行，但是，maven-help-plugin的system目标例外，它用来显示系统属性和环境变量信息，无需实际项目。
@requiresOnline <true/false>:是否要求maven必须是在线状态，默认值为false
@requiresReport <true/false>:是否要求项目报告已经生成，默认为false
@aggregator:当mojo在多模块项目上运行时，该标注表示目标只会在顶层模块运行。
@requiresDirectInvocation <true/false>:为true时，该目标就只能通过命令行直接调用。默认为false
@execute goal="<goal>":在运行该目标之前，让maven运行另外一个目标。如果是本插件目标，则直接调用目标名称，否则，使用“prefix:goal”
@execute phase="<phase>":在运行该目标前，让maven先运行一个并行的生命周期，到指定的阶段为止。到phase执行完，才执行插件目标
@execute lifecycle="<lifecycle>" phase = "<phase>":在运行该目标前，让maven先运行一个自定义的生命周期，到指定的阶段为止。
mojo参数
如可使用@parameter将mojo的某个字段标注为可配置参数，即mojo参数。支持boolean,int,float,String,Date,File,Url,数组,Collection,map,Propertes
boolean(boolean和Boolean)：
/**
[*@parameter](mailto:*@parameter)
*/
private boolean sampleBoolean:对应配置<sampleBoolean>true</sampleBoolean>
int(Integer,long.Long,short,Short,byte,Byte)：
/**
[*@parameter](mailto:*@parameter)
*/
private int sampleInt:对应配置<sampleInt>8</sampleInt>
float(Float,Double,double)：
/**
[*@parameter](mailto:*@parameter)
*/
private float sampleFloat:对应配置<sampleFloat>8.2</sampleFloat>
String(StringBuffer,char,Character)：
/**
[*@parameter](mailto:*@parameter)
*/
private String sampleString:对应配置<sampleString>heoll</sampleString>
Date(yyyy-MM-dd HH:mm:ss.S a或yyyy-MM-dd HH:mm:ssa)：
/**
[*@parameter](mailto:*@parameter)
*/
private Date sampleDate:对应配置<sampleDate>2010-06-09 3:14:55.1 PM或2010-06-09 3:14:55 PM</sampleDate>
File
/**
[*@parameter](mailto:*@parameter)
*/
private File sampleFile:对应配置<sampleFile>c:tem</sampleFile>
URL
/**
[*@parameter](mailto:*@parameter)
*/
private URL sampleUrl:对应配置<sampleUrl>http;//www.baidu.com</sampleUrl>
数组
/**
[*@parameter](mailto:*@parameter)
*/
private String[] includes:对应配置<includes><include>ee</include><include>dd</include></includes>
Collection(任何实现Collection接口的类)
/**
[*@parameter](mailto:*@parameter)
*/
private List includes:对应配置<includes><include>ee</include><include>dd</include></includes>
Map
/**
[*@parameter](mailto:*@parameter)
*/
private Map includes:对应配置<includes><key1>ee</key1><key2>dd</include></key2></includes>
Properties
/**
[*@parameter](mailto:*@parameter)
*/
private Properties includes:对应配置
<includes>
<property>
<name>ee</name>
<value>22</value>
</property>
<property>
<name>dd</name>
<value>11</value>
</property>
</includes>
@parameter额外属性：
@parameter alias="<aliasName>":为mojo参数使用别名
@parameter expression="${aSystemProperty}":使用系统属性表达式对mojo参数进行赋值
@parameter defaultValue="aValue/${anExpression}":提供一个默认值
**可获取mvn -Dxxx 中的参数值**
另外
@readonly:只读属性，不允许配置
@required:必需的属性，未配置，会报错
