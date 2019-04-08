# go语言学习总结

qq_33549434 · 2017-08-31 08:32:37 · 1207 次点击 · 预计阅读时间 20 分钟 · 大约2小时之前 开始浏览    

这是一个创建于 2017-08-31 08:32:37 的文章，其中的信息可能已经有所发展或是发生改变。

​       由于工作原因本人接触了一个月的go语言,使用过程中碰到很多问题也学习到很多知识下面对用到的知识以及注意事项做个总结，希望对刚接触go语言的同学有一些帮助

##  

## 一、数据类型转换

```go
package main

import (
	"fmt"
	"strconv"
)

func main() {
	//int到string
	str := strconv.Itoa(int(1))
	fmt.Println("int转string", str)
	//int64到string
	str2 := strconv.FormatInt(int64(1), 10)
	fmt.Println("int64转string", str2)
	//float64转string formatFloat只能接收float64如果想用float32需要强转float64(float32(0.8))
	//下面是参数说明
	// 'b' (-ddddp±ddd，二进制指数)
	// 'e' (-d.dddde±dd，十进制指数)
	// 'E' (-d.ddddE±dd，十进制指数)
	// 'f' (-ddd.dddd，没有指数)
	// 'g' ('e':大指数，'f':其它情况)
	// 'G' ('E':大指数，'f':其它情况)
	str3 := strconv.FormatFloat(float64(0.8), 'f', -1, 32)
	fmt.Println("float32转string", str3)
	//string到int  有异常的都不进行处理这个后面说
	i, _ := strconv.Atoi("10")
	fmt.Println("strin转int", i)
	//string 到int64
	i64, _ := strconv.ParseInt("123", 10, 64)
	fmt.Println("strin转int64", i64)
	//string转float64 如果想转float32 用float32(fl32)强转一下就可以
	fl32, _ := strconv.ParseFloat("3.1415926535", 32/64)
	fmt.Println("strin转float64", fl32)
}
```



##  

## 二、时间类型转换

```go
package main

import (
	"fmt"
	"time"
)

func main() {
	//获取当前时间
	now := time.Now()
	//时间转化为string
	//在go语言里将日期格式化并不是yyyy-MM-dd HH:ss:mm 而是用"2006-01-02 15:04:05具体含义如下
	//月份 1,01,Jan,January
	//日　 2,02,_2
	//时　 3,03,15,PM,pm,AM,am
	//分　 4,04
	//秒　 5,05
	//年　 06,2006
	//周几 Mon,Monday
	//时区时差表示 -07,-0700,Z0700,Z07:00,-07:00,MST
	//时区字母缩写 MST
	timeStr := now.Format("2006-01-02 15:04:05")
	fmt.Println("日期类型当前时间: ", now)
	fmt.Println("字符串类型当前时间: ", timeStr)
	//string转化为时间
	date, _ := time.Parse("2006-01-02 15:04:05", "2017-08-29 08:37:18")
	fmt.Println("string转日期:", date)
	//判断两个时间先后
	trueOrFalse := date.After(now)
	if trueOrFalse == true {
		fmt.Println("2017-08-29 08:37:18在", timeStr, "之后")
	} else {
		fmt.Println("2017-08-29 08:37:18在", timeStr, "之前")
	}
	// ParseDuration parses a duration string.
	// A duration string is a possibly signed sequence of decimal numbers,
	// each with optional fraction and a unit suffix,
	// such as "300ms", "-1.5h" or "2h45m".
	//  Valid time units are "ns", "us" (or "µs"), "ms", "s", "m", "h".
	// -代表时间之前,不加代表时间之后 m表示分钟,如20分钟前
	m, _ := time.ParseDuration("-20m")
	m1 := now.Add(m)
	fmt.Println("10分钟之前：", m1)
	// h代表小时 一天之前为-24h
	h, _ := time.ParseDuration("-8h")
	h1 := now.Add(h)
	fmt.Println("8小时之前", h1)
	//	// 一天前
	d, _ := time.ParseDuration("-24h")
	d1 := now.Add(d)
	fmt.Println(d1)
	//计算两个时间差几秒
	sec := now.Sub(m1)
	fmt.Println(sec.Seconds(), "秒")
	//计算两个时间差几分钟
	minutes := now.Sub(m1)
	fmt.Println(minutes.Minutes(), "分钟")
	//计算两个时间差几小时
	hours := now.Sub(h1)
	fmt.Println(hours.Hours(), "小时")
	//计算两个时间差几天
	day := now.Sub(d1)
	fmt.Println(day.Hours()/24, "天")
	//注意:splite3数据库中字段如果是datetime类型获取数据时格式转换会有问题
	//如2017-08-29 08:37:18这样的时间从数据库获取后会变成2017-08-29T08:37:18Z
	//进行格式转化之后不能比较，所以需要将T和Z替换为" "
	//不知道其他数据库有没有这样的问题
}
```

