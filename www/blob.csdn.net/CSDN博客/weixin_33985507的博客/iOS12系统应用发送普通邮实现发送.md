# iOS12系统应用发送普通邮实现发送 - weixin_33985507的博客 - CSDN博客
2018年11月16日 14:16:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
# **iOS12系统应用发送普通邮实现发送**
构建好邮件以后，可以发送该邮件。此时需要使用mailComposeDelegate属性，该属性用来设置委托，其语法形式如下：
unowned(unsafe) var mailComposeDelegate: MFMailComposeViewControllerDelegate? { get set }
该属性的值为MFMailComposeViewControllerDelegate协议类型。该协议中包含mailComposeController(_:didFinishWith:error:)方法。该方法会在用户关闭MFMailComposeViewController界面时调用，其语法形式如下：
optional func mailComposeController(_ controller: MFMailComposeViewController,
 didFinishWith result: MFMailComposeResult,
 error: Error?)
其中，参数说明如下：
**controller：**MFMailComposeViewController对象。
**result****：**用户操作的结果。该参数是MFMailComposeResult枚举类型，该枚举类型中包含了4个成员。其中，cancelled表示轻拍取消中的“删除草稿”按钮；saved表示轻拍取消中的“保存草稿”按钮，邮件会保存在用户的“草稿”文件夹中，而不会发送；sent表示轻拍“发送”按钮，邮件添加到用户的发件箱，准备发送；failed表示邮件未保存或排队，可能是出现错误。
**error：**Error对象。如果发生错误，此参数包含一个错误对象，其中包含失败类型的信息，如错误域和错误编码。开发者可以使用MFMailComposeErrorDomain全局变量获取错误域，其语法形式如下：
let MFMailComposeErrorDomain: String
而错误编码是MFMailComposeError.Code枚举类型。该枚举包含了2个成员。其中，saveFailed表示尝试将邮件保存到“草稿”文件夹时发生错误；sendFailed表示尝试排队或发送电子邮件时发生错误。
【示例3-1】下面发送一封普通邮件，内容为纯文本形式。代码如下：
@IBAction func sendMail(_ sender: Any) {
**let composeVC = MFMailComposeViewController() //****实例化**
**composeVC.mailComposeDelegate = self**
** composeVC.setToRecipients(["address@example.com"]) //****设置收件人**
** composeVC.setSubject("Hello!") //****设置主题**
** composeVC.setMessageBody("Hello from California!", isHTML: false) //****设置邮件正文**
 self.present(composeVC, animated: true, completion: nil)
}
**func mailComposeController(_ controller: MFMailComposeViewController, didFinishWith result: MFMailComposeResult, error: Error?) {**
 //轻拍取消中的“删除草稿”按钮
 if(**result==MFMailComposeResult.cancelled**){
 print("邮件取消")
 }
 //轻拍取消中的“保存草稿”按钮
 if(**result==MFMailComposeResult.saved**){
 print("邮件保存在草稿文件夹中")
 }
 //邮件失败
 if(**result==MFMailComposeResult.failed**){
 print("邮件失败")
 }
 //轻拍“发送”按钮
 if(**result==MFMailComposeResult.sent**){
 print("邮件已在用户的发件箱中排队，准备发送")
 }
 self.dismiss(animated: true, completion: nil)
**}**
运行程序，轻拍按钮，会打开MFMailComposeViewController提供的标准邮件界面，如图3.1所示。当轻拍“取消”按钮的“删除草稿”按钮后，会输出以下的内容：
邮件取消
当轻拍“取消”按钮的“保存草稿”按钮后，会输出以下的内容：
邮件保存在草稿文件夹中
当轻拍“发送”按钮后，会输出以下的内容：
邮件已在用户的发件箱中排队，准备发送
邮件发生错误，会输出以下的内容：
邮件失败
