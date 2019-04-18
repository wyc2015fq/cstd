# wget无法建立SSL连接 - xcw0754 - 博客园
# [wget无法建立SSL连接](https://www.cnblogs.com/xcw0754/p/5140716.html)
在使用wget工具的过程中，当URL使用HTTPS协议时，经常出现如下错误：“无法建立SSL连接”。
这是因为wget在使用HTTPS协议时，默认会去验证网站的证书，而这个证书验证经常会失败。加上"--no-check-certificate"选项，就能排除掉这个错误。

