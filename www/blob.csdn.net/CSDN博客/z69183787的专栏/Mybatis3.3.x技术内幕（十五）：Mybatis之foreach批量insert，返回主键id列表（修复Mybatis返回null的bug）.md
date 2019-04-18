# Mybatis3.3.x技术内幕（十五）：Mybatis之foreach批量insert，返回主键id列表（修复Mybatis返回null的bug） - z69183787的专栏 - CSDN博客
2017年08月04日 15:56:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4226
https://my.oschina.net/zudajun/blog/674946
**官方已经修复了https://github.com/mybatis/mybatis-3/pull/547**
摘要: Mybatis在执行批量插入时，如果使用的是for循环逐一插入，那么可以正确返回主键id。如果使用动态sql的foreach循环，那么返回的主键id列表，可能为null，这让很多人感到困惑；本文将分析问题产生的原因，并修复返回主键id为null的问题。该问题在开源中国社区，以及网络上，已经有很多人遇到并发帖咨询，似乎都没有得到期望的解决结果。今天，我将带领大家，分析并解决该问题，让foreach批量插入，返回正确的id列表。
Mybatis在执行批量插入时，如果使用的是for循环逐一插入，那么可以正确返回主键id。如果使用动态sql的foreach循环，那么返回的主键id列表，可能为null，这让很多人感到困惑；本文将分析问题产生的原因，并修复返回主键id为null的问题。该问题在开源中国社区，以及网络上，已经有很多人遇到并发帖咨询，似乎都没有得到期望的解决结果。今天，我将带领大家，分析并解决该问题，让foreach批量插入，返回正确的id列表。
```
<insert id="insertStudents" useGeneratedKeys="true" keyProperty="studId" parameterType="java.util.ArrayList">
		INSERT INTO
		STUDENTS(STUD_ID, NAME, EMAIL, DOB, PHONE)
		VALUES
	<foreach collection="list" item="item" index="index" separator=","> 
        	(#{item.studId},#{item.name},#{item.email},#{item.dob}, #{item.phone}) 
    	</foreach> 
	</insert>
```
以上便是Mybatis的foreach循环，其要生成的sql语句是：insert into students(stud_id, name) values(?, ?),(?, ?), (?, ?); 类似这样的批量插入。
Mybatis是对Jdbc的封装，我们来看看，Jdbc是否支持上述形式的批量插入，并返回主键id列表的。
```
PreparedStatement pstm = conn.prepareStatement("insert into students(name, email) values(?, ?), (?, ?), (?, ?)",
				Statement.RETURN_GENERATED_KEYS);
pstm.setString(1, "name1");
pstm.setString(2, "email1");
pstm.setString(3, "name2");
pstm.setString(4, "email2");
		
pstm.setString(5, "name2");
pstm.setString(6, "email2");
pstm.addBatch();
pstm.executeBatch();
ResultSet rs = pstm.getGeneratedKeys();
while (rs.next()) {
	Object value = rs.getObject(1);
	System.out.println(value);
}
```
Output:
```
248
249
250
```
好了，事实证明，Jdbc是支持上述批量插入，并能正确返回id列表的。Jdbc都支持，如果Mybatis却不支持，有点说不过去。
1. Mapper.xml中keyProperty和parameterType属性之间的关系（很重要）
`useGeneratedKeys="true" keyProperty="studId" parameterType="Student"`
上述xml配置，含义为，属性studId是参数类型Student对象的主键属性。毫无疑问，Student对象中有studId属性。
`useGeneratedKeys="true" keyProperty="studId" parameterType="java.util.ArrayList"`
那这个如何解释呢？ArrayList有studId属性吗？当然没有了。其正确含义为：ArrayList集合中的元素的studId属性。
所以，keyProperty和parameterType之间的关系，有时是直接关系，有时是间接关系。明白这个道理之后，我们就可以开始进一步阅读源码了。
2. Mybatis对parameter object的解析
org.apache.ibatis.executor.keygen.Jdbc3KeyGenerator.java源码（只保留了重点源码）
```java
```java
@Override
  public void processAfter(Executor executor, MappedStatement ms, Statement stmt, Object parameter) {
    processBatch(ms, stmt, getParameters(parameter));
  }
  public void processBatch(MappedStatement ms, Statement stmt, Collection<Object> parameters) {
    ResultSet rs = null;
    try {
      rs = stmt.getGeneratedKeys();
        // 迭代出来的对象parameter，一定要具备keyProperty属性
        for (Object parameter : parameters) {
          metaParam.setValue(keyProperties, value);
        }
      }
    }
  }
  private Collection<Object> getParameters(Object parameter) {
    Collection<Object> parameters = null;
    if (parameter instanceof Collection) {
      // 集合
      parameters = (Collection) parameter;
    } else if (parameter instanceof Map) {
      // map
      Map parameterMap = (Map) parameter;
      if (parameterMap.containsKey("collection")) {
        parameters = (Collection) parameterMap.get("collection");
      } else if (parameterMap.containsKey("list")) {
        parameters = (List) parameterMap.get("list");
      } else if (parameterMap.containsKey("array")) {
        parameters = Arrays.asList((Object[]) parameterMap.get("array"));
      }
    }
    if (parameters == null) {
      parameters = new ArrayList<Object>();
      parameters.add(parameter);
    }
    return parameters;
  }
