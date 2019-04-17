# JsonCpp使用方法详解 - DoubleLi - 博客园






JSON全称为JavaScript ObjectNotation，它是一种轻量级的数据交换格式，易于阅读、编写、解析。jsoncpp是c++解析JSON串常用的解析库之一。

jsoncpp中主要的类：

Json::Value：可以表示所有支持的类型，如：int , double ,string , object, array等。其包含节点的类型判断(isNull,isBool,isInt,isArray,isMember,isValidIndex等),类型获取(type),类型转换(asInt,asString等),节点获取(get,[]),节点比较(重载<,<=,>,>=,==,!=),节点操作(compare,swap,removeMember,removeindex,append等)等函数。


Json::Reader：将文件流或字符串创解析到Json::Value中，主要使用parse函数。Json::Reader的构造函数还允许用户使用特性Features来自定义Json的严格等级。

Json::Writer：与JsonReader相反，将Json::Value转换成字符串流等，Writer类是一个纯虚类，并不能直接使用。在此我们使用 Json::Writer 的子类：Json::FastWriter(将数据写入一行,没有格式),Json::StyledWriter(按json格式化输出,易于阅读)。

Json::Reader可以通过对Json源目标进行解析，得到一个解析好了的Json::Value，通常字符串或者文件输入流可以作为源目标。


如下Json文件example.json：


- 



{


- 



    "encoding" : "UTF-8",


- 



    "plug-ins" : [


- 



        "python",


- 



        "c++",


- 



        "ruby"


- 



        ],


- 



    "indent" : { "length" : 3, "use_space": true }


- 



	"tab":null


- 



}




使用Json::Reader对Json文件进行解析：




- 



Json::Value root;


- 



Json::Reader reader;


- 



std::ifstream ifs("example.json");//open file example.json


- 




- 



if(!reader.parse(ifs, root)){


- 



   // fail to parse


- 



}


- 



else{


- 



   // success


- 



   std::cout<<root["encoding"].asString()<<endl;


- 



std::cout<<root["indent"]["length"].asInt()<<endl;


- 



}




使用Json::Reader对字符串进行解析：




- 



Json::Value root;


- 



  Json::Reader reader;


- 



  const char* s = "{\"uploadid\": \"UP000000\",\"code\": 100,\"msg\": \"\",\"files\": \"\"}"; 


- 



  if(!reader.parse(s, root)){


- 



    // "parse fail";


- 



  }


- 



  else{


- 



      std::cout << root["uploadid"].asString();//print "UP000000"


- 



  }




Json::Writer 和 Json::Reader相反，是把Json::Value对象写到string对象中，而且Json::Writer是个抽象类，被两个子类Json::FastWriter和Json::StyledWriter继承。 
简单来说FastWriter就是无格式的写入，这样的Json看起来很乱没有格式，而StyledWriter就是带有格式的写入，看起来会比较友好。




- 



Json::Value root;


- 



Json::Reader reader;


- 



Json::FastWriter fwriter;


- 



Json::StyledWriter swriter;


- 




- 



if(! reader.parse("example.json", root)){


- 



// parse fail


- 



    return 0;


- 



}


- 



std::string str = fwriter(root);


- 



std::ofstream ofs("example_fast_writer.json");


- 



ofs << str;


- 



ofs.close();


- 




- 



str = swriter(root);


- 



ofs.open("example_styled_writer.json");


- 



ofs << str;


- 



ofs.close();


- 




- 



结果1:example_styled_writer.json：


- 



{


- 



    "encoding" : "UTF-8",


- 



    "plug-ins" : [


- 



        "python",


- 



        "c++",


- 



        "ruby"


- 



        ],


- 



    "indent" : { "length" : 3, "use_space": true }


- 



	"tab":null


- 



}


- 




- 



结果2:example_fast_writer.json：


- 



{"encoding" : "UTF-8","plug-ins" : ["python","c++","ruby"],"indent" : { "length" : 3, "use_space": true}}




Json其它函数的应用：
1、判断KEY值是否存在：




- 



if(root.isMember("encoding")){


- 



    std::cout<<"encoding is a member"<<std::endl;


- 



}


- 



else{


- 



std::cout<<"encoding is not a member"<<std::endl;


- 



}




2、判断Value是否为null：



if(root["tab"].isNull()){
    std::cout << "isNull" <<std::endl;//print isNull
}



完整例子使用举例来自于CSDN下载网友的程序：

