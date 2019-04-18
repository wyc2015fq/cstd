# swift  使用的宏 - weixin_33985507的博客 - CSDN博客
2018年03月23日 18:22:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
//屏幕的高
let K_ScreenHeight = UIScreen.main.bounds.height
//屏幕的款
let K_ScreenWidth = UIScreen.main.bounds.width
//是否是iphoneX
letK_isPhoneX =K_ScreenHeight==812?true:false
//tabbar 的高度
letK_BarHeight =K_isPhoneX?83.0:49.0
//导航栏的高度
letK_NavHeight =K_isPhoneX?88.0:64.0
//
letK_IntervalWidt =16
/// 服务器地址
let K_Base_URL = "http://api"
//适配屏幕
funcBTCERealValue(value:Int) -> (CGFloat){
    returnCGFloat(value) /414.0*K_ScreenWidth
}
//生成颜色
func RGBA(r:CGFloat, g:CGFloat, b:CGFloat, a:CGFloat) -> (UIColor) {
    returnUIColor(red: r/255.0, green: g/255.0, blue: b/255.0, alpha: a)
}
//十六进制颜色值
funcRGBColorFromHex(rgbValue:Int) -> (UIColor) {
        returnUIColor(red: ((CGFloat)((rgbValue &0xFF0000) >>16)) /255.0,
                            green: ((CGFloat)((rgbValue &0xFF00) >>8)) /255.0,
                            blue: ((CGFloat)(rgbValue &0xFF)) /255.0,
                            alpha:1.0)
}
//随机颜色
funcRandomColor () ->(UIColor){
    let red = CGFloat(arc4random()%255)
    letgreen =CGFloat(arc4random()%255)
    let blue = CGFloat(arc4random()%255)
    returnRGBA(r: red, g: green, b: blue, a:1)
}
//持续更新中
