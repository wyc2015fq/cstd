# 【iOS 开发】iOS 10.3 如何更换 app 图标 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年04月06日 12:15:38[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1056
本文来自简书，原文地址:[http://www.jianshu.com/p/f6f328123923](http://www.jianshu.com/p/f6f328123923)
iOS 10.3 开放了更换 app 图标的 API，核心方法是下面这个：
```
func setAlternateIconName(_ alternateIconName: String?, 
    completionHandler: ((Error?) -> Void)? = nil)
```
这是[官方文档](https://developer.apple.com/reference/uikit/uiapplication/2806818-setalternateiconname#parameters)，但是你还需要在 info.plist 里面填一些东西才能让它起作用，[这部分官方注释内容在这里](https://developer.apple.com/library/content/documentation/General/Reference/InfoPlistKeyReference/Articles/CoreFoundationKeys.html#//apple_ref/doc/uid/TP40009249-SW14)。
![](http://upload-images.jianshu.io/upload_images/251884-9a879593802a5812.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
但 info.plist 如何填写这部分读起来还是有些晦涩，一时可能搞不清楚如何操作，下面做个示范。
![](http://upload-images.jianshu.io/upload_images/251884-66a471edd2f64b3f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Assets.xcassets
![](http://upload-images.jianshu.io/upload_images/251884-462900fc2a4ea46f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
info.plist
```xml
<key>CFBundleIcons</key>
<dict>
    <key>CFBundleAlternateIcons</key>
    <dict>
        <key>blackBgColor</key>
        <dict>
            <key>CFBundleIconFiles</key>
            <array>
                <string>blackBgColor</string>
            </array>
            <key>UIPrerenderedIcon</key>
            <false/>
        </dict>
    </dict>
    <key>CFBundlePrimaryIcon</key>
    <dict>
        <key>CFBundleIconFiles</key>
        <array>
            <string>AppIcon60x60</string>
        </array>
    </dict>
</dict>
```
如图，`Primary Icon` 字段写为 `AppIcon60x60` 是因为这里
 xcassets 里面我只导入了 60pt@2x 和 60pt@3x 的图片资源，这里选为 60 是因为对于 iPhone，60pt 的图片资源图标所需最高质量，更低分辨率的版本系统会自动压缩以展示。
`blackBgColor` 是我的用于替换原生图标的图片资源。文件名需要和 info.plist 中保持一致（注意
 info.plist 中用到了两次 "blackBgColor"），同时这也是你在代码中设置图标时，需要给 API 传入的参数。同样是 60pt@2x 和 60pt@3x 的图片资源，文件不通过 Assets.xcassets 添加进来，而是直接放到目录中。
如果你需要支持 iPad，建议这里使用 83.5pt（iPad Pro）的图片资源。另外还有些其他关于在 iPad 上替换图标的注意事项，[在这里有说明](https://developer.apple.com/library/content/documentation/General/Reference/InfoPlistKeyReference/Articles/CoreFoundationKeys.html#//apple_ref/doc/uid/TP40009249-SW14)，注意我们这里在
 info.plist 里面所用的 key 是` CFBundleIcons`，还有另外一个 key 是 `CFBundleIcons~ipad`。
替换图标部分的代码就超级简单了：
```
import UIKit
class ViewController: UIViewController {
    override func viewDidLoad() {
        super.viewDidLoad()
    }
    @IBAction func changeAppIcon(_ sender: Any) {
        if UIApplication.shared.supportsAlternateIcons {
            print("you can change this app's icon")
        }else {
            print("you cannot change this app's icon")
            return
        }
        if let name = UIApplication.shared.alternateIconName {
            // CHANGE TO PRIMARY ICON
            UIApplication.shared.setAlternateIconName(nil) { (err:Error?) in
                print("set icon error：\(String(describing: err))")
            }
            print("the alternate icon's name is \(name)")
        }else {
            // CHANGE TO ALTERNATE ICON
            UIApplication.shared.setAlternateIconName("blackBgColor") { (err:Error?) in
                print("set icon error：\(String(describing: err))")
            }
        }
    }
}
```
[这是上述所有内容的完整 demo 地址](https://github.com/OpenMarshall/AlternateIconsSample)
![](http://upload-images.jianshu.io/upload_images/251884-6c856a7dddb42e5a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
screentshot0
![](http://upload-images.jianshu.io/upload_images/251884-54759db3e54267e8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
screentshot1
iOS 10.3 开放了更换 app 图标的 API，核心方法是下面这个：
```
func setAlternateIconName(_ alternateIconName: String?, 
    completionHandler: ((Error?) -> Void)? = nil)
```
这是[官方文档](https://developer.apple.com/reference/uikit/uiapplication/2806818-setalternateiconname#parameters)，但是你还需要在 info.plist 里面填一些东西才能让它起作用，[这部分官方注释内容在这里](https://developer.apple.com/library/content/documentation/General/Reference/InfoPlistKeyReference/Articles/CoreFoundationKeys.html#//apple_ref/doc/uid/TP40009249-SW14)。
![](http://upload-images.jianshu.io/upload_images/251884-9a879593802a5812.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
但 info.plist 如何填写这部分读起来还是有些晦涩，一时可能搞不清楚如何操作，下面做个示范。
![](http://upload-images.jianshu.io/upload_images/251884-66a471edd2f64b3f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Assets.xcassets
![](http://upload-images.jianshu.io/upload_images/251884-462900fc2a4ea46f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
info.plist
```xml
<key>CFBundleIcons</key>
<dict>
    <key>CFBundleAlternateIcons</key>
    <dict>
        <key>blackBgColor</key>
        <dict>
            <key>CFBundleIconFiles</key>
            <array>
                <string>blackBgColor</string>
            </array>
            <key>UIPrerenderedIcon</key>
            <false/>
        </dict>
    </dict>
    <key>CFBundlePrimaryIcon</key>
    <dict>
        <key>CFBundleIconFiles</key>
        <array>
            <string>AppIcon60x60</string>
        </array>
    </dict>
</dict>
```
如图，`Primary Icon` 字段写为 `AppIcon60x60` 是因为这里
 xcassets 里面我只导入了 60pt@2x 和 60pt@3x 的图片资源，这里选为 60 是因为对于 iPhone，60pt 的图片资源图标所需最高质量，更低分辨率的版本系统会自动压缩以展示。
`blackBgColor` 是我的用于替换原生图标的图片资源。文件名需要和 info.plist 中保持一致（注意
 info.plist 中用到了两次 "blackBgColor"），同时这也是你在代码中设置图标时，需要给 API 传入的参数。同样是 60pt@2x 和 60pt@3x 的图片资源，文件不通过 Assets.xcassets 添加进来，而是直接放到目录中。
如果你需要支持 iPad，建议这里使用 83.5pt（iPad Pro）的图片资源。另外还有些其他关于在 iPad 上替换图标的注意事项，[在这里有说明](https://developer.apple.com/library/content/documentation/General/Reference/InfoPlistKeyReference/Articles/CoreFoundationKeys.html#//apple_ref/doc/uid/TP40009249-SW14)，注意我们这里在
 info.plist 里面所用的 key 是` CFBundleIcons`，还有另外一个 key 是 `CFBundleIcons~ipad`。
替换图标部分的代码就超级简单了：
```
import UIKit
class ViewController: UIViewController {
    override func viewDidLoad() {
        super.viewDidLoad()
    }
    @IBAction func changeAppIcon(_ sender: Any) {
        if UIApplication.shared.supportsAlternateIcons {
            print("you can change this app's icon")
        }else {
            print("you cannot change this app's icon")
            return
        }
        if let name = UIApplication.shared.alternateIconName {
            // CHANGE TO PRIMARY ICON
            UIApplication.shared.setAlternateIconName(nil) { (err:Error?) in
                print("set icon error：\(String(describing: err))")
            }
            print("the alternate icon's name is \(name)")
        }else {
            // CHANGE TO ALTERNATE ICON
            UIApplication.shared.setAlternateIconName("blackBgColor") { (err:Error?) in
                print("set icon error：\(String(describing: err))")
            }
        }
    }
}
```
[这是上述所有内容的完整 demo 地址](https://github.com/OpenMarshall/AlternateIconsSample)
![](http://upload-images.jianshu.io/upload_images/251884-6c856a7dddb42e5a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
screentshot0
![](http://upload-images.jianshu.io/upload_images/251884-54759db3e54267e8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
screentshot1
