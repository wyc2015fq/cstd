# 【前-workbox-node模块1】CLI - weixin_33985507的博客 - CSDN博客
2018年09月04日 17:28:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
# 1、什么是workBox CLI
Workbox命令行界面（包含在workbox-cli包中）包含一个名为workbox的Node.js程序，该程序可以从兼容UNIX的命令行环境的Windows，macOS运行。 在引擎盖下，workbox-cli包装了workbox-build模块，并提供了一种通过灵活配置将Workbox集成到命令行构建过程的简便方法。
# 2、安装CLI
安装CLI很简单，只需在终端中运行以下命令即可。
```
npm install workbox-cli --global
```
应该能够在安装后运行命令`workbox --help`。
# 3、CLI Modes
CLI有四种不同的模式：
- wizard：为项目设置Workbox的分步指南。
- generateSW：为您生成一个完整的服务工作者。
- injectManifest：将资源注入到项目的预先缓存中。
- copyLibraries：将Workbox库复制到一个目录中。
## 3.1 wizard
Workbox wizard会询问有关本地目录设置以及要预先缓存的文件的一系列问题。 您的答案用于生成配置文件，然后在generateSW模式下运行时可以使用该配置文件。
大多数开发人员只需要运行一次向导，您就可以使用任何支持的构建配置选项手动自定义生成的初始配置文件。
## 3.2 generateSW
您可以使用Workbox CLI使用配置文件生成完整的service worker（如向导生成的文件）。
只需运行以下命令：
```
workbox generateSW  config.js
```
> 
适合generateSW的场景：
（1）需要预缓存文件
（2）有简单的运行时配置需求（例如，配置允许定义路由和策略）
> 
不适合generateSW的场景：
（1）想使用其他Service Worker功能（例如Web Push）。
（2）想要导入其他脚本或添加其他复杂的逻辑。
建议使用generateSW模式的开发人员如果对Workbox的内置预缓存和运行时缓存功能感到满意，就不需要自定义其service worker的行为。
## 3.3 injectManifest
对于希望更多控制其最终service worker文件的开发人员，可以使用injectManifest模式。 此模式假定您具有service worker文件（其位置在config.js中指定）。
运行workbox injectManifest时，它会在源service worker文件（swSrc）中查找特定字符串（默认情况下为precaching.precacheAndRoute（[]）。 它将**空数组替换为预先缓存的URL列表**，并根据config.js中的配置选项将service worker文件写入其目标位置（swDest）。 源service worker的其余代码保持不变。
您可以在此模式下使用Workbox，如下所示：
```
workbox injectManifest config.js
```
> 
适合injectManifest的场景：
（1）您希望更好地控制service worker。
（2）您想要预先缓存文件。
（3）您在路由方面有更复杂的需求。
（4）您希望将您的service worker与其他API（例如Web推送）一起使用。
> 
不适合injectManifest的场景：
（1）您希望有最简单的方法将service worker添加到您的站点。
## 3.4 copyLibraries
如果您想使用injectManifest并且想要使用托管在您自己的源上而不是使用CDN的Workbox库文件，则此模式很有用。
您只需要使用路径将其写入以下文件：
```
workbox copyLibraries third_party/workbox/
```
# 4 构建集成化的过程
## 4.1 为什么Workbox需要构建过程集成？
Workbox项目包含许多库，它们共同为Web应用程序的service worker提供支持。 为了有效地使用这些库，需要将Workbox集成到Web应用程序的构建过程中。 这可确保您的service worker能够有效地预先处理您的所有Web应用程序的关键内容，并使该内容保持最新。
## 4.2 workbox-cli是我的构建过程的正确选择吗？
- 
如果您的现有构建过程完全基于npm脚本，那么workbox-cli是一个不错的选择。
- 
如果您当前正在使用webpack作为构建工具，那么使用workbox-webback-plugin是更好的选择。
- 
如果您当前正在使用Gulp，Grunt或其他一些基于Node.js的构建工具，那么将workbox-build集成到构建脚本中是更好的选择。
- 
如果您根本没有构建过程，那么在使用Workbox预先缓存任何资产之前，您应该先提供一个构建过程。 试图记住手动运行workbox-cli可能容易出错，而忘记运行它可能会导致过时的内容会被提供给返回的访问者。
## 4.3使用CLI 集成化配置
安装workbox-cli作为本地项目的开发依赖项后，可以在现有构建过程的npm脚本末尾添加对workbox的调用：
来自package.json：
```
{
  "scripts": {
    "build": "my-build-script && workbox <mode> <path/to/config.js>"
  }
}
```
将<mode>替换为generateSW或injectManifest（取决于您的用例）和<path / to / config.js>替换配置选项的路径。 您的配置可能已由workbox向导自动创建或手动调整。
# 5、配置说明
## 5.1 generateSW使用的选项
|字段|说明|
|----|----|
|importWorkboxFrom|可选字符串，默认为'cdn'  有效值为“cdn”，“local”和“disabled”。 （1）默认情况下，“cdn”将使用托管在高可用性Google云端存储实例上的Workbox运行时库的URL。（2）'local'会将所有Workbox运行时库复制到生成的service worker旁边的版本化目录中，并将服务工作者配置为使用这些本地副本。 此选项适用于希望自己托管所有内容而不依赖于Google云端存储CDN的开发人员  （3）'disabled'将选择退出自动行为。 您可以在首选URL上托管Workbox库的本地副本，并通过importScripts配置选项将正确的路径传递给workbox-sw.js。|
|skipWaiting|可选布尔值，默认为falseservice worker是否应该跳过等待的生命周期阶段。 通常，它与`clientsClaim：true`一起使用。|
|clientsClaim|可选布尔值，默认为falseservice worker是否应该在激活后立即开始控制任何现有客户端。|
|runtimeCaching|可选的Object对象，默认为[]传入包含urlPatterns，handler和可能选项的对象数组将向生成的service worker添加适当的代码以处理运行时缓存。通过globPatterns获取的预先缓存的URL的请求是默认处理的，不需要在runtimeCaching中进行处理。handler的值是字符串，对应于workbox.strategies支持的策略名称。options属性可用于配置缓存过期，可缓存响应和广播缓存更新插件的实例，以应用于给定路由。|
|navigateFallback|可选字符串，默认为undefined 这将用于创建一个NavigationRoute，它将响应未预先缓存的URL的导航请求。这适用于单页应用程序场景，您希望所有导航都使用通用的App Shell HTML。它不适合用作浏览器脱机时显示的后备。|
|navigateFallbackBlacklist|可选的RegExp数组，默认为[]一个可选的正则表达式数组，用于限制配置的navigateFallback行为适用的URL。如果只将您网站的一部分网址视为单页面应用程序的一部分，则此功能非常有用。如果同时配置了navigateFallbackBlacklist和navigateFallbackWhitelist，则黑名单优先。|
|navigateFallbackWhitelist|可选的RegExp数组，默认为[]一个可选的正则表达式数组，用于限制配置的navigateFallback行为适用的URL。|
|importScripts|必需的字符串数组应该在生成的service worker文件中传递给importScripts（）的JavaScript文件的必需列表。|
|ignoreUrlParametersMatching|可选的RegEx数组，默认为[/ ^ utm_ /]在查找预先缓存匹配之前，将删除与此阵列中的一个正则表达式匹配的任何搜索参数名称。如果您的用户可能会请求包含用于跟踪流量来源的URL参数的网址，则此功能非常有用。 这些URL参数通常会导致缓存查找失败，因为用作缓存键的URL字符串不会包含它们。|
|directoryIndex|可选字符串，默认为index.html如果URL的结尾导航请求未能与预先缓存的URL匹配，则此值将附加到URL，并将检查是否存在预先缓存匹配。|
|cacheId|可选String，默认为null一个可选ID，用于缓存Workbox使用的名称。这对于本地开发非常有用，其中可以从相同的[http://localhost:port](http://localhost) origin提供多个站点。|
## 5.2 injectManifest 使用的参数
|字段|说明|
|----|----|
|swSrc|必填字符串除了包含injectPointRegexp的匹配项之外，源service worker文件的路径可以包含您自己的自定义代码在 node：service worker文件应该包含对workbox.precaching方法的调用，该方法使用注入的预缓存清单。在Webpack中，您的service worker文件应引用self .__ precacheManifest变量以获取作为编译的一部分获得的ManifestEntrys列表：workbox.precaching.precacheAndRoute（self .__ precacheManifest）|
|injectionPointRegexp|可选的RegExp，默认为/(.precacheAndRoute()\s*[\s*]\s*())/默认情况下，使用的RegExp将在swSrc文件中找到字符串precacheAndRoute（[]），并将empty []数组替换为包含应该预先缓存的ManifestEntrys的数组。如果您希望将ManifestEntrys注入到swSrc文件的不同部分，请将其配置为包含两个捕获组的不同RegExp。 清单数组将被注入捕获组之间|
## 5.3 共同使用的参数
其余选项的参数，两个命令都可以使用。
|字段|说明|
|----|----|
|swDest|必填字符串将由构建过程创建的service worker文件的路径和文件名。 在节点中，这将相对于当前工作目录。 在webpack中，这将相对于webpack输出目录。|
|globDirectory|可选字符串，默认为undefined  您希望匹配globPatterns的基本目录，相对于当前工作目录。如果设置了此项，请确保还配置globPatterns。|
|globFollow|可选布尔值，默认为true 确定生成预缓存清单时是否遵循符号链接。|
|globIgnores|可选的String数组，默认为['node_modules / ** / *'] 匹配文件的一组模式，在生成预缓存清单时始终排除。有关更多信息，请参阅glob文档=中的ignore定义。|
|globPatterns|可选的String数组，默认为['** / *。{js，css，html}']（用于workbox-build和workbox-cli）或[]（用于workbox-webpack-plugin）与任何这些模式匹配的文件将包含在预缓存清单中。注意：使用workbox-webpack-plugin时通常不需要设置globPatterns，默认情况下会自动预先处理属于webpack构建管道的文件。 使用webpack插件时，只需在需要缓存非webpack资产时进行设置。|
|globStrict|可选布尔值，默认为true如果为true，则在生成预缓存清单时读取目录时出错将导致生成失败。 如果为false，则将跳过有问题的目录。|
|templatedUrls|带String或Array数组的可选Object，默认为null如果基于某些服务器端逻辑生成URL，则其内容可能依赖于多个文件或某些其他唯一字符串值。如果与字符串数组一起使用，它们将被解释为glob模式，并且与模式匹配的任何文件的内容将用于唯一地对URL进行版本控制。如果与单个字符串一起使用，它将被解释为您为给定URL带外生成的唯一版本控制信息。|
|maximumFileSizeToCacheInBytes|可选号码，默认为2097152此值可用于确定将预先缓存的文件的最大大小。 这可以防止在预先处理中可能意外匹配其中一个非常大的文件。|
|dontCacheBustUrlsMatching|可选的RegExp，默认为null与此正则表达式匹配的资产将被假定为通过其URL进行唯一版本化，并且在填充预缓存时免除了正常的HTTP缓存破坏。虽然不是必需的，但建议如果您的现有构建过程已经在每个文件名中插入了[hash]值，那么您将提供一个将检测这些值的RegExp，因为它将减少预缓存时消耗的带宽量。|
|modifyUrlPrefix|带有String值的可选Object，默认为null前缀的映射（如果存在于预缓存清单中的条目中）将替换为相应的值。例如，如果您的Web主机设置与本地文件系统设置不匹配，则可以使用此选项从清单条目中删除或添加路径前缀。作为具有更大灵活性的替代方法，您可以使用manifestTransforms选项并提供一个函数，该函数使用您提供的任何逻辑修改清单中的条目。|
|manifestTransforms|可选的ManifestTransform数组，默认为null一个或多个ManifestTransform函数，将针对生成的清单顺序应用。如果还指定了modifyUrlPrefix或dontCacheBustUrlsMatching，则将首先应用其相应的转换。|
虽然在实际项目中，我们不太会直接使用命令行来构建sw文件，但是这种方式有助于帮助我们理解后续插件的工作原理，因为这些插件也都是在cli的基础上封装一些操作的。
# 6 实践
项目目录
![5471980-9fe22434abb07958.png](https://upload-images.jianshu.io/upload_images/5471980-9fe22434abb07958.png)
![5471980-e51240316ba45743.png](https://upload-images.jianshu.io/upload_images/5471980-e51240316ba45743.png)
我们先创建一个空白的sw.js
```
workbox.precaching.precacheAndRoute([
]);
```
workbox-injectManifest-config.js
```
module.exports = {
  "globDirectory": "src/",
  "globIgnores": ["**/css2/**",'**/dist/**'],
  "globPatterns": [
    "**/*.{css,html,png,jpg,js,eot,svg,ttf,woff,woff2,yml,json,md,rb,pug,swf,ico,txt,lock,less,nuspec,ps1,chm}"
  ],
  "swDest": "src\\dist\\sw-inj.js",
  "swSrc": "C:\\Users\\hugo\\Documents\\GitHub\\OneSugar\\前端项目\\workbox-cli\\sw.js"
};
```
workbox-generateSW-config.js
```
"globDirectory": "src/",
  "globIgnores": ["**/css2/**",'**/dist/**'],
  "globPatterns": [
    "**/*.{css,html,png,jpg,js,eot,svg,ttf,woff,woff2,yml,json,md,rb,pug,swf,ico,txt,lock,less,nuspec,ps1,chm}"
  ],
  "swDest": "src\\dist\\sw-gen.js",
```
package.json
```
"scripts": {
    "test": "echo \"Error: no test specified\" && exit 1",
    "sw-gen": "workbox generateSW workbox-generateSW-config.js",
    "sw-inj": "workbox injectManifest workbox-injectManifest-config"
  },
```
![5471980-12d748e5e6438e92.png](https://upload-images.jianshu.io/upload_images/5471980-12d748e5e6438e92.png)
![5471980-3e169b2bc95e7c17.png](https://upload-images.jianshu.io/upload_images/5471980-3e169b2bc95e7c17.png)
使用了一下injectManifest和generateSW，感觉差别不大，前者有一个swSrc，但是里面的sw.js只能写个空白的precacheAndRoute……先这样吧，回头看看。
# 9.5 更新：
injectManifest：inj
generateSW：gen
好吧，inj和gen两个方法还是有一定差别的，想明白了。
（1）gen不能指定swSrc，也就说生成的sw完全由框架控制，inj可以，所以我们可以先编写一个默认的sw模板，然后再inj.
（2）但是inj无法指定runtimeCaching，也就说运行时的缓存策略你需要手动地在swSrc指定的sw模板文件中预先写好，这当然对新手是不太友好的，所以这也就解释了为什么官方文档说，如果你想方便快捷地使用workbox，就使用gen。
（3)官方文档说如果你需要使用更多的插件或者自定义路由，可以使用inj。应该是由于自己手动编写sw文件，所以有更多的控制权。
笔者个人订阅号~欢迎小伙伴们关注
![5471980-9f5b4e5446178188.jpg](https://upload-images.jianshu.io/upload_images/5471980-9f5b4e5446178188.jpg)
微信公众号-感谢关注
> 
若有疑问可以QQ联系笔者，虽然不一定100%解决你的问题，但是可以交流探讨一波：**2276604211**
> 
顺便打个广告：如果有想入职**中国银联上海技术开发**的童鞋，也可以加上面的QQ资讯，笔者可以帮你回答一些相关问题~~
