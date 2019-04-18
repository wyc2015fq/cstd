# Spring 表达式语言 (SpEL) - z69183787的专栏 - CSDN博客
2016年07月11日 17:42:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1088
// List and Array navigation// evaluates to "Wireless communication"
String invention = parser.parseExpression("Members[0].Inventions[6]").getValue(
        societyContext, String.class);
maps的内容由内指定的key值获得. 在这种情况下，因为对于人员的键映射是字符串，我们可以指定 字符串。
// Officer's Dictionary
Inventor pupin = parser.parseExpression("Officers[*'president*']").getValue(
        societyContext, Inventor.class);
// evaluates to "Idvor"
String city = parser.parseExpression("Officers[*'president*'].PlaceOfBirth.City").getValue(
        societyContext, String.class);
// setting values
parser.parseExpression("Officers[*'advisors*'][0].PlaceOfBirth.Country").setValue(
        societyContext, "Croatia");
### [](http://spring.cndocs.tk/expressions.html#expressions-inline-lists)8.5.3 内联列表
列表可以直接在表达式中使用`{}`符号来表示。
// evaluates to a Java list containing the four numbers
List numbers = (List) parser.parseExpression("{1,2,3,4}").getValue(context);
List listOfLists = (List) parser.parseExpression("{{*'a*',*'b*'},{*'x*',*'y*'}}").getValue(context);
`{}`本身意味着空列表。出于性能的考虑，如果列表本身
 完全由固定面值的则恒定列表被创建以代表 表达，而不是建立一个新的列表上的每个都执行。
### [](http://spring.cndocs.tk/expressions.html#expressions-inline-maps)8.5.4 内联maps
{key:value}`记号的maps也可以使用`表达式直接表示。
// evaluates to a Java map containing the two entries
Map inventorInfo = (Map) parser.parseExpression("{name:'*Nikola*',dob:'*10-July-1856*'}").getValue(context);
Map mapOfMaps = (Map) parser.parseExpression("{name:{first:'*Nikola*',last:'*Tesla*'},dob:{day:10,month:'*July*',year:1856}}").getValue(context);
`{：}`本身意味着一个空映射。出于性能的考虑，如果map本身是由
 固定的文字或其他嵌套结构不变的（list或者map），那么一个恒定的map创建 代表表达，而不是建立新map每次去执行。使用.引用map的key 是可选的，上面的例子并没有使用.引用key。
### [](http://spring.cndocs.tk/expressions.html#expressions-array-construction)8.5.5 array构造
array可以使用熟悉的Java语法，选择性地提供一个初始建立 有在构造时的数组。
int[] numbers1 = (int[]) parser.parseExpression("new int[4]").getValue(context);
// Array with initializerint[] numbers2 = (int[]) parser.parseExpression("new int[]{1,2,3}").getValue(context);
// Multi dimensional arrayint[][] numbers3 = (int[][]) parser.parseExpression("new int[4][5]").getValue(context);
即未被初始化的多维数组也可以被构造。
### [](http://spring.cndocs.tk/expressions.html#expressions-methods)8.5.6 方法
方法被调用通过java典型的编程语法实现，您也可以调用方法 在表示式当中。可变参数也支持。
// string literal, evaluates to "bc"
String c = parser.parseExpression("*'abc*'.substring(2, 3)").getValue(String.class);
// evaluates to trueboolean isMember = parser.parseExpression("isMember(*'Mihajlo Pupin*')").getValue(
        societyContext, Boolean.class);
### [](http://spring.cndocs.tk/expressions.html#expressions-operators)8.5.7 运算符
#### [](http://spring.cndocs.tk/expressions.html#expressions-operators-relational)关系运算符
关系运算符;等于，不等于，小于，小于或等于，大于， 和大于或等于使用标准算子表示法的支持。
// evaluates to trueboolean trueValue = parser.parseExpression("2 == 2").getValue(Boolean.class);
// evaluates to falseboolean falseValue = parser.parseExpression("2 < -5.0").getValue(Boolean.class);
// evaluates to trueboolean trueValue = parser.parseExpression("*'black*' < *'block*'").getValue(Boolean.class);
除了标准的关系运算符SpEL支持`instanceof`和
 增则表达式的`matches`操作。
// evaluates to falseboolean falseValue = parser.parseExpression(
        "*'xyz*' instanceof T(int)").getValue(Boolean.class);
// evaluates to trueboolean trueValue = parser.parseExpression(
        "*'5.00*' matches *'^-?\\d+(\\.\\d{2})?$*'").getValue(Boolean.class);
//evaluates to falseboolean falseValue = parser.parseExpression(
        "*'5.0067*' matches *'\^-?\\d+(\\.\\d{2})?$*'").getValue(Boolean.class);
每个符号操作者也可以被指定为一个纯字母变量。这个 避免了在使用的符号有特殊含义的文档类型的问题 其表达被嵌入（例如，XML文档）。文本是等值 比如: `lt` (`<`),`gt` (`>`), `le` (`<=`), `ge` (`>=`), `eq` (`==`), `ne` (`!=`), `div` (`/`), `mod` (`%`), `not` (`!`).
 这些都是不区分大小写。
