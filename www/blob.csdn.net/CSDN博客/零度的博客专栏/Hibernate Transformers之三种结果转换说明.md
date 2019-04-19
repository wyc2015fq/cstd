# Hibernate Transformers之三种结果转换说明 - 零度的博客专栏 - CSDN博客
2016年11月17日 15:45:48[零度anngle](https://me.csdn.net/zmx729618)阅读数：4335

在hibernate使用的过程中，我们通常需要对结果进行解释。Hibernate为我们提供了以下3种查询结果解释方法：
- Transformers.ALIAS_TO_ENTITY_MAP //把输出结果转换成map  
- Transformers.TO_LIST //把结果按顺序排进List  
- Transformers.aliasToBean(target) //把结果通过setter方法注入到指定的对像属性中  
在Hibernate中Transformers的所有转换都是需要实现ResultTransformer接口。
① ALIAS_TO_ENTITY_MAP ：太简单了就是把key和value直接转换到Map当中 ：
- /** 
- * {@inheritDoc} 
- */
- // aliases key  
- // tuple value  
- 
- public Object transformTuple(Object[] tuple, String[] aliases) {   
-     Map result = new HashMap(tuple.length);   
- for ( int i=0; i<tuple.length; i++ ) {   
-         String alias = aliases[i];   
- if ( alias!=null ) {   
-             result.put( alias, tuple[i] );   
-         }   
-     } 
- 
- return result;   
- }  
② TO_LIST：转换过程很简单，就是把value转换成List对像：
- public Object transformTuple(Object[] tuple, String[] aliases) {   
- return Arrays.asList( tuple );   
- }  
③ aliasToBean：转换过程就是通过读取查询后的字段，然后通过使用setter方法注入到目标对像中 ：
- getSession().beginTransaction(); 
- Query query = getSession().createSQLQuery("select * from operation"); 
- list =query.setResultTransformer(Transformers.aliasToBean(Operation.class)).list(); 
- getSession().getTransaction().commit(); 
- 
[](http://blog.csdn.net/sheng__sheng/article/details/8438620#)[](http://blog.csdn.net/sheng__sheng/article/details/8438620#)[](http://blog.csdn.net/sheng__sheng/article/details/8438620#)[](http://blog.csdn.net/sheng__sheng/article/details/8438620#)[](http://blog.csdn.net/sheng__sheng/article/details/8438620#)[](http://blog.csdn.net/sheng__sheng/article/details/8438620#)
