# How to create PDF files in a Python/Django application using ReportLab - =朝晖= - 博客园
# [How to create PDF files in a Python/Django application using ReportLab](https://www.cnblogs.com/dhcn/p/9036155.html)
https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab
CONTENTS
- [Introduction](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#introduction)
- [How to serve the file for download](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#how-to-serve-the-file-for-download)
- [How to create a PDF document](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#how-to-create-a-pdf-document)
- [How to add paragraphs and spaces in between](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#how-to-add-paragraphs-and-spaces-in-between)
- [How to add a table](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#how-to-add-a-table)
- [How to add charts in PDF documents](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#how-to-add-charts-in-pdf-documents)- [a. How to add line charts](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#a-how-to-add-line-charts)
- [b. How to add bar charts](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#b-how-to-add-bar-charts)
- [c. How to add pie charts](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#c-how-to-add-pie-charts)
- [How to add page numbers to a PDF document](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#how-to-add-page-numbers-to-a-pdf-document)
- [In conclusion](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#in-conclusion)
![Cioata Petru](https://cdnsite1.assist.ro/sites/default/files/styles/avatar_100x100_/public/members_pictures/Cioat%C4%83%20Petru-Vasile%20%20%284%29_pp.jpg?itok=DrtMVlXm)
[Petru CIOATA](https://assist-software.net/team/petru-cioata)
Web Developer at ASSIST
„Nothing is too small to know and nothing too big to attempt.” - William Cornelius Van Horne
## Introduction
            Nowadays many web applications have the possibility to save and display data in a portable format. In Python/Django applications we have the possibility to display data in a web page, but also save it in such a format. Official Django documentation contains a basic tutorial about saving data in PDF ([Portable Document Format](https://docs.djangoproject.com/en/dev/howto/outputting-pdf)).
            First of all, in order to use this library you need to install it. You can install it using *pip *or *easy_install *by typing: *pip install reportlab* or *easy_install reportab *in your virtual environment. It's fully recommended to use a virtual environment for each of your projects especially if you work on many projects and each of them use different versions of libraries. For example in this application, which is also available on [GitHub](https://github.com/assist-software/python-django-exporting-files), we use these libraries:
- Django==1.8.2
- Pillow==2.8.2
- reportlab==3.2.0
            In a previous article, the main subject was [How to export excel files in a Python/Django application](https://assist-software.net/blog/how-export-excel-files-python-django-application), whereas this article will show you how to export PDF files. This application will extend the one in the previous article; as a recap, the application saves weather data for multiple towns. A user can add different data for weather using the administration interface provided by Django. The home page of application displays today's weather for different towns. The main menu also contains a link to the list of all towns and a link to weather history, which is a page where the user can filter weather history by town and can generate both Excel or PDF files reports.
##  How to serve the file for download
             In order to download the data written in a file format, we can use two buttons on the HTML page, one for Excel and one for PDF. Each of the buttons have an associated name:
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
<button
```
```java
class
```
```java
=
```
```java
"btn btn-default pull-right"
```
```java
name=
```
```java
"pdf"
```
```java
type=
```
```java
"submit"
```
```java
></button>
```
```
Forwarding in the corresponding view we have to check if the button name exists in the request. If it does then we create a HttpResponse object with the corresponding content. Afterwards we have to set the content_type to be “application/pdf”, the Content-Disposition to be an attachment, add a name for filename and the corresponding file type, in our case PDF. This will tell the browser to treat this HttpResponse object as an attachment of pdf type.
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
if
```
```java
'pdf'
```
```java
in request.POST:
```
```java
```
```java
response = HttpResponse(content_type=
```
```java
'application/pdf'
```
```java
)
```
```java
```
```java
today = date.today()
```
```java
```
```java
filename =
```
```java
'pdf_demo'
```
```java
+ today.strftime(
```
```java
'%Y-%m-%d'
```
```java
)
```
```java
```
```java
response[
```
```java
'Content-Disposition'
```
```java
] =
```
```java
'attachement; filename={0}.pdf'
```
```java
.format(filename)
```
```java
```
```java
buffer = BytesIO()
```
```java
```
```java
report = PdfPrint(buffer,
```
```java
'A4'
```
```java
)
```
```java
```
```java
pdf = report.report(weather_period,
```
```java
'Weather statistics data'
```
```java
)
```
```java
```
```java
response.write(pdf)
```
```java
```
```java
return
```
```java
response
```
```
We use a buffer which contains the PDF data. For this we can use BytesIO class that represents a stream implementation using an in-memory bytes buffer. For the filename we created a filename variable by joining a string and current date.
## How to create a PDF document
            In order to be as modular as possible we’ve created a class named PdfPrint that contains several methods for creating different elements. This class has an __init__ method with two parameters: buffer and pagesize. Buffer is used to hold data and pagesize is used to set page type and it's width and height. ReportLab has a series of build-in types: A0 to A6, B0 to B6 and letter type, A4 being the default format if we don't give one. Also you have the possibility to set your own page size by providing custom height and width.
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
def __init__(self, buffer, pageSize):
```
```java
```
```java
self.buffer = buffer
```
```java
```
```java
#
```
```java
default
```
```java
format is A4
```
```java
```
```java
if
```
```java
pageSize ==
```
```java
'A4'
```
```java
:
```
```java
```
```java
self.pageSize = A4
```
```java
```
```java
elif pageSize ==
```
```java
'Letter'
```
```java
:
```
```java
```
```java
self.pageSize = letter
```
```java
```
```java
self.width, self.height = self.pageSize
```
```
The lowest level interface used to generate PDF files is Canvas. Canvas can be seen as a white paper where you can “draw” elements like text, images or other graphics elements.
            In order to create more complex documents we need to use Platypus (Page Layout and Typography Using Scripts), a high level layout library of ReportLab. Platypus has several layers: DocTemplates, PageTemplates, Frames, Flowables and Canvas. DocTemplates and PageTemplates are used to set document specifications layout. Flowables it is used to wrap, draw or split text or graphics elements in the document. Main flowables used to draw text elements are Paragraph, Spacer and Table. In the next parts of this article we will try to explain how to add such elements in a PDF file.
            For the PDF template pages we used SimpleDocTemplate class, a special case document template that can handle multiple simple documents. The code presented bellow is used to set document characteristics (set the margins and the page size). After setting document characteristics, we've created a list used to add flowable elements. At this point we only add title to this list.
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
def report(self, weather_history, title):
```
```java
```
```java
# set some characteristics
```
```java
for
```
```java
pdf document
```
```java
```
```java
doc = SimpleDocTemplate(
```
```java
```
```java
self.buffer,
```
```java
```
```java
rightMargin=
```
```java
72
```
```java
,
```
```java
```
```java
leftMargin=
```
```java
72
```
```java
,
```
```java
```
```java
topMargin=
```
```java
30
```
```java
,
```
```java
```
```java
bottomMargin=
```
```java
72
```
```java
,
```
```java
```
```java
pagesize=self.pageSize)
```
```java
```
```java
styles = getSampleStyleSheet()
```
```java
```
```java
# create document
```
```java
```
```java
data = []
```
```java
```
```java
data.append(Paragraph(title, styles[
```
```java
'Title'
```
```java
]))
```
```java
```
```java
# create other flowables
```
```java
```
```java
doc.build(data)
```
```java
```
```java
pdf = self.buffer.getvalue()
```
```java
```
```java
self.buffer.close()
```
```java
```
```java
return
```
```java
pdf
```
```
## How to add paragraphs and spaces in between
            This section shows you how to add some paragraphs to the document. In order to do this we first need to use a certain style for the paragraph. ReportLab library comes with a set of predefined styles such as: Title, Heading1 to Heading6, Italic, Normal, etc. In order to use these styles we need to use this function getSampleStyleSheet:
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
styles = getSampleStyleSheet()
```
```
But we can also add our custom text style. To do this we can use the ParagraphStyle class.
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
styles.add(ParagraphStyle( name=
```
```java
"ParagraphTitle"
```
```java
, fontSize=
```
```java
11
```
```java
, alignment=TA_JUSTIFY, fontName=
```
```java
"FreeSansBold"
```
```java
))
```
```
This class construction method has several parameters, most used parameters are:
- *name* -  the name of the new style
- *fontSize *– the dimension of the new style
- *alignment *– the style alignment, it can be: TA_JUSTIFY, TA_CENTER, TA_LEFT or TA_RIGHT
- *fontName *– you can use 14 predefined fonts offered by ReportLab or you can define your own one
To use other fonts than the predefined ones you need to add the specific TTF font file in the static folder. Afterwards you need to register that font like this:
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
pdfmetrics.registerFont(TTFont(
```
```java
'FreeSans'
```
```java
, settings.STATIC_ROOT+
```
```java
'FreeSans.ttf'
```
```java
))
```
```
This feature offered by ReportLab library is very important, especially if you need to write in a language that uses special characters, others than the ones in English.
            After setting your custom paragraph styles it is time to use it. In order to do this, simply create an empty list and add to it any element that you want on your document. To add a paragraph in your document use Paragraph class by instantiating an object with the text you want to appear and the paragraph style.
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
Paragraph(
```
```java
'Paragraph example'
```
```java
, styles[
```
```java
'Title'
```
```java
])
```
```
But, between paragraphs or other elements there should be empty spaces. To do this ReportLab library came with a specific class called Spacer. So in order to create custom spaces between document elements we can use this method by adding it as an element on the page* Spacer(width, height)*.
Usually a PDF document can contain more than paragraphs, for example, we may want to add a table. This library came with specific classes for adding tables, you can find out more about that in the next section.
## How to add a table
Another important flowable element of ReportLab library is represented by tables. The class used to draw a table into a document is named *Table*. The __init__ method for this class has one mandatory parameter: a list with table data including the table header. It can also have optional parameters used to customize rows and columns: *colsWidths *or *rowHeights*. Official ReportLab library documentation tells us that “*a cell table can contain anything that can be converted to string or flowable*”.
In order to set some custom specifications for our table we can use *TableStyle *class. To customize our table we call *setStyle *function with an *TableStyle *object as parameter. This class came with a list of tuple commands, each of the tuples being represented like this:
- first element is the attached name;
- second and third are coordinates for starting and stopping points for the cells that will be affected;
- other options like color or thickness.
In our example INNERGRID and BOX are used to set the black color and thickness of 0.25 for the cell lines, VALIGN and BACKGROUND are used to set the text alignment and background color for the table header only.
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
wh_table.setStyle(TableStyle(
```
```java
```
```java
[(
```
```java
'INNERGRID'
```
```java
, (
```
```java
0
```
```java
,
```
```java
0
```
```java
), (-
```
```java
1
```
```java
, -
```
```java
1
```
```java
),
```
```java
0.25
```
```java
, colors.black),
```
```java
```
```java
(
```
```java
'BOX'
```
```java
, (
```
```java
0
```
```java
,
```
```java
0
```
```java
), (-
```
```java
1
```
```java
, -
```
```java
1
```
```java
),
```
```java
0.5
```
```java
, colors.black),
```
```java
```
```java
(
```
```java
'VALIGN'
```
```java
, (
```
```java
0
```
```java
,
```
```java
0
```
```java
), (-
```
```java
1
```
```java
,
```
```java
0
```
```java
),
```
```java
'MIDDLE'
```
```java
),
```
```java
```
```java
(
```
```java
'BACKGROUND'
```
```java
, (
```
```java
0
```
```java
,
```
```java
0
```
```java
), (-
```
```java
1
```
```java
,
```
```java
0
```
```java
), colors.gray)]))
```
```
After setting the style for our table we need to add it into the document like any ordinary flowable  element by calling append function with table name as parameter.
![Reportlab table data](https://cdnsite1.assist.ro/sites/default/files/styles/big/public/images/blog/reportlab-table-data-python-django.png?itok=iFG3xWot)
In other projects we may need to show data in a more friendly and easy way to visualize it, other than displaying it in a table. For this we need to create charts based on that data.
## How to add charts in PDF documents
            ReportLab library offers support to add some other graphics elements like charts. The library can be used to generate different types of charts like line, bar or pie charts. In order to create a chart first we need to create a drawing area by instantiating a Drawing object. Next we need to draw the chart into this area: * d = Drawing(0, 170).*
            For all our charts we've created other two methods to set chart title and chart legend. The method used to “draw” the chart title creates a Label object and sets a specific font, size and position for it.
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
chart_title = Label()
```
```java
```
```java
chart_title.fontName =
```
```java
'FreeSansBold'
```
```java
```
```java
chart_title.fontSize =
```
```java
16
```
```java
```
```java
chart_title.textAnchor =
```
```java
'middle'
```
```
The second method used, called *legend_draw *has two mandatory parameters: legend labels, a chart object and also **kwargs in order to provide an arbitrary number of custom arguments for each type of chart.
            Firstly, we need to create a Legend object and afterwards set some characteristics for it, beginning with the start position point and labels alignment.
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
legend = Legend()
```
```java
legend.x = kwargs[
```
```java
'x'
```
```java
]
```
```java
legend.y = kwargs[
```
```java
'y'
```
```java
]
```
```java
legend.alignment =
```
```java
'right'
```
```
Other characteristics that can be set for the legend:
- boxAnchor – sets anchor point for the legend area
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
legend.boxAnchor = kwargs[
```
```java
'boxAnchor'
```
```java
]
```
```
- columnMaximum – sets the maximum number of items included in one column
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
legend.columnMaximum = kwargs[
```
```java
'columnMaximum'
```
```java
]
```
```
- deltax – sets the distance between neighbouring items
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
legend.deltax =
```
```java
0
```
```
- colorNamePairs – creates a list of color/item name tuples using zip function
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
legend.colorNamePairs = zip(lcolors, labels)
```
```
Next using enumerate python feature we set the interior colors of the plot area according to the specific chart:
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
for
```
```java
i, color in enumerate(lcolors):
```
```java
```
```java
if
```
```java
chart_type ==
```
```java
'line'
```
```java
:
```
```java
```
```java
chart.lines[i].fillColor = color
```
```java
```
```java
elif chart_type ==
```
```java
'pie'
```
```java
:
```
```java
```
```java
chart.slices[i].fillColor = color
```
```java
```
```java
elif chart_type ==
```
```java
'bar'
```
```java
:
```
```java
```
```java
chart.bars[i].fillColor = color
```
```
Because we need to serve data into a certain format I have created some other functions in a file called utils.py. I've also created a function that generates random colors for the bar and pie charts.
### a. How to add line charts
            In our example we want to add a line chart that will display the data from a period of time with the minimum and maximum registered temperatures. The library came with some classes used to create line charts like *AbstractLineChart*, *LineChart*, *HorizontalLineChart*, *HorizontalLineChart3D *or *SampleHorizontalLineChart*. We use the *SampleHorizontalLineChart *which offers many simple methods to create a line chart.
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
chart = SampleHorizontalLineChart()
```
```
After creating the chart object, it is time to set some custom properties: width, height and some other characteristics for our chart:
- data – the displayed data, it's format being a list of tuples
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
chart.data = values
```
```
- lineLabels.fontName – sets the desired font for data lines
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
chart.lineLabels.fontName =
```
```java
'FreeSans'
```
```
- strokeColor and fillColor - sets colors for the plot area border and interior area
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
chart.strokeColor = colors.white
```
```java
chart.fillColor = colors.lightblue
```
```
These parameters can also set attributes for lines; color and line width:
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
chart.lines[
```
```java
0
```
```java
].strokeColor = colors.red
```
```java
chart.lines[
```
```java
0
```
```java
].strokeWidth =
```
```java
2
```
```
- lines.symbol - sets markers for points, you can chose from a list of markers types: Square, Circle, Diamond, StarFive, Triangle, Pentagon, StarSix, Heptagon, Octagon, Cross or even other symbols like UK_Flag
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
chart.lines.symbol = makeMarker(
```
```java
'Square'
```
```java
)
```
```
- lineLabelFormat – sets the format used for the points displayed in the plot
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
chart.lineLabelFormat =
```
```java
'%2.0f'
```
```
- joinAxisMode – this attribute is used to set the intersecting point for the axes
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
chart.categoryAxis.joinAxisMode =
```
```java
'bottom'
```
```
- labels – this attributes can be used to set some characteristic for axes labels like:
- 
fontName – sets font
- 
angle – sets text inclination
- 
boxAnchor – sets label position
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
chart.categoryAxis.labels.fontName =
```
```java
'FreeSans'
```
```java
chart.categoryAxis.labels.angle =
```
```java
45
```
```java
chart.categoryAxis.labels.boxAnchor =
```
```java
'e'
```
```
- categoryNames – sets labels for X axis
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
chart.categoryAxis.categoryNames = days
```
```
- labelTextFormat – sets the format displayed for Y axis
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
chart.valueAxis.labelTextFormat =
```
```java
'%2.0f °C'
```
```
-  valueStep – sets the steps for Y axis
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
chart.valueAxis.valueStep =
```
```java
10
```
```
And here is the result of setting all those characteristics:
![Reportlab line chart](https://d2z1ksq6nul58p.cloudfront.net/sites/default/files/styles/big/public/images/blog/reportlab-line-chart.png?itok=0Cwcyw2A)
You should know that this data is not real and I chose to add some negative values for temperatures only to show how you can set some characteristics for line charts.
### b. How to add bar charts
            Another chart type used to display data in a schematic mode is bar chart. ReportLab library comes with several types of bar charts: BarChart3D, HorizontalBarChart, HorizontalBarChart3D, VerticalBarChart or VerticalBarChart3D. In our example we used  VerticalBarChart to display the  wind speed statistic for all towns for a frame of time.
            Firstly we create an  *VerticalBarChart object bc = VerticalBarChart()* and subsequently we can set characteristics for this object. The characteristics that we can set are:
- width
- height
- data - list or lists of numbers
- barSpacing – sets the bar width between bars
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
bc.barSpacing =
```
```java
0.5
```
```
- labels – sets some characteristics for the labels that appear in the X axis. In our example we set the position
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
bc.categoryAxis.labels.dx =
```
```java
8
```
```java
bc.categoryAxis.labels.dy = -
```
```java
2
```
```
- categoryNames – sets the names displayed on the X axis
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
bc.categoryAxis.categoryNames = days
```
```
- barLabelFormat – sets the label format for the bars
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
bc.barLabelFormat =
```
```java
'%d'
```
```
- barLabels – sets some characteristics for the bar charts; we used nudge that sets distance between top of a bar and it's label
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
bc.barLabels.nudge =
```
```java
7
```
```
- valueAxis – sets the characteristics for the Y axis, for example
- labelTextFormat – to set the labels format
- valueMin - to set the minimum value for the Y axis
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
bc.valueAxis.labelTextFormat =
```
```java
'%d km/h'
```
```java
bc.valueAxis.valueMin =
```
```java
0
```
```
After setting all these characteristics we can see the result:
![Reportlab vertical bar chart](https://cdnsite1.assist.ro/sites/default/files/styles/big/public/images/blog/reportlab-verticalbar-chart.png?itok=hfIULdUp)
### c. How to add pie charts
            There are some projects where you need to display data that is linked. Each piece of data represents a proportional or percentage part of the total. Therefore, the best approach for this is representing it by pie charts. ReportLab library came with several classes that can be used to create pie charts: Pie, LegendedPie, Doughnut and Pie3d
            In our example we want to create a pie chart that will show a statistic regarding the percentage of the precipitation probability. We divided the precipitation probability in five categories: between 0-20, 21-40, 41-60, 61-80 and 81-100 percentage. We used the Pie class to create our chart, for this we set the x and y coordinates and some other characteristics:
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
pc = Pie()
```
```java
pc.x =
```
```java
0
```
```java
pc.y =
```
```java
50
```
```
- data – sets the data for the chart (list of numbers)
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
pc.data = values
```
```
- sideLabels – adds a line that link the label and it's specific slice
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
pc.sideLabels =
```
```java
1
```
```
- slices – sets some characteristics for the component slices, for example:
- strokeWidth – sets the width of line between slices
- 
 strokeColor – sets the line color
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
pc.slices.strokeWidth =
```
```java
0
```
```java
pc.slices.strokeColor = None
```
```
After setting these characteristics to our pie, it is time to see the result:
![Reportlab pie chart](https://cdnsite1.assist.ro/sites/default/files/styles/big/public/images/blog/reportlab-pie-chart.png?itok=J64xJYn2)
## How to add page numbers to a PDF document
            In some cases when your documents have more than one page you can add numbering. In order to apply numbering on the document pages we've created a method that will draw the numbers on the each page of document. To get the number of current page we used *getPageNumber *method that returns the page number for each page being generated. After we get the number we draw it in the page at a certain position by using the *drawCenteredString *method that places the given number on the canvas.
[?](https://assist-software.net/blog/how-create-pdf-files-python-django-application-using-reportlab#)
```
```java
def pageNumber(self, canvas, doc):
```
```java
```
```java
number = canvas.getPageNumber()
```
```java
```
```java
canvas.drawCentredString(
```
```java
100
```
```java
*mm,
```
```java
15
```
```java
*mm, str(number))
```
```
            We've used this function when we've called the build function for the *SimpleDocTemplate *and set the *onFirstPage* and *onLaterPages*  parameters.
## In conclusion
In the end, we can say that ReportLab library represents a very powerful tool. Moreover, I think it is an easier enough tool that can help a developer to create portable documents using Python language and integrate it into a Django project. This article tries to show you how to setup and use some of the features offered by ReportLab library to create PDF files.
You can access the entire source code of this application by accessing [GitHub](https://github.com/assist-software/python-django-exporting-files) . I hope this article is useful and  helps you when need to create and export data into PDF documents in a Python/Django application. If you want to do more than this and create more complex documents or use other options you can access the official [ReportLab library documentation](https://www.reportlab.com/documentation/).
However, if you need to export your data in other formats, like *xls* for example, you can check this article: [How to export excel files in a python/django application](https://assist-software.net/blog/how-export-excel-files-python-django-application).
