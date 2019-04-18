# swift 验证码输入框在iOS9iOS10上的卡顿BUG - weixin_33985507的博客 - CSDN博客
2017年08月11日 11:56:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
![1835695-4204cc9683f773f2.png](https://upload-images.jianshu.io/upload_images/1835695-4204cc9683f773f2.png)
效果是这样的 
实现就是六个UITextField 
切换第一响应者来变更 光标
遇到的问题
问题：1在iOS9和10 的部分手机上会卡顿
一开始以为是因为切换第一响应者造成的 怎么都改不好 
后来发现原因是因为设置textFiled.isEnabled = false
解决办法：添加一个view在textFiled上 阻止响应点击事件 
很简单的解决 坑爹的是这个问题 老子找了一天 才找到是isEnabled的问题导致卡顿
问题：2 在8.1和8.2上如果输入框没有内容的话 会不响应删除按钮点击事件 我的做法是
添加textField.text = "\u{200B}"加上一个隐藏的空字符
最后的结果需要把这个字符替换成空
string.replacingOccurrences(of: "\u{200B}", with: "").trimmingCharacters(in: CharacterSet.whitespaces)
问题：3 输入框输入很快的时候会导致框内有多个字符
拦截输入的字符只接受一个int值
func textField(_ textField: UITextField, shouldChangeCharactersIn range: NSRange, replacementString string: String) -> Bool {        let stringTrim = string.trim()        let length = stringTrim.lengthOfBytes(using: String.Encoding.utf8)        for loopIndex in 0..57 {
return false
}
}
let proposeLength = (textField.text?.trim().lengthOfBytes(using: String.Encoding.utf8))! - range.length + stringTrim.lengthOfBytes(using: String.Encoding.utf8)
if proposeLength > 1{
return false
}
return true
}
