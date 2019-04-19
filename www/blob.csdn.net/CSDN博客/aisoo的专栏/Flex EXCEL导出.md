# Flex EXCEL导出 - aisoo的专栏 - CSDN博客
2010年08月23日 13:55:00[aisoo](https://me.csdn.net/aisoo)阅读数：815标签：[flex																[excel																[datagrid																[string																[colors																[variables](https://so.csdn.net/so/search/s.do?q=variables&t=blog)](https://so.csdn.net/so/search/s.do?q=colors&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=datagrid&t=blog)](https://so.csdn.net/so/search/s.do?q=excel&t=blog)](https://so.csdn.net/so/search/s.do?q=flex&t=blog)
flex端：
     <?xml version = "1.0" encoding = "utf-8"?>
<mx:Application xmlns:mx = "[http://www.adobe.com/2006/mxml](http://www.adobe.com/2006/mxml)"
                layout = "absolute">
    <mx:Script>
        <![CDATA[
            import flash.errors.*;
            import flash.events.*;
            import flash.external.*;
            import flash.net.URLRequest;
            import flash.net.URLVariables;
            import mx.collections.ArrayCollection;
            [Bindable]
            private var datalist:ArrayCollection =
                            new ArrayCollection([{ 姓名: "张三", 性别: "男", 年龄: "23", 生日: "1984-01-12" },
                                                 { 姓名: "李四", 性别: "男", 年龄: "27", 生日: "1983-11-2" }]);
            /**
             * Convert the datagrid to a html table
             * Styling etc. can be done externally
             *
             * @param: dg Datagrid Contains the datagrid that needs to be converted
             * @returns: String
             */
            private function convertDGToHTMLTable(dg:DataGrid):String
            {
                //Set default values
                var font:String = dg.getStyle('fontFamily');
                var size:String = dg.getStyle('fontSize');
                var str:String = '';
                var colors:String = '';
                var style:String = 'style="font-family:' + font + ';font-size:' + size + 'pt;"';
                var hcolor:Array;
                //Retrieve the headercolor
                if (dg.getStyle("headerColor") != undefined)
                {
                    hcolor = [ dg.getStyle("headerColor")];
                }
                else
                {
                    hcolor = dg.getStyle("headerColors");
                }
                //Set the htmltabel based upon knowlegde from the datagrid
                //解决乱码<head><meta http-equiv="Content-Type" content="text/htm; charset=utf-8"></head>
                str +=
                                '<head><meta http-equiv="Content-Type" content="text/htm; charset=utf-8"></head><table width="' + dg.width + '" border="1"><thead><tr width="' + dg.width + '" style="background-color:#' + Number((hcolor[0])).toString(16) + '">';
                //Set the tableheader data (retrieves information from the datagrid header    
                for (var i:int = 0; i < dg.columns.length; i++)
                {
                    colors = dg.getStyle("themeColor");
                    if (dg.columns[i].headerText != undefined)
                    {
                        str += "<th " + style + ">" + dg.columns[i].headerText + "</th>";
                    }
                    else
                    {
                        str += "<th " + style + ">" + dg.columns[i].dataField + "</th>";
                    }
                }
                str += "</tr></thead><tbody>";
                colors = dg.getStyle("alternatingRowColors");
                //Loop through the records in the dataprovider and 
                //insert the column information into the table
                for (var j:int = 0; j < dg.dataProvider.length; j++)
                {
                    str += "<tr width=/"" + Math.ceil(dg.width) + "/">";
                    for (var k:int = 0; k < dg.columns.length; k++)
                    {
                        //Do we still have a valid item?      
                        if (dg.dataProvider.getItemAt(j) != undefined &&
                                        dg.dataProvider.getItemAt(j) != null)
                        {
                            //Check to see if the user specified a labelfunction which we must
                            //use instead of the dataField
                            if (dg.columns[k].labelFunction != undefined)
                            {
                                str +=
                                                "<td width=/"" + Math.ceil(dg.columns[k].width) + "/" " + style + ">" + dg.columns[k].labelFunction(dg.dataProvider.getItemAt(j),
                                                                                                                                                                              dg.columns[k].dataField) + "</td>";
                            }
                            else
                            {
                                //Our dataprovider contains the real data
                                //We need the column information (dataField)
                                //to specify which key to use.
                                str +=
                                                "<td width=/"" + Math.ceil(dg.columns[k].width) + "/" " + style + ">" + dg.dataProvider.getItemAt(j)[dg.columns[k].dataField] + "</td>";
                            }
                        }
                    }
                    str += "</tr>";
                }
                str += "</tbody></table>";
                return str;
            }
            private function loadDGInExcel(dg:DataGrid, [url:String):void](String)%3Avoid)
            {
                //Pass the htmltable in a variable so that it can be delivered
                //to the backend script
                var variables:URLVariables = new URLVariables();
                variables.htmltable = convertDGToHTMLTable(dg);
                //Setup a new request and make sure that we are 
                //sending the data through a post
                var u:URLRequest = new URLRequest(url);
                u.data = variables; //Pass the variables
                u.method = URLRequestMethod.POST; //Don't forget that we need to send as POST
                //Navigate to the script
                //We can use _self here, since the script will through a filedownload header
                //which results in offering a download to the user (and still remaining in you Flex app.)
                navigateToURL(u, "_self");
            }
        ]]>
    </mx:Script>
    <mx:DataGrid id = "statdatagrid"
                 dataProvider = "{datalist}"
                 width = "460"
                 x = "107"
                 y = "100">
    </mx:DataGrid>
    <mx:Button label = "导出Excel"
               x = "485"
               y = "70"
               click = "loadDGInExcel(statdatagrid,'excelexport.jsp');"/>
</mx:Application>
后台jsp：
        <%@ page language="java"%>
       <%@ page contentType="application/vnd.ms-excel;charset=UTF-8"
                                                                        pageEncoding="UTF-8"%>
       <%
         request.setCharacterEncoding("UTF-8");
         response.setHeader("Content-disposition","attachment; filename=test.xls");
         String str = request.getParameter("htmltable");
         out.print(str);
     %>
