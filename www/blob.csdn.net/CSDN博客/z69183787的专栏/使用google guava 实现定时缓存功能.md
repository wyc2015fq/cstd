# 使用google guava 实现定时缓存功能 - z69183787的专栏 - CSDN博客
2015年11月17日 10:36:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2990
使用google guava 实现定时缓存功能。
创建Book.java
**[java]**[view
 plain](http://blog.csdn.net/mach365/article/details/8103552#)[copy](http://blog.csdn.net/mach365/article/details/8103552#)[print](http://blog.csdn.net/mach365/article/details/8103552#)[?](http://blog.csdn.net/mach365/article/details/8103552#)
- publicclass Book {  
- privateint id;  
- publicint getId() {  
- return id;  
-     }  
- publicvoid setId(int id) {  
- this.id = id;  
-     }  
- public Book(int id) {  
- super();  
- this.id = id;  
-     }  
- public Book() {  
- super();  
-     }  
- @Override
- public String toString() {  
- return""+id;  
-     }  
- }  
创建BookDao.java
**[java]**[view
 plain](http://blog.csdn.net/mach365/article/details/8103552#)[copy](http://blog.csdn.net/mach365/article/details/8103552#)[print](http://blog.csdn.net/mach365/article/details/8103552#)[?](http://blog.csdn.net/mach365/article/details/8103552#)
- publicclass BookDao {  
- public Object executeSQL() {  
-         System.out.println("此处调用了Dao方法。executeSQL");  
-         List<Book> books = new ArrayList<Book>();  
- for (int i = 0; i < 3; i++) {  
-             Book b = new Book(i);  
-             books.add(b);  
-         }  
- 
- 
- return books;  
-     }  
- }  
创建BookCache.java
**[java]**[view
 plain](http://blog.csdn.net/mach365/article/details/8103552#)[copy](http://blog.csdn.net/mach365/article/details/8103552#)[print](http://blog.csdn.net/mach365/article/details/8103552#)[?](http://blog.csdn.net/mach365/article/details/8103552#)
- publicclass BookCache {  
- publicstatic Cache<String, List<Book>> cache = CacheBuilder.newBuilder()  
-             .expireAfterAccess(8, TimeUnit.SECONDS).maximumSize(10)  
-             .build();  
- }  
创建运行类。
**[java]**[view
 plain](http://blog.csdn.net/mach365/article/details/8103552#)[copy](http://blog.csdn.net/mach365/article/details/8103552#)[print](http://blog.csdn.net/mach365/article/details/8103552#)[?](http://blog.csdn.net/mach365/article/details/8103552#)
- publicstaticvoid main(String[] args) {  
- try {  
-         System.out.println("第一次调用dao方法,正确状态:应该调用Dao里的方法");  
-         List<Book> books = BookCache.cache.get("points",  
- new Callable<List<Book>>() {  
- @Override
- public List<Book> call() {  
-                         BookDao dao = new BookDao();  
-                         List<Book> list = (List<Book>) dao.executeSQL();  
- if (null == list || list.size() <= 0) {  
-                             list = new ArrayList<Book>();  
-                         }  
- return list;  
-                     }  
-                 });  
- for (Book book : books) {  
-             System.out.println(book);  
-         }  
-         System.out.println("第二次调用dao方法,正确状态:不调用Dao里的方法");  
-         List<Book> books2 = BookCache.cache.get("points",  
- new Callable<List<Book>>() {  
- @Override
- public List<Book> call() {  
-                         BookDao dao = new BookDao();  
-                         List<Book> list = (List<Book>) dao.executeSQL();  
- if (null == list || list.size() <= 0) {  
-                             list = new ArrayList<Book>();  
-                         }  
- return list;  
-                     }  
-                 });  
- for (Book book : books2) {  
-             System.out.println(book);  
-         }  
-     } catch (Exception e1) {  
-         e1.printStackTrace();  
-     }  
- try {  
-         Thread.currentThread();  
-         Thread.sleep(TimeUnit.SECONDS.toMillis(10));  
-     } catch (InterruptedException e) {  
- // TODO Auto-generated catch block
-         e.printStackTrace();  
-     }  
- try {  
-         System.out.println("休息十秒后,第三次调用dao方法,正确状态:调用Dao里的方法");  
-         List<Book> books = BookCache.cache.get("points",  
- new Callable<List<Book>>() {  
- @Override
- public List<Book> call() {  
-                         BookDao dao = new BookDao();  
-                         List<Book> list = (List<Book>) dao.executeSQL();  
- if (null == list || list.size() <= 0) {  
-                             list = new ArrayList<Book>();  
-                         }  
- return list;  
-                     }  
-                 });  
- for (Book book : books) {  
-             System.out.println(book);  
-         }  
-         System.out.println("第四次调用dao方法,正确状态:不调用Dao里的方法");  
-         List<Book> books2 = BookCache.cache.get("points",  
- new Callable<List<Book>>() {  
- @Override
- public List<Book> call() {  
-                         BookDao dao = new BookDao();  
-                         List<Book> list = (List<Book>) dao.executeSQL();  
- if (null == list || list.size() <= 0) {  
-                             list = new ArrayList<Book>();  
-                         }  
- return list;  
-                     }  
-                 });  
- for (Book book : books2) {  
-             System.out.println(book);  
-         }  
-     } catch (Exception e1) {  
-         e1.printStackTrace();  
-     }  
- }  
运行结果：
**[plain]**[view
 plain](http://blog.csdn.net/mach365/article/details/8103552#)[copy](http://blog.csdn.net/mach365/article/details/8103552#)[print](http://blog.csdn.net/mach365/article/details/8103552#)[?](http://blog.csdn.net/mach365/article/details/8103552#)
- 第一次调用dao方法,正确状态:应该调用Dao里的方法  
- 此处调用了Dao方法。executeSQL  
- 0  
- 1  
- 2  
- 第二次调用dao方法,正确状态:不调用Dao里的方法  
- 0  
- 1  
- 2  
- 休息十秒后,第三次调用dao方法,正确状态:调用Dao里的方法  
- 此处调用了Dao方法。executeSQL  
- 0  
- 1  
- 2  
- 第四次调用dao方法,正确状态:不调用Dao里的方法  
- 0  
- 1  
- 2  
- 
