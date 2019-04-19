# FreeTextBox3.3.0使用总结 - 左直拳的马桶_日用桶 - CSDN博客
2011年12月26日 18:55:25[左直拳](https://me.csdn.net/leftfist)阅读数：4207标签：[html																[mozilla																[image																[asp.net																[upload																[浏览器](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=upload&t=blog)](https://so.csdn.net/so/search/s.do?q=asp.net&t=blog)](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=mozilla&t=blog)](https://so.csdn.net/so/search/s.do?q=html&t=blog)
个人分类：[Web																[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)](https://blog.csdn.net/leftfist/article/category/674421)
他妈的！
为了使用这个FreeTextBox，着实折腾了不少时间。
其实我们一直都有用这个东西，不过版本很低，并且是其他同事的代码，我自己不怎么接触，不熟；现在跑到源站点上下载了一个3.3.0。
总结有几大问题：
0、参数配置问题
```
<FTB:FreeTextBox id="txtContent" OnSaveClick="btnSubmit_Click" 
			toolbarlayout="ParagraphMenu,FontFacesMenu,FontSizesMenu,FontForeColorsMenu,FontBackColorsMenu|Bold,Italic,Underline,Strikethrough,Superscript,Subscript,RemoveFormat|JustifyLeft,JustifyRight,JustifyCenter,JustifyFull;BulletedList,NumberedList,Indent,Outdent;CreateLink,Unlink,InsertImage|InsertRule,InsertDate,InsertTime|InsertTable,EditTable;InsertTableRowAfter,InsertTableRowBefore,DeleteTableRow;InsertTableColumnAfter,InsertTableColumnBefore,DeleteTableColumn|InsertDiv,EditStyle,InsertImageFromGallery,Preview,SelectAll,WordClean,NetSpell|Cut,Copy,Paste,Delete;Undo,Redo,Print,Save"
			Language="zh-CN"
			runat="Server"
			Width="95%"
			Height="400px"
			SupportFolder="~/news/ftb/"
	        JavaScriptLocation="ExternalFile"
			DesignModeCss="designmode.css"
		    ImageGalleryUrl="./ftb/ftb.imagegallery.aspx?rif={0}&cif={0}"
			/>
```
其中
language=“zh-CN”，中文
SupportFolder，对应JavaScriptLocation，是存放JS所在的路径。
ImageGalleryUrl，上传图片页面地址，注意它是这样写的："./ftb/ftb.imagegallery.aspx?rif={0}&cif={0}"
这个{0}系统会自动置换为ImageGalleryPath的值。
ImageGalleryPath，存放上传图片的路径。我在后台，从配置文件里读出设置赋值给它：
`txtContent.ImageGalleryPath = System.Configuration.ConfigurationManager.AppSettings["ImgUpload"];`
1、汉化问题
1）首先，引用  FreeTextBox   的时候，应当将属性language=“zh-CN”
```
<FTB:FreeTextBox id="txtContent" OnSaveClick="btnSubmit_Click" 
			toolbarlayout="ParagraphMenu,FontFacesMenu,FontSizesMenu,FontForeColorsMenu,FontBackColorsMenu|Bold,Italic,Underline,Strikethrough,Superscript,Subscript,RemoveFormat|JustifyLeft,JustifyRight,JustifyCenter,JustifyFull;BulletedList,NumberedList,Indent,Outdent;CreateLink,Unlink,InsertImage|InsertRule,InsertDate,InsertTime|InsertTable,EditTable;InsertTableRowAfter,InsertTableRowBefore,DeleteTableRow;InsertTableColumnAfter,InsertTableColumnBefore,DeleteTableColumn|InsertDiv,EditStyle,InsertImageFromGallery,Preview,SelectAll,WordClean,NetSpell|Cut,Copy,Paste,Delete;Undo,Redo,Print,Save"
			Language="zh-CN"
			runat="Server"
			Width="95%"
			Height="400px"
			SupportFolder="~/news/ftb/"
	        JavaScriptLocation="ExternalFile"
			DesignModeCss="designmode.css"
		    ImageGalleryUrl="./ftb/ftb.imagegallery.aspx?rif={0}&cif={0}"
			/>
```
2）有些提示，包括插入图片窗口的文字，可以直接修改js文件
这里有一些问题要注意：
引用FreeTextBox的时候，要将
`JavaScriptLocation="ExternalFile"`其中，`SupportFolder="~/news/ftb/"`就是JS存放的位置
汉化JS以后，要注意文件的编码问题。我编辑的时候，源文件是GB2312的，要另存为UTF-8编码的。
3）上传图片(image gallery)这个的汉化，要重载控件<FTB:ImageGallery> 的Render事件
```
public partial class ftb_imagegallery : System.Web.UI.Page
{
    protected override void Render(HtmlTextWriter writer)
    {
        writer.Write(this.ChineseGallery());
    }
    /// <summary>
    /// 获取控件，汉化之
    /// </summary>
    /// <returns></returns>
    private string ChineseGallery()
    {
        StringWriter writer = new StringWriter(); //提供一个可以写的文本区域
        HtmlTextWriter buffer = new HtmlTextWriter(writer); //让htmlWriter操作这个区域，我们就可以获得这个区域里的内容
        base.Render(buffer); //先让页面画一遍，让我们得到初始的页面html代码
        string html = writer.ToString(); //找到这段代码，我们来处理它，把英文变成汉字
        //
        html = html.Replace("There are no images:", "现在还没有图片");
        html = html.Replace("Upload File", "上传图片");
        html = html.Replace("Status</div>", "状态</div>");
        html = html.Replace("Selected Image", "选中的图片");
        html = html.Replace("Create Folder", "创建目录");
        html = html.Replace("Delete Image", "删除该图片");
        html = html.Replace("\"Upload\"", "\"上传\"");
        html = html.Replace("<legend>Preview", "<legend>预览图片");
        html = html.Replace("<legend>Dimensions", "<legend>设置显示尺寸");
        html = html.Replace("Original Size", "原图大小");
        html = html.Replace("Custom Size", "设置为");
        html = html.Replace("Lock image ratio", "锁定尺寸比例");
        html = html.Replace("Percentage", "百分比");
        html = html.Replace("Image Gallery", "我的像册");
        html = html.Replace("<legend>Properties", "<legend>图片属性");
        html = html.Replace("Align", "横向排列");
        html = html.Replace("Border", "边框");
        html = html.Replace("VSpace", "左边距");
        html = html.Replace("HSpace", "上边距");
        html = html.Replace("Alt", "转换文字");
        html = html.Replace("Title", "图片标题");
        html = html.Replace("value=\"Insert\"", "value=\"插入到编辑器中\"");
        //
        return html;
    }
}
```
这样就可以得到一个汉化的图片上传界面
2、license(许可证)问题
FreeTextBox有一些功能需要许可证。比如插入图片，如果没有许可证，它只弹出一个JS的对话框，让你简单地写个地址，而不是功能比较强的设置界面。我在本地测试时，因为会自动有个许可证，没发现这个问题，等上传到服务器后，才发现这个功能不能使用，大惑不解，搞了好一阵子，超级恶心。
网上有个神人传授了生成许可证的方法，又是DES加密，又是多少空格，我一看头都大了。时间耗不起。
幸好FreeTextBox的作者比较厚道，是否有许可证，目前看，只区别于是否会输出这么一句：
<script type="text/javascript" src="/WebManage/news/ftb/FTB-Pro.js"></script>
本地有许可证
```
<script type="text/javascript" src="/WebManage/news/ftb/FTB-Utility.js"></script>
<script type="text/javascript" src="/WebManage/news/ftb/FTB-FreeTextBox.js"></script>
<script type="text/javascript" src="/WebManage/news/ftb/FTB-ToolbarItems.js"></script><script type="text/javascript" src="/WebManage/news/ftb/FTB-Pro.js"></script>
<!-- ************************************************* -->
<!-- * FreeTextBox v3 (3.3.0.22838)                  * -->
<!-- * http://www.freetextbox.com/                   * -->
<!-- * ASP.NET HTML editor for PC/IE & Mozilla       * -->
<!-- * License Type: LocalhostLicense (To: none)     * -->
<!-- ************************************************* -->
```
服务器上没有许可证
```
<script type="text/javascript" src="/webmanage/news/ftb/FTB-Utility.js"></script>
<script type="text/javascript" src="/webmanage/news/ftb/FTB-FreeTextBox.js"></script>
<script type="text/javascript" src="/webmanage/news/ftb/FTB-ToolbarItems.js"></script>
<!-- ************************************************ -->
<!-- * FreeTextBox v3 (3.3.0.22838)                 * -->
<!-- * http://www.freetextbox.com/                  * -->
<!-- * ASP.NET HTML editor for PC/IE & Mozilla      * -->
<!-- * License Type: NoLicense (To: Unlicensed)     * -->
<!-- ************************************************ -->
```
结果我手动将这句加在控件后面，就可以用插入图片功能了。
```
<FTB:FreeTextBox id="txtContent" OnSaveClick="btnSubmit_Click" 
			toolbarlayout="ParagraphMenu,FontFacesMenu,FontSizesMenu,FontForeColorsMenu,FontBackColorsMenu|Bold,Italic,Underline,Strikethrough,Superscript,Subscript,RemoveFormat|JustifyLeft,JustifyRight,JustifyCenter,JustifyFull;BulletedList,NumberedList,Indent,Outdent;CreateLink,Unlink,InsertImage|InsertRule,InsertDate,InsertTime|InsertTable,EditTable;InsertTableRowAfter,InsertTableRowBefore,DeleteTableRow;InsertTableColumnAfter,InsertTableColumnBefore,DeleteTableColumn|InsertDiv,EditStyle,InsertImageFromGallery,Preview,SelectAll,WordClean,NetSpell|Cut,Copy,Paste,Delete;Undo,Redo,Print,Save"
			Language="zh-CN"
			runat="Server"
			Width="95%"
			Height="400px"
			SupportFolder="~/news/ftb/"
	        JavaScriptLocation="ExternalFile"
			DesignModeCss="designmode.css"
		    ImageGalleryUrl="./ftb/ftb.imagegallery.aspx?rif={0}&cif={0}"
			/>
			    <script type="text/javascript" src="/WebManage/news/ftb/FTB-Pro.js"></script>
```
注意要加在后面。放控件前面不行。不知道为什么。
3、IE及IE内核浏览器兼容问题
体现在上传图片这里。
用FF、Chrome浏览器都没问题，但IE6\7以及基于这些内核的酷狗也，360也，一打开图片上传，就死机。这个跟是否有许可证没有关系，估计是跟JS有关系。因为在官方DEMO上，我用同样的浏览器去试，一点问题没有。看页面源代码，它没有直接引用JS，应该是靠控件自己输出JS模式的。
后来，将FTB-ImageGallery.js里的2句代码注释了就好了。
```java
function FTB_ResizeGalleryArea() {
	gallery = document.getElementById('Gallery');
	galleryTop = document.getElementById('GalleryTop');
	galleryBottom = document.getElementById('GalleryBottom');
	sideBar = document.getElementById('GallerySideBar');
	
	if (FTB_Browser.isIE) {
	    // check window height
		if (document.body.offsetHeight < (galleryTop.offsetHeight + galleryBottom.offsetHeight + sideBar.offsetHeight) ) {
			window.resizeTo(750, galleryTop.offsetHeight + galleryBottom.offsetHeight + sideBar.offsetHeight+100);
		}
        //这段代码会死机				
		//gallery.style.height = document.body.offsetHeight - (galleryTop.offsetHeight + galleryBottom.offsetHeight);
		//gallery.style.width = document.body.offsetWidth - sideBar.offsetWidth;
	
	} else {
		if (window.innerHeight < (galleryTop.offsetHeight + galleryBottom.offsetHeight + sideBar.offsetHeight) ) {
			window.resizeTo(750, galleryTop.offsetHeight + galleryBottom.offsetHeight + sideBar.offsetHeight+100);
		}
		gallery.style.height =  window.innerHeight - (galleryTop.offsetHeight + galleryBottom.offsetHeight);
		gallery.style.width = window.innerWidth - sideBar.offsetWidth;				
	}	
};
```
