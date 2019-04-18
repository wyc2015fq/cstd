# c++中类型转换积累（持续更新） - writeeee的专栏 - CSDN博客
2015年08月20日 19:31:31[writeeee](https://me.csdn.net/writeeee)阅读数：233
- 情景：把const char* 赋值给 char[] 
 需要用memcpy(myRecordInfo->taskId,TaskId->GetText(),strlen(TaskId->GetText())); 
 //把TaskId->GetText(), 赋值给myRecordInfo->taskId
- 情景：把const char* 赋值给 int 
 myRecordInfo->taskType=atoi(tmpTaskType->GetText()); 
 //atoi 把tmpTaskType->GetText() 赋值给int 型 myRecordInfo->taskType
 3. 
