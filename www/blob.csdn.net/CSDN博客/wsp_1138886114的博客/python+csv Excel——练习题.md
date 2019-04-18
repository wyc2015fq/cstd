# python+csv/Excel——练习题 - wsp_1138886114的博客 - CSDN博客





2018年05月20日 11:05:27[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：362
所属专栏：[python基础及相关](https://blog.csdn.net/column/details/23328.html)









### 一、CSV文件操作

#####     读操作：csv.read   

```python
import csv
with open("/路径/文件名.csv","r") as csvfile    #固定写法，使用open()方法，避免还要关闭file，'r'表示读操作
    read=csv.reader(csvfile)                #使用csv.reader()方法，读取打开的文件，返回为可迭代类型
    for i in read:
    print i
```

#####     写操作：csv.writer

```python
import csv
with open("/路径/文件名.csv","w") as csvfile    #'w'表示写操作，有则修改，无则新建
    write=csv.writer(csvfile)
    write.writerow(data)        #写入一行操作，data为可迭代类型，如果为字符串，则单个字符为一个元素
    write.writerows(data)        #写入多行操作，data中一个元素为一行
```

#####     添加数据：csv.read--write


```python
方法1：
    import csv
    list1=[]
    with open('data.csv','r')as csvfile1:        #首先---读取数据
        read=csv.reader(csvfile1)
        for i in read:
            list1.append(i)
    with open('data.csv','w')as csvfile2:        #然后----写入之前读取的数据
        writer=csv.writer(csvfile2)
        date=[list1[0],                          #在第五行开始添加数据
              list1[1],
              list1[2],
              list1[3],
              ['Jack','104']]
        writer.writerows(date)
    with open('data.csv','r')as csvfile3:        #输出添加后的数据
        read2=csv.reader(csvfile3)
        for i in read2:
            print(i)
```


```python
方法2：
    #读取文件    
    f = open("./data.csv")              #f是一个可迭代对象
    result = []                         #用于存放每一行的数据
    for line in f:
        row = line.split(",")           #将每一行数据用","分割,保存到一个列表中
        data = []
        for word in row:
            data.append(word.strip())    #(word.strip())去除每一行数据中多余的空格
        result.append(data)
    f.close()
    print(result)

    #当前文件夹下创建一个output.csv文件，将数据写入到其中
    with open("output.csv","w") as csvFile:
        result.append(["Jack","104"])
        for row in result: 
            csvFile.write(",".join(row)+"\n") #将一个列表中的数据用","拼接成一个字符串
        csvFile.close()
```

```python
方法3：                                         #使用DictReader方法读写  
    import csv
    def getData(path):                          #path读取文件的路径
        result = []
        with open(path) as file:
            reader = csv.DictReader(file)
            for row in reader:                  # row.keys()  ["   name","   stuNo"]
                newRow = {}                     # row.values()  ["ZhangSan","101"]
                for key,value in zip(row.keys(),row.values()):    #获取列名，列值并去空格
                    newRow[key.strip()] = value.strip()
                result.append(newRow)
        return result
    data = getData("./data.csv")
    with open("./output.csv","w") as csvFile:
        data = getData("./data.csv")
        data.append({"name":"Jack","stuNo":"104"})
        
        fields = ["name","stuNo"]                            #列标题
        writer = csv.DictWriter(csvFile,fieldnames=fields)    #用csv模块的dictwrite方法将字典写入到csv文件
        writer.writeheader()                                #将csv文件的第一行（即列名写入到csv文件）
        for row in data:                                    #遍历字典写入到csv文件中
            writer.writerow(row)
        csvFile.close()
```

### 二、excel的操作

####     1：利用 xlwt、xlrd 进行操作

```python
xlrd 读取文件:    
        import xlrd
        myWorkbook=xlrd.open_workbook('/路径/文件名.xlsx')    #获取excel工作簿
        mySheets=myWorkbook.sheets()                    #其次获取所有的工作表
        mySheet1=mySheets[0]                            #获取第一个工作表
        mySheet1=myWorkbook.sheet_by_index(0)           #同上
        mySheet1=myWorkbook.sheet_by_name('sheet1')     #同上
            
        nrows=mySheet1.nrows            #获得所有的行数，int类型
        ncols=mySheet1.ncols            #获得所有的列数，int类型
        myRowValue=mySheet1.row_value(i)        #获得i列所有行的值
        myCell=mySheet1.cell(i,j)               #获取i行，j列单元格
                myCell.value                    #通过单元格获取值
        mySheet1.cell_value(i,j)                #直接获取表sheet1的i行，j列单元格的值
        mySheet1.cell_value(i,j)="new_value"    #直接赋值修改单元格的值：
        
xlwt 写入文件:
        import xlwt
        new_wk=xlwt.Workbook()                   #创建工作簿
        new_sheet1=new_wk.add_sheet('sheetname') #创建名为sheetname的工作表
        sheet.write(i,j,'content')               #在i行，j列写入内容：content，i，j从0开始
        new_wk.save('/路径/文件名.xlsx')          #使用xlwt写入操作后，需要保存
```

#### 2：xlutils 结合 xlrd 进行操作

```python
读、写文件：               
    import xlrd
    from xlutils.copy import copy
    workBook=xlrd.open_workbook('/路径/文件名.xlsx')        #参考xlrd读操作
    new_workBook=copy(workBook)        #使用xlutils中copy()方法复制一份工作簿
    ws=new_workBook.get_sheet(index)    #get_sheet()方法，在仅导入xlrd时不可用，仅对使用copy()之后的工作簿可用
        
    ws.write(i,j,'content')                    #在i行，j列写入内容：content，i，j从0开始，可覆盖
    new_workBook.save('/路径/文件名.xlsx')
```

#### 3：使用 openpyxl 操作

#####     写入单个数据

```python
读、写文件：
    from openpyxl import Workbook
    from openpyxl import load_workbook
    from openpyxl.writer.excel import ExcelWriter
    workBook=load_workbook('/路径/文件名.xlsx')
    sheetNames=workBook.sheetnames                    #获取工作簿所有工作表名称，返回列表
    sheet1=myWorkbook.get_sheet_by_name('Sheet1')     #获取myWorkbook的表单Sheet1
    sheet1.cell(i,j).value                            #获取i行，j列单元格的值，注意：此时单元格索引有变，从1开始
    sheet1['C3']='content'                            #为C3单元格写入值，注意：此时单元格索引有变，分别从A，1开始
```

```python
sheet1.cell(i,j).value  = 'values'
```

#####     批量写入数据

### 三、使用python函数来读写 excel 文件

在这里我封装了一个类，方便以后使用

```python
from openpyxl import Workbook                   #导入openpyxl模块的workbook模块，用于写入.xlsx
    from openpyxl import load_workbook
    from openpyxl.writer.excel import ExcelWriter
封装类： class saveExcelData(object):                #定义类（用于封装函数，方便以后使用）
        def __init__(self,dataList,sheetTitle,fileName):#定义构造函数分别传入数据，工作表标题，保存文件名称
            self.dataList=dataList
            self.sheetTitle=sheetTitle
            self.fileName=fileName
        def saveData(self):                # 定义保存操作
            workbook = Workbook()           # 1、新建一个工作簿，实例化对象
            sheet = workbook.active             # 2、激活一个工作表
            sheet.title = self.sheetTitle           # 3、给激活的工作表命名
            for i in self.dataList:                # dataList传入的应为iterable元素组成的iterable容器类型
                sheet.append(i)
            workbook.save(self.fileName)
```

```python
使用类：
    from day12_csv_xls.save_class_excel import SavaExcelData
    #     (上层文件夹)     （文件名）             （文件名里的类名）

    data = list(range(20))
    new_data = []
    for i in range(4):
        new_data.append(data[len(data)//4*i:len(data)//4*(i+1)])
        #写入数据顺序为 data[0:4],data[5:10],data[10:15],data[15:20]
        print(len(new_data))
    new1 = SavaExcelData(new_data, 'new_title', 'new_file.xlsx')
    new1.saveData()
```

### 四、综合练习

对Excel表中的行进行筛选并排序，（依照其中一列的数据对每行进行排序）写入到新表单中

```python
方法1：
        import  xlrd
	import xlwt

	old_wbk = xlrd.open_workbook("rank.xlsx")
	rank = old_wbk.sheets()[0]
	nrows = rank.nrows					#获取工作表数据的行数

	filterData = []     					#保存筛选后的数据
	for row in range(nrows):
		myRowValue = rank.row_values(row)		#从表中获取行数据
		if row is 0 or myRowValue[2]>5.0:  		#判断行数据，如果是第一行表头或者是points分数大于5.0的数据行保留
			filterData.append(myRowValue)

	headers = filterData[0]					#获取表头并写入到新的Excel表中
	new_wbk = xlwt.Workbook()				#创建一个新的工作簿
	new_rank = new_wbk.add_sheet("new_rank")	        #添加表单

	for col in range(len(headers)):
		new_rank.write(0,col,headers[col])
	del filterData[0]					#将表头从filterData中删除
	points = []						#将points从筛选后的数据中分离出来
									
	for item in filterData:
		points.append(item[2])
	points.sort(reverse=True)				#对分数进行排序

	resutl = []  #存放最终排序后的结果
	#将筛选后的数据与排序后的分数进行比较，依次添加到新的列表中

	for point in points:
		for row in filterData:
			if row[2] == point:
				resutl.append(row)
	print(resutl)
	for row in range(0,len(resutl)):
		for col in range(len(headers)):
			new_rank.write(row+1,col,resutl[row][col])
	new_wbk.save("new_rank.xls")

方法2--通用方法
        import xlrd
        import xlwt
        wb=xlrd.open_workbook("./素材/two/rank.xlsx")
        mysheet=wb.sheet_by_index(0)
        nrows=mysheet.nrows #获取所有的行数
        # print(nrows)
        all_data=[]
        for i in range(nrows):
            row_values=mysheet.row_values(i)    #将所有数据根据条件points>5帅选后保存
            # print(row_values[-1])
            if i>0 and row_values[-1] > 5:
                all_data.append(row_values)
            elif i==0:  #跳过表头
                all_data.append(row_values)
            else:
                continue
        # print(all_data)                        #读取数据
        head=all_data[:1]
        data=all_data[1:]
        for i in range(len(data)):
            for j in range(i):
                if data[i][-1]>data[j][-1]:      #对某行的指定列value值进行对比
                    data[i],data[j]=data[j],data[i]
        print(data)                              #排序后
        wb=xlwt.Workbook()
        new_sheet=wb.add_sheet("rank")
        for i in range(len(head)):
            new_sheet.write(0,i,head[i])
        for j in range(len(data)):
            for k in range(len(data[j])):
                new_sheet.write(j+1, k, data[j][k])    #写入新表单（更新）
        wb.save("output.xls")
```






