# Spring Security教程（12）---- 使用数据库来管理方法 - z69183787的专栏 - CSDN博客
2014年03月16日 08:16:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1260
这个稍微有一点复杂，我是通过AOP来实现的，前半部分跟上一章类似，主要在配置上有点不同
读取方法与权限对应列表DAO
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18453785#)[copy](http://blog.csdn.net/jaune161/article/details/18453785#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/162084/fork)
- public List<Map<String,String>> getMethodResourceMapping(){  
-     String sql = "SELECT S3.RESOURCE_PATH,S2.AUTHORITY_MARK FROM SYS_AUTHORITIES_RESOURCES S1 "+  
- "JOIN SYS_AUTHORITIES S2 ON S1.AUTHORITY_ID = S2.AUTHORITY_ID "+  
- "JOIN SYS_RESOURCES S3 ON S1.RESOURCE_ID = S3.RESOURCE_ID AND S3.RESOURCE_TYPE='METHOD' ORDER BY S3.PRIORITY DESC";  
- 
-     List<Map<String,String>> list = new ArrayList<Map<String,String>>();  
- 
-     Query query = this.entityManager.createNativeQuery(sql);  
-     List<Object[]> result = query.getResultList();  
-     Iterator<Object[]> it = result.iterator();  
- 
- while(it.hasNext()){  
-         Object[] o = it.next();  
-         Map<String,String> map = new HashMap<String,String>();  
-         map.put("resourcePath", (String)o[0]);  
-         map.put("authorityMark", (String)o[1]);  
-         list.add(map);  
-     }  
- 
- return list;  
- }  
这里只针对方法名拦截，对于同一个类中的同名方法统统作为一个方法拦截。如果需要更细粒度的拦截，即拦截到参数，请在完成这个内容之后自行进行深入的研究。
MethodKey用来做主键用的
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18453785#)[copy](http://blog.csdn.net/jaune161/article/details/18453785#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/162084/fork)
- public class MethodKey {  
-     private String className;  
-     private String methodName;  
- 
-     public MethodKey(){};  
- 
-     public MethodKey(String fullName){  
- this.className = StringUtils.stripFilenameExtension(fullName);  
- this.methodName = StringUtils.getFilenameExtension(fullName);  
-     };  
- 
-     public MethodKey(Method method) {  
-         super();  
- this.className = method.getDeclaringClass().getName();  
- this.methodName = method.getName();  
-     }  
-     public String getClassName() {  
-         return className;  
-     }  
-     public void setClassName(String className) {  
- this.className = className;  
-     }  
-     public String getMethodName() {  
-         return methodName;  
-     }  
-     public void setMethodName(String methodName) {  
- this.methodName = methodName;  
-     }  
- 
-     public String getFullMethodName(){  
-         return this.className + "." + this.methodName;  
-     }  
- 
-     @Override  
-     public boolean equals(Object obj) {  
-         if(!(obj instanceof MethodKey))return false;  
- 
-         MethodKey target = (MethodKey)obj;  
- 
-         if(this.className.equals(target.getClassName()) &&   
-                 this.methodName.equals(target.getMethodName()))return true;  
- 
-         return false;  
-     }  
- 
- }  
getDeclaringClass获取到的是接口，这里我们只拦截接口。
MethodSecurityMetadataSource
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18453785#)[copy](http://blog.csdn.net/jaune161/article/details/18453785#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/162084/fork)
- publicclass MethodSecurityMetadataSource extends
-         AbstractMethodSecurityMetadataSource implements InitializingBean{  
- 
- //protected Log logger = LogFactory.getLog(getClass());
- 
- privatefinalstatic List<ConfigAttribute> NULL_CONFIG_ATTRIBUTE = Collections.emptyList();  
- 
- privatefinalstatic String RES_KEY = "resourcePath";  
- privatefinalstatic String AUTH_KEY = "authorityMark";  
- 
- private Map<MethodKey, Collection<ConfigAttribute>> requestMap;  
- 
- @Autowired
- private SysResourceRepository sysResourceRepository;  
- 
- /**
-      * 根据方法获取到访问方法所需要的权限
-      * @param method 访问的方法
-      * @param targetClass 方法所属的类
-      */
- @Override
- public Collection<ConfigAttribute> getAttributes(Method method,  
-             Class<?> targetClass) {  
-         MethodKey key = new MethodKey(method);  
-         Collection<ConfigAttribute> attrs = NULL_CONFIG_ATTRIBUTE;  
- 
- for (Map.Entry<MethodKey, Collection<ConfigAttribute>> entry : requestMap.entrySet()) {  
- if (entry.getKey().equals(key)) {  
-                 attrs =  entry.getValue();  
- break;  
-             }  
-         }  
-         logger.info("METHOD资源："+key.getFullMethodName()+ " -> " +attrs);  
- 
- return attrs;  
-     }  
- 
- /**
-      * 获取到所有方法对应的权限集合
-      */
- @Override
- public Collection<ConfigAttribute> getAllConfigAttributes() {  
-         Set<ConfigAttribute> allAttributes = new HashSet<ConfigAttribute>();  
- 
- for (Map.Entry<MethodKey, Collection<ConfigAttribute>> entry : requestMap.entrySet()) {  
-             allAttributes.addAll(entry.getValue());  
-         }  
- 
- return allAttributes;  
-     }  
- 
- /**
-      * 初始化方法权限对应集合，绑定方法权限集合
-      */
- @Override
- publicvoid afterPropertiesSet() throws Exception {  
- this.requestMap = this.bindRequestMap();  
-     }  
- 
- /**
-      * 从数据库中获取方法及权限对应信息
-      * @return
-      */
- private Map<String,String> loadMehod(){  
-         Map<String,String> resMap = new LinkedHashMap<String, String>();  
-         List<Map<String,String>> list = this.sysResourceRepository.getMethodResourceMapping();  
- 
- for(Map<String,String> map : list){  
-             String resourcePath = map.get(RES_KEY);  
-             String authorityMark = map.get(AUTH_KEY);  
- 
- if(resMap.containsKey(resourcePath)){  
-                 String mark = resMap.get(resourcePath);  
-                 resMap.put(resourcePath, mark+","+authorityMark);  
-             }else{  
-                 resMap.put(resourcePath, authorityMark);  
-             }  
-         }  
- 
- return resMap;  
-     }  
- 
- /**
-      * 封装从数据库中获取的方法权限集合
-      * @return
-      */
- public Map<MethodKey, Collection<ConfigAttribute>> bindRequestMap(){  
-         Map<MethodKey, Collection<ConfigAttribute>> resMap =   
- new LinkedHashMap<MethodKey, Collection<ConfigAttribute>>();  
- 
-         Map<String,String> map = this.loadMehod();  
- for(Map.Entry<String, String> entry : map.entrySet()){  
-             MethodKey key = new MethodKey(entry.getKey());  
-             Collection<ConfigAttribute> atts =   
-                     SecurityConfig.createListFromCommaDelimitedString(entry.getValue());  
- 
-             resMap.put(key, atts);  
-         }  
- 
- return resMap;  
-     }  
- 
- }  
与资源的SecurityMetadataSource类似，只不过拦截方法的SecurityMetadataSource需要继承自AbstractMethodSecurityMetadataSource或实现MethodSecurityMetadataSource
具体配置
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18453785#)[copy](http://blog.csdn.net/jaune161/article/details/18453785#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/162084/fork)
- <beanid="methodSecurityInterceptor"
- class="org.springframework.security.access.intercept.aopalliance.MethodSecurityInterceptor">
- <propertyname="accessDecisionManager"ref="accessDecisionManager"/>
- <propertyname="authenticationManager"ref="authenticationManager"/>
- <propertyname="securityMetadataSource"ref="methodSecurityMetadataSource"/>
- </bean>
- 
- <beanid="methodSecurityMetadataSource"
- class="com.zrhis.system.security.MethodSecurityMetadataSource"/>
- 
- <aop:config>
- <aop:pointcutid="sevicePointcut"
- expression="execution(* com.zrhis.**.service.*.*(..))"/>
- <aop:advisoradvice-ref="methodSecurityInterceptor"pointcut-ref="sevicePointcut"order="1"/>
- </aop:config>
首先创建pointcut，pointcut是项目中的Service层。然后在advisor中配置拦截器及切面的对应关系
方法的拦截是通过AOP来实现的，方法的拦截到此结束
有资源的拦截和方法的拦截基本上就能保证项目的权限能够灵活分配了。
