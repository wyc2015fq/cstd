# 高仿IoC第一课：对Spring第二课效果的实现(上) - PeterBishop - CSDN博客





2019年01月31日 21:46:26[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：28








  这课是高仿IoC第一课，我的课程安排是这样的，我们讲一点ioc的知识，就用代码实现一点，当然我写的肯定不是Spring 的源码，只是一个高仿的IoC而已。



  下面就开始写吧。


- 新建工程，取名为IoCByXXX
- 引入xml解析需要的jar包，这个我准备好了，等下上传。
- 新建一个ApplicationContext接口，然后把昨天讲的那几个方法声明一下。
- 新建一个类取名叫ClassPathXmlApplicationContext,实现ApplicationContext接口
- 根据Spring第二课讲的，ClassPathXmlApplicationContext应该有一个构造器，参数为String类型
- 分析下Spring第二课所讲，其实可以感觉到容器底层个用的应该是一个Map，键为xml里配置的bean标签的id属性，值为bean对象，类型为class属性
- 上面各部完成后空壳子算是造出来了，下面开始实现里面的功能
- 首先，根据Spring官方文档的说法，bean是以BeanDefinition为单位存储的
- 新建一个类叫BeanDefinition，用于存储bean的各种信息
public class BeanDefinition {



    */**     * **在xml中配置的信息目前有两个，id，class     */    *String name;    //这个相当于xml里的id属性

    Class beanClass;//bean的Class对象，类名为xml里的class属性

    Object bean;    //要获取的bean对象



    public BeanDefinition(String name, Class beanClass, Object bean) {

        this.name = name;

        this.beanClass = beanClass;

        this.bean = bean;

    }



    //别忘了Spring IoC中允许id不填

    public BeanDefinition(Class beanClass, Object bean) {

        this.beanClass = beanClass;

        this.bean = bean;

    }



    public String getName() {

        return name;

    }



    public void setName(String name) {

        this.name = name;

    }



    public Class getBeanClass() {

        return beanClass;

    }



    public void setBeanClass(Class beanClass) {

        this.beanClass = beanClass;

    }



    public Object getBean() {

        return bean;

    }



    public void setBean(Object bean) {

        this.bean = bean;

    }

}- 新建一个类，用于存放BeanDefinition,取名为BeanDefinitionRegistry
//BeanDefinition的注册器，同时也是存放BeanDefinition的容器

public class BeanDefinitionRegistry {



    private Map<String,BeanDefinition> beanDefinitionContainer = new HashMap<>();



    public void regist(String name, BeanDefinition beanDefinition)

    {

        beanDefinitionContainer.put(name,beanDefinition);

    }



    public BeanDefinition get(String name)

    {

        return beanDefinitionContainer.get(name);

    }



    public <T> BeanDefinition get(Class<T> beanClass)

    {

        Collection<BeanDefinition> values = beanDefinitionContainer.values();

        for (BeanDefinition value : values)

        {

            if (value.beanClass == beanClass)

            {

                return value;

            }

        }

        return null;

    }



    public <T> BeanDefinition get(String name, Class<T> beanClass)

    {

        BeanDefinition beanDefinition = get(name);

        if (beanDefinition.beanClass == beanClass)

        {

            return beanDefinition;

        }

        return null;

    }

}- 完善ClassPathXmlApplicationContext
public class ClassPathXmlApplicationContext implements ApplicationContext {



    BeanDefinitionRegistry registry = new BeanDefinitionRegistry();



    public ClassPathXmlApplicationContext(String filepath)

    {



    }



    @Override

    public Object getBean(String name) {

        return registry.get(name).getBean();

    }



    @Override

    public <T> T getBean(Class<T> requiredType) {

        return (T) registry.get(requiredType).getBean();

    }



    @Override

    public <T> T getBean(String name, Class<T> requiredType) {

        return (T) registry.get(name,requiredType).getBean();

    }

}- 最后，解决ClassPathXmlApplicationContext的构造器，这个特别麻烦，下节课讲。



