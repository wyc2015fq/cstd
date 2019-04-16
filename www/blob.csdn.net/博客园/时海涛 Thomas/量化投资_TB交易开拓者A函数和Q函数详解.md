# 量化投资_TB交易开拓者A函数和Q函数详解 - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [量化投资_TB交易开拓者A函数和Q函数详解](https://www.cnblogs.com/noah0532/p/8878859.html)





**//////////////////A函数详解///////////////**

*//A函数主要在端口上进行下单操作////////////////*

**A_AccountID**
说明 返回当前公式应用的交易帐户ID。 
语法 String A_AccountID() 
参数 无 
备注 返回当前公式应用的交易帐户ID，返回值为字符串，无效时返回空串。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例:登录交易账号：Commentary("A_AccountID"+A_AccountID);


**A_BrokerID**
说明 返回当前公式应用的交易帐户对应的交易商ID。 
语法 String A_BrokerID() 
参数 无 
备注 返回当前公式应用的交易帐户对应的交易商ID，返回值为字符串，无效时返回空串。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例：登录交易账号：Commentary("A_BrokerID"+A_BrokerID);



**A_BuyAvgPrice**
说明 返回当前公式应用的帐户下当前商品的买入持仓均价。 
语法 Numeric A_BuyAvgPrice() 
参数 无 
备注 返回当前公式应用的帐户下当前商品的买入持仓均价，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例：登录交易账号+添加交易策略：PlotNumeric("A_BuyAvgPrice",A_BuyAvgPrice);

**A_BuyPosition**
说明 返回当前公式应用的帐户下当前商品的买入持仓。 
语法 Numeric A_BuyPosition() 
参数 无 
备注 返回当前公式应用的帐户下当前商品的买入持仓，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 登录交易账号+添加交易策略当前持多仓2手，A_BuyPosition返回2。  

**A_BuyProfitLoss**
说明 返回当前公式应用的帐户下当前商品的买入持仓盈亏。 
语法 Numeric A_BuyProfitLoss() 
参数 无 
备注 返回当前公式应用的帐户下当前商品的买入持仓盈亏，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例：登录交易账号+添加交易策略：PlotNumeric("A_BuyProfitLoss",A_BuyProfitLoss); 

**A_CurrentEquity**
说明 返回当前公式应用的交易帐户的动态权益。 
语法 Numeric A_CurrentEquity() 
参数 无 
备注 返回当前公式应用的交易帐户的动态权益，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 登录交易账号+添加交易策略： 

**A_DeleteOrder**
说明 针对当前公式应用的帐户、商品发送撤单指令。 
语法 Bool A_DeleteOrder(String strContractNo = "") 
参数 strContractNo 所要撤委托单的合同号。 
strContractNo = "" 时撤该帐户，该商品所有未成交委托单。 
备注 针对当前公式应用的帐户、商品发送撤单指令，发送成功返回True,发送失败返回False。 
该函数可针对叠加商品进行处理，可用Data1.A_DeleteOrder(...)进行调用。 
该函数直接发单，不经过任何确认，并会在每次公式计算时发送，一般需要配合着仓位头寸进行条件处理，在不清楚运行机制的情况下，请慎用。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 If (A_GetOpenOrderCount()>0)
{
A_DeleteOrder();
...
}
或者用如下方式撤最后发送的未成交单: 
If(A_GetOpenOrderCount()>0) {
A_DeleteOrder(A_OpenOrderContractNo());
...
} //配合A_GetOpenOrderCount()函数和A_OpenOrderContractNo()函数一起使用;

**A_FreeMargin**
说明 返回当前公式应用的交易帐户的可用资金。 
语法 Numeric A_FreeMargin() 
参数 无 
备注 返回当前公式应用的交易帐户的可用资金，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 登录交易账号+添加交易策略：

