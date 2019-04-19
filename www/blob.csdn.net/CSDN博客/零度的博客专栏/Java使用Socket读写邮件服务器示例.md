# Java使用Socket读写邮件服务器示例 - 零度的博客专栏 - CSDN博客
2016年06月13日 14:47:26[零度anngle](https://me.csdn.net/zmx729618)阅读数：3064
                
配置文件：
**[plain]**[view plain](http://blog.csdn.net/z69183787/article/details/23258311#)[copy](http://blog.csdn.net/z69183787/article/details/23258311#)[print](http://blog.csdn.net/z69183787/article/details/23258311#)[?](http://blog.csdn.net/z69183787/article/details/23258311#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/282562/fork)
- host=XXX.XX.XX.XX  
- port=8888  
- max_size=200  
- min_size=30  
```
host=XXX.XX.XX.XX
port=8888
max_size=200
min_size=30
```
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/23258311#)[copy](http://blog.csdn.net/z69183787/article/details/23258311#)[print](http://blog.csdn.net/z69183787/article/details/23258311#)[?](http://blog.csdn.net/z69183787/article/details/23258311#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/282562/fork)
- package com.wondersgroup.stjt.mail.util;  
- 
- import java.net.*;  
- import java.io.IOException;  
- /**
- * 
- */
- publicclass SocketAdapter extends  Socket{  
- /**
-    * 连接状态
- 
-    */
- privateboolean status=true;  
- /**
-    * 默认的构造函数
- 
-    */
- public SocketAdapter() {  
- super();  
-   }  
- public SocketAdapter(String host,int port)throws UnknownHostException,IOException{  
- super(host,port);  
-   }  
- /**
-    * 判断此连接是否空闲
- 
-    * @return boolean 空闲返回ture,否则false
-    */
- publicboolean isFree(){  
- return status;  
-   }  
- /**
-    * 当使用此连接的时候设置状态为false（忙碌）
-    */
- publicvoid setBusy(){  
- this.status=false;  
-   }  
- /**
-    * 当客户端关闭连接的时候状态设置为true(空闲）
- 
-    */
- @Override
- publicvoid close()throws IOException{  
- // System.out.println("Close : set the status is free ");
-         status=true;  
- super.close();  
-   }  
- 
- publicvoid destroy() throws IOException{  
-     close();  
- 
-   }  
- }  
```java
package com.wondersgroup.stjt.mail.util;
import java.net.*;
import java.io.IOException;
/**
* 
*/
public class SocketAdapter extends  Socket{
  /**
   * 连接状态
   */
  private boolean status=true;
  /**
   * 默认的构造函数
   */
  public SocketAdapter() {
    super();
  }
  public SocketAdapter(String host,int port)throws UnknownHostException,IOException{
    super(host,port);
  }
  /**
   * 判断此连接是否空闲
   * @return boolean 空闲返回ture,否则false
   */
  public boolean isFree(){
    return status;
  }
  /**
   * 当使用此连接的时候设置状态为false（忙碌）
   */
  public void setBusy(){
    this.status=false;
  }
  /**
   * 当客户端关闭连接的时候状态设置为true(空闲）
   */
  @Override
  public void close()throws IOException{
   // System.out.println("Close : set the status is free ");
    	status=true;
		super.close();
  }
  
  public void destroy() throws IOException{
    close();
  }
}
```
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/23258311#)[copy](http://blog.csdn.net/z69183787/article/details/23258311#)[print](http://blog.csdn.net/z69183787/article/details/23258311#)[?](http://blog.csdn.net/z69183787/article/details/23258311#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/282562/fork)
- package com.wondersgroup.stjt.mail.util;  
- 
- import java.net.*;  
- import java.util.*;  
- import java.io.IOException;   
- 
- publicinterface
-     ConnectionProvider {  
- publicstaticfinal String SERVER_IP ="host";  
- publicstaticfinal String SERVER_PORT = "port";  
- publicstaticfinal String MAX_SIZE = "max_size";  
- publicstaticfinal String MIN_SIZE = "min_size";   
- 
- /**
-    *判断连接池内是否有连接
- 
-    * @return true 有连接返回true,否则返回false
-    */
- publicboolean isPooled();   
- 
- /**
-    * 当此方法被调用的时候提供一个 socket
-    * @see Socket
-    * @return Socket a Connection object.
-    */
- public SocketAdapter getConnection() throws java.net.SocketException;   
- 
- /**
-    * 连接池初始化
-    */
- publicvoid init() throws UnknownHostException, IOException;   
- 
- /**
-    * 连接池重新启动
- 
-    */
- publicvoid restart() throws UnknownHostException, IOException;   
- 
- /**
-    * 注销连接池
- 
-    */
- publicvoid destroy() throws  IOException;  
- }  
```java
package com.wondersgroup.stjt.mail.util;
import java.net.*;
import java.util.*;
import java.io.IOException; 
public interface
    ConnectionProvider {
  public static final String SERVER_IP ="host";
  public static final String SERVER_PORT = "port";
  public static final String MAX_SIZE = "max_size";
  public static final String MIN_SIZE = "min_size"; 
  /**
   *判断连接池内是否有连接
   * @return true 有连接返回true,否则返回false
   */
  public boolean isPooled(); 
  /**
   * 当此方法被调用的时候提供一个 socket
   * @see Socket
   * @return Socket a Connection object.
   */
  public SocketAdapter getConnection() throws java.net.SocketException; 
  /**
   * 连接池初始化
   */
  public void init() throws UnknownHostException, IOException; 
  /**
   * 连接池重新启动
   */
  public void restart() throws UnknownHostException, IOException; 
  /**
   * 注销连接池
   */
  public void destroy() throws  IOException;
}
```
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/23258311#)[copy](http://blog.csdn.net/z69183787/article/details/23258311#)[print](http://blog.csdn.net/z69183787/article/details/23258311#)[?](http://blog.csdn.net/z69183787/article/details/23258311#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/282562/fork)
- package com.wondersgroup.stjt.mail.util;  
- 
- import java.util.*;  
- import java.net.*;  
- import java.io.IOException;   
- 
- /**
- *
- */
- publicclass MyConnectionProvider  
- implements ConnectionProvider {   
- 
- private Properties pro = null;  
- privatestatic ConnectionProvider provider = null;  
- privatestatic Object object_lock = new Object();  
- privatestatic Object[] object_lock_list = new Object[Integer.parseInt(ResourceBundle.getBundle("mailsocket").getString("max_size"))];  
- private String ip;  
- private String port;   
- privatestatic InetSocketAddress inetSockAddr=null;  
- /**
-    * 默认的最大连接数
-    */
- privateint max_size = 20;   
- 
- /**
-    * 默认的最小连接数 。。。没用
- 
-    */
- privateint min_size = 10;   
- 
- /**
-    * Socket connection
-    */
- private SocketAdapter[] socketpool = null;   
- 
- /**
-    * 构造对象的时候初始化连接池
- 
-    * @throws UnknownHostException
-    * @throws IOException
-    */
- private MyConnectionProvider() throws UnknownHostException,  
-       IOException {  
-       ResourceBundle  res =ResourceBundle.getBundle("mailsocket");  
-     ip =res.getString(SERVER_IP);  
-     port = res.getString(SERVER_PORT);  
-     String max_size_s = res.getString(MAX_SIZE);  
-     String min_size_s = res.getString(MIN_SIZE);  
- if (max_size_s != null) {  
-       max_size = Integer.parseInt(max_size_s);  
-     }  
- if (min_size_s != null) {  
-       min_size = Integer.parseInt(min_size_s);  
-     }   
- 
-     init(); //构造对象的时候初始化连接池
- 
-   }   
- 
- /**
-    * 判断是否已经池化
-    * @return boolean 如果池化返回ture,反之返回false
-    */
- publicboolean isPooled() {  
- if (socketpool != null) {  
- returntrue;  
-     }  
- elsereturnfalse;  
-   }   
- 
- /**
-    *返回一个连接
- 
-    * @return a Connection object.
-  * @throws IOException 
-    */
- public SocketAdapter getConnection()  {  
-       SocketAdapter s = null;  
- //System.out.println("loop");
- for (int i = 0; i < socketpool.length; i++) {  
- //System.out.println("loop detail i="+i);
- if (socketpool[i] != null) {  
- //如果有空闲的连接，返回一个空闲连接，如果没有，继续循环
- 
- if(!socketpool[i].isFree()){  
- continue;  
-             }else{  
- synchronized (object_lock_list[i]){  
- if (socketpool[i].isFree()) {  
-                   s = socketpool[i];  
- 
- // System.out.println(i+" socket isConnected()="+s.isConnected()+" binding"+s.isBound());
- try {  
-                       s=socketpool[i]=new SocketAdapter(ip, Integer.parseInt(port));  
-                     socketpool[i].setBusy();  
- // System.out.println("socket"+i);
-                   } catch (IOException e) {  
- 
-                       e.printStackTrace();  
- returnnull;  
-                   }  
- return s;  
-                  }else{  
- continue;  
-                  }  
-                 }  
-             }  
- 
- 
-       }  
- else { //如果连接为空，证明超过最小连接数，重新生成连接
- 
- try {  
-             System.out.println("new socket"+i);  
-           s = socketpool[i] = new SocketAdapter(ip, Integer.parseInt(port));  
- return s;  
-         }  
- catch (Exception e) {  
- //never throw
- returnnull;  
-         }  
-       }  
-     }  
- return s;  
-   }   
- 
- /**
-    * 初始化连接池
-    * @throws UnknownHostException 
-    * @throws IOException 
-    */
- publicvoid init() throws UnknownHostException, IOException {   
- 
-     socketpool = new SocketAdapter[max_size];  
- 
- for (int i = 0; i < max_size; i++) {  
-         object_lock_list[i]=new Object();  
-       }  
- 
-     System.out.println("System init success ....");  
-   }   
- 
- /**
-    * 重新启动连接池
- 
-    * @throws UnknownHostException
-    * @throws IOException
-    */
- publicvoid restart() throws UnknownHostException, IOException {  
-   destroy();  
-   init();  
-   }   
- 
- /**
-    * 注销此连接池
-  * @throws IOException 
-    */
- publicvoid destroy() throws IOException {  
- for (int i = 0; i < socketpool.length; i++) {  
- if (socketpool[i] != null) {  
-         SocketAdapter adapter = (SocketAdapter) socketpool[i];  
-         adapter.destroy();  
- //System.out.print("" );
-       }  
-     }  
-     System.out.println("\ndestory success ....");  
-   }  
- /**
-    * 生成此连接池实现的对象
- 
-    * @throws UnknownHostException
-    * @throws IOException 
-    * @return ConnectionProvider 
-    */
- publicstatic ConnectionProvider newInstance() throws
-       UnknownHostException, IOException {  
- if (provider == null) {  
- synchronized (object_lock) {  
- if (provider == null) {  
-           provider = new MyConnectionProvider();  
-         }  
-       }  
-     }  
- return provider;  
-   }  
- /**
-  * 读取properties文件。
- 
-  * @param key
-  * @param properName
-  * @return
-  */
- publicstatic String readProperties(String key, String properName) {  
-         ResourceBundle rsrc = null;  
-         String value = "";  
-         rsrc = ResourceBundle.getBundle(properName);  
-         value = rsrc.getString(key);  
- return value;  
- 
-     }  
- }  
```java
package com.wondersgroup.stjt.mail.util;
import java.util.*;
import java.net.*;
import java.io.IOException; 
/**
*
*/
public class MyConnectionProvider
    implements ConnectionProvider { 
  private Properties pro = null;
  private static ConnectionProvider provider = null;
  private static Object object_lock = new Object();
  private static Object[] object_lock_list = new Object[Integer.parseInt(ResourceBundle.getBundle("mailsocket").getString("max_size"))];
  private String ip;
  private String port; 
  private static InetSocketAddress inetSockAddr=null;
  /**
   * 默认的最大连接数
   */
  private int max_size = 20; 
  /**
   * 默认的最小连接数 。。。没用
   */
  private int min_size = 10; 
  /**
   * Socket connection
   */
  private SocketAdapter[] socketpool = null; 
  /**
   * 构造对象的时候初始化连接池
   * @throws UnknownHostException
   * @throws IOException
   */
  private MyConnectionProvider() throws UnknownHostException,
      IOException {
	  ResourceBundle  res =ResourceBundle.getBundle("mailsocket");
    ip =res.getString(SERVER_IP);
    port = res.getString(SERVER_PORT);
    String max_size_s = res.getString(MAX_SIZE);
    String min_size_s = res.getString(MIN_SIZE);
    if (max_size_s != null) {
      max_size = Integer.parseInt(max_size_s);
    }
    if (min_size_s != null) {
      min_size = Integer.parseInt(min_size_s);
    } 
    init(); //构造对象的时候初始化连接池
  } 
  /**
   * 判断是否已经池化
   * @return boolean 如果池化返回ture,反之返回false
   */
  public boolean isPooled() {
    if (socketpool != null) {
      return true;
    }
    else return false;
  } 
  /**
   *返回一个连接
   * @return a Connection object.
 * @throws IOException 
   */
  public SocketAdapter getConnection()  {
	  SocketAdapter s = null;
	  //System.out.println("loop");
    for (int i = 0; i < socketpool.length; i++) {
    	//System.out.println("loop detail i="+i);
      if (socketpool[i] != null) {
        //如果有空闲的连接，返回一个空闲连接，如果没有，继续循环
    	  if(!socketpool[i].isFree()){
    		  continue;
        	}else{
        		synchronized (object_lock_list[i]){
        		 if (socketpool[i].isFree()) {
       			  s = socketpool[i];
       			 
       			 // System.out.println(i+" socket isConnected()="+s.isConnected()+" binding"+s.isBound());
       			  try {
       				  s=socketpool[i]=new SocketAdapter(ip, Integer.parseInt(port));
       				socketpool[i].setBusy();
       				// System.out.println("socket"+i);
       			  } catch (IOException e) {
       				  e.printStackTrace();
       				  return null;
       			  }
       			  return s;
        		 }else{
        			 continue;
        		 }
        		}
        	}
    	 
        
      }
      else { //如果连接为空，证明超过最小连接数，重新生成连接
        try {
        	System.out.println("new socket"+i);
          s = socketpool[i] = new SocketAdapter(ip, Integer.parseInt(port));
          return s;
        }
        catch (Exception e) {
          //never throw
        	return null;
        }
      }
    }
    return s;
  } 
  /**
   * 初始化连接池
   * @throws UnknownHostException 
   * @throws IOException 
   */
  public void init() throws UnknownHostException, IOException { 
    socketpool = new SocketAdapter[max_size];
    for (int i = 0; i < max_size; i++) {
    	object_lock_list[i]=new Object();
      }
  
    System.out.println("System init success ....");
  } 
  /**
   * 重新启动连接池
   * @throws UnknownHostException
   * @throws IOException
   */
  public void restart() throws UnknownHostException, IOException {
  destroy();
  init();
  } 
  /**
   * 注销此连接池
 * @throws IOException 
   */
  public void destroy() throws IOException {
    for (int i = 0; i < socketpool.length; i++) {
      if (socketpool[i] != null) {
        SocketAdapter adapter = (SocketAdapter) socketpool[i];
        adapter.destroy();
        //System.out.print("" );
      }
    }
    System.out.println("\ndestory success ....");
  }
  /**
   * 生成此连接池实现的对象
   * @throws UnknownHostException
   * @throws IOException 
   * @return ConnectionProvider 
   */
  public static ConnectionProvider newInstance() throws
      UnknownHostException, IOException {
    if (provider == null) {
      synchronized (object_lock) {
        if (provider == null) {
          provider = new MyConnectionProvider();
        }
      }
    }
    return provider;
  }
/**
 * 读取properties文件。
 * @param key
 * @param properName
 * @return
 */
  public static String readProperties(String key, String properName) {
		ResourceBundle rsrc = null;
		String value = "";
		rsrc = ResourceBundle.getBundle(properName);
		value = rsrc.getString(key);
		return value;
	}
}
```
Main
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/23258311#)[copy](http://blog.csdn.net/z69183787/article/details/23258311#)[print](http://blog.csdn.net/z69183787/article/details/23258311#)[?](http://blog.csdn.net/z69183787/article/details/23258311#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/282562/fork)
- package com.wondersgroup.stjt.mail.action;  
- 
- import java.io.BufferedReader;  
- import java.io.FileInputStream;  
- import java.io.FileNotFoundException;  
- import java.io.IOException;  
- import java.io.InputStreamReader;  
- import java.io.PrintWriter;  
- import java.net.InetAddress;  
- import java.net.InetSocketAddress;  
- import java.net.Socket;  
- import java.net.SocketAddress;  
- import java.net.UnknownHostException;  
- import java.sql.ResultSet;  
- import java.sql.SQLException;  
- import java.util.Properties;  
- import java.util.ResourceBundle;  
- 
- 
- import com.wondersgroup.framework.core.web.action.xwork.AbstractAjaxAction;  
- import com.wondersgroup.module.common.ExecuteSql;  
- import com.wondersgroup.stjt.util.StringUtil;  
- import com.wondersgroup.stjt.mail.util.*;  
- 
- publicclass ShmetromailAction extends AbstractAjaxAction {  
- //private String emailDomain = "@shmetro.com";
- //private String host = "222.66.3.199";
- //private int port = 8888;
- private String errMsg = "-";  
- private String okMsg = "+";  
- 
- public String getMailDetail() throws UnknownHostException {  
- 
- //获得用户名
- 
-         String curLoginName = (String) this.getRequest().getSession()  
-                 .getAttribute("login_name");  
-         String mailLoginName = "";  
-         String mailPwd = "";  
- 
- //取得用户email地址和email密码！
- 
-         ExecuteSql dealsql = new ExecuteSql();  
-         String sql = "select t.id,t.email from cs_user t where t.login_name='"+curLoginName+"'";  
- //System.out.println("sql");
- //dealsql.ExecuteSql(sql);
- //String mailLoginName="";
- int iuserid=0;  
- //String mailPwd="";
- try{  
-             ResultSet rs = dealsql.ExecuteDemandSql(sql);  
- 
- if(rs.next()){  
-                  iuserid = rs.getInt("ID");  
-                  mailLoginName=rs.getString("EMAIL");  
- //System.out.println(iuserid+"----"+mailLoginName);
-             }  
-             sql="select t.email_passwd from t_cs_user t where t.id='"+iuserid+"'";  
-              rs = dealsql.ExecuteDemandSql(sql);  
- if(rs.next()){  
- 
-                  mailPwd=rs.getString("email_passwd");  
- //System.out.println(iuserid+"--++--"+mailPwd);
-             }  
-              rs.close();  
-         }catch(Exception e){  
-             e.printStackTrace();  
-             createJSonData("{\"success\":false, \"results\": \"connectionerr\"}");  
- return AJAX;  
-         }finally{  
- 
- try {  
-                 dealsql.close();  
-             } catch (SQLException e) {  
- 
-                 e.printStackTrace();  
-                 createJSonData("{\"success\":false, \"results\": \"connectionerr\"}");  
- return AJAX;  
-             }  
-         }  
- 
- //判断地址，密码和发性
- 
- //Socket connection=null;
- if(StringUtil.isNull(mailLoginName)||!mailLoginName.matches("\\S+@shmetro\\.com")||StringUtil.isNull(mailPwd)){  
-             createJSonData("{\"success\":false, \"results\": \"errgetemailifo\"}");  
- return AJAX;  
-         }  
-         SocketAdapter connection = null;  
- int allMailCount=0;  
- int unReadCount=0;  
- // connect to server
- try {  
- //获取新的connection 
-             ConnectionProvider conpool=MyConnectionProvider.newInstance();  
- 
-             connection=conpool.getConnection();  
- 
- if(connection==null){  
-                 createJSonData("{\"success\":false, \"results\": \"errgetconnection\"}");  
- return AJAX;  
-             }  
- //so1.setKeepAlive(true);
- //connection=so1;
- 
-             BufferedReader input = new BufferedReader(new InputStreamReader(  
-                     connection.getInputStream()));// 接受
-             PrintWriter out = new PrintWriter(connection.getOutputStream(),  
- true/* autoFlush */);// 传输
- 
- 
-             String info = null;// 接受信息
- // read information from server
-             info = input.readLine();  
- 
- //输入邮件地址，密码
- 
-             BufferedReader in = new BufferedReader(new InputStreamReader(System.in));  
-             String sInput =null;  
-             String line = null;  
-             out.println("USER " + mailLoginName);  
-             System.out.println("USER " + mailLoginName);  
-             out.flush();  
-             line = input.readLine();  
-             System.out.println("line " + line);  
- if(StringUtil.isNull(line)||line.startsWith(errMsg)){  
-                 createJSonData("{\"success\":false, \"results\": \"errloginname\"}");  
- return AJAX;  
-             }  
- //System.out.println("1"+line);
-             out.println("PASS " + mailPwd);  
-             out.flush();  
-             line = input.readLine();  
- //System.out.println("line2 " + line);
- if(StringUtil.isNull(line)||line.startsWith(errMsg)){  
-                 createJSonData("{\"success\":false, \"results\": \"errpwd\"}");  
- return AJAX;  
-             }  
- //System.out.println("2"+line);
- //进入目录查找邮件信息。
- 
-             out.println("CHDIR inbox");  
-             line = input.readLine();  
- if(StringUtil.isNull(line)||line.startsWith(errMsg)){  
-                 createJSonData("{\"success\":false, \"results\": \"errchdir\"}");  
- return AJAX;  
-             }  
- //System.out.println("3"+line);
-             out.println("LIST");  
-             line = input.readLine();  
- if(StringUtil.isNull(line)||line.startsWith(errMsg)){  
-                 createJSonData("{\"success\":false, \"results\": \"errchdir\"}");  
- return AJAX;  
-             }  
- int i=0;  
- //获取邮件信息并统计
- 
- while((line = input.readLine()) != null){  
- //System.out.println(i++);
- int temp=getMailInfo(line);  
- if(temp!=-1){  
- if(temp!=-2){  
-                         allMailCount++;  
- if(temp==0){  
-                             unReadCount++;  
-                         }  
-                     }else{  
- 
- break;  
-                     }  
-                 }else{  
-                     createJSonData("{\"success\":false, \"results\": \"errmailinfo\"}");  
- return AJAX;  
-                 }  
- //System.out.println(line);
-             }  
- //System.out.println(allMailCount+"----"+unReadCount);
- 
-         } catch (UnknownHostException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
- 
-         } catch (IOException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
- 
-         } finally {  
- if (connection != null)  
- try {  
-                     connection.close();  
-                 } catch (Exception e) {  
- // TODO Auto-generated catch block
-                     e.printStackTrace();  
-                 }  
-         }  
- //返回。
- 
-         createJSonData("{\"success\":true, \"results\":{\"allMailCount\": \""+allMailCount+"\", \"unReadCount\": \""+unReadCount+"\"}}");  
- return AJAX;  
-     }  
- privatestaticint getMailInfo(String line) {  
- if(line.matches("\\S+\\s+\\S+\\s+\\d+")){  
-             String re=line.split("\\s+")[2];  
- return Integer.parseInt(re);  
-         }  
- //System.out.println("==="+line+"===");
- if(line.matches(".")){  
- //System.out.println("yes");
- return -2;  
-         }  
- return -1;  
-     }  
- publicstaticvoid main(String[] args) throws IOException{  
-     String errMsg = "-";  
-     String okMsg = "+";  
-     String mailLoginName = "limingmin@shmetro.com";  
-     String mailPwd = "1111";  
- 
-     SocketAdapter connection = null;  
- int allMailCount=0;  
- int unReadCount=0;  
- // connect to server
- try {  
- //获取新的connection 
-         ConnectionProvider conpool=MyConnectionProvider.newInstance();  
- 
-         connection=conpool.getConnection();  
- 
- if(connection==null){  
- 
-         }  
- //so1.setKeepAlive(true);
- //connection=so1;
- 
-         BufferedReader input = new BufferedReader(new InputStreamReader(  
-                 connection.getInputStream()));// 接受
-         PrintWriter out = new PrintWriter(connection.getOutputStream(),  
- true/* autoFlush */);// 传输
- 
- 
-         String info = null;// 接受信息
- // read information from server
- //info = input.readLine();
- 
- //输入邮件地址，密码
- 
- //BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
-         String sInput =null;  
-         String line = null;  
-         out.println("USER " + mailLoginName);  
-         System.out.println("USER " + mailLoginName);  
-         out.flush();  
-         line = input.readLine();  
-         System.out.println("line " + line);  
- if(StringUtil.isNull(line)||line.startsWith(errMsg)){  
- 
-         }  
- //System.out.println("1"+line);
-         out.println("PASS " + mailPwd);  
-         out.flush();  
-         line = input.readLine();  
- //System.out.println("line2 " + line);
- if(StringUtil.isNull(line)||line.startsWith(errMsg)){  
- 
-         }  
- //System.out.println("2"+line);
- //进入目录查找邮件信息。
- 
-         out.println("CHDIR inbox");  
-         line = input.readLine();  
- if(StringUtil.isNull(line)||line.startsWith(errMsg)){  
- 
-         }  
- //System.out.println("3"+line);
-         out.println("LIST");  
-         line = input.readLine();  
- if(StringUtil.isNull(line)||line.startsWith(errMsg)){  
- 
-         }  
- int i=0;  
- //获取邮件信息并统计
- 
- while((line = input.readLine()) != null){  
- //System.out.println(i++);
- int temp=getMailInfo(line);  
- if(temp!=-1){  
- if(temp!=-2){  
-                     allMailCount++;  
- if(temp==0){  
-                         unReadCount++;  
-                     }  
-                 }else{  
- 
- break;  
-                 }  
-             }else{  
- 
-             }  
- //System.out.println(line);
-         }  
- //System.out.println(allMailCount+"----"+unReadCount);
- 
-     } catch (UnknownHostException e) {  
- // TODO Auto-generated catch block
-         e.printStackTrace();  
- 
-     } catch (IOException e) {  
- // TODO Auto-generated catch block
-         e.printStackTrace();  
-         System.out.println("111111111111111111111111111111111111111");  
-     } finally {  
- if (connection != null)  
- try {  
-                 connection.close();  
-             } catch (Exception e) {  
- // TODO Auto-generated catch block
-                 e.printStackTrace();  
-             }  
-     }  
- 
-   }  
- 
- }  