#### [](http://spring.cndocs.tk/expressions.html#expressions-operators-logical)逻辑运算符
所以支持的逻辑运算符 and, or, and not. 下文将证明他们的使用
// -- AND --// evaluates to falseboolean falseValue = parser.parseExpression("true and false").getValue(Boolean.class);
// evaluates to true
String expression = "isMember(*'Nikola Tesla*') and isMember(*'Mihajlo Pupin*')";
boolean trueValue = parser.parseExpression(expression).getValue(societyContext, Boolean.class);
// -- OR --// evaluates to trueboolean trueValue = parser.parseExpression("true or false").getValue(Boolean.class);
// evaluates to true
String expression = "isMember(*'Nikola Tesla*') or isMember(*'Albert Einstein*')";
boolean trueValue = parser.parseExpression(expression).getValue(societyContext, Boolean.class);
// -- NOT --// evaluates to falseboolean falseValue = parser.parseExpression("!true").getValue(Boolean.class);
// -- AND and NOT --
String expression = "isMember(*'Nikola Tesla*') and !isMember(*'Mihajlo Pupin*')";
boolean falseValue = parser.parseExpression(expression).getValue(societyContext, Boolean.class);
#### [](http://spring.cndocs.tk/expressions.html#expressions-operators-mathematical)数学运​​算符
加法运算符可以用于数字和字符串。减法，乘法 和除法只能在数字被使用。支持其他数学运算符 模量（％）和指数幂（^）。标准的运算符优先级执行。 这些 运算符展示在下文。
// Additionint two = parser.parseExpression("1 + 1").getValue(Integer.class); // 2
String testString = parser.parseExpression(
        "*'test*' + *' '* + *'string*'").getValue(String.class); // *test string*// Subtractionint four = parser.parseExpression("1 - -3").getValue(Integer.class); // 4double d = parser.parseExpression("1000.00 - 1e4").getValue(Double.class); // -9000// Multiplicationint six = parser.parseExpression("-2 * -3").getValue(Integer.class); // 6double twentyFour = parser.parseExpression("2.0 * 3e0 * 4").getValue(Double.class); // 24.0// Divisionint minusTwo = parser.parseExpression("6 / -3").getValue(Integer.class); // -2double one = parser.parseExpression("8.0 / 4e0 / 2").getValue(Double.class); // 1.0// Modulusint three = parser.parseExpression("7 % 4").getValue(Integer.class); // 3int one = parser.parseExpression("8 / 5 % 2").getValue(Integer.class); // 1// Operator precedenceint minusTwentyOne = parser.parseExpression("1+2-3*8").getValue(Integer.class); // -21
### [](http://spring.cndocs.tk/expressions.html#expressions-assignment)8.5.8 赋值
设置一个属性是通过使用赋值操作完成。这通常是 调用`setValue`，但也可以在调用`getValue`内完成。
Inventor inventor = new Inventor();
StandardEvaluationContext inventorContext = new StandardEvaluationContext(inventor);
parser.parseExpression("Name").setValue(inventorContext, "Alexander Seovic2");
// alternatively
String aleks = parser.parseExpression(
        "Name = *'Alexandar Seovic*'").getValue(inventorContext, String.class);
### [](http://spring.cndocs.tk/expressions.html#expressions-types)8.5.9 类型
`T`操作可以被用来指定安装一个java.lang.ClassClass
 (the *type*). 静态方法也可以使用该运算符调用。然后该 `StandardEvaluationContext`使用`TypeLocator`找到类型和 `StandardTypeLocator`（可替换）是建立与的理解
 java.lang package. 这意味着T()引用内的java.lang类型不需要是 完全限定，但所有其他类型的引用必须。
Class dateClass = parser.parseExpression("T(java.util.Date)").getValue(Class.class);
Class stringClass = parser.parseExpression("T(String)").getValue(Class.class);
boolean trueValue = parser.parseExpression(
        "T(java.math.RoundingMode).CEILING < T(java.math.RoundingMode).FLOOR")
        .getValue(Boolean.class);
### [](http://spring.cndocs.tk/expressions.html#expressions-constructors)8.5.10 构造
构造函数可以使用new运算符调用。所有地方的类名应该是符合要求的， ，除了原语类型和字符串（其中整数，浮点，等等，都可以 使用）。
Inventor einstein = p.parseExpression(
        "new org.spring.samples.spel.inventor.Inventor(*'Albert Einstein*', *'German*')")
        .getValue(Inventor.class);
//create new inventor instance within add method of List
p.parseExpression(
        "Members.add(new org.spring.samples.spel.inventor.Inventor(
*'Albert Einstein*', *'German*'))").getValue(societyContext);
### [](http://spring.cndocs.tk/expressions.html#expressions-ref-variables)8.5.11 变量
变量可以在使用语法`＃variableName`表达引用。变量
 使用在`StandardEvaluationContext`方法的setVariable设置。
Inventor tesla = new Inventor("Nikola Tesla", "Serbian");
StandardEvaluationContext context = new StandardEvaluationContext(tesla);
context.setVariable("newName", "Mike Tesla");
parser.parseExpression("Name = #newName").getValue(context);
System.out.println(tesla.getName()) // "Mike Tesla"
#### [](http://spring.cndocs.tk/expressions.html#expressions-this-root)该＃this 和 #root变量
变量#this 始终定义和指向的是当前的执行对象 （不支持对其中不合格的引用解析）。变量#root总是 定义和指向root context object。虽然＃this可能作为表达式的一些组件被执行 ，但#root总是指 root。
// create an array of integers
List<Integer> primes = new ArrayList<Integer>();
primes.addAll(Arrays.asList(2,3,5,7,11,13,17));
// create parser and set variable *primes* as the array of integers
ExpressionParser parser = new SpelExpressionParser();
StandardEvaluationContext context = new StandardEvaluationContext();
context.setVariable("primes",primes);
// all prime numbers > 10 from the list (using selection ?{...})// evaluates to [11, 13, 17]
List<Integer> primesGreaterThanTen = (List<Integer>) parser.parseExpression(
        "#primes.?[#this>10]").getValue(context);
