# [Golang] 写个小东西：今天吃什么好 - Pangda NoSilly - CSDN博客

2017年09月14日 17:42:12[hepangda](https://me.csdn.net/hepangda)阅读数：248


# 所以到底吃什么？

每天到了饭点都会莫名的焦虑，到底吃什么啊，感觉每个都想吃，每个也都不想吃的样子，非常的纠结。打开手机自带的Roll点App又要开始纠结到底点数对应啥，实在是非常的烦人，所以不如写一个程序自动帮我做决定好了。

然而自身太弱了，写不出手机应用，写Web应用的话……不也还是不会写吗！！就这么点小小的需求要等那么多东西学完才能写实在是太不Cool了。于是决定用`golang`自带的`HTTP`服务器，加上从网上10分钟水平的`HTML`，写个自己Roll着玩的就行了，于是一个很朴素的网页就有了：

```
var htmlFormat = `
<html>
<head>
    <style>
        a:link{text-decoration:none ; color:black;}
        a:visited {text-decoration:none ; color:black;}
        a:hover {text-decoration:underline ; color:black;}
        a:active {text-decoration:none ; color:black;} 
        body { font: normal 100%% Helvetica, Arial, sans-serif; }
    </style>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
</head>
    <body>
        %s
        <hr />
        <a href="../">点击此处返回</a>
    </body>
</html>
`
```

学习`golang`的时候也没有怎么好好的使用`map`，这次就强行把他当个数组来存储我们三餐都可以吃什么吧：

```
var breakfast, lunch, dinner map[int]string
var nbf, nlc, ndn int
```

既然有了三餐吃什么，当然要从文件里加载了啊（不然怎么练习一下读写文件呢嗯），然后直接爽快的复制三份，调整一下就有了三餐对应的函数，类似于：

```
func initbf() error {
    for ts := range breakfast {
        delete(breakfast, ts)
    }
    nbf = -1
    f, err := os.Open("breakfast")
    if err != nil {
        return err
    }
    defer f.Close()
    reader := bufio.NewReader(f)
    for {
        line, err := reader.ReadBytes('\n')
        nbf++
        breakfast[nbf] = string(line)
        if err != nil {
            if err == io.EOF {
                return nil
            }
            return err
        }
    }
}
```

然后写一个Roll早餐的函数，并且复制三份，微调一下（捂脸），类似于这样：

```
func hBreakfast(w http.ResponseWriter, r *http.Request) {
    rander := rand.New(rand.NewSource(time.Now().UnixNano()))
    fmt.Fprintf(w, htmlFormat, fmt.Sprintf(`
        早饭就吃【%s】了！！！<br /><br />
        <a href="../breakfast">我不服！再Roll一次！</a>
    `, breakfast[rander.Intn(nbf)]))
}
```

然后初始化一个http服务器，并关联一下处理函数，写出`main`函数：

```
func main() {
    //TODO：初始化一下下
    http.HandleFunc("/", hIndex)
    http.HandleFunc("/breakfast", hBreakfast)
    http.HandleFunc("/lunch", hLunch)
    http.HandleFunc("/dinner", hDinner)
    log.Fatal(http.ListenAndServe(":8080", nil))
}
```

最后加点花哨的功能←_←，全部代码如下：

```
package main

import (
    "bufio"
    "fmt"
    "io"
    "log"
    "math/rand"
    "net/http"
    "os"
    "time"
)

var htmlFormat = `
<html>
<head>
    <style>
        a:link{text-decoration:none ; color:black;}
        a:visited {text-decoration:none ; color:black;}
        a:hover {text-decoration:underline ; color:black;}
        a:active {text-decoration:none ; color:black;} 
        body { font: normal 100%% Helvetica, Arial, sans-serif; }
    </style>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
</head>
    <body>
        %s
        <hr />
        <a href="../">点击此处返回</a>
    </body>
</html>
`
var htmlBack = `<a href="../">更新请求已经发送，点击此处返回</a>`

var breakfast, lunch, dinner map[int]string
var nbf, nlc, ndn int

func main() {
    start()
    http.HandleFunc("/", hIndex)
    http.HandleFunc("/breakfast", hBreakfast)
    http.HandleFunc("/lunch", hLunch)
    http.HandleFunc("/dinner", hDinner)
    http.HandleFunc("/$update_breakfast$", hubf)
    http.HandleFunc("/$update_lunch$", hulc)
    http.HandleFunc("/$update_dinner$", hudn)
    log.Fatal(http.ListenAndServe(":8080", nil))
}

func start() {
    breakfast = make(map[int]string)
    lunch = make(map[int]string)
    dinner = make(map[int]string)
    nbf = 0
    nlc = 0
    ndn = 0
    initbf()
    initlc()
    initdn()
}

func hubf(w http.ResponseWriter, r *http.Request) {
    initbf()
    log.Println("Updated Breakfast")
    fmt.Fprintf(w, htmlFormat, htmlBack)
}

func hulc(w http.ResponseWriter, r *http.Request) {
    initlc()
    log.Println("Updated Lunch")
    fmt.Fprintf(w, htmlFormat, htmlBack)
}

func hudn(w http.ResponseWriter, r *http.Request) {
    initdn()
    log.Println("Updated Dinner")
    fmt.Fprintf(w, htmlFormat, htmlBack)
}

func initbf() error {
    for ts := range breakfast {
        delete(breakfast, ts)
    }
    nbf = -1
    f, err := os.Open("breakfast")
    if err != nil {
        return err
    }
    defer f.Close()
    reader := bufio.NewReader(f)
    for {
        line, err := reader.ReadBytes('\n')
        nbf++
        breakfast[nbf] = string(line)
        if err != nil {
            if err == io.EOF {
                return nil
            }
            return err
        }
    }
}

func initlc() error {
    for ts := range lunch {
        delete(lunch, ts)
    }
    nlc = -1
    f, err := os.Open("lunch")
    if err != nil {
        return err
    }
    defer f.Close()
    reader := bufio.NewReader(f)
    for {
        line, err := reader.ReadBytes('\n')
        nlc++
        lunch[nlc] = string(line)
        if err != nil {
            if err == io.EOF {
                return nil
            }
            return err
        }
    }
}

func initdn() error {
    for ts := range dinner {
        delete(dinner, ts)
    }
    ndn = -1
    f, err := os.Open("dinner")
    if err != nil {
        return err
    }
    defer f.Close()
    reader := bufio.NewReader(f)
    for {
        line, err := reader.ReadBytes('\n')
        ndn++
        dinner[ndn] = string(line)
        if err != nil {
            if err == io.EOF {
                return nil
            }
            return err
        }
    }
}

func hIndex(w http.ResponseWriter, r *http.Request) {
    fmt.Fprintf(w, htmlFormat, `
        <a href="/breakfast">早饭吃什么</a><br />
        <a href="/lunch">午饭吃什么</a><br />
        <a href="/dinner">晚饭吃什么</a>
    `)
}

func hBreakfast(w http.ResponseWriter, r *http.Request) {
    rander := rand.New(rand.NewSource(time.Now().UnixNano()))
    fmt.Fprintf(w, htmlFormat, fmt.Sprintf(`
        早饭就吃【%s】了！！！<br /><br />
        <a href="../breakfast">我不服！再Roll一次！</a>
    `, breakfast[rander.Intn(nbf)]))
}

func hLunch(w http.ResponseWriter, r *http.Request) {
    rander := rand.New(rand.NewSource(time.Now().UnixNano()))

    fmt.Fprintf(w, htmlFormat, fmt.Sprintf(`
        午饭就吃【%s】了！！！<br /><br />
        <a href="../lunch">我不服！再Roll一次！</a>
    `, lunch[rander.Intn(nlc)]))
}

func hDinner(w http.ResponseWriter, r *http.Request) {
    rander := rand.New(rand.NewSource(time.Now().UnixNano()))
    fmt.Fprintf(w, htmlFormat, fmt.Sprintf(`
        晚饭就吃【%s】了！！！<br /><br />
        <a href="../dinner">我不服！再Roll一次！</a>
    `, dinner[rander.Intn(ndn)]))
}
```

