# DuMix AR SDK文档 

# 新手引导

# 简介

Hi，您好，欢迎使用百度AR开放平台提供的AR应用集成开发服务。

本文档主要针对首次使用百度AR开放技术的应用开发者，介绍DuMix AR SDK产品的应用场景，核心优势，接入方式和SDK能力。如果您对文档内容有任何疑问，可以通过以下几种方式联系我们：

- 在百度云控制台内**[提交工单](http://ticket.bce.baidu.com/#/ticket/create)**，咨询问题类型请选择**人工智能>百度AR开放平台**；
- 如有疑问，请进入**[AI社区交流](http://ai.baidu.com/forum/topic/list/175)**
- 欢迎进入[百度AR官网](http://ar.baidu.com/home)了解更多
- 技术咨询邮件：dumix_ar@baidu.com

# 应用场景

DuMix AR SDK是百度AR提供给开发者的AR应用开发集成方案，具有效果领先、快速接入、超轻量、灵活易用、跨平台等特性，提供多种触发方式、感知跟踪、虚实融合、人机交互等AR核心技术。

AR关键技术： ![img](http://agroup-bos.su.bcebos.com/f94274cd0fb69b3cf63d57ade42db3a11c761c3c)

AR技术的主要应用场景包括营销广告、教育培训、娱乐互动、商品展示、特效拍摄、美妆美颜、游戏互动、导航导览等。

**儿童教育案例**

- **爱奇艺爱巴布App【AR课堂】** AR的情景教学，真实还原了海底世界和热带雨林中的动物以及交通工具等等。小朋友不仅可以身临其境的学习百科知识，还可与自己喜爱的动物同框留影，家长可以拍照记录孩子与动物互动的美妙瞬间，在孩子感受到知识趣味性的同时也达到增进亲子关系的效果。

![img](http://agroup-bos.su.bcebos.com/5244ad6f3f448a8c990598da8f27251f278ce5f0)

体验方式：打开爱奇艺奇巴布App，点击首屏左侧小鹿，再次点击右下角AR课堂，进入列表页，点击任一动物或交通工具，即可进入AR认知体验内容。

**IP互动案例**

- **欧莱雅男士变形金刚IP合作** 借势《变形金刚5》强档上映，欧莱雅男士结合影视娱乐IP的跨界合作，通过AR技术与之零距离互动，营销变“型”势不可挡。

识别图：

![img](http://agroup-bos.su.bcebos.com/adf03d26a01edd9ec804e8da3c8a4fee46c13fde)

体验方式：打开最新版百度App，点击搜索框右侧相机入口，选择AR，对准图案即可体验。

效果图：

![img](http://agroup-bos.su.bcebos.com/bf43e90c983be742ac0c25fcc83526f7ad0d0f6e)

**商品包装案例**

- **中秋月饼礼盒** 通过商品包装触发AR，赋予礼品之外的科技象征意义，拓展企业文化赋能的新营销模式。

识别图：

![img](http://agroup-bos.su.bcebos.com/694b10658eccb7e75563b30080879da26d296a79)

体验方式：打开最新版百度App，点击搜索框右侧相机入口，选择AR，对准图案即可体验。

**服装特效案例**

- **AR文化衫** T恤上印制了维密翅膀，看上去跟普通T恤无异，但当你使用手机百度APP扫描时，就会出现相应的维密翅膀模型，可滑动产生粒子效果。

识别图：

![img](http://agroup-bos.su.bcebos.com/539c33aa8c2719320fc69662c39e6e117e0161a7)

体验方式：打开最新版百度App，点击搜索框右侧相机入口，选择AR，对准图案即可体验。

效果图：

![img](http://agroup-bos.su.bcebos.com/8a98e96d142aeea08544c52a70519aed0fa95ab5)

**人脸贴纸特效**

基于百度人脸检测技术，实现人脸2D贴纸特效，广泛应用于特效相机、视频拍摄、直播等App。

体验方式：请[下载DuMix AR SDK Pro 3.0版](http://ai.baidu.com/sdk#ar)，提供的SDK demo中包括20款人脸贴纸特效，如需定制更多贴纸可联系商务合作，请将需求发邮件至ar_business@baidu.com。

效果图：

![img](http://agroup-bos.cdn.bcebos.com/59776d7a866efabdf551e7866c68c40628bd750a)

**实景小游戏**

基于百度AR自研的ARplay引擎能力，以及ARKit平面检测能力，实现基于实景的人机对战小游戏。

体验方式：请[下载DuMix AR SDK Pro 3.0版](http://ai.baidu.com/sdk#ar)，已随Pro 3.0版SDK提供“消灭虫洞”实景小游戏的演示demo。

效果图：

![img](http://agroup-bos.cdn.bcebos.com/ec410485b5a48bc92997b538706dbb19fdb204e3)

# SDK介绍

## AR SDK集成方案

![img](http://agroup-bos.cdn.bcebos.com/68b7dc0b28dcd1a652f591455afed97dfb6d2030)

## AR SDK版本特点

### AR SDK Easy版

- 互动UI开发框架10分钟集成
- 无需AR知识，极简UI级开发框架
- 10分钟快速集成 超低学习门槛，无需AR专业知识
- 内容管理成本低，提供一站式内容开发和部署服务

### AR SDK Pro版

- 内置轻量引擎、可选私有化部署
- 高灵活相机增强框架，可友好集成至各类相机应用
- 内置自研跨平台渲染引擎，支持游戏级内容和交互
- 内容管理方式灵活，任选官方内容服务或私有化部署

### AR SDK Core版

- 接口按需集成、可自选引擎
- 全面开放底层算法能力，支持按需集成，如SLAM、平面跟踪等
- 接口友好，支持自主适配多种主流渲染引擎

### AR SDK Unity版

- 集成AI识图、实例丰富
- 接口简单易用，可快速集成发布至多平台
- 快速集成百度AI图像识别云服务
- 丰富应用实例在Asset Store免费开放下载

## AR SDK能力介绍

![img](http://agroup-bos.cdn.bcebos.com/1eba2bd7329541655bc41e7168e1ee97b4609bf6)

更多内容请访问[AR技术开放平台](https://ar.baidu.com/developer)

# 接入方式

![img](http://agroup-bos.su.bcebos.com/b35fced8c71045c8320cd8048f06edcc3d5433fc)

DuMix AR SDK的应用授权方式为在线授权，每次调起使用SDK时通过token进行鉴权，由百度云和百度AI开放平台提供授权服务，点此[立即使用](https://ar.baidu.com/console#)。

**授权说明**

每个百度账号限100个应用授权，授权可在四个SDK版本通用，授权接口为调起AR，每应用可免费调用100,000次/天。

授权成功后自动开通AR内容平台使用权限，Easy版、Pro版基于百度AR渲染引擎，免费提供AR内容编辑器，并可免费使用AR内容平台提供的内容存储和分发服务，无需开通百度云BOS存储和CSN内容分发服务，不保证并发。

如需提升调用次数和内容分发能力，可进行商务合作咨询ar_business@baidu.com。

**注意：2.5及以上版本AR SDK，试用应用授权下线，并将于2018年9月30日正式下线停止服务端鉴权服务，请尽快将内容平台上的老数据迁移至正式应用授权渠道下，点此查看试用授权，点此前往AR内容平台。**

# 帮助和支持

点此[下载SDK](http://ai.baidu.com/sdk#ar)，查看[开发文档](http://ai.baidu.com/docs#/DuMixAR-Android-SDK/top)，查看[教学视频](http://ai.baidu.com/support/video)。

点此查看[AR场景开发指南](http://ai.baidu.com/docs#/DuMixAR-MakeContent-GettingStarted/top)（仅适用于Easy版和Pro版）。

欢迎进入[技术论坛](http://ai.baidu.com/forum/topic/list/175)交流，百度AR开发者交流群QQ：472081119。

定制化需求请发邮件至商务合作邮箱：ar_business@baidu.com







