# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之96——BREW中运营商管理的测试UBT - 我相信...... - CSDN博客





2011年01月21日 11:11:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1000标签：[brew																[测试																[dependencies																[手机																[image																[中国电信](https://so.csdn.net/so/search/s.do?q=中国电信&t=blog)
个人分类：[消失的技术](https://blog.csdn.net/wireless_com/article/category/597607)





这一节以UBT为例，来说明商用测试的主要内容，中国电信运营BREW业务时，会有些不同点。

接受检查测试时，测试人员还将对在线提交的应用包准确性及开发商资质作相应检查。进入测试时，主要检查MIF设置，应用程序图标和应用在手机上的基本运行状况。

MIF设置包括MIF文件可以正常被打开，文件中的优先级设置是否正确。如果支持iLicense，须选择File优先级，ClassID不同时出现Applets和Dependencies栏内，且不同时出现Applets 栏和Extensions 栏，MIF 中出现的所有通知均存在于应用程序规范中，编码应选择unicode。如果具有屏幕保护功能,那么屏幕保护选项被选定。如果被使用的扩展类已经通过UBT测试，是否在提交报告中及MIF文件中准确填写了该扩展类的ClassID号。应用程序图标的尺寸包括Image、Icon、Thumbnail三种图标都应包括在MIF中，微缩图像(Thumbnail)为16（w） x 16（h）像素，图标(icon) 26（w） x 26（h）像素支持部分或全部透明。图像(Image)是65（w）x42（h）。如果手机支持的话，允许满屏显示或者按标准显示尺寸进行显示。应用程序必需包含版本号码，当应用程序有多个版本号码时，所有的版本号码必需与提交清单一致。

应用程序必需可以下载到手机上，图标显示正确，名称与提交清单的一致性。MIF文件中应用名字只允许采用中文名称显示，中文名称应和提交清单中应用程序名称一致，应用程序能使用应用程序管理器启动并退出。主屏幕应包括帮助，帮助项名称应包含“帮助”二字。主屏幕应包括关于选项，关于项名称应包含“关于”二字，应给出公司名称、应用程序名称、应用程序的版本号、公司客服电话。如果应用程序是一个联网程序，须采用中国联通的内网URL，确认应用能够访问互联网上的服务器。

全面测试主要是测试应用在实际运行环境中的正常运行，对相关接口与功能的正确使用和实现，以及应用与手机核心呼叫功能模块保持兼容且不影响手机的正常呼叫功能。UBT测试不包含应用程序内容方面的测试，只帮助开发商对应用的基本逻辑与使用用例进行最基本的检查，同时检查应用是否符合中国联通的UI规范，任何有关应用内容方面的质量保证由开发商自行承担。](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=dependencies&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=brew&t=blog)