## 三、配置文件读取  

```go
package main

import (
	"flag"
	"sync"

	"github.com/larspensjo/config"
)

var Conf_Main_Topic = "DEFAULT"

var (
	//config.ini为配置文件格式为 key=value
	configFile = flag.String("configfile", "config.ini", "General configuration file")
)
var commonConf = make(map[string]string)
var lock sync.RWMutex

func LoadCommonConfiguration() {
	lock.Lock()
	defer lock.Unlock()
	cfg, err := config.ReadDefault(*configFile)
	if err != nil {
		//....
	}
	commonConf = make(map[string]string)
	if cfg.HasSection(Conf_Main_Topic) {
		section, err := cfg.SectionOptions(Conf_Main_Topic)
		if err != nil {
			//....
		}
		for _, v := range section {
			options, err := cfg.String(Conf_Main_Topic, v)
			if err != nil {
				//....
			}
			commonConf[v] = options
		}
	}
}

//通过GetConf方法将key传入获取value值
func GetConf(key string) string {
	lock.RLock()
	defer lock.RUnlock()
	return commonConf[key]
}
```



## 四、Json与Map转换

```go
package main

import (
	"encoding/json"
	"strings"
	//	"github.com/bitly/go-simplejson" // for json get
)

//把两层嵌套结构的json格式的数据组转成map(map中不含interface结构)
func NoInterfaceJsonToMap(input string) (map[string]map[string]interface{}, error) {
	result := make(map[string]map[string]interface{})
	err := json.Unmarshal([]byte(input), &result)
	if err != nil {
		return nil, err
	}
	return result, nil
}
func MapToJson(input map[string]interface{}) (string, error) {
	result, err := json.Marshal(input)
	if err != nil {
		//		panic(err)
		return "", err
	}
	return string(result), nil
}

func MapMapToJson(input map[string]map[string]interface{}) (string, error) {
	result, err := json.Marshal(input)
	if err != nil {
		return "", err
	}
	return string(result), nil
}

func JsonToMap(input string) (map[string]interface{}, error) {
	result := make(map[string]interface{})
	err := json.Unmarshal([]byte(input), &result)
	if err != nil {
		return nil, err
	}
	return result, nil
}

func BoltKeyValueToJson(key, value string, delimeter string) (string, error) {
	keys := []string{key}
	values := []string{value}
	return BoltKeyValuesToJson(keys, values, delimeter)
}
func BoltKeyValuesToJson(keys, values []string, delimeter string) (string, error) {
	mapResult := make(map[string]interface{})
	for i := range keys {
		key := strings.Split(keys[i], delimeter)
		value := values[i]
		cur := mapResult
		for j := range key {
			if j == len(key)-1 {
			} else if j == len(key)-2 {
				if cur[key[j]] == nil {
					cur[key[j]] = map[string]string{key[len(key)-1]: value}
				} else {
					cur[key[j]].(map[string]string)[key[len(key)-1]] = value
				}
			} else {
				if cur[key[j]] == nil {
					cur[key[j]] = make(map[string]interface{})
				}
				cur = cur[key[j]].(map[string]interface{})
			}
		}
	}
	return MapToJson(mapResult)
}
```

##     

## 五、通过JSON数据发送Email

```go
package main

import (
	"bytes"
	"errors"
	"fmt"
	"io/ioutil"
	"net/http"
	"strconv"
	"strings"
)


func HttpPostByJSON(accessUrl string, json string, redo int) error {
	fmt.Println(json)
	fmt.Println("post write json bytes:" + strconv.Itoa(len(json)))
	for j := 1; j <= redo; j++ {
		req_new := bytes.NewBuffer([]byte(json))
		request, err := http.NewRequest("POST", accessUrl, req_new)
		if err == nil {
			request.Header.Set("Content-Type", "application/json;charset=UTF-8")
			client := http.Client{}
			response, err1 := client.Do(request)
			if err1 == nil {
				body, err := ioutil.ReadAll(response.Body)
				if err != nil {
					fmt.Println("Unknown error in sending Email")
				} else {
					resp := string(body)
					if strings.Contains(resp, "\"code\":\"200\"") {
						return nil
					} else {
						fmt.Println(string(body))
					}
				}
			} else {
				fmt.Println(err1)
			}
		} else {
			fmt.Println(err)
		}
	}
	return errors.New("Fail to send email notification")
}
```

