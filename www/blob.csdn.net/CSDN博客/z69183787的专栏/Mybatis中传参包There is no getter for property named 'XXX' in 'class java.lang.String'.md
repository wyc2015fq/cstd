# Mybatis中传参包There is no getter for property named 'XXX' in 'class java.lang.String' - z69183787的专栏 - CSDN博客
2015年06月14日 20:48:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5294
**一、发现问题**
<select id="queryStudentByNum" resultType="student" parameterType="string">  
select num,name,phone from student  
<where> 
<if test = " num!=null and num!='' ">
AND num = #{num}
</if>
</where>
</select> 
Mybatis查询传入一个字符串传参数，报There is no getter for property named 'num' in 'class java.lang.String'。
**二、解决问题**
<select id="queryStudentByNum" resultType="student" parameterType="string">  
select num,name,phone from student  
<where> 
<if test = " **_parameter**!=null and**_parameter**!='' ">
AND num = #{**_parameter**}
</if>
</where>
</select>
无论参数名，都要改成"_parameter"。
**三、原因分析**
Mybatis默认采用ONGL解析参数，所以会自动采用对象树的形式取string.num值，引起报错。也可以public List methodName(@Param(value="num") String num)的方法说明参数值
参考博客：
http://blog.sina.com.cn/s/blog_86e49b8f010191hw.html
http://txin0814.iteye.com/blog/1533645
**[java]**[view
 plain](http://blog.csdn.net/chao121/article/details/7542684#)[copy](http://blog.csdn.net/chao121/article/details/7542684#)
- 在类DynamicSqlSource   
- 方法   
- public BoundSql getBoundSql(Object parameterObject) {   
-     DynamicContext context = new DynamicContext(configuration, parameterObject);   
-     rootSqlNode.apply(context);   
-     SqlSourceBuilder sqlSourceParser = new SqlSourceBuilder(configuration);   
-     Class<?> parameterType = parameterObject == null ? Object.class : parameterObject.getClass();   
-     SqlSource sqlSource = sqlSourceParser.parse(context.getSql(), parameterType);   
-     BoundSql boundSql = sqlSource.getBoundSql(parameterObject);   
- for (Map.Entry<String, Object> entry : context.getBindings().entrySet()) {   
-       boundSql.setAdditionalParameter(entry.getKey(), entry.getValue());   
-     }   
- return boundSql;   
-   }   
- 第一行DynamicContext context = new DynamicContext(configuration, parameterObject);   
- 这句话是根据配置，sql语句，以及参数，来生成context。生成的context会会加入到ognl的contxt，因此在我们的sql访问#{_parameter}，只要你插入参数，就一定可以找到我们传入的这个参数，而不用管它是什么类型   
- 
- 进入后可以看到   
- public DynamicContext(Configuration configuration, Object parameterObject) {   
- if (parameterObject != null && !(parameterObject instanceof Map)) {   
-       MetaObject metaObject = configuration.newMetaObject(parameterObject);   
-       bindings = new ContextMap(metaObject);   
-     } else {   
-       bindings = new ContextMap(null);   
-     }   
-     bindings.put(PARAMETER_OBJECT_KEY, parameterObject);   
-   }   
- 1.bindings.put(PARAMETER_OBJECT_KEY, parameterObject);   
- 这里的PARAMETER_OBJECT_KEY = _parameter;   
- 因此在我们的sql中,类似于   
- <if test="_parameter != null" >   
-       <include refid="Example_Where_Clause" />   
-     </if>   
- 是没有问题的。   
- 
- 随后它会将这个传入的参数对象，根据它的类型，活得它可以得到的表达式。   
- if (object instanceof ObjectWrapper) {   
- this.objectWrapper = (ObjectWrapper) object;   
-     } elseif (objectWrapperFactory.hasWrapperFor(object)) {   
- this.objectWrapper = objectWrapperFactory.getWrapperFor(this, object);   
-     } elseif (object instanceof Map) {   
- this.objectWrapper = new MapWrapper(this, (Map) object);   
-     } else {   
- this.objectWrapper = new BeanWrapper(this, object);   
-     }   
- 
- 传入的是bean，BeanWrapper这个处理器会获取这个传入的参数的所有的get方法，并获取get方法后面的Xxx，如getName，就是name   
- 
- 多个参数   
- 我们可能会碰到多个参数作为参数的情况。   
- 如   
- public MessagegetMessages(String id, String messageType);   
- 
- 但ibatis只接受一个参数。   
- 这个时候可以使用javabean作为参数传递，或者使用Map作为参数传递。如   
- <select id="getMessages" parameterType="java.util.Map"
- resultType="com.liyixing.ibatis.model.Message" >   
- SELECT *   
- FROM message WHERE ID = #{id} AND MESSAGE_TYPE = #{messageType}   
- </select>   
- 
- Map<String, Object> params = new HashMap<String, Object>();   
- 
- params.put("id", 1);   
- params.put("messageType", 1);   
- List<Message> messages = messageDao.getMessages(params);   
- 
- 因为ibatis使用OGNL作为表达式的。它把参数作为root，因为ognl的root只有一个。所以ibatis只能支持一个参数。  
