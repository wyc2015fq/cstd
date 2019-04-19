# Neo4j安装后的密码修改 - =朝晖= - 博客园
# [Neo4j安装后的密码修改](https://www.cnblogs.com/dhcn/p/7124825.html)
     首先默认用户名/密码是neo4j/neo4j。
      在安全验证打开的时候，你访问服务器/db/data之类的地址可能会提示您以下信息：
- {  
-   "password_change" : "http://serverip:7474/user/neo4j/password",  
-   "errors" : [ {  
-     "code" : "Neo.ClientError.Security.Forbidden",  
-     "message" : "User is required to change their password."  
-   } ]  
- }  
      以上提示的那个页面地址其实没什么用，尝试访问根本无效，最后可用的解决方法是访问 http://serverip:7474/，进去以后随便点个命令会跳到http://serverip:7474/browser/地址，会提示您登陆，用neo4j:neo4j账号登陆后，第一个界面就是让你修改密码，然后重新设置一个密码即可。
参考资料：http://stackoverflow.com/questions/27645951/how-to-configure-user-and-password-for-neo4j-cluster-without-rest-api

