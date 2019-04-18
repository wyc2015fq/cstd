# python 读取json文件等处理 - wsp_1138886114的博客 - CSDN博客





2018年10月23日 09:46:55[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：625











### 文章目录
- [三个 pandas读取json文件方式](#_pandasjson_1)
- [批量读取 json 文件（中文 json）](#_json__json_63)
- [复杂json格式解析 保存 Excel](#json__Excel_108)
- [读取excel中某列的json数据（每个单元格数据格式为上面：数据格式一）](#exceljson_176)
- [特殊json文件格式化](#json_246)
- [写json文件](#json_415)




### 三个 pandas读取json文件方式

```python
from pandas.io.json import json_normalize
import pandas as pd
import json
import time
 
# 读入数据
data_str = open('AgriculturalDisease_train_annotations.json').read()
 
#———————————————————— 测试json_normalize ————————————————————
start_time = time.time()
for i in range(0, 300):
    data_list = json.loads(data_str)
    df = json_normalize(data_list)
end_time = time.time()
print (end_time - start_time)#耗时109秒
 
#———————————————————— 测试自己构造 ————————————————————
start_time = time.time()
for i in range(0, 300):
    data_list = json.loads(data_str)
    data = [[d["disease_class"], d["image_id"]] for d in data_list]
    df = pd.DataFrame(data, columns=["disease_class", "image_id"])
end_time = time.time()
print (end_time - start_time)#耗时22秒
 
#———————————————————— 测试read_json ————————————————————
start_time = time.time()
for i in range(0, 300):
    df = pd.read_json(data_str, orient='records')
end_time = time.time()
print (end_time - start_time)#耗时36秒
```

```python
# read_json
df2 = pd.read_json(data_str, orient='records')

# 自己构造
data_list = json.loads(data_str)
data = [[d["disease_class"], d["image_id"]] for d in data_list]
df = pd.DataFrame(data, columns=["disease_class", "image_id"])
df.head(5)

三种代码输出均如下
  disease_class	      	  image_id
0		1		62fd8bf4d53a1b94fbac16738406f10b.jpg
1		1		0bdec5cccbcade6b6e94087cb5509d98.jpg
2		1		8951e940341f77c8d361c1872c67b16d.jpg
3		1		7ed158da58c451f75fb790530d6f19cc.jpg
4		1		9b7399aa-1c3c-4137-ae4e-196cd23fe573___FREC_Sc...
```

**技巧**：将复杂的json串整理成以下格式再读取，再使用`data_list = json.loads(data_str)`读取即可

```
{"error_code":40007,"error_msg":"fail to recognize"}

[{"department": "abcdef",
 "query_result": {"code": "1000", "description": "1000"}, 
 "is_invoice": 1, 
 "imageName": "./imgs/8888888.jpeg", 
 "reco_result": {"total": "", "invoice_no": "123", "create_date": "", "check_code": ""}}]
```

### 批量读取 json 文件（中文 json）

`./out_file`下两个json文件内容如下：

```
out_01.txt 内容为："{"name_ID":"12343","name":"张三","身份编码":"未知"}"
out_02.txt 内容为："{"name_ID":"12344","name":"李四","身份编码":"98983"}"
```

```python
import json
import os

def img_w_h(text_path):
    data_str_list = []
    img_name_list = []
    for filename in os.listdir(text_path):
        file_path = text_path+'/'+filename
        print("获取文件:",file_path)
        data_str = open(file_path,"r",encoding='UTF-8').read()
        data_str_list.append(data_str)
        img_name_list.append(filename)
    print("data_str_list",data_str_list)
    return data_str_list,img_name_list

def json_to_excel(data_str_list):
    data_all = []
    for data_str in data_str_list:
        if data_str.startswith(u'\ufeff'):
            content = data_str.encode('utf8')[3:].decode('utf8')
            text = json.loads(content[1:-1])
            if text["身份编码"] =="未知":
                data_all.append(text["身份编码"])
    return data_all

if __name__ == "__main__":
    text_path = "./out_file"
    data_str_list, img_name_list = img_w_h(text_path)
    data_all = json_to_excel(data_str_list)
    print("data_all:",data_all)

输出：
获取文件: ./out_file/out_01.txt
获取文件: ./out_file/out_02.txt
data_str_list ['\ufeff"{"name_ID":"12343","name":"张三","身份编码":"98983"}"', '\ufeff"{"name_ID":"12343","name":"张三","身份编码":"未知"}"']
data_all: ['未知']
```

### 复杂json格式解析 保存 Excel

```python
import json
import pandas as pd

"""
数据格式一（为方便查看格式化如下）：

[{"department": "abcdef",
 "query_result": {"code": "1000", "description": "1000"}, 
 "is_invoice": 1, 
 "imageName": "./imgs/8888888.jpeg", 
 "reco_result": {"total": "", "invoice_no": "01111111", "create_date": "", "check_code": "", "invoice_code": ""}}, 
 {"department": "abcdef",
 "query_result": {}, 
 "is_invoice": 0, 
 "imageName": "./imgs/51111111.jpeg", 
 "reco_result": {}},
 ...]
"""


data_str = open('json_img.json').read()
data_list = json.loads(data_str)


data1_all = [[d["department"], d["is_invoice"], d["imageName"]] for d in data_list]
data2_all = [d["query_result"] for d in data_list]
data5_all = [d["reco_result"] for d in data_list]
for i in data1_all:
    print("data1_0:",i[0])
print("data5",data5_all)
print("data1",data1_all)


def get_data(data1_all,data2_all,data5_all):
    col_value = []
    for data1,data2,data5 in zip(data1_all,data2_all,data5_all):
        department = data1[0]
        is_invoice = data1[1]
        imageName = data1[2]
        if 'code' in data2:
            code = str(data2).split(",")[0].split(":")[1].replace("'", "").replace("}", "")
            description = str(data2).split(",")[1].split(":")[1].replace("'", "").replace("}", "")
        else:
            code = "NAN"
            description = "NAN"

        if 'total' in data5:
            total = str(data5).split(",")[0].split(":")[1].replace("'", "").replace("}", "")
            invoice_no = str(data5).split(",")[1].split(":")[1].replace("'", "").replace("}", "")
            create_date = str(data5).split(",")[2].split(":")[1].replace("'", "").replace("}", "")
            check_code = str(data5).split(",")[3].split(":")[1].replace("'", "").replace("}", "")
            invoice_code = str(data5).split(",")[4].split(":")[1].replace("'", "").replace("}", "")
        else:
            total = "NAN"
            invoice_no = "NAN"
            create_date = "NAN"
            check_code = "NAN"
            invoice_code = "NAN"
        col_value.append((department,is_invoice,imageName, code,description, total, invoice_no, create_date, check_code, invoice_code))
    return col_value

col_value = get_data(data1_all,data2_all,data5_all)

df = pd.DataFrame(col_value, index=None,columns=["department", "is_invoice", "imageName", "code", "description", "total", "invoice_no", "create_date", "check_code", "invoice_code"])
df.to_excel('excel_pd.xls')
```

### 读取excel中某列的json数据（每个单元格数据格式为上面：数据格式一）

```python
import json
import pandas as pd
import xlrd

excel_path = "C:\\Users\\Desktop\\test_data.xlsx"
def read_excel(excel_path):
    workbook = xlrd.open_workbook(excel_path)
    sheet = workbook.sheet_by_name("Sheet1")
    nrows = sheet.nrows
    list1 = []
    for i in range(1,nrows):
        list1.append(sheet.row_values(i)[0])
    return list1

def get_data(excel_path):
    list1 = read_excel(excel_path)
    All_data = []
    for i in range(len(list1)):          #遍历列表数据（相当于遍历该列所有单元格）
        data_list = json.loads(list1[i])
        # print("data_list：", type(data_list))
        for i in range(len(data_list)): #遍历该单元格列表中所有json串
            # print(type(data_list[i]))
            data_dict = data_list[i]
            try:
                imageNo = data_dict["imageNo"]
                businessType = data_dict["businessType"]
                reco_result = data_dict["reco_result"]

                try:
                    total = reco_result["total"]
                    invoice_no = reco_result["invoice_no"]
                    create_date = reco_result["create_date"]
                    check_code = reco_result["check_code"]
                    invoice_code = reco_result["invoice_code"]
                except:
                    total = "NAN"
                    invoice_no = "NAN"
                    create_date = "NAN"
                    check_code = "NAN"
                    invoice_code = "NAN"
                is_invoice = data_dict["is_invoice"]
                billId = data_dict["billId"]
                imageName = data_dict["imageName"]
                applyNum = data_dict["applyNum"]
                department = data_dict["department"]
                query_result = data_dict["query_result"]
                try:
                    code = query_result["code"]
                    description = query_result["description"]
                except:
                    code = "NAN"
                    description = "NAN"

                All_data.append((imageNo, businessType, total, invoice_no, create_date, check_code,
                                 invoice_code, is_invoice, billId, imageName,
                                 applyNum, department, code, description))
            except:
                print("数据格式出错！")
                pass

    return All_data
All_data = get_data(excel_path)
df = pd.DataFrame(All_data, index=None,columns=["imageNo", "businessType", "total","invoice_no", "create_date", "check_code", \
                                                 "invoice_code","is_invoice","billId","imageName",\
                                                 "applyNum","department","code","description"])
df.to_excel('C:\\Users\\Desktop/001.xls')
print("done!")
```

### 特殊json文件格式化

```
最原始数据：
{"41196516":"{\"type\":\"身份证正面\",\"name\":\"徐XX\",\"sex\":\"男\",\"people\":\"汉\",...,"41196243":"{\"error_code\"

处理成如下json文件：(非常不正规)
{"41196516":"{"type":"身份证正面","name":"徐XX","sex":"男","people":"汉","birthday":"19XX年7月XX日","address":"广州市花都区*****号","id_number":"4401***15","issue_authority":"广州市XXX局","validity":"20XX.XX.13-20XX.XX.13","time_cost":{"recognize":348,"preprocess":28},"complete":true,"border_covered":false,"head_covered":false,"head_blurred":false,"gray_image":true,"error_code":0,"error_msg":"OK"}",
"41196243":"{"error_code":40007,"error_msg":"fail to recognize"}",
"41196510":"{"type":"二代身份证","name":"魏XX","sex":"男","people":"汉","birthday":"19XX年9月XX日","address":"江苏省江阴市XXX号","id_number":"320XXX17","time_cost":{"recognize":398,"preprocess":29},"complete":true,"border_covered":false,"head_covered":false,"head_blurred":false,"gray_image":false,"error_code":0,"error_msg":"OK"}",
"41197139":"{"type":"身份证背面","issue_authority":"佛山市XXX分局","validity":"2005.XX.XX-2025.XX.XX","time_cost":{"recognize":464,"preprocess":48},"complete":true,"error_code":0,"error_msg":"OK"}"}

格式化展示：
{"41196516":"{"type":"身份证正面",
			  "name":"徐XX",
			  "sex":"男",
			  "people":"汉",
			  "birthday":"19XX年7月XX日",
			  "address":"广州市花都区*****号",
			  "id_number":"4401***15",
			  "issue_authority":"广州市XXX局",
			  "validity":"20XX.XX.13-20XX.XX.13",
			  "time_cost":{"recognize":348,"preprocess":28},
			  "complete":true,
			  "border_covered":false,
			  "head_covered":false,
			  "head_blurred":false,
			  "gray_image":true,
			  "error_code":0,
			  "error_msg":"OK"}",
"41196243":"{"error_code":40007,"error_msg":"fail to recognize"}",
"41196510":"{"type":"二代身份证",
			 "name":"魏XX",
			 "sex":"男",
			 "people":"汉",
			 "birthday":"19XX年9月XX日",
			 "address":"江苏省江阴市XXX号",
			 "id_number":"320XXX17",
			 "time_cost":{"recognize":398,"preprocess":29},
			 "complete":true,
			 "border_covered":false,
			 "head_covered":false,
			 "head_blurred":false,
			 "gray_image":false,
			 "error_code":0,
			 "error_msg":"OK"}",
"41197139":"{"type":"身份证背面",
		    "issue_authority":"佛山市XXX分局",
			"validity":"2005.XX.XX-2025.XX.XX",
			"time_cost":{"recognize":464,"preprocess":48},
			"complete":true,
			"error_code":0,
			"error_msg":"OK"}"}
```

解析代码如下：

```python
import json
import pandas as pd


data_str = open('D:/XXX/XXX文档/reize_result20181227.txt',"r",encoding="utf-8").read()
data_str0 = data_str.replace("\\","")
print(data_str0)

imgName_list = []
def get_data(data_str0):
    All_data = []
    num = data_str0.count("error_code")                        #统计共有多少个json("error_code"每个json都有)
    for i in range(num):
        imgName = data_str0[1:-1].split("\":\"{")[i][-8:]      #获取ImageID([1:-1]去除最外层括号)
        print("imgName", imgName)
        img_str1 = "{"+data_str0[1:-1].split("\":\"{")[i+1].split("}\",\"")[0]+"}"         #获取整个json

        img_str1 = img_str1.replace("\"}\"}}","\"}") if "\"}\"}" in img_str1 else img_str1 #去除末尾多余的符号
        print("img_str1：", img_str1)
        data_list = json.loads(img_str1)

        #########################################################################
        try:
            type_ = data_list["type"]
        except:
            type_ = "NAN"
        try:
            name = data_list["name"]
        except:
            name = "NAN"
        try:
            sex = data_list["sex"]
        except:
            sex = "NAN"
        try:
            people = data_list["people"]
        except:
            people = "NAN"
        try:

            birthday = data_list["birthday"]
        except:
            birthday = "NAN"
        try:
            address = data_list["address"]
        except:
            address = "NAN"
        try:
            id_number = data_list["id_number"]
        except:
            id_number = "NAN"
        try:
            issue_authority = data_list["issue_authority"]
        except:
            issue_authority = "NAN"
        try:
            validity = data_list["validity"]
        except:
            validity = "NAN"
        try:
            time_cost = data_list["time_cost"]
            recognize = time_cost["recognize"]
            preprocess = time_cost["preprocess"]
        except:
            time_cost = "NAN"
            recognize = "NAN"
            preprocess = "NAN"

        try:
            complete = data_list["complete"]
        except:
            complete = "NAN"
        try:
            border_covered = data_list["border_covered"]
        except:
            border_covered = "NAN"
        try:
            head_covered = data_list["head_covered"]
        except:
            head_covered = "NAN"
        try:

            head_blurred = data_list["head_blurred"]
        except:
            head_blurred = "NAN"
        try:
            gray_image = data_list["gray_image"]
        except:
            gray_image = "NAN"



        error_code = data_list["error_code"]
        error_msg = data_list["error_msg"]

        All_data.append((imgName,type_,name,sex,people,birthday,\
                         address,id_number,issue_authority,validity,\
                         recognize,preprocess,complete,border_covered,\
                         head_covered,head_blurred,gray_image,error_code,error_msg))
    return All_data
All_data = get_data(data_str0)

df = pd.DataFrame(All_data, index=None,columns=["imgName", "type_", "name","sex", "people", "birthday", \
                                                 "address","id_number","issue_authority","validity",\
                                                 "recognize","preprocess","complete","border_covered",\
                                                 "head_covered","head_blurred","gray_image","error_code","error_msg"])
df.to_excel('D:/XXX/XXX文档/reize_result20181227.xls')
```

复杂json解析
- 报错1:`json.decoder.JSONDecodeError: Expecting property name enclosed in double quotes: line 1 column 3 (char 2)`

原因在于：字符串里用单引号来标识字符。

解决办法如下：将字符串里的**单引号**替换成**双引号**
### 写json文件

```python
import json
import os


def get_img(file_path):
	img_path = []
	for path,dirname,filenames in os.walk(file_path):
		for filename in filenames:
			img_path.append(path+"/"+filename)
	return img_path
	
def json_str(file_path):
	dict_str = []
	img_path = get_img(file_path)
	for i in img_path:
		dict_str.append({"ImageName":"/image/bus/"+i,"id":"8abs63twy2001"})
	return dict_str
	
file_path = "./image/ocr"
dict_str = json_str(file_path)
json_str = json.dumps(dict_str)

with open("./dict_str_to_json.json","w") as json_file:
	json_file.write(json_str)
	print("done!")
```

以上纯属个人原创仅供新手json解析参考，老手请忽略，谢谢




