# express 清除session - weixin_33985507的博客 - CSDN博客
2018年01月13日 21:37:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
原文链接：[http://www.jb51.net/article/111989.htm](https://link.jianshu.com?t=http%3A%2F%2Fwww.jb51.net%2Farticle%2F111989.htm)
```
let identityKey = 'skey';
 
//使用session
app.use(session({
  name: identityKey,
  secret: 'chyingp', // 用来对session id相关的cookie进行签名
  store: new FileStore(), // 本地存储session（文本文件，也可以选择其他store，比如redis的）
  saveUninitialized: false, // 是否自动保存未初始化的会话，建议false
  resave: false, // 是否每次都重新保存会话，建议false
  cookie: {
    maxAge: 10 * 1000 // 有效期，单位是毫秒
  }
}));
......
//清除session
app.get('/logout', function(req, res, next){
  // 备注：这里用的 session-file-store 在destroy 方法里，并没有销毁cookie
  // 所以客户端的 cookie 还是存在，导致的问题 --> 退出登陆后，服务端检测到cookie
  // 然后去查找对应的 session 文件，报错
  // session-file-store 本身的bug  
 
  req.session.destroy(function(err) {
    if(err){
      res.json({ret_code: 2, ret_msg: '退出登录失败'});
      return;
    }
     
    // req.session.loginUser = null;
    res.clearCookie(identityKey);
    res.redirect('/');
  });
});
```
