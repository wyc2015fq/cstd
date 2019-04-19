# Spring MVC与mybatis学习-----执行路程详解 - BlueSky - CSDN博客
2015年12月28日 20:11:11[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：1175
Spring MVC学习重点就是学会其基本的处理数据的流程，明白它的架构然后往里面填充代码。
以下为基本Spring MVC架构的格式：
Controller类、Service类、Dao类、Mapper.xml、POJO对象类
## 整体流程如下：
**Controller类---（调用）--- Service类---（调用）---Dao类---（调用/映射）--- Mapper.xml----（调用/映射)----  Pojo类对象**
## 整体流程图：
![](https://img-blog.csdn.net/20151228201043733?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## Controller类
此类主要对应页面请求的处理方法，并把数据返回视图
Eg：
@Controller
@RequestMapping("/user")
Public class UserController {
    @Autowired
    Private UserService userService;
@RequestMapping(value="/showAll")
public String showall(HttpServletRequest request,Model model){
List<User> users = this.userService.getAll();
model.addAttribute("users", users);
return "test";
}
## Service
此类主要为通过Dao中相应的接口，实现相应数据逻辑的处理，返回给控制层所需的数据。
Eg：
@Service("userService")
public class UserService {
@Autowired
private UserDao userDao;
@Override
public List<User> getAll() {
List<User> users = 
                this.userDao.selectAll();
return users;
}
}
## Dao类
一般Dao为接口，提供映射过来的sql语句对应的方法，也包括sql返回数据的结果的存储类型。
eg:
public interface UserDao {
public User selectAll();
public List<User> selectAll();
}
说明：上面代表一个返回user对象类，一个返回用户列表List<User>
## Mapper.xml
对应的sql语句，包括的设置：
<mapper namespace=””>
   --对应的映射空间，即将此映射       
   id的sql语句对应到相应的空间
   （Dao类）
Id：相应sql语句的唯一标识,如  
     id=“selectAll”
parameterType：传给sql输入参数类型
resultType：sql输出的参数类型（一般为与相应的数据库对应POJO的Java对象，即将其每一列对应为对象的属性）
Eg：
<mapper namespace="com.asia.dao.UserDao">
  <select id="selectAll"       
     parameterType="com.asia.pojo.User"   
      resultType="com.asia.pojo.User">   
       select * from user_t 
  </select>
</mapper>
## POJO对象类
package com.asia.pojo;
public class User {
private int id;
private String user_name;
private String password;
private int age;
public int getId() {
return id;
}
public void setId(int id) {
this.id = id;
                .........
