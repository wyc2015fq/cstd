
# 量化交易研究———基础篇（5）除去停牌和ST - leofionn的博客 - CSDN博客


2018年05月17日 14:54:26[leofionn](https://me.csdn.net/qq_36142114)阅读数：445


stock = get_concept_stocks('301636',date)
\#除去停牌和ST
price=get_price(stock, None, date, '1d', ['is_paused', 'is_st'], False, None, 1, is_panel=1)
stopstk=price['is_paused'].iloc[-1]
ststk=price['is_st'].iloc[-1]
startstk=(stopstk[stopstk==0].index)
okstk=(ststk[ststk==0].index)
\#最终结果
tradestk=list(set(startstk)&set(okstk)&set(stock))
log.info('停牌个股：{},ST个股:{},已剔除'.format(stopstk[stopstk!=0].index,ststk[ststk!=0].index))
return tradestk

