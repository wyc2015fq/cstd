# redigo长连接/复用 - 三少GG - CSDN博客
2016年02月01日 00:56:44[三少GG](https://me.csdn.net/scut1135)阅读数：3210
Connecting to Redigo and manipulating data inside a function is easy like butter, but the problem comes when you have to re-use
 its connection, obviously for performance/practicality reasons.
**How do you go about re-using (not recreating everytime) a Redigo connection?**
最佳答案：

The best way turned out to be using Pools, which are briefly documented here: [Redigo
 Pools](https://github.com/garyburd/redigo/blob/master/redis/pool.go#L89).
A global variable won't eventually reuse a connection, so I ended up with something like this (using Pools as noted before):
```
func newPool() *redis.Pool {
return &redis.Pool{
            MaxIdle: 80,
            MaxActive: 12000, // max number of connections
            Dial: func() (redis.Conn, error) {
                    c, err := redis.Dial("tcp", ":6379")
                    if err != nil {
                            panic(err.Error())
                    }
                    return c, err
            },
    } 
}
var pool = newPool()
func main() {
        c := pool.Get()
        defer c.Close()
        test,_:=c.Do("HGETALL", "test:1")
        fmt.Println(test)
}
```
If for example you want to reuse a pool inside another function you do it like this:
```
func test() {
        c := pool.Get()
        defer c.Close()
        test2,_:=c.Do("HGETALL", "test:2")
        fmt.Println(test2)
}
```
###########
https://github.com/garyburd/redigo/blob/ee4f539b48a64737dd7a991cdebf1cb367ddb7fb/redis/pool_test.go
############
https://github.com/garyburd/redigo/issues/104
https://github.com/garyburd/redigo/issues/114
