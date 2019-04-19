# 揭秘 Reddit 愚人节项目的技术实现过程 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [一杯哈希不加盐](http://www.jobbole.com/members/snowhere) 翻译。未经许可，禁止转载！
英文出处：[Reddit](https://redditblog.com/2017/04/13/how-we-built-rplace/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
![](https://i2.wp.com/redditupvoted.files.wordpress.com/2017/04/place-final.png?crop=0px%2C0px%2C1000px%2C483px&resize=1200%2C580&ssl=1)
每年的愚人节，我们喜欢创建项目来探索人类大规模的交流互动，而不是做一些恶作剧。今年我们提出了 r/Place，这是一个协作的画板，每个用户每 5 分钟只能修改一个小块。这一限制弱化了个体的重要性，强化了大量用户协作完成复杂创作的必要性。每个小块的变化实时传递给观察者。
许多开发团队（前端、后端、移动端）协作开发这个项目，项目大部分基于 Reddit 已有的技术。这篇文章从技术角度详细描述我们如何完成 r/Place。
且慢。如果你想查看我们的代码，[在这里](http://github.com/reddit/reddit-plugin-place-opensource)。如果你对构建 r/Place 这一类项目感兴趣，[我们欢迎你](https://about.reddit.com/careers/)！
## 需求
定义愚人节项目的需求十分重要，因为它一旦发布即面向所有 Reddit 用户，没有增长过程。如果它一开始并不能完美运作，似乎就不能吸引足够的用户来创作并获得有趣的体验。
- 画板必须有 1000×1000 个小块，所以它会非常大。
- 所有客户端必须和当前画板状态同步，并显示一致，否则用户基于不同版本的画板难以协作。
- 我们必须支持至少 100000 的并发同步用户。
- 用户每 5 分钟可以修改一个小块，所以我们必须支持平均每 5 分钟 100000 个小块的更新（每秒 333 个更新）。
- 项目的设计必须遵循这一点，即使 r/Place 流量巨大，也不能影响站点其他功能。
- 配置必须有足够弹性，应对意外的瓶颈或故障。这意味着画板的大小和小块的使用间隔可以在运行时调节，以防数据量过大或更新过于频繁。
- API 必须开放和透明，reddit 社区如果对此有兴趣，可以在此之上构建项目（机器人、扩展、数据收集、外部可视化等等）。
## 后端
### 实施决策
后端最大的挑战就是保持所有客户端与画板状态同步。我们的解决方案是初始化客户端状态时立刻实时监听小块的变化，然后请求整个画板状态。只要我们在生成画板的时候有实时的小块更改，那么响应返回的整个画板状态就会有几秒的延迟。当客户端接收到整个画板状态，把在等待请求时的小块变化在画板上重演。之后接收到所有小块变化实时绘制在画板上。
为了让这个策略正常实施，我们需要尽可能快的请求到画板的整体状态。我们的初步方案是用[单行 Cassandra](https://pandaforme.gitbooks.io/introduction-to-cassandra/content/understand_the_cassandra_data_model.html) 储存整个画板，每个针对整个画板的请求可以读取整行。行中的每列格式如下所示：
`(x, y): {‘timestamp’: epochms, ‘author’: user_name, ‘color’: color}`
因为画板包含一百万个小块，这意味着我们不得不读取有一百万列的行。在我们的生产集群上这种读取花费 30 秒，慢到无法接受，所以我们不能过度依赖 Cassandra。
我们下一个方案使用 redis 储存整个画板。我们使用 bitfield 处理一百万个 4 位的整型。每个 4 位的整型可以编码 4 位的颜色，横纵（x，y）坐标可以在 bitfield 里用偏移量表示（offset = x + 1000y）。我们可以通过读取整个 bitfield 来获取整个画板的状态。我们可以通过在 bitfield 中更新指定偏移量上的值，来更新单独的小块(不再需要加锁或读/改/写)。我们仍然需要在 Cassandra 中储存所有的细节，让用户可以检查单独的小块，看一看何时何人更改了它。我们也计划用 Cassandra 备份整个画板，以防 redis 失效。从 redis 中读取整个画板不超过 100ms，这已经足够快了。
插图展示了我们如何用 redis 储存 2×2 画板的颜色：
![](https://redditupvoted.files.wordpress.com/2017/04/drawio-1.png?w=720&h=199)
我们非常关心 redis 读取最大带宽。如果很多客户端同时链接或刷新，它们会同时请求整个画板的状态，全部都触发 redis 的读取操作。因为画板是全局共享状态，显而易见的解决方案是使用缓存。我们决定在 CDN 层（Fastly）使用缓存，因为实现简单，并且缓存离客户端更近可以提高响应速度。对整个画板的请求被 Fastly 缓存下来并设置 1 秒的超时时间。我们也添加了 [stale-while-revalidate](https://docs.fastly.com/guides/performance-tuning/serving-stale-content#usage) 这个控制缓存的头信息，来应对画板缓存过期导致超过预期的大量请求。[Fastly 维护着大约 33 处独立缓存 POPs（接入点）](https://www.fastly.com/network-map)，所以我们预期每秒最多有 33 个针对整个画板的请求。
我们使用我们的 [websocket 服务](https://github.com/reddit/reddit-service-websockets) 向所有客户端推送更新。我们已经成功地在 [reddit live](https://www.reddit.com/live) 生产环境中应用过它，来处理超过 100000 的并发用户，比如 live PM notifications 功能或其他特性。wesocket 服务也曾是我们过去愚人节项目的基础，比如 [The Button](https://redditblog.com/2015/04/01/the-button/) 和 [Robin](https://redditblog.com/2016/04/01/Robin/) 两个项目。对于 r/Place 项目，客户端维护一个 websocket 链接来接收实时的小块变化更新。
### API
**检索整个画板**
![](https://redditupvoted.files.wordpress.com/2017/04/board-bitmap.png?w=720&h=368)
请求首先到达 Fastly。如果那里有一份未过期的画板副本，它会立刻返回从而不需要访问 reddit 应用服务器。否则如果缓存未命中或副本过时，reddit 应用会从 redis 中读取整个画板然后返回到 Fastly 中并缓存，并返回给客户端。
reddit 应用测量的请求速率和响应时间：
![](http://wx1.sinaimg.cn/large/7cc829d3gy1fgiu8lqg7tj20jw08v3z5.jpg)
注意，请求速率从没超过 33 个/秒，说明 Fastly（CDN） 缓存非常给力，阻止了大量直接访问 reddit 应用的请求。
![](http://wx4.sinaimg.cn/large/7cc829d3gy1fgiu8mejg3j20jt08nwf9.jpg)
当请求访问 reddit 应用时，redis 的读取操作非常迅速。
**绘制一个小块**
![](https://redditupvoted.files.wordpress.com/2017/04/draw.png?w=720&h=533)
绘制一个小块的步骤如下：
- 从 Cassandra 读取用户上一次更改小块的时间戳。如果和当前时间间隔比冷却时间（5 分钟）短，拒绝绘制请求，返回给用户一个错误。
- 向 redis 和 Cassandra 写入小块详情。
- 向 Cassandra 写入用户上一次修改小块的时间戳。
- 让 websocket 服务向所有链接的客户端发送新的小块。
Cassandra 的所有读写操作的[一致性设置为 QUORUM 级别](http://docs.datastax.com/en/archived/cassandra/1.2/cassandra/dml/dml_config_consistency_c.html)，来确保强一致性。
我们当然也有竞态条件允许用户一次更改多个小块。在步骤 1-3 中并没有锁，因此批量小块修改的操作通过步骤 1 的检查之后将在步骤 2 中进行修改。看起来一些用户发现了这个漏洞或一些机器脚本不遵守速率限制，所以大概有 15000 个小块被利用这个漏洞进行更改（占全部更改小块的 0.09%）
reddit 应用测量的请求速率和响应时间：
![](http://wx4.sinaimg.cn/large/7cc829d3gy1fgiu8mu894j20jn08rwf3.jpg)
我们经历了更改小块最大速率大概 200/s。这比我们估算的最大速率 333/s 要低（平均每 5 分钟 100000 个用户更改小块）。
![](http://wx1.sinaimg.cn/large/7cc829d3gy1fgiu8o0satj20jn08o3ze.jpg)
**获取单个小块详情**
![](https://redditupvoted.files.wordpress.com/2017/04/pixel1.png?w=720&h=251)
直接从 Cassandra 请求单个小块。
*reddit 应用测量的请求速率和响应时间：*
*![](http://wx2.sinaimg.cn/large/7cc829d3gy1fgiu8oq24dj20jt08jwf4.jpg)*
这个服务端点用的很多。除了客户端频繁的请求之外，有人编写抓取工具每次检索整个画板的一个小块。由于这个服务端点没有在 CDN 缓存，所有请求被 reddit 应用程序处理。
![](http://wx4.sinaimg.cn/large/7cc829d3gy1fgiu8p6m34j20jp08q754.jpg)
在整个项目中，这些请求的响应时间非常迅速稳定。
### Websockets
我们并没有在 websocket 服务中为 r/Place 做单独指标，但是我们可以估计并减去项目开始前后的基本使用量。
*websocket 服务总连接数：*
*![](http://wx1.sinaimg.cn/large/7cc829d3gy1fgiu8q6c2yj20jr08qt9b.jpg)*
r/Place 开始前的基本使用量大概有 20000 个连接，而峰值 100000 个链接，所以高峰期我们大概有 80000 个用户连接到 r/Place。
*Websocket 服务带宽：*
*![](http://wx2.sinaimg.cn/large/7cc829d3gy1fgiu8qrc27j20js08p0tk.jpg)*
高峰期 r/Place 的 websocket 服务吞吐量超过 4gbps（24个实例，每个 150 Mbps）
## 前端：Web和移动端
构建 r/Place 的前端工程涉及到了跨平台开发的众多挑战。我们期望 r/Place 在我们所有主流平台上拥有无缝体验，包括桌面web、移动web、iOS 和 Android。
r/Place 的 UI 需要做三件很重要的事：
- 实时展示画板状态。
- 让用户和画板交互方便容易
- 在我们所有平台上正常运行，包括移动端 app。
UI 的主要焦点集中在了 canvas，并且 [Canvas API](https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API) 完全能胜任要求。我们使用一个 1000 x 1000 的 `<canvas>` 元素，把每个小块当做一个像素进行绘制。
### 绘制 canvas
canvas 需要实时展示整个画板的状态。我们需要在页面载入的时候绘制整个画板的状态，然后更新通过 websocket 传输过来的画板状态。通过 [CanvasRenderingContext2D](https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D) 接口，有三种方式更新 canvas 元素。
- 用 [drawImage()](https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/drawImage) 将一个存在的图像绘制进 canvas。
- 通过众多图形绘制的方法来绘制各种形状，比如用 [fillRect()](https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/fillRect) 绘制一个有颜色的矩形。
- 构造一个 ImageData 对象，然后用 [putImageData()](https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/putImageData) 方法将它绘制进 canvas。
第一种选项并不适合我们，因为我们并没有画板的图像格式，还剩下 2、3 选项。用`fillRect()`方法更新单独的小块非常简洁：当 websocket 通知更新时，只需要在(x,y)位置处绘制一个 1 x 1 的矩形。一般来说这很棒，但并不适合绘制画板的初始状态。`putImageData()`方法显然更合适，我们可以在 ImageData 对象中定义每个像素的颜色，然后一次性绘制整个 canvas。
**绘制画板的初始状态**
我们使用`putImageData()`方法，前提需要将画板状态定义成 [Uint8ClampedArray](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Uint8ClampedArray) 形式，每个值用 8 位无符号整型表示 0-255 之间的数字。每一个值表示单个颜色通道（红、绿、蓝、alpha），每个像素需要 4 个值组成的数组。一个 2 x 2 的 canvas 需要一个 16 字节的数组，前 4 字节表示 canvas 左上角的像素，最后 4 字节表示右下角像素。
插图展示了 canvas 像素和 Uint8ClampedArray 映射关系：
![](https://redditupvoted.files.wordpress.com/2017/04/drawio-2.png?w=720&h=294)
对于 r/Place 的 canvas，数组大小是四百万字节，也就是 4MB。
在后端，画板状态储存格式是 4 位的 bitfield。每个颜色用 0 到 15 之间的数字表示，这允许我们将 2 像素的颜色信息打包进 1 个字节（1字节=8位）。为了在客户端配合使用，我们需要做 3 件事：
- 将二进制数据从我们的 API 拉取到客户端。
- “解压”数据
- 将 4 位颜色映射成可用的 32 位颜色。
为了拉取二进制数据，我们在支持 [Fetch API](https://developer.mozilla.org/en-US/docs/Web/API/Fetch_API) 的浏览器中使用此 API。在不支持的浏览器中，我们使用 [XMLHttpRequest](https://www.youtube.com/watch?v=Pubd-spHN-0)，并把 responseType 设置为 `“arraybuffer”`。
我们从 API 接收到的二进制数据中，每个字节有 2 像素的颜色数据。[TypedArray](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/TypedArray) 的构造函数允许操作的最小单位是 1 字节。这在客户端上并不方便使用，所以我们做的第一件事就是“解压”，让数据更容易处理。方式很简洁，我们遍历打包的数据并按照高位低位分割比特位，将它们复制到另一个数组的不同字节中。最后，4 位的颜色值映射成可用的 32 位颜色。
![](http://wx4.sinaimg.cn/large/7cc829d3gy1fgiu8qtp38j20k703qt8z.jpg)
`ImageData`这种数据结构需要使用`putImageData`方法，最终结果要求是可读的`Uint8ClampedArray`格式并且颜色通道字节要按照 RGBA 这种顺序。这意味着我们要做另一遍“解压”，将每个颜色拆分成颜色通道字节并按顺序排列。每个像素要做 4 次操作,这不是很方便，但幸运的是有其他方式。
`TypeArray`对象们本质上是`ArrayBuffer`的数组视图，实际上表示二进制数据。它们共同的一点就是多个`TypeArray`实例可以基于一个`ArrayBuffer`实例进行读写。我们不必将 4 个值写入 8 位的数组，我们可以直接把单个值写入一个 32 位的数组。使用`Uint32Array`写入值，我们可以通过更新数组单个索引来轻松更新单个小块颜色。我们唯一需要做的就是把我们的颜色字节逆序储存（ABGR），这样一来使用`Uint8ClampedArray`读取数据时可以自动把字节填入正确位置。
![](http://wx3.sinaimg.cn/large/7cc829d3gy1fgiu8r9pxaj20k403u74h.jpg)
**处理 websocket 更新**
响应每个像素更新时，用`drawRect()`方法绘制它们很方便，但这有个缺点：当大量更新在同一时间来到，会影响浏览器性能。我们知道画板状态更新十分频繁，所以我们需要处理这个问题。
我们希望在一个时间点前后的 websocket 更新能够批量绘制一次，而不是每次 websocket 更新来到就立刻重新绘制 canvas。我们做了以下两点改变：
- 因为我们发现了使用`putImageData()`一次更新多个像素这条明路，所以我们不再使用`drawRect()`。
- 我们把绘制 canvas 操作放到`requestAnimationFrame`循环中。
把绘制移到动作循环中，我们可以及时将 websocket 更新写入`ArrayBuffer`，然后延迟绘制。每一帧（大概 16ms）间的 websocket 更新会再一次绘制中批量执行。因为我们使用`requestAnimationFrame`，这意味着每次绘制时间不能太长（不超过 16ms），只有 canvas 的刷新速率受影响（而不是拖慢整个浏览器）。
### Canvas 的交互
还有非常重要的一点，canvas 需要方便用户的交互。用户与 canvas 核心交互方式是更改上面的小块。在 100% 缩放下，精确地选择绘制单个像素很不方便，而且容易出错。所以我们需要放大显示（放大很多）。我们也需要方便的平移 canvas，因为在多数浏览器上它太大了（尤其是放大后）。
**视角缩放**
用户只能每五分钟绘制一次小块，所以选错小块非常令人不爽。我们需要把 canvas 放大到每个小块都成为一个相当大的目标。这在触摸设备上尤其重要。我们使用 40x 的放大比例，给每个小块 40 x 40 的目标区域。为了应用缩放，我们把`<canvas>`元素包裹进一个`<div>`，并给 div 设置 CSS 属性`transform: scale(40, 40)`。这样一来，小块的布置变得非常方便，但整个画板的显示并不理想（尤其是在小屏幕上），所以我们混合使用两种缩放级别：40x 用于绘制，4x 用于显示。
使用 CSS 来放大 canvas 使得绘制画板的代码和缩放代码相分离，但不巧这种方式也带来一些问题。当放大一个图片（或 canvas），浏览器默认使用“平滑”算法处理图片。这适用于一些场景，但也彻底毁灭了像素艺术并把它变得混乱模糊。好消息是有另一个 CSS [image-rendering](https://developer.mozilla.org/en-US/docs/Web/CSS/image-rendering) 允许我们命令浏览器不这么做。坏消息并不是所有浏览器完全支持这个属性。
*坏消息，变得模糊：*
![](https://redditupvoted.files.wordpress.com/2017/04/www-reddit-com-place-webviewtrue-1.png?w=720&h=251)
我们需要在那些浏览器上用其他方式放大 canvas。我之前提到过绘制 canvas 有三种方式。其中第一个是`drawImage()`方法，它可以把一个存在的图像或另一个 canvas 绘制进一个 canvas。它也支持在绘制的时候放大或缩小图像，虽然放大的时候会和在 CSS 中放大一样出现模糊问题，但是可以通过关闭 [CanvasRenderingContext2D.imageSmoothingEnabled](https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/imageSmoothingEnabled) 标识，这种跨浏览器兼容性的方式来解决。
所以修复模糊 canvas 问题的答案就是在渲染过程中增加额外一步。我们引入了另一个`<canvas>`元素，它大小位置适应于容器元素（比如画板的可见区域）。每次重新绘制 canvas 后，我们使用`drawImage()`把它的一部分绘制到新的、有合适缩放比例的 canvas。因为额外的步骤给渲染过程带来微小的开销，所以我们只在不支持`image-rendering`CSS 属性的浏览器上这样做。
**视角平移**
canvas 是一个相当大的图像，尤其是放大之后，所以我们需要提供一些方式操作它。为了调整 canvas 在屏幕上的位置，我们采取和解决缩放问题一样的方式：我们将`<canvas>`包裹进另一个`<div>`，并在它上面应用 CSS 属性`transform: translate(x, y)`。使用单独的 div 使得应用在 canvas 上的变换操作更容易控制，这对于防止视角在缩放时产生移动非常重要。
我们最后支持多种方式调整视角位置，包括：
- 点击拖拽
- 点击移动
- 键盘导航
每种操作需要一点不同的实现方式。
**点击拖拽**
最基本的导航方式就是点击拖拽（或触摸拖拽）。我们保存了`mousedown`事件的 x、y 坐标。对于每次`mousemove`事件，我们计算鼠标相对于起点的偏移量，然后把偏移量加到已存在的 canvas 偏移量中。视角位置立刻改变，让人感觉这种到导航方式很灵敏。
**点击移动**
我们也支持点击一个小块，使得小块定位到屏幕中心。为了实现这个功能，我们需要跟踪`mousedown`和`mouseup`事件，为了区别“点击”和“拖动”。如果鼠标移动距离达不到“拖动”的标准，我们会根据鼠标位置和屏幕中心的距离来调整视角位置。和点击拖动不同，视角位置的更新使用了缓动函数（easing function）。我们没有立刻设定新的位置，而是把它保存成“目标”位置。在动画循环中（每次绘制 canvas 的循环），我们使用缓动函数移动当前视角逐渐接近目标。这避免了视角移动太突然。
**键盘导航**
我们也支持键盘导航，既可以使用 WASD 键也可以使用方向键。四个键控制内置 **移动向量**。没有按键按下时，向量默认是 (0, 0)，每个按键按下时会增加或减少向量的 x 或 y 轴 1 个单位。举个例子，按下“右”和“上”键会把移动向量设置成 (1,-1)。这个移动向量随后应用在动画循环中，来移动视角。
在动画循环中，移动速度是基于当前缩放级别而计算出来的，公式如下：
`movementSpeed = maxZoom / currentZoom * speedMultiplier`
在缩小状态下，键盘导航移动速度更快，这样显得更自然。
移动向量[单位化](http://mathworld.wolfram.com/NormalizedVector.html)并乘以移动速度，然后应用到当前视角位置。我们用单位向量来确保对角线移动和正交移动拥有相同速度，这也显得更自然。最后我们对移动向量自身的变化也使用了缓动函数。这使得移动方向和速度变化的更平滑，视角变得流畅生动。
### 移动应用支持
在 iOS 和 Android 的移动应用嵌入 canvas 过程中，我们遇到一些挑战。首先，我们需要认证用户，然后用户才能更改小块。和基于 session 的 web 认证不同，移动应用中我们使用 OAuth。这意味着应用需要为 webview 提供当前登录用户的访问令牌。最安全的方式就是用 JavaScript 在应用调用 webview 时注入 oauth 认证头信息（这也允许我们设置其他需要的头信息）。问题就简化为在每个 api 调用中传递认证头信息了。
`r.place.injectHeaders({‘Authorization’: ‘Bearer <access token>’});`
在 iOS 端，当你可以更改 canvas 中的下一个小块时，我们实现了消息提醒功能。因为小块的变更完全在 webview 中，所以我们需要实现向原生应用的回调。辛运的是在 iOS 8 及以上版本中只需要一个简单的 JavaScript 调用：
`webkit.messageHandlers.tilePlacedHandler.postMessage(this.cooldown / 1000);`
应用中的委派方法根据传入的冷却计时器，会随后调度发送一条通知。
![](https://redditupvoted.files.wordpress.com/2017/04/place-notif.png?w=720&h=210)
## 我们学到了什么
### 你总会疏漏一些事
我们完美计划好了任何事情，我们知道上线时，没有什么可能出错的地方。我们对前端和后端分别进行了负载测试，我们不可能再遇到其他错误。
真的吗？
上线过程很顺利。经历了一个黎明，r/Place 人气迅速上升，我们 websocket 实例的链接数量和通信量也随之增加：
![](http://wx3.sinaimg.cn/large/7cc829d3gy1fgiu8s6hfaj20js0hvq44.jpg)
并没有什么惊喜，所有和我们预期的一样。奇怪的是，我们怀疑限制了这些服务器实例的网络带宽，因为我们预计会有更大的流量。查看了一下 CPU 的实例情况，却显示出一幅不同的图片：
![](http://wx3.sinaimg.cn/large/7cc829d3gy1fgiu8su5fdj20jt08mmxt.jpg)
服务器实例是 8 核的，所以很明显它们快到上限了。为什么它们突然表现的如此不同？我们将原因归结于 r/Place 的工作负载类型不同于以往项目。毕竟这里有很多微小的消息，我们一般发送大型消息，比如直播帖子的更新和通知。我们也没有处理过大量用户接收相同消息的情况，所以有很多地方都不同。
这没什么大不了，我们预计只需要调用和测量它一天而已。待命的运维数量是服务器实例的两倍，而且他们两耳不闻窗外事，一心只顾服务器。
然后，发生了这个：
![](http://wx1.sinaimg.cn/large/7cc829d3gy1fgiu8tjp99j20jq08rwez.jpg)
这幅图看上去可能并没什么，但事实上这是我们生产环境的 Rabbit MQ 实例，不仅处理 websocket 消息，也处理 reddit.com 所有底层的依赖项。这不容乐观，一点都不。
经过了各种调查、束手无策和升级实例，我们把问题锁定在管理接口。它总是有点慢，随后我们意识到，我们为了获取项目状态用 [rabbit diamond collector](https://github.com/python-diamond/Diamond/blob/master/src/collectors/rabbitmq/rabbitmq.py) 会频繁查询接口。我们认为创建新的 websocket 实例时创建了额外的 exchange （RabbitMq 中概念），再加上这些 exchange 的消息吞吐量，导致了管理界面在查询和记录时，rabbit 卡住了。我们把它关掉，情况好多了。
但我们不想在一片黑暗之中（关掉了状态显示程序），所以我们迅速做了一个手工艺术品——监控脚本，帮助我们观察整个项目：


```
$ cat s****y_diamond.sh
#!/bin/bash
/usr/sbin/rabbitmqctl list_queues | /usr/bin/awk '$2~/[0-9]/{print "servers.foo.bar.rabbit.rabbitmq.queues." $1 ".messages " $2 " " systime()}' | /bin/grep -v 'amq.gen' | /bin/nc 10.1.2.3 2013
```
如果你好奇为什么我们不断调整修改像素（小块）的超时时间（冷却时间），那么现在你就知道了。我们试着减轻服务器压力以便整个项目顺利运行。这也是为什么在某个时间段，一些像素的呈现花费较长时间。
所以不幸的是，你当时坚信如下的信息：
[10K upvotes to reduce the cooldown even further! *ADMIN APPROVED*](https://www.reddit.com/r/Place/comments/62xms3/10k_upvotes_to_reduce_the_cooldown_even_further/?ref=share&ref_source=embed&utm_content=title&utm_medium=post_embed&utm_name=8f15a84993df435b970ed0f5e2bf05d8&utm_source=embedly&utm_term=62xms3)
尽管调整完看 r/Place/new 版块很有意思，但调整完全出于技术原因：
![](https://redditupvoted.files.wordpress.com/2017/04/place-page.png?w=720&h=475)
或许这也是调整的部分动机。
### 机器人终归是机器人
我们在项目的末期经历了一点小波折。一般来说，我们经常遇到的问题之一便是客户端重试行为。众多客户端在遇到问题时便不停地重试。这意味着站点一旦有点小问题，对于那些没有对故障进行[回退](https://en.wikipedia.org/wiki/Exponential_backoff)编程的一些客户端，很容易引发重试风暴。
当我们关闭 r/Place 时，很多机器人端点请求时返回非 200 的响应码。像这样的[代码](https://github.com/PlaceStart/placestart/blob/7d27beacc75dcf84d634ea32a5bf0421bf2c042a/monitor.py#L114-L125)不是十分友好。值得庆幸的是，在 Fastly 层很容易拦截它们。
## 创造点其他东西
如果没有庞大团队的协作，项目不会这么成功。我们很感谢 u/gooeyblob、u/egonkasper、u/eggplanticarus、u/spladug、u/thephilthe、u/d3fect 等人对 r/Place 团队的贡献，让愚人节的尝试变成现实。
正如我们之前提到的，如果你对为百万用户创造特殊体验感兴趣，看一看我们的[招聘页](https://about.reddit.com/careers/)。
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2017/01/5085cce1bd4e144b7d148079a34b20c1.jpg)![](http://jbcdn2.b0.upaiyun.com/2017/01/02b09a29fdb3ab5fe03d4bd1afe95664.jpg)