**A_GetLastOpenOrderIndex**
说明 返回当前公式应用的帐户下当前商品的最后一个未成交委托单索引，按输入参数为条件。 
语法 Integer A_GetLastOpenOrderIndex(Integer BuyOrSell,Integer EntryOrExit) 
参数 BuyOrSell 发送委托单的买卖类型，取值为Enum_Buy或Enum_Sell之一； 
EntryOrExit 发送委托单的开平仓类型，取值为Enum_Entry,Enum_Exit,Enum_ExitToday之一； 
备注 返回当前公式应用的帐户下当前商品的最后一个未成交委托单索引，按输入参数为条件，返回值为整型。
如果返回值不等于InvalidInteger，即为有效，可通过该索引获取相关的委托单状态，价格，数量等信息。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 LastIndex = A_GetLastOpenOrderIndex(Enum_Buy,Enum_Entry);
If(LastIndex != InvalidInteger)
{
orderPrice = A_OpenOrderPrice(LastIndex);
...
} 

**A_GetLastOrderIndex**
说明 返回当前公式应用的帐户下当前商品的最后一个当日委托单索引，按输入参数为条件。 
语法 Integer A_GetLastOrderIndex(Integer BuyOrSell,Integer EntryOrExit) 
参数 BuyOrSell 发送委托单的买卖类型，取值为Enum_Buy或Enum_Sell之一； 
EntryOrExit 发送委托单的开平仓类型，取值为Enum_Entry,Enum_Exit,Enum_ExitToday之一； 
备注 返回当前公式应用的帐户下当前商品的最后一个当日委托单索引，按输入参数为条件，返回值为整型。
如果返回值不等于InvalidInteger，即为有效，可通过该索引获取相关的委托单状态，价格，数量等信息。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 LastIndex = A_GetLastOrderIndex(Enum_Buy,Enum_Entry);
If(LastIndex != InvalidInteger)
{
orderPrice = A_OrderPrice(LastIndex);
...
} 

**A_GetOpenOrderCount**
说明 返回当前公式应用的帐户下当前商品的未成交委托单数量。 
语法 Integer A_GetOpenOrderCount() 
参数 无 
备注 返回当前公式应用的帐户下当前商品的未成交委托单数量，返回值为整型。
该函数返回委托单数量中只包含未成交的类型：部分成交和已申报。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOpenOrderCount();
For i = 1 To nCount
{
nStatus = A_OpenOrderStatus(i);
...
} 

**A_GetOrderCount**
说明 返回当前公式应用的帐户下当前商品的当日委托单数量。 
语法 Integer A_GetOrderCount() 
参数 无 
备注 返回当前公式应用的帐户下当前商品的当日委托单数量，返回值为整型。
该函数返回委托单数量中包含所有的类型：全部成交，已申报，已撤单，部分成交等。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOrderCount();
For i = 1 To nCount
{
nStatus = A_OrderStatus(i);
...
} 

**A_OpenOrderBuyOrSell**
说明 返回当前公式应用的帐户下当前商品的某个未成交委托单的买卖类型。 
语法 Integer A_OpenOrderBuyOrSell(Integer nIndex = 0) 
参数 nIndex 当日委托单数组的索引值，以1为基值递增。 
nIndex = 0 时取最后提交的委托单数据。 
备注 返回当前公式应用的帐户下当前商品的某个未成交委托单的买卖类型，返回值为整型。
该函数返回值可以与Enum_Buy、Enum_Sell等买卖状态枚举值进行比较，根据类型不同分别处理。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOpenOrderCount();
For i = 1 To nCount
{
nBorS = A_OpenOrderBuyOrSell(i);
If (nBorS == Enum_Buy())
...
} 

**A_OpenOrderContractNo**
说明 返回当前公式应用的帐户下当前商品的某个未成交的委托单的合同号。(本函数范围是所有未成交的委托单的合同号,有区别于A_OrderContractNo) 
语法 String A_OpenOrderContractNo(Integer nIndex = 0) 
参数 nIndex 当日委托单数组的索引值，以1为基值递增。 
nIndex = 0 时取最后提交的委托单数据。  
备注 返回当前公式应用的帐户下当前商品的某个委托单的合同号，返回值为字符串。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOpenOrderCount();
For i = 1 To nCount
{
strContractNo = A_OpenOrderContractNo(i);
...
} 

