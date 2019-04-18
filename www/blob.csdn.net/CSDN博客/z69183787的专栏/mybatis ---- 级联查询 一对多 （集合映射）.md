# mybatis ---- 级联查询 一对多 （集合映射） - z69183787的专栏 - CSDN博客
2015年07月10日 17:42:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：22476
关联有嵌套查询和嵌套结果两种方式，本文是按照嵌套结果这种方式来说明的
上一章介绍了多对一的关系，用到了<association></association>，这是一个复杂类型的关联。我们选择一个示例来回顾下,比如：一个博客有一个用户，关联映射就工作于这种结果之上。首先看下，我们在本文中要用到的表结构字段:
**[sql]**[view
 plain](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)[copy](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/256412/fork)
- 博客  
- blog :  id  title  author_id   
- 
- 作者  
- author:  id  username password email  bio favourite_section  
- 
- 文章  
- post :id  blog_id  author_id  created_on  section  subject  draft  body    
- 
- 评论  
- comment : id   post_id  name  comment    
- 
- 标签  
- T : id name
我们把一个博客和一个用户关联在一起，就像：
**[sql]**[view
 plain](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)[copy](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/256412/fork)
- <select id="selectBlog" parameterType="int" resultMap="blogResult">  
- select
-                b.id as blog_id,  
-                b.title as blog_title,  
-                b.author_id as blog_author_id  
-                a.id as author_id,  
-                a.username as author_username,  
-                a.passwordas author_passowrd,  
-                a.email as auhtor_email,  
-                a.bio as author_bio  
- from  blog b leftouterjoin author a on b.author_id=a.id  
- where b.id=#{id}  
-     </select>  
注意这个联合查询，以及所有结果被唯一而清晰的名字而重命名。这使得映射非常简单。现在我们可以映射这个结果：
**[sql]**[view
 plain](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)[copy](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/256412/fork)
- <resultMap id="blogResult" type="Blog">  
-      <id property="id"column="blog_id"/>  
-       <result property="title"column="blog_title"/>  
- 
-       <!-- 和一个用户关联,Blog 类里面属性时author,关联的列是原先的blog.author_id-->
-       <association property="author"column="blog_author_id" javaType="Author"  resultMap="authorResult"/>  
- </resultMap>  
- 
- <resultMap id="authorResult" type="Author">  
-           <id property="id"column="author_id"/>  
-           <result property="username"column="author_username"/>  
-           <result property="password"column="author_password"/>  
-           <result property="email"column="author_email"/>  
-           <result property="bio"column="author_bio"/>  
- </resultMap>  
在上面的例子中，你可以看到博客的作者关联authorResult 结果映射来加载作者实例。  上面的实例中，用了外部的结果映射元素来映射关联。这使得Author结果映射可以重用。然而，你不需要重用它的话，或者你仅仅引用你所有的结果映射到一个单独描述的结果映射中。你可以嵌套结果映射。这里给出使用这种方式的相同示例：
**[html]**[view
 plain](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)[copy](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/256412/fork)
- <resultMapid="blogResult"type="Blog">
- <idproperty="id"column="blog_id"/>
- <resultproperty="title"column="blog_title"/>
- 
- <!-- 和一个用户关联,Blog 类里面属性时author,关联的列是原先的blog.author_id-->
- <associationproperty="author"column="blog_author_id"javaType="Author">
- <idproperty="id"column="author_id"/>
- <resultproperty="username"column="author_username"/>
- <resultproperty="password"column="author_password"/>
- <resultproperty="email"column="author_email"/>
- <resultproperty="bio"column="author_bio"/>
- </association>
- </resultMap>
上面你已经看到了如何处理有一个类型的关联.但是“有很多个”是怎样的呢？，也就是集合类型，本文的主要工作是来说这个的。
## 集合
相对于关联来说，集合映射多了一个属性”ofType“.这个属性用来区分JavaBean（或字段）属性类型和集合包含的类型来说是很重要的. ，ofType用来表示集合包含类型。
**[html]**[view
 plain](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)[copy](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/256412/fork)
