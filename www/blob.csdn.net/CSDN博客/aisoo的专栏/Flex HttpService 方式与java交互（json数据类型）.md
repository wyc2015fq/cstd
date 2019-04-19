# Flex HttpService 方式与java交互（json数据类型） - aisoo的专栏 - CSDN博客
2010年08月17日 16:54:00[aisoo](https://me.csdn.net/aisoo)阅读数：2929
mxml 代码：
<?xml version="1.0" encoding="utf-8"?>
<mx:Application xmlns:mx="[http://www.adobe.com/2006/mxml](http://www.adobe.com/2006/mxml)" layout="absolute" applicationComplete="getData.send()">
<mx:Script>
   <![CDATA[
    import mx.rpc.events.ResultEvent;
    import mx.controls.Alert;
    import com.adobe.serialization.json.*;
    [Bindable]
    private var jsonObj:Object
    internal function getUser(event:ResultEvent):void{
      var re:String=event.result.toString();
       jsonObj=JSON.decode(re);
    }
   ]]>
</mx:Script>
<mx:HTTPService id="getData" url="[http://localhost:8080/search/getusers](http://localhost:8080/search/getusers)" useProxy="false" result="getUser(event)" showBusyCursor="true"/>
<mx:Panel layout="absolute" left="10" top="10" bottom="10" right="10" title="用户记录表" fontSize="12">
   <mx:DataGrid left="10" bottom="10" right="10" top="10" dataProvider="{jsonObj}">
    <mx:columns>
     <mx:DataGridColumn headerText="姓名" dataField="name" sortable="false" draggable="false"/>
     <mx:DataGridColumn headerText="年龄" dataField="age" draggable="false"/>
     <mx:DataGridColumn headerText="家庭住址" dataField="address" draggable="false"/>
     <mx:DataGridColumn headerText="电子邮箱" dataField="email" draggable="false"/>
    </mx:columns>
   </mx:DataGrid>
</mx:Panel>
</mx:Application>
java代码
package mxr.flex;
import java.io.IOException;
import java.util.*;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import net.sf.json.JSONArray;
import mxr.flex.model.User;
public class DataSource extends HttpServlet{
@Override
protected void service(HttpServletRequest req, HttpServletResponse resp)
    throws ServletException, IOException {
   String re=DataSource.getAllUsers();
   resp.setCharacterEncoding("utf-8");
   resp.getWriter().write(re);
   System.out.println("aaaaaaaaaa");
   //req.setAttribute("users", re);
}
public static void main(String[] args) {
}
public static String getAllUsers(){
   String users="null";
   List l=new ArrayList();
   for(int i=0;i<15;i++){
    User u=new User();
    u.setName("user-->"+(i+1));
    u.setAddress("中国.北京.海淀.上地"+(i+1)+"号");
    u.setAge(18+i);
    u.setEmail(i+1+"@digitalchina.com");
    l.add(u);
   }
   JSONArray jsonList = JSONArray.fromObject(l);
   users=jsonList.toString();
   return users;
  }
}
