# Jquery读取返回的JSON数据 - 左直拳的马桶_日用桶 - CSDN博客
2012年09月28日 16:41:15[左直拳](https://me.csdn.net/leftfist)阅读数：1878标签：[jquery																[json																[function																[string																[c#																[url](https://so.csdn.net/so/search/s.do?q=url&t=blog)](https://so.csdn.net/so/search/s.do?q=c#&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=json&t=blog)](https://so.csdn.net/so/search/s.do?q=jquery&t=blog)
个人分类：[Web](https://blog.csdn.net/leftfist/article/category/674421)
1、从asmx文件返回，其实是字符串，需要特别处理
jquery：
```java
function GetInfo() {
            $.ajax({
                type: "POST"
                , contentType: "application/json"
                , url: "pub.asmx/GetInfo"
                , data: "{'pId':'" + _PublicId + "','userId':'" + _UserId + "'}"
                , dataType: 'json'
                , success: function(data) {
                    var json = eval('(' + data.d + ')');
                    alert(json.flower + "," + json.egg + "," + json.isvote + "," + json.iskeep);
                }
            });
        }
```
C#：
```
[WebMethod]
    public string GetInfo(int pId,int userId)
    {
	……
        return String.Format(@"{{""flower"":""{0}"",egg:""{1}"",isvote:""{2}"",iskeep:""{3}""}}"
            , dr["flower"]
            , dr["egg"]
            , Convert.ToBoolean(dr["isvote"])
            , Convert.ToBoolean(dr["iskeep"]));
    }
```
参考文章
[http://laputaliya.blog.51cto.com/751941/536858](http://laputaliya.blog.51cto.com/751941/536858)
