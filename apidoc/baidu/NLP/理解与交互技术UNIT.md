# 理解与交互技术UNIT 

# 快速开始

随着AI技术和理念的兴起，很多产品都希望采用对话式的人机交互方式。然而对话系统（对话技能、对话机器人）的研发对于大多数开发者而言却是一个很困难的工作，对技术和数据的要求都很高。

为此，百度将积累多年的自然语言理解与交互技术对外开放，推出了可定制的对话系统开发UNIT（Understanding and Interaction Technology），将业界领先的技术能力输出给广大的开发者，以便降低对话系统的研发门槛。

接下来，你只需花费3~5分钟的时间，即可配置出一个完整的对话技能了。

一个简单的对话技能从无到有需要以下四个步骤。

- 创建技能
- 配置意图及词槽
- 配置训练数据
- 训练模型

其中创建技能，配置意图和词槽这两步是为了定义对话能力。

比如创建一个叫做“查天气”的技能，查天气即为该技能的能力，这个技能里包含了查天气的对话意图，其中时间是这个对话意图的词槽，今天为时间这个词槽的字典值。

配置训练数据和训练模型是为了让UNIT理解你的对话能力。其中训练数据主要是配置一系列的规则，可以配置对话模板或对话集，训练模型则是最后一步，即生成对话模型，此时就可以在UNIT上完成自己的对话了！

## 创建技能

**注：技能即代表某个特定场景下的能力，比如电视指令就是针对一种能力。**

