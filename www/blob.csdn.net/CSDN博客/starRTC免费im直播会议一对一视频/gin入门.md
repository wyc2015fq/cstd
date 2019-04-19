# gin入门 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年02月07日 11:49:55[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：253

Download and install it:
$ go get github.com/gin-gonic/gin
Import it in your code:
import "github.com/gin-gonic/gin"
(Optional) Import net/http. This is required for example if using constants such as http.StatusOK.
import "net/http"
router := gin.Default()
router.GET("/someGet", getting)
router.POST("/somePost", posting)
// By default it serves on :8080 unless a
// PORT environment variable was defined.
router.Run() // router.Run(":3000") for a hard coded port
Run your project
$ go run main.go

func main() {
router := gin.Default()
// Query string parameters are parsed using the existing underlying request object.
// The request responds to a url matching:
/welcome?firstname=Jane&lastname=Doe
router.GET("/welcome", func(c *gin.Context) {
firstname := c.DefaultQuery("firstname", "Guest")
lastname := c.Query("lastname") // shortcut for c.Request.URL.Query().Get("lastname")
c.String(http.StatusOK, "Hello %s %s", firstname, lastname)
})
router.Run(":8080")
}

