
# lr 中cookie的解释与用法 - anlcy - 博客园






# [lr 中cookie的解释与用法](https://www.cnblogs.com/camilla/p/10283722.html)
Loadrunner 中 cookie 解释与用法
loadrunner 中与 cookie 处理相关的常用函数如下：
web_add_cookie()： 添加新的 cookie 或者修改已经存在的 cookie web_remove_cookie()： 删除指定的 cookie web_cleanup_cookies()： 删除所有被 Vuser 保存的 cookie web_reg_add_cookie()： 如果找到某些特定文字，则添加到 cookie 中
loadrunner 中 cookie 处理的经典问答： 问题 1：当录制 Vuser 脚本时，VuGen 处理 Cookie 吗？ VuGen 会 自 动 处 理 所 有 通 过 HTTP 标 头 设 置 的 Cookie 。 但 是 ， VuGen 无 法 始 终 正 确 地 处 理 由 JavaScript. 或 <meta-> 标记设置的 Cookie。
问题 2：当运行 Web Vuser 脚本时，Vuser 会重新使用在录制 Vuser 脚本时使用过的相同 Cookie 吗？ 可能会，也可能不会，这取决于 Cookie 的类型。
Cookies 可以分成两种类别：永久 Cookie 和会话 Cookie： 永久 Cookie： Web 服务器标识您的身份并且在有限的时间段内有效的纯文本字符串。 向 永久 Cookie 存 储在硬盘上。 会话 Cookie：仅在当前访问（会话）期间向 Web 服务器标识您的身份的纯文本字符串。会话 Cookie 不 会存储在硬盘上。
当录制 Web Vuser 脚本时，VuGen 会检测发送到浏览器的所有 Cookie。 VuGen 会按如下方式区分永久 Cookie 和会话 Cookie： 永 久 Cookie ： VuGen 将 永 久 Cookie 的 详 细 信 息 直 接 录 制 成 Vuser 脚 本 。 VuGen 使 用 web_add_cookie 将永久 Cookie 包括在 Vuser 脚本中。当运行 Vuser 脚本时，Vuser 会根据需要使用 这些永久 Cookie。 会话 Cookie：VuGen 不保存在录制会话期间所使用的会话 Cookie。而是在录制时缓存会话 Cookie，然 后在停止录制时丢弃它们。
当运行 Vuser 脚本时，Vuser 使用从 Web 服务器接收的新会话 Cookie。即，Vuser 不会重新使用在录 制脚本时生成的相同会话 Cookie。这些会话 Cookie 存储在 Vuser Cookie 缓存中，然后在 Vuser 停止 时被丢弃。Vuser 不会保存这些会话 Cookie。
问题 3：每个 Vuser 都有其自己唯一的 Cookie 缓存吗？ 是的，每个 Vuser 都有其自己唯一的 Cookie 缓存 - 会话 Cookie 不共享，即使 Vuser 在同一负载生 成器上运行。
问题 4：在运行脚本之前，必须在录制的 Vuser 脚本中将 Cookie 参数化吗？ 有时需要。 在录制脚本时， VuGen 会将永久 Cookie 复制到 Vuser 脚本中。 当运行 Vuser 脚本时， Vuser 会使用所录制的永久 Cookie。如果每个 Vuser 都需要唯一的永久 Cookie，那么您需要在 Vuser 脚本中 将 Cookie 参数化。
本文转自：http://www.cnblogs.com/bugua/archive/2012/10/15/2724074.html