### [](http://spring.cndocs.tk/expressions.html#expressions-ref-functions)8.5.12 函数
您可以通过注册，可以在该调用用户自定义函数扩展SpEL 表达式字符串。该函数注册到'StandardEvaluationContext`使用 该方法。
publicvoid registerFunction(String name, Method m)
引用一个Java方法提供了函数的实现。举个例子 一个实用的方法来扭转字符串如下所示。
publicabstractclass StringUtils {
    publicstatic String reverseString(String input) {
        StringBuilder backwards = new StringBuilder();
        for (int i = 0; i < input.length(); i++)
            backwards.append(input.charAt(input.length() - 1 - i));
        }
        return backwards.toString();
    }
}
这个方法在解析器上线文当中被注册，作为字符串被调用。
ExpressionParser parser = new SpelExpressionParser();
StandardEvaluationContext context = new StandardEvaluationContext();
context.registerFunction("reverseString",
    StringUtils.class.getDeclaredMethod("reverseString", new Class[] { String.class }));
String helloWorldReversed = parser.parseExpression(
    "#reverseString(*'hello*')").getValue(context, String.class);
### [](http://spring.cndocs.tk/expressions.html#expressions-bean-references)8.5.13 bean引用
如果解析上下文已经配置，那么bean解析器能够 从表达式使用（@）符号查找bean类。
ExpressionParser parser = new SpelExpressionParser();
StandardEvaluationContext context = new StandardEvaluationContext();
context.setBeanResolver(new MyBeanResolver());
// This will end up calling resolve(context,"foo") on MyBeanResolver during evaluation
Object bean = parser.parseExpression("@foo").getValue(context);
### [](http://spring.cndocs.tk/expressions.html#expressions-operator-ternary)8.5.14 三元运算符（IF-THEN-ELSE）
您可以使用三元运算符内部执行的if-then-else条件逻辑 的表达。一个最简单的例子是：
String falseString = parser.parseExpression(
        "false ? *'trueExp*' : *'falseExp*'").getValue(String.class);
在这种情况下，在返回字符串值“falseExp'布尔假的结果。 更多案例:
parser.parseExpression("Name").setValue(societyContext, "IEEE");
societyContext.setVariable("queryName", "Nikola Tesla");
expression = "isMember(#queryName)? #queryName + *' is a member of the '*" +
"+ Name + *' Society*' : #queryName + *' is not a member of the '* + Name + *' Society*'";
String queryResultString = parser.parseExpression(expression)
        .getValue(societyContext, String.class);