```
```
上面这段代码，非常关键且重要，特别是我做了注释的地方，for(Object parameter : parameters)循环，表示parameters一定是一个集合，如果传递的是Student对象，那么Mybatis会将其封装到List<Student>中，然后再进行迭代操作。于是，迭代出来的parameter就是Student对象，就具备了keyProperty指定的属性了，比如studId属性。
如果传递的是一个List<Student>呢？
org.apache.ibatis.session.defaults.DefaultSqlSession.wrapCollection(Object)源码。
```
executor.update(ms, wrapCollection(parameter));
// ...
  private Object wrapCollection(final Object object) {
    // 如果是集合，再度包装为Map对象
    if (object instanceof Collection) {
      StrictMap<Object> map = new StrictMap<Object>();
      map.put("collection", object);
      if (object instanceof List) {
        map.put("list", object);
      }
      return map;
    } else if (object != null && object.getClass().isArray()) {
      // 数组
      StrictMap<Object> map = new StrictMap<Object>();
      map.put("array", object);
      return map;
    }
    return object;
  }
```
上面这段代码也非常重要，如果传递的是List<Student>，那么，将包装为一个Map<String, Collection>对象。
于是，List<Student>形式的parameter object就变成了下面这个样子，一个Map<String, List<Student>>对象，Map的size()为2，key分别为“collection”和“list”。下面会经常用到这个Map<String, List<Student>>对象，所以，要记住其数据结构。
```
{
    collection=[
        com.mybatis3.domain.Student@2d2ffcb7,
        com.mybatis3.domain.Student@762ef0ea
    ],
    list=[
        com.mybatis3.domain.Student@2d2ffcb7,
        com.mybatis3.domain.Student@762ef0ea
    ]
}
```
因此，Mybatis将集合类参数对象，包装成上面的一个Map<String, List<Student>>结构了。明白了数据的组织结构，就可以进行下一步的分析了。
3. SimpleExecutor和ReuseExecutor可以正确返回foreach批量插入后的id列表的原理
还记得如何配置Executor吗？
```xml
<setting name="defaultExecutorType" value="SIMPLE" />
```
既然集合参数，已经被包装成了Map<String, List<Student>>对象，当然就无法使用for(Object parameter : parameters)来迭代Map<String, List<Student>>了，我们看看SimpleExecutor和ReuseExecutor是如何做到的。
```java
```java
private Collection<Object> getParameters(Object parameter) {
    Collection<Object> parameters = null;
    if (parameter instanceof Collection) {
      parameters = (Collection) parameter;
    } else if (parameter instanceof Map) {
      Map parameterMap = (Map) parameter;
      if (parameterMap.containsKey("collection")) {
        // 返回map中key=collection的value
        parameters = (Collection) parameterMap.get("collection");
      } else if (parameterMap.containsKey("list")) {
        // 返回map中key=list的value
        parameters = (List) parameterMap.get("list");
      } else if (parameterMap.containsKey("array")) {
        parameters = Arrays.asList((Object[]) parameterMap.get("array"));
      }
    }
    if (parameters == null) {
      parameters = new ArrayList<Object>();
      parameters.add(parameter);
    }
    return parameters;
  }
