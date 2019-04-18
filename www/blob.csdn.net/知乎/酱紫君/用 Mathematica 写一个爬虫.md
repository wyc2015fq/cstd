# 用 Mathematica 写一个爬虫 - 知乎
# 

所谓爬虫, 说白了就是替你浏览网页, 然后帮你把你要的信息存下来的脚本.

下面我们就以 B站 为例, 写一个**静态爬虫**.

## 构造请求

所谓静态爬虫, 就是只管静态部分, 不管各种加载的简单爬虫.

通过分析网页的请求可以找到各种各样的接口, 然后直接访问就行了.

比如 tag 标签, 可以发现浏览器向`http://api.bilibili.com/tags/info_description?id=12`发送了一个 GET 请求, 那么我们访问这个地址就能获得数据了.

```
>> URLExecute["http://api.bilibili.com/tags/info_description?id=12","RawJSON"]
<|
    "code"->, 
    "result"-><|
        "tag_id"->,
        "name"->"公告",
        "cover"->"",
        "subscribed"->,
        "visit_count"->,
        "subscribe_count"->
    |>
|>
```

ok, 爬虫教程结束...

好吧开玩笑的, 这种一行的爬虫半分钟都活不过去的...

**爬虫重点不是在爬, 而是如何与反爬程序斗智斗勇.**

虽说直接写地址也没关系, 但是如果他们突然改了加了验证啥的就不好玩了...

所以最好充分解耦, 用一个 HTTPRequest 来替代.

```
TagURL[tid_]:=HTTPRequest[
    "http://api.bilibili.com/tags/info_description?id=" <> ToString[tid],
    TimeConstraint->
];
```

有时候会很复杂, 又要 POST, 又要查 appkey 啥的, 比如这个用户信息.

```
MemberURL[mid_] := HTTPRequest[
    "https://space.bilibili.com/ajax/member/GetInfo",
    <|
        "Method" -> "POST",
        "Headers" -> {
            "content-type" -> "application/x-www-form-urlencoded;charset=UTF-8",
            "user-agent" -> RandomSample@$BilibiliLinkUA,
            "Referer" -> "https://space.bilibili.com/" <> ToString[mid]
        },
        "Body" -> "mid=" <> ToString[mid] <> "&csrf="
    |>,
    TimeConstraint -> 
];
```

然后搞个 UA 表, 像这样的:

```
$BilibiliLinkUA = {
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; AcooBrowser; .NET CLR 1.1.4322; .NET CLR 2.0.50727)",
    "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0; Acoo Browser; SLCC1; .NET CLR 2.0.50727; Media Center PC 5.0; .NET CLR 3.0.04506)",
    "Mozilla/5.0 (X11; U; Linux x86_64; zh-CN; rv:1.9.2.10) Gecko/20100922 Ubuntu/10.10 (maverick) Firefox/3.6.10"
};
```

这就稳了, 对面再也没法通过 URL 请求来判定你的身份了.

虽说 Heads 都是君子协议, 一般也不看, 但是有的接口他偏要验人也没办法...

## 任务划分

接下来切分任务, 任务的最小单位是一次请求, 最大单位就是所需要的一切数据了.

但是呢, 我们也不指望它一次完成, 讲道理我们甚至希望他是分布式的...

好吧分布式这个有点超纲了, 但我们还是要按这个标准来写.

假如现在 B站 有800万 tag 吧.

我们把这个大任务划分为小任务, 100万为一个批次 , 每个批次分 1000 个区块, 每个区块包含 1000 个请求.

分布式情况下这个区块就是发给每个节点的任务, 每个节点本身是并行或者异步的.

批次这个概念是没必要的, 但是单机的情况下有必要, 个人 PC 的内存和硬盘读写都有限, 比如现在用户数快4亿了, 单机甚至没法开这么大个队列出来, 只能分批处理.

我们来看如何写一个区块:

```
TagGet[i_, t_ /; t <= ] := AppendTo[i, $fail];
TagGet[i_, try_] := Block[
    {get = URLExecute[TagURL@i, "RawJSON"]},
    If[TagGetQ[get],
        AppendTo[$block, Inactive[TagGet][i, try - ]];
        retry++; Return[Nothing]
    ];
    finish++;
    TagFormat[get["result"]]
];
```

i 表示 id 编号, try 表示剩余的重试次数, 降到0就不再重试了.

这里还有几个辅助函数:

```
$fail = {};
retry = ;
finish = ;
TagGetQ[get_] := Or[FailureQ@get, get["code"] != ];
TagFormat[asc_] := <|
    "ID" -> asc["tag_id"],
    "Name" -> asc["name"],
    "Count" -> asc["visit_count"],
    "Watch" -> asc["subscribe_count"]
|>;
```

retry 和 finish 来自最上层, 用于监督进度.
用 $block 来保存这次的任务队列, $fail 用来保存重试还失败的任务.
Format 格式化输出, 反正都在等 IO, 不如先把要的都提取出来再说.
- 如果是用数据库的话这里写插入操作就行, 不过我不建议这么搞, 一是不好处理异常, 二是双向等 IO 没法优化.
- 如果要限速或者随机等待的话可以末尾加这个函数, 但是如果有代理就可以不用等待了.

```
TagRange[a_, b_] := Block[
    {$fail = {}, $this = {}, $now = Now, $block, $query},
    $block = Table[Inactive[TagGet][i, ], {i, a, b}];
    While[Length[$block] > ,
        {$query, $block} = {RandomSample[$block], {}};
        $this = Join[$this, Activate[$query]]
    ];
    <|
        "Now" -> $now,
        "Data" -> $this,
        "Failed" -> $fail,
        "Time" -> Now - $now
    |>
];
```

然后把这个函数分发一下就行了, 无论是并行还是分布式都行.

## 使用代理

代理这个比较麻烦了, Mathematica 没有内置代理选项, 那就只能底层 hack 一下了.

不过查查源码还是不难找到的, 这个`$InternetProxyRules`就是代理设置.

```
$WhoAmI[ip_ : Nothing, port_ : Nothing] := Block[
    {$InternetProxyRules = {
        "UseProxy" -> True,
        "HTTP" -> {ip, port},
        "HTTPS" -> {ip, port},
        "FTP" -> {},
        "Socks" -> {},
        "UseWPAD" -> True
    }},
    URLExecute[HTTPRequest["http://ip-api.com/json", TimeConstraint -> ], "RawJSON"]
];
```

可以直接测试一下. 啊不错, 我已经到德国去了.
![](https://pic1.zhimg.com/v2-6adb1c8b59f3ed7472cb59aee74664e0_b.jpg)
代理比较推荐的方式是挂个SS, 开本地代理, 全局代理以及随机策略负载均衡, 这样好处就是不用变这个设置, 有20个以上节点开满线程都不会被 ban IP 了.

反正我平常每个月流量也用不了几百个G, 速度也可以, 比野代理高到不知道哪里去了...

专栏高亮不太好看, 我放了一个 notebook 的版本在 github 上:
[https://github.com/Moe-Net/BilibiliLink/blob/master/Packages/Crawlers/TagCrawlers.nb​github.com](https://link.zhihu.com/?target=https%3A//github.com/Moe-Net/BilibiliLink/blob/master/Packages/Crawlers/TagCrawlers.nb)
哦, 还有开头那个一行爬虫能活5分钟的样子...单线程的话...

