# UML (Unified Modeling Language) - 無名黑洞 - CSDN博客
2012年03月24日 00:14:03[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：1133
UML語言的目標是把系統的構造可視化。分為兩個部分：Structural，系統的靜態特點；Behavioral：系統的動態特點。
Structural具體組成如下：
Class: 提供分類和類的接口。
Object: 類的對象。
Component: 一些組成物理實現的邏輯元素。
Deployment: 節點和它們的關係。
Package: 把一些UML元素集合起來就是Package。``
Behavioral具體組成如下：
Use Case：系統的基本Behavior。
Interaction Diagram: 分為Sequence（用於表現時間序列）和Collaboration（用於表現不同Object之間的交互）。
Statechart：用於事件處理和不同狀態的系統。
Activity：用於控制流。
工程設計的一般流程如下：
1、 Use Case Diagram建立需求
2、 Class Diagram標識主要對象和其關係
3、 使用Sequence或Collaboration建立Scenarios。
4、 使用State或Activity來描述Behavior
5、 在Component和Deployment加入細節
下面分別分析每一個概念。
**UseCase**
Use Case相當於一個函數。比如如下就是一個Use Case:
Format a document
Reverse Car
Use Case的目標是實現User Goal， 注意不是User Interaction（user實現goal的做法），如下：
Change a style
Use Case是一個短小的描述，實現了系統的一個特徵，一個系統通常包含很多Use Case。
介紹一個具體例子，這裡使用了開源軟件ArgoUML。
![](http://hi.csdn.net/attachment/201203/23/0_1332518887KJFk.gif)
以上圖標說明了超市裡收銀員和顧客之間的交互關係。外面的方框是邊界。這種模型也叫POST模型(Point ofSale Terminal)。其中，橢圓型的物件就是UseCase。
Use Case之間也會有交互，用箭頭和雙引號表示，如下：
![](http://hi.csdn.net/attachment/201203/23/0_1332518920vl77.gif)
Include表示某Use case包含指向的Use case。Extend表示某Use case擴展了指向的Use case。這兩種交互的作用是使整個圖表更容易理解。
現在，仔細分析一個UseCase, 比如“購買”，
Actors: 顧客，收銀員
Type：Primary and Essential
Description: 顧客到收銀台就所購買的物品付錢，收銀員收錢，之後顧客拿著商品離開。
Cross Ref.: requirement XX, YY, ZZ
Use-cases: 收銀員必須已經完成“登錄”
**Class**
類（Class）是對象(Object)的抽象名稱。類有兩個屬性：Attribute（屬性）和Operation（方法）。類有幾個特點：Polymorphism（多態）和Inheritance(繼承)。
類的標識(ID)一般不再屬性中列出。
Association，Aggregation和Inheritance是類的三大關係。
類間的關係有Association，畫有小箭頭和文字的連線表示，比如
Issued-by
Works-for
Has-capital
Is-issued
使用接了線的菱形表示高階Association，比如Language, Project和Person。
一個特別的Association是Multiply，使用0..*的小箭頭連線表示，比如一個人持有多張信用卡。
Link Attribute：一種特殊的類，比如工資是員工和公司的Link attributes
Role Name: 與連線標在一起，表明類的職務，比如Boss, employee等。
類間關係的Aggregation使用貼著類的菱形箭頭表示，從A指向B表示A是B的一部分。比如句子指向段落，段落指向文檔。再比如汽車的部件指向汽車。
Inheritance使用白色三角形箭頭表示繼承關係，A指向B表示A是B的子類。比如信用卡類是卡片類的子類。
**DynamicModel**
下面介紹類的Behavior，也叫動態模型。動態模型刻畫了模型轉換狀態（State）的過程。
1、Interactiondiagram：
分為sequencediagrams (invaluable in multitasking不適用於多任務 )和collaboration
 diagrams(stack structure靜態結構)
![](http://hi.csdn.net/attachment/201203/23/0_1332518986W8Bp.gif)
![](http://hi.csdn.net/attachment/201203/23/0_1332519010tYx5.gif)
2、Activitydiagram (狀態轉換，不同對象的不同Use-case)。
不適合對象間Collaboration，也不適合時間相關的模型，只適合分析一些Use-cases和Multi-thread和多物體並行(Parallelism)運行。下面是一個煮咖啡的例子。
![](http://hi.csdn.net/attachment/201203/23/0_1332519033ZsWE.gif)
3、Statetransition diagram（是狀態機State Machines的圖像化表示，同一對象的不同Use-case）
適合Concurrency的情況。在商業中應用廣。不適合多對象的交互（Interaction）
定義幾個概念：事件（Event在某時候發生的事情），條件（Condition持續發生的事情），狀態（State某個對象的一些抽象屬性的集合），Action(Operation，限於某個State時的動作)
每個Statetransition分幾種Scenarios，以下是打電話的兩種Scenarios：
![](http://hi.csdn.net/attachment/201203/23/0_1332519069W41R.gif)
![](http://hi.csdn.net/attachment/201203/23/0_1332519143cA6Q.gif)
狀態圖如下：
![](http://hi.csdn.net/attachment/201203/23/0_13325191821dMI.gif)
Condition的例子如下：
![](http://hi.csdn.net/attachment/201203/23/0_1332519217LpkK.gif)
Concurrency是指在某個State之中有可能出現兩種情況，比如AlarmsDisable這個State的Concurrency圖如下：
![](http://hi.csdn.net/attachment/201203/23/0_133251923776YB.gif)
隱藏一些State叫做Hiding，某些狀態分類叫做Hierarchy，分別如下：
![](http://hi.csdn.net/attachment/201203/23/0_1332519278ngZu.gif)
![](http://hi.csdn.net/attachment/201203/23/0_1332519329eE4K.gif)
**Component和Deployment**
最後分析Component(集結一些類)和Deployment(設一些節點)問題。
Package可以打包很多類和Component。
Component在處理源文件、可執行文件、物理數據庫和適應性模型有很大優勢。Component是由一些類加上一些接口(Interface)組成。
![](http://hi.csdn.net/attachment/201203/23/0_1332519358IYo0.gif)
Deployment把模型分為很多節點，每個節點由諸如類或Component等組成。下面是一個互聯網模型。
![](http://hi.csdn.net/attachment/201203/23/0_1332519422bs14.gif)
**擴展UML**
Stereotype: 用於限定一些Problem Domain，使用雙引號表示。比如《Sensor》至於Embeddedsystem。前面提過的《include》《extend》也是這類。
Tagged Value: 元素的補充信息，用大括號括在元素內部。
![](https://img-my.csdn.net/uploads/201204/30/1335765925_2205.jpg)
Constraint：用大括號括在元素之外，表示一些規則。
![](https://img-my.csdn.net/uploads/201204/30/1335765909_1077.jpg)
Dependency: 需要一些前置模型部件的模型元素。
![](https://img-my.csdn.net/uploads/201204/30/1335765792_6497.jpg)
Metamodeling(元建模): 是一些*概念的集合。
一般来说，模型是真实事物的抽象。而元建模是对模型本身的抽象。就好象程序代码模拟汽车前进，而程序语法又规范和抽象代码一样。严格来说，元建模是一种针对模型框架(Frame)，规则(Rule)，约束(Constraints)的构造(Construct)和分析(analyse)的理论。Cheng的表述为元建模是一种类图标，它就像一个盒子，描述了语法（Syntax），消息(Message)等内容来表现语言的元素(Element)和它们之间的联系（Relation and Coinnection）。
值得提出的是Meta-这个前缀在英语中是表示“抽象”的意思。比如Metadata就是对数据的抽象，或者说，是关于数据的数据。一本书是一个数据集合，这本书的作者、出版日期等数据可以说就是数据的数据。Meta目前没有特别确切的翻译，一般翻译为“元”。
![](https://img-my.csdn.net/uploads/201204/30/1335765803_9257.jpg)
**參考資料**
Betty Cheng, Michigan State University, softwareengineering lectures
