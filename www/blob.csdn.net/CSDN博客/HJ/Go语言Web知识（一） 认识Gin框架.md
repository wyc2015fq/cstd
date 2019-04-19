# Go语言Web知识（一） 认识Gin框架 - HJ - CSDN博客
2018年11月29日 19:40:59[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：90
入门代码
```
package main
import "github.com/gin-gonic/gin"
func main()  {
	r := gin.Default()
	r.GET("/ping", func(c *gin.Context) {
		c.JSON(200,gin.H{
			"message":"pong",
		})
	})
	r.Run(":8080")
}
```
curl http://localhost:8080/ping
返回 {"meseege" : "pong"}
接收Get与Post方式传输的参数
```
package main
import (
	"github.com/gin-gonic/gin"
	"fmt"
)
func main() {
	r := gin.Default()
	r.GET("/login", login)
	r.POST("/form_post", formPost)
	r.Run(":8080")
}
func login(c *gin.Context) {
	username := c.Query("username")
	password := c.Query("password")
	fmt.Println(username, password)
}
func formPost(c *gin.Context) {
	message:= c.PostForm("message")
	nick := c.DefaultPostForm("nick", "anonymous")
	c.JSON(200, gin.H{
		"status":  "posted",
		"message": message,
		"nick":    nick,
	})
}
```
