# Xcode打包ipa的步骤（简述） - xqhrs232的专栏 - CSDN博客
2019年02月22日 17:10:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：28
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://blog.csdn.net/cyuyanenen/article/details/51925005](https://blog.csdn.net/cyuyanenen/article/details/51925005)
相关文章
1、Xcode打包ipa的基本步骤----[https://www.jianshu.com/p/a19d2d0747ee](https://www.jianshu.com/p/a19d2d0747ee)
2、Xcode打包生成ipa四个选项的具体区别----[https://www.jianshu.com/p/94e581861707](https://www.jianshu.com/p/94e581861707)
3、2017年xcode打包IPA上传苹果应用市场详细教程----[https://blog.csdn.net/xxw888/article/details/74066680](https://blog.csdn.net/xxw888/article/details/74066680)
4、Xcode7最新app打包发布详细过程（一）----[https://blog.csdn.net/song773743813/article/details/51274632](https://blog.csdn.net/song773743813/article/details/51274632)
5、Xcode7最新app打包发布详细过程（二）----[https://blog.csdn.net/song773743813/article/details/51280770](https://blog.csdn.net/song773743813/article/details/51280770)
一、概述 
（1）iOS开发中，应用程序打包 = ipa = 安装在手机上。(注意，并不是所有的ipa都可以随意安装) 
（2）如果想让用户可以安装ipa，必须在打包程序的时候说清楚 哪一个应用程序可以安装到哪一台设备上。 
（3）一般要想打包（测试或发布），必须成为苹果开发者。 
（4）下面的打包环境：Xcode 7.3.1、iOS 9.3.2。
二、打包（生成ipa测试包）步骤 
第1步：配置发布证书和AdHoc描述文件 
（1）登陆苹果开发者主页 https://developer.apple.com，选择Account，然后选择 Certificates, Identifiers & Profiles，点击Certificates： 
（2）点击右上角的加号： 
（3）然后 continue，再选择 App Store and Ad Hoc： 
（4）之后，一路 continue，直到需要我们上传 CSR 文件: 
（5）创建CSR，即证书请求文件。首先，在钥匙串访问中，如下图从证书颁发机构请求证书： 
（6）然后，将CSR文件存储到磁盘，其中的用户电子邮件可以随便写，也可以写你的真实邮箱，其余两项也无特殊要求，可不写。 
（7）最后，将CSR文件上传到苹果服务器。此时苹果服务器会自动生成，签名后的CER文件（ios_distribution.cer），然后 Download 到本地磁盘，并双击该CER文件，即可完成安装。 安装之后，可到 钥匙串访问->登录->证书 中查看该证书是否有效。
（8）AppID ＝ Bundld ID 用来在App Store上全球唯一标识每一个应用程序的。注册AppID， 告诉苹果哪一个APP需要调试。 
（9）点击Identifiers ->点击加号 
（10）其中的 App ID Description 可以随便写 
（11）在 App ID Suffix 中，有两个选项可以填写bundle ID，如下图： 
<1> Explicit App ID：填写一个精确的ID，如果需要做远程推送/游戏中心/内购等功能，必须填写准确的bundle ID。 
<2> Wildcard App ID：填写一个模糊的ID,，如果不需要做远程推送/游戏中心/内购等功能，直接填写模糊的bundle ID即可，这样可以提升我们的开发效率， 
所以在开发中，为了保证方便，使用AppID时，可以使用通配符＊，替代所有的应用程序。 
<3> 之后，一直 continue 即可。
（12）点击Devices，告诉苹果哪一台设备可以进行真机调试 
（13）点击右上角的加号： 
（14）在Xcode中，点击window->Devices，获取设备的UDID 
（15）填写设备的描述和设备的UDID注册设备 
<1> 使用设备的唯一标示符号制定，哪些真机能够调试开发。 
<2> 普通的开发者账号，一个账号只能注册100台设备，苹果并没有提供删除设备ID的功能，仅仅只能禁止某台设备调试（禁用后，名额仍然会被占用），被禁止的设备会在下一次付费时被清空（第二年），从而释放名额。 
<3> 每一个设备的唯一标示符号，能够唯一准确的定位到一台设备，只能在”添加设备时”使用。 
<4> 如果程序中使用了唯一标示符号，将无法上架。
（16）描述文件（Profiles），即告诉系统哪一台电脑的哪一个应用程序可以在哪一台设备上运行。 点击Provisioning Profiles ： 
（17）点击加号 
（18）然后按照提示一步一步往下走，选择对应的证书、AppID、Devices，告诉系统哪一台电脑/哪一个APP/哪一个设备可以调试。 
注意，选择对应的证书时，选择的是：Ad Hoc 
（19）最终得到 xxxxxx.mobileprovision文件，下载到本地磁盘，双击，保存到Xcode 中。
（20）注意点 
<1> 检查证书的状态，可到 钥匙串访问->登录->证书 中查看该证书是否有效。若无效，则可能现象为“ WWDCRA 无效 或 此证书的签发者无效”，解决方案可以参考如下文章： iOS 探讨之 WWDCRA 无效 or “此证书的签发者无效” 
<2> 生成AdHoc描述文件的时候要注意勾选所有的设备，只有被描述文件包含的设备才能进行相应的测试。如果是企业账号的话则不需要添加设备的UDID。 
<3> 打包ipa时的CER证书，名字为：ios_distribution.cer； 
真机调试时的CER证书，名字为：ios_development.cer；
知识点补充： 
在苹果开发者主页中配置好需要的证书之后，也可以不用“下载到本地，然后再安装“，还有另一种解决方案，如下： 
（1）Xcode->Preferences 
（2）选择 Accounts，然后点击加号，然后选择“Add Apple ID“ 
（3）输入刚刚在苹果开发者主页中使用的 Apple ID，然后点击 Sign In。 
（4）登录成功后，点击右下角的 View Details，如下： 
（5）点击后，会有两项显示： 
<1> Signing Identities，即签名证书 
<2> Provisioning Profiles，即描述文件 
在最下方，有一个 Download All 按钮，点击后，再点击 Done。即可将对应的证书和描述文件自动安装到对应的位置。 
第2步：配置Xcode（即在Xcode中，设置有效的Archive配置） 
此处方法众多，本人都亲自尝试过，先打包成ipa，再成功安装到手机上。 
方法1：（前提是已经用开发者帐号，注册过相应的证书） 
（1）Xcode->Preferences->Accounts，登录你注册过相应证书的 Apple ID 帐号。
（2）运行环境选择真机，此时项目工程下方中的 Products 中是全红的，如下： 
（3）Command+B，然后显示如下： 
（4）右键选中第一项，选择“Show in Finder“ 
（5）显示如下 
（6）打开你的iTunes，然后选择“应用“，并直接将该app文件拖到iTunes中即可。 
怎样验证此时该文件是ipa形式呢？如下图： 
结果显示为： 
说明我们的确得到了ipa文件。
备注：此处对（6）做个补充，即还有另一种方法得到ipa文件。 
将（5）中得到的app文件，拷贝到一个新建文件夹中，该文件夹命名为“Payload“，然后右键选择“压缩Payload“，如下图： 
我们将得到一个Payload.zip文件，将该文件重命名为Payload.ipa即可我们想要的ipa文件，然后将该ipa文件拖入到iTunes中即可（或直接双击该ipa文件）。
（7）然后，如下图，点击安装即可将该打包应用安装到手机上。 
方法2：（前提是已经用开发者帐号，注册过相应的证书） 
（1）Xcode->Preferences->Accounts，登录你注册过相应证书的 Apple ID 帐号。
（2）运行环境选择真机，此时项目工程下方中的 Products 中是全红的，如下： 
（3）Xcode->Product->Archive，此时会直接跳转到“Xcode->Window->Organizer“界面，如下： 
（4）点击 Export，并依次按下图顺序选择： 
（5）最终得到ipa文件，如下图： 
（6）直接双击该ipa文件，并在iTunes中将该应用安装到手机上即可。
方法3：（前提是已经用开发者帐号，注册过相应的证书） 
（1）方法1和2中，Xcode可能已经默默帮我们做了很多配置，现在我们针对“要打包ipa测试包的任务“明确的做一些特定的配置。 
（2）点击 Product->Scheme->Edit Scheme，或者通过如下快捷方式： 
进入到如下界面，并点击Archive，确保Build Configuration 为 Release，如下： 
（3）经过亲测，TARGETS->Identity->Team 可以不用选择注册相应证书时对应的 Apple ID 帐号，而且选择别的对应／非对应帐号也都没问题。（可能Xcode已经默默的帮我们选择了，所以，此步操作可做／可不做） 
（4）选择 TARGETS->Build Setting->Code Signing，如下： 
（5）找到 Code Signing Identity 中的 MiGuRead_2AdHoc，点击 Don't Code Sign，选择之前配置好的证书，因为是打包，此处应选择：iPhone Distribution: xxxxxx，其中Debug 和 Release里暂时不要改动。 
（6）其中下方的 Provisioning Profile 可以使用默认的 Automatic，也可以指定对应的描述文件。（一般我们使用Automatic） 
（7）执行方法2中的步骤（3）~（7）。
特别说明：此处步骤（5）中，网上有很多种设置方法； 
（1）有的将Debug、Release 和 Provisioning Profile 都设置为了对应的证书； 
（2）有的将上述打包ipa测试包的证书设置为了“iPhone Developer: xxxxxx“开发证书，但也打包成功了，但这其实是不匹配的，Xcode可能已经默默帮我们更正了。 
（3）有的添加了AdHoc，这仅仅是为了区分，方便管理 
<1> 先通过PROJECT->Info->Configurations，点击加号，并选择 Duplicate “Release” Configuration（这里一定要选择复制“Release”，因为打包／发布版本一定是release的） 
添加后，如下： 
<2> 重命名（即命名可以自定义），一般证书和描述文件取名时默认用项目名称+对应的功能缩写。（有时也直接命名为AdHoc）重命名后，如下： 
<3> 选择 TARGETS->Build Setting->Code Signing，如下： 
<4> 找到 Code Signing Identity 中的 MiGuRead_2AdHoc，点击 Don’t Code Sign，选择之前配置好的证书，因为是打包，此处应选择：iPhone Distribution: xxxxxx，其中Debug 和 Release里暂时不要改动。 
<5> 选好之后，显示如下： 
<6> 同样，其中的 Provisioning Profile 可以使用默认的 Automatic，也可以指定对应的描述文件。（一般我们使用Automatic）
<7> 执行方法2中的步骤（3）~（7）。
