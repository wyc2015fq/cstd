# C++ 制作 json 数据 并 传送给服务端(Server) 的 php - Ms.Fox的专栏 - CSDN博客
2017年06月18日 23:39:40[lulu-lu](https://me.csdn.net/smbluesky)阅读数：350
json数据格式，这里举个基础的例子：
      {"name":"LGH"}
在C++里面，我用个函数把特定的数据组合成 json
```
1 void toJson(int count){
 2     char json[100];
 3     char result[200] = "{";
 4     char* temp = "\"count\"";
 5     char* temp_1 = "\"";
 6     char* temp_2 = "}";
 7     //cout<<count;
 8     _itoa(count,json,10);
 9     strcat(result,temp);
10     strcat(result,":");
11     strcat(result,temp_1);
12     strcat(result,json);
13     strcat(result,temp_1);
14     strcat(result,temp_2);
15     //cout<<"toJson="<<result<<endl;
16     sendMessage(result);//自定义函数，传送数据
17 }
```
注意我里面的传送函数，选择在里面执行，原因是，由 toJson 返回 char* result 会造成内存溢出，后来这样做，变为可以。
下面是sendMassage() 函数
```
1 void sendMessage(char *Information){
 2     // Powered by LGH - 2014
 3     //char url_for_lgh_connect_database[200000]="http://linguanh.nat123.net/updata.php?id=";//url
 4     char url_for_lgh_connect_database[200000]="http://localhost:8080/C++_face.php?DB=";//链接
 5     const char *x="From_AF";
 6 
 7     //Information=(char*)malloc(sizeof(char)*(102400));    
 8     strcat(url_for_lgh_connect_database,Information);
 9     WCHAR exchange_text_from_url[256],exchange_text_from_x[256];//宽字符char，如果爆红，修改项目的编码为多字节就可以了
10     LPCWSTR py = exchange_text_from_url;
11     LPCWSTR pz = exchange_text_from_x;
12 
13     MultiByteToWideChar( 0, 0,x, -1,exchange_text_from_x, 64 );//WCHAR to LPCWSTR，转化
14 
15     MultiByteToWideChar( 0, 0,url_for_lgh_connect_database, -1, exchange_text_from_url, 256 );
16 
17     if(InternetAttemptConnect(0) != ERROR_SUCCESS){
18         cout<<"你的电脑无法连接互联网，请开启联网功能。"<<endl;
19         cout<<"（Your computer can not connect the internet,please try to fix it!）"<<endl;
20     }
21     else{
22         //头文件 winInet 的 API 函数，访问一个链接
23         if(InternetOpenUrl(InternetOpen(x,INTERNET_OPEN_TYPE_DIRECT,NULL,NULL,NULL),url_for_lgh_connect_database,NULL,NULL,NULL,NULL)==NULL){
24             sendMessage(Information);
25         }
26         else{
27             //cout<<"result="<<Information<<endl;
28             //cout<<url_for_lgh_connect_database<<endl;
29             cout<<"信息已经传送给Lgh的数据库了。(The information has been sent to Lgh's database.)"<<endl;
30         }
31     }    
32     //free(Information);
33     //Information=NULL;
34 }
```
