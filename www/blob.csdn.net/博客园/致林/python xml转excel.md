
# python xml转excel - 致林 - 博客园






# [python xml转excel](https://www.cnblogs.com/bincoding/p/8031125.html)
因公司业务需要，临时写的一段代码:
fromxml.etreeimportElementTreeimportxlrdimportxlwt\#读取xmldefread_xml(path):print("read_xml start...")
 root=ElementTree.fromstring(path)\#读取row子节点rows = root.findall('ROW')returnrowsdefopen_excel(path):try:
  data=xlrd.open_workbook(path)returndataexceptException as ex:returnexdefwrite_excel(path):
 workbook=xlwt.Workbook()defread_excel(path,secondtype,by_index=0):
 data=open_excel(path)
 table=data.sheets()[by_index]print(table.nrows)\#xml文件workbook =xlwt.Workbook()\#添加xml工作表worksheet = workbook.add_sheet('警情类型')\#print(table.row_values(1)[1])foriinrange(table.nrows):
  word= table.row_values(i)[2]if(wordinsecondtype):\#写入excelworksheet.write(i,1,word)print("word:",word)
workbook.save("temp.xls");
xml_path="test.xml"excel_path="ICCTranslate.xlsx"\#指定编码uft-8，否则会报错rows = read_xml(open(xml_path,encoding='utf-8').read())
secondtype=[]foriteminrows:iflen(item[1].text) < 10:
  secondtype.append(item[2].text)\#print(secondtype)read_excel(excel_path, secondtype)





