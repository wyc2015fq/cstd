# iOS-发短信 - weixin_33985507的博客 - CSDN博客
2017年06月09日 14:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
在APP内如何发短信呢?
MFMessageComposeViewController这个类提供了发短信的功能。
###### 例子
```
guard MFMessageComposeViewController.canSendText() else {
            print("不能发送短信")
            return
        }
        let messageVC = MFMessageComposeViewController()
        messageVC.messageComposeDelegate = self // 代理
        messageVC.recipients = ["18888888888"] // 收件人
        messageVC.body = "短信内容" // 内容
        // 发送主题
        if MFMessageComposeViewController.canSendSubject() {
            messageVC.subject = "ICE"
        }
        // 发送附件
        if MFMessageComposeViewController.canSendAttachments() {
            // 路径添加
            if let path = Bundle.main.path(forResource: "Info", ofType: "plist") {
                messageVC.addAttachmentURL(URL(fileURLWithPath: path), withAlternateFilename: "Info.plist")
            }
            // NSData添加
            if MFMessageComposeViewController.isSupportedAttachmentUTI("public.png") {
                // See [Uniform Type Identifiers Reference](https://developer.apple.com/library/ios/documentation/Miscellaneous/Reference/UTIRef/Introduction/Introduction.html)
                if let image = UIImage(named: "qq") {
                    if let data = UIImagePNGRepresentation(image) {
                        // 添加文件
                        messageVC.addAttachmentData(data, typeIdentifier: "public.png", filename: "qq.png")
                    }
                }
            }
        }
        // messageVC.disableUserAttachments() // 禁用添加附件按钮
        self.present(messageVC, animated: true, completion: nil)
```
```
发送短信的结果
typedef NS_ENUM(NSInteger, MessageComposeResult) {
    MessageComposeResultCancelled,//取消
    MessageComposeResultSent,//已发送
    MessageComposeResultFailed//发送失败
} NS_ENUM_AVAILABLE_IOS(4_0);
/*!
 @constant  给定的附件URL
*/
extern NSString *const MFMessageComposeViewControllerAttachmentURL;
/*!
 @constant  附件名
 */
extern NSString *const MFMessageComposeViewControllerAttachmentAlternateFilename;
/*!
 @const      MFMessageComposeViewControllerTextMessageAvailabilityDidChangeNotification
 @abstract   Notification posted when the value of <tt>+[MFMessageComposeViewController canSendText]</tt> has changed.
 @discussion 在[MFMessageComposeViewController canSendText]的值改变后会改变这个通知的值
*/
extern NSString *const MFMessageComposeViewControllerTextMessageAvailabilityDidChangeNotification __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);
/*!
 @const      MFMessageComposeViewControllerTextMessageAvailabilityKey
 @abstract   UserInfo key for MFMessageComposeViewControllerTextMessageAvailabilityDidChangeNotification
             containing the value of <tt>+[MFMessageComposeViewController canSendText]</tt>
 @discussion 这个参数的值和+[MFMessageComposeViewController canSendText]一样
*/
extern NSString *const MFMessageComposeViewControllerTextMessageAvailabilityKey __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);
/*!
 @method     canSendText
 @abstract   Returns <tt>YES</tt> if the user has set up the device for sending text only messages.
 @discussion If the return value is YES, the client can set the recipients and body of the message.
             If the return value is NO, the client may notify the user of the failure, or the
             client may open an SMS URL via <tt>-[UIApplication openURL:]</tt>.
 */
+ (BOOL)canSendText  __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_4_0);
/*!
 @method     canSendSubject
 @abstract   Returns <tt>YES</tt> if the user has set up the device for including subjects in messages.</tt>.
 */
+ (BOOL)canSendSubject __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_7_0);
/*!
 @method     canSendAttachments
 @abstract   Returns <tt>YES</tt> if the user has set up the device for including attachments in messages.</tt>.
 */
+ (BOOL)canSendAttachments __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_7_0);
/*!
 @method     isSupportedAttachmentUTI:
 @abstract   Returns <tt>YES</tt>if the attachment at the specified URL could be accepted by the current composition.
 @discussion If the return value is YES, the UTI is acceptable for attachment to a message, a return value of NO
 indicates that the given UTI is unsupported.
 */
+ (BOOL)isSupportedAttachmentUTI:(NSString *)uti __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_7_0);
/*!
 @property   recipients
 @abstract   This property sets the initial value of the To field for the message to the specified addresses.
 @discussion 收件人
 */
@property(nonatomic,copy,nullable) NSArray<NSString *> *recipients /*__OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_4_0)*/;
/*!
 @property   body
 @abstract   This property sets the initial value of the body of the message to the specified content.
 @discussion 短信内容
 */
@property(nonatomic,copy,nullable) NSString *body /*__OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_4_0)*/;
/*!
 @property   subject
 @abstract   This property sets the initial value of the subject of the message to the specified content.
 @discussion 消息主题
 */
@property(nonatomic,copy,nullable) NSString *subject /*__OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_7_0)*/;
/*!
 @property   attachments
 @abstract   This property returns an NSArray of NSDictionaries describing the properties of the current attachments.
 @discussion 以数组包含字典的形式的附件
             See MFMessageComposeViewControllerAttachmentURL, MFMessageComposeViewControllerAttachmentAlternateFilename.
 */
@property(nonatomic,copy,readonly,nullable) NSArray<NSDictionary *> *attachments /*__OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_7_0)*/;
/*!
 @property   message
 @abstract   This property sets the initial interactive message.
 */
@property(nonatomic,copy,nullable) MSMessage *message __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_10_0);
/*!
 @method     addAttachmentURL:withAlternateFilename:
 @abstract   Returns <tt>YES</tt>if the attachment at the specified URL was added to the composition successfully.
 @discussion 如果返回值是肯定的,附件添加到数组。如果返回值是NIL,附件没有添加到组合。所有附件url必须文件的url。该文件网址不能为NIL。备用文件名会显示给用户的leiu附件URL。备用文件名可能是NIL。
 */
- (BOOL)addAttachmentURL:(NSURL *)attachmentURL withAlternateFilename:(nullable NSString *)alternateFilename __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_7_0);
/*!
 @method     addAttachmentData:typeIdentifier:filename:
 @abstract   Returns <tt>YES</tt>if the attachment was added to the composition successfully.
 @discussion 如果返回值是肯定的,附件添加到组合。如果返回值是NIL,附件没有添加到组合。数据和typeIdentifer必须非nil。typeIdentifier应该是一个有效的统一类型标识符。
 */
- (BOOL)addAttachmentData:(NSData *)attachmentData typeIdentifier:(NSString *)uti filename:(NSString *)filename __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_7_0);
@protocol MFMessageComposeViewControllerDelegate <NSObject>
@required
/*!
 @method     messageComposeViewController:didFinishWithResult:
 @abstract   Delegate callback which is called upon user's completion of message composition.
 @discussion 发送短信的结果回调
 @param      controller   The MFMessageComposeViewController instance which is returning the result.
 @param      result       MessageComposeResult indicating how the user chose to complete the composition process.
 */
- (void)messageComposeViewController:(MFMessageComposeViewController *)controller didFinishWithResult:(MessageComposeResult)result;
```
