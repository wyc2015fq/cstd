# Go Web学习（1）——标准库http实现server - weixin_33985507的博客 - CSDN博客
2018年01月22日 09:43:20[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
最近放假在家好好学习了一下Go语言，Go作为Google官推的Server语言，因为天生的并发性和完备的标准库让Go语言在服务端如鱼得水。笔者在简单的学习了之后，真的是惊讶连连，好了进入正题。
首先，我们必须实现一个Go Web版的Hello World。
```bash
package main
import (
	"fmt"
	"net/http"
	"log"
)
func main() {
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprintf(w, "%s\n", "Hello World")
	})
	err := http.ListenAndServe(":8000", nil)
	if err != nil {
		log.Fatal(err)
	}
}
复制代码
```
我们可以看到Go语言实现一个Web HelloWorld的简洁程度甚至直接媲美Node.js，不需要任何容器便可以实现一个高并发的简单服务器。下面我们来分析一下这个代码：
首先，我们导入了fmt,http包，log包其实对于HelloWorld来说并没有导入的必要，但是日志输出这个良好习惯还是得遵从。在main()函数的第一行，我们通过http.HandleFunc定义了路由为"/"的响应函数，这个响应函数，接受传来的Request，并对Response做一定的处理即写入HelloWorld然后直接返回给浏览器。然后便可以直接调用http.ListenAndServe来监听本地的8000端口，便可以直接在浏览器上看到HelloWorld。
好，上面的流程其实很简单，有一定Web编程的人便都能明白，接下来我们便从Go的源码中看一看，这段代码究竟是如何实现的。
```bash
// HandleFunc registers the handler function for the given pattern
// in the DefaultServeMux.
// The documentation for ServeMux explains how patterns are matched.
func HandleFunc(pattern string, handler func(ResponseWriter, *Request)) {
	DefaultServeMux.HandleFunc(pattern, handler)
}
复制代码
```
上面这段便是Go源码中对HandleFunc函数的实现，我们可以看到这个函数直接将所有参数全部传递给了DefaultServeMux.HandleFunc来调用。
```bash
// DefaultServeMux is the default ServeMux used by Serve.
var DefaultServeMux = &defaultServeMux
var defaultServeMux ServeMux
复制代码
```
DefaultServeMux是http包中的全局变量，它的原型是ServeMux这个结构体，我们再往上翻看这个结构体的HandleFunc方法。
```bash
// HandleFunc registers the handler function for the given pattern.
func (mux *ServeMux) HandleFunc(pattern string, handler func(ResponseWriter, *Request)) {
	mux.Handle(pattern, HandlerFunc(handler))
}
复制代码
```
我们可以看到，似乎没完没了，HandleFunc也是直接调用这个结构体的另一个方法Handle，另外HandlerFunc(handler)中的HandlerFunc也只是一个type的定义。
```bash
type HandlerFunc func(ResponseWriter, *Request)
复制代码
```
这个函数本身并没有实现什么，需要我们自己去实现它的内容。也就是我们上面所提到的响应函数。
```bash
// Handle registers the handler for the given pattern.
// If a handler already exists for pattern, Handle panics.
func (mux *ServeMux) Handle(pattern string, handler Handler)
复制代码
```
终于我们找到了源头，当然这个方法的源代码还比较长，这里就不贴出全部，Handle这个方法接受两个参数，pattern这个string类型的参数表示路由，第二个参数handle它其实是Handler接口。
```bash
type Handler interface {
	ServeHTTP(ResponseWriter, *Request)
}
复制代码
```
可以看到Handler这个接口中只定义了ServeHTTP这一个方法，换句话说，我们也可以直接实现ServeHTTP这个方法来实现Handler这个接口，然后我们便可以传给ServeMux来自定义我们的HelloWorld.
```bash
package main
import (
	"fmt"
	"net/http"
	"log"
)
type CustomHandler struct{}
func (*CustomHandler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "%s\n", "Hello World")
}
func main() {
	mux := http.NewServeMux()
	mux.Handle("/", &CustomHandler{})
	err := http.ListenAndServe(":8000", mux)
	if err != nil {
		log.Fatal(err)
	}
}
复制代码
```
上面的代码可以看到，我们定义了一个CustomHandler,然后实现了ServeHTTP这个方法从而实现了Handler这个接口，在main方法中，我们通过NewServeMux创建了一个自己的mux而不去使用http内的默认ServerMux。然后调用ListenAndServe方法，并将自己的mux传入，程序便会实现自定义的HelloWorld了。
接下来我们来看一下ListenAndServe这个方法：
```bash
// ListenAndServe always returns a non-nil error.
func ListenAndServe(addr string, handler Handler) error {
	server := &Server{Addr: addr, Handler: handler}
	return server.ListenAndServe()
}
复制代码
```
源码中可以看到该方法会将传入进来的addr参数和handler送给Server这个结构体，从而新建一个server然后调用这个server的ListenAndServe方法，对于Server这个结构它已经是Go语言对于这个方面非常底层的实现了，它非常强大，而且实现了很多的方法，这里不过多阐述，主要是实力不够（笑）。
好，回到正题，既然如此，我们便可以自己创建Server这个实例，来自定义我们的HelloWorld的第二版本。
```bash
package main
import (
	"fmt"
	"net/http"
	"log"
	"time"
)
type CustomHandler struct{}
var mux = make(map[string]func(http.ResponseWriter, *http.Request))
func Hello(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "%s\n", "Hello World")
}
func (*CustomHandler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	if handler, ok := mux[r.URL.String()]; ok {
		handler(w, r)
	}
}
func main() {
	server := http.Server{
		Addr:":8000",
		Handler:&CustomHandler{},
		ReadHeaderTimeout:5 * time.Second,
	}
	mux["/"] = Hello
	err := server.ListenAndServe()
	if err != nil {
		log.Fatal(err)
	}
}
复制代码
```
上面这段代码便是自创server的实现了，这里挑选几条新的代码说明一下，我们定义了一个mux的全局变量，它来装配我们的路由与相应函数的映射，相当于上面的mux.Handle("/", .....)，这里比较简陋的直接用Map来实现，接下来我们定义了Hello这个响应函数，我们也重写了ServeHTTP这个方法，它会判断request的url路径与我们mux里面的路径是否匹配，如果匹配在从mux中取出相应的响应函数并将w http.ResponseWriter, r *http.Request这两个参数传递给这个相应函数。
在main函数里，我们创建了自己的server，通过端口号，Handler及timeout时间来定义它，然后调用它的ListenAndServe方法，便可以实现与前面两个相同的HelloWorld功能。好了，今天写到这里，太晚了（笑）。