##    

## 六、文件读写删除操作

```go
package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"strings"
)
func FileExist(filename string) bool {
	_, err := os.Stat(filename)
	if os.IsNotExist(err) {
		return false
	}
	fmt.Println(err)
	return err == nil || os.IsExist(err)
}

func ReadFileByLine(filename string) []string {
	if !FileExist(filename) {
		return []string{}
	}
	f, err := os.Open(filename)
	fmt.Println(err)
	defer f.Close()
	rd := bufio.NewReader(f)
	result := []string{}
	for {
		line, err := rd.ReadString('\n')
		if err == nil || io.EOF == err {
			line = strings.TrimSpace(line)
			if len(line) > 0 {
				result = append(result, line)
			}
		}
		if io.EOF == err {
			break
		}
		fmt.Println(err)
	}
	return result
}

func WriteFileByLine(filename string, data []string) {
	f, err := os.Create(filename)
	fmt.Println(err)
	defer f.Close()
	wt := bufio.NewWriter(f)
	for i := range data {
		_, err := wt.WriteString(data[i])
		if io.EOF == err {
			break
		}
		fmt.Println(err)
	}
	wt.Flush()
}

func DeleteFile(filename string) {
	if FileExist(filename) {
		err := os.Remove(filename)
		fmt.Println(err)
	}
}
```

##     

## 七、go执行linux和windows命令操作

```go
package main

import (
	"errors"
	"fmt"
	"os/exec"
	"runtime"
	"strings"
)

func runInLinux(cmd string) string {
	fmt.Println("Running Linux Cmd:" + cmd)
	result, err := exec.Command("/bin/sh", "-c", cmd).Output()
	if err != nil {
		fmt.Println(err.Error())
	}
	return strings.TrimSpace(string(result))
}

func runInWindows(cmd string) string {
	fmt.Println("Running Win Cmd:" + cmd)
	result, err := exec.Command("cmd", "/c", cmd).Output()
	if err != nil {
		fmt.Println(err.Error())
	}
	return strings.TrimSpace(string(result))
}

func RunCommand(cmd string) string {
	if runtime.GOOS == "windows" {
		return runInWindows(cmd)
	} else {
		return runInLinux(cmd)
	}
}

func RunLinuxCommand(cmd string) string {
	if runtime.GOOS == "windows" {
		return ""
	} else {
		return runInLinux(cmd)
	}
}

func runInLinuxWithErr(cmd string) (string, error) {
	fmt.Println("Running Linux Cmd:" + cmd)
	result, err := exec.Command("/bin/sh", "-c", cmd).Output()
	if err != nil {
		fmt.Println(err.Error())
	}
	return strings.TrimSpace(string(result)), err
}

func runInWindowsWithErr(cmd string) (string, error) {
	fmt.Println("Running Win Cmd:" + cmd)
	result, err := exec.Command("cmd", "/c", cmd).Output()
	if err != nil {
		fmt.Println(err.Error())
	}
	return strings.TrimSpace(string(result)), err
}

func RunCommandWithErr(cmd string) (string, error) {
	if runtime.GOOS == "windows" {
		return runInWindowsWithErr(cmd)
	} else {
		return runInLinuxWithErr(cmd)
	}
}

func RunLinuxCommandWithErr(cmd string) (string, error) {
	if runtime.GOOS == "windows" {
		return "", errors.New("could not run in windows OS")

	} else {
		return runInLinuxWithErr(cmd)
	}
}
```

##         

## 八、异常处理

Golang 有2个内置的函数 panic() 和 recover()，用以报告和捕获运行时发生的程序错误，与 error 不同，panic-recover 一般用在函数内部。一定要注意不要滥用 panic-recover，可能会导致性能问题，我一般只在未知输入和不可靠请求时使用。

golang 的错误处理流程：当一个函数在执行过程中出现了异常或遇到 panic()，正常语句就会立即终止，然后执行 defer 语句，再报告异常信息，最后退出 goroutine。如果在 defer 中使用了 recover() 函数,则会捕获错误信息，使该错误信息终止报告。