// queryResultString = "Nikola Tesla is a member of the IEEE Society"
同时可以在下一节看到 Elvis 运算符 使用一个更短的三元运算符语法。
### [](http://spring.cndocs.tk/expressions.html#expressions-operator-elvis)8.5.15 Elvis操作符
Elvis操作符是三元运算符语法的缩短，并用于在 [Groovy](http://groovy.codehaus.org/Operators#Operators-ElvisOperator(%3F%3A))语言。
 与三元运算符的语法，你通常要重复变量两次， 示例：
String name = "Elvis Presley";
String displayName = name != null ? name : "Unknown";
取而代之，你可以使用Elvis操作符，命名灵感来自猫王的发型风格。
ExpressionParser parser = new SpelExpressionParser();
String name = parser.parseExpression("null?:'*Unknown*'").getValue(String.class);
System.out.println(name); // *Unknown*
这里是一个更复杂的例子。
ExpressionParser parser = new SpelExpressionParser();
Inventor tesla = new Inventor("Nikola Tesla", "Serbian");
StandardEvaluationContext context = new StandardEvaluationContext(tesla);
String name = parser.parseExpression("Name?:'*Elvis Presley*'").getValue(context, String.class);
System.out.println(name); // Nikola Tesla
tesla.setName(null);
name = parser.parseExpression("Name?:'*Elvis Presley*'").getValue(context, String.class);
System.out.println(name); // Elvis Presley
### [](http://spring.cndocs.tk/expressions.html#expressions-operator-safe-navigation)8.5.16 安全导航运算符
安全导航操作符是用来避免'NullPointerException`和来自 该http://groovy.codehaus.org/Operators#Operators-SafeNavigationOperator(%3F.)[Groovy] 语言。通常情况下，当你有一个参考的对象，你可能需要验证 它不是访问方法或对象的属性之前空。为了避免这种情况，该 安全航行运算符将简单地返回空代替抛出的异常。
ExpressionParser parser = new SpelExpressionParser();
Inventor tesla = new Inventor("Nikola Tesla", "Serbian");
tesla.setPlaceOfBirth(new PlaceOfBirth("Smiljan"));
StandardEvaluationContext context = new StandardEvaluationContext(tesla);
String city = parser.parseExpression("PlaceOfBirth?.City").getValue(context, String.class);
System.out.println(city); // Smiljan
tesla.setPlaceOfBirth(null);
city = parser.parseExpression("PlaceOfBirth?.City").getValue(context, String.class);
System.out.println(city); // null - does not throw NullPointerException!!!
[注意]
Elvis操作符可用于应用中的表达式的默认值，例如在一个 
```
@
 Value
```
表达式：
@Value("#{systemProperties[*'pop3.port*'] ?: 25}")
如果它不存在,那么将定义为25
### [](http://spring.cndocs.tk/expressions.html#expressions-collection-selection)8.5.17 集合选择
选择是一个强大的表达式语言功能，他允许你转换一些 源集合到另一个通过其条目选择。
选择使用语法`？[selectionExpression]`.这将过滤收集和
 返回一个包含原有元素的子集的新的集合。例如， 选择使我们能够轻松地获得Serbian inventors的列表：
List<Inventor> list = (List<Inventor>) parser.parseExpression(
        "Members.?[Nationality == *'Serbian*']").getValue(societyContext);
选择可以被使用在lists或者maps集合当中。在前者的情况下，选择 标准执行对每个列表元素，同时针对map 选择定义的操作将会对map中的每个key执行。 （对象类似‘Map.Entry’） Map entries have their key and value accessible as properties for use in the selection。
下面这个例子将返回由原始映射（其中条目值小于27）所取得的这些元素的新map。
Map newMap = parser.parseExpression("map.?[value<27]").getValue();
除了返回所有选定的元素，也可以用来获取 第一或最后一个值。以获得第一条目相匹配的选择的语法是 
```
^
 [...]
```
而获得最后一个匹配选择语法是
```
$
 [...]
```
.
### [](http://spring.cndocs.tk/expressions.html#expressions-collection-projection)8.5.18 集合投影
投影允许集合驱动子表达式和解析 生成一个新的集合。语法投影`！[projectionExpression]`.
 多数 功能通过实例容易理解，假设我们有Inventor list，希望得到 他们出生的城市。有效的方式是我们要使用“placeOfBirth.city”解析 在Inventor 中的每个条目。使用投影：
// returns [*Smiljan*, *Idvor* ]
List placesOfBirth = (List)parser.parseExpression("Members.![placeOfBirth.city]");
一个map也可以用于驱动投影。 在这种情况下，投影表达式 将解析map中的每一个元素（作为Java `Map.Entry`方法的一个代理）。
 通过投影一个map将获得一个由投影表达式遍历每个元素所得到的list。
### [](http://spring.cndocs.tk/expressions.html#expressions-templating)8.5.19 表达模板
表达式模板允许文字文本与一个或多个解析块的混合。 你可以每个解析块分隔前缀和后缀的字符， 当然，常见的选择是使用`＃{}`作为分隔符。例如，
String randomPhrase = parser.parseExpression(
        "random number is #{T(java.lang.Math).random()}",
        new TemplateParserContext()).getValue(String.class);
// evaluates to "random number is 0.7038186818312008"
该字符串是通过连接文字“random number is”与 计算表达式的＃{}定界符获取的结果，在此情况下的结果 中调用一个随机（）方法。第二个参数的方法`parseExpression（）` 是类型`ParserContext`的。在`ParserContext`接口用于影响如何
 表达被解析，以便支持所述表达模板的功能。`的TemplateParserContext`的定义如下所示。
publicclass TemplateParserContext implements ParserContext {
    public String getExpressionPrefix() {
        return"#{";
    }
    public String getExpressionSuffix() {
        return"}";
    }
    publicboolean isTemplate() {
        return true;
    }
}
## [](http://spring.cndocs.tk/expressions.html#expressions-example-classes)8.6 Classes used in the examples
Inventor.java
package org.spring.samples.spel.inventor;
import java.util.Date;
import java.util.GregorianCalendar;
publicclass Inventor {
    private String name;
    private String nationality;
    private String[] inventions;
    private Date birthdate;
    private PlaceOfBirth placeOfBirth;
    public Inventor(String name, String nationality) {
        GregorianCalendar c= new GregorianCalendar();
        this.name = name;
        this.nationality = nationality;
        this.birthdate = c.getTime();
    }
    public Inventor(String name, Date birthdate, String nationality) {
        this.name = name;
        this.nationality = nationality;
        this.birthdate = birthdate;
    }
    public Inventor() {
    }
    public String getName() {
        return name;
    }
    publicvoid setName(String name) {
        this.name = name;
    }
    public String getNationality() {
        return nationality;
    }
    publicvoid setNationality(String nationality) {
        this.nationality = nationality;
    }
    public Date getBirthdate() {
        return birthdate;
    }
    publicvoid setBirthdate(Date birthdate) {
        this.birthdate = birthdate;
    }
    public PlaceOfBirth getPlaceOfBirth() {
        return placeOfBirth;
    }
    publicvoid setPlaceOfBirth(PlaceOfBirth placeOfBirth) {
        this.placeOfBirth = placeOfBirth;
    }
    publicvoid setInventions(String[] inventions) {
        this.inventions = inventions;
    }
    public String[] getInventions() {
        return inventions;
    }
}
PlaceOfBirth.java
package org.spring.samples.spel.inventor;
publicclass PlaceOfBirth {
    private String city;
    private String country;
    public PlaceOfBirth(String city) {
        this.city=city;
    }
    public PlaceOfBirth(String city, String country) {
        this(city);
        this.country = country;
    }
    public String getCity() {
        return city;
    }
    publicvoid setCity(String s) {
        this.city = s;
    }
    public String getCountry() {
        return country;
    }
    publicvoid setCountry(String country) {
        this.country = country;
    }
}
Society.java
package org.spring.samples.spel.inventor;
import java.util.*;
publicclass Society {
    private String name;
    publicstatic String Advisors = "advisors";
    publicstatic String President = "president";
    private List<Inventor> members = new ArrayList<Inventor>();
    private Map officers = new HashMap();
    public List getMembers() {
        return members;
    }
    public Map getOfficers() {
        return officers;
    }
    public String getName() {
        return name;
    }
    publicvoid setName(String name) {
        this.name = name;
    }
    publicboolean isMember(String name) {
        for (Inventor inventor : members) {
            if (inventor.getName().equals(name)) {
                return true;
            }
        }
        return false;
    }
}
// List and Array navigation// evaluates to "Wireless communication"
String invention = parser.parseExpression("Members[0].Inventions[6]").getValue(
        societyContext, String.class);
maps的内容由内指定的key值获得. 在这种情况下，因为对于人员的键映射是字符串，我们可以指定 字符串。
// Officer's Dictionary
Inventor pupin = parser.parseExpression("Officers[*'president*']").getValue(
        societyContext, Inventor.class);
// evaluates to "Idvor"
String city = parser.parseExpression("Officers[*'president*'].PlaceOfBirth.City").getValue(
        societyContext, String.class);
// setting values
parser.parseExpression("Officers[*'advisors*'][0].PlaceOfBirth.Country").setValue(
        societyContext, "Croatia");
### [](http://spring.cndocs.tk/expressions.html#expressions-inline-lists)8.5.3 内联列表
列表可以直接在表达式中使用`{}`符号来表示。
// evaluates to a Java list containing the four numbers
List numbers = (List) parser.parseExpression("{1,2,3,4}").getValue(context);
List listOfLists = (List) parser.parseExpression("{{*'a*',*'b*'},{*'x*',*'y*'}}").getValue(context);
`{}`本身意味着空列表。出于性能的考虑，如果列表本身
 完全由固定面值的则恒定列表被创建以代表 表达，而不是建立一个新的列表上的每个都执行。
### [](http://spring.cndocs.tk/expressions.html#expressions-inline-maps)8.5.4 内联maps
{key:value}`记号的maps也可以使用`表达式直接表示。
// evaluates to a Java map containing the two entries
Map inventorInfo = (Map) parser.parseExpression("{name:'*Nikola*',dob:'*10-July-1856*'}").getValue(context);
Map mapOfMaps = (Map) parser.parseExpression("{name:{first:'*Nikola*',last:'*Tesla*'},dob:{day:10,month:'*July*',year:1856}}").getValue(context);
`{：}`本身意味着一个空映射。出于性能的考虑，如果map本身是由
 固定的文字或其他嵌套结构不变的（list或者map），那么一个恒定的map创建 代表表达，而不是建立新map每次去执行。使用.引用map的key 是可选的，上面的例子并没有使用.引用key。
### [](http://spring.cndocs.tk/expressions.html#expressions-array-construction)8.5.5 array构造
array可以使用熟悉的Java语法，选择性地提供一个初始建立 有在构造时的数组。
int[] numbers1 = (int[]) parser.parseExpression("new int[4]").getValue(context);
// Array with initializerint[] numbers2 = (int[]) parser.parseExpression("new int[]{1,2,3}").getValue(context);
// Multi dimensional arrayint[][] numbers3 = (int[][]) parser.parseExpression("new int[4][5]").getValue(context);
即未被初始化的多维数组也可以被构造。
### [](http://spring.cndocs.tk/expressions.html#expressions-methods)8.5.6 方法
方法被调用通过java典型的编程语法实现，您也可以调用方法 在表示式当中。可变参数也支持。
// string literal, evaluates to "bc"
String c = parser.parseExpression("*'abc*'.substring(2, 3)").getValue(String.class);
// evaluates to trueboolean isMember = parser.parseExpression("isMember(*'Mihajlo Pupin*')").getValue(
        societyContext, Boolean.class);
