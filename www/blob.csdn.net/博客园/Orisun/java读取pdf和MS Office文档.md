# java读取pdf和MS Office文档 - Orisun - 博客园







# [java读取pdf和MS Office文档](https://www.cnblogs.com/zhangchaoyang/articles/2192185.html)





有时候PDF中的文字无法复制，这可能是因为PDF文件加密了，不过使用PDFBox开源软件就可以把它读出来。

还有一个用于创建PDF文件的项目----iText。

PDFBox下面有两个子项目：FontBox是一个处理PDF字体的java类库；JempBox是一个处理XMP元数据的java类库。

一个简单示例：

要引入pdfbox-app-1.6.0.jar这个包。

```
package pdf;

import java.io.File;
import java.net.MalformedURLException;

import org.apache.pdfbox.pdmodel.PDDocument;
import org.apache.pdfbox.util.PDFTextStripper;

public class StripPDFContent {

    public static String getText(File file)throws Exception{
        boolean sort=false;
        int startPage=1;
        int endPage=10;
        PDDocument document=null;
        try{
            try{
                document=PDDocument.load(file);
            }catch(MalformedURLException e){
                
            }
            PDFTextStripper stripper=new PDFTextStripper();
            stripper.setSortByPosition(sort);
            stripper.setStartPage(startPage);
            stripper.setEndPage(endPage);
            return stripper.getText(document);
        }catch(Exception e){
            e.printStackTrace();
            return "";
        }finally{
            if(document!=null){
                document.close();
            }
        }
    }
    
    public static void main(String[] args){
        File file=new File("/home/orisun/123.pdf");
        try{
            String cont=getText(file);
            System.out.println(cont);
        }catch(Exception e){
            System.out.println("Strip failed.");
            e.printStackTrace();
        }
    }
}
```

Apache的POI项目可以用来处理MS Office文档，codeplex上还有一个它的.net版本。POI项目可创建和维护操作各种基于OOXML和OLE2文件格式的Java API。大多数MS Office都是OLE2格式的。POI通HSMF子项目来支持Outlook，通过HDGF子项目来支持Visio，通过HPBF子项目来支持Publisher。

使用POI抽取Word简单示例：

要引入poi-3.7.jat和poi-scratchpad-3.7.ajr这两个包。

```
package msoffice;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import org.apache.poi.hwpf.HWPFDocument;
import org.apache.poi.hwpf.extractor.WordExtractor;
import org.apache.poi.hwpf.usermodel.CharacterRun;
import org.apache.poi.hwpf.usermodel.Paragraph;
import org.apache.poi.hwpf.usermodel.Range;
import org.apache.poi.hwpf.usermodel.Section;

public class Word {

    // 直接抽取全部内容
    public static String readDoc1(InputStream is) throws IOException {
        WordExtractor extractor = new WordExtractor(is);
        return extractor.getText();
    }
    
    //分章节Section、段落Paragraph、字符串CharacterRun抽取
    public static void readDoc2(InputStream is) throws IOException {
        HWPFDocument doc=new HWPFDocument(is);
        Range r=doc.getRange();
        for(int x=0;x<r.numSections();x++){
            Section s=r.getSection(x);
            for(int y=0;y<s.numParagraphs();y++){
                Paragraph p=s.getParagraph(y);
                for(int z=0;z<p.numCharacterRuns();z++){
                    CharacterRun run=p.getCharacterRun(z);
                    String text=run.text();
                    System.out.print(text);
                }
            }
        }
    }

    public static void main(String[] args) {
        File file = new File("/home/orisun/1.doc");
        try {
            FileInputStream fin = new FileInputStream(file);
            String cont = readDoc1(fin);
            System.out.println(cont);
            fin.close();
            fin = new FileInputStream(file);
            readDoc2(fin);
            fin.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```

