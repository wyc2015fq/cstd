# golang的ssh包 - weixin_33985507的博客 - CSDN博客
2016年04月05日 16:56:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
git clone [https://github.com/golang/crypto.git](https://github.com/golang/crypto.git)，复制到 golang.org/x/ 目录下。
常常用来建立ssh连接发送一条命令，但有时需要模拟ssh客户端功能，即直接连接上远程主机一直操作：（http://www.zhihu.com/question/30368538/answer/47835043）
```
package main
import (
    "golang.org/x/crypto/ssh"
    "log"
    "os"
)
func main() {
    ce := func(err error, msg string) {
        if err != nil {
            log.Fatalf("%s error: %v", msg, err)
        }   
    }   
    client, err := ssh.Dial("tcp", "172.16.116.200:22", &ssh.ClientConfig{
        User: "root",
        Auth: []ssh.AuthMethod{ssh.Password("^Two^Ten=1024$")},
    })  
    ce(err, "dial")
    session, err := client.NewSession()
    ce(err, "new session")
    defer session.Close()
    session.Stdout = os.Stdout
    session.Stderr = os.Stderr
    session.Stdin = os.Stdin
    modes := ssh.TerminalModes{
        ssh.ECHO:          0,  
        ssh.TTY_OP_ISPEED: 14400,
        ssh.TTY_OP_OSPEED: 14400,
    }   
    err = session.RequestPty("xterm", 25, 80, modes)
    ce(err, "request pty")
    err = session.Shell()
    ce(err, "start shell")
    err = session.Wait()
    ce(err, "return")
}
```
　　实际使用中有缺陷，通过 tab 补全时并不能正确显示。还是直接使用shell编写吧，简单实用：
```
#!/usr/bin/expect 
set timeout 30
spawn ssh root@122.116.16.200
expect "password:"
send "123456\r"
interact
```