```
```
getParameters()方法，会再次处理参数类型，前面是包装，这里是拆封，于是，无论返回上面的哪一个value，都是List<Student>或Collection集合，于是就可以使用for(Object parameter : parameters)来迭代，迭代出来的parameter就是Student，Student的主键属性为keyProperty。
结论：使用SimpleExecutor和ReuseExecutor，执行foreach批量插入，可以正确返回主键id列表。
然而，很可惜，BatchExecutor却存在bug，返回主键id列表为null值。
4. BatchExecutor执行foreach批量插入，返回主键id列表为null的原因以及如何修复
每当提到批量插入，同学们总是自然而然的想到BatchExecutor，这是程序员的本能。就像一想到交女朋友，就想到美女是一样的道理。
BatchExecutor使用了一个BatchResult对象，来保存执行参数以及执行结果。
org.apache.ibatis.executor.BatchResult.java源码。
```java
```java
public class BatchResult {
  private final List<Object> parameterObjects;
  // 居然不建议使用了
  @Deprecated
  public Object getParameterObject() {
    return parameterObjects.get(0);
  }
  // 直接返回List<map>对象
  public List<Object> getParameterObjects() {
    return parameterObjects;
  }
  // 将parameterObject放到List中
  public void addParameterObject(Object parameterObject) {
    this.parameterObjects.add(parameterObject);
  }
```
```
前面已经讲述了，List<Student>，被包装为Map<String, List<Student>>对象了，BatchResult又把Map<String, List<Student>>放到List中，于是，参数对象数据结构就变成了List<Map<String, List<Student>>>。
org.apache.ibatis.executor.BatchExecutor.doFlushStatements()方法源码。
```
Jdbc3KeyGenerator jdbc3KeyGenerator = (Jdbc3KeyGenerator) keyGenerator;
jdbc3KeyGenerator.processBatch(ms, stmt, parameterObjects);
```
此时的parameterObjects对象，已经是List<Map<String, List<Student>>>对象了，再执行for(Object parameter : parameterObjects)迭代，迭代出来的parameter是Map<String, List<Student>>对象，Map<String, List<Student>>对象当然没有keyProperty指定的属性了，期望迭代出来的目标对象是Student，而不是Map。于是，就产生了错误。由于不能正确赋值，自然就无法将主键id值，赋值给Student对象的主键属性studId了，所以返回主键id值null，大家就认为是Mybatis不支持，其实是个误会。
自己动手，修复该问题(修改BatchExecutor.doFlushStatements()方法源码)：
```java
```java
//Mybaits源码
//jdbc3KeyGenerator.processBatch(ms, stmt, parameterObjects);
//修复后代码
jdbc3KeyGenerator.processBatch(ms, stmt, this.getParameters(batchResult.getParameterObject()));
// org.apache.ibatis.executor.BatchExecutor中手动新增下面这个方法
public Collection<Object> getParameters(Object parameter) {
    Collection<Object> parameters = null;
    if (parameter instanceof Collection) {
      parameters = (Collection) parameter;
    } else if (parameter instanceof Map) {
      Map parameterMap = (Map) parameter;
      if (parameterMap.containsKey("collection")) {
        parameters = (Collection) parameterMap.get("collection");
      } else if (parameterMap.containsKey("list")) {
        parameters = (List) parameterMap.get("list");
      } else if (parameterMap.containsKey("array")) {
        parameters = Arrays.asList((Object[]) parameterMap.get("array"));
      }
    }
    if (parameters == null) {
      parameters = new ArrayList<Object>();
      parameters.add(parameter);
    }
    return parameters;
  }
```
```
解释一下上面的代码：
1. batchResult.getParameterObject()返回List<Map<String, List<Student>>>中的第0个元素（List长度本身就是1），于是得到Map<String, List<Student>>对象。
2. getParameters(map)方法拆封，返回map的任一value对象，该value对象就是原始的List<Student>对象。该方法本是org.apache.ibatis.executor.keygen.Jdbc3KeyGenerator内的一个private方法，在外面不能调用，于是，复制一份出来，放到BatchExecutor中来使用。
3. for(Object parameter : parameters)迭代后，parameter就是Student元素，该元素有主键属性studId，于是把数据库返回的主键id值，赋给sutdId属性。
经过以上三个步骤，我们的BatchExecutor就可以通过foreach批量插入，正确返回id列表了。
至此，SimpleExecutor、ReuseExecutor、BatchExecutor，均可以执行foreach批量插入，并正确返回id列表了。直接修改源代码，有点暴力，后续讲到plugin拦截器时，可以再看看，有没有更优雅的方式。
注：我不清楚Mybatis为何要这么设计，这究竟真是一个bug，还是Mybatis故意为之，只有时间能给出答案了。
