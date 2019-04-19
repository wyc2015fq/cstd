# 通过信鸽来解释 HTTPS - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [tsteho](http://www.jobbole.com/members/q3198108035) 翻译，[艾凌风](http://www.jobbole.com/members/hanxiaomax) 校稿。未经许可，禁止转载！
英文出处：[freecodecamp](https://medium.freecodecamp.org/https-explained-with-carrier-pigeons-7029d2193351)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
密码学可能是一个难以理解的主题。它充满了数学证明。除非你真的需要开发密码系统，否则，如果你只想从宏观的角度了解密码学，你并不需要理解这些复杂的内容。
如果你抱着能够创建下一个 HTTPS 协议的期望打开这篇文章，我不得不遗憾地表示只有信鸽是不够的。否则，煮一些咖啡，享受这篇文章吧。
## [](https://github.com/iython/translation-project/blob/bb4444d36a0b5f65a783cbbb18def75fde267739/translation/HTTPS-explained-with-carrier-pigeons.md#alicebob-%E5%92%8C%E4%BF%A1%E9%B8%BD)Alice，Bob 和信鸽？
![](https://camo.githubusercontent.com/6723aa4811cb162a1fbb8e4525f43afd5672e7b9/68747470733a2f2f63646e2d696d616765732d312e6d656469756d2e636f6d2f6d61782f313630302f312a764846364e4e645a58397a6969575f7552597a7641412e706e67)
你在互联网上的任何活动（阅读这篇文章，在亚马逊上买东西，上传猫咪的图片）都归结为向服务器发送消息和从服务器接收消息。
这么讲听起来可能有点抽象，因此，我们不妨假设这些信息是通过**信鸽**传递的。我明白这么假设显得很随意，但请相信我：HTTPS 的工作原理就是这样的，只是快得多。
本文中我们并不会使用，服务器，客户端和黑客这样的术语，取而代之的是，我们会依次使用人名 Alice、 Bob 和 Mallory来代替它们。如果你不是第一次尝试理解密码概念，你可以认出这些名字，因为它们被广泛用于技术文献中。
## [](https://github.com/iython/translation-project/blob/bb4444d36a0b5f65a783cbbb18def75fde267739/translation/HTTPS-explained-with-carrier-pigeons.md#%E4%B8%80%E6%AC%A1%E7%AE%80%E5%8D%95%E7%9A%84%E9%80%9A%E8%AE%AF)一次简单的通讯
如果 Alice 想要给 Bob 传递一条信息，她将信息绑在信鸽的腿上，然后让信鸽传给 Bob。Bob 收到信息，读取信息。一切都正常。
但要是 Mallory 途中拦截了 Alice 的信鸽，并且改变了信息的内容？Bob 无法知道 Alice 发送的信息在传递途中被修改了。
这就是 **HTTP** 的工作原理。挺可怕的，对吧？我不会通过 HTTP 协议来发送我的银行凭证，你也不应该这么做。
## [](https://github.com/iython/translation-project/blob/bb4444d36a0b5f65a783cbbb18def75fde267739/translation/HTTPS-explained-with-carrier-pigeons.md#%E4%B8%80%E4%B8%AA%E5%AF%86%E4%BB%A4)一个密令
那如果 Alice 和 Bob 都很机灵呢。他们同意将使用密令来写信息。他们将字母表中的每个字母偏移 3 个位置。比如：D -> A, E -> B, F -> C。明文“secret message”将转换成“pbzobq jbppxdb”。
现在，如果 Mallory 拦截了信鸽，她既不能把信息改变成一些有意义的信息，也不能明白信息里说的内容，因为她不知道密令。但是Bob可以简单地反向应用密令（A -> D, B -> E, C -> F）将信息解密。密文“pbzobq jbppxdb”将被解密回“secret message”。
大功告成！
这被称为**对称密钥密码术**，因为当你知道如何加密一条信息，你也知道如何给信息解密。
我在上面介绍的密令通常被称为**凯撒密码**。在现实生活中，我们使用更高级和复杂的密令，但是主要思路是相同的。
## [](https://github.com/iython/translation-project/blob/bb4444d36a0b5f65a783cbbb18def75fde267739/translation/HTTPS-explained-with-carrier-pigeons.md#%E6%88%91%E4%BB%AC%E5%A6%82%E4%BD%95%E5%86%B3%E5%AE%9A%E5%AF%86%E9%92%A5%E6%98%AF%E4%BB%80%E4%B9%88)我们如何决定密钥是什么？
如果只有发送方和接收方知道密钥，对称密钥密码术是很安全的。在凯撒密码中，**密钥是一个偏移值**，这个偏移值决定每个字母应该偏移多少。在我们的例子中，我们使用的偏移值是 3，但是也能是 4 或者 12。
不过这么设计会有个问题：在用信鸽传递信息之前，如果 Alice 和 Bob 之前从没见过，他们没有安全的方式创建一个密钥。如果他们将密钥包含在信息之中，Mallory 将拦截信息并且发现密钥。后果就是：无论 Alice 和 Bob 发送的信息是否加密，Mallory 都能读取或者改变拦截到的信息。
这是一个典型的**中间人攻击**例子。避免它的唯一方法是改变之前的密码系统。
## [](https://github.com/iython/translation-project/blob/bb4444d36a0b5f65a783cbbb18def75fde267739/translation/HTTPS-explained-with-carrier-pigeons.md#%E6%90%BA%E5%B8%A6%E7%9B%92%E5%AD%90%E7%9A%84%E4%BF%A1%E9%B8%BD)携带盒子的信鸽
所以 Alice 和 Bob 想出了一个更好的系统。当 Bob 想要发送信息给 Alice 时，Alice 将遵照下面的流程：
- Bob 向 Alice 传送一只信鸽，信鸽不携带任何信息。
- Alice 将这只信鸽传回给 Bob，信鸽携带一只开着锁的盒子以及密钥。
- Bob 把信息放到盒子里，将锁锁上，将盒子传送给 Alice。
- Alice 收到盒子，使用密钥打开盒子，读取信息。
通过这种方式传递信息，Mallory 不可能通过拦截信鸽的方式来改变信息，因为她没有密钥。当 Alice 想要向 Bob 发送信息时，遵循相同的流程。
Alice 和 Bob 刚刚使用了通常所说的非对称密钥密码术。之所以称它为非对称，是因为即使你可以加密一条信息（锁上盒子）但你也不能将它解密（打开锁住的盒子）。
## [](https://github.com/iython/translation-project/blob/bb4444d36a0b5f65a783cbbb18def75fde267739/translation/HTTPS-explained-with-carrier-pigeons.md#%E6%88%91%E6%80%8E%E4%B9%88%E4%BF%A1%E4%BB%BB%E8%BF%99%E4%B8%AA%E7%9B%92%E5%AD%90)我怎么信任这个盒子？
如果你够仔细的话，你可能已经意识到我们仍然有一个问题。当 Bob 收到那个开着的盒子时，他如何确信这是来自 Alice 的盒子，而不是 Mallory 拦截信鸽后，将来自于 Alice 的盒子替换成 Mallory 自己设置了密钥后的盒子。
Alice 决定对盒子进行数字签名，通过这种方式，当 Bob 收到盒子，他通过核对签名的一致性来确定盒子是否来自 Alice。
有些人可能就会想 Bob 如何识别 Alice 的签名？不错的问题。Alice 和 Bob 也有同样的疑问。因此他们决定让 Ted 对盒子进行数字签名，而不是 Alice。
Ted 是谁？Ted 是一个著名且值得信赖的人。每个人都可以从Ted 那里获得签名，每个人都相信Ted 只会为合法的人提供盒子的数字签名服务。
只有当Ted 确信正在请求签名的人是 Alice，Ted 才会为 Alice 提供盒子数字签名的服务。因此 Mallory 不能再像之前那样拦截 Alice 的盒子、替换盒子后传送给 Bob 了，因为 Bob 会发现这个盒子在 Ted 那进行数字签名的是 Mallory，而不是 Alice。
Ted 在技术术语中通常被称为**证书颁发机构**，你阅读这篇文章所使用的浏览器安装着各种证书颁发机构的签名。
因此当你第一次连接到一个网站，你信任它的盒子，因为你信任 Ted，而 Ted 告诉你这个盒子是合法的。
## [](https://github.com/iython/translation-project/blob/bb4444d36a0b5f65a783cbbb18def75fde267739/translation/HTTPS-explained-with-carrier-pigeons.md#%E7%9B%92%E5%AD%90%E5%A4%AA%E9%87%8D%E4%BA%86)盒子太重了
Alice 和 Bob 现在有了一个可靠的通信系统，但是他们意识到和仅仅携带信息的信鸽相比，携带盒子的信鸽太慢了。
他们决定只在传递密钥的时候使用盒子的方法（非对称密码术），加密信息使用对称密码术（记得之前提到的凯撒密码？）。
这样的话可谓两全其美：非对称密码术的可靠性和对称密码术的效率都有了。
在现实世界中，“信鸽”的传送速度都很快，但尽管这么讲，使用非对称密码技术加密消息比使用对称密码技术慢，所以我们只使用它来交换加密密钥。
现在你知道了**HTTPS**的工作原理，你的咖啡也该煮好了。去喝吧，这是你应得的 😉
