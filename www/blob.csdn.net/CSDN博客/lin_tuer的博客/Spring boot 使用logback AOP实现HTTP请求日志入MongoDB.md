
# Spring boot 使用logback AOP实现HTTP请求日志入MongoDB - lin_tuer的博客 - CSDN博客


2017年10月29日 14:02:48[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：1322


# Spring boot 使用logback AOP实现HTTP请求日志入MongoDB
---MongoDB安装与远程访问

---[参考链接–[在ubuntu上安装mongodb]](http://blog.csdn.net/chenpy/article/details/50324989)

---安装MongoDB


---1.1 为软件包管理系统导入公钥


---Ubuntu 软件包管理工具为了保证软件包的一致性和可靠性需要用 GPG 密钥检验软件包。使用下列命令导入 MongoDB 的 GPG 密钥 （ MongoDB public GPG Key

---[http://docs.mongodb.org/10gen-gpg-key.asc](http://docs.mongodb.org/10gen-gpg-key.asc)

---）_：

---`sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv EA312927`

---1.2 为MongoDB创建列表文件


---使用下列命令创建 /etc/apt/sources.list.d/mongodb.list 列表文件

---Unbuntu 12.xx

---echo "deb http://repo.mongodb.org/apt/ubuntu precise/mongodb-org/3.2 multiverse" | sudo tee /etc/apt/sources.list.d/mongodb-org-3.2.list

---Unbuntu 14.xx

---echo "deb http://repo.mongodb.org/apt/ubuntu trusty/mongodb-org/3.2 multiverse" | sudo tee /etc/apt/sources.list.d/mongodb-org-3.2.list

---1.3 重载软件包数据库


---使用如下命名重载本地软件包数据库

---sudo apt-get update

---1.4 安装MOngoDB软件包

---安装MongoDB最新的稳定版本–(这个过程有点漫长)

---sudo apt-get install -y mongodb-org

---至此MongoDB就安装好了


---MongoDB默认的数据文件和日志文件分别存储在下面的位置 :

---数据文件：/var/lib/mongodb

---日志文件：/var/log/mongodb

---你可以修改/etc/mongod.conf 文件来改变相应的存储位置。

---1.5 检查MongoDB是否成功启动


---通过检查 /var/log/mongodb/mongod.log 文件来确定 mongod 进程是否已经成功启动。

---或者查看进程

---ps -ef | grep mongod

---MongoDB权限设置



---[参考链接–[MongoDB 用户和身份验证]](http://bubkoo.com/2014/02/07/mongodb-authentication/)

---数据库是由超级用户来创建的，一个数据库可以包含多个用户，一个用户只能在一个数据库下，不同数据库中的用户可以同名

---如果在 admin 数据库中不存在用户，即使 mongod 启动时添加了 –auth 参数，此时不进行任何认证还是可以做任何操作

---在 admin 数据库创建的用户具有超级权限，可以对 MongoDB 系统内的任何数据库的数据对象进行操作

---特定数据库比如 test1 下的用户 test_user1，不能够访问其他数据库 test2，但是可以访问本数据库下其他用户创建的数据

---不同数据库中同名的用户不能够登录其他数据库。比如数据库 test1 和 test2 都有用户 test_user，以 test_user 登录 test1 后,不能够登录到 test2 进行数据库操作

---查看 admin 数据库中的用户信息，因为是刚建立的数据库所以 user 为空


---use admin


---switched to db admin


---db.system.users.find(); \# 默认 admin 数据库中不存在用户

---创建超级权限用户


---db

---.createUser

---(

---{

---user

---:

---"Admin"

---,
    pwd:

---"admin"

---,
    roles: [ { role:

---"userAdminAnyDatabase"

---, db:

---"admin"

---} ]
  }
)

---创建某数据库的普通用户

---db

---.createUser

---(

---{

---user

---:

---"user"

---,
    pwd:

---"user"

---,
    roles: [ { role:

---"readWrite"

---, db:

---"dazhiLogs"

---}]
  }

---s

---)

---运行MongoDB


---安装好MongoDB之后，默认是开启的，但是不可以远程访问，通过如下操作使它可以远程访问

---3.1 修改绑定ip

---vi /etc/mongod.conf


---注释掉

---\#bindIp: 127.0.0.1

---3.2：重启MongoDB

---以开启认证方式启动MongoDB


---/usr/bin/mongod

----

----

---config

---/etc/mongod

---.

---conf

----

----

---auth

---&

---3.3:开启端口

---`iptables -A INPUT -p tcp -m state --state NEW -m tcp --dport 27017 -j ACCEPT `


---好啦 到了这里 关于Mongo的准备工作就足够了

---logback配置MongoDB

---在logback.xml文件中配置日志输入MongoDB


---在pom.xml中引入mongodb驱动

---<

---dependency

--->

---<

---groupId

--->

---org.mongodb

---</

---groupId

--->

---<

---artifactId

--->

---mongodb-driver

---</

---artifactId

--->

---<

---version

--->

---3.4.2

---</

---version

--->

---</

---dependency

--->

---<

---dependency

--->

---<

---groupId

--->

---org.mongodb

---</

---groupId

--->

---<

---artifactId

--->

---bson

---</

---artifactId

--->

---<

---version

--->

---3.4.2

---</

---version

--->

---</

---dependency

--->

---<appender

---name

---=

---"MyMongoDB"

---class

---=

---"ai.dazhi.util.MongoAppender"

---></appender>
<root level=

---"info"

--->
        <appender-

---ref

---ref

---=

---"console"

---/>
        <appender-

---ref

---ref

---=

---"rollingFile"

---/>
        <appender-

---ref

---ref

---=

---"MyMongoDB"

---/>
    </root>

---class=”ai.dazhi.util.MongoAppender” class中是的路径是MongoAppender的位置，我们要继承Appender基类来实现MongoAppender

---MongoAppender类内容如下：

---import ch.qos.logback.classic.spi.LoggingEvent;
import ch.qos.logback.core.UnsynchronizedAppenderBase;
import com.mongodb.*;
import com.mongodb.client.MongoCollection;
import com.mongodb.client.MongoDatabase;
import com.mongodb.util.JSON;
import org.bson.Document;

---/**
 * Created by Lintuer on 2017/10/27.
 * 实现MongoAppender
 */

---public

---class

---MongoAppender extends UnsynchronizedAppenderBase<LoggingEvent> {

---private

---MongoClient mongo;

---private

---MongoDatabase db;

---private

---MongoCollection<Document> _collection;

---//写上自己的配置

---private

---String DbHost  =

---"**.**.**.**"

---;

---private

---int

---DbPort= ****;

---private

---String DbName =

---"***"

---;

---private

---String DbCollectionName=

---"***"

---;

---private

---String user =

---"***"

---;

---private

---String pwd=

---"***"

---;

---private

---MongoCollection<BasicDBObject> logsCollection;
    @Override

---protected

---void

---append

---(LoggingEvent loggingEvent) {

---if

---(mongo ==

---null

---) {
            String connectionUrl = String.format(

---"mongodb://%s:%s@%s:%d/%s"

---, user, pwd, DbHost, DbPort,DbName);
            MongoClientURI connectionString =

---new

---MongoClientURI(connectionUrl);
            mongo =

---new

---MongoClient(connectionString);
            db = mongo.getDatabase(DbName);

---//_collection = db.getCollection(DbCollectionName);

---logsCollection = db.getCollection(DbCollectionName, BasicDBObject.class);
        }
        String jsonLog = loggingEvent.getFormattedMessage();

---//只将json格式的用户请求信息存入MongoDB，即用户请求信息--这里需要和后面的AOP配合

---if

---(jsonLog.startsWith(

---"{"

---)){
            BasicDBObject bsonLog = (BasicDBObject) JSON.parse(jsonLog);
            logsCollection.insertOne(bsonLog);
        }
    }
    @Override

---public

---void

---stop

---() {

---if

---(mongo !=

---null

---) {
            mongo.close();
        }
    }

---public

---MongoClient

---getMongo

---() {

---return

---mongo;
    }

---public

---void

---setMongo

---(MongoClient mongo) {

---this

---.mongo = mongo;
    }

---public

---MongoDatabase

---getDb

---() {

---return

---db;
    }

---public

---void

---setDb

---(MongoDatabase db) {

---this

---.db = db;
    }

---public

---MongoCollection<Document>

---get_collection

---() {

---return

---_collection;
    }

---public

---void

---set_collection

---(MongoCollection<Document> _collection) {

---this

---._collection = _collection;
    }

---public

---String

---getDbHost

---() {

---return

---DbHost;
    }

---public

---void

---setDbHost

---(String dbHost) {
        DbHost = dbHost;
    }

---public

---int

---getDbPort

---() {

---return

---DbPort;
    }

---public

---void

---setDbPort

---(

---int

---dbPort) {
        DbPort = dbPort;
    }

---public

---String

---getDbName

---() {

---return

---DbName;
    }

---public

---void

---setDbName

---(String dbName) {
        DbName = dbName;
    }

---public

---String

---getDbCollectionName

---() {

---return

---DbCollectionName;
    }

---public

---void

---setDbCollectionName

---(String dbCollectionName) {
        DbCollectionName = dbCollectionName;
    }

---public

---String

---getUser

---() {

---return

---user;
    }

---public

---void

---setUser

---(String user) {

---this

---.user = user;
    }

---public

---String

---getPwd

---() {

---return

---pwd;
    }

---public

---void

---setPwd

---(String pwd) {

---this

---.pwd = pwd;
    }
}

---AOP切面编程

---AOP为Aspect Oriented Programming的缩写，意为：面向切面编程，通过预编译方式和运行期动态代理实现程序功能的统一维护的一种技术。AOP是Spring框架中的一个重要内容，它通过对既有程序定义一个切入点，然后在其前后切入不同的执行内容，比如常见的有：打开数据库连接/关闭数据库连接、打开事务/关闭事务、记录日志等。基于AOP不会破坏原来程序逻辑，因此它可以很好的对业务逻辑的各个部分进行隔离，从而使得业务逻辑各部分之间的耦合度降低，提高程序的可重用性，同时提高了开发的效率。

---首先，在pom.xml中引入aop依赖

---<

---dependency

--->

---<

---groupId

--->

---org.springframework.boot

---</

---groupId

--->

---<

---artifactId

--->

---spring-boot-starter-aop

---</

---artifactId

--->

---</

---dependency

--->

---在application.properties中开启AOP

---\# AOP

---spring.aop.

---auto

---=

---true

---spring.aop.proxy-target-

---class

---=

---false

---实现AOP的切面主要有以下几个要素：

---使用@Aspect注解将一个java类定义为切面类

---使用@Pointcut定义一个切入点，可以是一个规则表达式，比如下例中某个package下的所有函数，也可以是一个注解等。

---根据需要在切入点不同位置的切入内容 使用@Before在切入点开始处切入内容 使用@After在切入点结尾处切入内容

---使用@AfterReturning在切入点return内容之后切入内容（可以用来对处理返回值做一些加工处理）

---使用@Around在切入点前后切入内容，并自己控制何时执行切入点自身的内容

---使用@AfterThrowing用来处理当切入内容部分抛出异常之后的处理逻辑

---编写Aspect 类，此类中，切点的定义是关键，具体规则，可以参考AOP切点匹配规则。


---import

---com

---.mongodb

---.BasicDBObject

---;

---import org

---.aspectj

---.lang

---.JoinPoint

---;

---import org

---.aspectj

---.lang

---.annotation

---.Aspect

---;

---import org

---.aspectj

---.lang

---.annotation

---.Before

---;

---import org

---.aspectj

---.lang

---.annotation

---.Pointcut

---;

---import org

---.slf

---4j

---.Logger

---;

---import org

---.slf

---4j

---.LoggerFactory

---;

---import org

---.springframework

---.core

---.annotation

---.Order

---;

---import org

---.springframework

---.stereotype

---.Component

---;

---import org

---.springframework

---.web

---.context

---.request

---.RequestContextHolder

---;

---import org

---.springframework

---.web

---.context

---.request

---.ServletRequestAttributes

---;

---import javax

---.servlet

---.http

---.HttpServletRequest

---;

---import java

---.text

---.SimpleDateFormat

---;

---import java

---.util

---.*

---;

---/**
 * Created by Lintuer on 2017/10/26.
 * AOP记录用户请求信息
 */

---@Aspect
@Order(

---1

---)
@Component
public class WebLogAspect {
    private Logger logger = LoggerFactory

---.getLogger

---(

---"MyMongoDB"

---)

---;

---//定义切入点--正则表达式
    @Pointcut(

---"execution(public * ai.dazhi.controller.*.*(..))"

---)
    public void webLog(){}
    @Before(

---"webLog()"

---)
    public void doBefore(JoinPoint joinPoint) throws Throwable {
        // 获取HttpServletRequest
        ServletRequestAttributes attributes = (ServletRequestAttributes) RequestContextHolder

---.getRequestAttributes

---()

---;

---HttpServletRequest request = attributes

---.getRequest

---()

---;

---// 获取要记录的日志内容
        BasicDBObject logInfo = getBasicDBObject(request, joinPoint)

---;

---//logger

---.info

---(

---"http-request"

---,

---"how"

---)

---;

---logger

---.info

---(logInfo

---.toJson

---())

---;

---// logger

---.info

---(logInfo)

---;

---}
    private BasicDBObject getBasicDBObject(HttpServletRequest request, JoinPoint joinPoint) {
        // 基本信息
        BasicDBObject r = new BasicDBObject()

---;

---//设置日期格式
        SimpleDateFormat df = new SimpleDateFormat(

---"yyyy-MM-dd HH:mm:ss"

---)

---;

---r

---.append

---(

---"requestTime"

---,df

---.format

---(new Date()))

---;

---r

---.append

---(

---"requestURL"

---, request

---.getRequestURL

---()

---.toString

---())

---;

---r

---.append

---(

---"requestURI"

---, request

---.getRequestURI

---())

---;

---r

---.append

---(

---"queryString"

---, request

---.getQueryString

---())

---;

---r

---.append

---(

---"remoteAddr"

---, request

---.getRemoteAddr

---())

---;

---r

---.append

---(

---"remoteHost"

---, request

---.getRemoteHost

---())

---;

---r

---.append

---(

---"remotePort"

---, request

---.getRemotePort

---())

---;

---r

---.append

---(

---"localAddr"

---, request

---.getLocalAddr

---())

---;

---r

---.append

---(

---"localName"

---, request

---.getLocalName

---())

---;

---r

---.append

---(

---"method"

---, request

---.getMethod

---())

---;

---r

---.append

---(

---"headers"

---, getHeadersInfo(request))

---;

---r

---.append

---(

---"parameters"

---, request

---.getParameterMap

---())

---;

---r

---.append

---(

---"classMethod"

---, joinPoint

---.getSignature

---()

---.getDeclaringTypeName

---() +

---"."

---+ joinPoint

---.getSignature

---()

---.getName

---())

---;

---r

---.append

---(

---"args"

---, Arrays

---.toString

---(joinPoint

---.getArgs

---()))

---;

---return r

---;

---}

---/**
     * 获取头信息
     *
     * @param request
     * @return
     */

---private Map<String, String> getHeadersInfo(HttpServletRequest request) {
        Map<String, String> map = new HashMap<>()

---;

---Enumeration headerNames = request

---.getHeaderNames

---()

---;

---while (headerNames

---.hasMoreElements

---()) {
            String key = (String) headerNames

---.nextElement

---()

---;

---String value = request

---.getHeader

---(key)

---;

---map

---.put

---(key, value)

---;

---}
        return map

---;

---}
}

---运行项目，mongo中结果如下

---{
        "

---_id

---" :

---ObjectId(

---"59f538e34c9eb26004c315a3"

---)

---,
        "

---requestTime

---" :

---"2017-10-29 10:11:47"

---,
        "

---requestURL

---" :

---"http://localhost:8090/about"

---,
        "

---requestURI

---" :

---"/about"

---,
        "

---queryString

---" :

---null

---,
        "

---remoteAddr

---" :

---"0:0:0:0:0:0:0:1"

---,
        "

---remoteHost

---" :

---"0:0:0:0:0:0:0:1"

---,
        "

---remotePort

---" :

---6998

---,
        "

---localAddr

---" :

---"0:0:0:0:0:0:0:1"

---,
        "

---localName

---" :

---"0:0:0:0:0:0:0:1"

---,
        "

---method

---" :

---"GET"

---,
        "

---headers

---" :

---{
                "

---Cookie

---" :

---"Webstorm-c51a98c9=87ab5b85-1df4-436d-9bc4-51d918923ad2; user_code=o9eK5w0Tr5NgkIMOAuAiKuv-4vKw"

---,
                "

---Accept

---" :

---"text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8"

---,
                "

---Upgrade-Insecure-Requests

---" :

---"1"

---,
                "

---Connection

---" :

---"keep-alive"

---,
                "

---User-Agent

---" :

---"Mozilla/5.0 (iPhone; CPU iPhone OS 9_1 like Mac OS X) AppleWebKit/601.1.46 (KHTML, like Gecko) Version/9.0 Mobile/13B143 Safari/601.1"

---,
                "

---Referer

---" :

---"http://localhost:8090/user/update_info_view"

---,
                "

---Host

---" :

---"localhost:8090"

---,
                "

---Accept-Encoding

---" :

---"gzip, deflate, br"

---,
                "

---Accept-Language

---" :

---"zh-CN,zh;q=0.8"

---}

---,
        "

---parameters

---" :

---{
        }

---,
        "

---classMethod

---" :

---"ai.dazhi.controller.IndexController.about"

---,
        "

---args

---" :

---"[]"

---}


