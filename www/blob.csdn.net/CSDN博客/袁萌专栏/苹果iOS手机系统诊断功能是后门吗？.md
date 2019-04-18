# 苹果iOS手机系统诊断功能是后门吗？ - 袁萌专栏 - CSDN博客
2014年08月06日 20:34:06[yuanmeng001](https://me.csdn.net/yuanmeng001)阅读数：1744
> 7月20日，美国知名苹果iOS手机系统侦破专家扎德尔斯基在2014年世界黑客大会（HOPE/X）用幻灯片讲演揭露了苹果手机存在系统级“后门”。为此，7月23日，苹果公司立即做出回应如下（部分内容）：
> 2.com.apple.mobile.file_relay（文件转发功能）
> “file_relay supports limited copying of diagnostic data from a device. This service is separate from user-generated backups, does not have
 access to all data on the device, and respects iOS Data Protection. Apple engineering uses file_relay on internal devices to qualify customer configurations. AppleCare, with user consent, can also use this tool to gather relevant diagnostic data from users'
 devices”。
> 这段话的中心意思是，文件转发功能（”file-relay”）支持“有限的”诊断数据的拷贝行为，并且“does
 not have access to all data on the device”（不访问设备上的其他所有数据）,尊重iOS的数据保护原则，...在用户同意（consent）的前提下，苹果护理程序（AppleCare）也可能收集相关诊断数据。由此可见，苹果公司并不认为文件转换功能是系统后门，不认为该功能会危害手机用户的信息安全，以及泄露用户的隐私信息。实际情况果真如此吗？非也！
> 扎德尔斯基利用十张讲演幻灯片的巨大篇幅，详细地阐明所谓“文件转发诊断功能”的后门本质特征，即系统十分隐蔽，人为故意设置，在用户完全不知情的情况下，执法人员与企业IT部门可以自由获取用户的所有私人信息（不限于诊断信息），甚至用户的个人“微信”也能被坏人随时监听，对执法部门而言，该功能完全是开放的，使用自如的。当今，许多人在质疑苹果公司为何如此替执法部门（比如，美国国家安全局）买力气，无限讨好？
> 据说，国内一些年轻人不在乎手机存在后门。实际上，假定你使用苹果iOS手机被坏人盯上了，你将成为一丝不挂被别人戏弄玩耍的动物，......对国人而言，苹果iOS手机是一个完全不设防的通信联络工具，非常要不得。
> 说明：我深信，扎德尔斯基关于苹果iOS手机存在系统后门的讲演幻灯片必将载入世界IT的发展史册。
> 