- <collectionproperty="posts"ofType="Post">
- <idproperty="id"column="post_id"/>
- <resultproperty="subject"column="post_subject"/>
- <resultproperty="body"column="post_body"/>
- </collection>
集合元素的作用和关联几乎是相同的。我们来继续上面的示例，一个博客只有一个作者。但是博客有很多文章，在博客类中，这可以由下面的写法来表示：
**[sql]**[view
 plain](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)[copy](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/256412/fork)
- private List<Post> posts;   
这一次联合博客表和文章表（一个blog_id可以对应很多的文章）SQL如下：
**[html]**[view
 plain](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)[copy](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/256412/fork)
- <selectid="selectBlog"parameterType="int"resultMap="blogResult">
-          select   
-              b.id as blog_id ,  
-              b.title as blog_title,  
-              b.author_id as blog_author_id,  
-              p.id as post_id,  
-              p.subject as post_subject,  
-              p.body as post_body  
-            from blog b  
-                left outer join  post p on b.id=p.blog_id  
-                where b.id=#{id}    
- </select>
现在用“文章映射集合”来映射 “博客“，可以简单写为：
**[html]**[view
 plain](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)[copy](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/256412/fork)
- <resultMapid="blogResult"type="Blog">
- <idproperty="id"column="blog_id"/>
- <resultproperty="title"column="blog_title"/>
- <collectionproperty="posts"ofType="Post">
- <idproperty="id"column="post_id"/>
- <resultproperty="subject"column="post_subject"/>
- <resultproperty="body"column="post_body"/>
- </collection>
- </resultMap>
高级关联和集合映射还有很多要琢磨的地方。就让面介绍的集合映射，稍微的整理下（一个博客下面，有很多文章）.
首先创建下，需要用到的表格，以及向其中插入一些数据. 
**[sql]**[view
 plain](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)[copy](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/256412/fork)
