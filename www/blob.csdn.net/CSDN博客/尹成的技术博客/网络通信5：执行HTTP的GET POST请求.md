
# 网络通信5：执行HTTP的GET-POST请求 - 尹成的技术博客 - CSDN博客

2018年11月16日 11:14:09[尹成](https://me.csdn.net/yincheng01)阅读数：139个人分类：[GO语言](https://blog.csdn.net/yincheng01/article/category/7679307)[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)[](https://blog.csdn.net/yincheng01/article/category/7679307)



**导入依赖包**
`import (
	"fmt"
	"net/http"
	"io/ioutil"
	"strings"
)`**提交GET请求并获得返回**
`func main521() {
	url := "http://www.baidu.com/s?wd=肉"
	resp, err := http.Get(url)
	if err != nil {
		fmt.Println("错误")
	}
	defer resp.Body.Close()
	bodyBytes, _ := ioutil.ReadAll(resp.Body) //读取信息
	fmt.Println(string(bodyBytes))            //读取网页源代码
}`**提交POST请求并获得返回**
`func main522() {
	//url := "http://www.baidu.com"
	url := "https://httpbin.org/post?name=张三"
	resp, err := http.Post(
		url,
		"application/x-www-form-urlencoded",
		strings.NewReader("id=nimei"))
	if err != nil {
		fmt.Println("错误")
	}
	defer resp.Body.Close()
	body, _ := ioutil.ReadAll(resp.Body) //读取信息
	fmt.Println(string(body))            //读取网页源代码
}`学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116111358699.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