**A_OpenOrderEntryOrExit**
说明 返回当前公式应用的帐户下当前商品的某个未成交委托单的开平仓状态。 
语法 Integer A_OpenOrderEntryOrExit(Integer nIndex = 0) 
参数 nIndex 当日委托单数组的索引值，以1为基值递增。 
nIndex = 0 时取最后提交的委托单数据。 
备注 返回当前公式应用的帐户下当前商品的某个未成交委托单的开平仓状态，返回值为整型。
该函数返回值可以与Enum_Entry、Enum_Exit等开平仓状态枚举值进行比较，根据类型不同分别处理。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOpenOrderCount();
For i = 1 To nCount
{
nEntryFlag = A_OpenOrderEntryOrExit(i);
If(nEntryFlag == Enum_ExitToday())
...
} 

**A_OpenOrderFilledLot**
说明 返回当前公式应用的帐户下当前商品的某个未成交委托单的成交数量。 
语法 Numeric A_OpenOrderFilledLot(Integer nIndex = 0) 
参数 nIndex 当日委托单数组的索引值，以1为基值递增。 
nIndex = 0 时取最后提交的委托单数据。 
备注 返回当前公式应用的帐户下当前商品的某个未成交委托单的成交数量，返回值为浮点数。
只有当前委托单为部分成交时，该函数才会返回有效值。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOpenOrderCount();
For i = 1 To nCount
{
OpenOrderFilledLot = A_OpenOrderFilledLot(i);
...
} 

**A_OpenOrderFilledPrice**
说明 返回当前公式应用的帐户下当前商品的某个未成交委托单的成交价格。 
语法 Numeric A_OpenOrderFilledPrice(Integer nIndex = 0) 
参数 nIndex 当日委托单数组的索引值，以1为基值递增。 
nIndex = 0 时取最后提交的委托单数据。 
备注 返回当前公式应用的帐户下当前商品的某个未成交委托单的成交价格，返回值为浮点数。
只有当前委托单为部分成交时，该函数才会返回有效值。该成交价格可能为多个成交价格的平均值。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOpenOrderCount();
For i = 1 To nCount
{
OpenOrderFilledPrice = A_OpenOrderFilledPrice(i);
...
} 

**A_OpenOrderLot**
说明 返回当前公式应用的帐户下当前商品的某个未成交委托单的委托数量。 
语法 Numeric A_OpenOrderLot(Integer nIndex = 0) 
参数 nIndex 当日委托单数组的索引值，以1为基值递增。 
nIndex = 0 时取最后提交的委托单数据。 
备注 返回当前公式应用的帐户下当前商品的某个未成交委托单的委托数量，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOpenOrderCount();
For i = 1 To nCount
{
OpenOrderLot = A_OpenOrderLot(i);
...
} 

**A_OpenOrderPrice**
说明 返回当前公式应用的帐户下当前商品的某个未成交委托单的委托价格。 
语法 Numeric A_OpenOrderPrice(Integer nIndex = 0) 
参数 nIndex 当日委托单数组的索引值，以1为基值递增。 
nIndex = 0 时取最后提交的委托单数据。 
备注 返回当前公式应用的帐户下当前商品的某个未成交委托单的委托价格，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOpenOrderCount();
For i = 1 To nCount
{
OpenOrderPrice = A_OpenOrderPrice(i);
...
} 

**A_OpenOrderStatus**
说明 返回当前公式应用的帐户下当前商品的某个未成交委托单的状态。 
语法 Integer A_GetOpenOrderStatus(Integer nIndex = 0) 
参数 nIndex 当日委托单数组的索引值，以1为基值递增。 
nIndex = 0 时取最后提交的委托单数据。  
备注 返回当前公式应用的帐户下当前商品的某个未成交委托单的状态，返回值为整型。
该函数返回值可以与Enum_Declared、Enum_FillPart委托状态枚举值进行比较，根据类型不同分别处理。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOpenOrderCount();
For i = 1 To nCount
{
nStatus = A_OpenOrderStatus(i);
If(nStatus == Enum_FillPart)
...
} 

