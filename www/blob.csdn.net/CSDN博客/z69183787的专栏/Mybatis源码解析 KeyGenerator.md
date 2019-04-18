# Mybatis源码解析 KeyGenerator - z69183787的专栏 - CSDN博客
2017年08月04日 16:18:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1048
KeyGenerator接口定义了2个函数：
```
//执行insert之前
void processBefore(Executor executor, MappedStatement ms, Statement stmt, Object parameter);
//执行insert之后
void processAfter(Executor executor, MappedStatement ms, Statement stmt, Object parameter);
```
目前Mybatis給了3个实现：
![](http://upload-images.jianshu.io/upload_images/1938592-dbb34019c0aa0ac9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
### NoKeyGenerator
没有执行任何操作，并且是Mybatis的默认实现
### Jdbc3KeyGenerator
适用于可以自动生成主键的sql(自增等)，由于Statement.execute执行后返回的是操作行数，并不会返回主键，当配置了
`<insert id="insert" useGeneratedKeys="true" keyProperty="id">`
后，该函数会自动将id赋值到keyProperty对应的javabean属性中
具体代码如：
```
public void processBatch(MappedStatement ms, Statement stmt, List<Object> parameters) {
    ResultSet rs = null;
    try {
      rs = stmt.getGeneratedKeys();
      final Configuration configuration = ms.getConfiguration();
      final TypeHandlerRegistry typeHandlerRegistry = configuration.getTypeHandlerRegistry();
      //获得配置的属性字段名
      final String[] keyProperties = ms.getKeyProperties();
      //返回的id值
      final ResultSetMetaData rsmd = rs.getMetaData();
      TypeHandler<?>[] typeHandlers = null;
      if (keyProperties != null && rsmd.getColumnCount() >= keyProperties.length) {
        for (Object parameter : parameters) {//循环用于批量insert操作
          if (!rs.next()) break; // there should be one row for each statement (also one for each parameter)
          //将javabean 包装
          final MetaObject metaParam = configuration.newMetaObject(parameter);
          //类型转换，一般为java基础类型转换
          if (typeHandlers == null) typeHandlers = getTypeHandlers(typeHandlerRegistry, metaParam, keyProperties);
          populateKeys(rs, metaParam, keyProperties, typeHandlers);
        }
      }
    } catch (Exception e) {
      throw new ExecutorException("Error getting generated key or setting result to parameter object. Cause: " + e, e);
    } finally {
      if (rs != null) {
        try {
          rs.close();
        } catch (Exception e) {
          // ignore
        }
      }
    }
  }
private void populateKeys(ResultSet rs, MetaObject metaParam, String[] keyProperties, TypeHandler<?>[] typeHandlers) throws SQLException {
    for (int i = 0; i < keyProperties.length; i++) {
      TypeHandler<?> th = typeHandlers[i];
      if (th != null) {
        Object value = th.getResult(rs, i + 1);//获得db->java的值
        //设值到javabean对应的字段属性中
        metaParam.setValue(keyProperties[i], value);
      }
    }
  }
```
### SelectKeyGenerator：
通过自定义sql手动获取主键值，有2种配置，before和after
before既是在insert之前设置到pojo中作为参数一起insert到db
after即为在insert之后，通过自定义sql获取并设置到pojo中
```
<selectKey resultType="java.lang.Integer" keyProperty="id" order="BEFORE">  
      select max(id) from TB_USER  
</selectKey>
```
核心代码：
```
private void processGeneratedKeys(Executor executor, MappedStatement ms, Object parameter) {
    try {
      if (parameter != null && keyStatement != null && keyStatement.getKeyProperties() != null) {
        //获取配置的属性字段名
        String[] keyProperties = keyStatement.getKeyProperties();
        final Configuration configuration = ms.getConfiguration();
        final MetaObject metaParam = configuration.newMetaObject(parameter);
        if (keyProperties != null) {
          // Do not close keyExecutor.
          // The transaction will be closed by parent executor.
          //这里使用SimpleStatement
          Executor keyExecutor = configuration.newExecutor(executor.getTransaction(), ExecutorType.SIMPLE);
          //执行selectKey对应的sql语句
          List<Object> values = keyExecutor.query(keyStatement, parameter, RowBounds.DEFAULT, Executor.NO_RESULT_HANDLER);
          if (values.size() == 0) {
            throw new ExecutorException("SelectKey returned no data.");            
          } else if (values.size() > 1) {
            throw new ExecutorException("SelectKey returned more than one value.");
          } else {
            MetaObject metaResult = configuration.newMetaObject(values.get(0));
            if (keyProperties.length == 1) {//设值
              if (metaResult.hasGetter(keyProperties[0])) {
                setValue(metaParam, keyProperties[0], metaResult.getValue(keyProperties[0]));
              } else {
                // no getter for the property - maybe just a single value object
                // so try that
                setValue(metaParam, keyProperties[0], values.get(0));
              }
            } else {
              handleMultipleProperties(keyProperties, metaParam, metaResult);
            }
          }
        }
      }
    } catch (ExecutorException e) {
      throw e;
    } catch (Exception e) {
      throw new ExecutorException("Error selecting key or setting result to parameter object. Cause: " + e, e);
    }
  }
```
```
```
