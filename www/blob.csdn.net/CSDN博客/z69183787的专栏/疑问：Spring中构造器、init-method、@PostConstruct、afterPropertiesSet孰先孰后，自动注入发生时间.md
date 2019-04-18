# 疑问：Spring中构造器、init-method、@PostConstruct、afterPropertiesSet孰先孰后，自动注入发生时间 - z69183787的专栏 - CSDN博客
2018年10月23日 15:53:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：206
[https://www.cnblogs.com/grey-wolf/p/6627925.html](https://www.cnblogs.com/grey-wolf/p/6627925.html)
问题：今天想写一个通用点的方法，根据传入的参数的类型（clazz对象），判断使用哪个mapper来插入mysql数据库。
下面是我的写法：
```
public interface BizNeeqCommonService<T> {
    void save(List<T> records, Class<T> clazz);
}
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
@Service
public class BizNeeqCommonServiceImpl<T> implements BizNeeqCommonService<T>{
    @Autowired
    private Neeq6002ItemMapper neeq6002ItemMapper;
    @Autowired
    private Neeq6004ItemMapper neeq6004ItemMapper;
    @Autowired
    private Neeq6005ItemMapper neeq6005ItemMapper;
    @Autowired
    private Neeq6006ItemMapper neeq6006ItemMapper;
    @Autowired
    private Neeq6007ItemMapper neeq6007ItemMapper;
    @Autowired
    private Neeq6008ItemMapper neeq6008ItemMapper;
    @Autowired
    private Neeq6009ItemMapper neeq6009ItemMapper;
    @Override
    public void save(List<T> records, Class<T> clazz) {
        MetaDatas metaDatas = map.get(clazz);//根据类型找到对应mapper对象
        Object mapperObject = metaDatas.mapperObject;
        Class<?> mapperObjectClass = mapperObject.getClass();
        try {
            Method method = mapperObjectClass.getDeclaredMethod("insert", clazz);
            for (T record : records) {
                method.invoke(mapperObject, record);
            }
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }
    }
    public static class MetaDatas{
        public Class  clazz; //返回的对象类型
        public Object mapperObject;//该clazz对应的mapper;反射获取clazz对象，调用insert方法
        public MetaDatas(Class clazz, Object mapperObject) {
            this.clazz = clazz;
            this.mapperObject = mapperObject;
        }
        public MetaDatas() {
        }
    }
    public static  final HashMap<Class,MetaDatas> map = new HashMap<Class,MetaDatas>();
　　
　　
　　//这一步必须要在自动注入已经发生了后执行，所以这里这么写不是很确定，到底注入发生了没
    {
        map.put(Neeq6004Item.class, new MetaDatas(Neeq6004Item.class, neeq6004ItemMapper));
        map.put(Neeq6005Item.class, new MetaDatas(Neeq6005Item.class, neeq6005ItemMapper));
        map.put(Neeq6006Item.class, new MetaDatas(Neeq6006Item.class, neeq6006ItemMapper));
        map.put(Neeq6007Item.class, new MetaDatas(Neeq6007Item.class, neeq6007ItemMapper));
        map.put(Neeq6008Item.class, new MetaDatas(Neeq6008Item.class, neeq6008ItemMapper));
        map.put(Neeq6009Item.class, new MetaDatas(Neeq6009Item.class, neeq6009ItemMapper));
    }
}
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
上面标红部分：
//这一步必须要在自动注入已经发生了后执行，所以这里这么写不是很确定，到底注入发生了没
这里这么写，感觉有些问题，所以要思考怎么写，大体知道有三种生命周期回调方法去参与到spring的生命周期，查阅了一下：
```
- 通过实现 InitializingBean/DisposableBean 接口来定制初始化之后/销毁之前的操作方法；
- 通过 <bean> 元素的 init-method/destroy-method属性指定初始化之后 /销毁之前调用的操作方法；
- 在指定方法上加上@PostConstruct 或@PreDestroy注解来制定该方法是在初始化之后还是销毁之前调用。 
```
但又有个疑问：
如果同时使用上面三种方式，会是什么结果？
实践出真知，
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
public class DemoController implements InitializingBean,DisposableBean{
    @Autowired
    private BizNeeqCommonService bizNeeqCommonService;
    public DemoController() {
        System.out.println("constructor " + bizNeeqCommonService);
    }
    @Override
    public void destroy() throws Exception {
        System.out.println("destroy interface" + bizNeeqCommonService);
    }
    @Override
    public void afterPropertiesSet() throws Exception {
        System.out.println("afterPropertiesSet interface" + bizNeeqCommonService);
    }
    @PostConstruct
    public void  postConstruct(){
        System.out.println("I'm  init  method  using  @PostConstrut...." + bizNeeqCommonService);
    }
    @PreDestroy
    public void  preDestroy(){
        System.out.println("I'm  destory method  using  @PreDestroy.....");
    }
    public void init(){
        System.out.println("init" + bizNeeqCommonService);
    }
    public void  cleanUp(){
        System.out.println("cleanUp");
    }
}
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
xml中定义：
```
<bean class="com.newthreeboard.DemoController" init-method="init"  destroy-method="cleanUp"></bean>
```
 运行结果：
constructor null
I'm init method using @PostConstrut....com.newthreeboard.Impl.BizNeeqCommonServiceImpl@5975fb
afterPropertiesSet interface com.newthreeboard.Impl.BizNeeqCommonServiceImpl@5975fb
init  com.newthreeboard.Impl.BizNeeqCommonServiceImpl@5975fb
所以顺序为：
**构造器-->自动注入-->PostConstrut-->InitializingBean-->xml中配置init方法**
关闭容器时，执行结果为：
I'm destory method using @PreDestroy.....
destroy interface   com.newthreeboard.Impl.BizNeeqCommonServiceImpl@5975fb
cleanUp
所以顺序为：
**PreDestroy--》DisposableBean-->xml中destroy-method方法**