**A_OpenOrderTime**
说明 返回当前公式应用的帐户下当前商品的某个未成交委托单的委托时间。 
语法 Numeric A_OpenOrderTime(Integer nIndex = 0) 
参数 nIndex 当日委托单数组的索引值，以1为基值递增。 
nIndex = 0 时取最后提交的委托单数据。 
备注 返回当前公式应用的帐户下当前商品的某个未成交委托单的委托时间，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOpenOrderCount();
For i = 1 To nCount
{
OpenOrderTime = A_OpenOrderTime(i);
...
} 

**A_OrderBuyOrSell**
说明 返回当前公式应用的帐户下当前商品的某个委托单的买卖类型。 
语法 Integer A_OrderBuyOrSell(Integer nIndex = 0) 
参数 nIndex 当日委托单数组的索引值，以1为基值递增。 
nIndex = 0 时取最后提交的委托单数据。 
备注 返回当前公式应用的帐户下当前商品的某个委托单的买卖类型，返回值为整型。
该函数返回值可以与Enum_Buy、Enum_Sell等买卖状态枚举值进行比较，根据类型不同分别处理。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOrderCount();
For i = 1 To nCount
{
nBorS = A_OrderBuyOrSell(i);
If (nBorS == Enum_Buy())
...
} 

**A_OrderCanceledLot**
说明 返回当前公式应用的帐户下当前商品的某个委托单的撤单数量。 
语法 Numeric A_OrderCanceledLot(Integer nIndex = 0) 
参数 nIndex 当日委托单数组的索引值，以1为基值递增。 
nIndex = 0 时取最后提交的委托单数据。 
备注 返回当前公式应用的帐户下当前商品的某个委托单的撤单数量，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOrderCount();
For i = 1 To nCount
{
OrderCanceledLot = A_OrderCanceledLot(i);
...
} 

**A_OrderContractNo**
说明 返回当前公式应用的帐户下当前商品的某个委托单的合同号。(本函数范围是所有的委托单的合同号,有区别于A_OpenOrderContractNo) 
语法 String A_OrderContractNo(Integer nIndex = 0) 
参数 nIndex 当日委托单数组的索引值，以1为基值递增。 
nIndex = 0 时取最后提交的委托单数据。  
备注 返回当前公式应用的帐户下当前商品的某个委托单的合同号，返回值为字符串。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOrderCount();
For i = 1 To nCount
{
strContractNo = A_OrderContractNo(i);
...
} 

**A_OrderEntryOrExit**
说明 返回当前公式应用的帐户下当前商品的某个委托单的开平仓状态。 
语法 Integer A_OrderEntryOrExit(Integer nIndex = 0) 
参数 nIndex 当日委托单数组的索引值，以1为基值递增。 
nIndex = 0 时取最后提交的委托单数据。 
备注 返回当前公式应用的帐户下当前商品的某个委托单的开平仓状态，返回值为整型。
该函数返回值可以与Enum_Entry、Enum_Exit等开平仓状态枚举值进行比较，根据类型不同分别处理。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOrderCount();
For i = 1 To nCount
{
nEntryFlag = A_OrderEntryOrExit(i);
If(nEntryFlag == Enum_ExitToday())
...
} 

**A_OrderFilledLot**
说明 返回当前公式应用的帐户下当前商品的某个委托单的成交数量。 
语法 Numeric A_OrderFilledLot(Integer nIndex = 0) 
参数 nIndex 当日委托单数组的索引值，以1为基值递增。 
nIndex = 0 时取最后提交的委托单数据。 
备注 返回当前公式应用的帐户下当前商品的某个委托单的成交数量，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOrderCount();
For i = 1 To nCount
{
OrderFilledLot = A_OrderFilledLot(i);
...
} 

**A_OrderFilledPrice**
说明 返回当前公式应用的帐户下当前商品的某个委托单的成交价格。 
语法 Numeric A_OrderFilledPrice(Integer nIndex = 0) 
参数 nIndex 当日委托单数组的索引值，以1为基值递增。 
nIndex = 0 时取最后提交的委托单数据。 
备注 返回当前公式应用的帐户下当前商品的某个委托单的成交价格，返回值为浮点数。
该成交价格可能为多个成交价格的平均值。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOrderCount();
For i = 1 To nCount
{
OrderFilledPrice = A_OrderFilledPrice(i);
...
} 

