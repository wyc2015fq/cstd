# windows环境下 生成git公钥和私钥 - DoubleLi - 博客园






## windows环境下 生成公钥和私钥
- 
上传代码到远程仓库的时候需要秘钥进行验证是否本人上传的。打开Git目录下的Git Bash
`输入ssh-keygen，回车`
- 可直接不输入路径，使用默认路径（c/Users/Administrator/.ssh/)
- 按照提示要输入两次密码，但是如果输入了，每次操作操作git库时都要输入密码，所以选择不输入密码
- 密码输入结束后，提示已生成秘钥
- 打开c:/Users/Administrator/.ssh/，在文件中，id_rsa是私钥文件，id_rsa.pub是公钥文件
- 将公钥文件内容添加到github中的ssh公钥
- 
回到git bash，输入
`ssh -T git@git.oschina.net`
- 
如果显示 'Welcome to [Git@OSC](mailto:Git@OSC), your username!'，则表示成功


## 修改本地的ssh remote url。不用https协议，改用git协议
- 
查看当前的remote url，可以看到是使用https协议进行访问的

```
git remote -v  

orgin https://github.com/xxx/xxx.git(fetch)  
orgin https://github.com/xxx/xxx.git(push)
```

- 
调整url，使用项目的ssh url
`git remote set-url origin git@github.com:xxx/xxx.git`

参考：http://jingyan.baidu.com/article/948f5924ee425dd80ff5f90c.html









