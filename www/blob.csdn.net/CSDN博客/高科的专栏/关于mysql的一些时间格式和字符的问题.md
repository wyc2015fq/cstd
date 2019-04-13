
# 关于mysql的一些时间格式和字符的问题 - 高科的专栏 - CSDN博客

2014年03月20日 18:17:31[高科](https://me.csdn.net/pbymw8iwm)阅读数：5751


最近在做一些游戏的数据分析，需要对大量数据的用户行为进行处理存库，其中有个数据库字段是datetime类型的，这个以前都没用过，我以前都喜欢用int来存放时间戳，但这次这样用，我就得在数据库中转换了，
char buf_update[81920] = {0};
int length = 0;
sprintf(buf_update, "update lost_player set account = %s,lastlogin = from_unixtime(%u), reallevel = %u, fox = %u, firstmap = %u, lasttask = %u, 2ndmap = %u, 3rdmap = %u, taskstatus = %u where playerid = %llu",localAction.account.c_str(),localAction.lastlogin,
 localAction.reallevel, localAction.fox, localAction.firstmap, localAction.lasttask, localAction.secondmap, localAction.thirdmap, localAction.taskstatus, localAction.playerId);
其中的lastlogin就是datetime类型的数据。
另外还有就是当设置文字编码是utf8的时候一个中文字符占用的是3个字节，而并不是我之前认识的2个字符，谨记。

