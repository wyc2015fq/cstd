# PADS如何导出GERBER文件以及DXF文件 - xqhrs232的专栏 - CSDN博客
2014年11月27日 16:45:30[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2881
原文地址::[http://jeanscheng.blog.163.com/blog/static/11253401120099178121958/](http://jeanscheng.blog.163.com/blog/static/11253401120099178121958/)
相关文章
1、如何将POWER PCB资料导出DXF给结构工程师确认----[http://wenku.baidu.com/link?url=esPZnlzuq0pSD9LUqJ3Rj3PSEvirz0HPAa90tK_JhZKbiPWTNQHcL9Ffdea00pe_YulwQBGEFA56PPwgUWbuDlSLMJx-w6x86PtfKabRji_](http://wenku.baidu.com/link?url=esPZnlzuq0pSD9LUqJ3Rj3PSEvirz0HPAa90tK_JhZKbiPWTNQHcL9Ffdea00pe_YulwQBGEFA56PPwgUWbuDlSLMJx-w6x86PtfKabRji_)
一、导出网络表至PCB版图。
首先，在电路图状态下，按上方的功能按钮，PROJECT MANAGER来到工程文件夹状态栏，点击DRC，对电路图进行错误检验，确保电路图无错后，进入TOOLS菜单，选择CREAT
 NETLIST，选择OTHER，在FORMATTER栏选择PADS2K.DLL，将默认的输出后缀名改为.ASC，保存，同时可以将VIEW
 OUTPUT打勾，可以直观看到输出的二进制文件的正确性。
![PADS如何导出GERBER文件以及DXF文件 - jeanscheng - 携手星辰](http://img.bimg.126.net/photo/uxXjxZDDrJ4-ZH9m8EkLGQ==/2592384535506127929.jpg)
新建PCB文件，点击FILE/IMPORT，导入ASC文件，将各个器件及其连接线导入到PCB中，此时可以用元器件推挤工具，TOOLS/DISPERSE
 COMPONENTS对元器件进行平铺放置。
[](http://img.bimg.126.net/photo/UjvOPflx5T_4cLyqfOlLQQ==/2592384535506127930.jpg)
二、导出CAMGERBER文件。
注意，导出CAM文件时，PCB的单位状态必须为mm，这样才能在CAM350里面得到正确的显示。FILE/CAM，进入以下界面，在这个界面可以看到每一个要导出的层次及其描述。点击PREVIEW，对每一层进行预览，在CAM下面的选项选择CREAT，并把所需导出的文件夹建立在你所需要的位置，点击RUN，所有的文件被导出在该文件夹。在这里需要注意的是，对NC
 DRILL而言，有一个参数必须为2，4，如图中所示，为CAM的标准参数，否则DRILL输出将不正常。
![PADS如何导出GERBER文件以及DXF文件 - jeanscheng - 携手星辰](http://img.bimg.126.net/photo/Hr4qH4xp1TlhS7ir3fWprQ==/2592384535506127931.jpg)[](http://img.bimg.126.net/photo/1MKkiDSN6qcDqkkgECLXrg==/2592384535506127924.jpg)
![PADS如何导出GERBER文件以及DXF文件 - jeanscheng - 携手星辰](http://img.bimg.126.net/photo/1MKkiDSN6qcDqkkgECLXrg==/2592384535506127924.jpg)
![PADS如何导出GERBER文件以及DXF文件 - jeanscheng - 携手星辰](http://img.bimg.126.net/photo/XhvMTS1RrwC2hJjMKaOlyg==/2592384535506127923.jpg)
在CAM中选择AUTOIMPORT，自动导入上面操作所放置的文件夹，将显示出PCB板的CAM图纸。也可以一层一层地点击观看，确保准确无误。
[](http://img.bimg.126.net/photo/uLLuUV_L5OemoyBoIKiHxg==/2592384535506127932.jpg)
三、导出CAD支持的DXF文件。
导出CAD文件是为了工程人员对PCB的外壳和器件的放置位置进行确认，单位必须为mm。直接选择导出文件，后缀名改为DXF，该文件就能被CAD打开，并进行观看。
[](http://img.bimg.126.net/photo/PY_hUAjVBah3_AOral9R9A==/2592384535506127933.jpg)
![PADS如何导出GERBER文件以及DXF文件 - jeanscheng - 携手星辰](http://img.bimg.126.net/photo/UxU9uZAn_rImoKhH21bKsQ==/2592384535506127934.jpg)![PADS如何导出GERBER文件以及DXF文件 - jeanscheng - 携手星辰](http://img.bimg.126.net/photo/d1BPv7D-NBm2E-hUCTEK-w==/2592384535506127936.jpg)
四、原理图与PCB的比较，及PCB之间的比较。
点击COMPARE ECO，弹出比较菜单，在此时，可以选择对两个不同的PCB文件进行比较，对原理图的比较必须先从原理图导出ASC，然后生成一个新的PCB文件，用该文件对先文件比较也就等同于PCB至于SCH的比较了。
![PADS如何导出GERBER文件以及DXF文件 - jeanscheng - 携手星辰](http://img.bimg.126.net/photo/usn87-OEdg3OQKX7DhWiMw==/2592384535506127920.jpg)
五、ECO模式下更改元件的标识符。如C2改为C21。
点击U1-U2就可以对某一个文件名进行替换，然后再在原理图中替换完，进行比较确认无误。
[](http://img.bimg.126.net/photo/usn87-OEdg3OQKX7DhWiMw==/2592384535506127920.jpg)![PADS如何导出GERBER文件以及DXF文件 - jeanscheng - 携手星辰](http://img.bimg.126.net/photo/qRJ2kxctdbvzKVX9th5JZw==/2592384535506127921.jpg)![PADS如何导出GERBER文件以及DXF文件 - jeanscheng - 携手星辰](http://img.bimg.126.net/photo/hwFLcsYUjsAD1w9oExig5g==/2592384535506127922.jpg)
//==================================================================================================
备注::
1>DXF文件一般结构工程师需要这个文件，DXF文件用CAM350软件也可以打开的。
2>外发DXF文件比发钢网文件安全，DXF可以设置不显示线路什么的