[http://code.google.com/p/text-mining/](http://code.google.com/p/text-mining/)上有另外一个Word解析包。

一上PPT文件由多个slide组成。

POI抽取PPT示例：

```
package msoffice;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import org.apache.poi.hslf.HSLFSlideShow;
import org.apache.poi.hslf.extractor.PowerPointExtractor;
import org.apache.poi.hslf.model.Slide;
import org.apache.poi.hslf.model.TextRun;
import org.apache.poi.hslf.usermodel.SlideShow;

public class PPT {

    //直接抽取幻灯片的全部内容
    public static String readDoc1(InputStream is) throws IOException{
        PowerPointExtractor extractor=new PowerPointExtractor(is);
        return extractor.getText();
    }
    
    //一张幻灯片一张幻灯片地读取
    public static void readDoc2(InputStream is) throws IOException{
        SlideShow ss=new SlideShow(new HSLFSlideShow(is));
        Slide[] slides=ss.getSlides();
        for(int i=0;i<slides.length;i++){
            //读取一张幻灯片的标题
            String title=slides[i].getTitle();
            System.out.println("标题:"+title);
            //读取一张幻灯片的内容(包括标题)
            TextRun[] runs=slides[i].getTextRuns();
            for(int j=0;j<runs.length;j++){
                System.out.println(runs[j].getText());
            }
        }
    }
    
    public static void main(String[] args){
        File file = new File("/home/orisun/2.ppt");
        try{
            FileInputStream fin=new FileInputStream(file);
            String cont=readDoc1(fin);
            System.out.println(cont);
            fin.close();
            fin=new FileInputStream(file);
            readDoc2(fin);
            fin.close();
        }catch(IOException e){
            e.printStackTrace();
        }
    }
}
```

Excel文件由多个Workbook组成，一个Workbook由多个Sheet组成。

POI抽取Excel简单示例：

```
package msoffice;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Iterator;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.hssf.extractor.ExcelExtractor;
import org.apache.poi.poifs.filesystem.POIFSFileSystem;
import org.apache.poi.ss.usermodel.Row;

public class Excel {

    //直接读取Excel的全部内容
    public static String readDoc1(InputStream is)throws IOException{
        HSSFWorkbook wb=new HSSFWorkbook(new POIFSFileSystem(is));
        ExcelExtractor extractor=new ExcelExtractor(wb);
        extractor.setFormulasNotResults(false);
        extractor.setIncludeSheetNames(true);
        return extractor.getText();
    }
    
    //读取时细化到Sheet、行甚至单元格
    public static double getAvg(InputStream is)throws IOException{
        HSSFWorkbook wb=new HSSFWorkbook(new POIFSFileSystem(is));
        //获取第一张sheet
        HSSFSheet sheet=wb.getSheetAt(0);
        double molecule=0.0;
        double denominator=0.0;
        //按行遍历sheet
        Iterator<Row> riter=sheet.rowIterator();
        while(riter.hasNext()){
            HSSFRow row=(HSSFRow)riter.next();
            HSSFCell cell1=row.getCell(4);
            HSSFCell cell2=row.getCell(4);
            if(cell1.getCellType()!=HSSFCell.CELL_TYPE_NUMERIC){
                System.err.println("数字类型错误！");
                System.exit(-2);
            }
            if(cell2.getCellType()!=HSSFCell.CELL_TYPE_NUMERIC){
                System.err.println("数字类型错误！");
                System.exit(-2);
            }
            denominator+=Double.parseDouble(cell2.toString().trim());
            molecule+=Double.parseDouble(cell2.toString().trim())*Float.parseFloat(cell1.toString().trim());
        }
        return molecule/denominator;
    }
    
    public static void main(String[] args){
        File file = new File("/home/orisun/3.xls");
        try{
            FileInputStream fin=new FileInputStream(file);
            String cont=readDoc1(fin);
            System.out.println(cont);
            fin.close();
            fin=new FileInputStream(file);
            System.out.println("加权平均分"+getAvg(fin));
            fin.close();
        }catch(IOException e){
            e.printStackTrace();
        }
    }
}
```

还有一个开源项目可以用来读写Excel文件----jxl，网址[http://www.andykhan.com/jexcelapi/](http://www.andykhan.com/jexcelapi/)