**A_OrderLot**
说明 返回当前公式应用的帐户下当前商品的某个委托单的委托数量。 
语法 Numeric A_OrderLot(Integer nIndex = 0) 
参数 nIndex 当日委托单数组的索引值，以1为基值递增。 
nIndex = 0 时取最后提交的委托单数据。 
备注 返回当前公式应用的帐户下当前商品的某个委托单的委托数量，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOrderCount();
For i = 1 To nCount
{
OrderLot = A_OrderLot(i);
...
} 


**A_OrderPrice**
说明 返回当前公式应用的帐户下当前商品的某个委托单的委托价格。 
语法 Numeric A_OrderPrice(Integer nIndex = 0) 
参数 nIndex 当日委托单数组的索引值，以1为基值递增。 
nIndex = 0 时取最后提交的委托单数据。 
备注 返回当前公式应用的帐户下当前商品的某个委托单的委托价格，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOrderCount();
For i = 1 To nCount
{
OrderPrice = A_OrderPrice(i);
...
} 

**A_OrderStatus**
说明 返回当前公式应用的帐户下当前商品的某个委托单的状态。 
语法 Integer A_OrderStatus(Integer nIndex = 0) 
参数 nIndex 当日委托单数组的索引值，以1为基值递增。 
nIndex = 0 时取最后提交的委托单数据。  
备注 返回当前公式应用的帐户下当前商品的某个委托单的状态，返回值为整型。
该函数返回值可以与Enum_Declare、Enum_Declared、Enum_Filled等委托状态枚举值进行比较，根据类型不同分别处理。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOrderCount();
For i = 1 To nCount
{
nStatus = A_OrderStatus(i);
If(nStatus == Enum_Filled)
...
} 

**A_OrderTime**
说明 返回当前公式应用的帐户下当前商品的某个委托单的委托时间。 
语法 Numeric A_OrderTime(Integer nIndex = 0) 
参数 nIndex 当日委托单数组的索引值，以1为基值递增。 
nIndex = 0 时取最后提交的委托单数据。 
备注 返回当前公式应用的帐户下当前商品的某个委托单的委托时间，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 nCount = A_GetOrderCount();
For i = 1 To nCount
{
OrderTime = A_OrderTime(i);
...
} 

**A_PositionProfitLoss**
说明 返回当前公式应用的帐户下当前商品的持仓盈亏。 
语法 Numeric A_PositionProfitLoss() 
参数 无 
备注 返回当前公式应用的帐户下当前商品的持仓盈亏，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 无  

**A_PreviousEquity**
说明 返回当前公式应用的交易帐户的昨日结存。 
语法 Numeric A_PreviousEquity() 
参数 无 
备注 返回当前公式应用的交易帐户的昨日结存，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 无  

**A_ProfitLoss**
说明 返回当前公式应用的交易帐户的浮动盈亏。 
语法 Numeric A_ProfitLoss() 
参数 无 
备注 返回当前公式应用的交易帐户的浮动盈亏，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 无  

**A_SellAvgPrice**
说明 返回当前公式应用的帐户下当前商品的卖出持仓均价。 
语法 Numeric A_SellAvgPrice() 
参数 无 
备注 返回当前公式应用的帐户下当前商品的卖出持仓均价，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 无  

**A_SellPosition**
说明 返回当前公式应用的帐户下当前商品的卖出持仓。 
语法 Numeric A_SellPosition() 
参数 无 
备注 返回当前公式应用的帐户下当前商品的卖出持仓，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 当前持空仓3手，A_SellPosition返回3。 

**A_SellProfitLoss**
说明 返回当前公式应用的帐户下当前商品的卖出持仓盈亏。 
语法 Numeric A_SellProfitLoss() 
参数 无 
备注 返回当前公式应用的帐户下当前商品的卖出持仓盈亏，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 无  

