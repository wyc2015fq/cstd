# java的(PO,VO,TO,BO,DAO,POJO)解释 - weixin_33985507的博客 - CSDN博客
2013年03月19日 10:11:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
PO(persistant object) 持久对象 
在o/r映射的时候出现的概念，如果没有o/r映射，没有这个概念存在了。通常对应数据模型(数据库),本身还有部分业务逻辑的处理。可以看成是与数据库中的表相映射的java对象。最简单的PO就是对应数据库中某个表中的一条记录，多个记录可以用PO的集合。PO中应该不包含任何对数据库的操作。 
VO(value object) 值对象 
通常用于业务层之间的数据传递，和PO一样也是仅仅包含数据而已。但应是抽象出的业务对象,可以和表对应,也可以不,这根据业务的需要.个人觉得同DTO(数据传输对象),在web上传递。 
TO(Transfer Object)，数据传输对象
在应用程序不同tie(关系)之间传输的对象 
BO(business object) 业务对象 
从业务模型的角度看,见UML元件领域模型中的领域对象。封装业务逻辑的java对象,通过调用DAO方法,结合PO,VO进行业务操作。 
POJO(plain ordinary java object) 简单无规则java对象
纯的传统意义的java对象。就是说在一些Object/Relation Mapping工具中，能够做到维护数据库表记录的persisent object完全是一个符合Java Bean规范的纯Java对象，没有增加别的属性和方法。我的理解就是最基本的Java Bean，只有属性字段及setter和getter方法！。 
DAO(data access object) 数据访问对象 
是一个sun的一个标准j2ee设计模式，这个模式中有个接口就是DAO，它负持久层的操作。为业务层提供接口。此对象用于访问数据库。通常和PO结合使用，DAO中包含了各种数据库的操作方法。通过它的方法,结合PO对数据库进行相关的操作。夹在业务逻辑与数据库资源中间。配合VO, 提供数据库的CRUD操作... 
O/R Mapper 对象/关系 映射   
定义好所有的mapping之后，这个O/R Mapper可以帮我们做很多的工作。通过这些mappings,这个O/R Mapper可以生成所有的关于对象保存，删除，读取的SQL语句，我们不再需要写那么多行的DAL代码了。 
实体Model(实体模式) 
DAL(数据访问层) 
IDAL(接口层) 
DALFactory(类工厂) 
BLL(业务逻辑层) 
BOF     Business Object Framework       业务对象框架 
SOA     Service Orient Architecture     面向服务的设计 
EMF     Eclipse Model Framework         Eclipse建模框架
----------------------------------------
PO：全称是
persistant object持久对象
最形象的理解就是一个PO就是数据库中的一条记录。
好处是可以把一条记录作为一个对象处理，可以方便的转为其它对象。
BO：全称是
business object:业务对象
主要作用是把业务逻辑封装为一个对象。这个对象可以包括一个或多个其它的对象。
比如一个简历，有教育经历、工作经历、社会关系等等。
我们可以把教育经历对应一个PO，工作经历对应一个PO，社会关系对应一个PO。
建立一个对应简历的BO对象处理简历，每个BO包含这些PO。
这样处理业务逻辑时，我们就可以针对BO去处理。
VO ：
value object值对象
ViewObject表现层对象
主要对应界面显示的数据对象。对于一个WEB页面，或者SWT、SWING的一个界面，用一个VO对象对应整个界面的值。
DTO ：
Data Transfer Object数据传输对象
主要用于远程调用等需要大量传输对象的地方。
比如我们一张表有100个字段，那么对应的PO就有100个属性。
但是我们界面上只要显示10个字段，
客户端用WEB service来获取数据，没有必要把整个PO对象传递到客户端，
这时我们就可以用只有这10个属性的DTO来传递结果到客户端，这样也不会暴露服务端表结构.到达客户端以后，如果用这个对象来对应界面显示，那此时它的身份就转为VO
POJO ：
plain ordinary java object 简单java对象
个人感觉POJO是最常见最多变的对象，是一个中间对象，也是我们最常打交道的对象。
一个POJO持久化以后就是PO
直接用它传递、传递过程中就是DTO
直接用来对应表示层就是VO
DAO：
data access object数据访问对象
这个大家最熟悉，和上面几个O区别最大，基本没有互相转化的可能性和必要.
主要用来封装对数据库的访问。通过它可以把POJO持久化为PO，用PO组装出来VO、DTO
-----------------------------------------------------------------
PO:persistant object持久对象,可以看成是与数据库中的表相映射的java对象。最简单的PO就是对应数据库中某个表中的一条记录，多个记录可以用PO的集合。PO中应该不包含任何对数据库的操作.                                                                                        
VO:value object值对象。通常用于业务层之间的数据传递，和PO一样也是仅仅包含数据而已。但应是抽象出的业务对象,可以和表对应,也可以不,这根据业务的需要.个人觉得同DTO(数据传输对象),在web上传递. 
DAO:data access object数据访问对象，此对象用于访问数据库。通常和PO结合使用，DAO中包含了各种数据库的操作方法。通过它的方法,结合PO对数据库进行相关的操作. 
BO:business object业务对象,封装业务逻辑的java对象,通过调用DAO方法,结合PO,VO进行业务操作; 
POJO:plain ordinary java object 简单无规则java对象,我个人觉得它和其他不是一个层面上的东西,VO和PO应该都属于它.
---------------------------------------------
VO：值对象、视图对象
PO：持久对象
QO：查询对象
DAO：数据访问对象
DTO：数据传输对象
----------------------------------------
struts 里的 ActionForm 就是个VO;
hibernate里的 实体bean就是个PO,也叫POJO;
hibernate里的Criteria 就相当于一个QO;
在使用hibernate的时候我们会定义一些查询的方法,这些方法写在接口里,可以有不同的实现类.而这个接口就可以说是个DAO.
个人认为QO和DTO差不多.
----------------------------------------
PO或叫BO，与数据库最接近的一层，是ORM中的O，基本上是数据库字段对应BO中的一个属性，为了同步与安全性考虑，最好只给DAO或者Service调用，而不要用packcode,backingBean,或者BO调。
DAO，数据访问层，把VO，backingBean中的对象可以放入。。。。
DTO，很少用，基本放入到DAO中，只是起到过渡的作用。
QO，是把一些与持久性查询操作与语句放入。。
VO，V层中用到的基本元素与方法等放其中。如果要其调用BO，则要做BO转换VO，VO转换BO操作。VO的好处是其页面的元素属性多于BO，可起到很好的作用。。。。
-----------------------------------------
楼上的不对吧，PO是持久化对象。BO＝business object—业务对象。
PO可以严格对应数据库表，一张表对映一个PO。
BO则是业务逻辑处理对象，我的理解是它装满了业务逻辑的处理，在业务逻辑复杂的应用中有用。
VO：value object值对象、view object视图对象
PO：持久对象
QO：查询对象
DAO：数据访问对象——同时还有DAO模式
DTO：数据传输对象——同时还有DTO模式
