# XML表示的数据库数据 - weixin_33985507的博客 - CSDN博客
2008年07月23日 12:28:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
[http://support.microsoft.com/kb/252767/zh-cn](http://support.microsoft.com/kb/252767/zh-cn)
## 概要
当您想记录的一个 XML 表示可用于新 ADODB.Stream 对象检索与 XML 字符串。 方法来检索 XML 字符串由记录并不需要使用保存硬盘上保存 ADO 2.5 中。
![](http://support.microsoft.com/library/images/support/en-us/uparrow.gif)回到顶端
## 更多信息
要检索 XML for 记录, 请使用以下代码。 一定要有对 " MicrosoftActiveX 数据对象 2.5 库 " 引用。 该代码假定记录是打开和填充。 
```
```
```
Dim oStream As ADODB.Stream
Set oStream = New ADODB.Stream
oRecordset.Save oStream, adPersistXML
Dim sXML As String
sXML = oStream.ReadText
oStream.Close
Set oStream = Nothing
```
```
```
分析它与 XSL 转换到 ASP 页, 中写出或例如, 可使用您 sXML 字符串。
生成 XML 方法与以下相似。 这是 SQLServer 中 Pubs 数据库中的 Authors 表示例:
```
```
```
<xml xmlns:s='uuid:BDC6E3F0-6DA3-11d1-A2A3-00AA00C14882'
xmlns:dt='uuid:C2F41010-65B3-11d1-A29F-00AA00C14882'
xmlns:rs='urn:schemas-microsoft-com:rowset'
xmlns:z='#RowsetSchema'>
<s:Schema id='RowsetSchema'>
<s:ElementType name='row' content='eltOnly' rs:CommandTimeout='30'>
<s:AttributeType name='au_id' rs:number='1' rs:writeunknown='true'>
<s:datatype dt:type='string' rs:dbtype='str' dt:maxLength='11'
rs:maybenull='false'/>
</s:AttributeType>
<s:AttributeType name='au_lname' rs:number='2' rs:writeunknown='true'>
<s:datatype dt:type='string' rs:dbtype='str' dt:maxLength='40'
rs:maybenull='false'/>
</s:AttributeType>
<s:AttributeType name='au_fname' rs:number='3' rs:writeunknown='true'>
<s:datatype dt:type='string' rs:dbtype='str' dt:maxLength='20'
rs:maybenull='false'/>
</s:AttributeType>
<s:AttributeType name='phone' rs:number='4' rs:writeunknown='true'>
<s:datatype dt:type='string' rs:dbtype='str' dt:maxLength='12'
rs:fixedlength='true' rs:maybenull='false'/>
</s:AttributeType>
<s:AttributeType name='address' rs:number='5' rs:nullable='true'
rs:writeunknown='true'>
<s:datatype dt:type='string' rs:dbtype='str' dt:maxLength='40'/>
</s:AttributeType>
<s:AttributeType name='city' rs:number='6' rs:nullable='true'
rs:writeunknown='true'>
<s:datatype dt:type='string' rs:dbtype='str' dt:maxLength='20'/>
</s:AttributeType>
<s:AttributeType name='state' rs:number='7' rs:nullable='true'
rs:writeunknown='true'>
<s:datatype dt:type='string' rs:dbtype='str' dt:maxLength='2'
rs:fixedlength='true'/>
</s:AttributeType>
<s:AttributeType name='zip' rs:number='8' rs:nullable='true'
rs:writeunknown='true'>
<s:datatype dt:type='string' rs:dbtype='str' dt:maxLength='5'
rs:fixedlength='true'/>
</s:AttributeType>
<s:AttributeType name='contract' rs:number='9' rs:writeunknown='true'>
<s:datatype dt:type='boolean' dt:maxLength='2' rs:fixedlength='true'
rs:maybenull='false'/>
</s:AttributeType>
<s:extends type='rs:rowbase'/>
</s:ElementType>
</s:Schema>
<rs:data>
<z:row au_id='172-32-1176' au_lname='White' au_fname='Johnson'
phone='408 496-7223' address='10932 Bigge Rd.' city='Menlo Park'
state='CA' zip='94025' contract='True'/>
<z:row au_id='213-46-8915' au_lname='Green' au_fname='Marjorie'
phone='415 986-7020' address='309 63rd St. #411' city='Oakland'
state='CA' zip='94618' contract='True'/>
<z:row au_id='238-95-7766' au_lname='Carson' au_fname='Cheryl'
phone='415 548-7723' address='589 Darwin Ln.' city='Berkeley'
state='CA' zip='94705' contract='True'/>
<z:row au_id='267-41-2394' au_lname='O'Leary' au_fname='Michael'
phone='408 286-2428' address='22 Cleveland Av. #14' city='San Jose'
state='CA' zip='95128' contract='True'/>
<z:row au_id='274-80-9391' au_lname='Straight' au_fname='Dean'
phone='415 834-2919' address='5420 College Av.' city='Oakland'
state='CA' zip='94609' contract='True'/>
<z:row au_id='341-22-1782' au_lname='Smith' au_fname='Meander'
phone='913 843-0462' address='10 Mississippi Dr.' city='Lawrence'
state='KS' zip='66044' contract='False'/>
<z:row au_id='409-56-7008' au_lname='Bennet' au_fname='Abraham'
phone='415 658-9932' address='6223 Bateman St.' city='Berkeley'
state='CA' zip='94705' contract='True'/>
<z:row au_id='427-17-2319' au_lname='Dull' au_fname='Ann'
phone='415 836-7128' address='3410 Blonde St.' city='Palo Alto'
state='CA' zip='94301' contract='True'/>
<z:row au_id='472-27-2349' au_lname='Gringlesby' au_fname='Burt'
phone='707 938-6445' address='PO Box 792' city='Covelo'
state='CA' zip='95428' contract='True'/>
<z:row au_id='486-29-1786' au_lname='Locksley' au_fname='Charlene'
phone='415 585-4620' address='18 Broadway Av.' city='San Francisco'
state='CA' zip='94130' contract='True'/>
<z:row au_id='527-72-3246' au_lname='Greene' au_fname='Morningstar'
phone='615 297-2723' address='22 Graybar House Rd.' city='Nashville'
state='TN' zip='37215' contract='False'/>
<z:row au_id='648-92-1872' au_lname='Blotchet-Halls'
au_fname='Reginald' phone='503 745-6402' address='55 Hillsdale Bl.'
city='Corvallis' state='OR' zip='97330' contract='True'/>
<z:row au_id='672-71-3249' au_lname='Yokomoto' au_fname='Akiko'
phone='415 935-4228' address='3 Silver Ct.' city='Walnut Creek'
state='CA' zip='94595' contract='True'/>
<z:row au_id='712-45-1867' au_lname='del Castillo' au_fname='Innes'
phone='615 996-8275' address='2286 Cram Pl. #86' city='Ann Arbor'
state='MI' zip='48105' contract='True'/>
<z:row au_id='722-51-5454' au_lname='DeFrance' au_fname='Michel'
phone='219 547-9982' address='3 Balding Pl.' city='Gary'
state='IN' zip='46403' contract='True'/>
<z:row au_id='724-08-9931' au_lname='Stringer' au_fname='Dirk'
phone='415 843-2991' address='5420 Telegraph Av.' city='Oakland'
state='CA' zip='94609' contract='False'/>
<z:row au_id='724-80-9391' au_lname='MacFeather' au_fname='Stearns'
phone='415 354-7128' address='44 Upland Hts.' city='Oakland'
state='CA' zip='94612' contract='True'/>
<z:row au_id='756-30-7391' au_lname='Karsen' au_fname='Livia'
phone='415 534-9219' address='5720 McAuley St.' city='Oakland'
state='CA' zip='94609' contract='True'/>
<z:row au_id='807-91-6654' au_lname='Panteley' au_fname='Sylvia'
phone='301 946-8853' address='1956 Arlington Pl.' city='Rockville'
state='MD' zip='20853' contract='True'/>
<z:row au_id='846-92-7186' au_lname='Hunter' au_fname='Sheryl'
phone='415 836-7128' address='3410 Blonde St.' city='Palo Alto'
state='CA' zip='94301' contract='True'/>
<z:row au_id='893-72-1158' au_lname='McBadden' au_fname='Heather'
phone='707 448-4982' address='301 Putnam' city='Vacaville'
state='CA' zip='95688' contract='False'/>
<z:row au_id='899-46-2035' au_lname='Ringer' au_fname='Anne'
phone='801 826-0752' address='67 Seventh Av.' city='Salt Lake City'
state='UT' zip='84152' contract='True'/>
<z:row au_id='998-72-3567' au_lname='Ringer' au_fname='Albert'
phone='801 826-0752' address='67 Seventh Av.' city='Salt Lake City'
state='UT' zip='84152' contract='True'/>
</rs:data>
</xml>
```
```
```
检查引用下面有关详细信息。
![](http://support.microsoft.com/library/images/support/en-us/uparrow.gif)回到顶端
## 参考
可在找到详细使用率方案 [MSDN information on Streams and persistence](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/ado270/htm/pg_ado_records_streams_1.asp) (http://msdn.microsoft.com/library/default.asp?url=/library/en-us/ado270/htm/pg_ado_records_streams_1.asp) 从 [Records And Streams](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/ado270/htm/pg_ado_records_streams.asp) (http://msdn.microsoft.com/library/default.asp?url=/library/en-us/ado270/htm/pg_ado_records_streams.asp) 文档。
如果您有兴趣 XML 检查 [MSDN XML Developer Center](http://msdn.microsoft.com/xml/default.asp) (http://msdn.microsoft.com/xml/default.asp) 有关深入和 - beginner 到高级 - 信息。
需要看 [http://www.biztalk.org/](http://www.biztalk.org/) (http://www.biztalk.org/) 站点以获取 BizTalk 倡议上详细信息。
还要检查 [http://msdn.microsoft.com/dataaccess](http://msdn.microsoft.com/dataaccess) (http://msdn.microsoft.com/dataaccess) 有关更多信息有关 MDAC 和 ADO。
