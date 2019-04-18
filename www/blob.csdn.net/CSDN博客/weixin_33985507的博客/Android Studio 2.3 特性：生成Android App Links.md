# Android Studio 2.3 特性：生成Android App Links - weixin_33985507的博客 - CSDN博客
2017年03月05日 19:16:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
> 
移动应用开发时常遇到应用跳转、h5唤起原生页面等使用场景，所以一个健壮的应用需要具备路由的能力。常见的操作方法需要在manifest中针对每个页面进行配置，设想应用有大量的页面那配置工作岂不是费时费力，所以我们便来一起看看Android Studio 2.3给我们带来了什么便利
## 概览及使用
**App Links**是Android M以上的特有功能,即实现了**auto-verify**支持用户点击一个链接即可唤起APP，而不用像以往版本中需要点击一个选择框列表。为了使自己的应用支持**App Links**，我们常需要以下三个步骤：
- 在manifest中创建Intent filter
- 在Activity中编写对应的代码逻辑处理收到的url links
- 通过配置证书文件，关联网页和应用
Android Studio 2.3及以上版本中提供了App Links Assistant协助开发者进行快速配置，该功能可在**tools > App Links Assistant**中开启。
![1894569-ebad0da90e99bcb2.png](https://upload-images.jianshu.io/upload_images/1894569-ebad0da90e99bcb2.png)
App Links Assistant
### 配置Intent filsters
首先打开URL匹配规则编辑器，配置基本的Scheme、Host等信息，并指定关联的Activity对这块不是很了解的同学可以点击[App Manifest](https://link.jianshu.com?t=https://developer.android.com/guide/topics/manifest/data-element.html)了解各个字段的含义以及优先级。
![1894569-97b47eba843ce391.png](https://upload-images.jianshu.io/upload_images/1894569-97b47eba843ce391.png)
URL Mapping
配置完毕后可查看已配置的映射的关系，并可以通过输入URL进行校验是否调通
![1894569-4b63c66b69c3137e.png](https://upload-images.jianshu.io/upload_images/1894569-4b63c66b69c3137e.png)
URL-to-Activity
### 选择Activity
配置好mapping后，点击**Select Activity**选择上述配置好的Activity可自动生成以下代码。变量`appLinkData`以Uri的形式存储了进行传递的数据，针对具体页面开发者可编写特定的处理逻辑。
```java
// ATTENTION: This was auto-generated to handle app links.
        Intent appLinkIntent = getIntent();
        String appLinkAction = appLinkIntent.getAction();
        Uri appLinkData = appLinkIntent.getData();
```
### 关联应用
前两步已经使我们的应用具备了URL支持，在最后一步中我们将生成站点的配置文件，使其能够唤起我们的应用。此处需要根据个人用途选择**Signing config**或**keystore**并选择debug/release类型，点击生成的配置文件为`assetlinks.json`，并将其上传至自己的站点。
![1894569-8f4ab6ce96b720c5.png](https://upload-images.jianshu.io/upload_images/1894569-8f4ab6ce96b720c5.png)
Associate Website
> 
根据官方文档，特别注意的是系统识别Links需要通过https协议，故我们需要确保assetlinks.json可以通过https链接，无论我们的应用是否在Intent-filter中包含了https
通过配置`assetlinks.json`我们可以使站点具备唤起单一APP或者多个APP的能力，文件中使用了以下字段来关联app：
- **package_name** 即manifest中定义的包名
- **sha256_cert_fingerprints** 即应用的sha256指纹证书
而指纹证书可以调用命令行进行生成
`$ keytool -list -v -keystore my-release-key.keystore`
若需要关联多个app，可以参照下面的格式进行配置
```
[{
  "relation": ["delegate_permission/common.handle_all_urls"],
  "target": {
    "namespace": "android_app",
    "package_name": "example.com.puppies.app",
    "sha256_cert_fingerprints":
    ["14:6D:E9:83:C5:73:06:50:D8:EE:B9:95:2F:34:FC:64:16:A0:83:42:E6:1D:BE:A8:8A:04:96:B2:3F:CF:44:E5"]
  }
  },
  {
  "relation": ["delegate_permission/common.handle_all_urls"],
  "target": {
    "namespace": "android_app",
    "package_name": "example.com.monkeys.app",
    "sha256_cert_fingerprints":
    ["14:6D:E9:83:C5:73:06:50:D8:EE:B9:95:2F:34:FC:64:16:A0:83:42:E6:1D:BE:A8:8A:04:96:B2:3F:CF:44:E5"]
  }
}]
```
最后点击**Test App Links**可以在设备或模拟器上进行测试。
## 后记
### 缺陷
使用App Links能为开发者带来福利，但目前仍然有几个不可忽视的缺陷：
- 当app配置好了对App Links的支持之后，只有运行在Android M上的用户才能直接体会到这项好处
- 开发者需要维护与App相关联的站点，并支持上传认证文件，可能造成更大的财力精力开销
### 扩展
利用**App Links Assistant**我们可以快速配置应用全局路由的支持，由于manifest中已存在url与activity的映射关系，我们可以基于Intent和PackageManager设计一个简单的路由框架。比起手动在manifest增删，**App Links Assistant**能帮助我们高效管理当前的映射关系并进行测试。
### 参考文献
1.[Handling App Links](https://link.jianshu.com?t=https://developer.android.com/training/app-links/index.html#web-assoc)
2.[Add Android App Links](https://link.jianshu.com?t=%5Bhttps://developer.android.com/studio/write/app-link-indexing.html%5D(https://developer.android.com/studio/write/app-link-indexing.html))
