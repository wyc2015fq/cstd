# WebService CXF学习（进阶篇2）：复杂对象传递(List，Map) - z69183787的专栏 - CSDN博客
2014年06月30日 11:23:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4368
前面讲到了JavaBean对象的传递，这一节我们就CXF框架复杂对象(List，Map)的传递进行讲解。 
**第一步：创建存储复杂对象的类(因为WebServices的复杂对象的传递，一定要借助第三方对象(即自定义对象)来实现)**
**[java]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606494#)[copy](http://blog.csdn.net/vickychen89/article/details/6606494#)
- package com.ws.model;  
- 
- import java.util.ArrayList;  
- import java.util.HashMap;  
- 
- publicclass Users {  
- private ArrayList<UserInfo> userList;  // 不要用List类型
- private HashMap<Integer, UserInfo> userMap; // 不要用Map类型
- private UserInfo[] userArray;  
- // geter/seter 方法
- }  
**第二步：创建WebServices的服务端接口和实现类**
**[java]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606494#)[copy](http://blog.csdn.net/vickychen89/article/details/6606494#)
- package com.ws.services;  
- 
- import javax.jws.WebService;  
- import com.ws.model.UserInfo;  
- import com.ws.model.Users;  
- 
- @WebService
- publicinterface IUserServices {  
- 
- public Users getAllUsers();  
- 
- public Users getUsersMap();  
- 
- public Users getUsersArray();  
- 
- }  
**[java]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606494#)[copy](http://blog.csdn.net/vickychen89/article/details/6606494#)
- package com.ws.services.impl;  
- 
- import java.util.ArrayList;  
- import java.util.HashMap;  
- import javax.jws.WebService;  
- import com.ws.model.UserInfo;  
- import com.ws.model.Users;  
- import com.ws.services.IUserServices;  
- 
- @WebService
- publicclass UserServicesImpl implements IUserServices {  
- 
- public Users getAllUsers() {  
-         ArrayList<UserInfo> list = new ArrayList<UserInfo>();  
-         list.add(new UserInfo("vicky",23));  
-         list.add(new UserInfo("ivy",26));  
-         list.add(new UserInfo("simon",26));  
-         list.add(new UserInfo("carol",29));  
-         Users users = new Users();  
-         users.setUserList(list);  
- return users;  
-     }  
- 
- public Users getUsersMap() {  
-         HashMap<Integer, UserInfo> map = new HashMap<Integer, UserInfo>();  
-         map.put(23, new UserInfo("vicky",23));  
-         map.put(22, new UserInfo("caty",22));  
-         map.put(24, new UserInfo("leah",24));  
-         map.put(25, new UserInfo("kelly",25));  
-         map.put(27, new UserInfo("ivy",27));  
-         map.put(26, new UserInfo("simon",26));  
-         map.put(29, new UserInfo("carol",29));  
- 
-         Users users = new Users();  
-         users.setUserMap(map);  
- return users;  
-     }  
- 
- public Users getUsersArray() {  
-         UserInfo[] userInfo = new UserInfo[5];  
- for(int i=0;i<5;i++){  
-             UserInfo info = new UserInfo();  
-             info.setUserAge(23+i);  
-             info.setUserName("Array"+(i+1));  
-             userInfo[i] = info;  
-         }  
-         Users users = new Users();  
-         users.setUserArray(userInfo);  
- return users;  
-     }  
- 
- }  
**第三步：创建WebServices的服务端**
**[java]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606494#)[copy](http://blog.csdn.net/vickychen89/article/details/6606494#)
- package com.test;  
- 
- import javax.xml.ws.Endpoint;  
- import org.apache.cxf.jaxws.JaxWsServerFactoryBean;  
- import com.ws.services.impl.UserServicesImpl;  
- 
- publicclass ServerTest {  
- public ServerTest(){  
- // 发布User服务接口
-         Endpoint.publish("http://localhost:8090/userInfoServices", new UserServicesImpl());  
-     }  
- publicstaticvoid main(String[] args) {  
- // 启动服务
- new ServerTest();  
-         System.out.println("Server ready...");     
- try {  
-             Thread.sleep(1000*300);//休眠五分分钟，便于测试 
-         } catch (InterruptedException e) {  
-             e.printStackTrace();  
-         }     
-         System.out.println("Server exit...");     
-         System.exit(0);  
-     }  
- }  
**第四步：创建WebServices的客户端，并测试**    1、将服务端创建的复杂对象的类和接口copy到客户工程中，且路径要与服务端一致；
    2、新建测试类进行测试
**[java]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606494#)[copy](http://blog.csdn.net/vickychen89/article/details/6606494#)
- package com.ws.client;  
- 
- import java.util.List;  
- import org.apache.cxf.jaxws.JaxWsProxyFactoryBean;  
- import com.ws.model.UserInfo;  
- import com.ws.model.Users;  
- import com.ws.server.IUserServices;  
- 
- publicclass UserTest {  
- publicstaticvoid main(String[] args) {  
- //创建WebService客户端代理工厂   
-         JaxWsProxyFactoryBean factory = new JaxWsProxyFactoryBean();     
- //注册WebService接口   
-         factory.setServiceClass(IUserServices.class);     
- //设置WebService地址   
-         factory.setAddress("http://localhost:8090/userInfoServices");          
-         IUserServices userServices = (IUserServices)factory.create();     
-         System.out.println("invoke userinfo webservice...");  
- // 测试Map
- //      testMap(userServices);
- // 测试List
- //      testList(userServices);
- // 测试Array
- //      testArray(userServices);
-         System.exit(0);     
-     }   
- 
- publicstaticvoid testArray(IUserServices userServices){  
-         Users users = userServices.getUsersArray();  
- if(users!=null){  
-             UserInfo[] array = users.getUserArray();  
- for(UserInfo info:array){  
-                 System.out.println("UserName: "+info.getUserName());  
-                 System.out.println("UserAge : "+info.getUserAge());  
-             }  
-         }  
-     }  
- 
- publicstaticvoid testList(IUserServices userServices){  
-         Users users = userServices.getAllUsers();  
- if(users!=null){  
-             List<UserInfo> list = users.getUserList();  
- for(UserInfo info:list){  
-                 System.out.println("UserName: "+info.getUserName());  
-                 System.out.println("UserAge : "+info.getUserAge());  
-             }  
-         }  
-     }  
- 
- publicstaticvoid testMap(IUserServices userServices){  
-         Users users = userServices.getUsersMap();  
- if(users!=null){  
-             UserInfo info = users.getUserMap().get(23);  
-             System.out.println("UserName: "+info.getUserName());  
-             System.out.println("UserAge : "+info.getUserAge());  
-         }  
-     }  
- }  
**第五步：运行服务端，验证webservices服务是否发布成功第六步：运行客户端，验证是否成功调用webservices服务**
**注：在做webServices复杂对象传递时，返回值的类型不要用接口类型。例如(List 应该换成ArrayList ,Map应该换成HashMap)**
