# [Drools]JAVA规则引擎 -- Drools - z69183787的专栏 - CSDN博客
2015年08月02日 07:53:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：846
个人分类：[规则引擎-Drools](https://blog.csdn.net/z69183787/article/category/5701601)
[http://blog.csdn.net/dac55300424/article/details/12950701](http://blog.csdn.net/dac55300424/article/details/12950701)
Drools是一个基于java的规则引擎，开源的，可以将复杂多变的规则从硬编码中解放出来，以规则脚本的形式存放在文件中，使得规则的变更不需要修正代码重启机器就可以立即在线上环境生效。
 本文所使用的demo已上传 [http://download.csdn.net/source/3002213](http://download.csdn.net/source/3002213)
## 1、Drools语法
开始语法之前首先要了解一下drools的基本工作过程，通常而言我们使用一个接口来做事情，首先要穿进去参数，其次要获取到接口的实现执行完毕后的结果，而drools也是一样的，我们需要传递进去数据，用于规则的检查，调用外部接口，同时还可能需要获取到规则执行完毕后得到的结果。在drools中，这个传递数据进去的对象，术语叫 Fact对象。Fact对象是一个普通的java bean，规则中可以对当前的对象进行任何的读写操作，调用该对象提供的方法，当一个java bean插入到workingMemory中，规则使用的是原有对象的引用，规则通过对fact对象的读写，实现对应用数据的读写，对于其中的属性，需要提供getter
 setter访问器，规则中，可以动态的往当前workingMemory中插入删除新的fact对象。
规则文件可以使用 .drl文件，也可以是xml文件，这里我们使用drl文件。
规则语法：
package：对一个规则文件而言，package是必须定义的，必须放在规则文件第一行。特别的是，package的名字是随意的，不必必须对应物理路径，跟java的package的概念不同，这里只是逻辑上的一种区分。同样的package下定义的function和query等可以直接使用。
比如：package com.drools.demo.point
import：导入规则文件需要使用到的外部变量，这里的使用方法跟java相同，但是不同于java的是，这里的import导入的不仅仅可以是一个类，也可以是这个类中的某一个可访问的静态方法。
比如：
import com.drools.demo.point.PointDomain;
import com.drools.demo.point.PointDomain.getById;
rule：定义一个规则。rule "ruleName"。一个规则可以包含三个部分：
属性部分：定义当前规则执行的一些属性等，比如是否可被重复执行、过期时间、生效时间等。
条件部分，即LHS，定义当前规则的条件，如  when Message(); 判断当前workingMemory中是否存在Message对象。
结果部分，即RHS，这里可以写普通java代码，即当前规则条件满足后执行的操作，可以直接调用Fact对象的方法来操作应用。
规则事例：
rule "name"
       no-loop true
       when
               $message:Message(status == 0)
       then
               System.out.println("fit");
               $message.setStatus(1);
               update($message);
end
上述的属性中：
no-loop : 定义当前的规则是否不允许多次循环执行，默认是false，也就是当前的规则只要满足条件，可以无限次执行。什么情况下会出现一条规则执行过一次又被多次重复执行呢？drools提供了一些api，可以对当前传入workingMemory中的Fact对象进行修改或者个数的增减，比如上述的update方法，就是将当前的workingMemory中的Message类型的Fact对象进行属性更新，这种操作会触发规则的重新匹配执行，可以理解为Fact对象更新了，所以规则需要重新匹配一遍，那么疑问是之前规则执行过并且修改过的那些Fact对象的属性的数据会不会被重置？结果是不会，已经修改过了就不会被重置，update之后，之前的修改都会生效。当然对Fact对象数据的修改并不是一定需要调用update才可以生效，简单的使用set方法设置就可以完成，这里类似于java的引用调用，所以何时使用update是一个需要仔细考虑的问题，一旦不慎，极有可能会造成规则的死循环。上述的no-loop
 true，即设置当前的规则，只执行一次，如果本身的RHS部分有update等触发规则重新执行的操作，也不要再次执行当前规则。
但是其他的规则会被重新执行，岂不是也会有可能造成多次重复执行，数据紊乱甚至死循环？答案是使用其他的标签限制，也是可以控制的：lock-on-active true
lock-on-active true：通过这个标签，可以控制当前的规则只会被执行一次，因为一个规则的重复执行不一定是本身触发的，也可能是其他规则触发的，所以这个是no-loop的加强版。当然该标签正规的用法会有其他的标签的配合，后续提及。
date-expires：设置规则的过期时间，默认的时间格式：“日-月-年”，中英文格式相同，但是写法要用各自对应的语言，比如中文："29-七月-2010"，但是还是推荐使用更为精确和习惯的格式，这需要手动在java代码中设置当前系统的时间格式，后续提及。属性用法举例：date-expires
 "2011-01-31 23:59:59" // 这里我们使用了更为习惯的时间格式
date-effective：设置规则的生效时间，时间格式同上。
duration：规则定时，duration 3000   3秒后执行规则
salience：优先级，数值越大越先执行，这个可以控制规则的执行顺序。
其他的属性可以参照相关的api文档查看具体用法，此处略。
规则的条件部分，即LHS部分：
when：规则条件开始。条件可以单个，也可以多个，多个条件一次排列，比如
 when
         eval(true)
         $customer:Customer()
         $message:Message(status==0)
上述罗列了三个条件，当前规则只有在这三个条件都匹配的时候才会执行RHS部分，三个条件中第一个
eval(true)：是一个默认的api，true 无条件执行，类似于 while(true)
$message:Message(status==0) 这句话标示的：当前的workingMemory存在Message类型并且status属性的值为0的Fact对象，这个对象通常是通过外部java代码插入或者自己在前面已经执行的规则的RHS部分中insert进去的。
前面的$message代表着当前条件的引用变量，在后续的条件部分和RHS部分中，可以使用当前的变量去引用符合条件的FACT对象，修改属性或者调用方法等。可选，如果不需要使用，则可以不写。
条件可以有组合，比如：
Message(status==0 || (status > 1 && status <=100))
RHS中对Fact对象private属性的操作必须使用getter和setter方法，而RHS中则必须要直接用.的方法去使用，比如
  $order:Order(name=="qu")
  $message:Message(status==0 && orders contains $order && $order.name=="qu")
特别的是，如果条件全部是 &&关系，可以使用“,”来替代，但是两者不能混用
如果现在Fact对象中有一个List，需要判断条件，如何判断呢？
看一个例子：
Message {
        int status;
        List<String> names;
}
$message:Message(status==0 && names contains "网易" && names.size >= 1)
上述的条件中，status必须是0，并且names列表中含有“网易”并且列表长度大于等于1
contains：对比是否包含操作，操作的被包含目标可以是一个复杂对象也可以是一个简单的值。
Drools提供了十二中类型比较操作符：
>  >=  <  <=  ==  !=  contains / not contains / memberOf / not memberOf /matches/ not matches
not contains：与contains相反。
memberOf：判断某个Fact属性值是否在某个集合中，与contains不同的是他被比较的对象是一个集合，而contains被比较的对象是单个值或者对象。
not memberOf：正好相反。
matches：正则表达式匹配，与java不同的是，不用考虑'/'的转义问题
not matches:正好相反。
规则的结果部分
当规则条件满足，则进入规则结果部分执行，结果部分可以是纯java代码，比如：
then
       System.out.println("OK"); //会在控制台打印出ok
end
当然也可以调用Fact的方法，比如  $message.execute();操作数据库等等一切操作。
结果部分也有drools提供的方法：
insert：往当前workingMemory中插入一个新的Fact对象，会触发规则的再次执行，除非使用no-loop限定；
update：更新
modify：修改，与update语法不同，结果都是更新操作
retract：删除
RHS部分除了调用Drools提供的api和Fact对象的方法，也可以调用规则文件中定义的方法，方法的定义使用 function 关键字
function void console {
   System.out.println();
   StringUtils.getId();// 调用外部静态方法，StringUtils必须使用import导入，getId()必须是静态方法
}
Drools还有一个可以定义类的关键字：
declare 可以再规则文件中定义一个class，使用起来跟普通java对象相似，你可以在RHS部分中new一个并且使用getter和setter方法去操作其属性。
declare Address
 @author(quzishen) // 元数据，仅用于描述信息
 @createTime(2011-1-24)
 city : String @maxLengh(100)
 postno : int
end
上述的'@'是什么呢？是元数据定义，用于描述数据的数据~，没什么执行含义
你可以在RHS部分中使用Address address = new Address()的方法来定义一个对象。
更多的规则语法，可以参考其他互联网资料，推荐：
[http://wenku.baidu.com/view/a6516373f242336c1eb95e7c.html](http://wenku.baidu.com/view/a6516373f242336c1eb95e7c.html)
（写的很基础，但是部分语法写的有些简单，含糊不好理解）
## 2、Drools应用实例：
现在我们模拟一个应用场景：网站伴随业务产生而进行的积分发放操作。比如支付宝信用卡还款奖励积分等。
发放积分可能伴随不同的运营策略和季节性调整，发放数目和规则完全不同，如果使用硬编码的方式去伴随业务调整而修改，代码的修改、管理、优化、测试、上线将是一件非常麻烦的事情，所以，将发放规则部分提取出来，交给Drools管理，可以极大程度的解决这个问题。
（注意一点的是，并非所有的规则相关内容都建议使用Drools，这其中要考虑系统会运行多久，规则变更频率等一系列条件，如果你的系统只会在线上运行一周，那根本没必要选择Drools来加重你的开发成本，java硬编码的方式则将是首选）
我们定义一下发放规则：
积分的发放参考因素有：交易笔数、交易金额数目、信用卡还款次数、生日特别优惠等。
定义规则：
// 过生日，则加10分，并且将当月交易比数翻倍后再计算积分
// 2011-01-08 - 2011-08-08每月信用卡还款3次以上，每满3笔赠送30分
// 当月购物总金额100以上，每100元赠送10分
// 当月购物次数5次以上，每五次赠送50分
// 特别的，如果全部满足了要求，则额外奖励100分
// 发生退货，扣减10分
// 退货金额大于100，扣减100分
在事先分析过程中，我们需要全面的考虑对于积分所需要的因素，以此整理抽象Fact对象，通过上述的假设条件，我们假设积分计算对象如下：
[java][view
 plain](http://blog.csdn.net/quzishen/article/details/6163012#)[copy](http://blog.csdn.net/quzishen/article/details/6163012#)
- /**
-  * 积分计算对象
-  * @author quzishen
-  */
- publicclass PointDomain {  
- // 用户名
- private String userName;  
- // 是否当日生日
- privateboolean birthDay;  
- // 增加积分数目
- privatelong point;  
- // 当月购物次数
- privateint buyNums;  
- // 当月退货次数
- privateint backNums;  
- // 当月购物总金额
- privatedouble buyMoney;  
- // 当月退货总金额
- privatedouble backMondy;  
- // 当月信用卡还款次数
- privateint billThisMonth;  
- 
- /**
-      * 记录积分发送流水，防止重复发放
-      * @param userName 用户名
-      * @param type 积分发放类型
-      */
- publicvoid recordPointLog(String userName, String type){  
-         System.out.println("增加对"+userName+"的类型为"+type+"的积分操作记录.");  
-     }  
- 
- public String getUserName() {  
- return userName;  
-     }  
- // 其他getter setter方法省略
- }  
定义积分规则接口
[java][view
 plain](http://blog.csdn.net/quzishen/article/details/6163012#)[copy](http://blog.csdn.net/quzishen/article/details/6163012#)
- /**
-  * 规则接口
-  * @author quzishen
-  */
- publicinterface PointRuleEngine {  
- 
- /**
-      * 初始化规则引擎
-      */
- publicvoid initEngine();  
- 
- /**
-      * 刷新规则引擎中的规则
-      */
- publicvoid refreshEnginRule();  
- 
- /**
-      * 执行规则引擎
-      * @param pointDomain 积分Fact
-      */
- publicvoid executeRuleEngine(final PointDomain pointDomain);  
- }  
规则接口实现，Drools的API很简单，可以参考相关API文档查看具体用法：
[java][view
 plain](http://blog.csdn.net/quzishen/article/details/6163012#)[copy](http://blog.csdn.net/quzishen/article/details/6163012#)
- import java.io.File;  
- import java.io.FileNotFoundException;  
- import java.io.FileReader;  
- import java.io.IOException;  
- import java.io.Reader;  
- import java.util.ArrayList;  
- import java.util.List;  
- 
- import org.drools.RuleBase;  
- import org.drools.StatefulSession;  
- import org.drools.compiler.DroolsParserException;  
- import org.drools.compiler.PackageBuilder;  
- import org.drools.spi.Activation;  
- 
- /**
-  * 规则接口实现类
-  * @author quzishen
-  */
- publicclass PointRuleEngineImpl implements PointRuleEngine {  
- private RuleBase ruleBase;  
- 
- /* (non-Javadoc)
-      * @see com.drools.demo.point.PointRuleEngine#initEngine()
-      */
- publicvoid initEngine() {  
- // 设置时间格式
-         System.setProperty("drools.dateformat", "yyyy-MM-dd HH:mm:ss");  
-         ruleBase = RuleBaseFacatory.getRuleBase();  
- try {  
-             PackageBuilder backageBuilder = getPackageBuilderFromDrlFile();  
-             ruleBase.addPackages(backageBuilder.getPackages());  
-         } catch (DroolsParserException e) {  
-             e.printStackTrace();  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
-     }  
- 
- /* (non-Javadoc)
-      * @see com.drools.demo.point.PointRuleEngine#refreshEnginRule()
-      */
- publicvoid refreshEnginRule() {  
-         ruleBase = RuleBaseFacatory.getRuleBase();  
-         org.drools.rule.Package[] packages = ruleBase.getPackages();  
- for(org.drools.rule.Package pg : packages) {  
-             ruleBase.removePackage(pg.getName());  
-         }  
- 
-         initEngine();  
-     }  
- 
- /* (non-Javadoc)
-      * @see com.drools.demo.point.PointRuleEngine#executeRuleEngine(com.drools.demo.point.PointDomain)
-      */
- publicvoid executeRuleEngine(final PointDomain pointDomain) {  
- if(null == ruleBase.getPackages() || 0 == ruleBase.getPackages().length) {  
- return;  
-         }  
- 
-         StatefulSession statefulSession = ruleBase.newStatefulSession();  
-         statefulSession.insert(pointDomain);  
- 
- // fire
-         statefulSession.fireAllRules(new org.drools.spi.AgendaFilter() {  
- publicboolean accept(Activation activation) {  
- return !activation.getRule().getName().contains("_test");  
-             }  
-         });  
- 
-         statefulSession.dispose();  
-     }  
- 
- /**
-      * 从Drl规则文件中读取规则
-      * @return
-      * @throws Exception
-      */
- private PackageBuilder getPackageBuilderFromDrlFile() throws Exception {  
- // 获取测试脚本文件
-         List<String> drlFilePath = getTestDrlFile();  
- // 装载测试脚本文件
-         List<Reader> readers = readRuleFromDrlFile(drlFilePath);  
- 
-         PackageBuilder backageBuilder = new PackageBuilder();  
- for (Reader r : readers) {  
-             backageBuilder.addPackageFromDrl(r);  
-         }  
- 
- // 检查脚本是否有问题
- if(backageBuilder.hasErrors()) {  
- thrownew Exception(backageBuilder.getErrors().toString());  
-         }  
- 
- return backageBuilder;  
-     }  
- 
- /**
-      * @param drlFilePath 脚本文件路径
-      * @return
-      * @throws FileNotFoundException
-      */
- private List<Reader> readRuleFromDrlFile(List<String> drlFilePath) throws FileNotFoundException {  
- if (null == drlFilePath || 0 == drlFilePath.size()) {  
- returnnull;  
-         }  
- 
-         List<Reader> readers = new ArrayList<Reader>();  
- 
- for (String ruleFilePath : drlFilePath) {  
-             readers.add(new FileReader(new File(ruleFilePath)));  
-         }  
- 
- return readers;  
-     }  
- 
- /**
-      * 获取测试规则文件
-      * 
-      * @return
-      */
- private List<String> getTestDrlFile() {  
-         List<String> drlFilePath = new ArrayList<String>();  
-         drlFilePath  
-                 .add("D:/workspace2/DroolsDemo/src/com/drools/demo/point/addpoint.drl");  
-         drlFilePath  
-                 .add("D:/workspace2/DroolsDemo/src/com/drools/demo/point/subpoint.drl");  
- 
- return drlFilePath;  
-     }  
- }  
为了获取单实例的RuleBase，我们定义一个工厂类
[java][view
 plain](http://blog.csdn.net/quzishen/article/details/6163012#)[copy](http://blog.csdn.net/quzishen/article/details/6163012#)
- import org.drools.RuleBase;  
- import org.drools.RuleBaseFactory;  
- 
- /**
-  * RuleBaseFacatory 单实例RuleBase生成工具
-  * @author quzishen
-  */
- publicclass RuleBaseFacatory {  
- privatestatic RuleBase ruleBase;  
- 
- publicstatic RuleBase getRuleBase(){  
- returnnull != ruleBase ? ruleBase : RuleBaseFactory.newRuleBase();  
-     }  
- }  
剩下的就是定义两个规则文件，分别用于积分发放和积分扣减
addpoint.drl
[java][view
 plain](http://blog.csdn.net/quzishen/article/details/6163012#)[copy](http://blog.csdn.net/quzishen/article/details/6163012#)
- package com.drools.demo.point  
- 
- import com.drools.demo.point.PointDomain;  
- 
- rule birthdayPoint  
- // 过生日，则加10分，并且将当月交易比数翻倍后再计算积分
-     salience 100
-     lock-on-active true
-     when  
-         $pointDomain : PointDomain(birthDay == true)  
-     then  
-         $pointDomain.setPoint($pointDomain.getPoint()+10);  
-         $pointDomain.setBuyNums($pointDomain.getBuyNums()*2);  
-         $pointDomain.setBuyMoney($pointDomain.getBuyMoney()*2);  
-         $pointDomain.setBillThisMonth($pointDomain.getBillThisMonth()*2);  
- 
-         $pointDomain.recordPointLog($pointDomain.getUserName(),"birthdayPoint");  
- end  
- 
- rule billThisMonthPoint  
- // 2011-01-08 - 2011-08-08每月信用卡还款3次以上，每满3笔赠送30分
-     salience 99
-     lock-on-active true
-     date-effective "2011-01-08 23:59:59"
-     date-expires "2011-08-08 23:59:59"
-     when  
-         $pointDomain : PointDomain(billThisMonth >= 3)  
-     then  
-         $pointDomain.setPoint($pointDomain.getPoint()+$pointDomain.getBillThisMonth()/3*30);  
-         $pointDomain.recordPointLog($pointDomain.getUserName(),"billThisMonthPoint");  
- end  
- 
- rule buyMoneyPoint  
- // 当月购物总金额100以上，每100元赠送10分
-     salience 98
-     lock-on-active true
-     when  
-         $pointDomain : PointDomain(buyMoney >= 100)  
-     then  
-         $pointDomain.setPoint($pointDomain.getPoint()+ (int)$pointDomain.getBuyMoney()/100 * 10);  
-         $pointDomain.recordPointLog($pointDomain.getUserName(),"buyMoneyPoint");  
- end  
- 
- rule buyNumsPoint  
- // 当月购物次数5次以上，每五次赠送50分
-     salience 97
-     lock-on-active true
-     when  
-         $pointDomain : PointDomain(buyNums >= 5)  
-     then  
-         $pointDomain.setPoint($pointDomain.getPoint()+$pointDomain.getBuyNums()/5 * 50);  
-         $pointDomain.recordPointLog($pointDomain.getUserName(),"buyNumsPoint");  
- end  
- 
- rule allFitPoint  
- // 特别的，如果全部满足了要求，则额外奖励100分
-     salience 96
-     lock-on-active true
-     when  
-         $pointDomain:PointDomain(buyNums >= 5 && billThisMonth >= 3 && buyMoney >= 100)  
-     then  
-         $pointDomain.setPoint($pointDomain.getPoint()+ 100);  
-         $pointDomain.recordPointLog($pointDomain.getUserName(),"allFitPoint");  
- end  
subpoint.drl
[java][view
 plain](http://blog.csdn.net/quzishen/article/details/6163012#)[copy](http://blog.csdn.net/quzishen/article/details/6163012#)
- package com.drools.demo.point  
- 
- import com.drools.demo.point.PointDomain;  
- 
- rule subBackNumsPoint  
- // 发生退货，扣减10分
-     salience 10
-     lock-on-active true
-     when  
-         $pointDomain : PointDomain(backNums >= 1)  
-     then  
-         $pointDomain.setPoint($pointDomain.getPoint()-10);  
-         $pointDomain.recordPointLog($pointDomain.getUserName(),"subBackNumsPoint");  
- end  
- 
- rule subBackMondyPoint  
- // 退货金额大于100，扣减100分
-     salience 9
-     lock-on-active true
-     when  
-         $pointDomain : PointDomain(backMondy >= 100)  
-     then  
-         $pointDomain.setPoint($pointDomain.getPoint()-10);  
-         $pointDomain.recordPointLog($pointDomain.getUserName(),"subBackMondyPoint");  
- end  
测试方法：
[java][view
 plain](http://blog.csdn.net/quzishen/article/details/6163012#)[copy](http://blog.csdn.net/quzishen/article/details/6163012#)
- publicstaticvoid main(String[] args) throws IOException {  
-         PointRuleEngine pointRuleEngine = new PointRuleEngineImpl();  
- while(true){  
-             InputStream is = System.in;  
-             BufferedReader br = new BufferedReader(new InputStreamReader(is));  
-             String input = br.readLine();  
- 
- if(null != input && "s".equals(input)){  
-                 System.out.println("初始化规则引擎...");  
-                 pointRuleEngine.initEngine();  
-                 System.out.println("初始化规则引擎结束.");  
-             }elseif("e".equals(input)){  
- final PointDomain pointDomain = new PointDomain();  
-                 pointDomain.setUserName("hello kity");  
-                 pointDomain.setBackMondy(100d);  
-                 pointDomain.setBuyMoney(500d);  
-                 pointDomain.setBackNums(1);  
-                 pointDomain.setBuyNums(5);  
-                 pointDomain.setBillThisMonth(5);  
-                 pointDomain.setBirthDay(true);  
-                 pointDomain.setPoint(0l);  
- 
-                 pointRuleEngine.executeRuleEngine(pointDomain);  
- 
-                 System.out.println("执行完毕BillThisMonth："+pointDomain.getBillThisMonth());  
-                 System.out.println("执行完毕BuyMoney："+pointDomain.getBuyMoney());  
-                 System.out.println("执行完毕BuyNums："+pointDomain.getBuyNums());  
- 
-                 System.out.println("执行完毕规则引擎决定发送积分："+pointDomain.getPoint());  
-             } elseif("r".equals(input)){  
-                 System.out.println("刷新规则文件...");  
-                 pointRuleEngine.refreshEnginRule();  
-                 System.out.println("刷新规则文件结束.");  
-             }  
-         }  
-     }  
执行结果：
-----------------
增加对hello kity的类型为birthdayPoint的积分操作记录.
增加对hello kity的类型为billThisMonthPoint的积分操作记录.
增加对hello kity的类型为buyMoneyPoint的积分操作记录.
增加对hello kity的类型为buyNumsPoint的积分操作记录.
增加对hello kity的类型为allFitPoint的积分操作记录.
增加对hello kity的类型为subBackNumsPoint的积分操作记录.
增加对hello kity的类型为subBackMondyPoint的积分操作记录.
执行完毕BillThisMonth：10
执行完毕BuyMoney：1000.0
执行完毕BuyNums：10
执行完毕规则引擎决定发送积分：380
==========================================================
