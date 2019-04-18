# JMeter扩展插件实现对自定义协议进行支持 - weixin_33985507的博客 - CSDN博客
2016年09月16日 21:53:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
本文版权归[xmeter.net](https://link.jianshu.com?t=http://www.xmeter.net) 所有。欢迎转载，转载请注明出处。
## 摘要##
JMeter本身提供了插件机制，允许第三方扩展JMeter以支持JMeter不支持的协议的测试。本文以扩展一个简单的Apache Kafka producer sampler为例描述了如何对JMeter进行扩展，方便地实现对新协议的支持。
## Kafka简介##
[Apache Kafka](https://link.jianshu.com?t=http://kafka.apache.org)是由Apache软件基金会开发的一个开源消息系统项目。Kafka最初是由LinkedIn开发，并于2011年初开源。2012年10月从Apache孵化器毕业。该项目的目标是为处理实时数据提供一个统一、高通量、低等待的平台。
如下图所示，Kafka的producer（数据生产者）通过Socket向Kafka集群上配置好的主题（topic）发送数据，consumer（数据消费者）在另外一端消费由生产者那边产生的数据，并进行业务的处理。Kafka作为一个优秀的消息处理系统，在集群配置，主题管理等方面有很多需要深入理解和优化等地方，本文的重点是JMeter的扩展，以Kafka的生产者为例来描述如何利用JMeter来模拟大量的生产者的调用，更多关于Kafka的细节请参考它的[官方文档](https://link.jianshu.com?t=http://kafka.apache.org/documentation.html)
![3048372-28ebdc1ed6bb5abe.png](https://upload-images.jianshu.io/upload_images/3048372-28ebdc1ed6bb5abe.png)
Kafka生产、消费者模型
## 准备工作##
扩展实现JMeter插件之前，需要先考虑清楚哪些选项需要暴露给测试人员。就像HTTP测试中，需要让用户输入服务器地址，端口号，路径等，同线程组里的连接是否共用等，如果提供是否共用连接这种选项的话，也需要在界面中体现，当然实现插件的时候处理连接的代码也会有所不同。
往Kafka上发送消息的时候，需要提供一些基本配置信息（实际Kafka的生产者配置不止这些），如果读者对下面所说的内容不了解也不要紧，只需要理解准备这些的目的是为了将这些配置信息提供给Kafka测试人员，在开始测试之前可以针对被测系统进行配置。
- 服务器所在地址，在Kafka中称之为Brokers
- 发送的主题的名字
- Message Serializer，消息通过网络发送，需要将其序列化，这里指定的是消息序列化的方式
- Key Serializer，Kafka发送消息的时候，可以指定Key，Key Serializer的主要作用是指定Key的序列化方式
- 发送的消息
下图是本文最终完成的JMeter Kafka插件的截图，测试开始之前需要输入上文所列的信息。
![3048372-a5b7956df331cb7f.png](https://upload-images.jianshu.io/upload_images/3048372-a5b7956df331cb7f.png)
Kafka JMeter插件截图
## JMeter插件实现步骤1 － 准备开发环境##
JMeter插件实现是标准的Java代码，打开Eclipse（作者使用的是4.5.2），新建一个Maven项目。我们的项目中需要引用到JMeter本身提供的一些库，包括ApacheJMeter_core和ApacheJmeter_java，并且对JMeter的版本依赖是3.0。另外，还需要对Kafka的类库也需要引入，pom.xml中依赖部分的代码如下。
```
<properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <jmeter-version>3.0</jmeter-version>
    </properties>
    <dependencies>
        <dependency>
            <groupId>org.apache.jmeter</groupId>
            <artifactId>ApacheJMeter_core</artifactId>
            <version>${jmeter-version}</version>
            <scope>provided</scope>
        </dependency>
        <dependency>
            <groupId>org.apache.jmeter</groupId>
            <artifactId>ApacheJMeter_java</artifactId>
            <version>${jmeter-version}</version>
            <scope>provided</scope>
        </dependency>
        
        <dependency>
            <groupId>org.apache.kafka</groupId>
            <artifactId>kafka_2.9.2</artifactId>
            <version>0.8.1.1</version>
            <exclusions>
                <exclusion>
                    <groupId>org.apache.zookeeper</groupId>
                    <artifactId>zookeeper</artifactId>
                </exclusion>
            </exclusions>
        </dependency>
    </dependencies>
```
工程创建完毕之后，开始编写代码来实现插件。
## JMeter插件实现步骤2 － 自定义界面##
JMeter的插件机制会在$JMETER_HOME/lib/ext目录下去动态加载符合指定条件的JAR包，并在JMeter中显示出来。比如要扩展UI的话，扩展的Java类的包名必须是”*.gui.*”，同样的扩展函数的Java类的包名必须是”*.function.*”.
新建一个类，org.apache.jmeter.protocol.kafka.control.gui.KafkaSamplerUI，并指定其父类为AbstractSamplerGui。该类需要实现以下的功能：
1）界面布局与控件。JMeter的界面是标准的Swing，所以里面的控件和布局都是标准的Swing写法。
2）界面与Sampler之间的数据交换。Sampler在JMeter中继承的是TestElement，用户输入的数据都是保存在Sampler中的，因此可以认为这个是界面的模型。界面与模型（Sampler）之间的数据交换需要实现父类的下面几个方法，
```
public void configure(TestElement el)
```
该方法用于把Sampler中的数据加载到界面中。在实现自己的逻辑之前，先调用一下父类的方法super.configure(el)，这样可以确保框架自动为你加载一些缺省数据，比如Sampler的名字。
```
public void modifyTestElement(TestElement e)
```
这个方法用于把界面的数据移到Sampler中，刚好与上面的方法相反。在调用自己的实现方法之前，请先调用一下super.configureTestElement(e)，这个会帮助移到一些缺省的数据。
```
public TestElement createTestElement()
```
该方法创建一个新的Sampler，然后将界面中的数据设置到这个新的Sampler实例中。
```
public void clearGui()
```
该方法会在reset新界面的时候调用，这里可以填入界面控件中需要显示的一些缺省的值。
在本文的实现中，分别实现上面的那些方法，包括显示5个控件（4个控制参数的控件在一个panel里，发送的消息的控件在另外一个panel里），以及另外的几个方法用于和Sampler之间的数据交换等。完整的代码请参见[这里](https://link.jianshu.com?t=https://github.com/XMeterSaaSService/kafka_jmeter)。
## JMeter插件实现步骤3 － 自定义Kafka Sampler##
新建一个Sampler，org.apache.jmeter.protocol.kafka.sampler.KafkaSampler，并指定其父类为AbstractSampler。该类需要实现以下的功能：
1）增加一些getter/setter方法，这些方法用于与UI之间的数据交换，这些数据在用户保存／打开脚本的时候将被自动序列化／反序列化。
2）实现sample方法:
```
public SampleResult sample(Entry entry)
```
该方法是JMeter实现对目标系统发起请求实际工作的地方。主要的工作是记录请求处理时间，对返回结果进行处理和判断，并根据处理结果返回SampleResult，该SampleResult中需要指定返回的内容是否成功，以及消息等。该方法的基本实现框架如下所示：
```
public SampleResult sample(Entry entry) {
        SampleResult result = new SampleResult();
        result.setSampleLabel(getName());
        try {
            result.sampleStart();
            //对目标系统发出测试请求
            result.sampleEnd();
            result.setSuccessful(true);
            result.setResponseCodeOK();
        } catch (Exception e) {
            result.sampleEnd(); // stop stopwatch
            result.setSuccessful(false);
            result.setResponseMessage("Exception: " + e);
            // get stack trace as a String to return as document data
            java.io.StringWriter stringWriter = new java.io.StringWriter();
            e.printStackTrace(new java.io.PrintWriter(stringWriter));
            result.setResponseData(stringWriter.toString(), null);
            result.setDataType(org.apache.jmeter.samplers.SampleResult.TEXT);
            result.setResponseCode("FAILED");
        }
        return result;
    }
```
本文的实现中，生成一个Kafka的Producer对象，并将用户界面里指定的信息发送到Kafka服务器上。完整的代码请参见[这里](https://link.jianshu.com?t=https://github.com/XMeterSaaSService/kafka_jmeter)。
## JMeter插件实现步骤4 － 打包、部署##
打包过程与普通的Maven项目相似，但是需要注意的是本插件需要把Kafka相关的依赖库文件也一并打入，否则还需要单独准备所依赖的JAR包。因此在pom.xml中加入下面的build插件。右击工程 》Run As 》Maven install，运行成功后，在工程的target目录下会生成一个kafka_jmeter-jar-with-dependencies.jar，这个就是我们打好的插件。
```
<build>
        <finalName>${project.artifactId}</finalName>
        <defaultGoal>install</defaultGoal>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <version>2.5.1</version>
                <configuration>
                    <source>1.7</source>
                    <target>1.7</target>
                </configuration>
            </plugin>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-assembly-plugin</artifactId>
                <configuration>
                    <descriptorRefs>
                        <descriptorRef>jar-with-dependencies</descriptorRef>
                    </descriptorRefs>
                </configuration>
                <executions>
                    <execution>
                        <id>assemble-all</id>
                        <phase>package</phase>
                        <goals>
                            <goal>single</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>
        </plugins>
    </build>
```
插件开发完成后，将kafka_jmeter-jar-with-dependencies.jar拷贝到$JMETER_HOME/lib/ext目录下，如果之前已经打开了JMeter，则需要将JMeter关闭重启。
## JMeter插件实现步骤5 － 测试插件##
新建一个测试脚本，加入一个线程组，然后添加一个Sampler，如果运行正常，在子菜单中应该能看到我们扩展出来的“Kafka Sampler”。
![3048372-4c715b7ca3d56596.png](https://upload-images.jianshu.io/upload_images/3048372-4c715b7ca3d56596.png)
开发好的插件
修改一下线程组里的线程数，则能实现多虚拟用户的并发测试了。下图是“查看结果树”里面显示的内容。
![3048372-46283535a6b12b2d.png](https://upload-images.jianshu.io/upload_images/3048372-46283535a6b12b2d.png)
查看结果树中的内容
下图是Kafka的消费者端，能看到相关的消息已经发送成功。
![3048372-9b105f8fc31d072b.png](https://upload-images.jianshu.io/upload_images/3048372-9b105f8fc31d072b.png)
服务器端控制台输出
## 更多扩展方式##
如本文所示，如果通过比较“标准”的方式来扩展JMeter对新协议的测试，还是有一定的工作量，特别是如果界面功能比较丰富的话，实现界面的那个类会比较复杂。如果对界面的要求不是很高，那么还有一种比较简单的可选项是通过扩展AbstractJavaSamplerClient，通过扩展该类，可以实现对简单的测试配置属性进行设置，并在runTest方法的实现中对目标系统发出请求
```
public SampleResult runTest(JavaSamplerContext context)
```
该方法的返回值是SampleResult，就是验证测试结果，与上文中实现步骤3比较类似。读者可以参见本文的参考资料中关于AbstractJavaSamplerClient中列出的几篇文章。
## 大规模自定义协议测试 － XMeter帮您提高效率，降低成本##
如果用户自定义的协议的测试，单台机器能够模拟的客户端是有限的，如果针对大规模的协议测试（比如本文举例的Kafka），可能需要准备大量的测试机器对被测系统进行打压，这个过程需要大量的准备机器环境，以及管理这些环境的工作，导致工作效率比较低、而且容易出错。XMeter将这些过程迁移至云端，弹性伸缩管理这些压力机，用户只要关注测试脚本的实现过程，而无需关心这些测试环境的管理，从而大大降低了大规模性能测试的复杂度。另外XMeter提供了丰富的测试报告，让性能问题一目了然。
## 参考资料##
[JMeter扩展官方文档](https://link.jianshu.com?t=http://jmeter.apache.org/extending/jmeter_tutorial.pdf)
[AbstractJavaSamplerClient的方式扩展JMeter文章1](https://link.jianshu.com?t=http://codyaray.com/2014/07/custom-jmeter-samplers-and-config-elements)
[AbstractJavaSamplerClient的方式扩展JMeter文章2](https://link.jianshu.com?t=http://ilkinbalkanay.blogspot.com/2010/03/load-test-whatever-you-want-with-apache.html)
[AbstractJavaSamplerClient的方式扩展JMeter文章3](https://link.jianshu.com?t=http://edwin.baculsoft.com/2011/09/how-to-create-a-java-class-performance-test-using-jmeters-abstractjavasamplerclient/)
## 关于我们##
XMeter成立于2016年，核心团队都来自于IBM，是一家领先技术的性能测试持续集成咨询与服务提供商。我们致力于提供给客户可靠,简单,低成本的性能测试解决方案。
