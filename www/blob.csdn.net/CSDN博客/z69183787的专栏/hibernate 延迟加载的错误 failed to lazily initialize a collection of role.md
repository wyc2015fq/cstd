# hibernate 延迟加载的错误 failed to lazily initialize a collection of role - z69183787的专栏 - CSDN博客
2014年02月27日 15:28:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2318
这个问题一般出现在一对多的情况下，解决的方法有两种
1、设置lazy=false
如果是用annotation，则配置如下
@OneToMany(
   targetEntity = CourseAuthorizationItem.class,
   cascade = {CascadeType.PERSIST, CascadeType.MERGE},
   mappedBy = "course", fetch=FetchType.EAGER
  )
将fetch类型设置成直接获取
2、就是使用filter，过滤所有的链接
如果在使用filter的时候，要配置事务处理，否则会导致session处于只读状态而不能做修改、删除的动作
<web-app>
<filter>
<filter-name>hibernateFilter</filter-name>
<filter-class>
org.springframework.orm.hibernate.support.OpenSessionInViewFilter
</filter-class>
</filter>
<filter-mapping>
<filter-name>hibernateFilter</filter-name>
<url-pattern>*.do</url-pattern>
</filter-mapping>
</web-app>

failed to lazily initialize a collection of role: XXXXXX, no session or session was closed
例如：failed to lazily initialize a collection of role: com.gtc.wzgl.model.User.roles, no session or session was closed
这个异常大致意思是说在多对一的时候（并且lazy="false"），对象的实例失败，多数出现的情况有
1、粗心造成
实例对象类名写错之类的
2、逻辑错误
如之前就已经传递过来一个实体对象，然后调用实体对象的方法时牵涉到1对多的情况，但此时SESSION已经关闭，所以根本无法进行一对多的操作。
3、设计到跨度的问题：
这样打比方有多个实体对象，他们直接或则间接的有关联。比如有4个实体，分别是广告信息、广告、广告问答题、广告商：他们之间的关系为：
广告商 1:n 广告
广告 1:n 广告问答题
广告商 1:n 广告商信息
大家可以看到广告和广告商信息是没有直接关系的。但我要添加广告的时候我就必须将广告商的实体做为条件。那么这么一来广告商信息可能间接的就必须用上。下面看我的操作：
ad(广告)，subject(题目)
     Ad ad = new Ad();
     ad.setAdProd(adform.getAdProd());
     ad.setIndustry(industry);
     ad.setAdPicture(pagefile.getFileName());
     ad.setAdFlack(adform.getAdFlack());
     ad.setAdDv(dvfile.getFileName());
     ad.setAdContent(adform.getAdContent());
     ad.setGray(gray);
     ad.setAdDate(new Date());
     ad.setOnlinetime(new Long(0));
     //以上为广告的基本信息填写，而重要的是看下面一句，在这里我的思路是subjectFormList是一个动态提交的表单，里面有若干个广告问答题。我将这些问答题变为一个Set，然后作为ad的一个属性。
     Set<Subject> subjectset=getSubjectSet(subjectFormList,ad);
     ad.setSubjects(subjectset);
//然后提交，makePersistent是一个封装的方法，用途就是save()啦。addao是一个DAO，里面有ADUS。
addao.makePersistent(ad);
表面上看来很符合逻辑，只要我们在ad的映射里面加上对subject的级联更新就可以完成这项操作。但实际上会发生我们意想不到的问题，来让我们看一下getSubjectSet（）的内容：
public Set getSubjectSet(List<SubjectForm> subjectlist,Ad ad)
{
   Set<Subject> set=new HashSet<Subject>(0);
   Subject subject;
   for(Iterator<SubjectForm> it=subjectlist.iterator();it.hasNext();)
   {
    subject=new Subject();
    SubjectForm sf=it.next();
    subject.setSuContent(sf.getSucontent());
    subject.setSuOption(sf.getSuoption());
    subject.setSuResult(Arrays.deepToString(sf.getSuresult()));
    subject.setSuType(String.valueOf(sf.getSutype()));
    subject.setAd(ad);
    set.add(subject);
   }
   return set;
}
我们在这个方法上设一个断点然后跟踪，之后你会发现断点在set.add(subject)只后就会出failed to lazily initialize a collection of role: XXXXXXXX no session or session was closed这个异常，并且这个异常还是出在了广告商的广告信息上 gray.messages。是不是很不可理解？这也是Hibernate的懒汉机制问题。没有任何一样技术是完美的。那我们该怎么处理这样的问题。有很多人以为我们在广告商对广告商信息的隐射上加lazy="false"这样在对gray操作会对messages进行关联，并查询时提出数据。但你会发现改完之后会出现org.hibernate.LazyInitializationException:
 illegal access to loading collection这个异常。并切lazy="false"是我们不推荐的一种方法。他会降低你的查询效率。
