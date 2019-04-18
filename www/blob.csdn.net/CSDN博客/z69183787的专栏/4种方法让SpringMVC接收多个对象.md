# 4种方法让SpringMVC接收多个对象 - z69183787的专栏 - CSDN博客
2018年10月16日 18:23:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：89
个人分类：[SpringMvc](https://blog.csdn.net/z69183787/article/category/2176803)
[https://blog.csdn.net/lutinghuan/article/details/46820023](https://blog.csdn.net/lutinghuan/article/details/46820023)
问题背景： 
我要在一个表单里同时一次性提交多名乘客的个人信息到SpringMVC，前端HTML和SpringMVC Controller里该如何处理？
第1种方法：表单提交，以字段数组接收；
第2种方法：表单提交，以BeanListModel接收；
第3种方法：将Json对象序列化成Json字符串提交，以List接收；
第4种方法：将表单对象序列化成Json字符串提交，以List接收； 
第4种方法其实是第3种方法的升级，就是将表单转成Json对象，再转成Json字符串提交； 
然而，第4种方法还不支持含有多选控件表单的提交，故应该还有第5种加强版的方法。
以上4种方法都共用同一个User实体类，代码如下：
public class User {
    private Integer id;
    private String name;
    private String pwd;
    @Override
    public String toString() {
        return "User{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", pwd='" + pwd + '\'' +
                '}';
    }
    // .......后面还有getter、setter方法，省略了
 }
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
第1种方法：表单提交，以字段数组接收 
HTML代码如下：
    <form action="/user/submitUserList_1" method="post">
        ID:<input type="text" name="id"><br/>
        Username:<input type="text" name="name"><br/>
        Password:<input type="text" name="pwd"><br/><br/>
        ID:<input type="text" name="id"><br/>
        Username:<input type="text" name="name"><br/>
        Password:<input type="text" name="pwd"><br/><br/>
        <input type="submit" value="submit">
    </form>
1
2
3
4
5
6
7
8
9
10
Java代码如下：
    @RequestMapping(value = "/submitUserList_1", method ={RequestMethod.POST})
    @ResponseBody
    public String submitUserList_1(HttpServletResponse response,Integer[] id, String[] name, String[] pwd)
                            throws Exception{
        String result = "";
        if(id == null || id.length <= 0){ return "No any ID.中文"; }
        List<User> userList = new ArrayList<User>();
        for (int i = 0; i < id.length; i++ ) {
            User user = new User();
            user.setId(id[i]);
            user.setName(name[i]);
            user.setPwd(pwd[i]);
            userList.add(user);
        }
        result = this.showUserList(userList);
        return result;
    }
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
第2种方法：表单提交，以BeanListModel接收 
HTML代码如下：
    <form action="/user/submitUserList_2" method="post">
        ID:<input type="text" name="users[0].id"><br/>
        Username:<input type="text" name="users[0].name"><br/>
        Password:<input type="text" name="users[0].pwd"><br/><br/>
        ID:<input type="text" name="users[2].id"><br/>
        Username:<input type="text" name="users[2].name"><br/>
        Password:<input type="text" name="users[2].pwd"><br/><br/>
        <input type="submit" value="Submit">
    </form>
1
2
3
4
5
6
7
8
9
10
Java代码： 
除了刚才公用的User类，还要封装一个User的容器类UserModel：
public class UserModel {
    private List<User> users;
    public List<User> getUsers() {
        return users;
    }
    public void setUsers(List<User> users) {
        this.users = users;
    }
    public UserModel(List<User> users) {
        super();
        this.users = users;
    }
    public UserModel() {
        super();
    }
}
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
SpringMVC Controller方法：
    @RequestMapping(value = "/submitUserList_2", method ={RequestMethod.POST})
    @ResponseBody
    public String submitUserList_2(UserModel users)
            throws Exception{
        String result = "";
        List<User> userList = users.getUsers();
        if(userList == null || userList.size() <= 0){ return "No any ID.中文"; }
        result = this.showUserList(userList);
        return result;
    }
1
2
3
4
5
6
7
8
9
10
第3种方法：将Json对象序列化成Json字符串提交，以List接收 
HTML代码：
<head>
    <title>submitUserList_3</title>
    <meta http-equiv="content-type" content="text/html; charset=utf-8">
    <script language="JavaScript" src="/js/jquery.min.js" ></script>
    <script language="JavaScript" src="/js/jquery.json.min.js" ></script>
    <script type="text/javascript" language="JavaScript">
        function submitUserList_3() {alert("ok");
            var customerArray = new Array();
            customerArray.push({id: "1", name: "李四", pwd: "123"});
            customerArray.push({id: "2", name: "张三", pwd: "332"});
            $.ajax({
                url: "/user/submitUserList_3",
                type: "POST",
                contentType : 'application/json;charset=utf-8', //设置请求头信息
                dataType:"json",
                //data: JSON.stringify(customerArray),    //将Json对象序列化成Json字符串，JSON.stringify()原生态方法
                data: $.toJSON(customerArray),            //将Json对象序列化成Json字符串，toJSON()需要引用jquery.json.min.js
                success: function(data){
                    alert(data);
                },
                error: function(res){
                    alert(res.responseText);
                }
            });
        }
    </script>
</head>
<body>
    <h1>submitUserList_3</h1>
    <input id="submit" type="button" value="Submit" onclick="submitUserList_3();">
</body>
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
Java代码：
    @RequestMapping(value = "/submitUserList_3", method ={RequestMethod.POST})
    @ResponseBody
    public String submitUserList_3(@RequestBody List<User> users)
            throws Exception{
        String result = "";
        if(users == null || users.size() <= 0){ return "No any ID.中文"; }
        result = this.showUserList(users);
        return result;
    }
1
2
3
4
5
6
7
8
9
第4种方法：将表单对象序列化成Json字符串提交，以List接收 
HTML代码：
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
        "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
    <title>submitUserList_4</title>
    <meta http-equiv="content-type" content="text/html; charset=utf-8">
    <script language="JavaScript" src="/js/jquery.min.js" ></script>
    <script type="text/javascript" language="JavaScript">
        //将表单序列化成json格式的数据(但不适用于含有控件的表单，例如复选框、多选的select)
        (function($){
            $.fn.serializeJson = function(){
                var jsonData1 = {};
                var serializeArray = this.serializeArray();
                // 先转换成{"id": ["12","14"], "name": ["aaa","bbb"], "pwd":["pwd1","pwd2"]}这种形式
                $(serializeArray).each(function () {
                    if (jsonData1[this.name]) {
                        if ($.isArray(jsonData1[this.name])) {
                            jsonData1[this.name].push(this.value);
                        } else {
                            jsonData1[this.name] = [jsonData1[this.name], this.value];
                        }
                    } else {
                        jsonData1[this.name] = this.value;
                    }
                });
                // 再转成[{"id": "12", "name": "aaa", "pwd":"pwd1"},{"id": "14", "name": "bb", "pwd":"pwd2"}]的形式
                var vCount = 0;
                // 计算json内部的数组最大长度
                for(var item in jsonData1){
                    var tmp = $.isArray(jsonData1[item]) ? jsonData1[item].length : 1;
                    vCount = (tmp > vCount) ? tmp : vCount;
                }
                if(vCount > 1) {
                    var jsonData2 = new Array();
                    for(var i = 0; i < vCount; i++){
                        var jsonObj = {};
                        for(var item in jsonData1) {
                            jsonObj[item] = jsonData1[item][i];
                        }
                        jsonData2.push(jsonObj);
                    }
                    return JSON.stringify(jsonData2);
                }else{
                    return "[" + JSON.stringify(jsonData1) + "]";
                }
            };
        })(jQuery);
        function submitUserList_4() {alert("ok");
            var jsonStr = $("#form1").serializeJson();
            //console.log("jsonStr:\r\n" + jsonStr);
            //alert(jsonStr);
            $.ajax({
                url: "/user/submitUserList_4",
                type: "POST",
                contentType : 'application/json;charset=utf-8', //设置请求头信息
                dataType:"json",
                data: jsonStr,
                success: function(data){
                    alert(data);
                },
                error: function(res){
                    alert(res.responseText);
                }
            });
        }
    </script>
</head>
<body>
    <h1>submitUserList_4</h1>
    <form id="form1">
        ID:<input type="text" name="id"><br/>
        Username:<input type="text" name="name"><br/>
        Password:<input type="text" name="pwd"><br/><br/>
        ID:<input type="text" name="id"><br/>
        Username:<input type="text" name="name"><br/>
        Password:<input type="text" name="pwd"><br/><br/>
        <input type="button" value="submit" onclick="submitUserList_4();">
    </form>
</body>
</html>