```go
package main
import (
 "log"
 "strconv"
)
//捕获因未知输入导致的程序异常
func catch(nums ...int) int {
 defer func() {
  if r := recover(); r != nil {
   log.Println("[E]", r)
  }
 }()
 return nums[1] * nums[2] * nums[3] //index out of range
}
//主动抛出 panic，不推荐使用，可能会导致性能问题
func toFloat64(num string) (float64, error) {
 defer func() {
  if r := recover(); r != nil {
   log.Println("[W]", r)
  }
 }()
 if num == "" {
  panic("param is null") //主动抛出 panic
 }
 return strconv.ParseFloat(num, 10)
}
func main() {
 catch(2, 8)
 toFloat64("")
}
```

上面为引用的内容原文地址

http://www.jb51.net/article/58139.htm



go语言有异常的返回会有两个返回参数包括值跟error，判断error是否为nil进行相应处理就可以最好不要panic



## 九、defer(延迟语句)

defer语句会在函数返回前，从后到前的依次执行defer



```go
package main

import (
	"fmt"
)

func main() {
	fmt.Println("start================")
	fmt.Println("t5返回值", t5())
	fmt.Println("end================")
}
func t1() {
	fmt.Println("t1")
}
func t2() {
	fmt.Println("t2")
}
func t3() {
	fmt.Println("t3")
}
func t4() {
	fmt.Println("t4")
}
func t5() int {
	defer t1()
	defer t2()
	defer t3()
	defer t4()
	return 1
}
```



```
程序输出:
start================
t4
t3
t2
t1
t5返回值 1
end================
```



## 十、数据库操作

```go
package main

import (
	"database/sql"
	"errors"
	"fmt"
	"sync"

	_ "github.com/mattn/go-sqlite3"
)

var db *sql.DB
var dbLock sync.Mutex

func main() {
	//查询调用RunQuery或者RunQueryWithErr
	//?为占位符对应相应参数
	rows := RunQuery("select age from table where id = ? and name = ?", 1, "abc")
	//也可以使用struct构建对象,获取数据库字段时需要用sql.Null...否则取空值会报错
	var age sql.NullInt64
	var ages []Int64

	//如果有多条返回值用for循环遍历,如果确定只有一条可以用if,这里只有一条所以用if,也可以将if改为for
	if rows.Next() {
		err := rows.Scan(&age)
		if err != nil {
			//........
		}
		//sql.NullInt64中有两个方法Int64和Valid,Valid用来判断值是否为空返回值为true、false
		//Int64为实际值,sql.NullInt64对象如果为空Int64值为0
		if age.Valid {
			//对获取到的age做操作
			fmt.Println(age.Int64)
		}
	}
	//sqlite3中使用完rows之后一定要调用close释放锁,否则继续调用数据库语句会报数据库被锁的异常
	rows.Close()
	//sqlite3中查询和增删改不能同时进行,也会报数据库被锁的异常
	//需要在查询之后统一进行增删改操作,增删改调用ExecSQL或ExecSqlWithErr
}

func OpenDB() {
	var err error
	//foo.db为数据库名字
	db, err = sql.Open("sqlite3", "./foo.db?_txlock=exclusive")
	fmt.Println(err)
	_, err = db.Exec("PRAGMA synchronous = OFF;")
	fmt.Println(err)
}

func CloseDB() {
	if db != nil {
		err := db.Close()
		db = nil
		fmt.Println(err)
	}
}

func ExecSQL(query string, args ...interface{}) sql.Result {
	dbLock.Lock()
	defer dbLock.Unlock()
	if db == nil {
		OpenDB()
	}
	if query == "" {
		panic("Empty sql input")
	}
	fmt.Println("Running Exec:")
	fmt.Println(query)
	fmt.Println(args...)
	res, err := db.Exec(query, args...)
	fmt.Println(err)
	return res
}

func RunQuery(query string, args ...interface{}) *sql.Rows {
	dbLock.Lock()
	defer dbLock.Unlock()
	if db == nil {
		OpenDB()
	}
	if query == "" {
		panic("Empty sql input")
	}
	fmt.Println("Running Query:")
	fmt.Println(query)
	fmt.Println(args...)
	res, err := db.Query(query, args...)
	fmt.Println(err)
	return res
}
//多条语句的事物操作
func ExecTransaction(querys []string, args [][]string) {
	dbLock.Lock()
	defer dbLock.Unlock()
	if db == nil {
		OpenDB()
	}
	tx, err := db.Begin()
	fmt.Println(err)
	fmt.Println("Running Trans:")
	for index, query := range querys {
		if query == "" {
			panic("Empty sql input")
		}
		oldArgs := args[index]
		newArgs := make([]interface{}, len(oldArgs))
		for i, v := range oldArgs {
			newArgs[i] = interface{}(v)
		}
		fmt.Println(query)
		fmt.Println(newArgs...)
		_, err := tx.Exec(query, newArgs...)
		fmt.Println(err)
	}
	err = tx.Commit()
	fmt.Println(err)
}

func OpenDBWithErr() error {
	var err error
	db, err = sql.Open("sqlite3", "./pjm.db?_txlock=exclusive")
	if err != nil {
		return err
	}
	_, err = db.Exec("PRAGMA synchronous = OFF;")
	return err
}

func CloseDBWithErr() error {
	if db != nil {
		err := db.Close()
		db = nil
		return err
	}
	return nil
}

func ExecSQLWithErr(query string, args ...interface{}) (sql.Result, error) {
	dbLock.Lock()
	defer dbLock.Unlock()
	if db == nil {
		err := OpenDBWithErr()
		if err != nil {
			return nil, err
		}
	}
	if query == "" {
		return nil, errors.New("Empty sql input")
	}
	fmt.Println("Running Exec With Err:")
	fmt.Println(query)
	fmt.Println(args...)
	return db.Exec(query, args...)
}

func RunQueryWithErr(query string, args ...interface{}) (*sql.Rows, error) {
	dbLock.Lock()
	defer dbLock.Unlock()
	if db == nil {
		err := OpenDBWithErr()
		if err != nil {
			return nil, err
		}
	}
	if query == "" {
		return nil, errors.New("Empty sql input")
	}
	fmt.Println("Running Query With Err:")
	fmt.Println(query)
	fmt.Println(args...)
	return db.Query(query, args...)
}

func ExecTransactionWithErr(querys []string, args [][]string) error {
	dbLock.Lock()
	defer dbLock.Unlock()
	if db == nil {
		err := OpenDBWithErr()
		if err != nil {
			return err
		}
	}
	tx, err := db.Begin()
	if err != nil {
		return err
	}
	fmt.Println("Running Trans With Err:")
	for index, query := range querys {
		if query == "" {
			return errors.New("Empty sql input")
		}
		oldArgs := args[index]
		newArgs := make([]interface{}, len(oldArgs))
		for i, v := range oldArgs {
			newArgs[i] = interface{}(v)
		}
		fmt.Println(query)
		fmt.Println(newArgs...)
		_, err := tx.Exec(query, newArgs...)
		if err != nil {
			return err
		}
	}
	return tx.Commit()
}
```

