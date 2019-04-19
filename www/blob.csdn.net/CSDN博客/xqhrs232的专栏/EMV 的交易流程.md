# EMV 的交易流程 - xqhrs232的专栏 - CSDN博客
2017年07月01日 18:54:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：306
原文地址::[http://blog.csdn.net/freud_lv/article/details/7557702](http://blog.csdn.net/freud_lv/article/details/7557702)
来源：    [http://www.cppblog.com/MichaelLiu/articles/9540.html](http://www.cppblog.com/MichaelLiu/articles/9540.html)
**EMV交易过程解析之一 Initiate Application**
Michael 2006年7月7日
EMV标准定义了十一个过程，这些过程主要通过交易数据的认证、持卡人身份验证以及风险管理等来实现交易的安全，接下来我们将对这些过程逐一进行剖析，从而明白，为什么EMV交易是安全的。
我将把每个过程喻作一个函数，采用函数的语法进行解析。
初始化应用（Initiate Application）是持卡人进行应用选择后的第一个过程。
/**//*----------------------------------------
 2![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)函数名称：InitiateApplication
 3![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)函数参数：
 4![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    [IN,OUT] TSI(Transaction Status Information)
 5![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    [IN,OUT] TVR(Terminal Verification Results)
 6![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    [IN] PDOL(Processing Options Data Object List)-extracted from the FCI of the ADF 
 7![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    [IN] Data specified in PDOL
 8![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    [OUT] AIP(Application Interchange Profile)
 9![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    [OUT] AFL(Application File Locator)
10![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    [OUT] SW1 SW2(9000-success, 6985-conditions of use not satisfied, others)
11![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)函数处理：
12![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    终端给ICC发送GET PROCESSING OPTIONS命令。
13![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)-----------------------------------------*/
**EMV交易过程解析之二 Read Application Data**
Michael 2006年7月7日
这个过程用来遍历ICC中的文件/记录/TLV对象，并保存在终端上。
1/**//*----------------------------------------
 2![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)函数名称：ReadApplicationData
 3![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)函数参数：
 4![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    [IN] AFL(Application File Locator),AFL结构如下：
 5![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    struct tAFLElement {
 6![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    BYTE nSFI; 
 7![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    BYTE nFirstRecordNo;   //the first (or only)
 record number to be read for that SFI.
 8![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    BYTE nLastRecordNo;   //the last record number
 to be read for that SFI.
 9![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    BYTE nRecordNumToOfflineDataAuthentication;   //the
 number of records involved in offline data
                                                                                        //authentication starting with the record number coded in the second byte.
10![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    }* pAFLList;  
11![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    [OUT] DATA OBJECTS
12![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    [OUT] SW1 SW2(9000-success, others-fail)
13![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)函数处理：
14![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)  终端给ICC发送READ RECORD命令。读取AFL指定的文件里的记录内容并返回数据供终端保存及使用。
15![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)-----------------------------------------*/
**EMV交易过程解析之三 Offline Data Authentication**Michael 2006年7月14日
/**//*----------------------------------------------------------------
 3![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)名称：OfflineDataAuthentication(ODA)
 4![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)参数：
 5![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] AIP(Application Interchange Profile)
 6![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] AFL(Application File Locator)
 7![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] AFL中指定的记录数据
 8![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] SDA Tag List(tag '9F4A')
 9![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [OUT] TVR(Terminal Verification Results)
10![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)       [OUT] TSI(Transaction Status Information)
11![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)处理：
12![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        终端从AIP数据判断执行哪种类型的ODA。
13![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        如果ICC和TERMINAL都支持CDDA/ACG(Combined Dynamic Data Authentication/Application Cryptogram Generation)，则终端需要执行CDDA/ACG。
14![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        如果终端不执行CDDA/ACG，此时，若ICC和TERMINAL都支持OFFLINE DDA，则终端执行OFFLINE DDA。
15![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        如果终端不执行OFFLINE DDA，此时，若ICC和TERMINAL都只持OFFLINE SDA，则终端执行OFFLINE SDA。
16![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        如果以上三种ODA都不执行则设置TVR的“ODA未执行”标志为1。
17![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
18![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)----------------------------------------------------------------*/
**EMV交易过程解析之四 Processing RestrictionsMichael 2006年7月14日**
/**//*----------------------------------------------------------------
 2![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)名称：ProcessingRestrictions
 3![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)参数：
 4![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] TERMINAL和ICC上的AVN(Application Version Number)
 5![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] AUC(Application Usage Control)
 6![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] Issuer Country Code
 7![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] Terminal Country Code
 8![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] Application Effective Date
 9![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] Application Expiration Date
10![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [OUT] TVR
11![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)处理：
12![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        包括：
13![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [应用版本判断] 比较TERMINAL和ICC的AVN，设置TVR相应位。
14![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [ICC地域判断] 根据交易类型，比较Issuer Country Code和Terminal Country 
15![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
16![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)Code，依据比较结果和AUC的相应位匹配与否，设置TVR相应位。
17![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [有效期判断] 判断当前交易日期是否在有效期内，设置TVR相应位。
18![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
19![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)----------------------------------------------------------------*/
**EMV交易过程解析之五 Cardholder Verification**Michael 2006年7月20日
/*----------------------------------------------------------------
 2![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)名称：CardholderVerification
 3![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)参数：
 4![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] AIP(Application Interchange Profile)
 5![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] CVM List(Tag '8E')
 6![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)              包括两个4-BYTE的AMOUNT FIELDs和数个2-BYTE的CVRs(Cardholder Verification Rule)，每个CVR包括两个字节，分别是CVM(Cardholder Verification Method，表示身份验证的可选方法) Codes和CVM Condition Codes(表示进行身份验证的条件)。
 7![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [OUT] TVR(Terminal Verification Result)
 8![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [OUT] TSI(Transaction Status Information)
 9![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)处理：
10![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        终端判断CVR列表的第一个CVM Condition是否满足，如果满足则采用对应的CVM进行验证。如果验证成功则CV过程结束，否则需要检查该CVM Code的第7位，以决定是否需要读取第二个CVR元素继续验证。
11![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        如果第一个CVR的Condition不满足，或者该条件不被终端理解，则需要读取下一个CVR继续验证。
12![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        当其中只要有一个CVR验证成功，则CV成功结束，若所有验证都失败则CV失败。CV结果在TVR相应位中作设置。
13![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        几个典型的CVM方法有：Offline PIN, Online PIN, Signature, Combination CVMs.
14![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)----------------------------------------------------------------*/
**EMV交易过程解析之六 Terminal Risk Management**Michael 2006年7月20日
/**//*----------------------------------------------------------------
 2![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)名称：TerminalRiskManagement
 3![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)参数：
 4![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] AIP(Application Interchange Profile)
 5![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] Terminal LOG
 6![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            用于获得本次交易帐号的最近一次交易金额
 7![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] TFL(Terminal Floor Limit - Tag '9F1B')
 8![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] TPURS(Target Percentage to be Used for Random Selection - 终端数据)
 9![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] TVBRS(Threshold Value for Biased Random Selection - 终端数据)
10![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] MTPUBRS(Maximum Target Percentage to be Used for Biased Random Selection - 终端数据)
11![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] LCOL(Lower Consecutive Offline Limit - tag '9F14')
12![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] UCOL(Upper Consecutive Offline Limit - tag '9F23')
13![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] ATC(Application Transaction Counter - tag '9F36')
14![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] LOAR(Last Online ATC Register - tag '9F13')
15![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [OUT] TVR(Terminal Verification Result)
16![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [OUT] TSI(Transaction Status Information)
17![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)处理：
18![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        包括：
19![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [金额上限检验(Floor Limit checking)] 判断本次交易金额和上次交易金额的总和是否超过终端交易金额上限(Terminal Floor Limit)，设置TVR相应位。
20![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [在线交易判断(Random Transaction Selection)] 对于交易金额小于TVBRS的情况，终端需要产生一个1－99的随机数，如果该随机数小于等于TPURS则该交易需要在线验证。对于金额大于等于TVBRS但小于TFL的交易，终端需要比较产生的随机数和TTP(Transaction Target Percent)，如果随机数小于等于TTP，则交易需要在线验证。其中，TTP=( (MTPUBRS-TPURS) * ((Amount,Authorised-TVBRS)/(TFL-TVBRS)) )+TPURS。若需要在线验证则设置TVR的相应位为1。
21![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [交易周期校验(Velocity Checking)] 发送GET DATA命令读取ATC和Last Online ATC Register，若读取失败则校验结束并置TVR相应位。若读取成功则两值相减的结果同LCOL比较，若结果大于LCOL则该交易需要在线授权。若终端当时无法在线交易则可以继续离线交易的上限为UCOL，当UCOL次数达到时必须进行在线授权，否则交易应当被拒绝。若LOAR为0则要设置TVR的“新卡”位为1。
22![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        TRM执行完后置TVR和TSI相应位。
23![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)-----------------------------------------------------------------*/
**EMV交易过程解析之七 Terminal Action Analysis**
Michael 2006年7月21日
/**//*----------------------------------------------------------------
 2![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)名称：Terminal Action Analysis（终端行为决策）
 3![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)参数：
 4![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] TVR(Terminal Verification Result)
 5![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] IAC(Issuer Action Codes)，和TVR完全一样的数据结构，记录了针对TVR中每位结果应采取的决策包括离线授权，离线拒绝或在线授权。IAC包括:
 6![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)              Issuer Action Code - Denial (Tag '9F0E'), 默认值为全0
 7![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)              Issuer Action Code - Online (Tag '9F0F'), 默认值为全1
 8![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)              Issuer Action Code - Default(Tag '9F0D'), 默认值为全1
 9![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        IAC-Denial指定TVR中哪些结果位为1时需要离线拒绝。
10![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        IAC-Online指定TVR中哪些结果位为1时需要在线授权。
11![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        IAC-Default指定当要求在线授权但是终端没办法在线的情况下TVR中哪些结果位为1时需要离线拒绝。对为0的结果位则离线授权。如果终端可以在线交易则IAC-Default不被执行。
12![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] TAC(Terminal Action Codes)，或者称作AAC(Acquirer Action Codes)，和TVR完全一样的数据结构，包括:
13![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)              Terminal Action Code - Denial, 
14![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)              Terminal Action Code - Online,
15![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)              Terminal Action Code - Default
16![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        TAC和IAC完全一样的意义，在进行行为决策的时候需要综合考虑IAC和TAC，只要其中某一个的相应位要求做出某种行为则需要执行该行为。比如，当TVR中某位为1时IAC要求Denial（离线拒绝），但是TAC没有要求Denial，那么结果是需要离线拒绝。
17![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [OUT] GENERATE AC Command
18![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)处理：
19![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        终端行为决策，就是根据TVR结果查找IAC和TAC表来决定向ICC发送GENERATE AC命令时需要请求AAC, ARQC还是TC，即离线拒绝，在线授权还是离线授权。
20![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        向ICC发送GENERATE AC命令后，ICC本身还要做出行为决策（Card Action Analysis），以最终确定该交易是离线拒绝/在线授权/离线授权。
21![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        行为决策的顺序是从IAC/TAC-Denial, IAC/TAC-Online到IAC/TAC-Default。
22![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)-----------------------------------------------------------------*/
**EMV交易过程解析之八 Card Action Analysis**
Michael 2006年7月21日
/**//*----------------------------------------------------------------
 2![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)名称：Card Action Analysis（卡行为决策）
 3![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)参数：
 4![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] GENERATE AC Command
 5![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] CDOL(Card Risk Management Data Object List)
 6![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] AIP(Application Interchange Profile)
 7![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [OUT] TSI(Transaction Status Information)
 8![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [OUT] GENERATE AC Response
 9![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)处理：
10![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        卡行为决策，即对终端发下来的GENERATE AC命令进行处理并做出响应（离线拒绝，在线授权，在线'冲正'还是离线授权），以对发卡行的风险进行管理。不同于终端行为(Terminal Action Analysis)的查表决策，发卡行如何处理AC命令及做何种响应完全由发卡行自身决定，EMV并无规定。
11![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)-----------------------------------------------------------------*/
**EMV交易过程解析之九 Online ProcessingMichael 2006年7月24日**
/* ----------------------------------------------------------------
 2![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)名称：Online Processing
 3![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)参数：
 4![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] ARQC(Authorization Request Cryptogram)
 5![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] AIP(Application Interchange Profile)
 6![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [OUT] TVR(Terminal Verification Result)
 7![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [OUT] TSI(Transaction Status Information)
 8![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)处理：
 9![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        这是发卡行(ICC)和发卡行自身(主机)之间的一次对话。
10![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        当终端收到ICC的ARQC决策时需要进行在线授权。
11![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        具体地，ARQC（用发卡行的密钥对交易数据进行运算得到的一段代码）被包含在交易请求包中发送给主机，主机进行授权确认后返回交易响应包。
12![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        如果交易响应包中包含IAD(Issuer Authentication Data - tag'91')，终端检查AIP值看ICC是否支持IA(Issuer Authentication)，如果支持则终端给ICC发送EXTERNAL AUTHENTICATE命令（一次交易只能发一次），根据ICC返回的命令执行结果设置TVR相应位。若终端不支持IA或交易响应包没有IAD数据，则不发送任何命令。
13![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        若EXTERNAL AUTHENTICATIE命令有被发送，则设置TSI的相应位为1。
14![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)----------------------------------------------------------------- */
**EMV交易过程解析之十 Issuer-to-Card Script ProcessingMichael 2006年7月24日**
 1 /**//*----------------------------------------------------------------
 2![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)名称：Issuer-to-Card Script Processing
 3![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)参数：
 4![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [IN] Authorization Response Message（包含Issuer Script - Tag'71' or '72'）
 5![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [OUT] TVR(Terminal Verification Result)
 6![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        [OUT] TSI(Transaction Status Information)
 7![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)处理：
 8![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        这是发卡行(ICC)和发卡行自身(主机)之间的又一次对话。
 9![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        主机可以在发给终端的交易响应包中包含一些特殊命令的脚本
10![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)（比如unblocking of an offline PIN命令，对于不同的发卡行命令迥异所以由发卡行直接发送）供ICC执行，终端只要将这些命令转发给ICC即可。
11![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        具体地，终端接收到主机的交易响应包后取出ICC执行脚本(Issuer Script-Tag'71' or '72'， 每个响应包中可能包含多个执行脚本)，终端对每个执行脚本进行解析得到该脚本的ICC命令序列(Tag-'86')，然后逐一按序给ICC发送ICC命令，如果ICC执行该命令失败则该脚本的后续命令不再执行，脚本执行结束；如果该命令执行成功则继续执行后续解析出来的命令，直到所有命令执行成功后该脚本的执行成功完成。
12![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        只要脚本被执行则设置TSI的"Script processing was performed"为1，若执行失败则设置TVR的相应位为1（对Tag'71'和'72'的脚本设置的TVR位不同）。
13![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)-----------------------------------------------------------------*/
**EMV交易过程解析之十一 CompletionMichael 2006年7月24日**
/*----------------------------------------------------------------
2![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)名称：Completion
3![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)参数：
4![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        无。
5![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)处理：
6![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        这是EMV交易的终点站。
7![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        ICC卡返回TC(Transaction Certificate，当响应第一次GENERATE AC命令)或者AAC(Application Authentication Cryptogram，当响应第二次GENERATE AC命令)给终端预示ICC对所有EMV函数的执行结束（即授权或者拒绝交易）。
8![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)-----------------------------------------------------------------*/
