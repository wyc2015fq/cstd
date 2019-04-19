# Cadstar格式导入功能 - xqhrs232的专栏 - CSDN博客
2015年03月12日 23:47:42[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：875
原文地址::[http://wiki.altium.com/pages/viewpage.action?pageId=8751434](http://wiki.altium.com/pages/viewpage.action?pageId=8751434)
相关文章
1、CADSTAR V6.0培训手册----[http://wenku.baidu.com/link?url=Z7KP0s-_MWTF24UE4FCSUyo8mROQv5glTykxGrieYYROzjqkCKif5KW25UDoKmxSbxLfWxsa475-QAKtBrh-S9C73r2vcjRUNDRPvjWmYym](http://wenku.baidu.com/link?url=Z7KP0s-_MWTF24UE4FCSUyo8mROQv5glTykxGrieYYROzjqkCKif5KW25UDoKmxSbxLfWxsa475-QAKtBrh-S9C73r2vcjRUNDRPvjWmYym)
2、CADSTAR_V12中文版教程----[http://wenku.baidu.com/link?url=4ZXXyxZ6C5t4ebRqsKRJJgHkkCjYXokrFiXwJzlsrj34Q4-_otJCGKn59cTLUlWGmakzOtCssRlPmwZasAdogRh-POgae_KoQDNByQ0SPYS](http://wenku.baidu.com/link?url=4ZXXyxZ6C5t4ebRqsKRJJgHkkCjYXokrFiXwJzlsrj34Q4-_otJCGKn59cTLUlWGmakzOtCssRlPmwZasAdogRh-POgae_KoQDNByQ0SPYS)
3、[Zuken
 CADSTAR 13.0 PCB设计软件](http://www.0daydown.com/06/15509.html)----[http://www.0daydown.com/06/15509.html](http://www.0daydown.com/06/15509.html)
CADSTAR格式导入功能将帮助用户从CADSTAR设计和库文件到Altium Designer文件的格式转换.
## 支持的版本和文件格式
Altium Designer内建CADSTAR导入工具支持CADSTAR9或10版本中输出的文件格式. 同时，还不支持二进制CADSTAR格式文件。用户需要将二进制CADSTAR格式文件转换成Altium Designer内识别的CADSTAR存档文件类型。CADSTAR存档文件通常采用 **.cpa** 或 **.csa** 作为文件的扩展名.
- PCB设计文件
- PCB元器件库文件
- 多部件库和原理图符号库文件
下表介绍了Altium Designer内建CADSTAR导入工具如何将待导入文件从CADSTAR二进制文件格式转换成存档文件格式；同时，映射到相关Altium Designer输出文件类型。
|**CADSTAR文件类型**|**导出成CADSTAR存档文件**|**Altium Designer输出文件**|
|----|----|----|
|PCB设计文件 (.pcb)|Use CADSTAR File->Export to convert the binary pcb design (.pcb) to CADSTAR PCB archive (.cpa)|Altium Design PCB document (.pcbdoc)|
|PCB库文件(.lib)|Use the archive tool in CADSTAR Libraries->PCB Components... to convert the binary pcb library (.lib) to CADSTAR PCB archive (.cpa)|Altium Designer PCB library (.pcblib)|
|多部件库文件 (.lib) 和原理图符号库文件(*.lib)|The part library (.lib) file is already in ASCII file format. You do not need to do any conversion on the part library. Use the archive tool in CADSTAR Libraries->Schematic Symbols to convert the binary symbol library (.lib) to CADSTAR schematic archive (.csa)|The importer uses both the parts.lib and the symbol schematic archive (.csa) to output an Altium Designer schematic library (.schlib)|
## 导入原理图符号库文件
The importer needs at least 2 files to import a schematic library. The first one is the CADSTAR part library. You can locate the part library file in the CADSTAR Part Library Manager dialog (Libraries -> Parts...):
![](http://wiki.altium.com/download/attachments/8751434/CADSTAR_Library_Part_Manager.jpg?version=1&modificationDate=1230666830603)
The part item in the part library references a symbol where the definition is in the separate file. These type of files are called Symbol Library files. Secondly, the importer requires the Symbol Library files in the CADSTAR Schematic archive (.csa) file format.
 You can convert the symbol library files to .csa file by using the CADSTAR Symbol Library Manager (Libraries -> Schematic Symbols...):
![](http://wiki.altium.com/download/attachments/8751434/CADSTAR_Library_Symbol_Manager.jpg?version=1&modificationDate=1230666848838)
For example, you have a part library file called "parts.lib" that uses the symbol definitions in another file called "symbol.lib". You can importYou need convert the symbol.lib file to symbol.csa file using the archive utility in the CADSTAR Symbol Library
 Manager. The importer will translate the parts.lib and symbol.csa files to an Altium Designer parts.schlib. You can input multiple "symbol.csa" files that the parts.lib references. Under the hood, a part item definition in the parts.lib file and the symbol
 definition in the symbol.csa file are equivalent to an Altium Designer schematic library component definition. 
## 导入PCB库文件
The CADSTAR PCB component is equivalent to an Altium Designer PCB footprint.  Given a CADSTAR pcb symbol file (*.CPA), the CADSTAR importer will translate into an Altium Designer pcb library (*.PcbLib).  You can archive the CADSTAR pcb component library (.lib)
 file to a .cpa file format using the CADSTAR PCB Component Library Manager (Libraries -> PCB Component...):
![](http://wiki.altium.com/download/attachments/8751434/CADSTAR_Library_PCB_Manager.jpg?version=1&modificationDate=1230666841245)
It does not strictly require a CADSTAR part library file to import a PCB library.  However, if the pcb component is linked to a schematic symbol via a part definition, it is a good practice to supply the part library file when importing pcb library.  This way,
 the CADSTAR import can map the pin name from schematic component to pcb footprint correctly by using the pin name in the part definition. 
## 导入PCB版图设计文件
The CADSTAR PCB layout file is equivalent to an Altium Designer PCB document.  Given a CADSTAR pcb layout file (*.CPA), the CADSTAR importer will translate into an Altium Designer pcb document (.PcbDoc).  If the pcb layout is in binary file format, the user
 can archive it to *.CPA file format using CADSTAR file export utility.  
### 运用CADSTAR导入向导工具
- Open the Altium Designer File Import Wizard (File » Import).  Click Next.
- Select CADSTAR Designs and Libraries in the list.  Click Next. The CADSTAR importer wizard has 2 pages for adding CADSTAR files to import: CADSTAR Design Files and CADSTAR Library Files.
- In the page with the header 'Importing CADSTAR Design Files', you can add/remove CADSTAR PCB archive files (.cpa) to the list. The CADSTAR pcb design (layout) archive (.cpa) file will be
 translated to Altium Designer pcb document (.pcbdoc).
- In the page with the header 'Importing CADSTAR Library Files', you can add/remove supported CADSTAR library files to the list. The CADSTAR part library (.LIB) and symbol archive (.csa) files
 will be translated to Altium Designer Schematic Library (.schlib). The CADSTAR pcb archive file (.cpa) will be translated to Altium Designer PCB Library (.pcbdoc).
- If the input files has file types of PCB library or layout files, the layer mapping dialog will display the default layer mapping for each PCB library/design.  You can edit the layer mapping
 setting. If you like the current layer setting, you can save it to a file. Later on, if you want to use the same later setting, you can load it from the file to apply it to the layer mapping setting.
- The wizard will display the output project structure. Each project will contains the output documents. The project and document structure is generated by the input files that you add earlier.
 The wizard picks the default output path based on the input paths from the files you added. You can change the output path in the 'Output Directory' edit box.
- The next step displays the translation status while the importer is working.
- Finish.  The output project/file structure will display in the Altium Designer workspace panel. !
- 

