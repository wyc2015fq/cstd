# Java代码规范编程 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年10月30日 17:18:59[boonya](https://me.csdn.net/boonya)阅读数：2404








**1、代码注释规范**

【情景一】在接口类添加方法注释

　　 对接口的方法加以说明是相当有必要的，方法的作用、参数的名称、以及返回类型都需要做出明确的定义。如果接口添加了完整的注释，在它的实现类基本上都不需要注释了（私有方法除外），开发者将鼠标悬停在该接口的实现方法上就可以看到接口所定义的参数的含义，尽量做有意义的注释而不是到处都是注释。
【情景二】在数据访问层类添加方法注释

        此类是与数据库打交道的，所以必须加以注释，表述清楚方法是用来干什么的，描述清晰之后才能让业务逻辑按照需要来进行调用。
【情景三】在工具类或视图控制层添加注释

      工具类注释是要让工具的使用者知道：工具方法的参数类型及其含义、返回值是什么，以便进行使用。 视图控制类注释则是需要表明某个方法是：为了完成页面上的某个视图展现而操作的。这里的参数、返回值可以不加注释，因为在逻辑控制层已经体现了我们需要哪些参数。
【情景四】为Java文件添加类注释

     类注释需要对该文件的作用和功能做出简短的描述，包括：文件名称、文件描述、内容摘要、版权所属、完成时间、修改记录。格式如下：




```java
/**
 * 
 * <li>文件名称: ObjectReuse</li> 
 * <li>文件描述: $</li> 
 * <li>内容摘要: 包括模块、函数及其功能的说明</li>
 * <li>版权所属: XX公司</li>
 * <li>完成日期：2013-10-24</li> 
 * <li>修改记录1:Little Tom 修改内容描述</li>
 *
 */
public class ObjectReuse
{

}
```



【情景五】添加行注释、以及补漏注释
**行注释：**”//”一般在方法内部使用，行注释应写于需要注释代码的上一行。

Java代码示例：


```java
public void fileOperate() throws IOException
{
   // 获取输入文件
   File srcfile = new File("D:\\myfiles\\my.txt");

   // 获取输入流
   InputStream is = new FileInputStream(srcfile);

   // 设置输出文件
   File destfile = new File("D:\\myfiles\\test.txt");

   // 获取输出流
   OutputStream os = new FileOutputStream(destfile);
        
   //............
}
```



**注释补漏：**一般使用“/**/”，它是在方法调用过程中调用重载方法的时候最常用到的，他可以不需要删除参数，需要用的时候随时去掉注释即可。

Java示例代码：


```java
public void dtest(int a,int b){

}

public void dtest(int a,int b,int c){

}

public static void main(String[] args) throws IOException
{
   ObjectReuse or=new ObjectReuse();
   or.dtest(1, /*b,*/ 3);
}
```



**2、代码结构规范：MVC模式与三层架构**

        以MVC模式加以引导，MVC释义：module（模型：负责出来业务逻辑以及数据库的交互）、view（视图：显示数据和提交数据 ）、controller（控制器：捕获请求并控制请求转发，控制数据流向、输出到哪个页面）。