源码下载地址：[http://download.csdn.net/download/woniu211111/9966907](http://download.csdn.net/download/woniu211111/9966907)


- 



/********************************************************


- 



Copyright (C), 2016-2017,


- 



FileName: 	main


- 



Author: 	woniu201


- 



Email: 		wangpengfei.201@163.com


- 



Created: 	2017/09/06


- 



Description:use jsoncpp src , not use dll, but i also provide dll and lib.


- 



********************************************************/


- 




- 



#include "stdio.h"


- 



#include <string>


- 



#include "jsoncpp/json.h"


- 




- 



using namespace std;


- 




- 



/************************************


- 



@ Brief:		read file


- 



@ Author:		woniu201 


- 



@ Created: 		2017/09/06


- 



@ Return:		file data  


- 



************************************/


- 



char *getfileAll(char *fname)


- 



{


- 



	FILE *fp;


- 



	char *str;


- 



	char txt[1000];


- 



	int filesize;


- 



	if ((fp=fopen(fname,"r"))==NULL){


- 



		printf("open file %s fail \n",fname);


- 



		return NULL;


- 



	}


- 




- 



	/*


- 



	获取文件的大小


- 



	ftell函数功能:得到流式文件的当前读写位置,其返回值是当前读写位置偏离文件头部的字节数.


- 



	*/


- 



	fseek(fp,0,SEEK_END); 


- 



	filesize = ftell(fp);


- 




- 



	str=(char *)malloc(filesize);


- 



	str[0]=0;


- 




- 



	rewind(fp);


- 



	while((fgets(txt,1000,fp))!=NULL){


- 



		strcat(str,txt);


- 



	}


- 



	fclose(fp);


- 



	return str;


- 



}


- 




- 



/************************************


- 



@ Brief:		write file


- 



@ Author:		woniu201 


- 



@ Created: 		2017/09/06


- 



@ Return:		    


- 



************************************/


- 



int writefileAll(char* fname,const char* data)


- 



{


- 



	FILE *fp;


- 



	if ((fp=fopen(fname, "w")) == NULL)


- 



	{


- 



		printf("open file %s fail \n", fname);


- 



		return 1;


- 



	}


- 




- 



	fprintf(fp, "%s", data);


- 



	fclose(fp);


- 




- 



	return 0;


- 



}


- 




- 



/************************************


- 



@ Brief:		parse json data


- 



@ Author:		woniu201 


- 



@ Created: 		2017/09/06


- 



@ Return:		    


- 



************************************/


- 



int parseJSON(const char* jsonstr)


- 



{


- 



	Json::Reader reader;


- 



	Json::Value  resp;


- 




- 



	if (!reader.parse(jsonstr, resp, false))


- 



	{


- 



		printf("bad json format!\n");


- 



		return 1;


- 



	}


- 



	int result = resp["Result"].asInt();


- 



	string resultMessage = resp["ResultMessage"].asString();


- 



	printf("Result=%d; ResultMessage=%s\n", result, resultMessage.c_str());


- 




- 



	Json::Value & resultValue = resp["ResultValue"];


- 



	for (int i=0; i<resultValue.size(); i++)


- 



	{


- 



Json::Value subJson = resultValue[i];


- 



string cpuRatio = subJson["cpuRatio"].asString();


- 



string serverIp = subJson["serverIp"].asString();


- 



string conNum = subJson["conNum"].asString();


- 



string websocketPort = subJson["websocketPort"].asString();


- 



string mqttPort = subJson["mqttPort"].asString();


- 



string ts = subJson["TS"].asString();


- 




- 



printf("cpuRatio=%s; serverIp=%s; conNum=%s; websocketPort=%s; mqttPort=%s; ts=%s\n",cpuRatio.c_str(), serverIp.c_str(),


- 



conNum.c_str(), websocketPort.c_str(), mqttPort.c_str(), ts.c_str());


- 



	}


- 



return 0;


- 



}


- 




- 



/************************************


- 



@ Brief:		create json data


- 



@ Author:		woniu201 


- 



@ Created: 		2017/09/06


- 



@ Return:		    


- 



************************************/


- 



int createJSON()


- 



{


- 



Json::Value req;


- 



req["Result"] = 1;


- 



req["ResultMessage"] = "200";


- 




- 



Json::Value	object1;


- 



object1["cpuRatio"] = "4.04";


- 



object1["serverIp"] = "42.159.116.104";


- 



object1["conNum"] = "1";


- 



object1["websocketPort"] = "0";


- 



object1["mqttPort"] = "8883";


- 



object1["TS"] = "1504665880572";


- 



Json::Value	object2;


- 



object2["cpuRatio"] = "2.04";


- 



object2["serverIp"] = "42.159.122.251";


- 



object2["conNum"] = "2";


- 



object2["websocketPort"] = "0";


- 



object2["mqttPort"] = "8883";


- 



object2["TS"] = "1504665896981";


- 




- 



Json::Value jarray;


- 



jarray.append(object1);


- 



jarray.append(object2);


- 




- 



req["ResultValue"] = jarray;


- 




- 



Json::FastWriter writer;


- 



string jsonstr = writer.write(req);


- 




- 



printf("%s\n", jsonstr.c_str());


- 




- 



writefileAll("createJson.json", jsonstr.c_str());


- 



return 0;


- 



}


- 




- 



int main()


- 



{


- 



	/*读取Json串，解析Json串*/


- 



char* json = getfileAll("parseJson.json");


- 



parseJSON(json);


- 



printf("===============================\n");


- 




- 



	/*组装Json串*/


- 



createJSON();


- 




- 



getchar();


- 



return 1;


- 



}




参考：



[http://open-source-parsers.github.io/jsoncpp-docs/doxygen/index.html](http://open-source-parsers.github.io/jsoncpp-docs/doxygen/index.html)

[http://blog.csdn.net/yc461515457/article/details/52749575](http://blog.csdn.net/yc461515457/article/details/52749575)









