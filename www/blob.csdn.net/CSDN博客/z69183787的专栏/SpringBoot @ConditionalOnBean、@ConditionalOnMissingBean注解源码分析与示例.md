# SpringBoot @ConditionalOnBean、@ConditionalOnMissingBean注解源码分析与示例 - z69183787的专栏 - CSDN博客
2019年01月28日 16:08:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：342
个人分类：[SpringBoot-条件注解](https://blog.csdn.net/z69183787/article/category/8646938)
[https://blog.csdn.net/xcy1193068639/article/details/81517456](https://blog.csdn.net/xcy1193068639/article/details/81517456)
前言：
Spring4推出了@Conditional注解，方便程序根据当前环境或者容器情况来动态注入bean，对@Conditional注解不熟悉的朋友可移步至 Spring @Conditional注解 详细讲解及示例 这篇博客进行学习。
继@Conditional注解后，又基于此注解推出了很多派生注解，比如@ConditionalOnBean、@ConditionalOnMissingBean、@ConditionalOnExpression、@ConditionalOnClass......动态注入bean变得更方便了。本篇将讲解@ConditionalOnBean注解。
配置类中有两个Computer类的bean，一个是笔记本电脑，一个是备用电脑。如果当前容器中已经有电脑bean了，就不注入备用电脑，如果没有，则注入备用电脑，这里需要使用到@ConditionalOnMissingBean。
```java
@Configuration
public class BeanConfig {
 
    @Bean(name = "notebookPC")
    public Computer computer1(){
        return new Computer("笔记本电脑");
    }
 
    @ConditionalOnMissingBean(Computer.class)
    @Bean("reservePC")
    public Computer computer2(){
        return new Computer("备用电脑");
    }
}
```
这个注解就实现了功能，这个@ConditionalOnMissingBean为我们做了什么呢？我们来一探究竟.。
一探究竟：
首先，来看@ConditionalOnMissingBean的声明：
```java
//可以标注在类和方法上
@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
//使用了@Conditional注解，条件类是OnBeanCondition
@Conditional({OnBeanCondition.class})
public @interface ConditionalOnMissingBean {
    Class<?>[] value() default {};
 
    String[] type() default {};
 
    Class<?>[] ignored() default {};
 
    String[] ignoredType() default {};
 
    Class<? extends Annotation>[] annotation() default {};
 
    String[] name() default {};
 
    SearchStrategy search() default SearchStrategy.ALL;
}
```
这时候，我们就看到了我们熟悉的@Conditional注解，OnBeanCondition作为条件类。
OnBeanCondition类的声明：
//定义带注释的组件的排序顺序，2147483647即为默认值
```java
@Order(2147483647)
class OnBeanCondition extends SpringBootCondition implements ConfigurationCondition {
```
它继承了SpringBootCondition类，OnBeanCondition类中没有matches方法，而SpringBootCondition类中有实现matches方法。OnBeanCondition还实现了ConfigurationCondition，ConfigurationCondition接口不熟悉的读者可以到Spring ConfigurationCondition接口详解 了解接口。OnBeanCondition类重写了getConfigurationPhase()方法，表示在注册bean的时候注解生效：
```java
public ConfigurationPhase getConfigurationPhase() {
        return ConfigurationPhase.REGISTER_BEAN;
    }
```
就从matches方法开始：
```java
//SpringBootCondition类中的matches方法
    public final boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
        //获取当前的类名或者方法名（由标注的位置决定）
        String classOrMethodName = getClassOrMethodName(metadata);
 
        try {
            //关键代码：这里就会判断出结果
            ConditionOutcome outcome = this.getMatchOutcome(context, metadata);
            //存入日志
            this.logOutcome(classOrMethodName, outcome);
            //存入记录
            this.recordEvaluation(context, classOrMethodName, outcome);
            //最后返回ConditionOutcome的isMatch就是返回boolean类型结果
            return outcome.isMatch();
        } catch (NoClassDefFoundError var5) {
            throw new IllegalStateException("Could not evaluate condition on " + classOrMethodName + " due to " + var5.getMessage() + " not found. Make sure your own configuration does not rely on that class. This can also happen if you are @ComponentScanning a springframework package (e.g. if you put a @ComponentScan in the default package by mistake)", var5);
        } catch (RuntimeException var6) {
            throw new IllegalStateException("Error processing condition on " + this.getName(metadata), var6);
        }
    }
```
关键代码在OnBeanCondition的getMatchOutcome方法上：
```java
/**
     * 获得判断结果的方法，ConditionOutcome类中存着boolean类型的结果
     */
    public ConditionOutcome getMatchOutcome(ConditionContext context, AnnotatedTypeMetadata metadata) {
    //返回一个新的ConditionMessage
        ConditionMessage matchMessage = ConditionMessage.empty();
        OnBeanCondition.BeanSearchSpec spec;
        List matching;
        //这是metadata会调用isAnnotated方法判断当前标注的注解是不是ConditionalOnMissingBean
    //其实@ConditionalOnBean、@ConditionalOnMissingBean和@ConditionalOnSingleCandidate都是使用这个条件类，所以这里做判断
        if (metadata.isAnnotated(ConditionalOnBean.class.getName())) {
            spec = new OnBeanCondition.BeanSearchSpec(context, metadata, ConditionalOnBean.class);
            matching = this.getMatchingBeans(context, spec);
            if (matching.isEmpty()) {
                return ConditionOutcome.noMatch(ConditionMessage.forCondition(ConditionalOnBean.class, new Object[]{spec}).didNotFind("any beans").atAll());
            }
 
            matchMessage = matchMessage.andCondition(ConditionalOnBean.class, new Object[]{spec}).found("bean", "beans").items(Style.QUOTE, matching);
        }
 
        if (metadata.isAnnotated(ConditionalOnSingleCandidate.class.getName())) {
            OnBeanCondition.BeanSearchSpec spec = new OnBeanCondition.SingleCandidateBeanSearchSpec(context, metadata, ConditionalOnSingleCandidate.class);
            matching = this.getMatchingBeans(context, spec);
            if (matching.isEmpty()) {
                return ConditionOutcome.noMatch(ConditionMessage.forCondition(ConditionalOnSingleCandidate.class, new Object[]{spec}).didNotFind("any beans").atAll());
            }
 
            if (!this.hasSingleAutowireCandidate(context.getBeanFactory(), matching, spec.getStrategy() == SearchStrategy.ALL)) {
                return ConditionOutcome.noMatch(ConditionMessage.forCondition(ConditionalOnSingleCandidate.class, new Object[]{spec}).didNotFind("a primary bean from beans").items(Style.QUOTE, matching));
            }
 
            matchMessage = matchMessage.andCondition(ConditionalOnSingleCandidate.class, new Object[]{spec}).found("a primary bean from beans").items(Style.QUOTE, matching);
        }
 
        //如果当前注入的bean是@ConditionalOnMissingBean
        if (metadata.isAnnotated(ConditionalOnMissingBean.class.getName())) {
            //返回一个spec（说明），这里的spec规定了搜索的内容，比如搜索策略、需要搜索的类名......
            spec = new OnBeanCondition.BeanSearchSpec(context, metadata, ConditionalOnMissingBean.class);
            //主要的搜索实现在这个方法里，最后返回一个list
            matching = this.getMatchingBeans(context, spec);
            //判断搜索出来的结果
            if (!matching.isEmpty()) {
                return ConditionOutcome.noMatch(ConditionMessage.forCondition(ConditionalOnMissingBean.class, new Object[]{spec}).found("bean", "beans").items(Style.QUOTE, matching));
            }
 
            matchMessage = matchMessage.andCondition(ConditionalOnMissingBean.class, new Object[]{spec}).didNotFind("any beans").atAll();
        }
 
        return ConditionOutcome.match(matchMessage);
    }
```
spec = new OnBeanCondition.BeanSearchSpec(context, metadata, ConditionalOnBean.class);
这句中，相当于从内部类中将标注@ConditionalOnMissingBean注解时的属性都取出来：
```java
BeanSearchSpec(ConditionContext context, AnnotatedTypeMetadata metadata, Class<?> annotationType) {
            this.annotationType = annotationType;
            MultiValueMap<String, Object> attributes = metadata.getAllAnnotationAttributes(annotationType.getName(), true);
            //将attributes这个map中的数据放到对应的list成员变量中
            this.collect(attributes, "name", this.names);
            this.collect(attributes, "value", this.types);
            this.collect(attributes, "type", this.types);
            this.collect(attributes, "annotation", this.annotations);
            this.collect(attributes, "ignored", this.ignoredTypes);
            this.collect(attributes, "ignoredType", this.ignoredTypes);
            this.strategy = (SearchStrategy)metadata.getAnnotationAttributes(annotationType.getName()).get("search");
            OnBeanCondition.BeanTypeDeductionException deductionException = null;
 
            try {
                if (this.types.isEmpty() && this.names.isEmpty()) {
                    this.addDeducedBeanType(context, metadata, this.types);
                }
            } catch (OnBeanCondition.BeanTypeDeductionException var7) {
                deductionException = var7;
            }
 
            this.validate(deductionException);
        }
 
        //验证的方法
        protected void validate(OnBeanCondition.BeanTypeDeductionException ex) {
            if (!this.hasAtLeastOne(this.types, this.names, this.annotations)) {
                String message = this.annotationName() + " did not specify a bean using type, name or annotation";
                if (ex == null) {
                    throw new IllegalStateException(message);
                } else {
                    throw new IllegalStateException(message + " and the attempt to deduce the bean's type failed", ex);
                }
            }
        }
```
看一下OnBeanCondition类中的getMatchingBeans方法，里面有用到搜索策略，详见搜索策略介绍
```java
private List<String> getMatchingBeans(ConditionContext context, OnBeanCondition.BeanSearchSpec beans) {
        //获得当前bean工厂
        ConfigurableListableBeanFactory beanFactory = context.getBeanFactory();
        //判断当前的搜索策略是否是PARENTS或者ANCESTORS，默认是ALL
        if (beans.getStrategy() == SearchStrategy.PARENTS || beans.getStrategy() == SearchStrategy.ANCESTORS) {
            BeanFactory parent = beanFactory.getParentBeanFactory();
            Assert.isInstanceOf(ConfigurableListableBeanFactory.class, parent, "Unable to use SearchStrategy.PARENTS");
            //如果是PARENTS或者ANCESTORS，当前bean工厂就用父工厂
            beanFactory = (ConfigurableListableBeanFactory)parent;
        }
 
        if (beanFactory == null) {
            return Collections.emptyList();
        } else {
            List<String> beanNames = new ArrayList();
            //如果当前搜索策略等于CURRENT，为true
            boolean considerHierarchy = beans.getStrategy() != SearchStrategy.CURRENT;
            //这里的type就是需要查找的bean的类型
            //下面，会从属性中找bean
            Iterator var6 = beans.getTypes().iterator();
 
            String beanName;
            while(var6.hasNext()) {
                beanName = (String)var6.next();
                //如果找到了类型，接下来就是根据类型找bean的实例名，找示例名的方法在下方，实际上就是一个getNamesForType
                beanNames.addAll(this.getBeanNamesForType(beanFactory, beanName, context.getClassLoader(), considerHierarchy));
            }
 
            var6 = beans.getIgnoredTypes().iterator();
 
            while(var6.hasNext()) {
                beanName = (String)var6.next();
                beanNames.removeAll(this.getBeanNamesForType(beanFactory, beanName, context.getClassLoader(), considerHierarchy));
            }
 
            var6 = beans.getAnnotations().iterator();
 
            while(var6.hasNext()) {
                beanName = (String)var6.next();
                beanNames.addAll(Arrays.asList(this.getBeanNamesForAnnotation(beanFactory, beanName, context.getClassLoader(), considerHierarchy)));
            }
 
            var6 = beans.getNames().iterator();
 
            while(var6.hasNext()) {
                beanName = (String)var6.next();
                if (this.containsBean(beanFactory, beanName, considerHierarchy)) {
                    beanNames.add(beanName);
                }
            }
            //将存放bean实例名的list返回
            return beanNames;
        }
    }
 
 
 
    //根据类型获取bean的name
    private Collection<String> getBeanNamesForType(ListableBeanFactory beanFactory, String type, ClassLoader classLoader, boolean considerHierarchy) throws LinkageError {
        try {
            Set<String> result = new LinkedHashSet();
            this.collectBeanNamesForType(result, beanFactory, ClassUtils.forName(type, classLoader), considerHierarchy);
            return result;
        } catch (ClassNotFoundException var6) {
            return Collections.emptySet();
        } catch (NoClassDefFoundError var7) {
            return Collections.emptySet();
        }
    }
 
    private void collectBeanNamesForType(Set<String> result, ListableBeanFactory beanFactory, Class<?> type, boolean considerHierarchy) {
        result.addAll(BeanTypeRegistry.get(beanFactory).getNamesForType(type));
        if (considerHierarchy && beanFactory instanceof HierarchicalBeanFactory) {
            BeanFactory parent = ((HierarchicalBeanFactory)beanFactory).getParentBeanFactory();
            if (parent instanceof ListableBeanFactory) {
                this.collectBeanNamesForType(result, (ListableBeanFactory)parent, type, considerHierarchy);
            }
        }
 
    }
```
找完bean了之后，回到刚才的代码里：
```java
//如果当前注入的bean是@ConditionalOnMissingBean
        if (metadata.isAnnotated(ConditionalOnMissingBean.class.getName())) {
            //返回一个spec（说明），这里的spec规定了搜索的内容，比如搜索策略、需要搜索的类名......
            spec = new OnBeanCondition.BeanSearchSpec(context, metadata, ConditionalOnMissingBean.class);
            matching = this.getMatchingBeans(context, spec);
            if (!matching.isEmpty()) {
                return ConditionOutcome.noMatch(ConditionMessage.forCondition(ConditionalOnMissingBean.class, new Object[]{spec}).found("bean", "beans").items(Style.QUOTE, matching));
            }
 
            matchMessage = matchMessage.andCondition(ConditionalOnMissingBean.class, new Object[]{spec}).didNotFind("any beans").atAll();
        }
```
如果第5行返回的list不是空的，就会返回ConditionOutcome对象noMatch方法，表示不匹配。ConditionOutcome类用于存放过滤结果，只有两个变量：
```java
/**
 * 过滤结果类
 */
public class ConditionOutcome {
    /**
     * 匹配结果 true or false
     */
    private final boolean match;
    /**
     * 匹配结果信息
     */
    private final ConditionMessage message;
```
两者区别：
@ConditionOnBean在判断list的时候，如果list没有值，返回false，否则返回true
@ConditionOnMissingBean在判断list的时候，如果list没有值，返回true，否则返回false，其他逻辑都一样
例子：
@ConditionalOnBean(javax.sql.DataSource.class)    
Spring容器或者所有父容器中需要存在至少一个javax.sql.DataSource类的实例
