# EasyDL定制化图像识别

# 简介

Hi，您好，欢迎使用百度EasyDL定制化训练和服务平台。

目前在图像方向该平台支持定制图像分类和物体检测两类图像识别模型。

**图像分类** 识别一张图中是否是某类物体/状态/场景。可以识别图片中主体单一的场景。

[应用场景] 1、图片内容检索，可定制识别各类物体 2、UGC内容审核 3、工业分拣或质检

**物体检测**： 在一张图包含多个物体的情况下，定制识别出每个物体的位置、数量、名称。可以识别图片中有多个主体的场景。

[应用场景] 1、物体计数，如零售行业货品摆放统计、医疗细胞计数 2、一图多物识别，识别一张图中多个物体

更多详情访问：[EasyDL定制化训练和服务平台](http://ai.baidu.com/easydl)。

# 使用流程概述

训练模型的基本流程如下图所示，全程可视化简易操作，在数据已经准备好的情况下，最快几分钟即可获得定制模型。

![img](https://ai.bdstatic.com/file/A3E4BE3FFD8C472D9487A1E0315FA652)

在[EasyDL首页](http://ai.baidu.com/easydl)进入模型中心，登录百度账号后可以看到左侧的目录，整体训练流程将按照目录栏的顺序依次操作即可。下面将详细介绍每一步的操作方式和注意事项（若遇到的问题在此文档没有找到答案，可以加入官方QQ群（群号:679517246）咨询群管）。

![img](https://ai.bdstatic.com/file/220861DE629F4AA8B67ADBEB51F52B0E)

# Step1 创建模型

![img](https://ai.bdstatic.com/file/224307B1100149589C3A02C39136E38B)

在目录【创建模型】中，只需选择模型类型、提交模型名称、模型描述、联系方式即可创建模型。

目前支持创建两类模型：

1、图像分类模型

2、物体检测模型

如何选择，取决于实际场景要解决怎样的问题，此处用一张图说明二者的区别。如果不确定实际场景到底应该训练什么样的模型，建议加入官方QQ群（群号:679517246）联系群管解决。

实际场景中也遇到一些要解决的问题比较复杂的用户，可以尽情发挥脑洞将两者结合一起使用。

目前单个用户在每种类型的模型下最多可创建**10个**模型，每个模型均支持多次训练，若需要创建超过10个以上的模型，可以加入官方QQ群（群号:679517246）联系群管解决。

![img](https://ai.bdstatic.com/file/7E99F8487D724813B44C3CC853E8303C)

操作示例：

![img](https://ai.bdstatic.com/file/E685CD2858C94E938C9C1CFBFE8E6A51)

模型创建成功后，可以在【我的模型】中看到刚刚创建的模型。

![img](https://ai.bdstatic.com/file/1696A1C76C204852B6A78CDA06176804)

# Step2 上传并标注数据

![img](https://ai.bdstatic.com/file/08D36327BD9F40A7BEE80668A1162ED0)

在训练之前需要在【数据集管理】中创建数据集，只需选择数据集类型（图像分类 or 物体检测）、数据集名称即可创建一个空数据集。每个数据集里面可以包含有多个标签的图片数据。

**注意：数据集的类型需要和模型类型一致**

## **【图像分类——数据集如何提交、格式要求】**

① 设计分类

首先想好分类如何设计，每个分类为你希望识别出的一种结果，如要识别水果，则可以以“苹果”、“梨”等分别作为一个分类；如果是审核的场景判断合规性，可以以“合格”“不合格”设计为两类，或者“合格”“不合格原因一”、“不合格原因二”、“不合格原因三”……设计为多类。

**注意：目前单个模型的上限为1000类，如果要超过这个量级可以加入QQ群（群号:679517246）联系群管解决**

② 准备数据：

基于设计好的分类准备图片数据，每个分类需要准备**20张以上，如果想要较好的效果，建议100张起**图片，如果某些分类的图片具有相似性，需要增加更多图片。

图片的基本格式要求： 目前支持图片类型为png、jpg、bmp、jpeg,图片大小限制在4M以内。长宽比在3：1以内，其中最长边需要小于4096px，最短边需要大于30px，一个模型的图片总量限制**10万张**图片。

**注意1：训练集图片需要和实际场景要识别的图片环境一致，举例：如果实际场景要识别的图片都是手机拍摄的，那训练的图片也需要同样的场景获得，而不要采用网上随便下载的图片**

**注意2：考虑实际应用场景可能有的种种可能性，每个分类的图片需要覆盖实际场景里面可能有的可能性，如拍照角度、光线明暗的变化，训练集覆盖的场景越多，模型的泛化能力越强。**

**注意3：如果需要寻求第三方数据采集团队协助数据采集，可以加入官方QQ群（群号:679517246）联系群管咨询了解**

你可能会有的问题：如果训练图片场景无法全部覆盖实际场景要识别的图片，怎么办？

答：本身模型算法会有一定的泛化能力，如果要识别的主体在图片中占比较大，应该效果上问题不大；**如果识别的主体在图片中占比较小，且实际环境很复杂无法覆盖全部的场景时，推荐用物体检测的模型来解决问题。**（物体检测可以支持将要识别的主体从训练图片中框出的方式来标注，所以能适应更泛化的场景和环境）。

③上传数据：

图像分类的数据上传方式非常简单，只需要将所有准备好的图片对应的分类以单个文件夹整理，同时将所有文件夹压缩为**.zip格式**，直接上传即可。

**注意1：单个压缩包限制大小为5G，图片张数上限10万张，若图片较多，建议分多个压缩包上传，若超出图片上限，请加入官方QQ群（群号:679517246）联系群管咨询解决**

**注意2：如果多次上传的压缩包里面分类命名存在一致，系统会自动合并数据**

**注意3：分类的命名需要以数字、字母、下划线格式，目前不支持中文格式命名，同时注意不要存在空格**

压缩包的结构示意图如下：

![img](https://ai.bdstatic.com/file/04A3D4EDB0BE4A85AB322AF6829404A3)

数据处理完后的图片示意：

![img](https://ai.bdstatic.com/file/FCC4CE1C6F4C4ABAA1C7D989C8DDA1F8)

## **【物体检测——数据集如何提交、格式要求】**

① 设计标签：

在上传之前需要想要都有哪些标签，每个标签就是对这张图片希望识别出的全部结果。

**注意：标签的上限为1000种**

② 准备图片：

基于设计好的标签准备图片数据，每种要识别出的标签在所有图片数量加起来需要有**20-100个**，如果某些标签的图片具有相似性，需要增加更多图片。

图片的基本格式要求：

目前支持图片类型为png、jpg、bmp、jpeg,图片大小限制在4M以内。长宽比在3：1以内，其中最长边需要小于4096px，最短边需要大于30px，一个模型的图片总量限制**10万张**图片。**物体检测1个模型需要至少4张图片**

**注意1：训练集图片需要和实际场景要识别的图片环境一致，举例：如果实际场景要识别的图片都是手机拍摄的，那训练的图片也需要同样的场景获得，而不要采用网上随便下载的图片**

**注意2：考虑实际应用场景可能有的种种可能性，每个标签的图片需要覆盖实际场景里面可能有的可能性，如拍照角度、光线明暗的变化，训练集覆盖的场景越多，模型的泛化能力越强。**

**注意3：如果需要寻求第三方数据采集团队协助数据采集，可以加入官方QQ群（群号:679517246）联系群管咨询了解**

③上传和标注图片 物体检测模型上传和标注图片的位置在目录中的【数据标注】中，进入后先选择模型，然后上传已准备好的图片。上传完后则在标注区域内开始标注。 首先在标注框上侧找到标注工具，点击标注按钮在图片中拖动画框，圈出要识别的目标。 如以“检测图片中的所有细胞”举例

![img](https://ai.bdstatic.com/file/8EC62923A3844B3EAEBC2CC3BAAC2EF6)

然后在右侧的标签栏中，增加新标签，或选择已有标签。**注意：标签名需要以字母或数字的格式，不支持中文命名** **需要特别需要注意的是，每张训练图片的想要识别的标签需要全部标出，不要有漏的**

![img](https://ai.bdstatic.com/file/D4B53DE67D094436AC66FCF9121E642C)

根据以上说明，就可以完成所有数据的上传，并在数据集管理中后续持续迭代数据。

# Step3 训练模型

**训练模型**

数据提交后，可以在目录中找到【训练模型】操作训练。

1、选择一个模型

2、勾选应用类型：支持云服务和离现实别SDK

3、云服务下，可以进一步选择算法，包括通用算法和AutoDL Transfer：

-- AutoDL Transfer是百度研发的AutoDL技术之一，结合模型网络结构搜索、迁移学习技术、并针对用户数据进行自动优化的模型，与通用算法相比，训练时间较长，但更适用于细分类场景。例如，通用算法可用于区分猫和狗，但如果要区分不同品种的猫，则AutoDL效果会更好

-- 通用算法下，可以选择训练方式，并进一步调节参数（需先申请权限）

4、勾选要训练的数据标签

训练时间与数据量大小有关，图像分类模型训练时间较短，1000张图片以内可以在30min内训练完成；物体检测训练时间较久，可能需要几个小时，请耐心等待。

**注意，图像分类模型操作训练的时候，提供了一个功能“增加识别结果为“其他”类的默认分类，勾选后，对于与训练集无关的图片会自动识别为“其他”结果，非常适合只有1个正例的分类场景、或者实际业务场景索要识别的图片内容不可控”**

**图像分类模型至少需要选择2个及以上分类**

![image-20181129004418202](/Users/tangxiaoyu/Library/Application Support/typora-user-images/image-20181129004418202.png)

# Step4 校验模型效果

**校验模型**

训练完成后，可以在【我的模型】列表中看到模型效果，以及详细的模型评估报告。

如果单个分类/标签的图片量在100张以内，这个数据基本参考意义不大。

实际效果可以在左侧目录中找到【模型校验】功能进行校验，或者发布为接口后测试。

模型校验功能示意图： ![img](https://ai.bdstatic.com/file/CD0BA2B8F22D407FB716EA48881BB498)

**模型迭代**

一个模型很难一次性就训练到最佳的效果，可能需要结合模型评估报告和校验结果不断扩充数据和调优。

为此我们特地设计了模型迭代功能，即当模型训练完毕后，会生成一个最新的版本号，首次V1、之后V2……以此类推，只要确定了一个模型后，之后有迭代需求只需要在训练模型中选择一个历史版本号，就可以基于历史版本进行迭代。

**注意：如果模型已经是上线状态，依然支持模型迭代，只是需要在训练完毕后更新下线上服务接口，在接口地址不变的情况下可以持续优化效果。**

# Step5 发布模型—在线API

**发布模型生成在线API**

训练完毕后就可以在左侧目录栏中找到【发布模型】，发布模型表单页面需要自定义接口地址后缀、服务名称，即可申请发布。

**申请发布后，通常的审核周期为T+1，即当天申请第二天可以审核完成，如果需要加急、或者遇到莫名被拒的情况，可以加入官方QQ群（群号:679517246）联系群管解决** 发布模型界面示意：

![img](https://ai.bdstatic.com/file/9E692190017741279D6F1E3212E779C6)

你可能关心的几个问题：

1、接口上线后是否收费？

答：目前接口是限量免费使用的原则，上线模型后可以免费获得500次/天，qps=2的调用限额。**超过这个量级可以加入官方QQ群（群号:679517246）联系群管扩容**。费用问题不用太过担心，如果所需要的量级非常高，可能会适当基于实际要求适当收费。价格可以以其他已推出的图像识别的价格作为参考。

2、模型能否支持私有化部署？

答：目前我们提供的方案包括在线API，以及即将上线的离线SDK，支持在线调用及设备端本地计算，目前尚不支持服务端的私有化部署。

3、模型上线后还可以继续进行优化么？

答：已经上线的模型依然可以持续优化，操作上还是按照标准流程在训练模型中-选择要优化的模型和数据完成训练，然后在模型列表中更新线上服务，完成模型的优化。

点击我的模型列表——找到新训练好的模型版本——点击申请发布

![img](https://ai.bdstatic.com/file/91B17D990EAF415097F5764CF6C70DCF)

在出来的弹窗中点击确定

![img](https://ai.bdstatic.com/file/C4EFE8BAFBB8488CA415F7346507143E)

**接口赋权**

在正式使用之前，还需要做的一项工作为接口赋权，需要登录[EasyDL控制台](https://console.bce.baidu.com/ai/?_=1537962688426&ed&no_xss&locale=zh-cn#/ai/easydl/overview/index)中创建一个应用，获得由一串数字组成的appid，然后就可以参考接口文档正式使用啦。

同时支持[在EasyDL控制台-云服务权限管理](https://console.bce.baidu.com/ai/?_=1537962688426&ed&no_xss&locale=zh-cn#/ai/easydl/service/list)中为第三方用户配置权限

示意图如下：

![img](https://ai.bdstatic.com/file/6AA1AB4533C3420BA4A611739219B6D3)

![img](https://ai.bdstatic.com/file/71F7F8B0502246E998E9CC1639978FC6)

# Step5 发布模型—离线SDK

当离线sdk的模型训练完毕后，可以在发布模型中申请发布为离线sdk。 ![img](https://ai.bdstatic.com/file/73F26B52F4424A018B79618118DD16B5)

说明:

① 图像分类模型生成的离线SDK： 目前仅支持Android系统且芯片为NPU类型的终端运行 目前芯片为NPU的主要移动端机型有：华为 荣耀V10、 mate10 、mate10pro、 p20、 p20pro 尚不支持非NPU芯片运行

② 物体检测模型生成的离线SDK： 支持Android及iOS两种操作系统 iOS支持8.0及以上版本，支持在armv7 、arm64离线运行 Android支持4.1及以上版本，支持在armeabi-v7a 、arm64-v8a开发板离线运行

③申请发布操作后需要等待3天左右的审核期，期间可能有工作人员电话沟通，请耐心等待并保持电话畅通。

④ 审核通过后可获得可直接体验的移动端app安装包，以及相应代码包、说明文档，供企业用户/开发者二次开发。

# 模型迭代

一个模型很难一次性就训练到最佳的效果，可能需要结合模型评估报告和校验结果不断扩充数据和调优。

为此我们特地设计了模型迭代功能，即当模型训练完毕后，会生成一个最新的版本号，首次V1、之后V2……以此类推，只要确定了一个模型后，之后有迭代需求只需要在训练模型中选择一个历史版本号，就可以基于历史版本进行迭代。

**注意：如果模型已经是上线状态，依然支持模型迭代，只是需要在训练完毕后更新下线上服务接口，在接口地址不变的情况下可以持续优化效果。**

点击我的模型列表——找到新训练好的模型版本——点击申请发布

![img](https://ai.bdstatic.com/file/91B17D990EAF415097F5764CF6C70DCF)

在出来的弹窗中点击确定

![img](https://ai.bdstatic.com/file/C4EFE8BAFBB8488CA415F7346507143E)

# 云服务调用数据管理

开通云服务调用数据管理功能后，用户可管理调用云服务接口时上传的数据，提高模型的迭代效率，实现训练数据的持续丰富和模型效果的持续优化。

具体使用流程如下：

1. 为已上线接口开通云服务调用数据服务 ![img](https://ai.bdstatic.com/file/F2AE48AE07C648F39E7AD73682A3C963)
2. 开通第三天即可查看调用数据（若周一开通功能，则周三起可以查看数据），通过选择调用时间、分类/标签，并设置筛选条件，查看疑似错误识别的图片 ![img](https://ai.bdstatic.com/file/8E17EE39D79A4686B41905740AF518E9)
3. 将接口识别错误的图片重新标注，并添加到指定数据集（建议新建数据集）。后续训练模型时，只需增加包含接口数据的数据集，即可提升模型效果





# 模型训练攻略

根据应用场景的不同，模型有不同的设计要点。

# 图像内容识别及审核

实现为海量图片定制打标签，或者定制图像审核标准。

**>> 海量图片打分类标签**

将图片数据进行定制分类标签并进行训练，实现对海量图片自动打标签，实现将图片更好的在前端面向C端用户展示，以及判断用户点击图片内容从而进行相关图片推荐等功能。

**示例：**

家装网站可以将图片分类为：卧室、餐厅、厨房等。

**>> 图像审核**

根据实际业务场景需求，制定图片审核标准，用EasyDL判断是否合规。常用于视频、新闻等内容平台定制内容审核策略，过滤不良信息。或用于线上活动，判断C端用户提交图片的合规性。

**示例：**

房产网站需要审核用户提交的信息是否为户型图、房源图、还是非房源图片。

# 视频或图像监控

实现监控现场特定的行为或状态，支持离线部署用于实时预警。

**>> 生产环境安全监控**

对生产环境现场做安全性监控，如是否出现挖掘机等危险物品、工人是否佩戴安全帽、是否穿工作服等进行检查，辅助人工判断安全隐患并及时预警，保证生产环境安全运行。

**示例：**

如输电线路附近的安全性检查，需要检测是否存在挖掘机、吊车等外部隐患物体。

**>> 超市防损监控**

对超市结算台下层安装的摄像头采集图片进行标注及训练，实现实时监测购物车下层图片中是否有未结算商品。

**示例：**

对于超市购物车下层安装摄像头视频，抽帧后判断有商品、无商品、无车、非购物车。

**>> 货物状态监控**

根据实际业务场景安装摄像头，采用定时抓拍或视频抽帧的方式，自动判断货物状态，提升业务运营、货品管理效率。

**示例：**

对于货船调运公司，智能监控船只上货品状态为有货或无货。

# 工业生产质检

支持在工业生产线定制模型实现良次品判断、瑕疵检测、零件分拣。

**>> 产品组装合格性检查**

在流水线作业中针对组合型产品可能存在的不合格情况进行列举，并投入示例图片进行训练，从而训练出自动判断合格或不合格的模型，辅助人工判断产品质量。

**示例：**

键盘生产可能存在错装、漏装、合格进行分类识别。

**>> 瑕疵检测**

针对微小的瑕疵检测，针对原始图片或基于光学成像图片进行瑕疵标注及训练，将模型集成在检测器或流水线中，辅助人工提升质检效率，降低成本。

**示例：**

针对地板质检的常见问题进行识别，例如虫眼、毛面、棘爪等。

# 零售商品识别

支持定制商品识别，实现拍摄货架判断陈列合规性、自助结算等业务场景。

**>> 货架陈列合规性检查**

将商品陈列图片进行标注及训练，集成在app中供巡货员或者店员拍照上传自动识别，通过系统自动检查完成合规性的准确检查。

**示例：**

在货架及货柜中检测出第一排商品的个数、位置。

**>> 自助结算**

对业务场景售卖的商品进行图片标注及训练，与摄像头、支付系统结合，辅助实现自助结算的流程。

**示例：**

结合在结算台的商品图像，识别出具体商品。

**>> 互动营销**

训练定制的商品识别，实现对C端用户提交的商品图片进行识别，配合游戏规则完成闯关/抽奖式的互动营销。

**示例：**

对KFC食物进行检测后，为就餐顾客提供拍照闯关活动。

**>> 电商平台商品管理**

根据对商品细节进行标注和模型训练，实现商品图库的快速分类，减少人工标注的成本。

**示例：**

针对鞋类商品判断鞋底花纹图案。

# 专业领域研究

支持定制医疗、农业等细分类识别模型，如中草药识别、鸟类识别、寄生虫识别等，将研究成果有效传承。

**>> 医疗镜检识别**

针对医疗检验场景中，可能存在的正常或异常的结果判断，基于图片关键特征进行标注进行物体检测/图像分类训练，协助医生高效完成结果判断。

**示例：**

针对寄生虫卵镜检图片，判断虫卵类型从而对症下药。

**>> 培训或科普**

将专业图片识别进行标注及模型训练，配合详细的图片介绍信息，集成在公司内部使用的培训app中，方便新人通过拍照识图快速上手业务。

**示例：**

车辆零部件识别。





# API参考

本文档主要说明定制化模型发布后获得的API如何使用，如有疑问可以通过以下方式联系我们：

- 在百度云控制台内[提交工单](http://ticket.bce.baidu.com/#/ticket/create)，咨询问题类型请选择**人工智能服务**
- 进入[EasyDL社区交流](http://ai.baidu.com/forum/topic/list/190) ,与其他开发者进行互动
- 加入EasyDL官方QQ群（群号:679517246）联系群管

# 图像分类API

## 接口描述

基于自定义训练出的图像分类模型，实现个性化图像识别。模型训练完毕后发布可获得定制化图像分类API 详情访问：[定制化图像训练平台](http://ai.baidu.com/customize/common/welcome)进行训练。 更多训练模型过程中的常见问题查看 <http://ai.baidu.com/docs#/FAQ/504affa3> 如有其它问题，欢迎加入QQ群679517246进行咨询。

## 接口鉴权

1、在[EasyDL——控制台](https://console.bce.baidu.com/ai/?_=1537962688426&ed&no_xss&locale=zh-cn#/ai/easydl/overview/index)创建应用 ![img](https://ai.bdstatic.com/file/03939F1583E842A7BEC009A1F8EEA002)![img](https://ai.bdstatic.com/file/00A1C077A1974A7CBB6045D1ED7E92EE)

2、应用详情页获取AK SK ![img](https://ai.bdstatic.com/file/14D747B7499E4ED2B7F9BD07230FEC58)

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： 请首先在[定制化图像训练平台](http://ai.baidu.com/customize/common/welcome)进行自定义模型训练，完成训练后可在服务列表中查看并获取url。

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token,参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值               |
| :----------- | :--------------- |
| Content-Type | application/json |

注意：如果出现336001的错误码很可能是因为请求方式错误，与其他图像识别服务不同的是定制化图像识别服务以json方式请求。

Body请求示例：

```
{
    "image": "<base64数据>",
    "top_num": 5
}
```

Body中放置请求参数，参数详情如下：

**请求参数**

| 参数    | 是否必选 | 类型   | 可选值范围 | 说明                                                         |
| :------ | :------- | :----- | :--------- | :----------------------------------------------------------- |
| image   | 是       | string | -          | 图像数据，base64编码，要求base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式 **注意请去掉头部** |
| top_num | 否       | number | -          | 返回分类数量，默认为6个                                      |

**请求代码示例**

**提示一**：使用示例代码前，请记得替换其中的示例Token、图片地址或Base64信息。

**提示二**：部分语言依赖的类或库，请在代码注释中查看下载地址。

bash

PHP

Java

Python

C++

C#

```
easydl图像分类
curl -i -k '【接口地址】?access_token=【调用鉴权接口获取的token】' --data '{"image":"sfasq35sadvsvqwr5q...","top_num":"5"}' -H 'Content-Type:application/json; charset=UTF-8'
```

## 返回说明

**返回参数**

| 字段    | 是否必选 | 类型          | 说明                       |
| :------ | :------- | :------------ | :------------------------- |
| log_id  | 是       | number        | 唯一的log id，用于问题定位 |
| results | 否       | array(object) | 分类结果数组               |
| +name   | 否       | string        | 分类名称                   |
| +score  | 否       | number        | 置信度                     |

# 物体检测API文档

## 接口描述

基于自定义训练出的物体检测模型，实现定制图像识别。 详情访问：[定制化图像训练平台](http://ai.baidu.com/customize/common/welcome)进行训练。 更多训练模型过程中的常见问题查看 [http://ai.baidu.com/docs#/FAQ/504affa3](http://ai.baidu.com/customize/common/welcome) 如有其它问题，欢迎加入QQ群679517246进行咨询。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： 请首先在[定制化图像训练平台](http://ai.baidu.com/customize/common/welcome)进行自定义模型训练，完成训练后申请上线，上线成功后可在服务列表中查看并获取url。

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token,参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值               |
| :----------- | :--------------- |
| Content-Type | application/json |

注意：如果出现336001的错误码很可能是因为请求方式错误，与其他图像识别服务不同的是定制化图像识别服务以json方式请求。

Body请求示例：

```
{
    "image": "<base64数据>"
}
```

Body中放置请求参数，参数详情如下：

**请求参数**

| 参数      | 是否必选 | 类型   | 可选值范围 | 说明                                                         |
| :-------- | :------- | :----- | :--------- | :----------------------------------------------------------- |
| image     | 是       | string | -          | 图像数据，base64编码，要求base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式 **注意请去掉头部** |
| threshold | 否       | number | -          | 默认值为0.3，请在我的模型列表-模型效果查看推荐阈值           |

**请求代码示例**

**提示一**：使用示例代码前，请记得替换其中的示例Token、图片地址或Base64信息。

**提示二**：部分语言依赖的类或库，请在代码注释中查看下载地址。

bash

PHP

Java

Python

C++

C#

```
easydl物体检测
curl -i -k '【接口地址】?access_token=【调用鉴权接口获取的token】' --data '{"image":"sfasq35sadvsvqwr5q..."}' -H 'Content-Type:application/json; charset=UTF-8'
```

## 返回说明

**返回参数**

| 字段      | 是否必选 | 类型          | 说明                                   |
| :-------- | :------- | :------------ | :------------------------------------- |
| log_id    | 是       | number        | 唯一的log id，用于问题定位             |
| results   | 否       | array(object) | 识别结果数组                           |
| +name     | 否       | string        | 分类名称                               |
| +score    | 否       | number        | 置信度                                 |
| +location | 否       |               |                                        |
| ++left    | 否       | number        | 检测到的目标主体区域到图片左边界的距离 |
| ++top     | 否       | number        | 检测到的目标主体区域到图片上边界的距离 |
| ++width   | 否       | number        | 检测到的目标主体区域的宽度             |
| ++height  | 否       | number        | 检测到的目标主体区域的高度             |

# 错误码

若请求错误，服务器将返回的JSON文本包含以下参数：

- **error_code：**错误码。
- **error_msg：**错误描述信息，帮助理解和解决发生的错误。

例如Access Token失效返回：

```
{
  "error_code": 110,
  "error_msg": "Access token invalid or no longer valid"
}
```

需要重新获取新的Access Token再次请求即可。

| 错误码 | 错误信息                                | 描述                                                         |
| :----- | :-------------------------------------- | :----------------------------------------------------------- |
| 1      | Unknown error                           | 服务器内部错误，请再次请求， 如果持续出现此类错误，请通过QQ群（649285136）或工单联系技术支持团队。 |
| 2      | Service temporarily unavailable         | 服务暂不可用，请再次请求， 如果持续出现此类错误，请通过QQ群（649285136）或工单联系技术支持团队。 |
| 3      | Unsupported openapi method              | 调用的API不存在，请检查后重新尝试                            |
| 4      | Open api request limit reached          | 集群超限额                                                   |
| 6      | No permission to access data            | 无权限访问该用户数据                                         |
| 13     | Get service token failed                | 获取token失败                                                |
| 14     | IAM Certification failed                | IAM鉴权失败                                                  |
| 15     | app not exsits or create failed         | 应用不存在或者创建失败                                       |
| 17     | Open api daily request limit reached    | 每天请求量超限额，已上线计费的接口，请直接在控制台开通计费，调用量不受限制，按调用量阶梯计费；未上线计费的接口，请通过QQ群（649285136）联系群管手动提额 |
| 18     | Open api qps request limit reached      | QPS超限额，已上线计费的接口，请直接在控制台开通计费，调用量不受限制，按调用量阶梯计费；未上线计费的接口，请通过QQ群（649285136）联系群管手动提额 |
| 19     | Open api total request limit reached    | 请求总量超限额，已上线计费的接口，请直接在控制台开通计费，调用量不受限制，按调用量阶梯计费；未上线计费的接口，请通过QQ群（649285136）联系群管手动提额 |
| 100    | Invalid parameter                       | 无效的access_token参数，请检查后重新尝试                     |
| 110    | Access token invalid or no longer valid | access_token无效                                             |
| 111    | Access token expired                    | access token过期                                             |
| 282000 | internal error                          | 服务器内部错误，请再次请求， 如果持续出现此类错误，请通过QQ群（649285136）或工单联系技术支持团队。 |
| 216100 | invalid param                           | 请求中包含非法参数，请检查后重新尝试                         |
| 216101 | not enough param                        | 缺少必须的参数，请检查参数是否有遗漏                         |
| 216102 | service not support                     | 请求了不支持的服务，请检查调用的url                          |
| 216103 | param too long                          | 请求中某些参数过长，请检查后重新尝试                         |
| 216110 | appid not exist                         | appid不存在，请重新核对信息是否为后台应用列表中的appid       |
| 216200 | empty image                             | 图片为空，请检查后重新尝试                                   |
| 216201 | image format error                      | 上传的图片格式错误，现阶段我们支持的图片格式为：PNG、JPG、JPEG、BMP，请进行转码或更换图片 |
| 216202 | image size error                        | 上传的图片大小错误，现阶段我们支持的图片大小为：base64编码后小于4M，分辨率不高于4096*4096，请重新上传图片 |
| 216203 | image size error                        | 上传的图片base64编码有误，请校验base64编码方式，并重新上传图片 |
| 216630 | recognize error                         | 识别错误，请再次请求，如果持续出现此类错误，请通过QQ群（649285136）或工单联系技术支持团队。 |
| 216631 | recognize bank card error               | 识别银行卡错误，出现此问题的原因一般为：您上传的图片非银行卡正面，上传了异形卡的图片或上传的银行卡正品图片不完整 |
| 216633 | recognize idcard error                  | 识别身份证错误，出现此问题的原因一般为：您上传了非身份证图片或您上传的身份证图片不完整 |
| 216634 | detect error                            | 检测错误，请再次请求，如果持续出现此类错误，请通过QQ群（649285136）或工单联系技术支持团队。 |
| 282003 | missing parameters: {参数名}            | 请求参数缺失                                                 |
| 282005 | batch  processing error                 | 处理批量任务时发生部分或全部错误，请根据具体错误码排查       |
| 282006 | batch task  limit reached               | 批量任务处理数量超出限制，请将任务数量减少到10或10以下       |
| 282100 | image transcode error                   | 图片压缩转码错误                                             |
| 282101 | image split limit reached               | 长图片切分数量超限                                           |
| 282102 | target detect error                     | 未检测到图片中识别目标                                       |
| 282103 | target recognize error                  | 图片目标识别错误                                             |
| 282114 | url size error                          | URL长度超过1024字节或为0                                     |
| 282808 | request id: xxxxx not exist             | request id xxxxx 不存在                                      |
| 282809 | result type error                       | 返回结果请求错误（不属于excel或json）                        |
| 282810 | image recognize error                   | 图像识别错误                                                 |
| 283300 | Invalid argument                        | 入参格式有误，可检查下图片编码、代码格式是否有误             |
| 336000 | Internal error                          | 服务器内部错误，请再次请求， 如果持续出现此类错误，请通过QQ群（679517246）或工单联系技术支持团队 |
| 336001 | Invalid Argument                        | 入参格式有误，比如缺少必要参数、图片base64编码错误等等，可检查下图片编码、代码格式是否有误。有疑问请通过QQ群（679517246）或工单联系技术支持团队 |





# 离线SDK简介

本文档主要说明定制化模型发布后获得的SDK如何使用，如有疑问可以通过以下方式联系我们：

- 在百度云控制台内[提交工单](http://ticket.bce.baidu.com/#/ticket/create)，咨询问题类型请选择**人工智能服务**
- 进入[EasyDL社区交流](http://ai.baidu.com/forum/topic/list/190) ,与其他开发者进行互动
- 加入EasyDL官方QQ群（群号:679517246）联系群管

## SDK说明

目前图像分类和物体检测均可发布模型生成离线SDK，SDK支持IOS及ANDROID两种操作系统。Windows及linux系统暂不支持，敬请期待。 以下为IOS及ANDROID SDK的系统支持及大小说明：

| 操作系统 | 系统支持       | 硬件环境要求                                                 |
| :------- | :------------- | :----------------------------------------------------------- |
| IOS      | IOS 8.0以上    | armv7 arm64 (Starndard architectures)（暂不支持模拟器）      |
| ANdroid  | Android 21以上 | Android 21 以上，CPU架构仅支持armeabi-v7a arm-v8a,即绝大部分的手机和平板 |

**说明**

> 虽然SDK库文件很大（ipa文件很大），但最终应用在用户设备中所占用的大小会**缩小很多**，**如图像分类下载的ipa文件可能会100M以上，但实际集成在设备中只有20M左右**。 这与multi architechtures、bitcode和AppStore的优化有关。

## SDK获取及使用流程

**发布模型获取SDK**

在[EasyDL定制化训练平台](http://ai.baidu.com/easydl)完成模型训练后，点击发布模型，依次选择模型、应用类型、版本。其中应用类型选择SDK，选择操作系统后申请发布。 ![img](https://ai.bdstatic.com/file/26A17F2A84D14199833CA002F652C151)经过0-1个工作日的审核周期，通过审核后，即可在模型列表处-相应模型查看离线sdk。同时可在离线SDK弹窗中直接下载试用版离线SDK. ![img](https://ai.bdstatic.com/file/BAA528E781F24E83B47798E713B37F84)

**试用版SDK与正式版区别**

试用版和正式版的SDK在功能/性能上无区别，唯一区别为使用有效期及设备数限制上。试用版有效期为三个月（超过可在线申请延期），且每个模型的试用版限制2台设备上使用。

> 正式版一旦获得可同时申请多台设备离线运行，并永久使用。

**使用SDK** 使用SDK需要在[EasyDL控制台——离线SDK](https://console.bce.baidu.com/ai/?_=1537962688426&ed&no_xss&locale=zh-cn#/ai/easydl/sdk/list)管理进行 无论试用版SDK或正式版SDK，使用上分以下四步： ① 下载SDK，获取SDK需要在[EasyDL控制台-离线SDK](https://console.bce.baidu.com/ai/?_=1537962688426&ed&no_xss&locale=zh-cn#/ai/easydl/sdk/list)管理下载。 ![img](https://ai.bdstatic.com/file/56E15C74184D4157BF6D5187C3357D3E)点击下载按钮后，选择**已发布的模型及版本**进行下载。 ![img](https://ai.bdstatic.com/file/A48C9C3889E540A6BFCA92291FA46E95)② 在控制台获取序列号 ![img](https://ai.bdstatic.com/file/F4A4ED41994943FABB4F5764AA93F895)③ 本地运行SDK，并完成首次联网激活 ![img](https://ai.bdstatic.com/file/159CA7769C2F4652ADB862AC9DAE8520) ④ 正式使用

# SDK常见问题

1、激活失败怎么办？ 激活失败可能由于以下几个原因造成： ①当前序列号已被其他设备激活使用，请核实序列号后用未被激活的序列号重新激活 ②序列号填写错误，请核实序列号后重新激活 ③同一台设备绑定同一个序列号激活次数过多（超过50次），请更换序列号后重试 ④首次激活需要联网，网络环境不佳或无网络环境，请检查网络环境后重试 ⑤模型发布者和序列号所属账号非同一账号，如果存在这种异常建议更换账号获取有效序列号 ⑥序列号已过有效期，请更换序列号后重试 ⑦如有其他异常请加入定制化训练平台QQ群679517246进行咨询。

2、怎样申请正式版？ 正式版可以在[EasyDL控制台——离线SDK管理——新增设备](https://console.bce.baidu.com/ai/?_=1537962688426&ed&no_xss&locale=zh-cn#/ai/easydl/sdk/create)进行申请，申请流程： 1、企业认证 2、填写申请信息 ![img](https://ai.bdstatic.com/file/E24A2559692840989C35D550AC3E91CA)3、等待审核：审核周期通常需要1-3个工作日左右，期间会有工作人员电话回访，请填写有效的联系方式并保证手机畅通。

3、SDK与API效果上是否有差异？ 离线SDK与在线API背后的模型采用了不同的算法，主要的考虑点在于本地计算通常受存储、硬件环境受限的原因，所以在效果表现上有一定差异，通常情况下在线API效果会明显更佳。

# EasyDL-IOS-SDK 集成文档

## 系统支持

系统：iOS 8.0 以上 硬件：armv7 arm64 (Starndard architectures) （暂不支持模拟器）

## SDK大小说明

SDK库的二进制与_TEXT增量约3M，物体检测模型约30M，图像分类模型约100M。

物体检测模型的DemoApp在iPhone 6, iOS 11.4下占用空间实测小于30M。

> 虽然SDK库文件很大（ipa文件很大），但最终应用在用户设备中所占用的大小会缩小很多。 这与multi architechtures、bitcode和AppStore的优化有关。

## Release Notes

| 时间       | 版本  | 说明                 |
| :--------- | :---- | :------------------- |
| 2018.09.25 | 0.2.0 | 新增图像分类         |
| 2018.06.05 | 0.1.0 | 第一版！支持物体检测 |

## 集成

## DemoApp的使用

从官网下载的SDK和XCode工程已集成开发者已经训练完成的模型，修改Code Sign相关之后，即可直接运行。 若报错 libEasyDl.a not found，自行添加 ${SDK_ROOT}/LIB/libEasyDL.a 即可。

## 如何集成

SDK的工程结构如下：

```
${SDK_ROOT}
	├─ EasyDLDemo  // Demo工程文件
	├─ LIB         // 库文件
	├─ RES         // 资源文件
	├─ DOC 		   // 文档
```

请开发者参考DemoApp的使用方法对SDK进行集成。 集成过程中，请务必参考Demo工程对库的引用方式，引入LIB文件夹下的所有库文件：

```
libEasyDL.a 
libgflags.a 
libglog.a 
libpaddle_capi_engine.a 
libpaddle_capi_layers.a 
libprotobuf-lite.a
libstdc+.tbd
libz.a
Accelerate.framework
```

并添加OtherLinkFlags: -force_load ${PROJECT_DIR}/../LIB/lib/libpaddle_capi_layers.a

最后import 头文件 <EasyDL/EasyDL.h> 即可。

## 调用流程

### 1. 设置序列号

序列号请在应用控制台申请

```
[EasyDL setSerialNumber:@"!!!Enter Your Serial Number Here!!!"];
```

### 2. 初始化模型

```
NSString* model = [[NSBundle mainBundle] pathForResource:@"EZDL" ofType:@"model"];
NSString* config = [[NSBundle mainBundle] pathForResource:@"conf" ofType:@"json"];
NSString* labels = [[NSBundle mainBundle] pathForResource:@"label_list" ofType:@"txt"];
NSString* preprocess = [[NSBundle mainBundle] pathForResource:@"preprocess_args" ofType:@"json"];    
NSError* err;
EasyDLModel* model = [EasyDLModel model:model byConfig:config andPreprocess:preprocess andLabel:labels withError:&err];
```

### 3. 调用检测接口

```
UIImage *img = .....;
NSArray *result = [model detect:img withFilterScore:0 andError:&err];
```

## 接口说明

### 图像相关

物体检测类定义在 EasyDLSSDModel.h 中

** 检测图像 **

```
/**
 * 检测图像
 * @param image 带检测图像
 * @param score 只返回得分高于score的结果(0 ~ 1)
 * @return 成功返回识别结果，NSArray的元素为对应模型的结果类型；失败返回nil，并在err中说明错误原因
 */
- (NSArray *)detect:(UIImage *)image
    withFilterScore:(CGFloat)score
           andError:(NSError **)err;
```

** 检测原始图片数据 **

```
/**
 * 检测原始图片数据
 * @param pixels 待检测图像数据
 * @param height 图像高度（旋转前）
 * @param width 图像宽度（旋转前）
 * @param format 图像格式
 * @param rotate 图像旋转
 * @param filterScore 只返回得分高于score的结果(0 ~ 1)
 * @return 成功返回识别结果，NSArray的元素为对应的结果类型；失败返回nil，并在err中说明错误原因
 *
 */
- (NSArray *)inference:(unsigned char *)pixels
            withHeight:(size_t)height
             withWidth:(size_t)width
            withFormat:(EImageByteOrder)format
            withRotate:(EImageRotation)rotate
       withFilterScore:(float)filterScore
              andError:(NSError **)err;
```

结果数据的返回类型：

| 模型类型      | 类型                      |
| :------------ | :------------------------ |
| 图像-物体检测 | EasyDLObjectDetectionData |
| 图像-图像分类 | EasyDLClassfiData         |

## 错误说明

SDK的方法会返回NSError错，直接返回的NSError的错误码定义在EEasyDLErrorCode中。 NSError附带message（有时候会附带NSUnderlyingError），开发者可根据code和message进行错误判断和处理。

# EasyDL-ANDROID-SDK 集成文档

**物体识别DETECT**

- INFER: Android 21 以上，CPU架构仅支持armeabi-v7a arm-v8a,即绝大部分的手机和平板

**图片分类CLASSIFY** Android 19 以上，

- DDK：CPU仅支持华为带NPU的arm-v8a的soc，支持的机型 mate10，mate10pro，P20等，需要额外的so支持
- SNPE：高通Soc，推荐高通Snapdragon 660, 710及820 之后的soc。这些型号含有 Hexagon DSP模块
- INFER：任意机型

## 调用流程

### 读取配置

- easydl-infer-detect: 为使用infer引擎，物体识别功能的配置 `InferConfig`
- easydl-infer-classify：为使用infer引擎，物体分类功能的配置 `InferConfig`
- easydl-snpe-classify：为使用snpe引擎，物体分类功能的配置 `SnpeConfig`
- easydl-ddk-classify：为使用ddk引擎，物体分类功能的配置 `DDKConfig`

**如无特殊原因，配置文件不需要改动** 其中model_file_asset_path参数表示assets目录下模型文件的路径

#### 配置类 XxxxConfig

SnpeConfig DDKConfig 同 InferConfig

下文以easydl-infer-classify及对应的InferConfig为例： 配置文件示例可以参见, assets\easydl-infer-classify\config.json

```
  InferConfig mInferConfig = new InferConfig(getAssets(),
                                "easydl-infer-classify/config.json");
// 

// 您也可以自己读取config.json
new InferConfig(content);
```

### 引擎管理

#### 引擎管理类 XxxxManager

- INFER：任意机型，`InferManager`
- -SNPE：高通Soc，`SnpeManager`
- DDK：华为Soc，`DdkManager`

#### 激活

每一个版本的模型， SDK强制首次使用联网激活，如app不能联网或者联网激活失败，则SDK不能使用。 激活方法：

```
//InferManager 为例:
new InferManager(this, mInferConfig, SERIAL_NUM);

// SERIAL_NUM为我们easydl平台上申请的序列号，一次激活后有效期内不必再次联网。
// 激活失败抛出ActivationException异常, 具体请参见激活部分文档
```

### 初始化

```
//InferManager 为例:
new InferManager(this, mConfig, SERIAL_NUM);

// this 为Android自带的Context
// mConfig为infer引擎对应的配置类
// SERIAL_NUM为我们easydl平台上申请的序列号
```

**注意:**

1. new InferManager() 会自动调用激活方法， 如果SDK没有在百度服务器端激活，会有异常抛出，SDK不能使用。
2. 同一个时刻只能有一个有效的InferManager。旧的InferManager必须调用destory后，才能新建一个new DetectManager() 。
3. InferManager的任何方法，都不能再UI线程中调用。
4. new InferManager() 及InferManager成员方法由于线程同步数据可见性问题，都必须在一个线程中执行。如使用android自带的ThreadHandler类。

### 识别Detect（分类classify可以跳过此节）

#### 输入

- 输入为Bitmap，其中Bitmap的options为默认。如果强制指定的话，必须使用*Bitmap.Config.ARGB_8888*
- 输出是多个DetectionResultModel
- confidence, 置信度[0-1），小于confidence的结果不返回。 填confidence=0，返回所有结果

```
public interface DetectInterface {
     List<DetectionResultModel> detect(Bitmap bitmap, float confidence) throws BaseException;
```

#### 输出

DetectionResultModel

- label： 标签， 定义在config.json中
- confidence ：置信度
- bounds：Rect， 左上角和右下角坐标

### 图像分类classify（识别Detect可以忽略此节）

#### 输入

- 输入为Bitmap，其中Bitmap的options为默认。如果强制指定的话，必须使用*Bitmap.Config.ARGB_8888*
- 输出是多个ClassificationResultModel （ddk 引擎至多5个，infer及snpe至多10个）

```
public interface ClassifyInterface {
    List<ClassificationResultModel> classify(Bitmap bitmap) throws BaseException;
```

#### 输出

ClassificationResultModel

- label： 标签， 定义在config.json中
- confidence ：置信度
- lableIndex：标签对应的序号

#### 释放

DetectManager对象释放后才可以新建一个DetectManager

```
 public synchronized void destory() throws BaseException 
```

### 整体示例

以下以SnpeManager为例子， 分类 assets目录下一张图片的整体流程

```
 // 在新线程且同一线程中运行以下代码

    private void testImageFile() throws IOException, BaseException {

        try {
             // 1. 读取配置文件
            SnpeClassifyConfig config = new SnpeClassifyConfig(context.getAssets(), "easydl-snpe-classify/config.json");
             // 2.  新建SnpeManager对象
            SnpeManager manager = new SnpeManager(context, config, "");
            
            // 准备图片
            InputStream is = context.getAssets().open("easydl-snpe-classify/blue.jpg");
            Bitmap image = BitmapFactory.decodeStream(is);
            
            // 3. 检测
            List<ClassificationResultModel> results = manager.classify(image);
            
             // 4. 释放
             // 不管任何情况，都需要调用destory。 建议放在finally中
            manager.destory();
            return results.get(0).getLableIndex() + ":" + results.get(0).getLabel() + ":"
                    + results.get(0).getConfidence();
        } catch (Exception e) {
            e.printStackTrace();
        }
       
    }
```

## 其它事项

### 代码混淆

easydl-sdk.jar jar包中的class均不需要混淆 proguard-rules.pro中添加

```
-keep class com.baidu.ai.easydl.core.**{ *;}
```

# 错误码

| 错误码 | 错误描述                                                 | 详细描述及解决方法                                           |
| :----- | :------------------------------------------------------- | :----------------------------------------------------------- |
| 1001   | assets 目录下用户指定的配置文件不存在                    | SDK可以使用assets目录下config.json作为配置文件。如果传入的config.json不在assets目录下，则有此报错 |
| 1002   | 用户传入的配置文件作为json解析格式不准确，如缺少某些字段 | 正常情况下，demo中的config.json不要修改                      |
| 1004   | 激活错误                                                 | 激活步骤失败，该步骤需要访问百度服务器，请检查序列号输入是否正确，网络是否联通 |
| 19xx   | Sdk内部错误                                              | 请与百度人员联系                                             |
| 2001   | XxxxMANAGER 只允许一个实例                               | 如已有XxxxMANAGER对象，请调用destory方法                     |
| 2002   | XxxxMANAGER 已经调用过destory方法                        | 在一个已经调用destory方法的DETECT_MANAGER对象上，不允许再调用任何方法 |
| 2003   | 传入的assets下模型文件路径为null                         | XxxxConfig.getModelFileAssetPath() 返回为null。由setModelFileAssetPath(null）导致 |
| 2011   | libeasy-xxxx.so 加载失败                                 | System.loadLibrary("easydl-xxxx"); libeasy-xxxx.so 没有在apk中。CPU架构仅支持armeabi-v7a arm-v8a |
| 2012   | JNI内存错误                                              | heap的内存不够                                               |
| 2601   | assets 目录下模型文件打开失败                            | 请根据报错信息检查模型文件是否存在                           |
| 2611   | 检测图片时，传递至引擎的图片二进制与长宽不符合           | 具体见报错信息                                               |
| 27xx   | Sdk内部错误                                              | 请与百度人员联系                                             |
| 28xx   | 引擎内部错误                                             | 请与百度人员联系                                             |
| 29xx   | Sdk内部错误                                              | 请与百度人员联系                                             |
| 3000   | so加载错误                                               | 请确认所有so文件存在于apk中                                  |
| 3001   | 模型加载错误                                             | 请确认模型放置于能被加载到的合法路径中，并确保config.json配置正确 |
| 3002   | 模型卸载错误                                             | 请与百度人员联系                                             |
| 3003   | 调用模型错误                                             | 在模型未加载正确或者so库为加载正确的情况下调用了分类接口     |

## 2011 错误说明

libeasy-detect.so 没有在apk中。CPU架构仅支持armeabi-v7a arm-v8a 。 将apk改为zip解压， 如果您的手机(绝大部分新手机)是arm-v8a， 检查libs/arm-v8a下有无 libeasy-detect.so。 可以与官方的apk文件的解压结果做对比



# 售卖定制化服务

本文将会引导您将定制化API服务售卖至AI市场中，从而快速获得品牌曝光，并获得丰厚收入。接下来，您将了解如下内容：

- 售卖市场介绍
- 服务售卖需要具备的条件
- 服务售卖的具体流程

## 售卖市场介绍

您在定制化平台训练出的服务可以在[百度AI市场](http://aim.baidu.com/)进行售卖。AI市场是为百度AI技术生态合作伙伴和应用百度AI技术的开发者而提供的第三方市场平台，平台包括技术服务、实体产品、解决方案、产业链周边服务等重要类目，帮助供需双方在市场内建立精准的对接与交易通道。

您将训练完成的API售卖至AI市场中，不仅会获得大量浏览用户关注，增强您企业的品牌曝光。同时，您所训练的服务还将发挥巨大的商业价值，对您服务感兴趣的买家可以在市场中一键下单购买，并集成到业务中使用，您也将获得相应的收入。

## 服务售卖需要具备的条件

您的服务在AI市场出售定制化API服务需要具备以下条件

• 满足AI市场服务商入驻标准：

1. 符合国家相关法律、法规规定，拥有正规的公司资质
2. 使用百度AI技术能力，并成功落地产品及应用
3. 具有专业的技术及售后团队支持，可提供在线客服服务
4. 接受《[百度AI市场入驻协议](http://ai.baidu.com/docs#/AI_Market_ContractForSupplier/top)》及相关协议及管理规范

• 定制化API服务满足售卖要求：

1. 定制化识别接口至少被成功调用1次及以上
2. 图像分类/声音分类模型准确率>80%，且总训练数据量>50
3. 物体检测模型mAP>60%，且总训练数据量>20

## 服务售卖的具体流程

您确认您的企业/单位符合AI市场服务商入驻标准，且有符合售卖条件的API服务，可按如下步骤完成售卖。期间有任何问题，可发邮件至AImarket@baidu.com咨询，我们会尽快与您联系，解答您的疑问，帮您顺利完成。
售卖具体流程如下：

- **1、入驻百度AI市场**

您在AI市场中点击[服务商入驻](https://console.bce.baidu.com/ai/?#/ai/market/provider/register)，按要求填写入驻申请表单，填写完成后，将会有专人对您资质进行审核。审核周期为一天，审核结果将会以短信或电话的方式通知到您。
具体入驻填写规范，请参考AI市场服务商[入驻文档](http://ai.baidu.com/docs#/AI_Marketing_process/5573d971)。

- **2、创建店铺、签订协议**

入驻完成后，在服务商中心将可以创建您个人商铺，按照要求上传店铺图片以及案例内容。如何创建更优质的店铺，请参考[运营宝典](http://ai.baidu.com/docs#/AI_Marketing_methods/01535efe).
入驻AI市场后，将会有市场商务人员与您对接，线下签署《[百度AI市场入驻协议](http://ai.baidu.com/docs#/AI_Market_ContractForSupplier/top)》，协议完成后您将可以发布在线售卖的API服务。

- **3、发布API商品**

第二步完成之后，您可在服务商中心进行商品发布。

商品发布需要填写三部分信息：

- 商品基本信息：具体填写指南，请参考[运营宝典](http://ai.baidu.com/docs#/AI_Marketing_methods/62841fb2)。注意：由于EasyDL服务的限制，买家使用的最大并发限制为2QPS，您可在商品信息中提醒买家，API服务暂无法支持高并发的需求。
- API生产信息：API生产信息主要包括请求参数、请求示例、返回示例以及错误码。由于本部分信息对于买家调用API非常重要，请谨慎填写。具体内容您可按照训练模型的类别，参考EasyDL官方提供的[定制化图像识别API文档](http://ai.baidu.com/docs#/EasyDL_VIS_API/top)进行填写。
- 交付信息：选择线上交易，创建规格。在线售卖API已默认添加0元/20次(买家只能购买一次)的体验版规格，方便买家进行简单测试，您可以根据需要自行删除或添加套餐包规格，例如配置套餐包：配额100次调用，价格10元。

商品信息填写完整后即可提交审核。提交后，将会由专人进行质量审核，约一个工作日给您结果反馈，审核期间，AI市场将会测试调用一次，来确定您的API是否符合售卖标准。

- **4、售卖并获得收入**

您的API服务审核通过并成功上架后，AI市场的买家将可在市场中购买您的服务。 成功付款后，收入将由AI市场代收，您可在月底发起结算申请，结算后所获得的收入=（当月销售额-当月退款额）X （1-平台服务费比例），其中平台服务费比例由入驻时与AI市场共同协商确定。





# 常见问题

## 训练相关问题

**数据处理失败或者状态异常怎么办？**

答：如是是图像分类模型上传处理失败，请先检查已上传的分类命名是否正确，是否存在中文命名、或者增加了空格；然后检查下数据图片量是否超过上限（10万张）；再检查图片中是否有损坏。如果自查没有发现问题可以加入官方QQ群（群号:679517246）咨询解决。

**模型训练失败怎么办？**

答：如果遇到模型训练失败的情况，请直接加入官方QQ群（群号:679517246）咨询解决。

**已经上线的模型还可以继续优化吗？**

答：已经上线的模型依然可以持续优化，操作上还是按照标准流程在训练模型中-选择要优化的模型和数据完成训练，然后在模型列表中更新线上服务，完成模型的优化。

点击我的模型列表——找到新训练好的模型版本——点击申请发布

![img](https://ai.bdstatic.com/file/91B17D990EAF415097F5764CF6C70DCF)

在出来的弹窗中点击确定

![img](https://ai.bdstatic.com/file/C4EFE8BAFBB8488CA415F7346507143E)

## 模型效果相关问题

**模型效果怎么调优？**

答：如果效果不满意先看下训练数据是否和实际场景要识别的图片一致，以及训练数据量是否太少。如果训练数据量已经达到一定丰富度，如单个分类/标签的图片量超过200张以上，效果仍然不佳，可以加入官方QQ群（群号:679517246）咨询群管解决。

## 模型上线相关问题

**希望加急上线怎么处理？**

答：加入官方QQ群（群号:679517246）咨询群管高优审核。

**每个账号可以上线几个模型？是否可以删除已上线的模型？**

答：每个账号最多申请发布十个模型，已上线模型无法删除。

## 收费相关问题

**接口上线后是否收费？调用量不够怎么办？**

答：目前接口是限量免费使用的原则，上线模型后可以免费获得500次/天，qps=2的调用限额。**超过这个量级可以加入官方QQ群（群号:679517246）联系群管扩容**。费用问题不用太过担心，如果所需要的量级非常高，可能会适当基于实际要求适当收费。价格可以以其他已推出的图像识别的价格作为参考。

## 其他问题

**模型能否支持私有化部署？**

答：目前我们提供的方案包括在线API和离线SDK，支持在线调用及设备端本地计算，目前尚不支持服务端的私有化部署。

**申请发布模型审核不通过都是什么原因？**

答：可能原因有，1、经过电话沟通当前模型存在一些问题或者不再使用，如训练数据异常、数据量不够、不想再继续使用等原因，沟通达成一致拒绝。2、电话未接通且模型效果较差，会直接拒绝。如果需要申诉，加入官方QQ群（群号:679517246）咨询群管解决。



