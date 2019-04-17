# jsoncpp解析拼装数组 - DoubleLi - 博客园






Cocos2d-x添加jsoncpp应该资料都有了，今天来讲讲数组的解析和拼装～





[cpp] [view plain](http://blog.csdn.net/chichigan1314/article/details/8280605#)[copy](http://blog.csdn.net/chichigan1314/article/details/8280605#)







- int main()  
- {  
- 数组创建与分析:  
- 例子一:  
- string strValue = "{\"ldh\":\"001\",\"gfc\":\"002\",\"yyj\":\"003\",\"andy\":[\"005\",\"123\",\"true\"]}";  
- Json::Reader read;  
- Json::Value value;  
- value["ldh"] = "001";  
- value["gfc"] = "002";  
- value["andy"].append( "005" );  
- value["andy"].append( "123" );  
- value["andy"].append( "true" );  
- //if( read.parse( strValue,value ) )  
- {  
- Json::Value val_array = value["andy"];  
- int iSize = val_array.size();  
- for ( int nIndex = 0;nIndex < iSize;++ nIndex )  
- {  
- cout<<val_array[nIndex]<<endl;  
- }  
- }  
- 
- 
- 例子二:  
- Json::Reader read;  
- Json::Value value;  
- value["ldh"] = "001";  
- value["gfc"] = "002";  
- Value item;  
- Value array;  
- item["andy1"] = "005";  
- array.append( item );  
- item["andy1"] = "123";  
- array.append( item );  
- item["andy1"] = "true";  
- array.append( item );  
- value["andy"] = array;  
- cout<<value.toStyledString()<<endl;  
- Json::Value val_array = value["andy"];  
- int iSize = val_array.size();  
- for ( int nIndex = 0;nIndex < iSize;++ nIndex )  
- {  
- cout<<val_array[nIndex]<<endl;  
- if ( !val_array[nIndex]["andy1"].isNull() )  
- {  
- cout<<val_array[nIndex]["andy1"]<<endl;  
- }  
- }  
- 
- 
- 例子三:  
- std::string strValue = "{\"name\":\"json\",\"array\":[{\"cpp\":\"jsoncpp\"},{\"java\":\"jsoninjava\"},{\"php\":\"support\"}]}";    
- Json::Value value;  
- Reader read;  
- if ( !read.parse( strValue,value ) )  
- {  
- return -1;  
- }  
- cout<<value.toStyledString()<<endl;  
- Json::Value val_array = value["array"];  
- int iSize = val_array.size();  
- for ( int nIndex = 0;nIndex < iSize;++ nIndex )  
- {  
- cout<<val_array[nIndex]<<endl;  
- if ( val_array[nIndex].isMember( "cpp" ) )  
- {  
- cout<<val_array[nIndex]["cpp"]<<endl;  
- }  
- }  
- 
- getchar();  
- return 0;  
- }  