1. 进入UNIT平台-我的技能，选择新建自定义技能。例如取名为“查天气”。 ![img](https://ai.bdstatic.com/file/2BD7D1610D614D76B750000C48C747CD)

## 配置意图及词槽

**注：意图包括对话意图和问答意图，需要参数化的应该使用对话意图，比如订购火车票意图，里面需要车次，出发时间等参数，则应该使用对话意图，问答意图更偏好于解决FAQ类型的问题。比如退票费怎么收？**

1. 点击进入查天气技能，选择新建对话意图。
2. 进入新建对话意图以后，填写意图名和别名。本例中意图名取为WEATHER,别名为查天气。 ![img](https://ai.bdstatic.com/file/83E90D1FF5D940CA9D35EA53DD5F21F9)
3. 在新建对话意图页面，可以添加词槽，这里添加以下几个词槽信息。 *注：UNIT为大家提供了强大的系统词槽，并在不断丰富中，词槽的词典值可以一键选用系统提供的词典，也可以自己添加自定义词典。*

|  词槽名   | 词槽别名 | 系统词典       | 词槽必填 | 澄清话术         |
| :-------: | :------- | :------------- | :------- | :--------------- |
| user_time | 时间     | sys_time(时间) | 必填     | 请澄清一下：时间 |
| user_loc  | 哪里     | sys_loc(地点） | 必填     | 请澄清一下：地点 |

![img](https://ai.bdstatic.com/file/4DB9E24560904B9DB9AB58F79E51DD94)![img](https://ai.bdstatic.com/file/970D33A03B884AB8A7B2FA7182578CA0)4. 配置答复

- 填写完意图和词槽之后，需要配置回复及触发规则，当满足相应规则之后，即可触发相应回复。
- 这里填写回复内容为：”正在为您查询天气...“
- 规则可以选择智能生成。 ![img](https://ai.bdstatic.com/file/CA0E59C3EA2E4863ACA732BCAC44D3FE)

1. 点击保存后，意图配置完成啦。

## 配置训练数据

简单而言，根据规则将一句话拆解成不同的部分标注好，再训练出对话模型，这样UNIT就可以理解用户的对话了。当你富集数据不够多的时候，训练模板可以帮你快速搭建一个对话模型；当你有大量数据时，可以使用对话模板+对话集，使你的对话模型更加强大！

点击左边菜单栏训练数据-对话模板，新增一个对话模板，添加时间，地点词槽，还有文本”天气“，作为三个模板片段。 ![img](https://ai.bdstatic.com/file/403EDB8945324C81A904F76D4B3AD362)**注意：这里user_loc和user_time的两个模板片段前面都选择了否，”天气“选择了是，意味着当一个Query进来时，进行模板匹配时，没有命中时间和地点，只要命中了天气，即命中了该模板。而且这里调节了模板的阈值，如果阈值0.7的话，有些Query就识别不了了哦，具体原因可以点击阈值旁边的问号查看阈值的详细解释哦~**

点击保存，一个对话机器人马上就要诞生啦！

**注：训练模板中可以配置特征词，来辅助匹配用户的Query，达到更好的理解效果，还可以通过调节模板的阈值，是否匹配等功能，以达到更强大的泛化能力，可以深入研究一下哦~**

## 训练模型

选择左侧导航栏中的训练模型，点击训练并生成新模型。 ![img](https://ai.bdstatic.com/file/431836EF561B4742B6E2A42E57EC99A5)

## 验证效果

等待模型训练完成后，点击左侧导航栏的测试按钮，发送”明天北京天气如何？“试试吧！ ![img](https://ai.bdstatic.com/file/19D8212A044B492094D5055254667B56)











# 各功能模块介绍

## 机器人

- 在完整对话环境下的对话理解与交互能力。
- 每个机器人目前可以添加5个对话技能，主要用于解决综合场景下的对话理解与交互任务。您可以将多个技能添加到一个机器人中，赋予这个机器人多种特定场景下的对话理解与交互能力。
- 比如一个儿童故事机的机器人，可以在这个添加讲故事，听儿歌，闲聊等多种对话技能，然后这个机器人即可完成听儿歌，讲故事，闲聊等多种对话。相关操作及说明可以查看[UNIT多技能对话机器人介绍](https://ai.baidu.com/forum/topic/show/941243);

## 技能

- 在特定场景下的对话理解与交互能力。
- 每个账号最多拥有20个自定义技能，主要用于解决特定场景下的对话理解与交互任务。同时UNIT平台提供了一些预置的系统技能（比如问候、电视指令等，更多系统技能会陆续开放），您可根据业务需要直接选用。
- 技能中包含了一系列『对话意图』或『问答意图』用于完成特定场景下的所有对话任务(例如，智能电视技能包含了换台、调音量、找部电影等一系列『对话意图』)。 技能训练出模型并生效到沙盒环境后，您可以在『训练/体验』对话窗口中与技能对话，体验技能的对话理解能力。您也可以在自己的产品中通API直接调用它，让您的产品具有智能对话理解与交互的能力。

## 对话意图

**【基本信息】**

- **对话意图**：intent，是在一个对话任务中技能要理解的用户目的（例如用户说”换到中央台”，那对应电视控制技能下的对话意图就是”换台”）。
- **名称**：对话意图的英文定义，后续您可在程序调用中使用。
- **别名**：对话意图的名称的中文表述，可以创建多个，对应一个对话意图的不同表达方式，可以帮助技能更容易理解一个对话意图的不同表述。

**【词槽】**

- **词槽**：slot，是技能需要理解的用户对话对话意图中的一些限定条件。例如：”换到中央台”中的”中央台”就是一个”电视台词槽”，它会一定程度上影响技能对”换台”这个对话意图的执行。
- **词槽澄清**：
  在口语对话中，用户的一句话 (我们称之为单轮对话) 中常常并不会包含完整的词槽(例如，换台)，而某些词槽对于对话技能的最终执行是不可或缺的（例如，电视台词槽）。因此，对话技能需要对用户发起”词槽澄清”询问（例如，换到哪个台？）来获取”不可或缺”词槽的信息(例如用户回答，要中央台)。UNIT平台可以支持定义哪些词槽是”不可或缺的”，也可以通过调整词槽顺序来定义哪个词槽应该优先被询问。
- **澄清话术**：
  在这里，我们可以定义对话技能进行词槽澄清时询问用户的问题(例如，换到哪个台呢？)。如果不加以定义，系统会使用默认的问题进行询问：请澄清词槽XXX（XXX代表词槽名称）。
- **添加方式**：
  开发者可以自行新建词槽，也可以复用当前技能中已创建过的词槽。此外依托百度的自然语言能力，UNIT平台内建了部分常用词槽供您使用。需要注意的是系统内建词槽是无法被进一步修改的。
- **词典**：
  每个词槽都需要对应一套词典，技能的专名词识别服务会结合词槽绑定的词典来识别用户问话中的词槽。开发者可以自行导入词典，对话技能进行词槽识别时，会优先考虑自定义词典中的词汇。
- **复用系统词槽**：
  复用系统词槽意味着开发者可以更灵活的”组装”自己的自定义词槽，让其直接整合系统内建词槽的识别能力。

**【技能回应】**

- **答复**：
  定义系统直接答复用户的方式，开发者可以选择：
  i.返回一句对话答复。例如技能答复：今天天气还不错。
  ii.执行一条函数来完成这一动作。例如技能抛出函数：flick（tv_channel）;注意，unit提供的API结果中仅会告知开发者应该执行函数flick（tv_channel）,但函数需要在开发者自身的产品中实现与执行。
- **引导**：
  对话技能除了可以直接答复用户，还可以通过配置引导动作，指示用户进一步明确需求。引导包含对话意图引导和问答意图引导，即分别引导至对话意图和问答意图。
  配置对话意图引导需要配置的内容包括主话术，目标对话意图和对话意图引导话术。例如，当前对话意图是：golden_retriever；主话术是：请问您是想了解有关金毛的什么信息？目标对话意图是”golden_retriever_habits”，而对话意图引导话术是”金毛习性么？” 配置问答意图引导需要配置目标问答意图。

## 问答意图

- 一个问答意图代表某一类问答对的主题，它的概念范围要比对话意图大，可以是同一类问题的集合。比如售前咨询FQA，售后咨询FAQ。开发者可以根据业务定义问答意图对应主题的范围。创建完问答意图后，可前往【训练与优化--训练数据--问答集】添加或导入具体的问答对内容。
- 名称：用于描述当前问答意图的主题的大写英文标识，在API调用中技能返回的JSON信息里会有。
- 别名：问答意图名称的中文表述。

## 效果优化

**【训练数据】**

**【对话模板】**

- **对话模板**
  - 在UNIT中，对话模板可以让开发者在冷启动（缺少训练语料）阶段快速让一个任务式对话技能具有很好的对话理解效果。或在表达方式有限、几乎可枚举各种表达方式的控制指令类快速获得很好的对话理解效果。同时对话模板对理解能力的作用会优先于对话样本集，因此也可以用于对错误case的紧急干预。在对话模板让技能的对话理解达到一定的效果而很难进一步提升时，可通过标注更多的对话样本以达到更好的理解效果。
  - 系统中对话模板的数量上限30000。
  - 优先级：系统对对话模板之间的排列顺序是敏感的，排序高的模板会被优先匹配与使用，开发者可通过移动来改变对话模板的排序。
  - 离线编辑：离线导入文件中对话模板之间的顺序即为导入后的优先级。注意：导入时会覆盖线上对话模板集中全部对话模板，请及时做好备份工作！
- **特征词**
  - 特征词是指一组具有相似特征的词，通常被用于约束某条对话模板的匹配范围(例如，天气、下雨、热等类别关键词)或提供一定限度的泛化能力(例如，我想要、我打算、我计划等辅助内容)；活用特征词机制可以事半功倍的提高对话模板的精度和覆盖度。
  - 特征词词典：开发者需自行导入词典，用于系统识别特征词。
- **口语化词**
  - 口语化词将在语义解析时会被自动忽略掉，以此来提高解析的精准率。比如通用场景下：嗯我看一下明天的天气吧 --> 明天天气，这里把"嗯、我、看一下、的、吧"等口语化词都去掉了。又比如特定场景里：预定如家酒店 --> 预订如家，在定酒店的场景里省略了 “酒店”， 可以将“酒店”配置为口语化的词。
  - 系统预置了口语化词，您可以增删改，调整后也可以恢复系统默认的词典值。
- **对话样本集**：是管理对话样本的集合，类似文件夹的作用，可以把不同来源的对话样本放在不同的对话样集中，也可以把不同时间段获取的对话样本分在不同的对话样本集中。
  - **对话样本**：是由开发者提供给技能的训练语料，它给对话技能示范了应该如何将用户的对话理解为对话意图和词槽。开发者可以在对话样本集中通过新建、标注、导入等操作来完成对对话样本的管理与调整。UNIT可以通过”训练”来观察并学习开发者标注的对话样本，并以此来优化技能的对话理解能力（在训练中需要打开对话样本集的开关并选择需要参与训练的样本集）。
- **标注状态**：样本在线标注/编辑后即可生效，“已标注”的样本也可以点击重新标注/编辑进行修改；对话样本集中“已标注”的对话样本才可用于训练模型。
- **标注对话样本**：开发者需要完成对对话样本对应的对话意图和词槽的标注。
- **标注对话意图**：当前样本对应的对话意图，对话意图变更后槽位标记信息会被自动重置。 SYS_OTHERS为系统自动生成的对话意图，用于标记噪音样本(注：噪音样本即为技能的负例样本，它可以帮助技能模型识别哪些对话可能是不属于当应该理解的)。 CMD_UPDATE系列为系统自动生成的对话意图，用于标记多轮对话中，用户的澄清答复(例如用户回答，要中央台，则需要标注这条回答为 CMD_UPDATE_TV_CHANNEL，并且将”中央台”标记为”TV_CHANNEL”类的词槽)

**【问答集】**

- 问答集
  - 问答集是承载问答对的容器，与技能中问答意图的定义一一对应，您可以批量将问答内容导入问答集，也可以在线编辑。
  - 离线编辑 问答对可以离线编辑后导入问答集，导入后会在当前问答集中追加问答内容；也可以将某问答集中问答对内容导出。

## 训练/体验

**【沙盒环境】**

- 沙盒是UNIT平台提供给开发者验证技能模型效果的环境，开发者将模型训练后生效至沙盒环境，即可和技能对话，通过对话可验证沙盒中模型的效果，也可实现数据标注。沙盒环境中只能生效1个模型，新模型生效后，之前生效的模型自动失效。
- 模型版本：沙盒环境当前生效的模型的版本。
- 更新时间：沙盒环境最近一次更新的时间。
- 状态：沙盒环境当前的状态，分为已停用、启用中、模型生效中、运行中、模型生效失败。
  - 已停用：表示沙盒环境处于停止状态，可通过点击“启动”按钮启动沙盒环境。
  - 启用中：表示沙盒环境正处于启动中，待启动完成后，才可进一步加载并生效模型。
  - 模型生效中：表示沙盒环境正在加载生效一个技能模型，生效成功后状态变为"运行中"，生效失败后，沙盒状态为"模型生效失败"。
  - 运行中：表示沙盒环境中有个可以正常对话（平台对话体验窗口或对接接口）的技能模型。而刚创建技能时沙盒也处在运行中，此时沙盒中的模型是一个空的模型，还不能用于对话，需要定义技能，添加训练数据，训练且生效到沙盒后才能用于对话。
  - 模型生效失败：表示沙盒环境模型加载生效失败，此时请通过百度云工单系统、QQ群（805312106）或者UNIT论坛联系客服；其中，提交百度云工单系统时，工单类型请选择“人工智能-理解与交互技术UNIT”。

**【模型】**

- 模型是开发者配置技能、添加/标注对话样本、创建对话模板、导入问答集后通过UNIT平台内置的各种学习引擎训练出的技能核心文件，模型需生效至沙盒环境后才可发挥作用。
- 模型训练时，开发者需选择训练数据，包括技能配置、高级设置、问答集、对话样本、对话模板。
  - **对话样本**：开发者提供的对话样本数据，以对话样本集为单位存储并选择，训练时可选择使用沙盒中的模型（不会重新学习，耗时短）或重新选择样本。 - **使用沙盒中的模型**：不会重新学习，耗时短。 - **重新选择样本**：重新学习，约100条样本/分钟，耗时长。选择的样本集中只有已标注的对话样本才会参与训练。
  - **对话模板**：开发者提供的对话模板数据在每次训练时都会默认参与训练。
- 模型在训练过程中分为初始化、训练中、训练完成、训练失败4个状态。
  - 初始化：模型训练过程中第1个阶段。
  - 训练中：模型训练过程中第2个阶段。
  - 训练完成：模型已训练完成，此时并不可以直接使用，需生效至沙盒环境才可以使用。
  - 训练失败：模型训练失败，需开发者手动删除。

## 智能对话

- 系统提供智能对话窗口，便于开发者通过和自己配置的技能对话，完成数据标注与验证。智能对话窗口分为三个区域：信息展示区、对话展示区、对话输入区。
  - 信息展示区：展示生效模型版本与当前对话样本集信息，开发者可在学习模式下快速切换对话样本集。
  - 对话展示区：展示开发者、技能和工程师UNIT之间的对话内容。
  - 对话输入区：用于开发者输入内容，系统也提供一些快捷键便于用户操作。
- 在对话展示区，开发者可以看到自己与技能和工程师UNIT之间的对话。技能可以展示对用户输入对话样本的解析结果，包括动作内容、动作名称、对话意图和词槽。
  - 动作内容：此时技能回复给终端用户的内容，可能是开发者配置的答复或引导，也可能是词槽的澄清话术；技能未能成功识别时，也会直接答复，开发者可以在技能的高级设置中编辑此话术。
  - 动作名称：当前动作的名称。
  - 意图：技能识别到的意图，包括对话意图和问答意图。
  - 词槽：技能识别到的词槽与词汇。
- 在对话输入区，开发者可以直接输入内容，技能会对开发者输入的内容进行解析并答复。
  - 重置对话状态：清空词槽澄清信息，体验模式中不保存对话状态。
  - 技能反馈频率：高频、中频、关闭。当技能对输入的内容理解不置信（对意图、词槽的理解结果不太确定）时，技能将主动询问开发者，开发者澄清确认后，技能将根据澄清的内容主动确定或纠正之前不太确定的理解结果。







# 实用范例

|                       标题                       | 难度 |  发布时间  |                             操作                             |
| :----------------------------------------------: | :--: | :--------: | :----------------------------------------------------------: |
|               酒店语音助手实例教程               | 中级 | 2018-10-11 |     [查看](http://ai.baidu.com/forum/topic/show/892294)      |
|                  儿童故事机示例                  | 中级 | 2018-09-30 |     [查看](http://ai.baidu.com/forum/topic/show/892097)      |
|                火车票场景实例教程                | 中级 | 2018-07-08 |     [查看](https://ai.baidu.com/forum/topic/show/869808)     |
|              3分钟打造查天气机器人               | 初级 | 2018-07-20 |     [查看](http://ai.baidu.com/forum/topic/show/870024)      |
| 深度解析一个月实现开发者大会媲美真人的外呼机器人 | 初级 | 2018-07-13 | [查看](http://ai.baidu.com/support/news?action=detail&id=421) |
|    教你一招实现开发者大会火爆的地图导航rap秀     | 初级 | 2018-07-12 | [查看](http://ai.baidu.com/support/news?action=detail&id=420) |







# UNIT机器人对话API文档

# 简介

Hi，您好，欢迎使用百度理解与交互技术（UNIT）机器人对话API服务。

本文档主要针对API开发者，描述百度理解与交互技术机器人对话接口服务的相关内容。如果您对文档内容有任何疑问，可以通过以下几种方式联系我们：

1. 在百度云控制台内提交工单，工单类型请选择人工智能-理解与交互技术UNIT服务；
2. 进入[UNIT开发者论坛](http://ai.baidu.com/forum/topic/list/167)发帖交流；

# 文档约定

## 参数结构约定

| 示例      | 说明                                             |
| :-------- | :----------------------------------------------- |
| +         | "+"号表示字段层级，首层为第0层级                 |
| aaa[].bbb | aaa是一个list，bbb是list中的一个元素的属性       |
| aaa{}.bbb | aaa是一个kvdict，bbb是某个key下的一个value的属性 |
| enum      | 枚举值                                           |
| optional  | 用于描述应答参数，表明某个参数在应答中不一定存在 |

- 所有参数都使用 UTF8 编码
- 输入、输出中，所有offset、begin、length字段的单位为字符（编码无关）

# 对话接口描述

基于用户输入的文本内容（语音输入最终也转为文本），返回机器人理解与应答的信息。调用本对话API的前提是您已经在 unit.baidu.com 创建了机器人，并添加了至少一个已定义过的技能（新建了对话意图或问答意图、添加了对话模板/对话样本，且最少完成了一次模型的训练）。

# 请求说明

HTTP方法：POST

请求URL：

- 沙盒环境 【不区分机房】<https://aip.baidubce.com/rpc/2.0/unit/service/chat>
- 生产环境 【待开放】

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token,参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值               |
| :----------- | :--------------- |
| Content-Type | application/json |

Body中放置请求参数，参数详情如下：

# 请求参数说明

| 参数                           | 类型                   | 是否必需 | 说明                                                         |
| :----------------------------- | :--------------------- | :------: | :----------------------------------------------------------- |
| version                        | string                 |   必需   | =2.0，当前api版本对应协议版本号为2.0，固定值                 |
| service_id                     | string                 |   可选   | 机器人ID，service_id 与skill_ids不能同时缺失，至少一个有值。 |
| skill_ids                      | list<string>           |   可选   | 技能ID列表。我们允许开发者指定调起哪些技能。这个列表是有序的——排在越前面的技能，优先级越高。技能优先级体现在response的排序上。具体排序规则参见【应答参数说明】 service_id和skill_ids可以组合使用，详见【请求参数详细说明】 |
| log_id                         | string                 |   必需   | 开发者需要在客户端生成的唯一id，用来定位请求，响应中会返回该字段。对话中每轮请求都需要一个log_id |
| session 或 session_id          | string 或 string(json) |   必需   | session保存机器人的历史会话信息，由机器人创建，客户端从上轮应答中取出并直接传递，不需要了解其内容。如果为空，则表示清空session（开发者判断用户意图已经切换且下一轮会话不需要继承上一轮会话中的词槽信息时可以把session置空，从而进行新一轮的会话）。 session字段内容较多，开发者可以通过传送session_id的方式节约传输流量。具体操作方式见【请求参数详细说明】 以下为session内部格式，仅供参考了解 |
| +service_id                    | string                 |   必需   | 机器人ID，标明该session由哪个机器人产生。                    |
| +session_id                    | string                 |   必需   | session本身的ID，客户端可以使用session_id代替session，节约传输流量。 |
| +skill_sessions                | kvdict of strings      |   必需   | 这里存储与当前对话相关的所有技能的session。key为技能ID，value为技能的session（同【UNIT对话API文档】中的bot_session)。 |
| +interactions                  | list<object>           |   必需   | 历史交互序列，即历史 request/response_list 序列，序列的每一个元素称作一次交互（interaction），随交互进行而交替插入，格式与上述不断增长直到发生清空操作。 |
| +interactions[].interaction_id | string                 |   必需   | 第 i 次交互的唯一标识。                                      |
| +interactions[].timestamp      | string                 |   必需   | interaction生成的时间（以interaction_id的生成时间为准）。格式：YYYY-MM-DD HH:MM:SS.fff （24小时制，精确到毫秒） |
| +interactions[].request        | object                 |   必需   | 第 i 次交互的 request，结构参考【请求参数说明】中的request   |
| +interactions[].response_list  | list<object>           |   必需   | 第 i 次交互的 response列表，结构参考【应答参数说明】中的response_list |
| dialog_state                   | object                 |   可选   | 机器人对话状态。                                             |
| +skill_states                  | kvdict of objects      |   可选   | 技能的对话状态key为技能ID，value为技能的对话状态数据。具体数据格式后续发布。 |
| +contexts                      | json object            |   可选   | 希望在多技能对话过程中贯穿的全局性上下文. 这里预留了一个key用于控制各技能的session记忆。详见【请求参数详细说明】 |
| request                        | object                 |   必需   | 本轮请求体。                                                 |
| +user_id                       | string                 |   必需   | 与技能对话的用户id（如果客户端是用户未登录状态情况下对话的，也需要尽量通过其他标识（比如设备id）来唯一区分用户），方便今后在平台的日志分析模块定位分析问题、从用户维度统计分析相关对话情况。详情见【请求参数详细说明】 |
| +query                         | string                 |   必需   | 本轮请求query（用户说的话），详情见【请求参数详细说明】      |
| +query_info                    | object                 |   可选   | 本轮请求query的附加信息                                      |
| ++type                         | string enum            |   可选   | 请求信息类型，取值范围："TEXT","EVENT"。详情见【请求参数详细说明】 |
| ++source                       | string enum            |   可选   | 请求信息来源，可选值："ASR","KEYBOARD"。ASR为语音输入，KEYBOARD为键盘文本输入。针对ASR输入，UNIT平台内置了纠错机制，会尝试解决语音输入中的一些常见错误 |
| ++asr_candidates               | list<object>           |   可选   | 请求信息来源若为ASR，该字段为ASR候选信息。（如果调用百度语音的API会有该信息，UNIT会参考该候选信息做综合判断处理。） |
| ++asr_candidates[].text        | string                 |   可选   | 语音输入候选文本                                             |
| ++asr_candidates[].confidence  | float                  |   可选   | 语音输入候选置信度                                           |
| +client_session                | string(json)           |   可选   | client希望传给UNIT的本地信息，以一组K-V形式保存，具体内容见【请求参数详细说明】 |
| +hyper_params                  | kvdict of object       |   可选   | key为技能id或机器人id（现在只实现技能id），value为控制相关技能/机器人内部行为的的超参数 |
| +hyper_params{}.bernard_level  | int                    |   可选   | 技能自动发现不置信意图/词槽，并据此主动发起澄清确认的频率。取值范围：0(关闭)、1(低频)、2(高频)。取值越高代表技能对不置信意图/词槽的敏感度就越高，默认值=1 |
| +hyper_params{}.slu_level      | int                    |   可选   | slu运行级别，值域1，2，3 默认值=1                            |
| +hyper_params{}.slu_threshold  | double                 |   可选   | slu阈值，值域0.0~1.0，值越高表示召回的阈值越高，避免误召回，默认值=0.5。 |
| +hyper_params{}.slu_tags       | list<string>           |   可选   | 用于限定slu的解析范围，只在打上了指定tag的意图、词槽、问答对的范围内执行slu。目前这个功能暂时仅针对意图解析生效。 |

# 请求参数详细说明

## service_id与skill_ids的组合关系

在一次请求中，service_id与skill_ids两者可以组合使用，不同的组合关系所代表的语义如下表。

| service_id   | skill_ids    | 语义                                                         |
| :----------- | :----------- | :----------------------------------------------------------- |
| 有           | 有           | 调用从属于service_id所对应机器人中的某些技能。要求skill_ids中不可以包含不属于该机器人的技能。 |
| 有           | 无（或为空） | 调用从属于service_id所对应机器人中的所有技能。技能的顺序按照机器人所配置的顺序设定。 |
| 无（或为空） | 有           | 调用开发者名下的一系列技能，这些技能不一定从属于某个机器人。 |

### session与session_id的使用方式

session保存了历史会话信息，随着对话的进行，在客户端和UNIT之间来回传递，并且被不断更新。理论上客户端与UNIT双方都可以对session进行更新与修改，然而，多数开发者并没有修改session的需求，而session数据的体量偏大，又对网络传输的流量与延时产生了额外的压力。

为了解决这一矛盾，UNIT支持基于session_id的会话托管：客户端在首轮对话时传入一个空的session_id字段，在应答阶段，UNIT会把会话信息托管在云端并且为其分配一个session_id。在后续对话中，客户端只需要继续传入这个session_id，就可以基于历史会话信息与机器人进行多轮对话，而无需接触复杂的原始session。

总结起来，开发者可以选择以下两种方式与机器人进行多轮对话：

- **方式１**：客户端在首轮会话时以“session”为key传入一个空串，UNIT会在应答中以“session”为key，返回原始的会话数据（一个json字符串）。如需进行多轮对话，客户端在后续对话中以“session”为key，继续传送前轮返回的会话数据。在这种方式下，开发者可以对会话数据做出自己想要的修改（只要符合UNIT的格式要求）。
- **方式2**：客户端在首轮会话时以“session_id”为key传入一个空串，UNIT会把会话数据托管在云端，在应答中以“session_id”为key，返回一个用于指代云端的会话数据的ID号。如需进行多轮对话，客户端在后续对话中以“session_id”为key，继续传送前轮返回的ID号。在这种方式下，开发者没有办法修改会话数据。

无论以上哪种方式，只要传入一个空的session_id或者session字段，即代表不再继承历史会话信息，后续将开始新的对话。

### dialog_state.contexts

dialog_state.contexts字段以一个开放的json对象存储所有与对话相关的上下文信息。我们当前预留了一个key，用于开发者控制各个技能的session记忆，如下表：

| key                   | type         | 说明                                                         |
| :-------------------- | :----------- | :----------------------------------------------------------- |
| SYS_REMEMBERED_SKILLS | list<string> | 当前UNIT默认同时请求机器下属的所有技能，或者由skill_ids设定的所有技能。因为我们不清楚开发者最终会选择使用哪个（或者哪些）技能的应答用于下轮对话，所以在默认情况下不做任何session记忆（在新一轮请求时清空所有技能的session）。 开发者可以通过在dialog_state.contexts["SYS_REMEMBERED_SKILLS"]中设定一个技能ID列表，避免处于列表中的技能session被清空，以便与这些技能进行多轮对话。 |

## request.user_id

- 开发者为其用户分配的id，用于区别开发者的每个最终用户。
- UNIT保留两种形式的id，开发者在为最终用户分配id时应当避免分配这些形式的id。

| ID形式     | 说明                                                         |
| :--------- | :----------------------------------------------------------- |
| UNIT_WEB_* | 用于标识来自UNIT网站对话窗口的请求。形式为UNIT_WEB_ + 技能ID/机器人ID |
| UNIT_DEV_* | 用于标识来自开发者自己的请求。形式为UNIT_DEV_ + 任意后缀（一个好的选择是使用开发者的百度id作为后缀） |

## request.query_info

> request.query：本轮请求query

- query_info.type为TEXT时，为常规的文本型query

- query_info.type为EVENT时，为一组K-V（json），且其中必须包含一个名为『event_name』的key，其他自便。

- 我们预定义三种event_name及对应的数据格式： "CHOICE","@UNIT"和"RESET"

  - CHOICE：用户点击了一个界面选择项

    | key            | 类型   | 说明                                       |
    | :------------- | :----- | :----------------------------------------- |
    | event_name     | string | =CHOICE                                    |
    | interaction_id | string | 选项来自哪个interaction_id（会由UNIT返回） |
    | index          | string | 点击了第几个选项（从1开始）                |

  - @UNIT：发起对上轮意图/词槽/问答对的干预**[机器人级的干预功能正在开发中，当前暂不支持]**

  - RESET：清空session。

    | key        | 类型   | 说明   |
    | :--------- | :----- | :----- |
    | event_name | string | =RESET |

## request.client_session

> request.client_session：client希望传给机器人的本地信息，以一组K-V形式保存，考虑兼容性，这里预定义两个key，如果要使用这个字段，必须包含这两个key（值可以分别设为空串和空list），然后再加入自己的K-V（自定义KV的Value类型，当前只支持int、float和string）

| key               | 类型         | 说明                                                         |
| :---------------- | :----------- | :----------------------------------------------------------- |
| client_results    | string(json) | 预留字段                                                     |
| candidate_options | list<object> | 上一轮是client反问用户时有值，存储client端提供的用户可选答案候选，每个候选对应一个object。 |

> candidate_options详细介绍：

示例格式：

```
{
  "attributes": {
    "attr_name_1": "attr_value_1",
    "attr_name_2": "attr_value_2"
  },
  "slot_updates": {
    "slot_name_1": "slot_value_1",
    "slot_name_2": "slot_value_2"
  },
  "remember": true
}
```

对于每个候选项，attributes、slot_updates、remember三个key，分别说明如下：

| key          | 类型   | 说明                                                         |
| :----------- | :----- | :----------------------------------------------------------- |
| attributes   | object | 一组kv，记录该候选的若干属性及属性值，attr_name可自行设定,如： `{"movie_name":"花样年华", "year":"1998", "actor":"梁朝伟"}` |
| slot_updates | object | 用户提供的候选项所对应的词槽，即选择成功以后会加入到解析结果中的词槽信息,如：`{"user_movie": "花样年华1998"}` |
| remember     | bool   | optional,是否对当前结果进行记录，解析相同query时，默认选择同一结果 |

# 响应参数说明

| 参数                                                         | 类型                  | 说明                                                         |
| :----------------------------------------------------------- | :-------------------- | :----------------------------------------------------------- |
| error_code                                                   | int                   | 错误码，为0时表示成功                                        |
| error_msg                                                    | string                | 错误信息，errno!= 0 时存在                                   |
| result                                                       | object                | 返回数据对象，当errno为0时有效                               |
| +version                                                     | string                | =2.0，当前api版本对应协议版本号为2.0，固定值                 |
| +service_id                                                  | string                | 机器人ID，同请求参数                                         |
| +log_id                                                      | string                | 日志唯一ID（用户与机器人的一问一答为一次interaction，其中用户每说一次对应有一个log_id） |
| +session 或 session_id                                       | string 或string(json) | 本轮对话后更新的session信息，具体为哪个字段，与请求数据有关，详情见【请求参数详细说明】 |
| +dialog_state                                                | object                | 对话状态数据，外提以方便session托管。                        |
| +interaction_id                                              | string                | 为本轮请求+应答之组合，生成的id                              |
| +timestamp                                                   | string                | interaction生成的时间（以interaction_id的生成时间为准）。格式：YYYY-MM-DD HH:MM:SS.fff （24小时制，精确到毫秒） |
| +response_list                                               | list<object>          | 本轮应答列表。由于请求接口支持请求多个技能，因此这里可能有多个应答。应答列表是有序的，其第一个元素是最为推荐采用的一个应答。决定应答列表顺序的规则详见【响应参数详细说明】 |
| +response_list[].status                                      | int                   | 状态码，0为正常                                              |
| +response_list[].msg                                         | string                | 错误信息，非零时有效                                         |
| +response_list[].origin                                      | string                | 应答来自哪个技能（skill_id）或机器人（service_id），注意有些应答可能是机器人给出的（不来自任何一个技能）。 |
| +response_list[].action_list                                 | list<object>          | 动作列表                                                     |
| +response_list[].action_list[].confidence                    | float                 | 动作置信度                                                   |
| +response_list[].action_list[].action_id                     | string                | 动作ID                                                       |
| +response_list[].action_list[].say                           | string                | 应答话术                                                     |
| +response_list[].action_list[].custom_reply                  | string(json)          | 用户自定义应答，如果action_type为event，对应事件定义在此处。详见【响应参数详细说明】 |
| +response_list[].action_list[].type                          | string enum           | 动作类型，具体有以下几种: clarify(澄清) satisfy(满足) guide(引导到对话意图) faqguide(引导到问答意图) understood(理解达成，注：内部使用) failure(理解失败) chat(聊天话术) event(触发事件，在答复型对话回应 中选择了"执行函数"，将返回event类型的action) |
| +response_list[].action_list[].refine_detail                 | object optional       | 澄清与引导(type=clarify/guide/faqguide)时有效，表达澄清或引导的详细信息。 |
| +response_list[].action_list[].refine_detail.interact        | string enum           | 交互形式。具体有以下几种： select(给出选项供选择) ask(提问) selectandask(给出选项并且追加提问) |
| +response_list[].action_list[].refine_detail.option_list     | list<object>          | 选项列表。                                                   |
| +response_list[].action_list[].refine_detail.option_list[].option | string                | 选项文字                                                     |
| +response_list[].action_list[].refine_detail.option_list[].info | kvdict                | 选项细节信息。详见【响应参数详细说明】                       |
| +response_list[].action_list[].refine_detail.clarify_reason  | string enum optional  | 动作类型为clarify时有值，表明起因                            |
| +response_list[].schema                                      | object                | 解析的schema，解析意图、词槽结果都从这里面获取               |
| +response_list[].schema.confidence                           | double                | schema的总体置信度                                           |
| +response_list[].schema.intent                               | string                | 意图                                                         |
| +response_list[].schema.intent_confidence                    | double                | 意图置信度                                                   |
| +response_list[].schema.slots                                | list<object>          | 词槽列表                                                     |
| +response_list[].schema.slots[].confidence                   | double                | 词槽置信度                                                   |
| +response_list[].schema.slots[].begin                        | int                   | 起始位置                                                     |
| +response_list[].schema.slots[].length                       | int                   | 长度                                                         |
| +response_list[].schema.slots[].original_word                | string                | 词槽值                                                       |
| +response_list[].schema.slots[].normalized_word              | string                | 归一化词槽值                                                 |
| +response_list[].schema.slots[].word_type                    | string                | 词槽值细化类型[保留字段]                                     |
| +response_list[].schema.slots[].name                         | string                | 词槽名称                                                     |
| +response_list[].schema.slots[].session_offset               | int                   | 词槽是在第几轮对话中引入的                                   |
| +response_list[].schema.slots[].merge_method                 | string                | 词槽引入的方式                                               |
| +response_list[].schema.slots[].sub_slots                    | list<object> optional | 子词槽list，内部结构同正常词槽。                             |
| +response_list[].qu_res                                      | object optional       | SLU解析结果，之所以是optional，是因为response不一定是由某个技能产生，因此不一定有SLU结果 |
| +response_list[].qu_res.timestamp                            | int                   | query结果时间戳                                              |
| +response_list[].qu_res.status                               | int                   | query结果状态                                                |
| +response_list[].qu_res.raw_query                            | string                | 原始query                                                    |
| +response_list[].qu_res.candidates                           | list<object>          | 意图候选项                                                   |
| +response_list[].qu_res.candidates[].confidence              | double                | 解析结果整体的（综合意图和词槽）置信度，如果返回结果中无该字段，请重新训练后尝试。 |
| +response_list[].qu_res.candidates[].intent                  | string                | 候选项意图名称                                               |
| +response_list[].qu_res.candidates[].intent_confidence       | double                | 候选项意图置信度                                             |
| +response_list[].qu_res.candidates[].intent_need_clarify     | bool                  | 意图是否需要澄清                                             |
| +response_list[].qu_res.candidates[].slots                   | list<object>          | 词槽列表                                                     |
| +response_list[].qu_res.candidates[].slots[].confidence      | double                | 词槽置信度                                                   |
| +response_list[].qu_res.candidates[].slots[].begin           | int                   | 起始位置，注意：单位为字符                                   |
| +response_list[].qu_res.candidates[].slots[].length          | int                   | 长度，注意：单位为字符                                       |
| +response_list[].qu_res.candidates[].slots[].original_word   | string                | 词槽原始值                                                   |
| +response_list[].qu_res.candidates[].slots[].normalized_word | string(json)          | 词槽归一化值                                                 |
| +response_list[].qu_res.candidates[].slots[].word_type       | string                | 细粒度词槽类型（预留字段）                                   |
| +response_list[].qu_res.candidates[].slots[].name            | string                | 词槽名                                                       |
| +response_list[].qu_res.candidates[].slots[].need_clarify    | bool                  | 词槽是否需要澄清                                             |
| +response_list[].qu_res.candidates[].slots[].father_idx      | int                   | 父词槽index，非子词槽，取值-1                                |
| +response_list[].qu_res.candidates[].from_who                | string                | 来自哪个qu策略（smart-qu对应对话模板，ml-qu对应对话样本学习） |
| +response_list[].qu_res.candidates[].match_info              | string                | query匹配信息                                                |
| +response_list[].qu_res.candidates[].extra_info              | kvdict                | 候选项附加信息                                               |
| +response_list[].qu_res.qu_res_chosen                        | string(json)          | 最终qu结果，内部格式同result.response.qu_res.candidates[]    |
| +response_list[].qu_res.lexical_analysis                     | list<object>          | query的词法分析结果                                          |
| +response_list[].qu_res.lexical_analysis[].term              | string                | 词汇(含命名实体)                                             |
| +response_list[].qu_res.lexical_analysis[].weight            | double                | 重要性权重                                                   |
| +response_list[].qu_res.lexical_analysis[].type              | string                | 词性或专名类别                                               |
| +response_list[].qu_res.lexical_analysis[].etypes            | list<string>          | 命名实体兼属的所有专名类别                                   |
| +response_list[].qu_res.lexical_analysis[].basic_word        | list<string>          | 构成词汇的基本词                                             |
| +response_list[].qu_res.sentiment_analysis                   | object                | query的情感分析结果                                          |
| +response_list[].qu_res.sentiment_analysis.label             | string enum           | 情感标签，取值范围："0"、"1"、"2"，分别代表：负向情感、无情感、正向情感 |
| +response_list[].qu_res.sentiment_analysis.pval              | double                | 置信度，取值范围0-1                                          |

# 响应参数详细说明

### respinse_list的排序规则

排序分为三轮完成。

第一轮：如果请求中传递了skill_ids，按照skill_ids的顺序排列。否则，按照机器人配置的技能优先级排列。

第二轮：从前到后遍历第一轮排序结果：

​	如果某个技能应答的action_list[0].id与其上一轮应答（如有）相同，且action_list[0].type为"clarify"，该技能的应答移到末尾。

 本轮排序的目的是避免低优先级技能对新请求的有效应答被高优先级技能的重复澄清覆盖掉。

第三轮：从前到后遍历第二轮排序结果

 如果某个技能的action_list[0].type为failure，该技能的应答移到末尾。

注：UNIT网站的机器人对话体验窗口中，每次对话展现的是response_list的第一个应答，在新一轮请求中记忆产出该应答的技能session。

### response_list[].action_list[].custom_reply

- 用户自定义应答，如果action_type为event，对应事件定义在此处。
- 这里预定义一个event，对应『执行函数』动作：

| Key        | 类型   | 说明         |
| :--------- | :----- | :----------- |
| event_name | string | =EXEC        |
| func       | string | 具体执行函数 |

### result.response.action_list[].refine_detail.option_list[].info

这个字段是一个kvdict，我们预定义2种格式：

1. result.response.action_list[].type 为 clarify 时

| Key   | 类型            | 说明                 |
| :---- | :-------------- | :------------------- |
| name  | string          | 意图、词槽的英文名   |
| text  | string          | 意图、词槽的中文描述 |
| value | string optional | 词槽值（仅针对词槽） |

1. result.response.action_list[].type 为 guide/faqguide 时

| Key                | 类型   | 说明     |
| :----------------- | :----- | :------- |
| next_expect_intent | String | 下一意图 |

# 请求示例代码

提示一：使用示例代码前，请记得替换其中的示例Token、请求参数等信息。

提示二：部分语言依赖的类或库，请在代码注释中查看下载地址。

bash

PHP

Java

Python

C++

C#

Node

```
#!/bin/bash
curl -i -k 'https://aip.baidubce.com/rpc/2.0/unit/service/chat?access_token={access_token}' --data '{"log_id":"UNITTEST_10000","version":"2.0","service_id":"S10000","session_id":"","request":{"query":"你好","user_id":"88888"},"dialog_state":{"contexts":{"SYS_REMEMBERED_SKILLS":["1057"]}}}'
```

# 错误信息











# UNIT对话API文档

# 简介

Hi，您好，欢迎使用百度理解与交互技术（UNIT）对话API服务。

本文档主要针对API开发者，描述百度理解与交互技术对话接口服务的相关内容。如果您对文档内容有任何疑问，可以通过以下几种方式联系我们：

1. 在百度云控制台内提交工单，工单类型请选择人工智能-理解与交互技术UNIT服务；
2. 进入[UNIT开发者论坛](http://ai.baidu.com/forum/topic/list/167)发帖交流；
3. 当前文档仅用于对话API，管理API请移步[相关下载](http://ai.baidu.com/docs#/UNIT-v2-download/top)查阅《UNIT技能管理API说明文档》；

# 文档约定

## 参数结构约定

| 示例      | 说明                                             |
| :-------- | :----------------------------------------------- |
| +         | "+"号表示字段层级，首层为第0层级                 |
| aaa[].bbb | aaa是一个list，bbb是list中的一个元素的属性       |
| enum      | 枚举值                                           |
| optional  | 用于描述应答参数，表明某个参数在应答中不一定存在 |

- 所有参数都使用 UTF8 编码
- 输入、输出中，所有offset、begin、length字段的单位为字符（编码无关）

# 对话接口描述

基于用户输入的文本内容（语音输入最终也转为文本），返回技能理解与应答的信息。调用本对话API的前提是您已经在unit.baidu.com 已经创建了技能，并定义了技能（新建了对话意图或问答意图）、添加了对话模板/对话样本，且最少完成了一次模型的训练。

# 请求说明

HTTP方法：POST

请求URL：

- 沙盒环境
  【不区分机房】<https://aip.baidubce.com/rpc/2.0/unit/bot/chat>
- 生产环境
  【华北机房】<https://unit.bj.baidubce.com/rpc/2.0/unit/bot/chat>
  【华东机房】<https://unit.su.baidubce.com/rpc/2.0/unit/bot/chat>
  【华南机房】<https://unit.gz.baidubce.com/rpc/2.0/unit/bot/chat>

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token,参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值               |
| :----------- | :--------------- |
| Content-Type | application/json |

Body中放置请求参数，参数详情如下：

# 请求参数说明

| 参数                           | 类型         | 是否必需 | 说明                                                         |
| :----------------------------- | :----------- | :------: | :----------------------------------------------------------- |
| version                        | string       |   必需   | 固定值2.0。当前api版本对应协议版本号为2.0                    |
| bot_id                         | string       |   必需   | 技能唯一标识，在『我的技能』的技能列表中的技能ID，详情见【请求参数详细说明】 |
| log_id                         | string       |   必需   | 开发者需要在客户端生成的唯一id，用来定位请求，响应中会返回该字段。对话中每轮请求都需要一个log_id |
| request                        | object       |   必需   | 本轮请求体                                                   |
| +user_id                       | string       |   必需   | 与技能对话的用户id（如果客户端是用户未登录状态情况下对话的，也需要尽量通过其他标识（比如设备id）来唯一区分用户），方便今后在平台的日志分析模块定位分析问题、从用户维度统计分析相关对话情况。详情见【请求参数详细说明】 |
| +query                         | string       |   必需   | 本轮请求query（用户说的话），详情见【请求参数详细说明】      |
| +query_info                    | object       |   必需   | 本轮请求query的附加信息                                      |
| ++type                         | string enum  |   必需   | 请求信息类型，取值范围："TEXT","EVENT"。详情见【请求参数详细说明】。 |
| ++source                       | string enum  |   必需   | 请求信息来源，可选值："ASR","KEYBOARD"。ASR为语音输入，KEYBOARD为键盘文本输入。针对ASR输入，UNIT平台内置了纠错机制，会尝试解决语音输入中的一些常见错误 |
| ++asr_candidates               | list<object> |   可选   | 请求信息来源若为ASR，该字段为ASR候选信息。（如果调用百度语音的API会有该信息，UNIT会参考该候选信息做综合判断处理。） |
| ++asr_candidates[].text        | string       |   可选   | 语音输入候选文本                                             |
| ++asr_candidates[].confidence  | float        |   可选   | 语音输入候选置信度                                           |
| ++mod_history                  | list<object> |   可选   | 本轮query的修改记录，开发者请求时，不需要带这个字段。该字段在本轮request记入bot_session后自动更新 |
| +bernard_level                 | int          |   必需   | 系统自动发现不置信意图/词槽，并据此主动发起澄清确认的频率。取值范围：0(关闭)、1(低频)、2(高频)。取值越高代表技能对不置信意图/词槽的敏感度就越高，建议值为1 |
| +client_session                | string(json) |   可选   | client希望传给技能的本地信息，以一组K-V形式保存。详情见【请求参数详细说明】 |
| +updates                       | string(json) |   可选   | 干预信息。详情见【请求参数详细说明】                         |
| ++type                         | string       |   可选   | 干预方式，当前只能取『DEFINE』，表明抛弃系统解析结果，转而由updates字段来定义 |
| ++ops                          | list<object> |   可选   | 干预操作集。                                                 |
| ++ops[].op                     | string       |   可选   | 操作方式，当前只能取『DEFINE』，表明定义一个对象             |
| ++ops[].target                 | string       |   可选   | 操作针对的对象，可选值为意图（INTENT）、词槽（SLOT）         |
| ++ops[].value                  | object       |   可选   | 操作对象的值                                                 |
| +hyper_params                  | object       |   可选   | 影响UNIT内部行为的超参数                                     |
| ++slu_threshold                | float        |   可选   | slu阈值，默认值为0.5，值域0.0~1.0，值越高表示召回的阈值越高，避免误召回。 |
| ++slu_level                    | int          |   可选   | slu运行级别，值域1，2，3，默认值=1                           |
| ++slu_tags                     | list<string> |   可选   | 用于限定slu的解析范围，只在打上了指定tag的意图、词槽、问答对的范围内执行slu。目前这个功能暂时仅针对意图解析生效。 |
| bot_session                    | string(json) |   必需   | 技能的session信息，由技能创建，client从上轮应答中取出并直接传递，不需要了解其内容。如果为空，则表示清空session（开发者判断用户意图已经切换且下一轮会话不需要继承上一轮会话中的词槽信息时可以把session置空，从而进行新一轮的会话）。传参时可只传session_id。 以下为bot_session内部格式，仅供参考了解 |
| +session_id                    | string       |   必需   | 会话ID                                                       |
| +bot_views                     | object       |   可选   | 技能视图数据（意图、词槽解析的历史与最新结果），一般是与交互元素相关但无直接对应关系，需要推演得出的数据。这种数据在此处缓存，可节约计算开销 |
| +dialog_state                  | object       |   可选   | 对话状态数据，供本地化对话管理模块（如[DMKit](https://github.com/baidu/unit-dmkit)）使用 |
| +interactions                  | list<object> |   可选   | 历史交互序列，即历史 request/response 序列，序列的每一个元素称作一次交互（interaction），随交互进行而交替插入，格式与上述不断增长直到发生清空操作 |
| +interactions[].interaction_id | string       |   可选   | 第 i 次交互的唯一标识                                        |
| +interactions[].request        | object       |   可选   | 第 i 次交互的 request，结构参考【请求参数说明】中的request   |
| +interactions[].response       | object       |   可选   | 第 i 次交互的 response，结构参考【响应参数说明】中的response |

# 请求参数详细说明

### bot_id

- 登录unit.baidu.com,新建技能后在【我的技能】列表中可看到技能ID，如下图：

![img](https://ai.bdstatic.com/file/554D18914C3248ED9D28845996B44A33)

## request.user_id

- 开发者为其用户分配的id，用于区别开发者的每个最终用户。
- UNIT保留两种形式的id，开发者在为最终用户分配id时应当避免分配这些形式的id。

| ID形式     | 说明                                                         |
| :--------- | :----------------------------------------------------------- |
| UNIT_WEB_* | 用于标识来自UNIT网站对话窗口的请求。形式为UNIT_WEB_ + 技能ID/机器人ID |
| UNIT_DEV_* | 用于标识来自开发者自己的请求。形式为UNIT_DEV_ + 任意后缀（一个好的选择是使用开发者的百度id作为后缀） |

### request.query_info

> request.query：本轮请求query

- query_info.type为TEXT时，为常规的文本型query

- query_info.type为EVENT时，代表query是一个事件，事件为一组K-V（json），且其中必须包含一个名为『event_name』的key，其他自便。

- 我们预定义三种event_name及对应的数据格式： "CHOOSE","@UNIT"和"RESET"

  - CHOOSE：用户点击了一个界面选择项，比如用户选择技能让用户澄清的多个选项中的一个，技能：有两首您想听的歌，你想听哪首--1……，2……；USER：第1首。

    | key            | 类型   | 说明                                       |
    | :------------- | :----- | :----------------------------------------- |
    | event_name     | string | =CHOICE                                    |
    | interaction_id | string | 选项来自哪个interaction_id（会由技能返回） |
    | index          | string | 点击了第几个选项（从1开始）                |

  - @UNIT：发起对上轮意图/词槽/问答对的干预

    | key                                               | 类型   | 说明   |
    | :------------------------------------------------ | :----- | :----- |
    | event_name                                        | string | =@UNIT |
    | （此处不需要其他数据，干预信息从updates字段传入） | -      | -      |

    （通过@UNIT和updates字段配合传入的干预信息会被平台记录下来，可以在`对话日志`→`反馈学习`界面中查看和操作。注意，updates字段传入的干预信息要和当前模型版本的schema一致，否则不保证结果的正确性。）

  - RESET：清空session，即重置会话状态，会清空当前技能已存储的意图与词槽信息。

    | key        | 类型   | 说明   |
    | :--------- | :----- | :----- |
    | event_name | string | =RESET |

### request.updates

> request.updates：干预操作信息

- target为INTENT时，value为一个string，代表要干预的意图。

- target为SLOT时，value为一个object，代表要干预的词槽：

  - op当前仅支持DEFINE，value格式为：

    | key             | 类型   | 说明                                                 |
    | :-------------- | :----- | :--------------------------------------------------- |
    | begin           | int    | 如果是query中不包含的词槽（如GPS位置信息等），取值-1 |
    | length          | int    | 如果是query中不包含的词槽（如GPS位置信息等），取值-1 |
    | original_word   | string | 词槽值                                               |
    | normalized_word | string | 归一化后的词槽值（标准表达）                         |
    | word_type       | string | 预留字段                                             |
    | name            | string | 词槽名                                               |

### request.client_session

> request.client_session：client希望传给技能的本地信息，以一组K-V形式保存，考虑兼容性，这里预定义两个key，如果要使用这个字段，必须包含这两个key（值可以分别设为空串和空list），然后再加入自己的K-V（自定义KV的Value类型，当前只支持int、float和string）

| key               | 类型         | 说明                                                         |
| :---------------- | :----------- | :----------------------------------------------------------- |
| client_results    | string(json) | 预留字段                                                     |
| candidate_options | list<object> | 上一轮是client反问用户时有值，存储client端提供的用户可选答案候选，每个候选对应一个object。 |

> candidate_options详细介绍：

示例格式：

```
{
  "attributes": {
    "attr_name_1": "attr_value_1",
    "attr_name_2": "attr_value_2"
  },
  "slot_updates": {
    "slot_name_1": "slot_value_1",
    "slot_name_2": "slot_value_2"
  },
  "remember": true
}
```

对于每个候选项，attributes、slot_updates、remember三个key，分别说明如下：

| key          | 类型   | 说明                                                         |
| :----------- | :----- | :----------------------------------------------------------- |
| attributes   | object | 一组kv，记录该候选的若干属性及属性值，attr_name可自行设定,如： `{"movie_name":"花样年华", "year":"1998", "actor":"梁朝伟"}` |
| slot_updates | object | 用户提供的候选项所对应的词槽，即选择成功以后会加入到解析结果中的词槽信息,如：`{"user_movie": "花样年华1998"}` |
| remember     | bool   | optional,是否对当前结果进行记录，解析相同query时，默认选择同一结果 |

# 响应参数说明

| 参数                                               | 类型                  | 说明                                                         |
| :------------------------------------------------- | :-------------------- | :----------------------------------------------------------- |
| error_code                                         | int                   | 错误码，为0时表示成功                                        |
| error_msg                                          | string                | 错误信息，errno!= 0 时存在                                   |
| result                                             | object                | 返回数据对象，当errno为0时有效                               |
| +version                                           | string                | =2.0，当前api版本对应协议版本号为2.0，固定值                 |
| +bot_id                                            | string                | 技能ID，同请求参数                                           |
| +log_id                                            | string                | 日志唯一ID（用户与技能的一问一答为一次interaction，其中用户每说一次对应有一个log_id），同输入参数 |
| +bot_session                                       | string(json)          | 本轮对话后更新的session信息，同请求参数                      |
| +interaction_id                                    | string                | 为本轮请求+应答之组合，生成的id                              |
| +response                                          | object                | 本轮应答体                                                   |
| ++status                                           | int                   | 状态码，0为正常                                              |
| ++msg                                              | string                | 错误信息，非零时有效                                         |
| ++action_list                                      | list<object>          | 动作列表                                                     |
| ++action_list[].confidence                         | float                 | 动作置信度                                                   |
| ++action_list[].action_id                          | string                | 动作ID                                                       |
| ++action_list[].say                                | string                | 应答话术                                                     |
| ++action_list[].custom_reply                       | string(json)          | 用户自定义应答，如果action_type为event，对应事件定义在此处   |
| ++action_list[].type                               | string enum           | 动作类型，具体有以下几种: clarify(澄清) satisfy(满足) guide(引导到对话意图) faqguide(引导到问答意图) understood(理解达成，注：内部使用) failure(理解失败) chat(聊天话术) event(触发事件，在答复型对话回应中选择了"执行函数"，将返回event类型的action) |
| ++action_list[].refine_detail                      | object optional       | 澄清与引导(type=clarify/guide/faqguide)时有效，表达澄清或引导的详细信息。 |
| ++action_list[].refine_detail.interact             | string enum           | 交互形式。具体有以下几种： select(给出选项供选择) ask(提问) selectandask(给出选项并且追加提问) |
| ++action_list[].refine_detail.option_list          | list<object>          | 选项列表。                                                   |
| ++action_list[].refine_detail.option_list[].option | string                | 选项文字                                                     |
| ++action_list[].refine_detail.option_list[].info   | kvdict                | 选项细节信息。详见【响应参数详细说明】                       |
| ++action_list[].refine_detail.clarify_reason       | string enum optional  | 动作类型为clarify时有值，表明起因                            |
| ++schema                                           | object                | 解析的schema，解析意图、词槽结果都从这里面获取               |
| +++intent                                          | string                | 意图                                                         |
| +++intent_confidence                               | float                 | 意图置信度                                                   |
| +++domain_confidence                               | float                 | domain分类置信度[已废弃]                                     |
| +++slots                                           | list<object>          | 词槽列表                                                     |
| +++slots[].confidence                              | float                 | 词槽置信度                                                   |
| +++slots[].begin                                   | int                   | 词槽起始                                                     |
| +++slots[].length                                  | int                   | 词槽长度                                                     |
| +++slots[].original_word                           | string                | 词槽值                                                       |
| +++slots[].normalized_word                         | string                | 归一化词槽值                                                 |
| +++slots[].word_type                               | string                | 词槽值细化类型[保留字段]                                     |
| +++slots[].name                                    | string                | 词槽名称                                                     |
| +++slots[].session_offset                          | int                   | 词槽是在第几轮对话中引入的                                   |
| +++slots[].merge_method                            | string                | 引入的方式                                                   |
| +++slots[].sub_slots                               | list<object> optional | 子词槽list，内部结构同正常词槽。                             |
| ++qu_res                                           | object                | SLU解析的结果                                                |
| +++timestamp                                       | int                   | query结果时间戳                                              |
| +++status                                          | int                   | query结果状态                                                |
| +++raw_query                                       | string                | 原始query                                                    |
| +++candidates                                      | list<object>          | 意图候选项                                                   |
| +++candidates[].confidence                         | float                 | 解析结果整体的（综合意图和词槽）置信度，如果返回结果中无该字段，请重新训练后尝试。 |
| +++candidates[].intent                             | string                | 候选项意图名称                                               |
| +++candidates[].intent_confidence                  | float                 | 候选项意图置信度                                             |
| +++candidates[].domain_confidence                  | float                 | 候选项domain分类置信度[已废弃]                               |
| +++candidates[].intent_need_clarify                | bool                  | 意图是否需要澄清                                             |
| +++candidates[].slots                              | list<object>          | 候选项词槽列表                                               |
| +++candidates[].slots[].confidence                 | float                 | 词槽置信度                                                   |
| +++candidates[].slots[].begin                      | int                   | 起始位置，单位为字符                                         |
| +++candidates[].slots[].length                     | int                   | 长度，单位为字符                                             |
| +++candidates[].slots[].original_word              | string                | 词槽原始值                                                   |
| +++candidates[].slots[].normalized_word            | string(json)          | 词槽归一化值                                                 |
| +++candidates[].slots[].word_type                  | string                | 细粒度词槽类型（预留字段）                                   |
| +++candidates[].slots[].name                       | string                | 词槽名                                                       |
| +++candidates[].slots[].need_clarify               | bool                  | 词槽是否需要澄清                                             |
| +++candidates[].slots[].father_idx                 | int                   | 父词槽index，非子词槽，取值-1                                |
| +++candidates[].from_who                           | string                | 来自哪个qu策略（smart-qu对应对话模板，ml-qu对应对话样本学习） |
| +++candidates[].match_info                         | string                | query匹配信息                                                |
| +++candidates[].extra_info                         | kvdict                | 候选项附加信息                                               |
| +++qu_res_chosen                                   | string(json)          | 最终qu结果，内部格式同result.response.qu_res.candidates[]    |
| +++lexical_analysis                                | list<object>          | query的词法分析结果                                          |
| +++lexical_analysis[].term                         | string                | 词汇(含命名实体)                                             |
| +++lexical_analysis[].weight                       | float                 | 重要性权重                                                   |
| +++lexical_analysis[].type                         | string                | 词性或专名类别                                               |
| +++lexical_analysis[].etypes                       | list<string>          | 命名实体兼属的所有专名类别                                   |
| +++lexical_analysis[].basic_word                   | list<string>          | 构成词汇的基本词                                             |
| +++sentiment_analysis                              | object                | query的情感分析结果                                          |
| +++sentiment_analysis.label                        | string enum           | 情感标签，取值范围："0"、"1"、"2"，分别代表：负向情感、无情感、正向情感 |
| +++sentiment_analysis.pval                         | float                 | 置信度，取值范围0-1                                          |

# 响应参数详细说明

### response_list[].action_list[].custom_reply

- 用户自定义应答，如果action_type为event，对应事件定义在此处。
- 这里预定义一个event，对应『执行函数』动作：

| key        | 类型   | 说明         |
| :--------- | :----- | :----------- |
| event_name | string | =EXEC        |
| func       | string | 具体执行函数 |

### result.response.action_list[].refine_detail.option_list[].info

这个字段是一个kvdict，我们预定义2种格式：

1. result.response.action_list[].type 为 clarify 时

| Key   | 类型            | 说明                 |
| :---- | :-------------- | :------------------- |
| name  | string          | 意图、词槽的英文名   |
| text  | string          | 意图、词槽的中文描述 |
| value | string optional | 词槽值（仅针对词槽） |

1. result.response.action_list[].type 为 guide/faqguide 时

| Key                | 类型   | 说明     |
| :----------------- | :----- | :------- |
| next_expect_intent | String | 下一意图 |

# 请求示例代码

提示一：使用示例代码前，请记得替换其中的示例Token、请求参数等信息。

提示二：部分语言依赖的类或库，请在代码注释中查看下载地址。

bash

PHP

Java

Python

C++

C#

Node

```
#!/bin/bash
curl -i -k 'https://aip.baidubce.com/rpc/2.0/unit/bot/chat?access_token={access_token}' --data '{"bot_session":"","log_id":"7758521","request":{"bernard_level":1,"client_session":"{\"client_results\":\"\", \"candidate_options\":[]}","query":"你好","query_info":{"asr_candidates":[],"source":"KEYBOARD","type":"TEXT"},"updates":"","user_id":"88888"},"bot_id":"1057","version":"2.0"}'
```

# 错误信息

| 错误码        | 类型                                    | 说明                                                         |
| :------------ | :-------------------------------------- | :----------------------------------------------------------- |
| 1             | Unknown error                           | 系统繁忙，如果持续出现该错误，请通过QQ群（805312106）或百度Hi群（1617262）联系技术支持团队 |
| 2             | Service temporarily unavailable         | 服务暂不可用，如果持续出现该错误，请通过QQ群（805312106）或百度Hi群（1617262）联系技术支持团队 |
| 3             | Unsupported openapi method              | 调用的API不存在，请检查后重新尝试                            |
| 4             | Open api request limit reached          | 集群超限额                                                   |
| 6             | No permission to access data            | 无权限访问该用户数据                                         |
| 17            | Open api daily request limit reached    | 每天请求量超限额，如需更多配额请通过工单系统提交申请         |
| 18            | Open api qps request limit reached      | QPS超限额，如需更多配额请通过工单系统提交申请                |
| 19            | Open api total request limit reached    | 请求总量超限额，如需更多配额请通过工单系统提交申请           |
| 100           | Invalid parameter                       | 无效的access_token参数，请检查后重新尝试                     |
| 110           | Access token invalid or no longer valid | access_token无效                                             |
| 111           | Access token expired                    | access_token过期                                             |
| 282000        | Internal error                          | 系统繁忙，如果持续出现该错误，请通过QQ群（805312106）或百度Hi群（1617262）联系技术支持团队 |
| 282001        | Strategy process failed                 | 系统内部错误，如果持续出现该错误，请通过QQ群（805312106）或百度Hi群（1617262）联系技术支持团队 |
| 282004        | Parameter[%s] invalid or missing        | 请求参数格式不正确                                           |
| 282008        | The request content type is illegal.    | 非法请求内容类型                                             |
| 282906        | The account cannot use the service      | 账户请求服务受限，如果持续出现该错误，请通过QQ群（805312106）或百度Hi群（1617262）联系技术支持团队 |
| 292001~292015 | -                                       | 请求参数错误，详见errorMsg字段描述                           |
| 299001~299999 | -                                       | 系统繁忙，如果持续出现该错误，请通过QQ群（805312106）或百度Hi群（1617262）联系技术支持团队 |









# UNIT资源API说明

*注：文档中提到的接入微信相关内容均基于指通过扫描微信开放平台二维码方式所实现的和微信的对接，开发者也可自行搭建server实现和微信对接，详见相关下载中的《UNIT微信接入文档》*

## 整体结构

![img](http://agroup-bos.su.bcebos.com/f76578fd7735dd291bd46e6eb1e81b2025bcfd71)

### 说明

1. UNIT开发者完成BOT配置及训练后，可以将BOT模型运行后的对话效果直接发布至DuerOS技能开放平台或微信公众平台中供业务中使用。
2. 在调用逻辑中，UNIT第三方平台接入层首先会调用BOT模型，然后根据BOT回复的动作类型判断是否调用开发者提供的资源API：
   - 当模型回复的动作类型为文本内容的答复或澄清时，UNIT第三方平台接入层会将内容直接返回DuerOS技能开放平台或微信公众平台等第三方平台，此时第三方平台会直接拿到文本内容答复或澄清时的回复结果；
   - 当模型回复的动作类型为执行函数的答复时，UNIT第三方平台接入层会携带BOT回复的结果调用资源API以获取要呈献第三方平台的内容；最终UNIT第三方平台接入层将资源API的返回的结果发给DuerOS技能开放平台或微信公众平台等第三方平台，此时第三方平台会拿到资源API返回的结果；
3. 若已将回复内容配置在UNIT的BOT内，则无需配置资源API。
4. 部分平台支持二次开发，开发者根据需要配置更加灵活的对话逻辑，也可获取资源、处理自定义指令等。如有需要请移步相关平台：
   - DuerOS技能开放平台： [查看文档](https://developer.dueros.baidu.com/didp/doc/index)；
   - 微信公众号：受微信开放平台的限制，暂不支持对话效果二次开发；
5. 暂时无法支持引导（含引导至对话意图和引导至问答意图）动作类型。

## 资源API协议

### 请求说明

**请求示例**

- HTTP 方法：`POST`
- 请求URL： 需要开发者提供可访问地址，推荐使用HTTPS协议。
- body中放置请求参数，具体内容参见《UNIT2.0对话接口文档》中返回参数说明部分。

说明：

1. from参数代表展示平台，其中
   - 1 为DuerOS技能开放平台；
   - 2 为微信公众平台。
2. bot_id 透传的bot_id

### 返回说明

**通用返回参数**

| 字段       | 是否必选 | 类型   | 说明                |
| :--------- | :------- | :----- | :------------------ |
| error_code | 是       | int    | 0为正常，其他为错误 |
| error_msg  | 否       | string | 错误描述            |
| result     | 否       | object | 对象                |

**result说明**

| 字段    | 是否必选 | 类型   | 说明                                  |
| :------ | :------- | :----- | :------------------------------------ |
| type    | 是       | string | 现在仅支持纯文本返回text，其他预留    |
| content | 是       | string | 当返回是text类型时，content为返回话术 |

### 具体示例

#### 实例说明

UNIT下配置好星座运势查询的场景，当意图满足时触发指令search_luck_data，此时会携带时间和星座词槽调用资源API。

#### 交互过程

1. 用户问：明天处女座运势怎么样?
2. BOT（通过第三方平台）回复：处女座明天会运势很旺哦～

#### 请求内容示例

```
{
  "from": 2,
  "log_id": "7758521",
  "response": {
    "schema": {
      "intent_confidence": 0,
      "slots": [],
      "domain_confidence": 0,
      "intent": ""
    },
    "qu_res": {
      "candidates": [],
      "qu_res_chosen": "",
      "sentiment_analysis": {
        "pval": 0,
        "label": "1"
      },
      "lexical_analysis": [
        {
          "basic_word": [
            "明天"
          ],
          "weight": 0,
          "term": "明天",
          "type": "32"
        },
        {
          "basic_word": [
            "处女",
            "座"
          ],
          "weight": 0,
          "term": "处女座",
          "type": "21"
        },
        {
          "basic_word": [
            "运势"
          ],
          "weight": 0,
          "term": "运势",
          "type": "21"
        },
        {
          "basic_word": [
            "怎么",
            "样"
          ],
          "weight": 0,
          "term": "怎么样",
          "type": "30"
        },
        {
          "basic_word": [
            "?"
          ],
          "weight": 0,
          "term": "?",
          "type": "37"
        }
      ],
      "raw_query": "明天处女座运势怎么样?",
      "status": 0,
      "timestamp": 0
    },
    "action_list": [
      {
        "action_id": "fail_action",
        "refine_detail": {
          "option_list": [],
          "interact": "",
          "clarify_reason": ""
        },
        "confidence": 0,
        "custom_reply": "search_luck_data",
        "say": "我不知道应该怎么答复您。",
        "type": "failure"
      }
    ]
  },
  "bot_session": "",
  "interaction_id": "interaction-1525336753134-677023095-8013-42",
  "version": "2.0",
  "bot_id": "1706"
}
```

#### 返回内容示例

```
{  
    "error_code": 0,
    "error_msg": "",
    "result": {
	    "type": "text",
	    "content": "处女座明天会运势很旺哦～"
    }
}
```





# 常见问题



# 产品使用

**Q：什么是词槽，什么是词槽的澄清？**
A：词槽是技能答复意图前用户需要提供的条件，词槽的澄清是指用户所说内容中不带条件时技能需要先和用户确认相关信息；若规则中的设置某词槽已澄清，则此条件为用户务必提供的信息。

**Q：答复动作中文本内容和执行函数的区别？**
A：文本内容是技能回复给用户一句话，通常用于给用户固定回复；执行函数为一串开发者自定义的识别码，开发者可和自己产品对接后，执行具体操作，例如：关闭电视机。

**Q："引导至对话意图"的使用场景是什么？**
A：引导至对话意图可以进一步确认、细化用户意图，提高用户体验，例如：用户搜索金毛，开发者配置“金毛的习性”、“如何训练金毛”，用户可以直接通过点击得到想要了解的信息。

**Q：对话样本和对话模板的区别是什么？**
A：对话样本是用户对话过程中表达需求（说的话或输入的内容）的实例文本，开发者需标注意图和词槽；对话模板为某种句式的模板，开发者编辑模板时需要用词槽、特征词等信息拼接成句，用来匹配用户对话过程中的各种表达方式。

**Q：词槽和特征词的区别是什么？**
A：词槽是需要用户提供的条件，例如：地点、时间；特征词是具有某特征的一类词，例如：我想要，我打算。

**Q：技能模型中的对话反问频率是什么？**
A：在平台的体验对话窗口，您可以设置对话反问频率来自动在对话过程中让技能对它不太确定的理解结果进行反问，从而让您进行澄清确定，以此来提高技能的对话意图理解正确率。

**Q：沙盒环境状态启动失败怎么办？**
A：沙盒环境启动失败时，请通过百度云工单系统、QQ群（805312106）或者UNIT论坛联系客服；其中，提交百度云工单系统时，工单类型请选择“人工智能-理解与交互技术UNIT”。

**Q：我的产品如何调用技能？**
A：配置好技能后，开发者可以在【发布上线--沙盒/生产环境】的对话接口中查看API调用地址，并前往百度云控制台中创建应用，获取API Key / Secret Key，同时按照技术文档配置相关参数即可。

**Q：技能是什么？**
A：技能是开发者配置的完成特定场景下对话理解与交互的机器人，训练模型并生效后即可使用。

**Q：如何优化技能？**
A：配置完技能（对话意图、问答意图、技能回应及触发规则），在训练数据中添加对训练数据（对话模板、对话样本、问答集），然后在训练与体验模块 训练完技能后，就可以进入到技能优化阶段了，[点击查看教程](http://developer.baidu.com/forum/topic/show?topicId=241520)

**Q：引导类答复的场景是什么？如何配置引导类型的技能？**
A：。引导类答复会主动给出一些候选项，让用户参考这些候选项提问，其实就是不断地让用户去做选择题，最后根据这些选择题给出一些信息或答案，这样可以让对话结果、流程更可控；[点击查看教程](http://developer.baidu.com/forum/topic/show?topicId=241582)

**Q：对话样本不够时怎么办？**
A：我们考虑到了大家对话样本不足，也便于大家更好的优化技能，UNIT将推出根据您已有对话样本推荐更多对话样本的功能。该功能在2.0平台尚出于开发测试阶段，敬请期待。

**Q：有没有系统词槽说明？**
A：系统词槽说明请移步论坛教学贴；[点此查看说明](http://developer.baidu.com/forum/topic/show?topicId=243997)

**Q：UNIT各单位配额上限是多少？**
A：UNIT各单位配额上限[点此查看](http://ai.baidu.com/forum/topic/show/496979)

**Q：UNIT怎么收费？**
A：免费提供3套生产环境。这3套生产环境，可以用于3个技能，也可以用于1或2个技能的多地域部署。每套生产环境700/月。每个生产环境默认可以提供40QPS，可以把多个生产环境叠加使用以获取更多QPS（例如：2个生产环境叠加，QPS可达到80），也可以通过付费提高QPS（35元/QPS·月）。每个账号默认可创建20个技能，如需更多技能请通过[工单系统](http://ticket.bce.baidu.com/?/#/ticket/create)与我们联系，工单类型选择“人工智能-理解与交互技术UNIT”

# 账号相关

**Q：需要使用什么账号登录?**
A：您需要拥有一个百度账号，用来登录百度云，可以点击此处[注册百度账户](https://passport.baidu.com/v2/?reg)。如您以前拥有百度推广账户，同样可以登录百度云。

**Q：注册百度账户时收不到验证码怎么办？**
A：由于欠费停机、存储信息已满、信号网络延迟等原因没有及时收到验证码，这时请检查您的手机及话费余额，保证手机可正常接收短信后，请尝试重新获取验证码。

**Q：AI服务支持推广账号使用么？**
A：支持推广账户使用。

**Q：为什么登录到百度云还要填写手机号、邮箱等信息？**
A：如果您是初次使用百度云，我们需要收集一些您的几个核心信息，用于做开发者认证，这些信息也会作为您使用产品过程中，我们与您取得联系的重要联系方式。如您之前已经是百度云用户、百度开发者中心用户，此步骤将会自动省略。

**Q：我以前是百度开发者中心用户，还需要进行开发者认证么？**
A：不需要。我们会自动同步您的开发者信息，但是为保证后续使用中可以及时联系到您，可能会提示您重新补充最新的开发者信息（手机号等）。

**Q：接口调用每日限额和QPS并发量不够用怎么办？**
A：如需更多请求量，可以与我们联系，我们会尽量协助免费上调（注：在UNIT网页端试用不占用该请求量。）

**Q：有问题如何和工作人员取得联系？**
A：我们为您提供3种联系方式，便于您随时随地的联系我们：①[UNIT社区](http://ai.baidu.com/forum/topic/list/167)；②客服QQ群：805312106 ；③[工单系统](http://ticket.bce.baidu.com/?/#/ticket/create)，工单类型选择“人工智能-理解与交互技术UNIT”。



