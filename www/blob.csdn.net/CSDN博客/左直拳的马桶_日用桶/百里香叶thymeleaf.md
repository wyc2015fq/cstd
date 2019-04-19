# 百里香叶thymeleaf - 左直拳的马桶_日用桶 - CSDN博客
2019年01月18日 19:58:34[左直拳](https://me.csdn.net/leftfist)阅读数：140
百香叶是模板。
**1、**既然是模板，就**要经过服务器处理**，才能起作用。当然浏览器也能显示，但里面那些动态标记会原封不动地呈现出来。
经过服务器处理是什么意思？就是通过路由，访问控制器，来返回页面咯。
```java
@Controller
public class TyphoonController {
    @Autowired
    private ITyphoonMapper typhoonMapper;
    @RequestMapping(value = {"/res/typhoon/history",""}, method = { RequestMethod.GET, RequestMethod.POST })
    public String history(Model model) {//返回页面
        Calendar cal = Calendar.getInstance();
        model.addAttribute("thisYear", cal.get(Calendar.YEAR));
        return "res/typhoon/history";//返回页面路径
    }
}
```
**2、模板文件存放的位置**
要放在指定的位置，否则找不到。
在哪指定？
配置文件啊，以application.yml为例，有
```
# 指定静态资源的路径
    resources:
        static-locations: classpath:/static/,classpath:/views/
    # thymeleaf配置
    thymeleaf:
        mode: LEGACYHTML5   # HTML5 LEGACYHTML5
        encoding: UTF-8
        content-type: text/html
        cache: false    # 开发时关闭缓存,不然没法看到实时页面
        prefix: classpath:/templates/   #喏，看这里，就是这里了
```
就是这个地方了： `prefix: classpath:/templates/`
如果有多个存放路径，比如静态文件这样，`static-locations: classpath:/static/,classpath:/views/`，行吗？
好像不大利索。似乎只有第一个有效。如果想用第二个路径，要在代码中指定，麻烦得很。原本我是想将有关的JS和CSS都和模板放一起的，组件化嘛，但好像路径总不对，忒麻烦，就放弃了。
**3、模板里的路径**
像JS，CSS，这些都需要路径。我觉得可以用相对根目录的路径，语法为`@{}`。
```
<script type="text/javascript" src="../../../static/res/typhoon.js" th:src="@{/res/typhoon.js}"></script>
```
这样子写了以后，页面从服务器端出来，就是后面一个设置；如果用浏览器直接打开该文件，生效的就是前面一个路径，护法去头皮，两全其美。
`@{/res/typhoon.js}` 这个路径，就是从`根目录/res/typhoon.js`
4、模板里的变量
变量从服务器代码里传过来
```
<script th:inline="javascript"> //这句必须的
    (function (){
        var now = (/*[[${thisYear}]]*/'') * 1;//thisYear从服务器传过来
        var objYear = document.getElementById("ddlYear");
        for (var i = now; i >= 1949; i--){
            var sContent = i;
            var varItem = new Option(sContent, sContent);
            objYear.options.add(varItem);
        }
    })();
</script>
```
相应的服务器代码
```java
@RequestMapping(value = {"/res/typhoon/history",""}, method = { RequestMethod.GET, RequestMethod.POST })
    public String history(Model model) {//返回页面
        Calendar cal = Calendar.getInstance();
        model.addAttribute("thisYear", cal.get(Calendar.YEAR));//通过model传递到前端
        return "res/typhoon/history";
    }
```