### [](http://spring.cndocs.tk/expressions.html#expressions-operators)8.5.7 运算符
#### [](http://spring.cndocs.tk/expressions.html#expressions-operators-relational)关系运算符
关系运算符;等于，不等于，小于，小于或等于，大于， 和大于或等于使用标准算子表示法的支持。
// evaluates to trueboolean trueValue = parser.parseExpression("2 == 2").getValue(Boolean.class);
// evaluates to falseboolean falseValue = parser.parseExpression("2 < -5.0").getValue(Boolean.class);
// evaluates to trueboolean trueValue = parser.parseExpression("*'black*' < *'block*'").getValue(Boolean.class);
除了标准的关系运算符SpEL支持`instanceof`和
 增则表达式的`matches`操作。
// evaluates to falseboolean falseValue = parser.parseExpression(
        "*'xyz*' instanceof T(int)").getValue(Boolean.class);
// evaluates to trueboolean trueValue = parser.parseExpression(
        "*'5.00*' matches *'^-?\\d+(\\.\\d{2})?$*'").getValue(Boolean.class);
//evaluates to falseboolean falseValue = parser.parseExpression(
        "*'5.0067*' matches *'\^-?\\d+(\\.\\d{2})?$*'").getValue(Boolean.class);
每个符号操作者也可以被指定为一个纯字母变量。这个 避免了在使用的符号有特殊含义的文档类型的问题 其表达被嵌入（例如，XML文档）。文本是等值 比如: `lt` (`<`),`gt` (`>`), `le` (`<=`), `ge` (`>=`), `eq` (`==`), `ne` (`!=`), `div` (`/`), `mod` (`%`), `not` (`!`).
 这些都是不区分大小写。