## 十一、go语言web编程

```go
package main

import (
	"fmt"
	"html/template"
	"log"
	"net/http"
	"strings"
)

func sayhelloName(w http.ResponseWriter, r *http.Request) {
	r.ParseForm() //解析url传递的参数，对于POST则解析响应包的主体（request body）
	//注意:如果没有调用ParseForm方法，下面无法获取表单的数据
	fmt.Println(r.Form) //这些信息是输出到服务器端的打印信息
	fmt.Println("path", r.URL.Path)
	fmt.Println("scheme", r.URL.Scheme)
	fmt.Println(r.Form["url_long"])
	for k, v := range r.Form {
		fmt.Println("key:", k)
		fmt.Println("val:", strings.Join(v, ""))
	}
	fmt.Fprintf(w, "Hello astaxie!") //这个写入到w的是输出到客户端的
}

func login(w http.ResponseWriter, r *http.Request) {
	fmt.Println("method:", r.Method) //获取请求的方法
	//Handler里面是不会自动解析form的，必须显式的调用r.ParseForm()后，才能对这个表单数据进行操作
	r.ParseForm()
	if r.Method == "GET" {
		t, _ := template.ParseFiles("login.html")
		t.Execute(w, nil)
	} else {
		//请求的是登陆数据，那么执行登陆的逻辑判断
		fmt.Println("username:", r.Form["username"])
		fmt.Println("password:", r.Form["password"])
		//获取username的值需要[0]
		if r.Form["username"][0] == "abc" {
			//如果username为abc跳转到user.html页面打印abc=====
			t, _ := template.ParseFiles("./user.html")
			//t.Execute第二个参数可以传各种类型的数据到页面
			t.Execute(w, "abc=====")
		} else if r.Form["username"][0] == "efg" {
			t, _ := template.ParseFiles("./userMap.html")
			result := map[string]string{}
			result["key"] = "value"
			//返回map
			t.Execute(w, result)
		} else {
			t, _ := template.ParseFiles("./userMaps.html")
			result := []map[string]string{}
			m1 := map[string]string{}
			m2 := map[string]string{}
			m3 := map[string]string{}
			m1["a1"] = "111"
			m1["a2"] = "222"
			m1["a3"] = "333"
			m1["a4"] = "444"
			m2["a1"] = "555"
			m2["a2"] = "666"
			m2["a3"] = "777"
			m2["a4"] = "888"
			m3["a1"] = "999"
			m3["a2"] = "123"
			m3["a3"] = "456"
			m3["a4"] = "789"
			result = append(result, m1, m2, m3)
			//返回多个map
			t.Execute(w, result)
		}
	}
}

func main() {
	http.HandleFunc("/", sayhelloName)       //设置访问的路由
	http.HandleFunc("/login", login)         //设置访问的路由
	err := http.ListenAndServe(":9090", nil) //设置监听的端口
	if err != nil {
		log.Fatal("ListenAndServe: ", err)
	}
}
```

