# kendoui实现单选多选汉字/拼音/简拼筛选过滤 - 零度的博客专栏 - CSDN博客
2016年08月12日 17:04:00[零度anngle](https://me.csdn.net/zmx729618)阅读数：2203
### 一、自定义多字段筛选框架（封装原有的kendoui组件）
单项选择：（汉字+拼音）
```
kendo.ui.plugin(kendo.ui.ComboBox.extend({
           options: {
               name: "MultiFilterComboBox"
           },
           _filterSource: function () {
               this.dataSource.filter({
                   logic: "or",
                   filters: [                                                     
                       { field: "name",operator: "contains", value:this.text()},
                       { field: "pinyin",operator: "contains", value:this.text()},
                       { field: "jianpin",operator: "contains", value: this.text()}                    
                   ]
               });
           } 
}));
```
多项选择：（汉字+拼音）
```
kendo.ui.plugin(kendo.ui.MultiSelect.extend({
           options: {
               name: "MultiFilterMultiSelect"
           },
          
           _filterSource: function () {
               this.dataSource.filter({
                   logic: "or",          
                   filters: [
                       { field: "name",operator: "contains", value:this._prev},                                   
                       { field: "pinyin",operator: "contains", value: this._prev},
                       { field: "jianpin",operator: "contains", value: this._prev}                    
                   ]
               });
           }     
    }));
```
### 二、前台使用
```
//定义数据源
	   var getDataSource = new kendo.data.DataSource({
              //type: "odata",
              serverFiltering: false,
              transport: {
                  read: function(options) {
	                   $.ajax({
	                       url:"${ctx}/all/basic/traitgroup/candidate?r="+Math.random(),                            
	                       data:{"groupId":"${groupId}","cropId":"${cropId}","categorySelected":$("#category").val(),"criterionSelected":$("#criterion").val()},
                           async:false,
	                       dataType:"json", 
	                       success: function(result) {
	                          result.splice(0,0,{id:"0",name:"全选",pinyin:"quanxuan"}); 
	                          result.splice(1,0,{id:"1",name:"全不选",pinyin:"quanbuxuan"}); 
	                          options.success(result);
	                        },                                                                
	                       type:"POST"
	                  }); 
	               } 
              }
          });
    	
    	  //性状选择框
    	  $("#details").kendoMultiFilterMultiSelect({
            placeholder: "请选择性状...",
            animation: {close: {effects: "fadeOut",}, open: {effects: "fadeIn",}},
            dataTextField: "name",
            dataValueField: "id",         
            filter: "contains",
            dataBound:function(e) {
                console.log(e.sender._prev);
                console.log($("#details").data("kendoMultiFilterMultiSelect").options);            
            },
                                   
            ignoreCase: false,
            autoBind: false,
            height: 400,
            autoClose: false,
	        select: function(e) {
	        	var dataItem = this.dataSource.view()[e.item.index()];
                if (dataItem.name === "全选") {                                
                     var values = $.map(this.dataSource.view(), function(dataItem) {
                       return dataItem.id;
                     });
                     values.splice(0,2);                                
                     this.value(values);
                      e.preventDefault();   //去掉默认项——全选                         
                                                     
                }else if(dataItem.name === "全不选"){                               
                     var values = $.map(this.dataSource.view(), function(dataItem) {
                       return dataItem.id;
                     });                                
                     this.value([]);
                     e.preventDefault();   //去掉默认项——全不选
                }                      
	        },                        
	        dataSource: getDataSource    //绑定数据源
        });
```
### 三、后台controller增加拼音字段和简拼字段，不需要存数据库，直接传到前台。
```
@RequestMapping("/all/basic/traitgroup/candidate")
	@ResponseBody 
	public String traitGroupDetailCandidate(String groupId,String cropId,String categorySelected,String criterionSelected){	
		try {
			logger.info("获取性状组可添加性状列表");
			String teamGroupId=LogInfoService.getLoginUserDetail().getTeamGroupId();
                        List<Trait> traits=traitGroupDao.getDetailCandidate(groupId,cropId,teamGroupId,categorySelected,criterionSelected);
                        JSONArray ja=new JSONArray();
                        for(Trait t:traits){
                              JSONObject jo=new JSONObject();
                              jo.put("id", t.getId());
                              jo.put("name", t.getName());
                              jo.put("pinyin",PinYinUtil.cn2Spell(t.getName()));
                              jo.put("jianpin",PinYinUtil.cn2FirstSpell(t.getName()));
                              ja.add(jo);
                         }
  
                         return JSON.toJSONString(ja);
		} catch (Exception e) {	
			e.printStackTrace();
			logger.info("获取性状组可添加性状列表失败");
			return null;
		}
	}
```
             上述中用到了pinyin2J工具包，maven引用如下：
```
<!-- pinyin4j -->
	<dependency>
		<groupId>com.belerweb</groupId>
		<artifactId>pinyin4j</artifactId>
		<version>2.5.0</version>
   </dependency>
```
### 四、在此基础上封装了两个工具类
#### （1）汉字转换拼音工具类：
```
package org.nercita.core.utils;
import net.sourceforge.pinyin4j.PinyinHelper;
import net.sourceforge.pinyin4j.format.HanyuPinyinCaseType;
import net.sourceforge.pinyin4j.format.HanyuPinyinOutputFormat;
import net.sourceforge.pinyin4j.format.HanyuPinyinToneType;
import net.sourceforge.pinyin4j.format.HanyuPinyinVCharType;
import net.sourceforge.pinyin4j.format.exception.BadHanyuPinyinOutputFormatCombination;
/**
 * 拼音操作类
 
 */
public class PinYinUtil {
	
	/**
	 * 获取汉字串拼音首字母，英文字符不变
	 * @param chinese    汉字串
	 * @return 汉语拼音首字母
	 */
	public static String cn2FirstSpell(String chinese) {
//		chinese = SBCToDBC.ToDBC(chinese);
		StringBuffer pybf = new StringBuffer();
		char[] arr = chinese.toCharArray();
		HanyuPinyinOutputFormat defaultFormat = new HanyuPinyinOutputFormat();
		defaultFormat.setCaseType(HanyuPinyinCaseType.LOWERCASE);
		defaultFormat.setToneType(HanyuPinyinToneType.WITHOUT_TONE);
		for (int i = 0; i < arr.length; i++) {
			if (arr[i] > 128) {
				try {
					String[] _t = PinyinHelper.toHanyuPinyinStringArray(arr[i], defaultFormat);
					if (_t != null) {
						pybf.append(_t[0].charAt(0));
					}
				} catch (BadHanyuPinyinOutputFormatCombination e) {
					e.printStackTrace();
				}
			} else {
				pybf.append(arr[i]);
			}
		}
		return pybf.toString().replaceAll("\\W", "").trim();
	}
	/**
	 * 获取汉字串拼音，英文字符不变
	 * @param chinese 汉字串
	 * @return 汉语拼音
	 */
	public static String cn2Spell(String chinese) {
		chinese = SBCToDBC.ToDBC(chinese);
		StringBuffer pybf = new StringBuffer();
		char[] arr = chinese.toCharArray();
		HanyuPinyinOutputFormat defaultFormat = new HanyuPinyinOutputFormat();
		defaultFormat.setCaseType(HanyuPinyinCaseType.LOWERCASE);
		defaultFormat.setToneType(HanyuPinyinToneType.WITHOUT_TONE);
		String[] hanPinYins = null;
		for (int i = 0; i < arr.length; i++) {
			if (arr[i] > 128) {
				try {
					hanPinYins = PinyinHelper.toHanyuPinyinStringArray(arr[i], defaultFormat);
					if(hanPinYins != null){
						pybf.append(hanPinYins[0]);
					}
//					pybf.append(PinyinHelper.toHanyuPinyinStringArray(arr[i], defaultFormat)[0]);
				} catch (BadHanyuPinyinOutputFormatCombination e) {
					e.printStackTrace();
				}
			} else {
				pybf.append(arr[i]);
			}
		}
		return pybf.toString();
	}
     /**
      * 获取汉字串拼音大写，英文字符不变
      * @param hanzhis
      * @return
      */
	public static String toPinYin(String hanzhis) {
		hanzhis = SBCToDBC.ToDBC(hanzhis);
		CharSequence s = hanzhis;
		char[] hanzhi = new char[s.length()];
		for (int i = 0; i < s.length(); i++) {
			hanzhi[i] = s.charAt(i);
		}
		char[] t1 = hanzhi;
		String[] t2 = new String[s.length()];
		/** */
		/**
		 * 设置输出格式
		 */
		net.sourceforge.pinyin4j.format.HanyuPinyinOutputFormat t3 = new HanyuPinyinOutputFormat();
		t3.setCaseType(HanyuPinyinCaseType.UPPERCASE);
		t3.setToneType(HanyuPinyinToneType.WITHOUT_TONE);
		t3.setVCharType(HanyuPinyinVCharType.WITH_V);
		int t0 = t1.length;
		String py = "";
		try {
			for (int i = 0; i < t0; i++) {
				t2 = PinyinHelper.toHanyuPinyinStringArray(t1[i], t3);
				py = py + t2[0].toString();
			}
		} catch (BadHanyuPinyinOutputFormatCombination e1) {
			e1.printStackTrace();
		}
		return py.trim();
	}
}
```
#### （2）汉字全角半角转换工具类：
```
package org.nercita.core.utils;
/**
 * 字符串全角/半角转换
 
 */
public class SBCToDBC {
    /**
     * 全角转半角
     * @param input 
     * @return 
     */
    public static String ToDBC(String input) {
    	char c[] = input.toCharArray();
    	for (int i = 0; i < c.length; i++) {
    		if (c[i] == '\u3000') {
    			c[i] = ' ';
    		} else if (c[i] > '\uFF00' && c[i] < '\uFF5F') {
    			c[i] = (char) (c[i] - 65248);
    		}
    	}
    	String returnString = new String(c);
    	return returnString;
    }
    /**
     * 半角转全角
     * @param input 
     * @return 
     */
    public static String ToSBC(String input) {
    	char c[] = input.toCharArray();
        for (int i = 0; i < c.length; i++) {
          if (c[i] == ' ') {
            c[i] = '\u3000';
          } else if (c[i] < '\177') {
            c[i] = (char) (c[i] + 65248);
          }
        }
        return new String(c);
    }    
}
```
### 五、显示效果
        显示效果如下：
        1、汉字匹配：
![](https://img-blog.csdn.net/20160812171057256?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       2、全拼音匹配：
![](https://img-blog.csdn.net/20160812171412868?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        3、汉字首字母简拼匹配：
![](https://img-blog.csdn.net/20160812171540697?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