**A_SendOrder**
说明 针对当前公式应用的帐户、商品发送委托单。 
语法 Bool A_SendOrder(Integer BuyOrSell,Integer EntryOrExit,Numeric Lot,Numeric Price,Numeric StopPrice,Numeric ProfitPrice) 
参数 BuyOrSell 发送委托单的买卖类型，取值为Enum_Buy或Enum_Sell之一； 
EntryOrExit 发送委托单的开平仓类型，取值为Enum_Entry,Enum_Exit,Enum_ExitToday之一； 
Lot 委托单的交易数量； 
Price 委托单的交易价格。 
StopPrice 止损触发单触发价格。 
ProfitPrice 止盈触发单触发价格。  
备注 针对当前公式应用的帐户、商品发送委托单，发送成功返回True,发送失败返回False。 
该函数可针对叠加商品进行处理，可用Data1.A_SendOrder(...)进行调用。 
该函数直接发单，不经过任何确认，并会在每次公式计算时发送，一般需要配合着仓位头寸进行条件处理，在不清楚运行机制的情况下，请慎用。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 示例1： 
If (A_BuyPosition()>0 && A_GetOpenOrderCount()==0)
{
A_SendOrder(Enum_Sell,Enum_Exit,A_BuyPosition(),Q_BidPrice());
...
}
注意:A_SendOrder不同于Buy和Sell指令，A_SendOrder只适用于实时行情中发送委托单，不可以用于测试。 

示例2：控制A_SendOrder发单配合全局变量示例(开多仓) 
if( /*开仓条件*/ && A_BuyPosition()==0 && GetGlobalVar(0)==0) 
{ 
A_SendOrder(Enum_Buy,Enum_Entry,1,Q_AskPrice); 
} 

示例3：设置A_SendOrder止损止盈单触发价格(止盈触发价格7000，止损触发价格6000) 
A_SendOrder(Enum_Buy,Enum_Entry,1,Q_AskPrice,6000,7000);  


**A_TodayBuyPosition**
说明 返回当前公式应用的帐户下当前商品的当日买入持仓。 
语法 Numeric A_TodayBuyPosition() 
参数 无 
备注 返回当前公式应用的帐户下当前商品的当日买入持仓，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 无  

**A_TodayDeposit**
说明 返回当前公式应用的交易帐户的当日入金。 
语法 Numeric A_TodayDeposit() 
参数 无 
备注 返回当前公式应用的交易帐户的当日入金，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 无  

**A_TodayDrawing**
说明 返回当前公式应用的交易帐户的当日出金。 
语法 Numeric A_TodayDrawing() 
参数 无 
备注 返回当前公式应用的交易帐户的当日出金，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 无  

**A_TodaySellPosition**
说明 返回当前公式应用的帐户下当前商品的当日卖出持仓。 
语法 Numeric A_TodaySellPosition() 
参数 无 
备注 返回当前公式应用的帐户下当前商品的当日卖出持仓，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 无  

**A_TotalAvgPrice**
说明 返回当前公式应用的帐户下当前商品的持仓均价。 
语法 Numeric A_TotalAvgPrice() 
参数 无 
备注 返回当前公式应用的帐户下当前商品的持仓均价，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 无  

**A_TotalFreeze**
说明 返回当前公式应用的交易帐户的冻结资金。 
语法 Numeric A_TotalFreeze() 
参数 无 
备注 返回当前公式应用的交易帐户的冻结资金，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 无  

**A_TotalMargin**
说明 返回当前公式应用的交易帐户的保证金。 
语法 Numeric A_TotalMargin() 
参数 无 
备注 返回当前公式应用的交易帐户的保证金，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 无  

**A_TotalPosition**
说明 返回当前公式应用的帐户下当前商品的总持仓。 
语法 Numeric A_TotalPosition() 
参数 无 
备注 返回当前公式应用的帐户下当前商品的总持仓，返回值为浮点数。
该持仓为所有持仓的合计值，正数表示多仓，负数表示空仓，零为无持仓。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 无  


**///////////////Q函数详解////////////////**
//Q函数返回端口API的相关订阅数据。
//Q函数除了tick级别的周期，在其他周期都可以调用，但是只在最后一根bar
	的位置显示tick每次订阅发送的当前数据。Q函数当中大多都是tick级别显示
	的订阅数据。

