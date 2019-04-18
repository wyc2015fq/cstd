# POI SAX 使用事件驱动解析Excel（03/07）---有错误 - z69183787的专栏 - CSDN博客
2017年05月23日 10:30:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2510
转自：[http://www.cnblogs.com/wshsdlau/p/5643862.html](http://www.cnblogs.com/wshsdlau/p/5643862.html)
Excel2007版本的代码如下，本文主要是用于POI解析大文件Excel容易出现内存溢出的现象而提出解决方案，故此解决了大数据量的Excel文件解析的难度，在此拿出来贡献给大家，谢谢！
里面用到的相关类请查看:  http://www.cnblogs.com/wshsdlau/p/5643862.html
1. Office2007与Office Open XML
    在Office 2007之前，Office一直都是以二进制位的方式存储，但这种格式不易被其它软件拿来使用，在各界的压力下，MicroSoft于2005年发布了基于XML的ooxml开放文档标准。ooxml的xml schema强调减少load time，增快parsing speed，将child elements分开存储，而不是multiple attributes一起存，这有点类似于HTML的结构。ooxml 使用XML和ZIP技术结合进行文件存储，因为XML是一个基于文本的格式，而且ZIP容器支持内容的压缩，所以其一大优势就是可以大大减小文件的尺寸。其它特点这里不再叙述。
2. SAX方式解析XML
    SAX全称Simple API for XML，它是一个接口，也是一个软件包。它是一种XML解析的替代方法，不同于DOM解析XML文档时把所有内容一次性加载到内存中的方式，它逐行扫描文档，一边扫描，一边解析。所以那些只需要单遍读取内容的应用程序就可以从SAX解析中受益，这对大型文档的解析是个巨大优势。另外，SAX “推" 模型可用于广播环境，能够同时注册多个ContentHandler，并行接收事件，而不是在一个管道中一个接一个地进行处理。一些支持 SAX 的语法分析器包括 Xerces，Apache parser（以前的 IBM 语法分析器）、MSXML（Microsoft 语法分析器）和 XDK（Oracle 语法分析器）。这些语法分析器是最灵活的，因为它们还支持 DOM。
3. POI以SAX解析excel2007文件
   所需jar包：poi-3.10-FINAL-20140208.jar，poi-ooxml-3.10-FINAL-20140208.jar， poi-ooxml-schemas-3.10-FINAL-20140208.jar
      xercesImpl.jar      xml-apis-2.0.2.jar      xmlbeans-2.6.0.jar     sax2.jar
辅助类
ExcelReaderUtil：
```java
package com.boguan.bte.util.excel;
import com.boguan.bte.service.common.IExcelRowReader;
import com.boguan.bte.service.common.impl.ExcelRowReader;
/**
 * 名称: ExcelReaderUtil.java<br>
 * 描述: <br>
 * 类型: JAVA<br>
 * 最近修改时间:2016年7月5日 上午10:10:20<br>
 * 
 * @since 2016年7月5日
 * @author “”
 */
public class ExcelReaderUtil {
    // excel2003扩展名
    public static final String EXCEL03_EXTENSION = ".xls";
    // excel2007扩展名
    public static final String EXCEL07_EXTENSION = ".xlsx";
    /**
     * 读取Excel文件，可能是03也可能是07版本
     * 
     * @param excel03
     * @param excel07
     * @param fileName
     * @throws Exception
     */
    public static void readExcel(IExcelRowReader reader, String fileName) throws Exception {
        // 处理excel2003文件
        if (fileName.endsWith(EXCEL03_EXTENSION)) {
            ExcelXlsReader exceXls = new ExcelXlsReader();
            exceXls.setRowReader(reader); 
            exceXls.process(fileName);
            // 处理excel2007文件
        } else if (fileName.endsWith(EXCEL07_EXTENSION)) {
            ExcelXlsxReader exceXlsx = new ExcelXlsxReader();
            exceXlsx.setRowReader(reader); 
            exceXlsx.process(fileName);
        } else {
            throw new Exception("文件格式错误，fileName的扩展名只能是xls或xlsx。");
        }
    }
    /**
     * 测试
     * @param args
     * @throws Exception
     */
    public static void main(String[] args) throws Exception {
        IExcelRowReader rowReader = new ExcelRowReader();
        ExcelReaderUtil.readExcel(rowReader, "E://test.xls");
    }
}
```
```java
package com.boguan.bte.service.common;
import java.util.List;
/**
 * 名称: IRowReader.java<br>
 * 描述: <br>
 * 类型: JAVA<br>
 * 最近修改时间:2016年7月5日 上午10:28:06<br>
 * 
 * @since 2016年7月5日
 * @author “”
 */
public interface IExcelRowReader {
    /**
     * 业务逻辑实现方法
     * 
     * @param sheetIndex
     * @param curRow
     * @param rowlist
     */
    void getRows(int sheetIndex, int curRow, List<String> rowlist);
}
```
```java
package com.boguan.bte.service.common.impl;
import java.util.List;
import com.boguan.bte.service.common.IExcelRowReader;
/**
 * 名称: ExcelRowReader.java<br>
 * 描述: <br>
 * 类型: JAVA<br>
 * 最近修改时间:2016年7月5日 上午10:30:11<br>
 * 
 * @since 2016年7月5日
 * @author “”
 */
public class ExcelRowReader implements IExcelRowReader {
    @Override
    public void getRows(int sheetIndex, int curRow, List<String> rowlist) {
        System.out.print(curRow+" ");  
        for (int i = 0; i < rowlist.size(); i++) {  
            System.out.print(rowlist.get(i)==""?"*":rowlist.get(i) + " ");  
        }  
        System.out.println();  
    }
    
}
```
03 Reader：
```java
package com.boguan.bte.util.excel;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import org.apache.poi.hssf.eventusermodel.EventWorkbookBuilder.SheetRecordCollectingListener;
import org.apache.poi.hssf.eventusermodel.FormatTrackingHSSFListener;
import org.apache.poi.hssf.eventusermodel.HSSFEventFactory;
import org.apache.poi.hssf.eventusermodel.HSSFListener;
import org.apache.poi.hssf.eventusermodel.HSSFRequest;
import org.apache.poi.hssf.eventusermodel.MissingRecordAwareHSSFListener;
import org.apache.poi.hssf.eventusermodel.dummyrecord.LastCellOfRowDummyRecord;
import org.apache.poi.hssf.eventusermodel.dummyrecord.MissingCellDummyRecord;
import org.apache.poi.hssf.model.HSSFFormulaParser;
import org.apache.poi.hssf.record.BOFRecord;
import org.apache.poi.hssf.record.BlankRecord;
import org.apache.poi.hssf.record.BoolErrRecord;
import org.apache.poi.hssf.record.BoundSheetRecord;
import org.apache.poi.hssf.record.FormulaRecord;
import org.apache.poi.hssf.record.LabelRecord;
import org.apache.poi.hssf.record.LabelSSTRecord;
import org.apache.poi.hssf.record.NumberRecord;
import org.apache.poi.hssf.record.Record;
import org.apache.poi.hssf.record.SSTRecord;
import org.apache.poi.hssf.record.StringRecord;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.poifs.filesystem.POIFSFileSystem;
import com.boguan.bte.service.common.IExcelRowReader;
/**
 * 名称: ExcelXlsReader.java<br>
 * 描述: <br>
 * 类型: JAVA<br>
 * 最近修改时间:2016年7月5日 上午10:00:32<br>
 * 
 * @since 2016年7月5日
 * @author “”
 */
public class ExcelXlsReader implements HSSFListener {
    private int minColumns = -1;
    private POIFSFileSystem fs;
    private int lastRowNumber;
    private int lastColumnNumber;
    /** Should we output the formula, or the value it has? */
    private boolean outputFormulaValues = true;
    /** For parsing Formulas */
    private SheetRecordCollectingListener workbookBuildingListener;
    // excel2003工作薄
    private HSSFWorkbook stubWorkbook;
    // Records we pick up as we process
    private SSTRecord sstRecord;
    private FormatTrackingHSSFListener formatListener;
    // 表索引
    private int sheetIndex = -1;
    private BoundSheetRecord[] orderedBSRs;
    @SuppressWarnings("unchecked")
    private ArrayList boundSheetRecords = new ArrayList();
    // For handling formulas with string results
    private int nextRow;
    private int nextColumn;
    private boolean outputNextStringRecord;
    // 当前行
    private int curRow = 0;
    // 存储行记录的容器
    private List<String> rowlist = new ArrayList<String>();;
    @SuppressWarnings("unused")
    private String sheetName;
    private IExcelRowReader rowReader;
    public void setRowReader(IExcelRowReader rowReader) {
        this.rowReader = rowReader;
    }
    /**
     * 遍历excel下所有的sheet
     * 
     * @throws IOException
     */
    public void process(String fileName) throws IOException {
        this.fs = new POIFSFileSystem(new FileInputStream(fileName));
        MissingRecordAwareHSSFListener listener = new MissingRecordAwareHSSFListener(this);
        formatListener = new FormatTrackingHSSFListener(listener);
        HSSFEventFactory factory = new HSSFEventFactory();
        HSSFRequest request = new HSSFRequest();
        if (outputFormulaValues) {
            request.addListenerForAllRecords(formatListener);
        } else {
            workbookBuildingListener = new SheetRecordCollectingListener(formatListener);
            request.addListenerForAllRecords(workbookBuildingListener);
        }
        factory.processWorkbookEvents(request, fs);
    }
    /**
     * HSSFListener 监听方法，处理 Record
     */
    @SuppressWarnings("unchecked")
    public void processRecord(Record record) {
        int thisRow = -1;
        int thisColumn = -1;
        String thisStr = null;
        String value = null;
        switch (record.getSid()) {
        case BoundSheetRecord.sid:
            boundSheetRecords.add(record);
            break;
        case BOFRecord.sid:
            BOFRecord br = (BOFRecord) record;
            if (br.getType() == BOFRecord.TYPE_WORKSHEET) {
                // 如果有需要，则建立子工作薄
                if (workbookBuildingListener != null && stubWorkbook == null) {
                    stubWorkbook = workbookBuildingListener.getStubHSSFWorkbook();
                }
                sheetIndex++;
                if (orderedBSRs == null) {
                    orderedBSRs = BoundSheetRecord.orderByBofPosition(boundSheetRecords);
                }
                sheetName = orderedBSRs[sheetIndex].getSheetname();
            }
            break;
        case SSTRecord.sid:
            sstRecord = (SSTRecord) record;
            break;
        case BlankRecord.sid:
            BlankRecord brec = (BlankRecord) record;
            thisRow = brec.getRow();
            thisColumn = brec.getColumn();
            thisStr = "";
            rowlist.add(thisColumn, thisStr);
            break;
        case BoolErrRecord.sid: // 单元格为布尔类型
            BoolErrRecord berec = (BoolErrRecord) record;
            thisRow = berec.getRow();
            thisColumn = berec.getColumn();
            thisStr = berec.getBooleanValue() + "";
            rowlist.add(thisColumn, thisStr);
            break;
        case FormulaRecord.sid: // 单元格为公式类型
            FormulaRecord frec = (FormulaRecord) record;
            thisRow = frec.getRow();
            thisColumn = frec.getColumn();
            if (outputFormulaValues) {
                if (Double.isNaN(frec.getValue())) {
                    // Formula result is a string
                    // This is stored in the next record
                    outputNextStringRecord = true;
                    nextRow = frec.getRow();
                    nextColumn = frec.getColumn();
                } else {
                    thisStr = formatListener.formatNumberDateCell(frec);
                }
            } else {
                thisStr = '"' + HSSFFormulaParser.toFormulaString(stubWorkbook, frec.getParsedExpression()) + '"';
            }
            rowlist.add(thisColumn, thisStr);
            break;
        case StringRecord.sid:// 单元格中公式的字符串
            if (outputNextStringRecord) {
                // String for formula
                StringRecord srec = (StringRecord) record;
                thisStr = srec.getString();
                thisRow = nextRow;
                thisColumn = nextColumn;
                outputNextStringRecord = false;
            }
            break;
        case LabelRecord.sid:
            LabelRecord lrec = (LabelRecord) record;
            curRow = thisRow = lrec.getRow();
            thisColumn = lrec.getColumn();
            value = lrec.getValue().trim();
            value = value.equals("") ? " " : value;
            this.rowlist.add(thisColumn, value);
            break;
        case LabelSSTRecord.sid: // 单元格为字符串类型
            LabelSSTRecord lsrec = (LabelSSTRecord) record;
            curRow = thisRow = lsrec.getRow();
            thisColumn = lsrec.getColumn();
            if (sstRecord == null) {
                rowlist.add(thisColumn, " ");
            } else {
                value = sstRecord.getString(lsrec.getSSTIndex()).toString().trim();
                value = value.equals("") ? " " : value;
                rowlist.add(thisColumn, value);
            }
            break;
        case NumberRecord.sid: // 单元格为数字类型
            NumberRecord numrec = (NumberRecord) record;
            curRow = thisRow = numrec.getRow();
            thisColumn = numrec.getColumn();
            value = formatListener.formatNumberDateCell(numrec).trim();
            value = value.equals("") ? " " : value;
            // 向容器加入列值
            rowlist.add(thisColumn, value);
            break;
        default:
            break;
        }
        // 遇到新行的操作
        if (thisRow != -1 && thisRow != lastRowNumber) {
            lastColumnNumber = -1;
        }
        // 空值的操作
        if (record instanceof MissingCellDummyRecord) {
            MissingCellDummyRecord mc = (MissingCellDummyRecord) record;
            curRow = thisRow = mc.getRow();
            thisColumn = mc.getColumn();
            rowlist.add(thisColumn, " ");
        }
        // 更新行和列的值
        if (thisRow > -1)
            lastRowNumber = thisRow;
        if (thisColumn > -1)
            lastColumnNumber = thisColumn;
        // 行结束时的操作
        if (record instanceof LastCellOfRowDummyRecord) {
            if (minColumns > 0) {
                // 列值重新置空
                if (lastColumnNumber == -1) {
                    lastColumnNumber = 0;
                }
            }
            lastColumnNumber = -1;
            // 每行结束时， 调用getRows() 方法
            rowReader.getRows(sheetIndex, curRow, rowlist);
            // 清空容器
            rowlist.clear();
        }
    }
　　
      public static void main(String[] args) {
       IExcelRowReader rowReader = new ExcelRowReader();
      try {
          // ExcelReaderUtil.readExcel(rowReader,
          // "E://2016-07-04-011940a.xls");
            System.out.println("**********************************************");
            ExcelReaderUtil.readExcel(rowReader, "E://test.xlsx");
            } catch (Exception e) {
            e.printStackTrace();
           }
       }
}
```
07Reader：
```java
package com.boguan.bte.util.excel;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import org.apache.commons.lang.StringUtils;
import org.apache.poi.openxml4j.exceptions.OpenXML4JException;
import org.apache.poi.openxml4j.opc.OPCPackage;
import org.apache.poi.ss.usermodel.BuiltinFormats;
import org.apache.poi.ss.usermodel.DataFormatter;
import org.apache.poi.xssf.eventusermodel.XSSFReader;
import org.apache.poi.xssf.model.SharedStringsTable;
import org.apache.poi.xssf.model.StylesTable;
import org.apache.poi.xssf.usermodel.XSSFCellStyle;
import org.apache.poi.xssf.usermodel.XSSFRichTextString;
import org.xml.sax.Attributes;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.DefaultHandler;
import org.xml.sax.helpers.XMLReaderFactory;
import com.boguan.bte.service.common.IExcelRowReader;
import com.boguan.bte.service.common.impl.ExcelRowReader;
/**
 * 名称: ExcelXlsxReader.java<br>
 * 描述: <br>
 * 类型: JAVA<br>
 * 最近修改时间:2016年7月5日 上午10:00:52<br>
 * 
 * @since 2016年7月5日
 * @author “”
 */
public class ExcelXlsxReader extends DefaultHandler {
    private IExcelRowReader rowReader;
    public void setRowReader(IExcelRowReader rowReader) {
        this.rowReader = rowReader;
    }
    /**
     * 共享字符串表
     */
    private SharedStringsTable sst;
    /**
     * 上一次的内容
     */
    private String lastContents;
    /**
     * 字符串标识
     */
    private boolean nextIsString;
    /**
     * 工作表索引
     */
    private int sheetIndex = -1;
    /**
     * 行集合
     */
    private List<String> rowlist = new ArrayList<String>();
    /**
     * 当前行
     */
    private int curRow = 0;
    /**
     * 当前列
     */
    private int curCol = 0;
    /**
     * T元素标识
     */
    private boolean isTElement;
    /**
     * 异常信息，如果为空则表示没有异常
     */
    private String exceptionMessage;
    /**
     * 单元格数据类型，默认为字符串类型
     */
    private CellDataType nextDataType = CellDataType.SSTINDEX;
    private final DataFormatter formatter = new DataFormatter();
    private short formatIndex;
    private String formatString;
    // 定义前一个元素和当前元素的位置，用来计算其中空的单元格数量，如A6和A8等
    private String preRef = null, ref = null;
    // 定义该文档一行最大的单元格数，用来补全一行最后可能缺失的单元格
    private String maxRef = null;
    /**
     * 单元格
     */
    private StylesTable stylesTable;
    /**
     * 遍历工作簿中所有的电子表格
     * 
     * @param filename
     * @throws IOException
     * @throws OpenXML4JException
     * @throws SAXException
     * @throws Exception
     */
    public void process(String filename) throws IOException, OpenXML4JException, SAXException {
        OPCPackage pkg = OPCPackage.open(filename);
        XSSFReader xssfReader = new XSSFReader(pkg);
        stylesTable = xssfReader.getStylesTable();
        SharedStringsTable sst = xssfReader.getSharedStringsTable();
        XMLReader parser = this.fetchSheetParser(sst);
        Iterator<InputStream> sheets = xssfReader.getSheetsData();
        while (sheets.hasNext()) {
            curRow = 0;
            sheetIndex++;
            InputStream sheet = sheets.next();
            InputSource sheetSource = new InputSource(sheet);
            parser.parse(sheetSource);
            sheet.close();
        }
    }
    public XMLReader fetchSheetParser(SharedStringsTable sst) throws SAXException {
        XMLReader parser = XMLReaderFactory.createXMLReader("org.apache.xerces.parsers.SAXParser");
        this.sst = sst;
        parser.setContentHandler(this);
        return parser;
    }
    public void startElement(String uri, String localName, String name, Attributes attributes) throws SAXException {
        // c => 单元格
        if ("c".equals(name)) {
            // 前一个单元格的位置
            if (preRef == null) {
                preRef = attributes.getValue("r");
            } else {
                preRef = ref;
            }
            // 当前单元格的位置
            ref = attributes.getValue("r");
            // 设定单元格类型
            this.setNextDataType(attributes);
            // Figure out if the value is an index in the SST
            String cellType = attributes.getValue("t");
            if (cellType != null && cellType.equals("s")) {
                nextIsString = true;
            } else {
                nextIsString = false;
            }
        }
        // 当元素为t时
        if ("t".equals(name)) {
            isTElement = true;
        } else {
            isTElement = false;
        }
        // 置空
        lastContents = "";
    }
    /**
     * 单元格中的数据可能的数据类型
     */
    enum CellDataType {
        BOOL, ERROR, FORMULA, INLINESTR, SSTINDEX, NUMBER, DATE, NULL
    }
    /**
     * 处理数据类型
     * 
     * @param attributes
     */
    public void setNextDataType(Attributes attributes) {
        nextDataType = CellDataType.NUMBER;
        formatIndex = -1;
        formatString = null;
        String cellType = attributes.getValue("t");
        String cellStyleStr = attributes.getValue("s");
        String columData = attributes.getValue("r");
        if ("b".equals(cellType)) {
            nextDataType = CellDataType.BOOL;
        } else if ("e".equals(cellType)) {
            nextDataType = CellDataType.ERROR;
        } else if ("inlineStr".equals(cellType)) {
            nextDataType = CellDataType.INLINESTR;
        } else if ("s".equals(cellType)) {
            nextDataType = CellDataType.SSTINDEX;
        } else if ("str".equals(cellType)) {
            nextDataType = CellDataType.FORMULA;
        }
        if (cellStyleStr != null) {
            int styleIndex = Integer.parseInt(cellStyleStr);
            XSSFCellStyle style = stylesTable.getStyleAt(styleIndex);
            formatIndex = style.getDataFormat();
            formatString = style.getDataFormatString();
            if ("m/d/yy" == formatString) {
                nextDataType = CellDataType.DATE;
                formatString = "yyyy-MM-dd hh:mm:ss.SSS";
            }
            if (formatString == null) {
                nextDataType = CellDataType.NULL;
                formatString = BuiltinFormats.getBuiltinFormat(formatIndex);
            }
        }
    }
    /**
     * 对解析出来的数据进行类型处理
     * 
     * @param value
     *            单元格的值（这时候是一串数字）
     * @param thisStr
     *            一个空字符串
     * @return
     */
    @SuppressWarnings("deprecation")
    public String getDataValue(String value, String thisStr) {
        switch (nextDataType) {
        // 这几个的顺序不能随便交换，交换了很可能会导致数据错误
        case BOOL:
            char first = value.charAt(0);
            thisStr = first == '0' ? "FALSE" : "TRUE";
            break;
        case ERROR:
            thisStr = "\"ERROR:" + value.toString() + '"';
            break;
        case FORMULA:
            thisStr = '"' + value.toString() + '"';
            break;
        case INLINESTR:
            XSSFRichTextString rtsi = new XSSFRichTextString(value.toString());
            thisStr = rtsi.toString();
            rtsi = null;
            break;
        case SSTINDEX:
            String sstIndex = value.toString();
            try {
                int idx = Integer.parseInt(sstIndex);
                XSSFRichTextString rtss = new XSSFRichTextString(sst.getEntryAt(idx));
                thisStr = rtss.toString();
                rtss = null;
            } catch (NumberFormatException ex) {
                thisStr = value.toString();
            }
            break;
        case NUMBER:
            if (formatString != null) {
                thisStr = formatter.formatRawCellContents(Double.parseDouble(value), formatIndex, formatString).trim();
            } else {
                thisStr = value;
            }
            thisStr = thisStr.replace("_", "").trim();
            break;
        case DATE:
            thisStr = formatter.formatRawCellContents(Double.parseDouble(value), formatIndex, formatString);
            // 对日期字符串作特殊处理
            thisStr = thisStr.replace(" ", "T");
            break;
        default:
            thisStr = " ";
            break;
        }
        return thisStr;
    }
    @Override
    public void endElement(String uri, String localName, String name) throws SAXException {
        // 根据SST的索引值的到单元格的真正要存储的字符串
        // 这时characters()方法可能会被调用多次
        if (nextIsString  && && StringUtils.isNotEmpty(lastContents) && StringUtils.isNumeric(lastContents)) {
            int idx = Integer.parseInt(lastContents);
            lastContents = new XSSFRichTextString(sst.getEntryAt(idx)).toString();
        }
        // t元素也包含字符串
        if (isTElement) {
            // 将单元格内容加入rowlist中，在这之前先去掉字符串前后的空白符
            String value = lastContents.trim();
            rowlist.add(curCol, value);
            curCol++;
            isTElement = false;
        } else if ("v".equals(name)) {
            // v => 单元格的值，如果单元格是字符串则v标签的值为该字符串在SST中的索引
            String value = this.getDataValue(lastContents.trim(), "");
            // 补全单元格之间的空单元格
            if (!ref.equals(preRef)) {
                int len = countNullCell(ref, preRef);
                for (int i = 0; i < len; i++) {
                    rowlist.add(curCol, "");
                    curCol++;
                }
            }
            rowlist.add(curCol, value);
            curCol++;
        } else {
            // 如果标签名称为 row ，这说明已到行尾，调用 optRows() 方法
            if (name.equals("row")) {
                // 默认第一行为表头，以该行单元格数目为最大数目
                if (curRow == 0) {
                    maxRef = ref;
                }
                // 补全一行尾部可能缺失的单元格
                if (maxRef != null) {
                    int len = countNullCell(maxRef, ref);
                    for (int i = 0; i <= len; i++) {
                        rowlist.add(curCol, "");
                        curCol++;
                    }
                }
                rowReader.getRows(sheetIndex, curRow, rowlist);
                rowlist.clear();
                curRow++;
                curCol = 0;
                preRef = null;
                ref = null;
            }
        }
    }
    /**
     * 计算两个单元格之间的单元格数目(同一行)
     * 
     * @param ref
     * @param preRef
     * @return
     */
    public int countNullCell(String ref, String preRef) {
        // excel2007最大行数是1048576，最大列数是16384，最后一列列名是XFD
        String xfd = ref.replaceAll("\\d+", "");
        String xfd_1 = preRef.replaceAll("\\d+", "");
        xfd = fillChar(xfd, 3, '@', true);
        xfd_1 = fillChar(xfd_1, 3, '@', true);
        char[] letter = xfd.toCharArray();
        char[] letter_1 = xfd_1.toCharArray();
        int res = (letter[0] - letter_1[0]) * 26 * 26 + (letter[1] - letter_1[1]) * 26 + (letter[2] - letter_1[2]);
        return res - 1;
    }
    /**
     * 字符串的填充
     * 
     * @param str
     * @param len
     * @param let
     * @param isPre
     * @return
     */
    String fillChar(String str, int len, char let, boolean isPre) {
        int len_1 = str.length();
        if (len_1 < len) {
            if (isPre) {
                for (int i = 0; i < (len - len_1); i++) {
                    str = let + str;
                }
            } else {
                for (int i = 0; i < (len - len_1); i++) {
                    str = str + let;
                }
            }
        }
        return str;
    }
    @Override
    public void characters(char[] ch, int start, int length) throws SAXException {
        // 得到单元格内容的值
        lastContents += new String(ch, start, length);
    }
    /**
     * @return the exceptionMessage
     */
    public String getExceptionMessage() {
        return exceptionMessage;
    }
    public static void main(String[] args) {
        IExcelRowReader rowReader = new ExcelRowReader();
        try {
            // ExcelReaderUtil.readExcel(rowReader,
            // "E://2016-07-04-011940a.xls");
            System.out.println("**********************************************");
            ExcelReaderUtil.readExcel(rowReader, "E://test.xlsx");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
