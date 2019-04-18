# 使用POI导出EXCEL 并设置格式 - z69183787的专栏 - CSDN博客
2014年08月26日 14:45:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2064
```java
public void exportJgjExcel(String title,List<String[]> header,List<Object[]> dataset, OutputStream out, short[] width,List<int[]> merge){
		// 声明一个工作薄
		HSSFWorkbook workbook = new HSSFWorkbook();
		// 生成一个表格
		HSSFSheet sheet = workbook.createSheet(title);
		// 设置表格默认列宽度为15个字节
		sheet.setDefaultColumnWidth((short) 15);
		// 生成一个样式
		HSSFCellStyle style = workbook.createCellStyle();
		// 设置这些样式
		style.setFillForegroundColor(HSSFColor.LIGHT_YELLOW.index);
		style.setFillPattern(HSSFCellStyle.SOLID_FOREGROUND);
		style.setBorderBottom(HSSFCellStyle.BORDER_THIN);
		style.setBorderLeft(HSSFCellStyle.BORDER_THIN);
		style.setBorderRight(HSSFCellStyle.BORDER_THIN);
		style.setBorderTop(HSSFCellStyle.BORDER_THIN);
		style.setAlignment(HSSFCellStyle.ALIGN_CENTER);
		// 生成一个字体
		HSSFFont font = workbook.createFont();
		font.setColor(HSSFColor.VIOLET.index);
		font.setFontHeightInPoints((short) 12);
		font.setBoldweight(HSSFFont.BOLDWEIGHT_BOLD);
		// 把字体应用到当前的样式
		style.setFont(font);
		// 生成并设置另一个样式
		HSSFCellStyle style2 = workbook.createCellStyle();
		style2.setFillForegroundColor(HSSFColor.WHITE.index);
		style2.setFillPattern(HSSFCellStyle.SOLID_FOREGROUND);
		style2.setBorderBottom(HSSFCellStyle.BORDER_THIN);
		style2.setBorderLeft(HSSFCellStyle.BORDER_THIN);
		style2.setBorderRight(HSSFCellStyle.BORDER_THIN);
		style2.setBorderTop(HSSFCellStyle.BORDER_THIN);
		style2.setAlignment(HSSFCellStyle.ALIGN_CENTER);
		style2.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);
		// 生成另一个字体
		HSSFFont font2 = workbook.createFont();
		font2.setBoldweight(HSSFFont.BOLDWEIGHT_NORMAL);
		// 把字体应用到当前的样式
		style2.setFont(font2);
		// 产生表格标题行
		HSSFRow row = sheet.createRow(0);
		for(int i=0;i<header.size();i++){
			row = sheet.createRow(i);
			for(int m=0;m<header.get(i).length;m++){
				HSSFCell cell = row.createCell(m);
				cell.setCellStyle(style);
				String headValue = header.get(i)[m];
				//HSSFRichTextString text = new HSSFRichTextString(header.get(i)[m]);
				cell.setCellValue(headValue);
			}
			
		}
		
		if(merge!=null && merge.size()>0){
			for(int m=0;m<merge.size();m++){
				sheet.addMergedRegion(new CellRangeAddress(merge.get(m)[0],merge.get(m)[1],merge.get(m)[2],merge.get(m)[3]));		//合并行
			}
		}
		
		
		//sheet.addMergedRegion(new CellRangeAddress(0,0,9,10));
//		sheet.addMergedRegion(new CellRangeAddress(0,0,11,12));
		
		
		for(int j=0;j<dataset.size();j++){
			row = sheet.createRow(j+header.size());
			
			for (short i = 0; i < dataset.get(j).length; i++) {
				HSSFCell cell = row.createCell(i);				
				cell.setCellStyle(style2);
				HSSFRichTextString richString = new HSSFRichTextString(
				String.valueOf( dataset.get(j)[i]).replace("null", ""));
//							HSSFFont font3 = workbook.createFont();
//							font3.setColor(HSSFColor.BLACK.index);
//							richString.applyFont(font3);
				cell.setCellValue(richString);
			}
		}
		
		if(width!=null){
			for(short i=0;i<width.length;i++){
				sheet.setColumnWidth(i,width[i]);
			}
		}
		try {
			workbook.write(out);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
```
调用方法：
```java
public String showReportProjectCount() throws UnsupportedEncodingException{
		String year = servletRequest.getParameter("year");
		List<String> allYear = jgjReportService.findAllYearJgjProjectCount(); 
		servletRequest.setAttribute("allYear", allYear);
		if(StringUtils.isEmpty(year)){
			if(allYear!=null && allYear.size()>0){
				year = allYear.get(0);
			}else{
				year = Calendar.getInstance().get(Calendar.YEAR)+"";
			}
		}
		servletRequest.setAttribute("year", year);
		List<DwJgjProjectCount> list = jgjReportService.findByYearAndTypeJgjProjectCount(year);
		servletRequest.setAttribute("list", list);
		
		String export = servletRequest.getParameter("export");
		if(StringUtils.isNotBlank(export) && "yes".equals(export)){
			this.servletResponse.setContentType("octets/stream");  
			String filename="上海申通地铁集团有限公司"+year+"年度轨道交通运营设施设备大修和更新改造项目资金计划申请表(项目详表)";
						    
			filename = new String(filename.getBytes("gb2312"),"iso8859-1");
			String sheettitle = "资金计划完成情况表(项目详表)";
			
			this.servletResponse.addHeader("Content-Disposition","attachment;filename="+filename+".xls"); 
			List<String[]> header = new ArrayList<String[]>();
			String[] head1 = {"专业","立项依据（集团批文号）","项目名称","执行主体","费用承担单位","费用承担金额","批准预算金额","维修类型","合同价总计","决算价（若有）","至上年底累计已支付","","本年度用款计划",""};
			String[] head2 = {"","","","","","","","","","","总数","其中政府补贴数","总数","其中政府补贴数"};
			short[] width = {2000,10000,10000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000};
			List<int[]> merge = new ArrayList<int[]>();			
			merge.add(new int[]{0,1,0,0});		//{firstRow,lastRow,firstCol,lastCol}
			for(int m=0;m<=9;m++){
				merge.add(new int[]{0,1,m,m});	//合并第0,1行的第m列
			}
			merge.add(new int[]{0,0,10,11});			//合并第0行的9,10列
			merge.add(new int[]{0,0,12,13});			//合并第0行的11,12列
			
			header.add(head1);
			header.add(head2);
			List<Object[]> dataset = new ArrayList<Object[]>();
			if(list!=null && list.size()>0){
				for(int i=0;i<list.size();i++){
					DwJgjProjectCount target = list.get(i);
					Object[] data = new Object[14];
					data[0] = transProfessionalType(target.getProfessionalType());
					data[1] = target.getDispatchNo();
					data[2] = target.getProjectName();
					data[3] = target.getMainExecute();
					data[4] = target.getFeeDepartment();
					data[5] = target.getFeeMoney();
					
					data[6] = target.getApprovalBugget();
					
					data[7] = target.getMaintainType();
					if(data[7]!=null && "2".equals(data[7].toString())){
						data[7] = "更新改造";
					}else if(data[7]!=null && "3".equals(data[7].toString())){
						data[7] = "大修";
					}else {
						data[7] ="";
					}
					data[8] = target.getContractPrice();
					data[9] = target.getFinalPrice();
					data[10] = target.getLastYearPayCount();
					data[11] = target.getLastYearPayGov();
					data[12] = target.getThisYearPlanCount();
					data[13] = target.getThisYearPlanGov();
					dataset.add(data);
				}
			}
			try {
				OutputStream out = this.servletResponse.getOutputStream();
				ExportExcel ee = new ExportExcel();
				ee.exportJgjExcel(sheettitle, header, dataset, out,width,merge); 
				out.close();  
			} catch (IOException e) {
				e.printStackTrace();
			}  
			return null;
		}
		return "success";
	}
```
