# esp8266中cjson测试代码 - c1063891514的博客 - CSDN博客
2019年03月22日 14:15:34[c1063891514](https://me.csdn.net/c1063891514)阅读数：20个人分类：[物联网](https://blog.csdn.net/c1063891514/article/category/7407086)
```
////////////////////////////////
char buf[64];
void ICACHE_FLASH_ATTR myparseJson() {

	//首先创建一个json数据
	/*
		{
			"myid": "84:f3:eb:b3:a7:05",
			"number": 2,
			"value": {
				"name": "XXX",
				"age": 18
			},
			"hexArry": [51, 15, 63, 22, 96]
		}
	 * */
	char * jsonStr = "{	\"myid\": \"84:f3:eb:b3:a7:05\",	"
			"\"number\": 2,\"value\": {\"name\": \"XXX\",\"age\": 18},	\"hexArry\": [51, 15, 63, 22, 96]	}";
	//首先整体判断是否为一个json格式的数据
	cJSON *pJsonstr = cJSON_Parse(jsonStr);
	if (pJsonstr !=NULL) {
	        //串口打印这段数据
			char *s = cJSON_Print(pJsonstr);
			os_sprintf(buf,"pJsonstr: %s\r\n", s);
			at_port_print(buf);

			cJSON_free((void *) s);
			//------------------------------------------------------
			//解析myid字段字符串内容
			cJSON *pMyidAdress = cJSON_GetObjectItem(pJsonstr, "myid");
			//判断myid字段是否json格式
			if (pMyidAdress) {
				//判断mac字段是否string类型
				if (cJSON_IsString(pMyidAdress))
					os_sprintf(buf,"get MyidAdress:%s \n", pMyidAdress->valuestring);
				at_port_print(buf);

			} else
				{
				os_sprintf(buf,"get MacAdress failed \n");
				at_port_print(buf);

				}
			//------------------------------------------------------
			 //解析number字段int内容
			cJSON *pNumber = cJSON_GetObjectItem(pJsonstr, "number");
			//判断number字段是否存在
			if (pNumber){
				 //判断number字段是否数字整型类型
				if (cJSON_IsNumber(pNumber))
				os_sprintf(buf,"get Number:%d \n", pNumber->valueint);
        		at_port_print(buf);
			}
			else
				{
				os_sprintf(buf,"get Number failed \n");
				at_port_print(buf);
				}
	        //解析value字段内容，判断是否为json
	        cJSON *pValue = cJSON_GetObjectItem(pJsonstr, "value");
	        if (pValue) {
	            //进一步剖析里面的name字段:注意这个根节点是 pValue
	            cJSON *pName = cJSON_GetObjectItem(pValue, "name");
	            if (pName)
	                if (cJSON_IsString(pName))
	                    os_sprintf(buf,"get value->Name : %s \n", pName->valuestring);
        		at_port_print(buf);

	            //进一步剖析里面的age字段:注意这个根节点是 pValue
	            cJSON *pAge = cJSON_GetObjectItem(pValue, "age");
	            if (pAge)
	                if (cJSON_IsNumber(pAge))
	                    os_sprintf(buf,"get value->Age : %d \n", pAge->valueint);
	            		at_port_print(buf);

	        }
			//------------------------------------------------------
	        //剖析
	            cJSON *pArry = cJSON_GetObjectItem(pJsonstr, "hexArry");
	            if (pArry) {
	            //获取数组长度
	            int arryLength = cJSON_GetArraySize(pArry);
	            os_sprintf(buf,"get arryLength : %d \n", arryLength);	at_port_print(buf);

	            //逐个打印
	            int i ;
	            for (i = 0; i < arryLength; i++)
	            os_sprintf(buf,"cJSON_GetArrayItem(pArry, %d)= %d\n",i,cJSON_GetArrayItem(pArry, i)->valueint);
				at_port_print(buf);

	            }

		} else {
			os_sprintf(buf,"this is not a json data ... \n");
			at_port_print(buf);

		}

	cJSON_Delete(pJsonstr);

}


void ICACHE_FLASH_ATTR mycreatJson(){

/*
{
	"project":	{
			"mcu":	"pro_mrt",
			"cpu":	"robot"
		},

}
 */
	//从里向外创建
	 int list[4] = {8,8,7,8};
	cJSON *pRoot = cJSON_CreateObject();
	cJSON *pRoot1 = cJSON_CreateObject();

    cJSON_AddStringToObject(pRoot1,"mcu","pro_mrt");

    cJSON_AddStringToObject(pRoot1,"cpu","robot");

	cJSON_AddItemToObject(pRoot,"project",pRoot1);

	  cJSON_AddItemToObject(pRoot, "lists", cJSON_CreateIntArray(list, 4));
	char *s = cJSON_Print(pRoot);
	os_sprintf(buf,"\r\n creatJson : %s\r\n", s);
	at_port_print(buf);

	cJSON_free((void *) s);

	cJSON_Delete(pRoot);
}

void ICACHE_FLASH_ATTR testjson(){

	/*
	 * {
		"content":
			"{
				"return":3,
				"data":[ 1, 2, 3 ]
			}",
		"type":	XX
	}
	 */
	//从里向外创建
	int senserlen = 1;
	 int list[4] = {5,6,7,8};
	cJSON *pRoot = cJSON_CreateObject();
	cJSON *pRoot1 = cJSON_CreateObject();

	cJSON_AddNumberToObject(pRoot,"type",1);

	cJSON_AddNumberToObject(pRoot1,"return",senserlen);
	if(senserlen >0)
	{
		cJSON_AddItemToObject(pRoot1, "data", cJSON_CreateIntArray(list, 4));
	}

	cJSON_AddItemToObject(pRoot, "content", pRoot1);
	char *s = cJSON_Print(pRoot);
	os_sprintf(buf,"\r\n creatJson : %s\r\n", s);
	at_port_print(buf);
	cJSON_free((void *) s);

	cJSON_Delete(pRoot);


}
```


