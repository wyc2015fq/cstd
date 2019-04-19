# Spring mvc + pushlet实现向特定用户实时推送信息 - 零度的博客专栏 - CSDN博客
2016年07月18日 11:16:31[零度anngle](https://me.csdn.net/zmx729618)阅读数：3239
环境:pushlet-2.0.4 
所遇问题： 
1.如何向特定用户推送 
参考网上资料：[http://tonl.iteye.com/blog/1398995](http://tonl.iteye.com/blog/1398995)
**首先**：修改ajax-pushlet-client.js，在PL对象后加入userId。
```
var PL = {
    NV_P_FORMAT: 'p_format=xml-strict',
    NV_P_MODE: 'p_mode=pull',
    pushletURL: null,
    webRoot: null,
    sessionId: null,
    STATE_ERROR: -2,
    STATE_ABORT: -1,
    STATE_NULL: 1,
    STATE_READY: 2,
    STATE_JOINED: 3,
    STATE_LISTENING: 3,
    state: 1,
    userId: '',//添加的userId变量
```
**其次**：修改pushlet源码，nl.justobjects.pushlet.core.SessionManager，将createSession方法修改如下：
```
/**
     * Create new Session (but add later).
     */
    public Session createSession(Event anEvent) throws PushletException {
        // Trivial
        //return Session.create(createSessionId()); //原写法
        return Session.create(anEvent.getField("userId", "visitor"));//修改后的写法
    }
```
这是我们就获得的session的id就是我们传送过来的登陆用户userId,getField的第二个参数是当得不到请求参数userId的值的时候设定的默认值。 
**最后**：对修改过的源码进行编译打包
编择：直接到下载的pushlet包下build 
打包：通过jar命令 
2.如何在配置的推送源中获取service对象 
参考：[http://blog.csdn.net/zollty/article/details/8710911](http://blog.csdn.net/zollty/article/details/8710911)
因为pushlet是在服务端做的轮询，并不经web容器，所以不能自动注入，要手动获取，方法如下：
```
applicationContext = sessions.length > 0 && applicationContext == null ?
                    ContextLoaderListener.getCurrentWebApplicationContext()
                    : applicationContext;
renwuService = sessions.length > 0 && renwuService == null ?
                    (IRenwuService) applicationContext.getBean("renwuServiceImpl")
                    : renwuService;
```
注：IRenwuService 
 为消息接口，RenwuServiceImpl为接口实现类
3.Spring 初始化的bean名称是什么 
看到2中getBean("renwuServiceImpl"), Spring容器初始化bean时，所有bean名称都以小写字母开头（这里从控制台也可以看出所有初始化bean的名称），所以bean名为renwuServiceImpl。
4.如果推信息为中文如何处理 
如查推送的信息为中文，前端JS不会返回信息，但从控制台可以看出，推送源已执行，但会自动将sessionId移除。
解决方法： 
new String("中文问题".getBytes("UTF-8"),"ISO-8859-1"); 
5.用Jquery实现无刷新框架要注的JS代码位置 
这里我是用 Jquery post方法在页面初始化时进行动态加载main页面，所以 
```
<script type="text/javascript">
<!--
  //pushlet
  if(PL.sessionId == null){
      PL.userId = '${userId}';
      PL._init();
      PL.joinListen('/pms/bxService');//事件标识 在数据源中引用
      function onData(event){
            console.log("sessionID==" + PL.sessionId + "====" + event.get('msg_' + '${userId}'));
      }
  }
//-->
</script>
```
要放入最后回载的main页，不能放入在他之前加载的页面 
6.关于key的取值问题
`event.setField(key, value)`
这里的key不能是数字（包括数字字符串），否则同样会出现4的问题 
=============================华丽的分隔线=========================
完整配置代码： 
**步骤一**： 创建一个类（事件推送源类）
```
package com.tony.demo.pushlet;
       
import java.io.Serializable;
       
import org.springframework.context.ApplicationContext;
import org.springframework.stereotype.Controller;
import org.springframework.web.context.ContextLoaderListener;
import com.tony.jrpms.service.IRenwuService;
import nl.justobjects.pushlet.core.Event;
import nl.justobjects.pushlet.core.EventPullSource;
import nl.justobjects.pushlet.core.Session;
import nl.justobjects.pushlet.core.SessionManager;
       
/**
 * @author Tony
 * @createDatime 2013-11-28 下午1:42:21
 */
@Controller
public class PmsEventPullSources implements Serializable {
       
    /**
     *
     */
    private static final long serialVersionUID = 8397335113632699297L;
           
    private static ApplicationContext applicationContext;
    private static IRenwuService renwuService;
       
    public static class BaoxiuEvent extends EventPullSource{
       
          @Override
          protected long getSleepTime() {
       
              return 1000; //刷新时间
       
          }
       
          @Override
          protected Event pullEvent() {
       
            Event event = Event.createDataEvent("/pms/bxService");//  事件标识 // 注意：此处”/pms/bxService”将对应页面js代码中的PjoinListen中的参数
       
            //  获取当前登陆用户Id(加入事件订阅的用户)
            Session[] sessions = SessionManager.getInstance().getSessions();
                   
            applicationContext = sessions.length > 0 && applicationContext == null ?
                    ContextLoaderListener.getCurrentWebApplicationContext()
                    : applicationContext;
            renwuService = sessions.length > 0 && renwuService == null ?
                    (IRenwuService) applicationContext.getBean("renwuServiceImpl")
                    : renwuService;
                   
            //查询当前用户的任务
            for(int i=0; i<sessions.length; i++){
                       
                //查询当前登录用户的报修任务
                int bxCount = renwuService.totalRenwu(Integer.parseInt(sessions<i>.getId()));
                event.setField("msg_" + sessions<i>.getId(), bxCount);//  封装参数
                       
            }
            return event;
       
          }
       
      }
}
```
**步骤二**： 配置文件（ sources.properties ）
`source1=com.tony.demo.pushlet.PmsEventPullSources$BaoxiuEvent`
将原有的source*全部注释或删除掉
**步骤三**： 配置文件（ web.xml ） 
```
<!--  pushlet servlet 配置-->
    <servlet>
        <servlet-name>pushlet</servlet-name>
        <servlet-class>nl.justobjects.pushlet.servlet.Pushlet</servlet-class>
        <load-on-startup>1</load-on-startup>
    </servlet>
           
    <!--  Define the Servlet Mappings. -->
       
    <!--  The pushlet -->
    <servlet-mapping>
        <servlet-name>pushlet</servlet-name>
        <url-pattern>/pushlet.srv</url-pattern>
    </servlet-mapping>
```
**步骤四**： jsp文件中js写法
```
<script type="text/javascript">
<!--
  //pushlet
  if(PL.sessionId == null){
      PL.userId = '${userId}';
      PL._init();
      PL.joinListen('/pms/bxService');//事件标识 在数据源中引用
      function onData(event){
            console.log("sessionID==" + PL.sessionId + "====" + event.get('msg_' + '${userId}'));
      }
  }
//-->
</script>
```
注：${userId}为当前登录用户的ID
