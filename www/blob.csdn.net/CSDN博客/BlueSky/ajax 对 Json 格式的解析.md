# ajax 对 Json 格式的解析 - BlueSky - CSDN博客
2016年01月12日 15:02:38[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：7119
**Json 的介绍：**
JSON 可以将 JavaScript 对象中表示的一组数据转换为字符串，然后就可以在函数之间轻松地传递这个字符串，或者在异步应用程序中将字符串从 Web 客户机传递给服务器端程序。
按照最简单的形式，可以用下面这样的 JSON 表示名称/值对：
{ "firstName": "Brett" }
这样的键/值对也可以是多个：
{"name":"aaa","sex":"男","age":"20"}
从语法方面来看，这与名称/值对相比并没有很大的优势，但是在这种情况下 JSON 更容易使用，而且可读性更好。
值的数组
当需要表示一组值时，JSON 不但能够提高可读性，而且可以减少复杂性。
如果使用 JSON，就只需将多个带花括号的记录分组在一起：
{ "people": [
  { "firstName": "Brett", "lastName":"McLaughlin", "email": "111111" },
  { "firstName": "Jason", "lastName":"Hunter", "email": "22222" },
  { "firstName": "Elliotte", "lastName":"Harold", "email": "33333" }
]}
可以使用相同的语法表示多个值（每个值包含多个记录）：
{ "programmers": [
  { "firstName": "Brett", "lastName":"McLaughlin", "email": "3333" },
  { "firstName": "Jason", "lastName":"Hunter", "email": "1222" },
  { "firstName": "Elliotte", "lastName":"Harold", "email": "3333" }
 ],
"authors": [
  { "firstName": "Isaac", "lastName": "Asimov", "genre": "science fiction" },
  { "firstName": "Tad", "lastName": "Williams", "genre": "fantasy" },
  { "firstName": "Frank", "lastName": "Peretti", "genre": "christian fiction" }
 ],
"musicians": [
  { "firstName": "Eric", "lastName": "Clapton", "instrument": "guitar" },
  { "firstName": "Sergei", "lastName": "Rachmaninoff", "instrument": "piano" }
 ]
}
Json也可以这样写：
`{ "listaccountsresponse" : { "count":1 ,"account" : [ {"id":5,"name":"xian1","accounttype":0,"domainid"``:2,"domain":"Server","receivedbytes":649444,"sentbytes":175467975,"vmlimit":"20","vmtotal":2,"vmavailable"``:"18","iplimit":"20","iptotal":1,"ipavailable":"19","volumelimit":"20","volumetotal":2,"volumeavailable"``:"18","snapshotlimit":"20","snapshottotal":0,"snapshotavailable":"20","templatelimit":"20","templatetotal"``:0,"templateavailable":"20","vmstopped":0,"vmrunning":2,"state":"enabled","user":[{"id":5,"username"``:"xian1","firstname":"Eric","lastname":"Tang","email":"Wang-Ngai.Tang@pccw.com","created":"2012-03-22T09``:36:44+0800","state":"enabled","account":"xian1","accounttype":0,"domainid":2,"domain":"Server","timezone"`
```
:"Asia/Shanghai"}]}
 ] } }
```
**ajax对Json格式数据的解析**：
1.对简单json格式的解析：
{ "firstName": "Brett" }
写道
$.ajax({
url:"这里是你要请求的地址",
data:{"id":id}, //以键/值对的形式
async : false,
dataType : "json",
success : function(data) {
alert("firstName = " + data.firstName);
}
});
 注：这里的data是你从后台穿过来的Json字符传串。
后台写法：
Java代码  ![收藏代码](http://xiaoxiong-it.iteye.com/images/icon_star.png)
- response.setCharacterEncoding("utf-8");  
-         response.setContentType("text/json;charset=UTF-8");  
-         PrintWriter out = null;  
-         String count = "<span style="color: #000000;">{ "firstName": "Brett" }</span>  
- 
- ;  
- try {  
-             out = response.getWriter();  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
-         out.print(count);  
-         out.flush();  
 这样就能在前台页面弹出提示框：firstName = Brett。
2.多个键/值对和上面的情况一样
3.解析Json如下数组：
{
  { "firstName": "Brett", "lastName":"McLaughlin", "email": "111111" },
  { "firstName": "Jason", "lastName":"Hunter", "email": "22222" },
  { "firstName": "Elliotte", "lastName":"Harold", "email": "33333" }
}
Js代码  ![收藏代码](http://xiaoxiong-it.iteye.com/images/icon_star.png)
- $.ajax({  
- url:"这里是你要请求的地址",  
- data:{"id":id}, //以键/值对的形式
- async : false,  
- dataType : "json",  
- success : function(data) {  
- for(int i = 0; i < data.length; i++) { //循环后台传过来的Json数组
- var datas = data[i];  
-      alert(datas.firstName);  
-      alert(datas.lastName);  
-      alert(datas.email);  
- }  
- }  
- });  
4.解析如下Json数据
{ "programmers": [
  { "firstName": "Brett", "lastName":"McLaughlin", "email": "3333" },
  { "firstName": "Jason", "lastName":"Hunter", "email": "1222" },
  { "firstName": "Elliotte", "lastName":"Harold", "email": "3333" }
 ],
"authors": [
  { "firstName": "Isaac", "lastName": "Asimov", "genre": "science fiction" },
  { "firstName": "Tad", "lastName": "Williams", "genre": "fantasy" },
  { "firstName": "Frank", "lastName": "Peretti", "genre": "christian fiction" }
 ],
"musicians": [
  { "firstName": "Eric", "lastName": "Clapton", "instrument": "guitar" },
  { "firstName": "Sergei", "lastName": "Rachmaninoff", "instrument": "piano" }
 ]
}
Js代码  ![收藏代码](http://xiaoxiong-it.iteye.com/images/icon_star.png)
- $.ajax({  
- url:"这里是你要请求的地址",  
- data:{"id":id}, //以键/值对的形式
- async : false,  
- dataType : "json",  
- success : function(data) {  
- var pro = data.programmers; // pro为programmers的一个数组
- for(int i = 0; i < pro.length; i++) { //循环后台传过来的Json数组
- var datas = pro[i];  
-      alert(pro.firstName);  
-      alert(pro.lastName);  
-      alert(pro.email);  
- }  
- var aut = data.authors; // aut为authors的一个数组
- for(int i = 0; i < aut.length; i++) { //循环后台传过来的Json数组
- var datas = aut[i];  
-      alert(aut.firstName);  
-      alert(aut.lastName);  
-      alert(aut.genre);  
- }  
- var mus = data.musicians; // aut为authors的一个数组
- for(int i = 0; i < mus.length; i++) { //循环后台传过来的Json数组
- var datas = mus[i];  
-      alert(mus.firstName);  
-      alert(mus.lastName);  
-      alert(mus.instrument);  
- }  
- 
- }  
- });  
 5.
`{ "listaccountsresponse" : { "count":1 ,"account" : [ {"id":5,"name":"xian1","accounttype":0,"domainid"``:2,"domain":"Server","receivedbytes":649444,"sentbytes":175467975,"vmlimit":"20","vmtotal":2,"vmavailable"``:"18","iplimit":"20","iptotal":1,"ipavailable":"19","volumelimit":"20","volumetotal":2,"volumeavailable"``:"18","snapshotlimit":"20","snapshottotal":0,"snapshotavailable":"20","templatelimit":"20","templatetotal"``:0,"templateavailable":"20","vmstopped":0,"vmrunning":2,"state":"enabled","user":[{"id":5,"username"``:"xian1","firstname":"Eric","lastname":"Tang","email":"Wang-Ngai.Tang@pccw.com","created":"2012-03-22T09``:36:44+0800","state":"enabled","account":"xian1","accounttype":0,"domainid":2,"domain":"Server","timezone"`
```
:"Asia/Shanghai"}]}
 ] } }
```

Js代码  ![收藏代码](http://xiaoxiong-it.iteye.com/images/icon_star.png)
- $.ajax({  
- url:"这里是你要请求的地址",  
- data:{"id":id}, //以键/值对的形式
- async : false,  
- dataType : "json",  
- success : function(data) {  
- var accounts = data.listaccountsresponse.account; //取到“account”中的数据
- for(int i = 0; i < accounts.length; i++) { //循环后台传过来的Json数组
- var datas = accounts[i];  
-      alert(datas.id);  
-      alert(datas.name);  
-      alert(datas.accounttype);  
-      ..........  
- }  
- }  
- });  
-----这里是自己写的一个后台传值----------------------------------------------------------------------------------------------------------
Js代码  ![收藏代码](http://xiaoxiong-it.iteye.com/images/icon_star.png)
- @RequestMapping(value="/applyVm/listApplyVmCount", method={RequestMethod.GET, RequestMethod.POST})  
- publicvoid listApplyVmCount(HttpServletRequest request, HttpServletResponse response) {  
-         response.setCharacterEncoding("utf-8");  
-         response.setContentType("text/json;charset=UTF-8");  
-         PrintWriter out = null;  
-         String count = "";  
- try {  
-             out = response.getWriter();  
-             Map<String, Integer> map = applyVmService.findApplyVmCount();  
-             Gson gson = new Gson();  
-             count = gson.toJson(map);  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
-         out.print(count);  
-         out.flush();  
-     }   