**Q_AskPrice**
说明 当前公式应用商品的最新卖盘价格。 
语法 Numeric Q_AskPrice(Integer nIndex=0) 
参数 nIndex 买卖盘数组的索引值，以0为基值递增，默认值为0。 
备注 当前公式应用商品的最新卖盘价格，返回值为浮点数。如果Index小于0或者大于等于BidAskSize，则返回无效值。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例：显示盘口的卖一的价格，或者有些软件中的盘口的卖出价位置的价格。 

**Q_AskPriceFlag**
说明 当前公式应用商品的卖盘价格变化标志。 
语法 Integer Q_AskPriceFlag() 
参数 无 
备注 当前公式应用商品的卖盘价格变化标志，返回值为整型，1为上涨，-1为下跌，0为不变。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 无 

**Q_AskVol**
说明 当前公式应用商品的最新卖盘量。 
语法 Numeric Q_AskVol(Integer nIndex=0) 
参数 nIndex 买卖盘数组的索引值，以0为基值递增，默认值为0。 
备注 当前公式应用商品的最新卖盘量，返回值为浮点数。如果Index小于0或者大于等于BidAskSize，则返回无效值。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 无 

**Q_AvgPrice**
说明 当前公式应用商品的实时均价。 
语法 Numeric Q_AvgPrice() 
参数 无 
备注 当前公式应用商品的实时均价，即结算价，返回值为浮点数。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 无 

**Q_BidPrice**
说明 当前公式应用商品的最新买盘价格。 
语法 Numeric Q_BidPrice(Integer nIndex=0) 
参数 nIndex 买卖盘数组的索引值，以0为基值递增，默认值为0。 
备注 当前公式应用商品的最新最新买盘价格，返回值为浮点数。如果Index小于0或者大于等于BidAskSize，则返回无效值。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例：显示盘口买一的价格。或者有些软件中的盘口的买入价位置的价格。

**Q_BidPriceFlag**
说明 当前公式应用商品的买盘价格变化标志。 
语法 Integer Q_BidPriceFlag() 
参数 无 
备注 当前公式应用商品的买盘价格变化标志，返回值为整型，1为上涨，-1为下跌，0为不变。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 无 

**Q_BidVol**
说明 当前公式应用商品的最新买盘量。 
语法 Numeric Q_BidVol(Integer nIndex=0) 
参数 nIndex 买卖盘数组的索引值，以0为基值递增，默认值为0。 
备注 当前公式应用商品的最新买盘量，返回值为浮点数。如果Index小于0或者大于等于BidAskSize，则返回无效值。 
注：不能使用于历史测试，仅适用于实时行情交易。  
示例 无 