- createtable author(id int (11) notnull auto_increment,  
-                     username varchar(20) notnull,  
- passwordvarchar(20) notnull,   
-                     email varchar(20) notnull,  
-                     bio varchar(20) notnull,  
-                     favourite_section varchar(20) notnull,  
- primarykey(id)  
-                   )ENGINE=InnoDB DEFAULT CHARSET=utf8;  
- 
- 
- insertinto author(id,username,password,email,bio,favourite_section)   
- values(1001,'林枫','123456','824156593@qq.com','合肥的小男孩','旅游');  
- 
- 
- 
- createtable blog (id int (11) notnull auto_increment,  
-                     title varchar(20) notnull,  
-                      author_id int(11) notnull,  
- primarykey(id))  
-                      ENGINE=InnoDB DEFAULT CHARSET=utf8;    
- 
- insertinto blog(id,title,author_id) values(1,'小说博客',1001);                      
- 
- createtable post(id int (11) notnull auto_increment,  
-                   blog_id int(11) notnull,  
-                   author_id int(11) notnull,  
-                   created_on datenotnull,  
- sectionvarchar(20) notnull,  
-                   subject varchar(20) notnull,  
-                   draft varchar(20) notnull,  
-                   body varchar(20) notnull,  
- primarykey(id)  
-                     )ENGINE=InnoDB DEFAULT CHARSET=utf8;         
- 
- 
- insertinto post(id,blog_id,author_id,created_on,section,subject,draft,body)  
- values(1,1,1001,now(),'旅游','玄幻','草稿','绝世武神');    
- 
- insertinto post(id,blog_id,author_id,created_on,section,subject,draft,body)  
- values(2,1,1001,now(),'旅游','玄幻','草稿','大主宰');  
- 
- insertinto post(id,blog_id,author_id,created_on,section,subject,draft,body)  
- values(3,1,1001,now(),'旅游','玄幻','草稿','灵域');       
在贴JAVA代码之前，先看下目录结构吧：
![](https://img-blog.csdn.net/20140325200435140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveXVsZWlfcXE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
作者类 Author.java
**[java]**[view
 plain](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)[copy](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/256412/fork)
- package com.mybatis.model;  
- /**
-  * 作者类
-  * @author Administrator
-  *
-  */
- publicclass Author {  
- 
- privateint id;  
- private String username;  
- private String password;  
- private String email;  
- private String bio; //个人资料
- private String favourite_section; //最喜欢的。。
- 
- publicint getId() {  
- return id;  
-     }  
- publicvoid setId(int id) {  
- this.id = id;  
-     }  
- public String getUsername() {  
- return username;  
-     }  
- publicvoid setUsername(String username) {  
- this.username = username;  
-     }  
- public String getPassword() {  
- return password;  
-     }  
- publicvoid setPassword(String password) {  
- this.password = password;  
-     }  
- public String getEmail() {  
- return email;  
-     }  
- publicvoid setEmail(String email) {  
- this.email = email;  
-     }  
- public String getBio() {  
- return bio;  
-     }  
- publicvoid setBio(String bio) {  
- this.bio = bio;  
-     }  
- public String getFavourite_section() {  
- return favourite_section;  
-     }  
- publicvoid setFavourite_section(String favouriteSection) {  
-         favourite_section = favouriteSection;  
-     }  
- 
- }  
Blog.java
**[java]**[view
 plain](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)[copy](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/256412/fork)
- package com.mybatis.model;  
- 
- import java.util.List;  
- 
- /**
-  * 博客类
-  * @author Administrator
-  *
-  */
- publicclass Blog {  
- privateint id;  
- private String title;  
- private Author author;  
- private List<Post> posts; //博客类有很多文章， 与post表中的blog_id对应
- 
- public List<Post> getPosts() {  
- return posts;  
-     }  
- publicvoid setPosts(List<Post> posts) {  
- this.posts = posts;  
-     }  
- publicint getId() {  
- return id;  
-     }  
- publicvoid setId(int id) {  
- this.id = id;  
-     }  
- public String getTitle() {  
- return title;  
-     }  
- publicvoid setTitle(String title) {  
- this.title = title;  
-     }  
- public Author getAuthor() {  
- return author;  
-     }  
- publicvoid setAuthor(Author author) {  
- this.author = author;  
-     }  
- 
- }  
文章类Post.,java
**[java]**[view
 plain](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)[copy](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/256412/fork)
- package com.mybatis.model;  
- 
- import java.util.Date;  
- 
- /**
-  * 文章类
-  * @author Administrator
-  *
-  */
- publicclass Post {  
- privateint id;  
- privateint blog_id;  
- privateint author_id;  
- private Date created_on;  
- private String section;  
- private String subject;  
- private String draft;  
- private String body;  
- 
- publicint getId() {  
- return id;  
-     }  
- publicvoid setId(int id) {  
- this.id = id;  
-     }  
- publicint getBlog_id() {  
- return blog_id;  
-     }  
- publicvoid setBlog_id(int blogId) {  
-         blog_id = blogId;  
-     }  
- publicint getAuthor_id() {  
- return author_id;  
-     }  
- publicvoid setAuthor_id(int authorId) {  
-         author_id = authorId;  
-     }  
- public Date getCreated_on() {  
- return created_on;  
-     }  
- publicvoid setCreated_on(Date createdOn) {  
-         created_on = createdOn;  
-     }  
- public String getSection() {  
- return section;  
-     }  
- publicvoid setSection(String section) {  
- this.section = section;  
-     }  
- public String getSubject() {  
- return subject;  
-     }  
- publicvoid setSubject(String subject) {  
- this.subject = subject;  
-     }  
- public String getDraft() {  
- return draft;  
-     }  
- publicvoid setDraft(String draft) {  
- this.draft = draft;  
-     }  
- public String getBody() {  
- return body;  
-     }  
- publicvoid setBody(String body) {  
- this.body = body;  
-     }  
- 
- }  
总配置文件
**[html]**[view
 plain](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)[copy](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/256412/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE configuration     
-     PUBLIC "-//mybatis.org//DTD Config 3.0//EN"     
-     "http://mybatis.org/dtd/mybatis-3-config.dtd">
- <configuration>
- <typeAliases>
- <!-- 给实体类去一个别名  -->
- <typeAliastype="com.mybatis.model.Blog"alias="Blog"/>
- <typeAliastype="com.mybatis.model.Post"alias="Post"/>
- </typeAliases>
- 
- <!-- 数据源配置，这里用MySQL数据库 -->
- <environmentsdefault="development">
- <environmentid="development">
- <transactionManagertype="JDBC"/>
- <dataSourcetype="POOLED">
- <propertyname="driver"value="com.mysql.jdbc.Driver"/>
- <propertyname="url"value="jdbc:mysql://127.0.0.1:3306/test"/>
- <propertyname="username"value="root"/>
- <propertyname="password"value="123456"/>
- </dataSource>
- </environment>
- </environments>
- 
- <mappers>
- <mapperresource="com/mybatis/model/Blog.xml"/>
- </mappers>
- 
- </configuration>
Blog.xml
**[html]**[view
 plain](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)[copy](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/256412/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE mapper PUBLIC   
-     "-//mybatis.org//DTD Mapper 3.0//EN"  
-     "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
- 
- <mappernamespace="com.mybatis.dao.IBlogDao">
- 
- <!-- 用文章映射集合来映射博客 -->
- <resultMapid="blogResult"type="Blog">
- <idproperty="id"column="blog_id"/>
- <resultproperty="title"column="blog_title"/>
- 
- <!--文章集合 -->
- <collectionproperty="posts"ofType="Post">
- <idproperty="id"column="post_id"/>
- <resultproperty="subject"column="post_subject"/>
- <resultproperty="body"column="post_body"/>
- </collection>
- </resultMap>
- 
- <selectid="selectBlog"parameterType="int"resultMap="blogResult">
-          select   
-              b.id as blog_id ,  
-              b.title as blog_title,  
-              b.author_id as blog_author_id,  
-              p.id as post_id,  
-              p.subject as post_subject,  
-              p.body as post_body  
-            from blog b  
-                left outer join  post p on b.id=p.blog_id  
-                where b.id=#{id}    
- </select>
- </mapper>
测试类Test.java
**[java]**[view
 plain](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)[copy](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/256412/fork)
- package com.mybatis.test;  
- 
- import java.io.IOException;  
- import java.util.List;  
- 
- import org.apache.ibatis.io.Resources;  
- import org.apache.ibatis.session.SqlSession;  
- import org.apache.ibatis.session.SqlSessionFactory;  
- import org.apache.ibatis.session.SqlSessionFactoryBuilder;  
- 
- import com.mybatis.dao.IBlogDao;  
- import com.mybatis.model.Blog;  
- import com.mybatis.model.Post;  
- 
- publicclass Test {  
- 
- /***
-      * 获得MyBatis SqlSessionFactory
-      * SqlSessionFactory 负责创建SqlSession ,一旦创建成功，就可以用SqlSession实例来执行映射语句
-      * ，commit,rollback,close等方法
-      * @return
-      */
- privatestatic SqlSessionFactory getSessionFactory(){  
-         SqlSessionFactory sessionFactory=null;  
-         String resource="configuration.xml";  
- try {  
-             sessionFactory=new SqlSessionFactoryBuilder().build(Resources.getResourceAsReader(resource));  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         }  
- return sessionFactory;  
-     }  
- 
- /**
-      * main 方法
-      * @param args
-      */
- publicstaticvoid main(String[] args) {  
- 
-           SqlSession session=getSessionFactory().openSession();  
- try {  
-          IBlogDao blogDao=session.getMapper(IBlogDao.class);  
-          Blog blog=blogDao.selectBlog(1);  
-          List<Post> postList=blog.getPosts();  
- for(Post post:postList){  
-              System.out.println(post.getBody());  
-          }  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
- finally{  
-             session.close();  
-         }  
-     }  
- }  
运行后结果如下：
**[java]**[view
 plain](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)[copy](http://blog.csdn.net/yulei_qq/article/details/22057633?utm_source=tuicool#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/256412/fork)
- DEBUG [com.mybatis.dao.IBlogDao.selectBlog] - ooo Using Connection [com.mysql.jdbc.Connection@e00ed0]  
- DEBUG [com.mybatis.dao.IBlogDao.selectBlog] - ==>  Preparing: select b.id as blog_id , b.title as blog_title, b.author_id as blog_author_id, p.id as post_id, p.subject as post_subject, p.body as post_body from blog b left outer join post p on b.id=p.blog_id where b.id=?   
- DEBUG [com.mybatis.dao.IBlogDao.selectBlog] - ==> Parameters: 1(Integer)  
- 绝世武神  
- 大主宰  
- 灵域  
- 


