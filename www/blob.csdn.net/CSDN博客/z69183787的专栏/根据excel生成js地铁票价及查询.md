# 根据excel生成js地铁票价及查询 - z69183787的专栏 - CSDN博客
2014年12月26日 09:55:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2490
```java
package com.wonders.stww.price;
import java.io.*;
import java.util.*;
import jxl.*;
import jxl.read.biff.BiffException;
//根据票价映射excel文件，生成票价查询的js文件search.js和price.js
public class GenerateJavascript {
	private TreeMap<String,String> tmCode = new TreeMap<String,String>(new CodeCompare());
	public static void main(String[] args) {
		String filePath = "D:\\xls\\price.xls";
		String outputPath = "D:\\xls\\";
		GenerateJavascript gj = new GenerateJavascript();
		try {
			Sheet[] sheets = gj.getSheets(gj.getWorkbook(filePath));
			// 生成中文的票价查询js
			System.out.println("生成中文票价开始");
			/**/Map map = gj.getStationName2Code(sheets);
			Map map2 = gj.getStationCode2Name(sheets);
			String searchJs = gj.generateSearchJS(map2);//先执行generateSearchJS再执行generatePriceJS
			String priceJs = gj.generatePriceJS(map, sheets);
			String priceDtyyJs = gj.generateSearchDtyyJS(map2);
			gj.generateFile(outputPath + "searchDtyy.js", priceDtyyJs);
			gj.generateFile(outputPath + "Price.js", priceJs);
			gj.generateFile(outputPath + "search.js", searchJs);
			System.out.println("生成中文票价结束");
			/*
			 * //生成英文的票价查询js
			 */
			System.out.println("生成英文票价开始");
			Map mapE = gj.getStationEName2Code(sheets);
			Map mapE2 = gj.getStationCode2EName(sheets);
			String searchJsE = gj.generateSearchJSE(mapE2);
			String priceJsE = gj.generatePriceJSE(mapE, sheets);
			gj.generateFile(outputPath + "PriceE.js", priceJsE);
			gj.generateFile(outputPath + "searchE.js", searchJsE);
			System.out.println("生成英文票价结束");
		} catch (BiffException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	public String generateSearchDtyyJS(Map map) {
		//String temp = "function ShowRoadNew(){document.write('<option value=\"1\">1号线</option>');";
		//String t2 = "var State=new Array();State[0]=new Array(\"2\",\"莘庄\",\"1\");";
		List stationList = (List) map.get("stationList");
		StringBuffer sb = new StringBuffer();
		
		sb.append("function ShowRoad(choose){" + "\n");
		sb.append("	if(choose==undefined){" + "\n");
		sb.append("		document.write('<option value=\"1\">1号线</option>');" + "\n");
		sb.append("		document.write('<option value=\"2\">2号线</option>');" + "\n");
		sb.append("		document.write('<option value=\"3\">3号线</option>');" + "\n");
		sb.append("		document.write('<option value=\"4\">4号线</option>');" + "\n");
		sb.append("		document.write('<option value=\"5\">5号线</option>');" + "\n");
		sb.append("		document.write('<option value=\"6\">6号线</option>');" + "\n");
		sb.append("		document.write('<option value=\"7\">7号线</option>');" + "\n");
		sb.append("		document.write('<option value=\"8\">8号线</option>');" + "\n");
		sb.append("		document.write('<option value=\"9\">9号线</option>');" + "\n");
		sb.append("		document.write('<option value=\"10\">10号线</option>');" + "\n");
		sb.append("		document.write('<option value=\"11\">11号线</option>');" + "\n");
		sb.append("}else{" + "\n");
		sb.append("		for(var i=1;i<12;i++){" + "\n");
					//if(i==10)
						//continue;
					//else{
		sb.append("				if(parseInt(choose)==i)" + "\n");
		sb.append("					document.write('<option value=\"'+i+'\" selected=\"selected\">'+i+'号线</option>');" + "\n");
		sb.append("				else" + "\n");
		sb.append("					document.write('<option value=\"'+i+'\" >'+i+'号线</option>');" + "\n");
					//}
		sb.append("		}" + "\n");
		sb.append("	}" + "\n");
		sb.append("}" + "\n");
		
		sb.append("var State=new Array();" + "\n");
		map.remove("stationList");
		Iterator it = map.entrySet().iterator();
		int stationCount = 0;
		TreeMap tm = new TreeMap(new CodeCompare());
		//TreeMap tmCode = new TreeMap(new CodeCompare());
		//TreeMap<String,String> tempMap = new TreeMap<String,String>(new CodeCompare());
		while (it.hasNext()) {
			Map.Entry e = (Map.Entry) it.next();
			String lineNumber = (String) e.getKey();
			Map stationMap = (Map) e.getValue();
			Iterator sIt = stationMap.entrySet().iterator();
			while (sIt.hasNext()) {
				Map.Entry sE = (Map.Entry) sIt.next();
				String code = (String) sE.getKey();
				String name = (String) sE.getValue();
				if (name != null && !"".equals(name)) {
					// String t
					// ="=new Array(\""+this.modifyCode(code)+"\",\""+name+"\",\""+lineNumber+"\");"+"\n";
					// tm.put(this.modifyCode(code), t);
					
					//this.setCodeMap(new HashMap());
					//this.getCodeMap().put(code, stationCount);
					//String t = "=new Array(\"" + code + "\",\"" + name + "\",\"" + lineNumber + "\");" + "\n";
					//String t = "=new Array(\"" + stationCount + "\",\"" + name + "\",\"" + lineNumber + "\");" + "\n";
					String t = "=new Array(\"" + "<code><code/>" + "\",\"" + name + "\",\"" + lineNumber + "\");" + "\n";
					tm.put(code, t);
					//tempMap.put(code, "");
					//System.out.println(code + " "+String.valueOf(stationCount) );
					stationCount++;
				}
			}
		}
		int ii = 0;
		for (Object o : tm.keySet()) {
			//System.out.println(o+" "+this.getTmCode().get(o));
			//System.out.print(o+" ");
			String v = this.generate(ii);
			this.getTmCode().put((String)o, v);
			ii++;
		}
		
		
		int i = 0;
		for (Object o : tm.keySet()) {
			//System.out.println((String)o+"-----------"+(String)this.getTmCode().get(o));
			//System.out.println((((String)tm.get(o)).replace("\""+(String)o+"\",","\""+(String)this.getTmCode().get(o))+"\","));
			sb.append("State[" + i + "]" + (String) ((String) tm.get(o)).replace("<code><code/>",(String)this.getTmCode().get(o)));
			i++;
		}
		
		
		//for (Object o : this.getTmCode().keySet()) {
			//System.out.println((String)o+"---"+this.getTmCode().get(o));
			//System.out.print(o+" ");
			//this.getTmCode().put((String)o, this.generate(ii));
			
			//ii++;
		//}
		
		sb.append("var STotal=" + stationCount + ";" + "\n");
		sb.append("function ShowState(obj,RID,start,end,type,val,val2){"+ "\n");
		sb.append("if(val == undefined){" + "\n");
		sb.append("if(start != undefined) {start.innerHTML = \"\";}" + "\n");
		sb.append("if (end != undefined){" + "\n");
		sb.append("end.innerHTML=\"\";}" + "\n");
		sb.append("if(RID!='0'){" + "\n");
		sb.append("var length=0;" + "\n");
		sb.append("for(i=0;i<STotal;i++){" + "\n");
		sb.append("if(State[i][2]==RID){" + "\n");
		sb.append("start.options[length] = new Option(State[i][1], State[i][1]);" + "\n");
		sb.append("if(end!=undefined)" + "\n");
		sb.append("end.options[length] = new Option(State[i][1], State[i][1]);" + "\n");
		sb.append("length++;" + "\n");
		sb.append("}" + "\n");
		sb.append("}" + "\n");
		//type标明用来表示线路方向，去除中间段的车站。
		sb.append("if(end!=undefined&&type!=undefined){" + "\n");
		sb.append("var endLength = end.length;" + "\n");
		sb.append("for(var j=1;j<endLength-1;j++){" + "\n");
		sb.append("end.remove(1);	" + "\n");
		sb.append("}" + "\n");
		sb.append("}" + "\n");
		sb.append("}else{" + "\n");
		sb.append("obj.checked=\"\";" + "\n");
		sb.append("}" + "\n");
		sb.append("}else{//编辑状态" + "\n");
		sb.append("if(RID!='0'){" + "\n");
		sb.append("var length=0;" + "\n");
		sb.append("for(i=0;i<STotal;i++){" + "\n");
		sb.append("if(State[i][2]==RID){" + "\n");
		sb.append("if(State[i][1] == val){" + "\n");
		sb.append("start.options[length].selected = true;" + "\n");
		sb.append("}" + "\n");
		sb.append("if(end){" + "\n");
		sb.append("end.options[length] = new Option(State[i][1], State[i][1]);" + "\n");
		sb.append("if(State[i][1] == val2){end.options[length].selected = true;}" + "\n");
		sb.append("}" + "\n");
		sb.append("length++;" + "\n");
		sb.append("}" + "\n");
		sb.append("}" + "\n");
			//type标明用来表示线路方向，去除中间段的车站。
		sb.append("if(end&&type){" + "\n");
		sb.append("var endLength = end.length;" + "\n");
		sb.append("for(var j=1;j<endLength-1;j++){" + "\n");
		sb.append("end.remove(1);	" + "\n");
		sb.append("}" + "\n");
		sb.append("}" + "\n");
		sb.append("}else{" + "\n");
		sb.append("alert('请先选择线路');" + "\n");
		sb.append("obj.checked=\"\";" + "\n");
		sb.append("}" + "\n");
		sb.append("}" + "\n");
		sb.append("}" + "\n");
		/**/
		
		
		
		
		return sb.toString();
	}
	
	public String generateSearchJS(Map map) {
		//String temp = "function ShowRoadNew(){document.write('<option value=\"1\">1号线</option>');";
		//String t2 = "var State=new Array();State[0]=new Array(\"2\",\"莘庄\",\"1\");";
		List stationList = (List) map.get("stationList");
		StringBuffer sb = new StringBuffer();
		sb.append("function ShowRoad(){" + "\n");
		for (int i = 0; i < stationList.size(); i++) {
			String lineNumber = (String) stationList.get(i);
			sb.append("document.write('<option value=\"" + lineNumber + "\">"
					+ lineNumber + "号线</option>');" + "\n");
		}
		sb.append("}" + "\n");
		sb.append("var State=new Array();" + "\n");
		map.remove("stationList");
		Iterator it = map.entrySet().iterator();
		int stationCount = 0;
		TreeMap tm = new TreeMap(new CodeCompare());
		//TreeMap tmCode = new TreeMap(new CodeCompare());
		//TreeMap<String,String> tempMap = new TreeMap<String,String>(new CodeCompare());
		while (it.hasNext()) {
			Map.Entry e = (Map.Entry) it.next();
			String lineNumber = (String) e.getKey();
			Map stationMap = (Map) e.getValue();
			Iterator sIt = stationMap.entrySet().iterator();
			while (sIt.hasNext()) {
				Map.Entry sE = (Map.Entry) sIt.next();
				String code = (String) sE.getKey();
				String name = (String) sE.getValue();
				if (name != null && !"".equals(name)) {
					// String t
					// ="=new Array(\""+this.modifyCode(code)+"\",\""+name+"\",\""+lineNumber+"\");"+"\n";
					// tm.put(this.modifyCode(code), t);
					
					//this.setCodeMap(new HashMap());
					//this.getCodeMap().put(code, stationCount);
					//String t = "=new Array(\"" + code + "\",\"" + name + "\",\"" + lineNumber + "\");" + "\n";
					//String t = "=new Array(\"" + stationCount + "\",\"" + name + "\",\"" + lineNumber + "\");" + "\n";
					String t = "=new Array(\"" + "<code><code/>" + "\",\"" + name + "\",\"" + lineNumber + "\");" + "\n";
					tm.put(code, t);
					//tempMap.put(code, "");
					//System.out.println(code + " "+String.valueOf(stationCount) );
					stationCount++;
				}
			}
		}
		int ii = 0;
		for (Object o : tm.keySet()) {
			//System.out.println(o+" "+this.getTmCode().get(o));
			//System.out.print(o+" ");
			String v = this.generate(ii);
			this.getTmCode().put((String)o, v);
			ii++;
		}
		
		
		int i = 0;
		for (Object o : tm.keySet()) {
			//System.out.println((String)o+"-----------"+(String)this.getTmCode().get(o));
			//System.out.println((((String)tm.get(o)).replace("\""+(String)o+"\",","\""+(String)this.getTmCode().get(o))+"\","));
			sb.append("State[" + i + "]" + (String) ((String) tm.get(o)).replace("<code><code/>",(String)this.getTmCode().get(o)));
			i++;
		}
		
		
		//for (Object o : this.getTmCode().keySet()) {
			//System.out.println((String)o+"---"+this.getTmCode().get(o));
			//System.out.print(o+" ");
			//this.getTmCode().put((String)o, this.generate(ii));
			
			//ii++;
		//}
		
		sb.append("var STotal=" + stationCount + ";" + "\n");
		sb.append("function ShowState(RID,Select){" + "\n");
		sb.append("	var SelectName=Select;" + "\n");
		sb.append("	SelectName.length=1;" + "\n");
		sb.append("	for(i=0;i<STotal;i++){" + "\n");
		sb.append("		if(State[i][2]==RID){" + "\n");
		sb.append("			SelectName.options[SelectName.length] = new Option(State[i][1], State[i][0]);"
						+ "\n");
		sb.append("		}" + "\n");
		sb.append("	}" + "\n");
		// sb.append("	alert(SelectName.options[1].value);"+"\n");
		sb.append("}" + "\n");
		sb.append("function ShowPrice(FSelect,TSelect,Text){" + "\n");
		sb.append("	if(FSelect.value!==\"\"&&TSelect.value!==\"\"){" + "\n");
		sb.append("		for(i=0;i<PTotal;i++){" + "\n");
		sb.append("			if((SPrice[i][0]==FSelect.value&&SPrice[i][1]==TSelect.value)||(SPrice[i][0]==TSelect.value&&SPrice[i][1]==FSelect.value)){"
						+ "\n");
		sb.append("				Text.innerHTML=SPrice[i][2];" + "\n break;\n");
		sb.append("			}" + "\n");
		sb.append("		}" + "\n");
		sb.append("	}else{" + "\n");
		sb.append("		Text.innerHTML=0;" + "\n");
		sb.append("	}" + "\n");
		sb.append("}" + "\n");
		
		return sb.toString();
	}
	public String generatePriceJS(Map n2cMap, Sheet[] sheets) {
		StringBuffer sb = new StringBuffer();
		Map<String, String> map = new HashMap<String, String>();
		
		//TreeMap<String,Integer> tmCount = new TreeMap<String,Integer>(new CodeCompare());
		try {
			sb.append("var SPrice=[" + "");
			int priceCount = 0;
			String str = "";
			// System.out.println("sheets.length="+sheets.length);
			for (int i = 0; i < sheets.length; i++) {
				int rCount = sheets[i].getRows();
				int cCount = sheets[i].getColumns();
				String sheetName = sheets[i].getName();
				String xLineNumber = sheetName.split("-")[0];
				String yLineNumber = sheetName.split("-")[1];
				// System.out.println(xLineNumber+" "+yLineNumber);
				String[] yCode = new String[cCount];
				String[] xCode = new String[rCount];
				Cell[] row0 = sheets[i].getRow(0);
				Cell[] col0 = sheets[i].getColumn(0);
				System.out.println("i=" + i + " !!!!:" + row0.length + " " + col0.length + " " + sheetName + " " + rCount + " " + cCount);
				
				
				
				try {
					// System.out.println(sheetName+" "+xLineNumber+" "+yLineNumber);
					for (int y = 0; y < cCount; y++) {
						// System.out.println("y="+y);
						// System.out.println(row0[y].getContents());
						yCode[y] = (String) (n2cMap.get(yLineNumber
								+ row0[y].getContents()));
					}
					for (int x = 0; x < rCount; x++) {
						// System.out.println("x="+x);
						// System.out.println("i="+i+":"+col0[x].getContents()+":"+xLineNumber);
						// System.out.println(col0[x].getContents());
						xCode[x] = (String) (n2cMap.get(xLineNumber
								+ col0[x].getContents()));
					}
					for (int r = 1; r < rCount; r++) {
						Cell[] cols = sheets[i].getRow(r);
						for (int c = 1; c < cCount; c++) {
							// sb.append("SPrice["+priceCount+"]=new Array(\""+xCode[r]+"\",\""+yCode[c]+"\",\""+cols[c].getContents()+"\");");
							/**/
							if (!(map.containsKey(xCode[r] + "&" + yCode[c]) || map .containsKey(yCode[c] + "&" + xCode[r]))) {
								// if(this.chineseValid(cols[c].getContents()))
								// System.out.println(r+"-"+c+" "+xCode[r]+" "+yCode[c]+":"+cols[c].getContents());
								if (!this.chineseValid(cols[c].getContents())) {
									if (!"".equals(cols[c].getContents())) { 
										map.put(xCode[r] + "&" + yCode[c], "");
										/*
										sb.append("[\"" + xCode[r] + "\",\""
												+ yCode[c] + "\",\""
												+ cols[c].getContents()
												+ "\"],");
										*/
										//统计记录条数
										/*
										if(tmCount.containsKey(xCode[r])){
											tmCount.put(xCode[r], tmCount.get(xCode[r])+1);
										}else{
											tmCount.put(xCode[r], 1);
										}
										*/
										//System.out.println(xCode[r]+"!"+yCode[c]+"!"+this.getTmCode().get(xCode[r])+"!"+this.getTmCode().get(yCode[c]));
										sb.append("[\"" + this.getTmCode().get(xCode[r]) + "\",\""
												+ this.getTmCode().get(yCode[c]) + "\",\""
												+ cols[c].getContents()
												+ "\"],");
										priceCount++;
									}
								}
							}
							/*
							 * sb.append("[\"" + xCode[r] + "\",\"" + yCode[c]+
							 * "\",\"" + cols[c].getContents() + "\"],");
							 * priceCount++;
							 */
						}
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
			// str = sb.substring(0, sb.length()-1);
			// sb.append(str);
			sb.append("[\"\",\"\",\"\"]];");
			sb.append("var PTotal=" + priceCount + ";");
			// System.out.println(sb.length()+" "+str.length());
			
			/*
			for (Object o : tmCount.keySet()) {
				 System.out.println(o+":"+tmCount.get(o));
			}
			*/
			
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		return sb.toString();
	}
	/**
	 * 返回站名称与代码的映射，如：name=1人民广场 code=1-10
	 */
	public Map getStationName2Code(Sheet[] sheets) {
		String startNumber = "1";
		Map n2cMap = new HashMap();
		for (int i = 0; i < sheets.length; i++) {
			String sheetName = sheets[i].getName();
			String xLineNumber = sheetName.split("-")[0];
			String yLineNumber = sheetName.split("-")[1];
			if (xLineNumber.equals(startNumber)) {
				Cell[] allColumns = sheets[i].getRow(0);
				for (int j = 0; j < allColumns.length; j++) {
					String code = yLineNumber + "-" + j;
					// System.out.println("------>"+yLineNumber+" "+allColumns[j].getContents()+" "+code);
					n2cMap.put(yLineNumber + allColumns[j].getContents(), code);
				}
			}
		}
		
		
		return n2cMap;
	}
	/**
	 * 返回站名称与代码的映射，返回map包含各条线路的站点映射信息，如：key="1" 1号线map 如：code=1-10 name=人民广场
	 */
	public Map getStationCode2Name(Sheet[] sheets) {
		String startNumber = "1";
		Map maps = new HashMap();
		List stationList = new ArrayList();
		for (int i = 0; i < sheets.length; i++) {
			String sheetName = sheets[i].getName();
			String xLineNumber = sheetName.split("-")[0];
			String yLineNumber = sheetName.split("-")[1];
			if (xLineNumber.equals(startNumber)) {
				Map c2nMap = new HashMap();
				Cell[] allColumns = sheets[i].getRow(0);
				for (int j = 0; j < allColumns.length; j++) {
					String code = yLineNumber + "-" + j;
					c2nMap.put(code, allColumns[j].getContents());
				}
				stationList.add(yLineNumber);
				maps.put(yLineNumber, c2nMap);
			}
		}
		maps.put("stationList", stationList);
		return maps;
	}
	// 生成英文车站名
	public String generateSearchJSE(Map map) {
		String temp = "function ShowRoad(){document.write('<option value=\"1\">line 1</option>');";
		String t2 = "var State=new Array();State[0]=new Array(\"2\",\"XinZhuang\",\"1\");";
		List stationList = (List) map.get("stationList");
		StringBuffer sb = new StringBuffer();
		sb.append("function ShowRoad(){" + "\n");
		for (int i = 0; i < stationList.size(); i++) {
			String lineNumber = (String) stationList.get(i);
			sb.append("document.write('<option value=\"" + lineNumber
					+ "\">line " + lineNumber + "</option>');" + "\n");
		}
		sb.append("}" + "\n");
		sb.append("var State=new Array();" + "\n");
		map.remove("stationList");
		Iterator it = map.entrySet().iterator();
		int stationCount = 0;
		TreeMap tm = new TreeMap(new CodeCompare());
		while (it.hasNext()) {
			Map.Entry e = (Map.Entry) it.next();
			String lineNumber = (String) e.getKey();
			Map stationMap = (Map) e.getValue();
			Iterator sIt = stationMap.entrySet().iterator();
			while (sIt.hasNext()) {
				Map.Entry sE = (Map.Entry) sIt.next();
				String code = (String) sE.getKey();
				String name = (String) sE.getValue();
				if (name != null && !"".equals(name)) {
					// String t
					// ="=new Array(\""+this.modifyCode(code)+"\",\""+name+"\",\""+lineNumber+"\");"+"\n";
					// tm.put(this.modifyCode(code), t);
					String t = "=new Array(\"" + code + "\",\"" + name
							+ "\",\"" + lineNumber + "\");" + "\n";
					tm.put(code, t);
					stationCount++;
				}
			}
		}
		int i = 0;
		for (Object o : tm.keySet()) {
			sb.append("State[" + i + "]" + (String) tm.get(o));
			i++;
		}
		sb.append("var STotal=" + stationCount + ";" + "\n");
		sb.append("function ShowState(RID,Select){" + "\n");
		sb.append("	var SelectName=Select;" + "\n");
		sb.append("	SelectName.length=1;" + "\n");
		sb.append("	for(i=0;i<STotal;i++){" + "\n");
		sb.append("		if(State[i][2]==RID){" + "\n");
		sb
				.append("			SelectName.options[SelectName.length] = new Option(State[i][1], State[i][0]);"
						+ "\n");
		sb.append("		}" + "\n");
		sb.append("	}" + "\n");
		// sb.append("	alert(SelectName.options[1].value);"+"\n");
		sb.append("}" + "\n");
		sb.append("function ShowPrice(FSelect,TSelect,Text){" + "\n");
		sb.append("	if(FSelect.value!==\"\"&&TSelect.value!==\"\"){" + "\n");
		sb.append("		for(i=0;i<PTotal;i++){" + "\n");
		sb
				.append("			if((SPrice[i][0]==FSelect.value&&SPrice[i][1]==TSelect.value)||(SPrice[i][0]==TSelect.value&&SPrice[i][1]==FSelect.value)){"
						+ "\n");
		sb.append("				Text.innerHTML=SPrice[i][2];" + "\n break;\n");
		sb.append("			}" + "\n");
		sb.append("		}" + "\n");
		sb.append("	}else{" + "\n");
		sb.append("		Text.innerHTML=0;" + "\n");
		sb.append("	}" + "\n");
		sb.append("}" + "\n");
		return sb.toString();
	}
	public String generatePriceJSE(Map n2cMap, Sheet[] sheets) {
		StringBuffer sb = new StringBuffer();
		// sb.append("var SPrice=new Array();"+"\n");
		Map<String, String> map = new HashMap<String, String>();
		sb.append("var SPrice=[" + "");
		try {
			int priceCount = 0;
			// System.out.println("sheets.length="+sheets.length);
			Map t = new HashMap();
			t = this.getEnglishName();
			for (int i = 0; i < sheets.length; i++) {
				int rCount = sheets[i].getRows(); // 票价分线表中每张表的行数
				int cCount = sheets[i].getColumns(); // 票价分线表中每张表的列数
				String sheetName = sheets[i].getName(); // 票价分线表中每张表的表名
				String xLineNumber = sheetName.split("-")[0];
				String yLineNumber = sheetName.split("-")[1];
				// System.out.println(xLineNumber+" "+yLineNumber);
				String[] yCode = new String[cCount];
				String[] xCode = new String[rCount];
				Cell[] row0 = sheets[i].getRow(0); // 票价分线表中每张表的行的站名
				Cell[] col0 = sheets[i].getColumn(0);
				System.out.println("i=" + i + " !!!!:" + row0.length + " "
						+ col0.length + " " + sheetName + " " + rCount + " "
						+ cCount);
				try {
					// System.out.println(sheetName+" "+xLineNumber+" "+yLineNumber);
					for (int y = 0; y < cCount; y++) {
						// System.out.println("y="+y);
						// System.out.println(row0[y].getContents());
						String[] temp = new String[cCount]; // 存放英文站名
						for (Iterator iter = t.entrySet().iterator(); iter
								.hasNext();) {
							Map.Entry entry = (Map.Entry) iter.next();
							String key = (String) entry.getKey();
							if ("".equals(row0[y].getContents())
									|| row0[y].getContents() == null) {
								continue;
							}
							if ((row0[y].getContents()).equals(key)) {
								temp[y] = (String) t.get(key);
							}
							// String value = (String)t.get(key);
						}
						yCode[y] = (String) (n2cMap.get(yLineNumber + temp[y]));
						// System.out.println("---->yCode====" + yCode[y]);
					}
					for (int x = 0; x < rCount; x++) {
						// System.out.println("x="+x);
						// System.out.println("i="+i+":"+col0[x].getContents()+":"+xLineNumber);
						// System.out.println(col0[x].getContents());
						String[] temp = new String[rCount]; // 存放英文站名
						for (Iterator iter = t.entrySet().iterator(); iter
								.hasNext();) {
							Map.Entry entry = (Map.Entry) iter.next();
							String key = (String) entry.getKey();
							if ("".equals(col0[x].getContents())
									|| col0[x].getContents() == null) {
								continue;
							}
							if ((col0[x].getContents()).equals(key)) {
								temp[x] = (String) t.get(key);
							}
							// String value = (String)t.get(key);
						}
						xCode[x] = (String) (n2cMap.get(xLineNumber + temp[x]));
						// System.out.println("---->xCode====" + xCode[x]);
					}
					for (int r = 1; r < rCount; r++) {
						Cell[] cols = sheets[i].getRow(r);
						for (int c = 1; c < cCount; c++) {
							if (!(map.containsKey(xCode[r] + "&" + yCode[c]) || map
									.containsKey(yCode[c] + "&" + xCode[r]))) {
								if (!this.chineseValid(cols[c].getContents())) {
									if (!"".equals(cols[c].getContents())) {
										map.put(xCode[r] + "&" + yCode[c], "");
										sb.append("[\"" + xCode[r] + "\",\""
												+ yCode[c] + "\",\""
												+ cols[c].getContents()
												+ "\"],");
										priceCount++;
									}
								}
							}
						}
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
			sb.append("[\"\",\"\",\"\"]];");
			sb.append("var PTotal=" + priceCount + ";");
		} catch (Exception e) {
			e.printStackTrace();
		}
		return sb.toString();
	}
	/**
	 * 返回站“英文”名称与代码的映射，如：name=1 people' square code=1-10
	 */
	public Map getStationEName2Code(Sheet[] sheets) {
		String startNumber = "1";
		Map n2cMap = new HashMap();
		Map t = new HashMap();// 存放英文与中文名，key: 中文名，value: 英文名
		t = this.getEnglishName();
		for (int i = 0; i < sheets.length; i++) {
			String sheetName = sheets[i].getName();
			String xLineNumber = sheetName.split("-")[0];
			String yLineNumber = sheetName.split("-")[1];
			if (xLineNumber.equals(startNumber)) {
				Cell[] allColumns = sheets[i].getRow(0);
				for (int j = 0; j < allColumns.length; j++) {
					String code = yLineNumber + "-" + j;
					String[] temp = new String[allColumns.length]; // 存放英文站名
					for (Iterator iter = t.entrySet().iterator(); iter
							.hasNext();) {
						Map.Entry entry = (Map.Entry) iter.next();
						String key = (String) entry.getKey();
						if ("".equals(allColumns[j].getContents())|| allColumns[j].getContents() == null) {
							continue;
						}
						if ((allColumns[j].getContents()).equals(key)) {
							temp[j] = (String) t.get(key);
						}
						// String value = (String)t.get(key);
					}
					// System.out.println("------>" + yLineNumber + " " +
					// temp[j]+ " " + code);
					n2cMap.put(yLineNumber + temp[j], code);
				}
			}
		}
		return n2cMap;
	}
	/**
	 * 返回站“英文”名称与代码的映射，返回map包含各条线路的站点映射信息，如：key="1" 1号线map 如：code=1-10 name=人民广场
	 */
	public Map getStationCode2EName(Sheet[] sheets) {
		String startNumber = "1";
		Map maps = new HashMap();
		List stationList = new ArrayList();
		Map t = new HashMap();// 存放英文与中文名，key: 中文名，value: 英文名
		t = this.getEnglishName();
		for (int i = 0; i < sheets.length; i++) {
			String sheetName = sheets[i].getName();
			String xLineNumber = sheetName.split("-")[0];
			String yLineNumber = sheetName.split("-")[1];
			if (xLineNumber.equals(startNumber)) {
				Map c2nMap = new HashMap();
				Cell[] allColumns = sheets[i].getRow(0);
				for (int j = 0; j < allColumns.length; j++) {
					String code = yLineNumber + "-" + j;
					String[] temp = new String[allColumns.length]; // 存放英文站名
					for (Iterator iter = t.entrySet().iterator(); iter
							.hasNext();) {
						Map.Entry entry = (Map.Entry) iter.next();
						String key = (String) entry.getKey();
						if ("".equals(allColumns[j].getContents())
								|| allColumns[j].getContents() == null) {
							continue;
						}
						if ((allColumns[j].getContents()).equals(key)) {
							temp[j] = (String) t.get(key);
						}
						String value = (String) t.get(key);
					}
					// System.out.println("------>" + yLineNumber + " " +
					// temp[j]+ " " + code);
					c2nMap.put(code, temp[j]);
				}
				stationList.add(yLineNumber);
				maps.put(yLineNumber, c2nMap);
			}
		}
		maps.put("stationList", stationList);
		return maps;
	}
	public Sheet[] getSheets(Workbook workbook) {
		Sheet[] sheet = null;
		if (workbook != null) {
			sheet = workbook.getSheets();
		}
		return sheet;
	}
	public Workbook getWorkbook(String xslPath) throws BiffException,
			IOException {
		File xls = new File(xslPath);
		return Workbook.getWorkbook(xls);
	}
	public void generateFile(String filePath, String text) {
		File file = new File(filePath);
		try {
			BufferedWriter bw = new BufferedWriter(new FileWriter(file));
			bw.write(text);
			bw.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	public Map getEnglishName() {
		String fpath = "D:\\xls\\translate.xls";
		Map t = new HashMap();
		try {
			Sheet[] tran = this.getSheets(this.getWorkbook(fpath));
			for (int i = 0; i < tran.length; i++) {
				Cell[] col1 = tran[i].getColumn(0);
				Cell[] col2 = tran[i].getColumn(1);
				for (int j = 0; j < col1.length; j++) {
					String[] str = new String[col1.length];// 中文站名
					String[] str2 = new String[col1.length];// 英文站名
					str[j] = col1[j].getContents();
					str2[j] = col2[j].getContents();
					t.put(str[j], str2[j]);
					if ("".equals(str[j]) || "".equals(str2[j])) {
						break;
					}
				}
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (BiffException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return t;
	}
	public static boolean chineseValid(String s) {
		int length = s.length();
		byte[] b;
		for (int i = 0; i < length; i++) {
			b = s.substring(i).getBytes();
			if ((b[0] & 0xff) > 128)
				return true;
		}
		return false;
	}
	
	private String generate(int i){
		i++;
		int s1 = i/26;
		int s2 = i%26;
		char ch1 = 0;
		char ch2 = 0;
		
		if(s1!=0){
			ch1 = (char)((s1-1) + 'a');
			//System.out.println("ch1"+String.valueOf(ch1));
		}
		if(s2!=0){
			ch2 = (char)(s2-1 + 'a');
			//System.out.println("ch2"+String.valueOf(ch2));
		}
		/**/
		if(s1!=0&&s2==0){
			if(s1==1){
				ch1 = 0;
			}else{
				ch1 = (char) ((s1-1)+'a'-1);
			}
			ch2 = 'z';
		}
		
		String ret = ""+String.valueOf(ch1)+""+String.valueOf(ch2);
		//System.out.println(s1+" "+s2+" ch1:"+ch1+" ch2:"+ch2);
		//System.out.println(ret.trim());
		return ret.trim();
	}
	
	public TreeMap<String, String> getTmCode() {
		return tmCode;
	}
	public void setTmCode(TreeMap<String, String> tmCode) {
		this.tmCode = tmCode;
	}
	
}
```
生成js：
search.js
```java
function ShowRoad(){
document.write('<option value="1">1号线</option>');
document.write('<option value="2">2号线</option>');
document.write('<option value="3">3号线</option>');
document.write('<option value="4">4号线</option>');
document.write('<option value="5">5号线</option>');
document.write('<option value="6">6号线</option>');
document.write('<option value="7">7号线</option>');
document.write('<option value="8">8号线</option>');
document.write('<option value="9">9号线</option>');
document.write('<option value="10">10号线</option>');
document.write('<option value="11">11号线</option>');
}
var State=new Array();
State[0]=new Array("a","莘庄","1");
State[1]=new Array("b","外环路","1");
State[2]=new Array("c","莲花路","1");
State[3]=new Array("d","锦江乐园","1");
State[4]=new Array("e","上海南站","1");
State[5]=new Array("f","漕宝路","1");
State[6]=new Array("g","上海体育馆","1");
State[7]=new Array("h","徐家汇","1");
State[8]=new Array("i","衡山路","1");
State[9]=new Array("j","常熟路","1");
State[10]=new Array("k","陕西南路","1");
State[11]=new Array("l","黄陂南路","1");
State[12]=new Array("m","人民广场","1");
State[13]=new Array("n","新闸路","1");
State[14]=new Array("o","汉中路","1");
State[15]=new Array("p","上海火车站","1");
State[16]=new Array("q","中山北路","1");
State[17]=new Array("r","延长路","1");
State[18]=new Array("s","上海马戏城","1");
State[19]=new Array("t","汶水路","1");
State[20]=new Array("u","彭浦新村","1");
State[21]=new Array("v","共康路","1");
State[22]=new Array("w","通河新村","1");
State[23]=new Array("x","呼兰路","1");
State[24]=new Array("y","共富新村","1");
State[25]=new Array("z","宝安公路","1");
State[26]=new Array("aa","友谊西路","1");
State[27]=new Array("ab","富锦路","1");
State[28]=new Array("ac","徐泾东","2");
State[29]=new Array("ad","虹桥火车站","2");
State[30]=new Array("ae","虹桥2号航站楼","2");
State[31]=new Array("af","淞虹路","2");
State[32]=new Array("ag","北新泾","2");
State[33]=new Array("ah","威宁路","2");
State[34]=new Array("ai","娄山关路","2");
State[35]=new Array("aj","中山公园","2");
State[36]=new Array("ak","江苏路","2");
State[37]=new Array("al","静安寺","2");
State[38]=new Array("am","南京西路","2");
State[39]=new Array("an","人民广场","2");
State[40]=new Array("ao","南京东路","2");
State[41]=new Array("ap","陆家嘴","2");
State[42]=new Array("aq","东昌路","2");
State[43]=new Array("ar","世纪大道","2");
State[44]=new Array("as","上海科技馆","2");
State[45]=new Array("at","世纪公园","2");
State[46]=new Array("au","龙阳路","2");
State[47]=new Array("av","张江高科","2");
State[48]=new Array("aw","金科路","2");
State[49]=new Array("ax","广兰路","2");
State[50]=new Array("ay","唐镇","2");
State[51]=new Array("az","创新中路","2");
State[52]=new Array("ba","华夏东路","2");
State[53]=new Array("bb","川沙","2");
State[54]=new Array("bc","凌空路","2");
State[55]=new Array("bd","远东大道","2");
State[56]=new Array("be","海天三路","2");
State[57]=new Array("bf","浦东国际机场","2");
State[58]=new Array("bg","上海南站","3");
State[59]=new Array("bh","石龙路","3");
State[60]=new Array("bi","龙漕路","3");
State[61]=new Array("bj","漕溪路","3");
State[62]=new Array("bk","宜山路","3");
State[63]=new Array("bl","虹桥路","3");
State[64]=new Array("bm","延安西路","3");
State[65]=new Array("bn","中山公园","3");
State[66]=new Array("bo","金沙江路","3");
State[67]=new Array("bp","曹杨路","3");
State[68]=new Array("bq","镇坪路","3");
State[69]=new Array("br","中潭路","3");
State[70]=new Array("bs","上海火车站","3");
State[71]=new Array("bt","宝山路","3");
State[72]=new Array("bu","东宝兴路","3");
State[73]=new Array("bv","虹口足球场","3");
State[74]=new Array("bw","赤峰路","3");
State[75]=new Array("bx","大柏树","3");
State[76]=new Array("by","江湾镇","3");
State[77]=new Array("bz","殷高西路","3");
State[78]=new Array("ca","长江南路","3");
State[79]=new Array("cb","淞发路","3");
State[80]=new Array("cc","张华浜","3");
State[81]=new Array("cd","淞滨路","3");
State[82]=new Array("ce","水产路","3");
State[83]=new Array("cf","宝杨路","3");
State[84]=new Array("cg","友谊路","3");
State[85]=new Array("ch","铁力路","3");
State[86]=new Array("ci","江杨北路","3");
State[87]=new Array("cj","上海体育馆","4");
State[88]=new Array("ck","宜山路","4");
State[89]=new Array("cl","虹桥路","4");
State[90]=new Array("cm","延安西路","4");
State[91]=new Array("cn","中山公园","4");
State[92]=new Array("co","金沙江路","4");
State[93]=new Array("cp","曹杨路","4");
State[94]=new Array("cq","镇坪路","4");
State[95]=new Array("cr","中潭路","4");
State[96]=new Array("cs","上海火车站","4");
State[97]=new Array("ct","宝山路","4");
State[98]=new Array("cu","海伦路","4");
State[99]=new Array("cv","临平路","4");
State[100]=new Array("cw","大连路","4");
State[101]=new Array("cx","杨树浦路","4");
State[102]=new Array("cy","浦东大道","4");
State[103]=new Array("cz","世纪大道","4");
State[104]=new Array("da","浦电路","4");
State[105]=new Array("db","蓝村路","4");
State[106]=new Array("dc","塘桥","4");
State[107]=new Array("dd","南浦大桥","4");
State[108]=new Array("de","西藏南路","4");
State[109]=new Array("df","鲁班路","4");
State[110]=new Array("dg","大木桥路","4");
State[111]=new Array("dh","东安路","4");
State[112]=new Array("di","上海体育场","4");
State[113]=new Array("dj","莘庄","5");
State[114]=new Array("dk","春申路","5");
State[115]=new Array("dl","银都路","5");
State[116]=new Array("dm","颛桥","5");
State[117]=new Array("dn","北桥","5");
State[118]=new Array("do","剑川路","5");
State[119]=new Array("dp","东川路","5");
State[120]=new Array("dq","金平路 ","5");
State[121]=new Array("dr","华宁路","5");
State[122]=new Array("ds","文井路","5");
State[123]=new Array("dt","闵行开发区","5");
State[124]=new Array("du","东方体育中心","6");
State[125]=new Array("dv","灵岩南路","6");
State[126]=new Array("dw","上南路","6");
State[127]=new Array("dx","华夏西路","6");
State[128]=new Array("dy","高青路","6");
State[129]=new Array("dz","东明路","6");
State[130]=new Array("ea","高科西路","6");
State[131]=new Array("eb","临沂新村","6");
State[132]=new Array("ec","上海儿童医学中心","6");
State[133]=new Array("ed","蓝村路","6");
State[134]=new Array("ee","浦电路","6");
State[135]=new Array("ef","世纪大道","6");
State[136]=new Array("eg","源深体育中心","6");
State[137]=new Array("eh","民生路","6");
State[138]=new Array("ei","北洋泾路","6");
State[139]=new Array("ej","德平路","6");
State[140]=new Array("ek","云山路","6");
State[141]=new Array("el","金桥路","6");
State[142]=new Array("em","博兴路","6");
State[143]=new Array("en","五莲路","6");
State[144]=new Array("eo","巨峰路","6");
State[145]=new Array("ep","东靖路","6");
State[146]=new Array("eq","五洲大道","6");
State[147]=new Array("er","洲海路","6");
State[148]=new Array("es","外高桥保税区南站","6");
State[149]=new Array("et","航津路","6");
State[150]=new Array("eu","外高桥保税区北站","6");
State[151]=new Array("ev","港城路","6");
State[152]=new Array("ew","美兰湖","7");
State[153]=new Array("ex","罗南新村","7");
State[154]=new Array("ey","潘广路","7");
State[155]=new Array("ez","刘行","7");
State[156]=new Array("fa","顾村公园","7");
State[157]=new Array("fb","祁华路","7");
State[158]=new Array("fc","上海大学","7");
State[159]=new Array("fd","南陈路","7");
State[160]=new Array("fe","上大路","7");
State[161]=new Array("ff","场中路","7");
State[162]=new Array("fg","大场镇","7");
State[163]=new Array("fh","行知路","7");
State[164]=new Array("fi","大华三路","7");
State[165]=new Array("fj","新村路","7");
State[166]=new Array("fk","岚皋路","7");
State[167]=new Array("fl","镇坪路","7");
State[168]=new Array("fm","长寿路","7");
State[169]=new Array("fn","昌平路","7");
State[170]=new Array("fo","静安寺","7");
State[171]=new Array("fp","常熟路","7");
State[172]=new Array("fq","肇嘉浜路","7");
State[173]=new Array("fr","东安路","7");
State[174]=new Array("fs","船厂路","7");
State[175]=new Array("ft","后滩","7");
State[176]=new Array("fu","长清路","7");
State[177]=new Array("fv","耀华路","7");
State[178]=new Array("fw","云台路","7");
State[179]=new Array("fx","高科西路","7");
State[180]=new Array("fy","杨高南路","7");
State[181]=new Array("fz","锦绣路","7");
State[182]=new Array("ga","芳华路","7");
State[183]=new Array("gb","龙阳路","7");
State[184]=new Array("gc","花木路","7");
State[185]=new Array("gd","航天博物馆","8");
State[186]=new Array("ge","联航路","8");
State[187]=new Array("gf","江月路","8");
State[188]=new Array("gg","浦江镇","8");
State[189]=new Array("gh","芦恒璐","8");
State[190]=new Array("gi","凌兆新村","8");
State[191]=new Array("gj","东方体育中心","8");
State[192]=new Array("gk","杨思","8");
State[193]=new Array("gl","成山路","8");
State[194]=new Array("gm","耀华路","8");
State[195]=new Array("gn","中华艺术宫","8");
State[196]=new Array("go","西藏南路","8");
State[197]=new Array("gp","陆家浜路","8");
State[198]=new Array("gq","老西门","8");
State[199]=new Array("gr","大世界","8");
State[200]=new Array("gs","人民广场","8");
State[201]=new Array("gt","曲阜路","8");
State[202]=new Array("gu","中兴路","8");
State[203]=new Array("gv","西藏北路","8");
State[204]=new Array("gw","虹口足球场","8");
State[205]=new Array("gx","曲阳路","8");
State[206]=new Array("gy","四平路","8");
State[207]=new Array("gz","鞍山新村","8");
State[208]=new Array("ha","江浦路","8");
State[209]=new Array("hb","黄兴路","8");
State[210]=new Array("hc","延吉中路","8");
State[211]=new Array("hd","黄兴公园","8");
State[212]=new Array("he","翔殷路","8");
State[213]=new Array("hf","嫩江路","8");
State[214]=new Array("hg","市光路","8");
State[215]=new Array("hh","松江新城","9");
State[216]=new Array("hi","松江大学城","9");
State[217]=new Array("hj","洞泾","9");
State[218]=new Array("hk","佘山","9");
State[219]=new Array("hl","泗泾","9");
State[220]=new Array("hm","九亭","9");
State[221]=new Array("hn","中春路","9");
State[222]=new Array("ho","七宝","9");
State[223]=new Array("hp","星中路","9");
State[224]=new Array("hq","合川路","9");
State[225]=new Array("hr","漕河泾开发区","9");
State[226]=new Array("hs","桂林路","9");
State[227]=new Array("ht","宜山路","9");
State[228]=new Array("hu","徐家汇","9");
State[229]=new Array("hv","肇嘉浜路","9");
State[230]=new Array("hw","嘉善路","9");
State[231]=new Array("hx","打浦桥","9");
State[232]=new Array("hy","马当路","9");
State[233]=new Array("hz","陆家浜路","9");
State[234]=new Array("ia","小南门","9");
State[235]=new Array("ib","商城路","9");
State[236]=new Array("ic","世纪大道","9");
State[237]=new Array("id","杨高中路","9");
State[238]=new Array("ie","航中路","10");
State[239]=new Array("if","紫藤路","10");
State[240]=new Array("ig","龙柏新村","10");
State[241]=new Array("ih","虹桥火车站","10");
State[242]=new Array("ii","虹桥2号航站楼","10");
State[243]=new Array("ij","虹桥1号航站楼","10");
State[244]=new Array("ik","上海动物园","10");
State[245]=new Array("il","龙溪路","10");
State[246]=new Array("im","水城路","10");
State[247]=new Array("in","伊犁路","10");
State[248]=new Array("io","宋园路","10");
State[249]=new Array("ip","虹桥路","10");
State[250]=new Array("iq","交通大学","10");
State[251]=new Array("ir","上海图书馆","10");
State[252]=new Array("is","陕西南路","10");
State[253]=new Array("it","新天地","10");
State[254]=new Array("iu","老西门","10");
State[255]=new Array("iv","豫园","10");
State[256]=new Array("iw","南京东路","10");
State[257]=new Array("ix","天潼路","10");
State[258]=new Array("iy","四川北路","10");
State[259]=new Array("iz","海伦路","10");
State[260]=new Array("ja","邮电新村","10");
State[261]=new Array("jb","四平路","10");
State[262]=new Array("jc","同济大学","10");
State[263]=new Array("jd","国权路","10");
State[264]=new Array("je","五角场","10");
State[265]=new Array("jf","江湾体育场","10");
State[266]=new Array("jg","三门路","10");
State[267]=new Array("jh","殷高东路","10");
State[268]=new Array("ji","新江湾城","10");
State[269]=new Array("jj","安亭","11");
State[270]=new Array("jk","上海汽车城","11");
State[271]=new Array("jl","昌吉东路","11");
State[272]=new Array("jm","上海赛车场","11");
State[273]=new Array("jn","嘉定北","11");
State[274]=new Array("jo","嘉定西","11");
State[275]=new Array("jp","白银路","11");
State[276]=new Array("jq","嘉定新城","11");
State[277]=new Array("jr","马陆","11");
State[278]=new Array("js","南翔","11");
State[279]=new Array("jt","桃浦新村","11");
State[280]=new Array("ju","武威路","11");
State[281]=new Array("jv","祁连山路 ","11");
State[282]=new Array("jw","李子园","11");
State[283]=new Array("jx","上海西站","11");
State[284]=new Array("jy","真如","11");
State[285]=new Array("jz","枫桥路","11");
State[286]=new Array("ka","曹杨路","11");
State[287]=new Array("kb","隆德路","11");
State[288]=new Array("kc","江苏路","11");
var STotal=289;
function ShowState(RID,Select){
	var SelectName=Select;
	SelectName.length=1;
	for(i=0;i<STotal;i++){
		if(State[i][2]==RID){
			SelectName.options[SelectName.length] = new Option(State[i][1], State[i][0]);
		}
	}
}
function ShowPrice(FSelect,TSelect,Text){
	if(FSelect.value!==""&&TSelect.value!==""){
		for(i=0;i<PTotal;i++){
			if((SPrice[i][0]==FSelect.value&&SPrice[i][1]==TSelect.value)||(SPrice[i][0]==TSelect.value&&SPrice[i][1]==FSelect.value)){
				Text.innerHTML=SPrice[i][2];
 break;
			}
		}
	}else{
		Text.innerHTML=0;
	}
}
```
Price.js:
```java
var SPrice=[["a","a","2"],["a","b","3"],["a","c","3"],["a","d","3"],["a","e","3"],["a","f","4"],["a","g","4"],["a","h","4"],["a","i","4"],["a","j","4"],["a","k","4"],["a","l","4"],["a","m","4"],["a","n","5"],["a","o","5"],["a","p","5"],["a","q","5"],["a","r","5"],["a","s","5"],["a","t","5"],["a","u","6"],["a","v","6"],["a","w","6"],["a","x","6"],["a","y","6"],["a","z","6"],["a","aa","6"],["a","ab","7"],["b","b","3"],["b","c","3"],["b","d","3"],["b","e","3"],["b","f","3"],["b","g","4"],["b","h","4"],["b","i","4"],["b","j","4"],["b","k","4"],["b","l","4"],["b","m","4"],["b","n","4"],["b","o","5"],["b","p","5"],["b","q","5"],["b","r","5"],["b","s","5"],["b","t","5"],["b","u","5"],["b","v","6"],["b","w","6"],["b","x","6"],["b","y","6"],["b","z","6"],["b","aa","6"],["b","ab","6"],["c","c","3"],["c","d","3"],["c","e","3"],["c","f","3"],["c","g","4"],["c","h","4"],["c","i","4"],["c","j","4"],["c","k","4"],["c","l","4"],["c","m","4"],["c","n","4"],["c","o","4"],["c","p","5"],["c","q","5"],["c","r","5"],["c","s","5"],["c","t","5"],["c","u","5"],["c","v","5"],["c","w","6"],["c","x","6"],["c","y","6"],["c","z","6"],["c","aa","6"],["c","ab","6"],["d","d","3"],["d","e","3"],["d","f","3"],["d","g","3"],["d","h","4"],["d","i","4"],["d","j","4"],["d","k","4"],["d","l","4"],["d","m","4"],["d","n","4"],["d","o","4"],["d","p","4"],["d","q","5"],["d","r","5"],["d","s","5"],["d","t","5"],["d","u","5"],["d","v","5"],["d","w","5"],["d","x","6"],["d","y","6"],["d","z","6"],["d","aa","6"],["d","ab","6"],["e","e","3"],["e","f","3"],["e","g","3"],["e","h","3"],["e","i","3"],["e","j","4"],["e","k","4"],["e","l","4"],["e","m","4"],["e","n","4"],["e","o","4"],["e","p","4"],["e","q","4"],["e","r","5"],["e","s","5"],["e","t","5"],["e","u","5"],["e","v","5"],["e","w","5"],["e","x","5"],["e","y","6"],["e","z","6"],["e","aa","6"],["e","ab","6"],["f","f","3"],["f","g","3"],["f","h","3"],["f","i","3"],["f","j","3"],["f","k","4"],["f","l","4"],["f","m","4"],["f","n","4"],["f","o","4"],["f","p","4"],["f","q","4"],["f","r","4"],["f","s","4"],["f","t","5"],["f","u","5"],["f","v","5"],["f","w","5"],["f","x","5"],["f","y","5"],["f","z","6"],["f","aa","6"],["f","ab","6"],["g","g","3"],["g","h","3"],["g","i","3"],["g","j","3"],["g","k","3"],["g","l","4"],["g","m","4"],["g","n","4"],["g","o","4"],["g","p","4"],["g","q","4"],["g","r","4"],["g","s","4"],["g","t","4"],["g","u","5"],["g","v","5"],["g","w","5"],["g","x","5"],["g","y","5"],["g","z","5"],["g","aa","5"],["g","ab","6"],["h","h","3"],["h","i","3"],["h","j","3"],["h","k","3"],["h","l","3"],["h","m","4"],["h","n","4"],["h","o","4"],["h","p","4"],["h","q","4"],["h","r","4"],["h","s","4"],["h","t","4"],["h","u","4"],["h","v","5"],["h","w","5"],["h","x","5"],["h","y","5"],["h","z","5"],["h","aa","5"],["h","ab","5"],["i","i","3"],["i","j","3"],["i","k","3"],["i","l","3"],["i","m","3"],["i","n","3"],["i","o","4"],["i","p","4"],["i","q","4"],["i","r","4"],["i","s","4"],["i","t","4"],["i","u","4"],["i","v","4"],["i","w","5"],["i","x","5"],["i","y","5"],["i","z","5"],["i","aa","5"],["i","ab","5"],["j","j","3"],["j","k","3"],["j","l","3"],["j","m","3"],["j","n","3"],["j","o","3"],["j","p","4"],["j","q","4"],["j","r","4"],["j","s","4"],["j","t","4"],["j","u","4"],["j","v","4"],["j","w","4"],["j","x","5"],["j","y","5"],["j","z","5"],["j","aa","5"],["j","ab","5"],["k","k","3"],["k","l","3"],["k","m","3"],["k","n","3"],["k","o","3"],["k","p","3"],["k","q","4"],["k","r","4"],["k","s","4"],["k","t","4"],["k","u","4"],["k","v","4"],["k","w","4"],["k","x","5"],["k","y","5"],["k","z","5"],["k","aa","5"],["k","ab","5"],["l","l","3"],["l","m","3"],["l","n","3"],["l","o","3"],["l","p","3"],["l","q","3"],["l","r","4"],["l","s","4"],["l","t","4"],["l","u","4"],["l","v","4"],["l","w","4"],["l","x","4"],["l","y","5"],["l","z","5"],["l","aa","5"],["l","ab","5"],["m","m","3"],["m","n","3"],["m","o","3"],["m","p","3"],["m","q","3"],["m","r","3"],["m","s","4"],["m","t","4"],["m","u","4"],["m","v","4"],["m","w","4"],["m","x","4"],["m","y","4"],["m","z","5"],["m","aa","5"],["m","ab","5"],["n","n","3"],["n","o","3"],["n","p","3"],["n","q","3"],["","",""]];var PTotal=41905;
```
调用方式：
```
<tr>
                                          <td width="23%" align="right">起始站:</td>
                                          <td width="77%" nowrap="nowrap"><select onChange="ShowState(this.options(this.selectedIndex).value,document.SearchPrice.FState)" name="FRoad">
                                              <option value="0" selected="selected">线路</option>
                                              <script language="javascript">ShowRoad()</script>
                                            </select>
                                            <select onChange="ShowPrice(document.SearchPrice.FState,document.SearchPrice.TState,Price)" name="FState">
                                              <option value="" selected="selected">请选择</option>
                                            </select></td>
                                        </tr>
                                        <tr>
                                          <td align="right">终点站:</td>
                                          <td nowrap="nowrap"><select onChange="ShowState(this.options(this.selectedIndex).value,document.SearchPrice.TState)" name="TRoad">
                                              <option value="" selected="selected">线路</option>
                                              <script language="javascript">ShowRoad()</script>
                                            </select>
                                            <select onChange="ShowPrice(document.SearchPrice.FState,document.SearchPrice.TState,Price)" name="TState">
                                              <option value="" selected="selected">请选择</option>
                                            </select></td>
                                        </tr>
                                        <tr>
                                          <td> </td>
                                          <td>    票价：<span id="Price">0</span> 元</td>
                                        </tr>
```