#### [](http://spring.cndocs.tk/expressions.html#expressions-operators-logical)逻辑运算符
所以支持的逻辑运算符 and, or, and not. 下文将证明他们的使用
// -- AND --// evaluates to falseboolean falseValue = parser.parseExpression("true and false").getValue(Boolean.class);
// evaluates to true
String expression = "isMember(*'Nikola Tesla*') and isMember(*'Mihajlo Pupin*')";
boolean trueValue = parser.parseExpression(expression).getValue(societyContext, Boolean.class);
// -- OR --// evaluates to trueboolean trueValue = parser.parseExpression("true or false").getValue(Boolean.class);
// evaluates to true
String expression = "isMember(*'Nikola Tesla*') or isMember(*'Albert Einstein*')";
boolean trueValue = parser.parseExpression(expression).getValue(societyContext, Boolean.class);
// -- NOT --// evaluates to falseboolean falseValue = parser.parseExpression("!true").getValue(Boolean.class);
// -- AND and NOT --
String expression = "isMember(*'Nikola Tesla*') and !isMember(*'Mihajlo Pupin*')";
boolean falseValue = parser.parseExpression(expression).getValue(societyContext, Boolean.class);
#### [](http://spring.cndocs.tk/expressions.html#expressions-operators-mathematical)数学运​​算符
加法运算符可以用于数字和字符串。减法，乘法 和除法只能在数字被使用。支持其他数学运算符 模量（％）和指数幂（^）。标准的运算符优先级执行。 这些 运算符展示在下文。
// Additionint two = parser.parseExpression("1 + 1").getValue(Integer.class); // 2
String testString = parser.parseExpression(
        "*'test*' + *' '* + *'string*'").getValue(String.class); // *test string*// Subtractionint four = parser.parseExpression("1 - -3").getValue(Integer.class); // 4double d = parser.parseExpression("1000.00 - 1e4").getValue(Double.class); // -9000// Multiplicationint six = parser.parseExpression("-2 * -3").getValue(Integer.class); // 6double twentyFour = parser.parseExpression("2.0 * 3e0 * 4").getValue(Double.class); // 24.0// Divisionint minusTwo = parser.parseExpression("6 / -3").getValue(Integer.class); // -2double one = parser.parseExpression("8.0 / 4e0 / 2").getValue(Double.class); // 1.0// Modulusint three = parser.parseExpression("7 % 4").getValue(Integer.class); // 3int one = parser.parseExpression("8 / 5 % 2").getValue(Integer.class); // 1// Operator precedenceint minusTwentyOne = parser.parseExpression("1+2-3*8").getValue(Integer.class); // -21
### [](http://spring.cndocs.tk/expressions.html#expressions-assignment)8.5.8 赋值
设置一个属性是通过使用赋值操作完成。这通常是 调用`setValue`，但也可以在调用`getValue`内完成。
Inventor inventor = new Inventor();
StandardEvaluationContext inventorContext = new StandardEvaluationContext(inventor);
parser.parseExpression("Name").setValue(inventorContext, "Alexander Seovic2");
// alternatively
String aleks = parser.parseExpression(
        "Name = *'Alexandar Seovic*'").getValue(inventorContext, String.class);
### [](http://spring.cndocs.tk/expressions.html#expressions-types)8.5.9 类型
`T`操作可以被用来指定安装一个java.lang.ClassClass
 (the *type*). 静态方法也可以使用该运算符调用。然后该 `StandardEvaluationContext`使用`TypeLocator`找到类型和 `StandardTypeLocator`（可替换）是建立与的理解
 java.lang package. 这意味着T()引用内的java.lang类型不需要是 完全限定，但所有其他类型的引用必须。
Class dateClass = parser.parseExpression("T(java.util.Date)").getValue(Class.class);
Class stringClass = parser.parseExpression("T(String)").getValue(Class.class);
boolean trueValue = parser.parseExpression(
        "T(java.math.RoundingMode).CEILING < T(java.math.RoundingMode).FLOOR")
        .getValue(Boolean.class);
### [](http://spring.cndocs.tk/expressions.html#expressions-constructors)8.5.10 构造
构造函数可以使用new运算符调用。所有地方的类名应该是符合要求的， ，除了原语类型和字符串（其中整数，浮点，等等，都可以 使用）。
Inventor einstein = p.parseExpression(
        "new org.spring.samples.spel.inventor.Inventor(*'Albert Einstein*', *'German*')")
        .getValue(Inventor.class);
//create new inventor instance within add method of List
p.parseExpression(
        "Members.add(new org.spring.samples.spel.inventor.Inventor(
*'Albert Einstein*', *'German*'))").getValue(societyContext);
### [](http://spring.cndocs.tk/expressions.html#expressions-ref-variables)8.5.11 变量
变量可以在使用语法`＃variableName`表达引用。变量
 使用在`StandardEvaluationContext`方法的setVariable设置。
Inventor tesla = new Inventor("Nikola Tesla", "Serbian");
StandardEvaluationContext context = new StandardEvaluationContext(tesla);
context.setVariable("newName", "Mike Tesla");
parser.parseExpression("Name = #newName").getValue(context);
System.out.println(tesla.getName()) // "Mike Tesla"
#### [](http://spring.cndocs.tk/expressions.html#expressions-this-root)该＃this 和 #root变量
变量#this 始终定义和指向的是当前的执行对象 （不支持对其中不合格的引用解析）。变量#root总是 定义和指向root context object。虽然＃this可能作为表达式的一些组件被执行 ，但#root总是指 root。
// create an array of integers
List<Integer> primes = new ArrayList<Integer>();
primes.addAll(Arrays.asList(2,3,5,7,11,13,17));
// create parser and set variable *primes* as the array of integers
ExpressionParser parser = new SpelExpressionParser();
StandardEvaluationContext context = new StandardEvaluationContext();
context.setVariable("primes",primes);
// all prime numbers > 10 from the list (using selection ?{...})// evaluates to [11, 13, 17]
List<Integer> primesGreaterThanTen = (List<Integer>) parser.parseExpression(
        "#primes.?[#this>10]").getValue(context);