![](https://img-blog.csdn.net/20131030171732234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        在代码中的体现最重要的就是分层思想的运用，代码分层结构分为三层：数据访问层、业务逻辑层、表现层。看到这里是不是觉得这两个东西很相似呢？很多人都会将这连个东西联系在一起。实际上他们却不是一个东西。


![](https://img-blog.csdn.net/20131030171739265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        三层架构是界面层（UI/USL）业务逻辑层（BLL）和数据访问层（DAL）构成的，而MVC是模型层（M）界面层（View）和控制层（Controller）构成的，而且他们之间是不对应的。

       如果硬要给他们对应的话，那么三层架构中的UI对应MVC中的view（jsp），都是用于显示以及获取界面的数据；三层架构中的BLL层和DAL层对应MVC中的Model（javabean）层都是用于处理上层传递来的数据以及从数据库获取的数据的；MVC中的Controller（Servlet）最多算是三层架构中的UI的一部分，也就我们常说的是Servlet。


![](https://img-blog.csdn.net/20131030171746015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**MVC与三层架构的设计思想是一致的那就是：**分层、解耦(把视图设计与数据持久化进行分离，从而降低耦合性，易于扩展，提高团队开发效率)。


在代码实现中严格按照三层架构来写，便于代码维护和扩展。



USL(restLink/servlet) ->BLL（I*Service ->  *Service)  -> DAL(*Dao)


【情景一】至上而下调用减少无谓的数据库操作

　　如果采用了三次架构的代码规范，在业务逻辑处理的时候就可以处理掉一些为空等条件判断、就不需要在数据访问层进行处理这类问题了；另外如果是业务发生改变只需要在逻辑代码层增加逻辑处理即可而不需要每次都去数据访问层做改动，使代码非常灵活，易于扩展。

       常用的数据库操作无非是增、删、改、查，所以真正变化的部分是对业务逻辑的处理。所以service层就是来适应业务需要做相应的处理的逻辑代码。
*Action->  *Serives(I*service ->  *Service ) -> *Dao

按照这个顺序访问程度代码，层与层之间的访问是透明的，但实现是不透明的。大多数时候，我们关心的是方法是否可以使用，而并不关心他们是怎样实现的。

表现层：Action可以是servlet也可以是其他方式。
**业务访问层：**


接口类：


```java
public interface IPersonService
{
    public Person updatePerson(Person person);
}
```

实现类：

```java
import javax.annotation.Resource;

public class PersonService implements IPersonService
{
   @Resource
   PersonDao personDao;

   @Override
   public Person updatePerson(Person person)
   {
      boolean success=personDao.edit(person);
      if(success){
          return person;
      }
     return null;
   }
}
```



**数据访问层：数据访问的方法可以有多个**


```java
public class PersonDao
{

  public boolean add(Person person){
     return false;
  }

  public boolean delete(int id){
     return false;
  }

  public boolean edit(Person person){
     return false;
  }

  public Person getPersonById(int id){
     return null;
  }

  public List<Person> getPersonList(){
    return null;
  }
}
```

如果要新增不同的业务，只需要新增接口和对应的实现就行了，不用做太多的数据访问方面的改动。



【情景二】无需等待让接口决定实现

     这是敏捷开发中的一个要求，首先定义好接口，不用知道对方是怎么实现的，调用就行了，这样可以避免因等待接口而导致的工作滞后问题。

     在团队开发中，最常见的就是在同事的配合下完成工作，常常需要对方提供接口，我们需要要天天都去催促他赶快完成给你使用的接口吗，不需要的，因为给你提供接口只是别人工作中的一小部分工作。只要接口被定义（方法名称、参数、返回数据）明确了，我们就可以直接调用，保证自己的程序不出差错就可以了。

     此处以修改用户是否能跳舞的接口为例。

步骤一：定义需要的接口


```java
public interface ISports
{
   public void dance(String name, boolean canDance);
}
```

步骤二：接口调用

```java
import javax.annotation.Resource;

public class SportDoing 
{
    @Resource
    ISports sports;

   public void changeDance(String name,boolean canDance){
       sports.dance(name, canDance);
   }
}
```



【情景三】方法必须遵循单一职能原则

   一个方法的存在是为了解决一个或一类问题，它可以被系统中其它有有相同业务功能的操作提供调用和辅助，不需要重复造车轮。

   如果A类中，和B类中同时用到了一个方法，而该方法逻辑上并不隶属于A、B任何一方，这时候就可以将该方法单独提出来作为一个类或是写成工具方法。

   这里以一个时间格式化为例。

Java代码示例：


```java
//class A
public String getCurrentTimeStr(){
   SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
   return formatter.format(new Date());
}

//class B
public String getCurrentDateStr(){
    SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd");
    return formatter.format(new Date());
}
    
//提炼的方法
public static String formatDate(Date date,String format){
    SimpleDateFormat formatter = new SimpleDateFormat(format);
    return formatter.format(date);
}
```

而这里的formatDate方法可能会被A、B以外的类用到。



**3、代码风格规范**

【情景一】代码块分行和缩进

  代码块作用域以”{”开始，”}”结束，最好二者对齐，缩进一致。


```java
public void dtest(int a,int b)
 {
       //要求换行时要有一定缩进，便于阅读
 }
```

这样便于查看方法体是否已经结束。
【情景二】行注释代码间隔

   行注释建议间隔一行代码，不至于使代码看起来太过紧凑，同时增强可读性。



Java代码：


```java
// 获取输入流
InputStream is = new FileInputStream(srcfile);

// 设置输出文件
File destfile = new File("D:\\myfiles\\test.txt");
```
【情景三】统一代码格式化与模版配置

        修改Eclipse的代码风格格式，可以导入自定义的代码格式化工具配置和代码模版工具配置的XML文件。只有所有的开发人员都导入这些文件配置才能保证代码风格的统一性。这两个文件的模版网上都有现成的，可以下载下来做相应的修改来规范代码开发。





