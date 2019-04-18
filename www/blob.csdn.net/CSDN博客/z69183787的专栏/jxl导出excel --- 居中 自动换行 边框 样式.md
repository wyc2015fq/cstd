# jxl导出excel --- 居中 自动换行 边框 样式 - z69183787的专栏 - CSDN博客
2014年02月13日 20:39:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：10541
```java
public void writeToExl_GW(ByteArrayOutputStream byteOs)throws Exception{ 
        try{ 
            int timeLength = 0; 
            ITrendService trendService = new TrendServiceImpl(); 
            if(null != stepTime && !"".equals(stepTime)){ 
                timeLength = Integer.valueOf(stepTime); 
            } 
            gwjcList=trendService.querygwjcList(beginTime,endTime,timeLength); 
            String filename="轨温监测历史查询"+ new SimpleDateFormat("yyyy-MM-dd").format(new Date(System.currentTimeMillis())) + ".xls";  
            exportFileName = new String(filename.getBytes("GBK"), "ISO-8859-1"); 
            WritableWorkbook wbook = Workbook.createWorkbook(byteOs); //建立excel文件  
            WritableSheet wsheet = wbook.createSheet("轨温监测历史查询", 0); //工作表名称  
            WritableFont wfont1 = new WritableFont(WritableFont.ARIAL, 9,      //初始化一些样式 
                    WritableFont.NO_BOLD, false, 
                    jxl.format.UnderlineStyle.NO_UNDERLINE, jxl.format.Colour.RED); 
            WritableFont wfont2 = new WritableFont(WritableFont.ARIAL, 10); 
            WritableFont wfont3 = new WritableFont(WritableFont.ARIAL, 9, 
                    WritableFont.NO_BOLD, false, 
                    jxl.format.UnderlineStyle.NO_UNDERLINE, 
                    jxl.format.Colour.AUTOMATIC); 
            WritableFont wfont4 = new WritableFont(WritableFont.ARIAL, 10); 
            WritableCellFormat titleFormat1 = new WritableCellFormat(wfont1); 
            WritableCellFormat titleFormat2 = new WritableCellFormat(wfont2); 
            WritableCellFormat titleFormat3 = new WritableCellFormat(wfont3); 
            WritableCellFormat titleFormat3_1 = new WritableCellFormat(wfont3); 
            WritableCellFormat titleFormat4 = new WritableCellFormat(wfont4); 
            titleFormat1.setVerticalAlignment(VerticalAlignment.CENTRE);     
            titleFormat1.setAlignment(Alignment.CENTRE); 
            titleFormat1.setWrap(true); 
               
            titleFormat2.setVerticalAlignment(VerticalAlignment.CENTRE);   //设置居中对齐 
            titleFormat2.setAlignment(Alignment.CENTRE);//设置居中对齐（这俩哪个是上下/左右对齐也没验证过） 
            titleFormat2.setBackground(jxl.format.Colour.GREY_25_PERCENT);//单元格背景色 
            titleFormat3.setVerticalAlignment(VerticalAlignment.CENTRE); 
            titleFormat3.setAlignment(Alignment.CENTRE); 
            titleFormat3.setWrap(true);//是否自动换行 
            titleFormat3.setBorder(jxl.format.Border.ALL,jxl.format.BorderLineStyle.THIN);//给单元格加边框          titleFormat3_1.setVerticalAlignment(VerticalAlignment.CENTRE); 
            titleFormat3_1.setAlignment(Alignment.CENTRE); 
            titleFormat3_1.setWrap(true); 
            titleFormat3_1.setBackground(jxl.format.Colour.ORANGE); 
            titleFormat3_1.setBorder(jxl.format.Border.ALL,jxl.format.BorderLineStyle.THIN);//Biankuang 
            titleFormat4.setVerticalAlignment(VerticalAlignment.CENTRE); 
            titleFormat4.setAlignment(Alignment.CENTRE); 
            titleFormat4.setBackground(jxl.format.Colour.GRAY_25); 
            titleFormat4.setBorder(jxl.format.Border.ALL,jxl.format.BorderLineStyle.HAIR);//Biankuang 
            StringBuffer title = new StringBuffer(); 
            title.append("轨温监测查询条件:"); 
            title.append("开始时间 ["); 
            title.append(beginTime); 
            title.append("]"); 
            title.append(" 结束时间:["); 
            title.append(endTime); 
            title.append("]"); 
            wsheet.setColumnView(0, 19); 
            wsheet.setRowView(0, 600); 
            wsheet.addCell(new Label(0,0,title.toString(),titleFormat1)); 
            if("1".equals(tdzt)){ 
                title.append(" 启用通道状态"); 
                //设置表头行合并单元格 
                wsheet.mergeCells(0, 0, 9, 0); 
                int rows =1; 
                wsheet.addCell(new Label(0,rows,"时间",titleFormat4)); 
                wsheet.addCell(new Label(1,rows,"左温度(℃)",titleFormat4)); 
                wsheet.addCell(new Label(2,rows,"右温度(℃)",titleFormat4)); 
                wsheet.addCell(new Label(3,rows,"环温(℃)",titleFormat4)); 
                wsheet.addCell(new Label(4,rows,"通道一",titleFormat4)); 
                wsheet.addCell(new Label(5,rows,"通道二",titleFormat4)); 
                wsheet.addCell(new Label(6,rows,"通道三",titleFormat4)); 
                wsheet.addCell(new Label(7,rows,"通道四",titleFormat4)); 
                wsheet.addCell(new Label(8,rows,"通道五",titleFormat4)); 
                wsheet.addCell(new Label(9,rows,"通道六",titleFormat4)); 
                   
                for(int i=0;i<gwjcList.size();i++){ 
                    wsheet.addCell(new Label(0,rows+1,gwjcList.get(i).getSj(),titleFormat3)); 
                    wsheet.addCell(new Label(1,rows+1,gwjcList.get(i).getZwd(),titleFormat3)); 
                    wsheet.addCell(new Label(2,rows+1,gwjcList.get(i).getYwd(),titleFormat3)); 
                    wsheet.addCell(new Label(3,rows+1,gwjcList.get(i).getHw(),titleFormat3)); 
                    wsheet.addCell(new Label(4,rows+1,Integer.valueOf(gwjcList.get(i).getTd1zt())==1?"正   常":"异常",Integer.valueOf(gwjcList.get(i).getTd1zt())==1?titleFormat3:titleFormat3_1)); 
                    wsheet.addCell(new Label(5,rows+1,Integer.valueOf(gwjcList.get(i).getTd2zt())==1?"正常":"异常",Integer.valueOf(gwjcList.get(i).getTd2zt())==1?titleFormat3:titleFormat3_1)); 
                    wsheet.addCell(new Label(6,rows+1,Integer.valueOf(gwjcList.get(i).getTd3zt())==1?"正常":"异常",Integer.valueOf(gwjcList.get(i).getTd3zt())==1?titleFormat3:titleFormat3_1)); 
                    wsheet.addCell(new Label(7,rows+1,Integer.valueOf(gwjcList.get(i).getTd4zt())==1?"正常":"异常",Integer.valueOf(gwjcList.get(i).getTd4zt())==1?titleFormat3:titleFormat3_1)); 
                    wsheet.addCell(new Label(8,rows+1,Integer.valueOf(gwjcList.get(i).getTd5zt())==1?"正常":"异常",Integer.valueOf(gwjcList.get(i).getTd5zt())==1?titleFormat3:titleFormat3_1)); 
                    wsheet.addCell(new Label(9,rows+1,Integer.valueOf(gwjcList.get(i).getTd6zt())==1?"正常":"异常",Integer.valueOf(gwjcList.get(i).getTd6zt())==1?titleFormat3:titleFormat3_1)); 
                    rows= rows +1; 
                } 
            }else{ 
                wsheet.mergeCells(0, 0, 3, 0); 
                int rows =1; 
                wsheet.addCell(new Label(0,rows,"时间",titleFormat4)); 
                wsheet.addCell(new Label(1,rows,"左温度(℃)",titleFormat4)); 
                wsheet.addCell(new Label(2,rows,"右温度(℃)",titleFormat4)); 
                wsheet.addCell(new Label(3,rows,"环温(℃)",titleFormat4)); 
                for(int i=0;i<gwjcList.size();i++){ 
                    wsheet.addCell(new Label(0,rows+1,gwjcList.get(i).getSj(),titleFormat3)); 
                    wsheet.addCell(new Label(1,rows+1,gwjcList.get(i).getZwd(),titleFormat3)); 
                    wsheet.addCell(new Label(2,rows+1,gwjcList.get(i).getYwd(),titleFormat3)); 
                    wsheet.addCell(new Label(3,rows+1,gwjcList.get(i).getHw(),titleFormat3)); 
                    rows = rows +1 ; 
                } 
            } 
            contentType = "application/vnd.ms-excel"; 
            wbook.write(); 
            wbook.close(); 
        }catch(Exception e){ 
            e.printStackTrace(); 
        } 
    } 
    public String exportGW(){ 
        try{ 
            ByteArrayOutputStream byteOs = new ByteArrayOutputStream(); 
            writeToExl_GW(byteOs); 
            byteOs.flush(); 
            excelStream = new ByteArrayInputStream(byteOs.toByteArray(),0,byteOs.toByteArray().length); 
            byteOs.close(); 
        }catch(Exception e){ 
            e.printStackTrace(); 
        } 
        return SUCCESS; 
    }
```
