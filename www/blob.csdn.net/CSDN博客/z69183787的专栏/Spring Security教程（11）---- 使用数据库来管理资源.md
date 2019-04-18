# Spring Security教程（11）---- 使用数据库来管理资源 - z69183787的专栏 - CSDN博客
2014年03月16日 08:15:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2412
这个可以说是SpringSecurity最核心的东西，在项目中资源很多肯定不能一一配置到配置文件中，所以用数据库来管理资源是必然的。这个也很容易实现。表结构已经在之前都创建过了。
首先我们要来从数据库中获取到资源与权限的对应列表，这个在dao层实现即可需要获取到url地址和AUTH_**这种权限标识，注意：不是权限ID和资源ID。
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18446481#)[copy](http://blog.csdn.net/jaune161/article/details/18446481#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/161723/fork)
- public List<Map<String,String>> getURLResourceMapping(){  
-     String sql = "SELECT S3.RESOURCE_PATH,S2.AUTHORITY_MARK FROM SYS_AUTHORITIES_RESOURCES S1 "+  
- "JOIN SYS_AUTHORITIES S2 ON S1.AUTHORITY_ID = S2.AUTHORITY_ID "+  
- "JOIN SYS_RESOURCES S3 ON S1.RESOURCE_ID = S3.RESOURCE_ID S3.RESOURCE_TYPE='URL' ORDER BY S3.PRIORITY DESC";  
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
创建SecurityMetadataSource供过滤器使用，SecurityMetadataSource需要实现FilterInvocationSecurityMetadataSource
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18446481#)[copy](http://blog.csdn.net/jaune161/article/details/18446481#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/161723/fork)
- publicclass URLFilterInvocationSecurityMetadataSource implements
-         FilterInvocationSecurityMetadataSource,InitializingBean {  
- 
- protectedfinal Log logger = LogFactory.getLog(getClass());  
- 
- privatefinalstatic List<ConfigAttribute> NULL_CONFIG_ATTRIBUTE = Collections.emptyList();  
- //权限集合
- private Map<RequestMatcher, Collection<ConfigAttribute>> requestMap;  
- 
- @Autowired
- private SysResourceRepository sysResourceRepository;  
- 
- /* (non-Javadoc)
-      * @see org.springframework.security.access.SecurityMetadataSource#getAttributes(java.lang.Object)
-      */
- @Override
- public Collection<ConfigAttribute> getAttributes(Object object)  
- throws IllegalArgumentException {  
- final HttpServletRequest request = ((FilterInvocation) object).getRequest();  
- 
-         Collection<ConfigAttribute> attrs = NULL_CONFIG_ATTRIBUTE;  
- 
- for (Map.Entry<RequestMatcher, Collection<ConfigAttribute>> entry : requestMap.entrySet()) {  
- if (entry.getKey().matches(request)) {  
-                 attrs =  entry.getValue();  
- break;  
-             }  
-         }  
-         logger.info("URL资源："+request.getRequestURI()+ " -> " + attrs);  
- return attrs;  
-     }  
- 
- /* (non-Javadoc)
-      * @see org.springframework.security.access.SecurityMetadataSource#getAllConfigAttributes()
-      */
- @Override
- public Collection<ConfigAttribute> getAllConfigAttributes() {  
-         Set<ConfigAttribute> allAttributes = new HashSet<ConfigAttribute>();  
- 
- for (Map.Entry<RequestMatcher, Collection<ConfigAttribute>> entry : requestMap.entrySet()) {  
-             allAttributes.addAll(entry.getValue());  
-         }  
- 
- return allAttributes;  
-     }  
- 
- /* (non-Javadoc)
-      * @see org.springframework.security.access.SecurityMetadataSource#supports(java.lang.Class)
-      */
- @Override
- publicboolean supports(Class<?> clazz) {  
- return FilterInvocation.class.isAssignableFrom(clazz);  
-     }  
- 
- private Map<String,String> loadResuorce(){  
-         Map<String,String> map = new LinkedHashMap<String,String>();  
- 
-         List<Map<String,String>> list = this.sysResourceRepository.getURLResourceMapping();  
-         Iterator<Map<String,String>> it = list.iterator();  
- while(it.hasNext()){  
-             Map<String,String> rs = it.next();  
-             String resourcePath = rs.get("resourcePath");  
-             String authorityMark = rs.get("authorityMark");  
- 
- if(map.containsKey(resourcePath)){  
-                 String mark = map.get("resourcePath");  
-                 map.put(resourcePath, mark+","+authorityMark);  
-             }else{  
-                 map.put(resourcePath, authorityMark);  
-             }  
-         }  
- return map;  
-     }  
- 
- protected Map<RequestMatcher, Collection<ConfigAttribute>> bindRequestMap(){  
-         Map<RequestMatcher, Collection<ConfigAttribute>> map =   
- new LinkedHashMap<RequestMatcher, Collection<ConfigAttribute>>();  
- 
-         Map<String,String> resMap = this.loadResuorce();  
- for(Map.Entry<String,String> entry:resMap.entrySet()){  
-             String key = entry.getKey();  
-             Collection<ConfigAttribute> atts = new ArrayList<ConfigAttribute>();  
-             atts = SecurityConfig.createListFromCommaDelimitedString(entry.getValue());  
-             map.put(new AntPathRequestMatcher(key), atts);  
-         }  
- 
- return map;  
-     }  
- 
- /* (non-Javadoc)
-      * @see org.springframework.beans.factory.InitializingBean#afterPropertiesSet()
-      */
- @Override
- publicvoid afterPropertiesSet() throws Exception {  
- this.requestMap = this.bindRequestMap();  
-         logger.info("资源权限列表"+this.requestMap);  
-     }  
- 
- publicvoid refreshResuorceMap(){  
- this.requestMap = this.bindRequestMap();  
-     }  
- 
- }  
bindRequestMap需要在类初始化的时候就完成，但是这个不能写在构造函数中，因为构造函数执行是SysResourceRepository还没有注入过来。所以就通过实现InitializingBean把初始化操作放在afterPropertiesSet方法中。
getAllConfigAttributes:获取所有权限集合
getAttributes：根据request请求获取访问资源所需权限
代码很简单，很容易看懂，就不再多做解释，下面看配置文件
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18446481#)[copy](http://blog.csdn.net/jaune161/article/details/18446481#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/161723/fork)
- <sec:httpauto-config="true"access-decision-manager-ref="accessDecisionManager">
- 
- <sec:access-denied-handlerref="accessDeniedHandler"/>
- 
- <sec:session-managementinvalid-session-url="/login.jsp"/>
- 
- <sec:form-loginlogin-page="/login.jsp"
- login-processing-url="/login.do"
- authentication-failure-url="/login.jsp"
- authentication-success-handler-ref="authenticationSuccessHandler"
- />
- 
- <sec:custom-filterref="filterSecurityInterceptor"before="FILTER_SECURITY_INTERCEPTOR"/>
- 
- </sec:http>
- 
- <beanid="filterSecurityInterceptor"class="org.springframework.security.web.access.intercept.FilterSecurityInterceptor">
- <propertyname="accessDecisionManager"ref="accessDecisionManager"/>
- <propertyname="authenticationManager"ref="authenticationManager"/>
- <propertyname="securityMetadataSource"ref="securityMetadataSource"/>
- </bean>
- 
- <beanid="securityMetadataSource"
- class="com.zrhis.system.security.URLFilterInvocationSecurityMetadataSource"/>
通过配置custom-filter来增加过滤器，before="FILTER_SECURITY_INTERCEPTOR"表示在SpringSecurity默认的过滤器之前执行。
FilterSecurityInterceptor还用SpringSecurity默认的就可以了，这个是没有必要自己写的只要在SecurityMetadataSource处理好资源与权限的对应关系就可以了。
到此为止SpringSecurity框架已基本完善，可以说在项目中用已经没什么问题了。