**Q_Open**
说明 当前公式应用商品的当日开盘价。 
语法 Numeric Q_Open() 
参数 无 
备注 当前公式应用商品的当日开盘价，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_Close**
说明 当前公式应用商品的当日收盘价。（当日未收盘则取得昨日收盘价。） 
语法 Numeric Q_Close() 
参数 无 
备注 当前公式应用商品的当日收盘价，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_High**
说明 当前公式应用商品的当日最高价。 
语法 Numeric Q_High() 
参数 无 
备注 当前公式应用商品的当日最高价，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_HisHigh**
说明 当前公式应用商品的历史最高价。（暂时不可用） 
语法 Numeric Q_HisHigh() 
参数 无 
备注 当前公式应用商品的历史最高价，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_HisLow**
说明 当前公式应用商品的历史最低价。（暂时不可用） 
语法 Numeric Q_HisLow() 
参数 无 
备注 当前公式应用商品的历史最低价，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_InsideVol**
说明 当前公式应用商品的内盘。 
语法 Numeric Q_InsideVol() 
参数 无 
备注 当前公式应用商品的内盘，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_Last**
说明 当前公式应用商品的最新价。 
语法 Numeric Q_Last() 
参数 无 
备注 当前公式应用商品的最新价，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_LastDate**
说明 当前公式应用商品的最新成交日期。 
语法 Integer Q_LastDate() 
参数 无 
备注 当前公式应用商品的最新成交日期，返回值为Date类型。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_LastFlag**
说明 当前公式应用商品的最新价变化标志。 
语法 Integer Q_LastFlag() 
参数 无 
备注 当前公式应用商品的最新价变化标志，返回值为整型，1为上涨，-1为下跌，0为不变。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_LastTime**
说明 当前公式应用商品的最新成交时间。 
语法 Numeric Q_LastTime() 
参数 无 
备注 当前公式应用商品的最新成交时间，返回值为Time类型。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_LastVol**
说明 当前公式应用商品的现手。 
语法 Numeric Q_LastVol() 
参数 无 
备注 当前公式应用商品的现手，返回值为浮点数，单位为手。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_Low**
说明 当前公式应用商品的当日最低价。 
语法 Numeric Q_Low() 
参数 无 
备注 当前公式应用商品的当日最低价，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_LowerLimit**
说明 当前公式应用商品的当日跌停板价。 
语法 Numeric Q_LowerLimit() 
参数 无 
备注 当前公式应用商品的当日跌停板价，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_OpenInt**
说明 当前公式应用商品的持仓量。 
语法 Numeric Q_OpenInt() 
参数 无 
备注 当前公式应用商品的持仓量，返回值为浮点数，单位为手。仅对期货有效，其他商品返回无效值。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_OpenIntFlag**
说明 当前公式应用商品的持仓量变化标志。 
语法 Integer Q_OpenIntFlag() 
参数 无 
备注 当前公式应用商品的持仓量变化标志，返回值为整型，1为增加，-1为下降，0为不变。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_Oscillation**
说明 当前公式应用商品的振幅。 
语法 Numeric Q_Oscillation() 
参数 无 
备注 当前公式应用商品的振幅，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_OutsideVol**
说明 当前公式应用商品的外盘。 
语法 Numeric Q_OutsideVol() 
参数 无 
备注 当前公式应用商品的外盘，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_PreOpenInt**
说明 当前公式应用商品的昨日持仓量。 
语法 Numeric Q_PreOpenInt() 
参数 无 
备注 当前公式应用商品的昨日持仓量，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_PreSettlePrice**
说明 当前公式应用商品的昨日结算价。 
语法 Numeric Q_PreSettlePrice() 
参数 无 
备注 当前公式应用商品的昨日结算价，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_PriceChg**
说明 当前公式应用商品的当日涨跌。 
语法 Numeric Q_PriceChg() 
参数 无 
备注 当前公式应用商品的当日涨跌，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_PriceChgRatio**
说明 当前公式应用商品的当日涨跌幅。 
语法 Numeric Q_PriceChgRatio() 
参数 无 
备注 当前公式应用商品的当日涨跌幅，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_TickChg**
说明 当前公式应用商品的最新笔升跌。 
语法 Numeric Q_TickChg() 
参数 无 
备注 当前公式应用商品的最新笔升跌，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_TodayEntryVol**
说明 当前公式应用商品的当日开仓量。 
语法 Numeric Q_TodayEntryVol() 
参数 无 
备注 当前公式应用商品的当日开仓量，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_TodayExitVol**
说明 当前公式应用商品的当日平仓量。 
语法 Numeric Q_TodayExitVol() 
参数 无 
备注 当前公式应用商品的当日平仓量，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_TotalVol**
说明 当前公式应用商品的当日成交量。 
语法 Numeric Q_TotalVol() 
参数 无 
备注 当前公式应用商品的当日成交量，返回值为浮点数，单位为手。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_TurnOver**
说明 当前公式应用商品的成交金额。 
语法 Numeric Q_TurnOver() 
参数 无 
备注 当前公式应用商品的成交金额，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**Q_UpperLimit**
说明 当前公式应用商品的当日涨停板价。 
语法 Numeric Q_UpperLimit() 
参数 无 
备注 当前公式应用商品的当日涨停板价，返回值为浮点数。
注：不能使用于历史测试，仅适用于实时行情交易。 
示例 无 

**QuoteDataExist**
说明 当前公式应用商品的行情数据是否有效。 
语法 Bool QuoteDataExist() 
参数 无 
备注 当前公式应用商品的行情数据是否有效，返回布尔值。如果数据已经准备好，返回True，则返回False。 
示例 无














