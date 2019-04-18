# Swift中 const 怎么表示 - weixin_33985507的博客 - CSDN博客
2016年11月23日 12:33:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
#### 在 OC 中 定义的 不可变 的静态变量通常是
> 
static NSString * const kAuthTokenKey = @"auth_token";
#### 到了Swift的转变 直接用let 修饰，即
> 
let kAuthTokenKey: String = "auth_token"
let 即代表不可变，这里的不可变的对象的首地址不能发生变更，即内存地址不能发生改变。
PS：有什么问题欢迎留言。
