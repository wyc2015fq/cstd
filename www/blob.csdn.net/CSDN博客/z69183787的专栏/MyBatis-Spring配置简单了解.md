# MyBatis-Spring配置简单了解 - z69183787的专栏 - CSDN博客
2015年10月06日 19:26:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：823
个人分类：[对象映射框架-Ibatis/Mybatis](https://blog.csdn.net/z69183787/article/category/2261201)
# MyBatis-Spring配置简单了解
## [SqlSessionFactoryBean]([https://mybatis.github.io/spring/zh/factorybean.html](https://mybatis.github.io/spring/zh/factorybean.html))配置
在基本的 MyBatis 中,session 工厂可以使用 SqlSessionFactoryBuilder 来创建。而在 MyBatis-Spring 中,则使用 SqlSessionFactoryBean 来替代。
### 示例
```
<bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
  <property name="dataSource" ref="dataSource"/>
  <property name="mapperLocations">
    <array>
      <value>classpath:mapper/*.xml</value>
    </array>
  </property>
  <property name="typeAliasesPackage" value="com.isea533.mybatis.model"/>
  <property name="plugins">
    <array>
      <bean class="com.github.pagehelper.PageHelper">
        <property name="properties">
          <value>
            dialect=hsqldb
            reasonable=true
          </value>
        </property>
      </bean>
      <bean class="com.github.abel533.mapperhelper.MapperInterceptor">
        <property name="properties">
          <value>
            mappers=com.github.abel533.mapper.Mapper
            IDENTITY=MYSQL
            notEmpty=true
          </value>
        </property>
      </bean>
    </array>
  </property>
</bean>
```
### `dataSource`属性
该属性必须配置，多数据源时会有多个`dataSource`，同时也需要配置多个`sqlSessionFactory`来对应。
### `mapperLocations`属性
配置该属性后，`sqlSessionFactory`会自动扫描该路径下的所有文件并解析。
该路径支持多个，可以用`,;\t\n`进行分割。
每一个路径都可以用直接的包路径，或者Ant风格的表达式。
### `configLocation`属性
上面例子中并没有使用该属性，当`SqlSessionFactoryBean`提供的配置不能满足使用时，你可以使用`mybatis-config.xml`配置文件配置其他属性，然后通过`configLocation`属性指定该配置的路径，`SqlSessionFactoryBean`会使用该配置文件创建`Configuration`。
### `typeAliasesPackage`属性
该属性可以给包中的类注册别名，注册后可以直接使用类名，而不用使用全限定的类名（就是不用包含包名）。
该属性可以配置多个，可以用`,;\t\n`进行分割。但是不支持Ant风格的路径。
### `plugins`属性
该属性可以配置MyBatis的拦截器，拦截器的配置顺序会影响拦截器的执行顺序。
从上往下的拦截器，实际的执行顺序是这样，第一个拦截器会最后执行，最后一个会首先执行。
然后出拦截器的顺序和配置的顺序一致，第一个最先返回，最后一个最后返回。
就以上面的配置为例，一个简单的执行顺序图如下：
![执行顺序](https://img-blog.csdn.net/20150511105056542)
这些拦截器执行的顺序都是环绕型，不要简单理解为简单的顺序执行。
从配置顺序来看，第一个配置的在最里面，后面的依次环绕上一个拦截器。
## [MapperScannerConfigurer](https://mybatis.github.io/spring/zh/mappers.html) 配置
为了代替手工使用 SqlSessionDaoSupport 或 SqlSessionTemplate 编写数据访问对象 (DAO)的代码,MyBatis-Spring 提供了一个动态代理的实现:MapperFactoryBean。这个类 可以让你直接注入数据映射器接口到你的 service 层 bean 中。当使用映射器时,你仅仅如调 用你的 DAO 一样调用它们就可以了,但是你不需要编写任何 DAO 实现的代码,因为 MyBatis-Spring 将会为你创建代理。
### 示例
```
<bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
    <property name="basePackage" value="com.isea533.mybatis.mapper"/>
    <property name="annotationClass" value="XXX"/>
    <property name="sqlSessionFactoryBeanName" ref="sqlSessionFactory"/>
</bean>
```
### 一般用过的最多也就是这3个属性，实际情况下最常用的只有`basePackage`。
### `basePackage`属性
`basePackage`可以写多个，可以用`,;\t\n`进行分割。
每一个路径都可以用直接的包路径，或者Ant风格的表达式。
### `annotationClass`属性
该属性实际上就是起到一个过滤的作用，如果设置了该属性，那么MyBatis的接口只有包含该注解，才会被扫描进去。
### `sqlSessionFactoryBeanName`属性
这个属性一般都用不到，只有当你配置多数据源的时候，这是会有多个`sqlSessionFactory`，你就需要通过该属性来指定哪一个`sqlSessionFactory`（值为`SqlSessionFactoryBean``<bean>`配置中的`id`属性）。
## Ant通配符
- `?` 匹配任何单字符
- `*` 匹配0或者任意数量的字符
- `**` 匹配0或者更多的目录
### 举例
- `/project/*.a` 匹配项目根路径下所有在project路径下的.a文件
- `/project/p?ttern` 匹配项目根路径下 /project/pattern 和 /app/pXttern,但是不包括/app/pttern
- `/**/example` 匹配项目根路径下 /project/example, /project/foow/example, 和 /example
- `/project/**/dir/file.*` 匹配项目根路径下/project/dir/file.jsp, /project/foow/dir/file.html
- `/**/*.jsp` 匹配项目根路径下任何的.jsp 文件
### 最长匹配原则(has more characters)
URL请求`/project/dir/file.jsp`，现在存在两个路径匹配模式`/**/*.jsp和/project/dir/*.jsp`，那么会根据模式`/project/dir/*.jsp`来匹配