对于这样的情况最好的解决办法就是不要偷懒，对一个实体进行操作的时候就该用那个实体的DAO，即应该有2句HQL。如下把getSubjectSet（）改一改：
public void getSubjectSet(List<SubjectForm> subjectlist,Ad ad)
{
   Set<Subject> set=new HashSet<Subject>(0);
   SubjectDAO subjectdao=DAOFactory.getDao(SubjectDAO.class);
   for(Iterator<SubjectForm> it=subjectlist.iterator();it.hasNext();)
   {
    Subject subject=new Subject();
    SubjectForm sf=it.next();
    subject.setSuContent(sf.getSucontent());
    subject.setSuOption(sf.getSuoption());
    subject.setSuResult(Arrays.deepToString(sf.getSuresult()));
    subject.setSuType(String.valueOf(sf.getSutype()));
    subject.setAd(ad);
    subjectdao.makePersistent(subject);
    //set.add(subject);
   }
}//遍历出所有subject一个个的往数据库里加。这样便不会出问题了。
1、OpenSessionInView模式：
以下有2种方法，第1种是结合SPRING，第2种是采用了拦截器
Spring+Hibernate中，     集合映射如果使用lazy="true"， 当PO传到View层时， 出现未初始化session已关闭的错误，只能在dao先初始化
parent.getChilds().size();
Spring提供Open Session In View来解决这个问题, 有两种方式
1. Interceptor
<!--</span><span style="COLOR: rgb(0,128,0)"> =========== OpenSession In View pattern ==============</span><span style="COLOR: rgb(0,128,0)">-->
<bean id="openSessionInViewInterceptor"
             class="org.springframework.orm.hibernate3.support.OpenSessionInViewInterceptor">
<property name="sessionFactory" ref="sessionFactory"/>
</bean>
<bean id="urlMapping"class="org.springframework.web.servlet.handler.SimpleUrlHandlerMapping">
<property name="interceptors" ref="openSessionInViewInterceptor"/>
<property name="mappings">
<props>
               ......
</props>
</property>
</bean>
2. Filter
<web-app>
<filter>
<filter-name>hibernateFilter</filter-name>
<filter-class>
org.springframework.orm.hibernate.support.OpenSessionInViewFilter
</filter-class>
</filter>
<filter-mapping>
<filter-name>hibernateFilter</filter-name>
<url-pattern>*.do</url-pattern>
</filter-mapping>
</web-app>
第2种解决方法：
Hibernate.initialize()强制加载关联对象
今天又碰到错误
failed to lazily initialize a collection of role: no session or session was closed
试验了一下发现了几个解决方法：
1、是把对应一对多的那两个列lazy=true改为lazy＝false即可
2、对于查询中如果用的是xxx.load（class，id）则改为xxx,get(class，id)
3在web.xml文件中加入
<filter>
   <filter-name>hibernateFilter</filter-name>
   <filter-class>org.springframework.orm.hibernate3.support.OpenSessionInViewFilter</filter-class>
   <init-param>
            <param-name>singleSession</param-name>
            <param-value>false</param-value>
        </init-param>
<!--这个--   <init-param>一定要加不然很可能会报错：org.springframework.dao.InvalidDataAccessApiUsageException: Write operations are not allowed in read-only mode (FlushMode.NEVER) -
 turn your Session into FlushMode.AUTO or remove 'readOnly' marker from transaction definition
>
</filter>
<filter-mapping>
   <filter-name>hibernateFilter</filter-name>
   <url-pattern>*.mmg</url-pattern>
</filter-mapping>