### [](http://spring.cndocs.tk/expressions.html#expressions-ref-functions)8.5.12 函数
您可以通过注册，可以在该调用用户自定义函数扩展SpEL 表达式字符串。该函数注册到'StandardEvaluationContext`使用 该方法。
publicvoid registerFunction(String name, Method m)
引用一个Java方法提供了函数的实现。举个例子 一个实用的方法来扭转字符串如下所示。
publicabstractclass StringUtils {
    publicstatic String reverseString(String input) {
        StringBuilder backwards = new StringBuilder();
        for (int i = 0; i < input.length(); i++)
            backwards.append(input.charAt(input.length() - 1 - i));
        }
        return backwards.toString();
    }
}
这个方法在解析器上线文当中被注册，作为字符串被调用。
ExpressionParser parser = new SpelExpressionParser();
StandardEvaluationContext context = new StandardEvaluationContext();
context.registerFunction("reverseString",
    StringUtils.class.getDeclaredMethod("reverseString", new Class[] { String.class }));
String helloWorldReversed = parser.parseExpression(
    "#reverseString(*'hello*')").getValue(context, String.class);
### [](http://spring.cndocs.tk/expressions.html#expressions-bean-references)8.5.13 bean引用
如果解析上下文已经配置，那么bean解析器能够 从表达式使用（@）符号查找bean类。
ExpressionParser parser = new SpelExpressionParser();
StandardEvaluationContext context = new StandardEvaluationContext();
context.setBeanResolver(new MyBeanResolver());
// This will end up calling resolve(context,"foo") on MyBeanResolver during evaluation
Object bean = parser.parseExpression("@foo").getValue(context);
### [](http://spring.cndocs.tk/expressions.html#expressions-operator-ternary)8.5.14 三元运算符（IF-THEN-ELSE）
您可以使用三元运算符内部执行的if-then-else条件逻辑 的表达。一个最简单的例子是：
String falseString = parser.parseExpression(
        "false ? *'trueExp*' : *'falseExp*'").getValue(String.class);
在这种情况下，在返回字符串值“falseExp'布尔假的结果。 更多案例:
parser.parseExpression("Name").setValue(societyContext, "IEEE");
societyContext.setVariable("queryName", "Nikola Tesla");
expression = "isMember(#queryName)? #queryName + *' is a member of the '*" +
"+ Name + *' Society*' : #queryName + *' is not a member of the '* + Name + *' Society*'";
String queryResultString = parser.parseExpression(expression)
        .getValue(societyContext, String.class);
