# 基于one2team框架的Highcharts图表图片导出方案 - =朝晖= - 博客园
# [基于one2team框架的Highcharts图表图片导出方案](https://www.cnblogs.com/dhcn/p/7105401.html)
      这篇文章已经没有什么意义了，新版的HIghcharts提供[Java](http://lib.csdn.net/base/java)图片导出解决方案，你需要做的就是下个Maven，bulid一个war就Ok了。---addedy on 2012-11-15
     多说一句废话：我觉得这个功能其实对于大多数应用场景来说是多余的。
       Highcharts是一个纯JSWeb图表绘制解决方案，它的功能之丰富，使用之简单可能是目前开源领域排名比较考前的优秀解决方案，它对个人使用是免费的。
      它的默认版本也有图片导出功能，不过导出服务器是Highcharts官方服务器，我开发的过程试了一下，好像特别慢，图片导出服务用自己的对商业用户来说也有“便利之处”。
       官方的下载页面推荐的Java图片导出方案是[one2team/highcharts-serverside-export](https://github.com/one2team/highcharts-serverside-export)，这个方案是基于apache的batik包的，也有人直接采用batik包开发了[图片导出Servlet代码](http://www.hencuo.com/archives/109) (这个代码没好像没有解决中文问题)。我是比较轴的那种人，本来one2team和上面这个代码核心是一样，我还是研究了一下怎么使用这个官方推荐的导出框架。说实话，官方推荐的这个框架不是很好用，它采用JDK6的泛型特性，项目的编译器兼容性必须提高到1.6,否则编译会出错。其次这个框架的主HighchartsExporter类的功能是转换以Json数据或者Java语言对象为数据源的导出功能，而Highchart导出服务器是要转换Highchart图表post的SVG数据。所以需要对One2Team的框架稍加改造才能使用。One2team的框架的OO设计比较复杂，我就不画具体的UML图了。只列出需要引进的几个类：
      首先继承SVGRendererInternal抽象类，重写callJavascript方法，这个方法的实际作用是把其他格式的数据源转换成SVG数据，我不是很明白为什么SVG数据生成非要在Java里面调用[JavaScript](http://lib.csdn.net/base/javascript)来做，难道为了和浏览器的高度一致？，这个地方我们直接返回chartoption即可，这个chartoption在SVG源数据导出情况下就是SVG数据本书，所以无需处理，直接返回即可。
```
/** 
 *  
 */  
package org.one2team.highcharts.server.export.util;  
  
import org.mozilla.javascript.ScriptableObject;  
import org.one2team.highcharts.server.export.util.SVGRendererInternal;  
import org.one2team.highcharts.shared.ChartOptions;  
  
/** 
 * @author Dipolar 
 * 
 */  
public class SVGRendererInternalSVG extends SVGRendererInternal<String> {  
    @Override  
    protected Object callJavascript (final String generalOptions, final String chartOptions) {  
        //return ScriptableObject.callMethod (null, SCRIPTABLE, "renderSVGFromObject",  new Object [] {'(' + generalOptions + ')', chartOptions});  
        return chartOptions;  
    }  
}
```
      本来加这么一个类，导出图片就够了，我为了方便Servlet操作，我又模仿HighchartsExporter类重写了一个HttpHighchartsExporter类，代码如下：上面这两个类都放在了one2team的包类，因为涉及到的一些类是protected的，为了不改写人家的代码，所以只好放在人家的包里了。还有代码中的globalOptions参数可以省略不计，直接赋null即可，因为这个参数在实际转换中并没有用，实际转换中的globalOptions用的是一个框架本身预定义的常量。
```
package org.one2team.highcharts.server.export;  
import java.io.File;  
import java.io.FileNotFoundException;  
import java.io.FileOutputStream;  
import java.io.OutputStream;  
  
import org.apache.commons.io.IOUtils;  
import org.one2team.highcharts.server.export.ExportType;  
import org.one2team.highcharts.server.export.util.*;  
  
public class HttpHighchartsExporter<T> {  
  
    public HttpHighchartsExporter(ExportType type, SVGRendererInternal<T> internalRenderer) {  
        this.type = type;  
        this.renderer =   
            new SVGStreamRenderer<T> (new SVGRenderer<T> (internalRenderer),  
                                     type.getTranscoder ());  
    }  
    public HttpHighchartsExporter(ExportType type) {  
        SVGRendererInternal svgRender=new SVGRendererInternalSVG();  
        this.type = type;  
        this.renderer =   
            new SVGStreamRenderer<T> (new SVGRenderer<T> (svgRender),  
                                     type.getTranscoder ());  
    }  
  
    public void export (T chartOptions,  
                            T globalOptions,  
                            OutputStream out) {  
          
        OutputStream fos = null;  
        try {  
            fos = render (chartOptions, globalOptions, out);  
  
        } catch (Exception e) {  
            e.printStackTrace ();  
            throw (new RuntimeException (e));  
        } finally {  
            if (fos != null)  
                IOUtils.closeQuietly (fos);  
        }  
    }  
  
    private OutputStream render (T chartOptions,  
                                     T globalOptions,  
                               OutputStream out) throws FileNotFoundException {  
          
        renderer.setChartOptions (chartOptions)  
                    .setGlobalOptions (globalOptions)  
                    .setOutputStream (out)  
                    .render ();  
        return out;  
    }  
  
    public SVGStreamRenderer<T> getRenderer () {  
        return renderer;  
    }  
  
    public ExportType getType () {  
        return type;  
    }  
  
    private final SVGStreamRenderer<T> renderer;  
  
    private final ExportType type;  
}
```
     下面来一下真正的Servlet的 doPost的代码，这个代码虽然解决了jpg和png的中文乱码问题，但是没有解决SVG的导出的中文乱码问题，SVG导出采用svg2svgTranscoder的话，在框架内部的字体调用上出bug了，直接输出svg，不加OutputStreamWriter包装的情况下，chrome可以正常打开这个xml文件，但是中文是乱码)，IE8下在中文位置出问题，加了OutputStreamWriter包装器以后，Chrome下图片可以正常渲染但是报某元素属性值有问题，IE下报了另外一个错误。PDF的导出需要扩充下一下ExportType枚举，PDFTranscoder类也不在batik的包里在[apache的fop](http://xmlgraphics.apache.org/fop/download.html)项目里面，我尝试了一下报出Java堆空间不够的错误，我的生产服务器资源也有限，PDF导出功能就省略把。
```
public void doPost(HttpServletRequest request, HttpServletResponse response)  
        throws ServletException, IOException {  
     try{  
         request.setCharacterEncoding("utf-8");//这一行解决了PNG、JPG的中文乱码问题。  
         String filename=request.getParameter("filename");  
         String type=request.getParameter("type");  
         type=type.replace("svg+", "");  
         MimeType mtype=new MimeType(type);  
         response.addHeader("Content-Disposition", "attachment; filename="+ filename + "."+mtype.getSubType());  
         response.addHeader("Content-Type", mtype.getBaseType()+"; charset=UTF-8");  
         ServletOutputStream out=response.getOutputStream();  
         String svg=request.getParameter("svg");  
         if (mtype.getSubType().equals("xml")){  
             //OutputStreamWriter writer = new OutputStreamWriter(out, "utf-8");  
             //writer.write(svg);  
         }else{  
             HttpHighchartsExporter<String> httpExporter = new HttpHighchartsExporter<String> (Enum.valueOf(ExportType.class,mtype.getSubType()));  
             httpExporter.export(svg,null,out);   
         }  
         out.flush();  
         out.close();  
     }catch(Exception e){  
         e.printStackTrace();  
     }  
      
}
```
       结语：最后提示一下one2team虽然在做图片导出并不一定好，但是JSM系列类可以用来很方便地为前端生成图表绘制所需要的Json数据，当然这个Json数据可以你自己硬代码生成Json数据，但是既然有了与HighChart对象一一对象的Java类，为什么不用一下哪？