login.html页面

```html
<html>
<head>
<title></title>
</head>
<body>
<form action="/login" method="post">
    用户名:<input type="text" name="username">
    密码:<input type="password" name="password">
    <input type="submit" value="登陆">
</form>
</body>
</html>
```

user.html页面

go语言中页面获取值必须加{{  }}

```html
<html>
<head>
<title></title>
</head>
<body>
从后台获取的值为: {{.}}
</body>
</html>
```

userMap.html页面

```html
<html>
<head>
<title></title>
</head>
<body>
从后台获取的值为: {{.key}}
</body>
</html>
```

userMaps.html页面

```html
<html>
<head>
<title></title>
</head>
<body>
{{range $index,$re :=.}}
{{$re.a1}}</br>
{{$re.a2}}</br>
{{$re.a3}}</br>
{{$re.a4}}</br>
{{end}}
</body>
</html>
```





## 十二、分页实现

```go
//分页方法，根据传递过来的页数，每页数，总数，返回分页的内容 7个页数 前 1，2，3，4，5 后 的格式返回,小于5页返回具体页数
func Paginator(page, prepage int, nums int64) map[string]interface{} {

	var firstpage int //前一页地址
	var lastpage int  //后一页地址
	//根据nums总数，和prepage每页数量 生成分页总数
	totalpages := int(math.Ceil(float64(nums) / float64(prepage))) //page总数
	if page > totalpages {
		page = totalpages
	}
	if page <= 0 {
		page = 1
	}
	var pages []int
	switch {
	case page >= totalpages-5 && totalpages > 5: //最后5页
		start := totalpages - 5 + 1
		firstpage = page - 1
		lastpage = int(math.Min(float64(totalpages), float64(page+1)))
		pages = make([]int, 5)
		for i, _ := range pages {
			pages[i] = start + i
		}
	case page >= 3 && totalpages > 5:
		start := page - 3 + 1
		pages = make([]int, 5)
		firstpage = page - 3
		for i, _ := range pages {
			pages[i] = start + i
		}
		firstpage = page - 1
		lastpage = page + 1
	default:
		pages = make([]int, int(math.Min(5, float64(totalpages))))
		for i, _ := range pages {
			pages[i] = i + 1
		}
		firstpage = int(math.Max(float64(1), float64(page-1)))
		lastpage = page + 1
		//fmt.Println(pages)
	}
	paginatorMap := make(map[string]interface{})
	paginatorMap["pages"] = pages
	paginatorMap["totalpages"] = totalpages
	paginatorMap["firstpage"] = firstpage
	paginatorMap["lastpage"] = lastpage
	paginatorMap["currpage"] = page
	return paginatorMap
}
```

html页面

```html
  <div class="am-cf">
  共{{.totals}}条记录 共记{{.paginator.totalpages}} 页 当前页  {{.paginator.currpage}}
  <div class="am-fr">
    <ul class="am-pagination">
    <li class=""><a href="/clubadmin/topics/{{.paginator.firstpage}}">«</a></li> 
    {{range $index,$page := .paginator.pages}}
      <li  {{if eq $.paginator.currpage $page }}class="am-active"{{end}}><a href="/clubadmin/topics/{{$page}}">{{$page}}</a></li>  
    {{end}}
      <li><a href="/clubadmin/topics/{{.paginator.lastpage}}">»</a></li>
    </ul>
  </div>
</div>
```

我自己写的分页是参考上面的例子做的,原文地址

https://studygolang.com/articles/5718

