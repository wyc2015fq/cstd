# 关于cocos3.x getTextureForKey的那些事 - weixin_33985507的博客 - CSDN博客
2017年07月09日 00:08:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
这个函数 我在看3.10的display框架的时候发现的
一直不知道有什么作用,本着曾国藩上页不读完不读下页,一本书不读完不读下本的精神
决定追本溯源
以下就是我发现的代码
```
local fileUtils = cc.FileUtils:getInstance()
function display.getImage(imageFilename)
    local fullpath = fileUtils:fullPathForFilename(imageFilename)
    return textureCache:getTextureForKey(fullpath)
end
```
前面的都很好理解 什么fileUtils 文件类 textureCache其实是个
`cc.Director:getInstance():getTextureCache()`
getTextureForKey是什么呢
起先我以为是获得一张图片纹理 然后设置到精灵里面去
后来发现并不是我想的那样
目前我发现了这个函数有3个作用
### 判断你的plist png 纹理是否加载到内存里面了
```
cc.SpriteFrameCache:getInstance():addSpriteFrames("filpCardAllSpr.plist", "filpCardAllSpr.png")
    local fileUtils = cc.FileUtils:getInstance()
    local fullpath = fileUtils:fullPathForFilename("filpCardAllSpr.png")
    local texture = cc.Director:getInstance():getTextureCache():getTextureForKey(fullpath)
    dump(texture, "tex = ")
```
输出
`"tex = " = userdata: 0x0a78e858`
如果没有addSpriteFrames
`"tex = " = nil`
之后在C++源码里面搜getTextureForKey
发现还有另外2个用法
```
dump(texture:getPixelFormat(), "pixel format: ")
    dump(texture:hasPremultipliedAlpha(), "premultiplied alpha: ")
```
输出
```
"pixel format: " = 2
"premultiplied alpha: " = true
```
那么 这2个函数又是干什么的呢
texture:getPixelFormat 给出的翻译是
### 取得当前纹理的贴图像素格式。
> 
参考网页:[http://www.cnblogs.com/sevenyuan/archive/2013/07/17/3195798.html](https://link.jianshu.com?t=http://www.cnblogs.com/sevenyuan/archive/2013/07/17/3195798.html)
texture:hasPremultipliedAlpha()给出的翻译是
### 是否有ALPHA渐变值
> 
参考网页:同上
参考网页AE中的通道预乘:[http://www.cgspread.com/7677.html](https://link.jianshu.com?t=http://www.cgspread.com/7677.html)
"pixel format: " = 2
那这个2又对应的是什么呢
发现是对应RGBA8888的
这样就可以判断纹理的格式了
```
cc.TEXTURE2_D_PIXEL_FORMAT_AUTO = 0x0
cc.TEXTURE2_D_PIXEL_FORMAT_BGR_A8888 = 0x1
cc.TEXTURE2_D_PIXEL_FORMAT_RGB_A8888 = 0x2
cc.TEXTURE2_D_PIXEL_FORMAT_RG_B888    = 0x3
cc.TEXTURE2_D_PIXEL_FORMAT_RG_B565   = 0x4
cc.TEXTURE2_D_PIXEL_FORMAT_A8        = 0x5
cc.TEXTURE2_D_PIXEL_FORMAT_I8        = 0x6
cc.TEXTURE2_D_PIXEL_FORMAT_A_I88     = 0x7
cc.TEXTURE2_D_PIXEL_FORMAT_RGB_A4444     = 0x8
cc.TEXTURE2_D_PIXEL_FORMAT_RGB5_A1       = 0x9
cc.TEXTURE2_D_PIXEL_FORMAT_PVRTC4        = 0xa
cc.TEXTURE2_D_PIXEL_FORMAT_PVRTC4A       = 0xb
cc.TEXTURE2_D_PIXEL_FORMAT_PVRTC2        = 0xc
cc.TEXTURE2_D_PIXEL_FORMAT_PVRTC2A       = 0xd
cc.TEXTURE2_D_PIXEL_FORMAT_ETC           = 0xe
cc.TEXTURE2_D_PIXEL_FORMAT_S3TC_DXT1     = 0xf
cc.TEXTURE2_D_PIXEL_FORMAT_S3TC_DXT3     = 0x10
cc.TEXTURE2_D_PIXEL_FORMAT_S3TC_DXT5     = 0x11
cc.TEXTURE2_D_PIXEL_FORMAT_DEFAULT       = 0x0
```
2017年7月10日21:46:50
新发现一个用法 那就是 不用plist 创建一张大图中的一部分作为纹理
```
local BugSprite = class("BugSprite", function(imageFilename)
    local texture = display.getImage(imageFilename)
    local frameWidth = texture:getPixelsWide() / 3
    local frameHeight = texture:getPixelsHigh()
    local spriteFrame = display.newSpriteFrame(texture, cc.rect(0, 0, frameWidth, frameHeight))
    local sprite = display.newSprite(spriteFrame)
    sprite.animationName_ = imageFilename
    sprite.frameWidth_ = frameWidth
    sprite.frameHeight_ = frameHeight
    return sprite
end)
```
