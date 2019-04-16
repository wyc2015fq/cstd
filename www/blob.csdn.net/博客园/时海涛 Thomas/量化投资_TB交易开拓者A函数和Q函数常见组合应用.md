# 量化投资_TB交易开拓者A函数和Q函数常见组合应用 - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [量化投资_TB交易开拓者A函数和Q函数常见组合应用](https://www.cnblogs.com/noah0532/p/8879352.html)





1　　在交易开拓者当中，关于交易的做单方式一般分为：图表函数和A函数两类。

　　　两类的主要区别为：如果采用图表函数的话，所有的交易内容都是以图表上面的信号为准，当前仓位运行的实际状态是没有的，但是可以显示交易图标和图像，并且可以进行回测；对于A函数而已，不具有显示交易图表和图像和回测的功能，除了与图表函数具有要求点位做单的功能外，仓位的实际运行状态可以进行操作。

　　举例：比如图表函数中，在某一个点位进行下单信号，此时运用图表函数可在这个位置进行下单；但是如果此单并没有成交，如果突然又遇到一个平仓信号，图表函数认为已经成交了，会发出平仓指令。如果采用A函数，此单如果没有成交，即使遇到平仓信号，设置条件也不会发出平仓信号。

　　因此最大一个不同就是A函数考虑了单子的实际状态，而图表函数所有信号全部以图像显示的为准，忽略单子的实际状态。



2　　A函数的下单常用操作。

2.1　　A_BuyPosition和A_SellPosition、A_TotalPosition(合计仓位)

　　如果判断多图表上的

　　Data0.A_BuyPosition 或 Data0.A_SellPosition

　　Data1.A_BuyPosition 或 Data1.A_SellPosition

　　其中：A_TotalPosition：正数表示多仓，负数表示空仓，0表示无持仓

2.2　　以A_BuyPosition为例，A_SellPosition同理

2.3　　A_BuyPosition 是当前真实账户，当前商品的持多仓量

　　MarketPositon 是指测试过程中的持仓状态，不会出现锁仓的情况。在做真实交易时，尽量同步真实账户和测试的仓位及资金等信息。

2.4　　如果当前持多单3手，返回值为3

2.5　　最后一个bar，指的就是价格在跳动的那个bar

2.6　　只在最后一个bar上，也就是2.5说的那根bar上才有值，其他bar都是N/A



2.7　　关于下单函数A_SendOrder

　　对应图表函数，对应的下单函数如下：
|Buy Or Sell (Enum_Buy(买入)或Enum_Sell(卖出))|Entry(开仓)/Exit(平仓)/ExitToday(平今)|fLot(发送委托单量)|fPrice(交易价格)|示例||
|----|----|----|----|----|----|
|1.建多单使用buy  替换为可用参数Enum_Buy|开仓使用Entry 替换为Enum_Entry|开多仓单5手|价格可指定和使用Q函数，如Q_AskPrice()|A_SendOrder(Enum_Buy,Enum_Entry,5,Q_AskPrice());||
|2.平多仓使用sell  替换为可用参数Enum_Sell|平仓使用Exit /ExitToday(平今)替换为Enum_Exit(平仓),Enum_ExitToday(平今仓)之一|平多仓单5手,也可使用A_BuyPosition()获取|价格可指定和使用Q函数，如Q_BidPrice()|A_SendOrder(Enum_Sell,Enum_Exit,5,Q_BidPrice()||
|3.建空单使用Sell 替换为可用参数Enum_Sell|开仓使用Entry,替换为Enum_Entry|开空仓单5手|价格可指定和使用Q函数，如Q_BidPrice()|A_SendOrder(Enum_Sell,Enum_Entry,5,Q_BidPrice());||
|4.平空单使用Buy 替换为可用参数Enum_Buy|平仓使用Exit /ExitToday(平今)替换为Enum_Exit(平仓),Enum_ExitToday(平今仓)之一|开空仓单5手,也可使用A_SellPosition()获取|价格可指定和使用Q函数，如Q_AskPrice()|A_SendOrder(Enum_Buy,Enum_Exit,5,Q_AskPrice());| |



2.8　　该函数直接发单，不经过任何确认，并会在每次公式计算时发送，一般要配合仓位头寸条件处理。

2.9　　不能用于历史测试，仅适用于实时行情。

2.10　　持仓手数一定要写要平或开多少。不能写零，容易造成误平。



3　　用几个实例来说明函数应用。

If(BarStatus == 0)
{	// 全局变量初始化
	SetGlobalVar(1,0);  // 记录高空低多状态 1 0
	SetGlobalVar(2,0);  // 记录高多低空状态 -1 0
	SetGlobalVar(3,0);  // 记录平仓后当根不再开仓 1 0
}



//高空低多状态
		If(GetGlobalVar(1) == 0 And Data0.A_SellPosition() == 0 And Data1.A_BuyPosition() == 0 And Dclose[1] > st1_upperband[1] And Dclose[1] < st3_upperband[1] And Data0.Open == Data0.Close[1] And Data1.Open == Data1.Close[1] And Data0.Vol > 5 And Data1.Vol > 5)   // 高空低多开仓
		{
			Data0.A_SendOrder(Enum_Sell,Enum_Entry,LotsA,Data0.Open);	//Data0.高空开仓
			Data1.A_SendOrder(Enum_Buy,Enum_Entry,LotsB,Data1.Open);	//Data1.低多开仓
			SetGlobalVar(1,1);
		}

		If (GetGlobalVar(1) == 1 And Data0.A_SellPosition() > 0 And Data1.A_BuyPosition() > 0 And Dclose[1] < mean[1] And Data0.Open == Data0.Close[1] And Data1.Open == Data1.Close[1] And Data0.Vol > 5 And Data1.Vol > 5)   // 高空低多平仓
			{
				Data0.A_SendOrder(Enum_Buy,Enum_Exit,LotsA,Data0.Open);		//Data0.高空平仓
				Data1.A_SendOrder(Enum_Sell,Enum_Exit,LotsB,Data1.Open);    //Data1.低多平仓
				SetGlobalVar(1,0);
			}



 //高多低空状态
		If(GetGlobalVar(2) == 0 And Data0.A_BuyPosition() == 0 And Data1.A_SellPosition() == 0 And Dclose[1] < st1_downband[1] And Dclose[1] > st3_downband[1] And Data0.Open == Data0.Close[1] And Data1.Open == Data1.Close[1] And Data0.Vol > 5 And Data1.Vol > 5)   // 高多低空开仓
		{
			Data0.A_SendOrder(Enum_Buy,Enum_Entry,LotsA,Data0.Open);	//Data0.高多开仓
			Data1.A_SendOrder(Enum_Sell,Enum_Entry,LotsB,Data1.Open);	//Data1.低空开仓
			SetGlobalVar(2,-1);
		}

		If (GetGlobalVar(2) == -1 And Data0.A_BuyPosition() > 0 And Data1.A_SellPosition() > 0 And Dclose[1] > mean[1] And Data0.Open == Data0.Close[1] And Data1.Open == Data1.Close[1] And Data0.Vol > 5 And Data1.Vol > 5)   // 高多低空平仓
			{
				Data0.A_SendOrder(Enum_Sell,Enum_Exit,LotsA,Data0.Open);
				Data1.A_SendOrder(Enum_Buy,Enum_Exit,LotsB,Data1.Open);	
				SetGlobalVar(2,0);
			}




































