
# NPOI向word中插入图片的Bug解决方案 - 追求卓越,做到专业 - CSDN博客


2018年06月06日 16:43:06[Waldenz](https://me.csdn.net/enter89)阅读数：910


NPOI包中插入图片，会导致最终生成的word打不开。使用自定义的插入。
```python
public class XWPFInsertPicture
    {
        public void WordIndertPicTest()
        {
            try
            {
                var wordDoc = new XWPFDocument();
                var picAbsolutePath = @"D:\test.jpg";
                if (File.Exists(picAbsolutePath))
                {
                    var picID = wordDoc.AddPictureData(new FileStream(picAbsolutePath, FileMode.Open), (int)PictureType.JPEG);
                    CreatePicture(wordDoc, picID, 800, 400);
                }
                var outputPath = Path.Combine(@"D:\", Guid.NewGuid().ToString() + ".docx");
                var writeStream = new FileStream(outputPath, FileMode.Create);
                wordDoc.Write(writeStream);
                writeStream.Close();
            }
            catch (Exception ex)
            {

            }
        }
        public static void CreatePicture(XWPFDocument doc, string id, int width, int height)
        {
            int EMU = 9525;
            width *= EMU;
            height *= EMU;
            string picXml = ""
                    //+ "<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">"
                    //+ "   <a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">"
                    + "      <pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">"
                    + "         <pic:nvPicPr>" + "            <pic:cNvPr id=\""
                    + "0"
                    + "\" name=\"Generated\"/>"
                    + "            <pic:cNvPicPr/>"
                    + "         </pic:nvPicPr>"
                    + "         <pic:blipFill>"
                    + "            <a:blip r:embed=\""
                    + id
                    + "\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\"/>"
                    + "            <a:stretch>"
                    + "               <a:fillRect/>"
                    + "            </a:stretch>"
                    + "         </pic:blipFill>"
                    + "         <pic:spPr>"
                    + "            <a:xfrm>"
                    + "               <a:off x=\"0\" y=\"0\"/>"
                    + "               <a:ext cx=\""
                    + width
                    + "\" cy=\""
                    + height
                    + "\"/>"
                    + "            </a:xfrm>"
                    + "            <a:prstGeom prst=\"rect\">"
                    + "               <a:avLst/>"
                    + "            </a:prstGeom>"
                    + "         </pic:spPr>"
                    + "      </pic:pic>";
            //+ "   </a:graphicData>" + "</a:graphic>";
            var run = doc.CreateParagraph().CreateRun();
            CT_Inline inline = run.GetCTR().AddNewDrawing().AddNewInline();
            inline.graphic = new CT_GraphicalObject();
            inline.graphic.graphicData = new CT_GraphicalObjectData();
            inline.graphic.graphicData.uri = "http://schemas.openxmlformats.org/drawingml/2006/picture";
            // CT_GraphicalObjectData graphicData = inline.graphic.AddNewGraphicData();
            // graphicData.uri = "http://schemas.openxmlformats.org/drawingml/2006/picture";
            //XmlDocument xmlDoc = new XmlDocument();
            try
            {
                //xmlDoc.LoadXml(picXml);
                //var element = xmlDoc.DocumentElement;
                inline.graphic.graphicData.AddPicElement(picXml);
            }
            catch (XmlException xe)
            {
            }
            NPOI.OpenXmlFormats.Dml.WordProcessing.CT_PositiveSize2D extent = inline.AddNewExtent();
            extent.cx = width;
            extent.cy = height;
            NPOI.OpenXmlFormats.Dml.WordProcessing.CT_NonVisualDrawingProps docPr = inline.AddNewDocPr();
            docPr.id = 1;
            docPr.name = "Image" + id;
        }
    }
```


