
# TestNG配置注解 - anlcy - 博客园






# [TestNG配置注解](https://www.cnblogs.com/camilla/p/6839766.html)
以下是TestNG支持的注释列表：
|注解|描述|
|---|---|
|[@BeforeSuite](https://github.com/BeforeSuite)|在该套件的所有测试都运行在注释的方法之前，仅运行一次。|
|[@AfterSuite](https://github.com/AfterSuite)|在该套件的所有测试都运行在注释方法之后，仅运行一次。|
|[@BeforeClass](https://github.com/BeforeClass)|在调用当前类的第一个测试方法之前运行，注释方法仅运行一次。|
|[@AfterClass](https://github.com/AfterClass)|在调用当前类的第一个测试方法之后运行，注释方法仅运行一次|
|[@BeforeTest](https://github.com/BeforeTest)|注释的方法将在属于|<test>|标签内的类的所有测试方法运行之前运行。|
|[@AfterTest](https://github.com/AfterTest)|注释的方法将在属于|<test>|标签内的类的所有测试方法运行之后运行。|
|[@BeforeGroups](https://github.com/BeforeGroups)|配置方法将在之前运行组列表。 此方法保证在调用属于这些组中的任何一个的第一个测试方法之前不久运行。|
|[@AfterGroups](https://github.com/AfterGroups)|此配置方法将在之后运行组列表。该方法保证在调用属于任何这些组的最后一个测试方法之后不久运行。|
|[@BeforeMethod](https://github.com/BeforeMethod)|注释方法将在每个测试方法之前运行。|
|[@AfterMethod](https://github.com/AfterMethod)|注释方法将在每个测试方法之后运行。|
|[@DataProvider](https://github.com/DataProvider)|标记一种方法来提供测试方法的数据。 注释方法必须返回一个|Object [] []|，其中每个|Object []|可以被分配给测试方法的参数列表。 要从该|DataProvider|接收数据的|[@Test](https://github.com/Test)|方法需要使用与此注释名称相等的|dataProvider|名称。|
|[@Factory](https://github.com/Factory)|将一个方法标记为工厂，返回|TestNG|将被用作测试类的对象。 该方法必须返回|Object []|。|
|[@Listeners](https://github.com/Listeners)|定义测试类上的侦听器。|
|[@Parameters](https://github.com/Parameters)|描述如何将参数传递给|[@Test](https://github.com/Test)|方法。|
|[@Test](https://github.com/Test)|将类或方法标记为测试的一部分。|

## 使用注释/注解的好处
以下是使用注释/注解的一些好处：
TestNG通过查找注释/注解来识别它感兴趣的方法。 因此，方法名称不限于任何模式或格式。
可以将其他参数传递给注释。
注释是强类型的，所以编译器会马上标记任何错误。
测试类不再需要扩展任何东西(如TestCase，对于JUnit3)。