// queryResultString = "Nikola Tesla is a member of the IEEE Society"
同时可以在下一节看到 Elvis 运算符 使用一个更短的三元运算符语法。
### [](http://spring.cndocs.tk/expressions.html#expressions-operator-elvis)8.5.15 Elvis操作符
Elvis操作符是三元运算符语法的缩短，并用于在 [Groovy](http://groovy.codehaus.org/Operators#Operators-ElvisOperator(%3F%3A))语言。
 与三元运算符的语法，你通常要重复变量两次， 示例：
String name = "Elvis Presley";
String displayName = name != null ? name : "Unknown";
取而代之，你可以使用Elvis操作符，命名灵感来自猫王的发型风格。
ExpressionParser parser = new SpelExpressionParser();
String name = parser.parseExpression("null?:'*Unknown*'").getValue(String.class);
System.out.println(name); // *Unknown*
这里是一个更复杂的例子。
ExpressionParser parser = new SpelExpressionParser();
Inventor tesla = new Inventor("Nikola Tesla", "Serbian");
StandardEvaluationContext context = new StandardEvaluationContext(tesla);
String name = parser.parseExpression("Name?:'*Elvis Presley*'").getValue(context, String.class);
System.out.println(name); // Nikola Tesla
tesla.setName(null);
name = parser.parseExpression("Name?:'*Elvis Presley*'").getValue(context, String.class);
System.out.println(name); // Elvis Presley
### [](http://spring.cndocs.tk/expressions.html#expressions-operator-safe-navigation)8.5.16 安全导航运算符
安全导航操作符是用来避免'NullPointerException`和来自 该http://groovy.codehaus.org/Operators#Operators-SafeNavigationOperator(%3F.)[Groovy] 语言。通常情况下，当你有一个参考的对象，你可能需要验证 它不是访问方法或对象的属性之前空。为了避免这种情况，该 安全航行运算符将简单地返回空代替抛出的异常。
ExpressionParser parser = new SpelExpressionParser();
Inventor tesla = new Inventor("Nikola Tesla", "Serbian");
tesla.setPlaceOfBirth(new PlaceOfBirth("Smiljan"));
StandardEvaluationContext context = new StandardEvaluationContext(tesla);
String city = parser.parseExpression("PlaceOfBirth?.City").getValue(context, String.class);
System.out.println(city); // Smiljan
tesla.setPlaceOfBirth(null);
city = parser.parseExpression("PlaceOfBirth?.City").getValue(context, String.class);
System.out.println(city); // null - does not throw NullPointerException!!!
[注意]
Elvis操作符可用于应用中的表达式的默认值，例如在一个 
```
@
 Value
```
表达式：
@Value("#{systemProperties[*'pop3.port*'] ?: 25}")
如果它不存在,那么将定义为25
### [](http://spring.cndocs.tk/expressions.html#expressions-collection-selection)8.5.17 集合选择
选择是一个强大的表达式语言功能，他允许你转换一些 源集合到另一个通过其条目选择。
选择使用语法`？[selectionExpression]`.这将过滤收集和
 返回一个包含原有元素的子集的新的集合。例如， 选择使我们能够轻松地获得Serbian inventors的列表：
List<Inventor> list = (List<Inventor>) parser.parseExpression(
        "Members.?[Nationality == *'Serbian*']").getValue(societyContext);
选择可以被使用在lists或者maps集合当中。在前者的情况下，选择 标准执行对每个列表元素，同时针对map 选择定义的操作将会对map中的每个key执行。 （对象类似‘Map.Entry’） Map entries have their key and value accessible as properties for use in the selection。
下面这个例子将返回由原始映射（其中条目值小于27）所取得的这些元素的新map。
Map newMap = parser.parseExpression("map.?[value<27]").getValue();
除了返回所有选定的元素，也可以用来获取 第一或最后一个值。以获得第一条目相匹配的选择的语法是 
```
^
 [...]
```
而获得最后一个匹配选择语法是
```
$
 [...]
```
.
### [](http://spring.cndocs.tk/expressions.html#expressions-collection-projection)8.5.18 集合投影
投影允许集合驱动子表达式和解析 生成一个新的集合。语法投影`！[projectionExpression]`.
 多数 功能通过实例容易理解，假设我们有Inventor list，希望得到 他们出生的城市。有效的方式是我们要使用“placeOfBirth.city”解析 在Inventor 中的每个条目。使用投影：
// returns [*Smiljan*, *Idvor* ]
List placesOfBirth = (List)parser.parseExpression("Members.![placeOfBirth.city]");
一个map也可以用于驱动投影。 在这种情况下，投影表达式 将解析map中的每一个元素（作为Java `Map.Entry`方法的一个代理）。
 通过投影一个map将获得一个由投影表达式遍历每个元素所得到的list。
### [](http://spring.cndocs.tk/expressions.html#expressions-templating)8.5.19 表达模板
表达式模板允许文字文本与一个或多个解析块的混合。 你可以每个解析块分隔前缀和后缀的字符， 当然，常见的选择是使用`＃{}`作为分隔符。例如，
String randomPhrase = parser.parseExpression(
        "random number is #{T(java.lang.Math).random()}",
        new TemplateParserContext()).getValue(String.class);
// evaluates to "random number is 0.7038186818312008"
该字符串是通过连接文字“random number is”与 计算表达式的＃{}定界符获取的结果，在此情况下的结果 中调用一个随机（）方法。第二个参数的方法`parseExpression（）` 是类型`ParserContext`的。在`ParserContext`接口用于影响如何
 表达被解析，以便支持所述表达模板的功能。`的TemplateParserContext`的定义如下所示。
publicclass TemplateParserContext implements ParserContext {
    public String getExpressionPrefix() {
        return"#{";
    }
    public String getExpressionSuffix() {
        return"}";
    }
    publicboolean isTemplate() {
        return true;
    }
}
## [](http://spring.cndocs.tk/expressions.html#expressions-example-classes)8.6 Classes used in the examples
Inventor.java
package org.spring.samples.spel.inventor;
import java.util.Date;
import java.util.GregorianCalendar;
publicclass Inventor {
    private String name;
    private String nationality;
    private String[] inventions;
    private Date birthdate;
    private PlaceOfBirth placeOfBirth;
    public Inventor(String name, String nationality) {
        GregorianCalendar c= new GregorianCalendar();
        this.name = name;
        this.nationality = nationality;
        this.birthdate = c.getTime();
    }
    public Inventor(String name, Date birthdate, String nationality) {
        this.name = name;
        this.nationality = nationality;
        this.birthdate = birthdate;
    }
    public Inventor() {
    }
    public String getName() {
        return name;
    }
    publicvoid setName(String name) {
        this.name = name;
    }
    public String getNationality() {
        return nationality;
    }
    publicvoid setNationality(String nationality) {
        this.nationality = nationality;
    }
    public Date getBirthdate() {
        return birthdate;
    }
    publicvoid setBirthdate(Date birthdate) {
        this.birthdate = birthdate;
    }
    public PlaceOfBirth getPlaceOfBirth() {
        return placeOfBirth;
    }
    publicvoid setPlaceOfBirth(PlaceOfBirth placeOfBirth) {
        this.placeOfBirth = placeOfBirth;
    }
    publicvoid setInventions(String[] inventions) {
        this.inventions = inventions;
    }
    public String[] getInventions() {
        return inventions;
    }
}
PlaceOfBirth.java
package org.spring.samples.spel.inventor;
publicclass PlaceOfBirth {
    private String city;
    private String country;
    public PlaceOfBirth(String city) {
        this.city=city;
    }
    public PlaceOfBirth(String city, String country) {
        this(city);
        this.country = country;
    }
    public String getCity() {
        return city;
    }
    publicvoid setCity(String s) {
        this.city = s;
    }
    public String getCountry() {
        return country;
    }
    publicvoid setCountry(String country) {
        this.country = country;
    }
}
Society.java
package org.spring.samples.spel.inventor;
import java.util.*;
publicclass Society {
    private String name;
    publicstatic String Advisors = "advisors";
    publicstatic String President = "president";
    private List<Inventor> members = new ArrayList<Inventor>();
    private Map officers = new HashMap();
    public List getMembers() {
        return members;
    }
    public Map getOfficers() {
        return officers;
    }
    public String getName() {
        return name;
    }
    publicvoid setName(String name) {
        this.name = name;
    }
    publicboolean isMember(String name) {
        for (Inventor inventor : members) {
            if (inventor.getName().equals(name)) {
                return true;
            }
        }
        return false;
    }
}
