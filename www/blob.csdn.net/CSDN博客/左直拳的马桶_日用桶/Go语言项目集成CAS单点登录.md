# Go语言项目集成CAS单点登录 - 左直拳的马桶_日用桶 - CSDN博客
2018年11月29日 17:55:57[左直拳](https://me.csdn.net/leftfist)阅读数：743
网上有高手开源了一个网盘项目：[蓝眼云盘](https://github.com/eyebluecn/tank)，我一看还行，版权也很宽松，是[MIT](https://baike.baidu.com/item/MIT%E8%AE%B8%E5%8F%AF%E8%AF%81/6671281?fr=aladdin)，就用到了项目里面去。
有个问题就是我们项目采用了CAS作为单点登录，而这个蓝眼云盘有自己的一套登录机制。需要改造一下，将单点登录也集成到云盘中来。
蓝眼云盘项目服务器端是用GO语言开发的，前端则用了VUE.JS框架，这两样我都没接触过。而且成品的前端还用了webpack进行打包，很难看清。webpack也没接触过。一片空白啊。
赶鸭子上架，花了约2周的时间，尽管还不完美，但终于像点样子了。
思路或步骤记录如下：
**一、要有一个CAS for Go语言的客户端**，这属于准备工作
这个客户端做什么用呢？接管和控制用户请求，发现未经过CAS认证就转向CAS。
这个CAS客户端当然也是golang开发的。
源代码请狠狠点击[这里](https://github.com/go-cas/cas)
下载，编译以后，可以运行_examples/cas_chi.go（命令行方式下，go run cas_chi.go）,就能看到这个客户端的效果了：在浏览器输入 localhost:9999，会先跳到CAS的登录页。
但是！官方代码有个坑，就是登录以后，会出现重定向次数过多的错误。有人修复了这个坑。修改过的_examples/cas_chi.go代码如下：
```
package main
import (
	"bytes"
	"fmt"
	"html/template"
	"log"
	"net/http"
	"net/url"
	"github.com/go-chi/chi"
	"github.com/shenshouer/cas"
)
var casURL = "http://192.168.0.22:8080/cas2/" //单点登录地址
type templateBinding struct {
	Username   string
	Attributes cas.UserAttributes
}
func main() {
	url, _ := url.Parse(casURL)
	client := cas.NewClient(&cas.Options{URL: url})
	root := chi.NewRouter()
	root.Use(client.Handler)
	//这句为新增代码
	server := &http.Server{
		Addr:    ":9999",
		Handler: client.Handle(root),
	}
	root.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		w.Header().Add("Content-Type", "text/html")
		tmpl, err := template.New("index.html").Parse(index_html)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			fmt.Fprintf(w, error_500, err)
			return
		}
		binding := &templateBinding{
			Username:   cas.Username(r),
			Attributes: cas.Attributes(r),
		}
		html := new(bytes.Buffer)
		if err := tmpl.Execute(html, binding); err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			fmt.Fprintf(w, error_500, err)
			return
		}
		html.WriteTo(w)
	})
	//if err := http.ListenAndServe(":9999", root); err != nil { //屏蔽原先这句代码
		if err := server.ListenAndServe(); err != nil {//改为这句
			log.Fatal(err)
			//fmt.Println("error!")
		}
	//}
}
const index_html = `<!DOCTYPE html>
<html>
  <head>
    <title>Welcome {{.Username}}</title>
  </head>
  <body>
    <h1>Welcome {{.Username}} <a href="/logout">Logout</a></h1>
    <p>Your attributes are:</p>
    <ul>{{range $key, $values := .Attributes}}
      <li>{{$len := len $values}}{{$key}}:{{if gt $len 1}}
        <ul>{{range $values}}
          <li>{{.}}</li>{{end}}
        </ul>
      {{else}} {{index $values 0}}{{end}}</li>{{end}}
    </ul>
  </body>
</html>
`
const error_500 = `<!DOCTYPE html>
<html>
  <head>
    <title>Error 500</title>
  </head>
  <body>
    <h1>Error 500</h1>
    <p>%v</p>
  </body>
</html>
`
```
**二、参照范例_examples/cas_chi.go 的原理**，修改云盘的main函数
golang的项目必有一个main包，main包里必有一个main函数。云盘初始化之际，执行main函数，就是植入CAS客户端之时。
**思路是这样子的：**
云盘初始化的时候，改用CAS客户端接管和监听服务请求。
**具体如何处理请求呢？**
CAS客户端有个最大的特点，就是如果尚未进行认证，就一定会先转向单点登录；认证过，才会到达我们自己写的这些代码。这一切都由CAS客户端控制。
好，现在到达我们代码部分，每个请求过来的时候，先判断一下是否需要进行自动登录。如果不需要，就按照云盘设定的路由规则执行，否则转向自动登录页面。
**如何判断是否需要进行自动登录呢？**算法如下：
是静态文件吗？，如JS，CSS，图片之类，是的话，肯定不需要登录。
否则检查cookie，如果cookie有相关信息，则已经登录过，也不需要再登录。
一系列判断之后，才输出自动登录页面。输出的时候，会将CAS客户端获取到的登录账号一并输出。
修改过的云盘main函数代码如下：
[//main.go](//main.go)
```
func main() {
	//将运行时参数装填到config中去。
	rest.PrepareConfigs()
	context := rest.NewContext()
	defer context.Destroy()
	//http.Handle("/", context.Router)//屏蔽了原先的路由
	dotPort := fmt.Sprintf(":%v", rest.CONFIG.ServerPort)
	info := fmt.Sprintf("App started at http://localhost%v", dotPort)
	rest.LogInfo(info)
	fmt.Println("网盘运行中：http://localhost%v，请勿关闭",dotPort)
	rest.CasDog(dotPort,context)//人来落闸放狗，CAS客户端接管了服务监听
/*	err := http.ListenAndServe(dotPort, nil)
	if err != nil {
		log.Fatal("ListenAndServe: ", err)
	}*/
}
```
“CAS狗”文件：
rest/cas_chi.go
```
package rest
import (
	"bytes"
	"fmt"
	"html/template"
	"github.com/go-chi/chi"
	"github.com/shenshouer/cas"
	"log"
	"net/http"
	"net/url"
	"strings"
)
type templateBinding struct {
	Account   string
}
func CasDog(port string,ctx *Context) {
	url, _ := url.Parse(CONFIG.Cas)
	client := cas.NewClient(&cas.Options{URL: url})
	root := chi.NewRouter()
	root.Use(client.Handler)
	server := &http.Server{
		Addr:    port,
		Handler: client.Handle(root),
	}
	root.HandleFunc("/*", func(w http.ResponseWriter, r *http.Request) {//注意路由规则是“/*”，而不是"/"
	//否则静态文件等无法访问
		if !autoLogin(ctx,w,r){//如果无须转向自动登录页面，就按照原先的路由规则执行
			ctx.Router.ServeHTTP(w,r)
		}
	})
	if err := server.ListenAndServe(); err != nil {
		log.Fatal(err)
	}
}
func autoLogin(ctx *Context,w http.ResponseWriter,r *http.Request) bool {//自动登录
	path := r.URL.Path
	if strings.Index(path, "/autoLogin") >= 0 || isStaticFile(r){
		return false
	}
	cookie, _ := r.Cookie(COOKIE_AUTH_KEY)
	if cookie != nil{
		ar := strings.Split(cookie.Value,"|")
		if len(ar) > 1{
			username := ar[1]
			if username == cas.Username(r){//已经在云盘中登录过了，有相关的cookie信息
				return false
			}
		}
	}
	outputAutoLoginPage(w,r)
	return true
}
func isStaticFile(r *http.Request) bool{//是静态资源吗
	suffixs := [6]string{".js",".css",".png",".jpg",".gif",".html"}
	for _,sf := range suffixs{
		if strings.HasSuffix(strings.ToLower(r.URL.Path),sf){
			return true
		}
	}
	return false
}
func outputAutoLoginPage(w http.ResponseWriter, r *http.Request){//输出自动登录页面
	w.Header().Add("Content-Type", "text/html")
	tmpl, err := template.New("autoLogin.html").Parse(index_html)
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		fmt.Fprintf(w, error_500, err)
		return
	}
	binding := &templateBinding{
		Account:   cas.Username(r),//在CAS中登录的账号
	}
	html := new(bytes.Buffer)
	if err := tmpl.Execute(html, binding); err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		fmt.Fprintf(w, error_500, err)
		return
	}
	html.WriteTo(w)
}
const index_html = `<!DOCTYPE html>
<html>
  <head>
    <title>Welcome {{.Account}}</title>
	<script src="/static/dist/jquery-1.8.3.min.js"></script>
	<script src="/static/dist/auto.js"></script>
  </head>
  <body>
	<div>login,please wait....</div>
  </body>
</html>
<script>
autoLogin('/api/user/autoLogin','name={{.Account}}');//账号信息
</script>
`
const error_500 = `<!DOCTYPE html>
<html>
  <head>
    <title>Error 500</title>
  </head>
  <body>
    <h1>Error 500</h1>
    <p>%v</p>
  </body>
</html>
`
```
**三、自动登录**
如果已经过CAS认证，那么首次访问云盘，就要自动登录。
改动工作包括两部分，一是服务器端注册一个自动登录路由，二是客户端新增一个自动登录页面。
**1、自动登录路由**
负责实现将CAS传回来的账号在云盘中自动登录，如果不存在该账号，还要先创建，再自动登录。
这个功能是在服务器端写一个函数，然后注册为路由，开放给自动登陆页面调用。
在user_controller.go中修改
```
//注册一个路由
routeMap["/api/user/autoLogin"] = this.Wrap(this.AutoLogin, USER_ROLE_GUEST)
。。。
func (this *UserController) AutoLogin(writer http.ResponseWriter, request *http.Request) *WebResult {
	name := request.FormValue("name")
	user := this.userDao.FindByUserName(name)//新加的方法，用账号名来获取用户信息
	if user == nil {
		return this.autoCreate(name,writer,request)//自动创建
	} else {
		return this.loginImpl(user,writer,request)//登录实现
	}
}
```
以上详细代码就不贴了，容易实现
**2、自动登录页面**
这个页面负责将账号信息提交给自动登录路由。
本来呢，从CAS转回到云盘，账号信息等都在服务器端，为什么不直接在服务器端处理，还要将信息输出到页面，然后又提交回服务器？兜了这么大的一个圈子。原因是不好改，原先云盘的结构都是客户端调用服务器端的api，登录也是，所以暂时先这么处理。
该页面主体是在云盘的main函数中输出，外链的js放在`/static/dist/auto.js`
这里要介绍一下蓝眼云盘的登录机制。它并不是完全依赖cookie的。貌似cookie，只是它的后端使用；而前端则完全依赖于local storage。所以在自动登录成功后，需要将登录信息写入local storage。
```
const index_html = `<!DOCTYPE html>
<html>
  <head>
    <title>Welcome {{.Account}}</title>
	<script src="/static/dist/jquery-1.8.3.min.js"></script>
	<script src="/static/dist/auto.js"></script>
  </head>
  <body>
	<div>login,please wait....</div>
  </body>
</html>
<script>
autoLogin('/api/user/autoLogin','name={{.Account}}');
</script>
`
```
/static/dist/auto.js
```
function autoLogin(url,data){
	$.ajax({
		type: 'post',
		url: url,
		contentType: "application/x-www-form-urlencoded; charset=utf-8",
		dataType: "json",
		data:data,
		timeout: 30000,
		success: function (msg) {
			msg.data.isLogin = true;
			let json = JSON.stringify(msg.data);
			let key = "user";
			window.localStorage.removeItem(key);
			window.localStorage.setItem(key,json);//将信息写入local storage
			location.href = "/";
		},
		error: function (err) {
			alert(err.responseText);
		}
	});
}
```
**四、遗留问题**
单点登出没解决。就是别的系统已经登出了，但云盘这里没反应，仍然处于登录状态。这在别的系统退出并切换账号登录时，就发现两边账号不一致。本来大家都是张三，现在一个是张三，一个是李四。
