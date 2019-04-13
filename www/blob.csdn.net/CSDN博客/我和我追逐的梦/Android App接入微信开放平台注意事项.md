
# Android App接入微信开放平台注意事项 - 我和我追逐的梦~~~ - CSDN博客


2013年09月13日 11:30:36[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：3157个人分类：[微信开放平台																](https://blog.csdn.net/heyongluoyao8/article/category/1634657)


一、Android第三方应用接入微信开放平台的注意事项：
1. 到微信开放平台官网申请正式的AppID（需通过审核），要填写包名、app签名的md5值。至于如何获取app签名信息，官方提供签名包apk
（http://open.weixin.qq.com/download/sdk/gen_signature.apk）
2. 如果只是测试或debug，可以用临时AppID（到官网申请但不需要通过审核）。
二、如何运行SDK Demo？（“分享到好友”之后没反应）自己的App呢？
1. Eclipse打开Window->Preferences，在弹出的对话框中，选择Android目录下的Build，指定Custom debugkeystore选项的路径为sdk demo工程目录中的debug.keystore文件。
2. 在自己的App里，记得把debugkeystore改回来（改成当前用户目录）。并且，debug和正式App的AppID记得切换。
三、如何App中实现微信分享？
1. 申请AppID。
2. 下载微信sdk库：libammsdk.jar。
3. 将jar放入app\libs\。
4. 代码中：createWXAPI和registerApp来注册到微信，isWXAppInstalled和isWXAppSupportAPI来判断微信客户端是否安装及安装的版本是否支持微信开放平台，isTimeLineSupportedVersion用于判断微信是否支持朋友圈。
四、如何让我们的App接收到微信的反馈？
1. 在你的包名相应目录下新建一个wxapi目录，并在该wxapi目录下新增一个WXEntryActivity类，该类继承自Activity，并在manifest文件里面加上exported属性，设置为true。
2. 实现IWXAPIEventHandler接口，微信发送的请求将回调到onReq方法，发送到微信请求的响应结果将回调到onResp方法。
3. 在WXEntryActivity中将接收到的intent及实现了IWXAPIEventHandler接口的对象传递给IWXAPI接口的handleIntent方法。当微信发送请求到你的应用，将通过IWXAPIEventHandler接口的onReq方法进行回调，类似的，应用请求微信的响应结果将通过onResp回调。
五、其他注意事项：
1. 是不是必须在WXEntryActivity里发送sendReq及WXEntryActivity必须被调用？
WXEntryActivity可以不被调用，在sendReq后微信有响应就会到WXEntryActivity的onReq，所以一般onReq里Toast发送结果后需要finish()，这样直接返回到发送sendReq的UI（用户感觉不到有WXEntryActivity的存在）。
2. 如何发送到好友与朋友圈？
a. 发送到好友：SendMessageToWX.Req的scene成员，填WXSceneSession*。*
b. 发送到朋友圈：SendMessageToWX.Req的scene成员，填WXSceneTimeline（微信4.2以上支持，如果需要检查微信版本支持API的情况， 可调用IWXAPI的getWXAppSupportAPI方法,0x21020001及以上支持发送朋友圈），那么消息会发送至朋友圈。
c. scene默认值为WXSceneSession。
3. 如何发送图片同时显示title及description？
SDK分享信息发送WXMediaMessage中包含WXImageObject，发送后没有显示title和description，这是由于微信客户端对话框中对图片只会呈现其缩略图，标题和描述文字会自动忽略。
替代办法：
a. 对于分享到好友：以WXFileObject代替WXImageObject，在发送之前截图保存到sdcard，然后发送时设置setFilePath，这样在微信接收方点击缩略图后会提示是否接收文件并显示。
b. 对于分享到朋友圈：由于朋友圈不支持文件传输，只能发送WXImageObject，title及description无法显示。


