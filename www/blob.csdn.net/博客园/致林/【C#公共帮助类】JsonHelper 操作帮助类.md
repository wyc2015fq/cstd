
# 【C\#公共帮助类】JsonHelper 操作帮助类 - 致林 - 博客园






# [【C\#公共帮助类】JsonHelper 操作帮助类](https://www.cnblogs.com/bincoding/p/8182122.html)
**四个主要操作类：JsonConverter 、JsonHelper 、JsonSplit 、****AjaxResult**
**一、JsonConverter：**
**自定义查询对象转换动态类、object动态类转换json包、json转换object动态类、DataReader转换为Json、DataSet转换为Json、DataTable转成Json、Datatable转换为Json 、格式化字符型日期型布尔型、过滤特殊字符等**
usingSystem;usingSystem.Collections.Generic;usingSystem.Text;usingSystem.Data;usingSystem.Linq;usingSystem.Web.Script.Serialization;usingSystem.Collections;namespaceCommon
{publicclassJsonConverter
    {///<summary>///自定义查询对象转换动态类///add yuangang by 2015-05-19///</summary>///<param name="obj"></param>///<returns></returns>publicstaticdynamicJsonClass(objectobj) 
        {returnConvertJson(Serialize(obj,true));
        }///<summary>///object动态类转换json包///add yuangang by 2015-05-19///</summary>///<param name="obj">对象</param>///<param name="DateConvert">时间戳是否转换成日期类型</param>///<returns></returns>publicstaticstringSerialize(objectobj,boolDateConvert =false)
        {
            JavaScriptSerializer jss=newJavaScriptSerializer();varstr =jss.Serialize(obj);if(DateConvert)
            {
                str= System.Text.RegularExpressions.Regex.Replace(str,@"\\/Date\((\d+)\)\\/", match =>{
                    DateTime dt=newDateTime(1970,1,1);
                    dt= dt.AddMilliseconds(long.Parse(match.Groups[1].Value));
                    dt=dt.ToLocalTime();returndt.ToString("yyyy-MM-dd HH:mm:ss");
                });
            }returnstr;
        }///<summary>///json转换object动态类///add yuangang by 2015-05-19///</summary>///<param name="json"></param>///<returns></returns>publicstaticdynamicConvertJson(stringjson)
        {
            JavaScriptSerializer jss=newJavaScriptSerializer();
            jss.RegisterConverters(newJavaScriptConverter[] {newDynamicJsonConverter() });dynamicdy = jss.Deserialize(json,typeof(object))asdynamic;returndy;
        }///<summary>///DataReader转换为Json///</summary>///<param name="dataReader">DataReader对象</param>///<returns>Json字符串</returns>publicstaticstringToJson(IDataReader dataReader)
        {try{
                StringBuilder jsonString=newStringBuilder();
                jsonString.Append("[");while(dataReader.Read())
                {
                    jsonString.Append("{");for(inti =0; i < dataReader.FieldCount; i++)
                    {
                        Type type=dataReader.GetFieldType(i);stringstrKey =dataReader.GetName(i);stringstrValue =dataReader[i].ToString();
                        jsonString.Append("\""+ strKey +"\":");
                        strValue=StringFormat(strValue, type);if(i < dataReader.FieldCount -1)
                        {
                            jsonString.Append(strValue+",");
                        }else{
                            jsonString.Append(strValue);
                        }
                    }
                    jsonString.Append("},");
                }if(!dataReader.IsClosed)
                {
                    dataReader.Close();
                }
                jsonString.Remove(jsonString.Length-1,1);
                jsonString.Append("]");if(jsonString.Length ==1)
                {return"[]";
                }returnjsonString.ToString();
            }catch(Exception ex)
            {throwex;
            }
        }///<summary>///DataSet转换为Json///add yuangang by 2015-05-19///</summary>///<param name="dataSet">DataSet对象</param>///<returns>Json字符串</returns>publicstaticstringToJson(DataSet dataSet)
        {stringjsonString ="{";foreach(DataTable tableindataSet.Tables)
            {
                jsonString+="\""+ table.TableName +"\":"+ ToJson(table) +",";
            }
            jsonString= jsonString.TrimEnd(',');returnjsonString +"}";
        }///<summary>///DataTable转成Json///add yuangang by 2015-05-19///</summary>///<param name="jsonName"></param>///<param name="dt"></param>///<returns></returns>publicstaticstringToJson(DataTable dt,stringjsonName)
        {
            StringBuilder Json=newStringBuilder();if(string.IsNullOrEmpty(jsonName))
                jsonName=dt.TableName;
            Json.Append("{\""+ jsonName +"\":[");if(dt.Rows.Count >0)
            {for(inti =0; i < dt.Rows.Count; i++)
                {
                    Json.Append("{");for(intj =0; j < dt.Columns.Count; j++)
                    {
                        Type type=dt.Rows[i][j].GetType();
                        Json.Append("\""+ dt.Columns[j].ColumnName.ToString() +"\":"+ StringFormat(dt.Rows[i][j]isDBNull ?string.Empty : dt.Rows[i][j].ToString(), type));if(j < dt.Columns.Count -1)
                        {
                            Json.Append(",");
                        }
                    }
                    Json.Append("}");if(i < dt.Rows.Count -1)
                    {
                        Json.Append(",");
                    }
                }
            }
            Json.Append("]}");returnJson.ToString();
        }///<summary>///Datatable转换为Json///add yuangang by 2015-05-19///</summary>///<param name="table">Datatable对象</param>///<returns>Json字符串</returns>publicstaticstringToJson(DataTable dt)
        {
            StringBuilder jsonString=newStringBuilder();
            jsonString.Append("[");
            DataRowCollection drc=dt.Rows;for(inti =0; i < drc.Count; i++)
            {
                jsonString.Append("{");for(intj =0; j < dt.Columns.Count; j++)
                {stringstrKey =dt.Columns[j].ColumnName;stringstrValue =drc[i][j].ToString();
                    Type type=dt.Columns[j].DataType;
                    jsonString.Append("\""+ strKey +"\":");
                    strValue=StringFormat(strValue, type);if(j < dt.Columns.Count -1)
                    {
                        jsonString.Append(strValue+",");
                    }else{
                        jsonString.Append(strValue);
                    }
                }
                jsonString.Append("},");
            }
            jsonString.Remove(jsonString.Length-1,1);
            jsonString.Append("]");if(jsonString.Length ==1)
            {return"[]";
            }returnjsonString.ToString();
        }///<summary>///格式化字符型、日期型、布尔型///add yuangang by 2015-05-19///</summary>///<param name="str"></param>///<param name="type"></param>///<returns></returns>privatestaticstringStringFormat(stringstr, Type type)
        {if(type !=typeof(string) &&string.IsNullOrEmpty(str))
            {
                str="\""+ str +"\"";
            }elseif(type ==typeof(string))
            {
                str=String2Json(str);
                str="\""+ str +"\"";
            }elseif(type ==typeof(DateTime))
            {
                str="\""+ str +"\"";
            }elseif(type ==typeof(bool))
            {
                str=str.ToLower();
            }elseif(type ==typeof(byte[]))
            {
                str="\""+ str +"\"";
            }elseif(type ==typeof(Guid))
            {
                str="\""+ str +"\"";
            }returnstr;
        }///<summary>///过滤特殊字符///add yuangang by 2015-05-19///</summary>///<param name="s"></param>///<returns></returns>publicstaticstringString2Json(String s)
        {
            StringBuilder sb=newStringBuilder();for(inti =0; i < s.Length; i++)
            {charc =s.ToCharArray()[i];switch(c)
                {case'\"':
                        sb.Append("\\\"");break;case'\\':
                        sb.Append("\\\\");break;case'/':
                        sb.Append("\\/");break;case'\b':
                        sb.Append("\\b");break;case'\f':
                        sb.Append("\\f");break;case'\n':
                        sb.Append("\\n");break;case'\r':
                        sb.Append("\\r");break;case'\t':
                        sb.Append("\\t");break;case'\v':
                        sb.Append("\\v");break;case'\0':
                        sb.Append("\\0");break;default:
                        sb.Append(c);break;
                }
            }returnsb.ToString();
        }publicstaticstringGetDataGridJsonByDataSet(DataSet ds,stringtotalProperty,stringroot)
        {returnGetDataGridJsonByDataTable(ds.Tables[0], totalProperty, root);
        }publicstaticstringGetDataGridJsonByDataTable(DataTable dt,stringtotalProperty,stringroot)
        {
            StringBuilder jsonBuilder=newStringBuilder();
            jsonBuilder.Append("({\""+ totalProperty +"\":\""+ dt.Rows.Count +"\",");
            jsonBuilder.Append("\"");
            jsonBuilder.Append(root);
            jsonBuilder.Append("\":[");for(inti =0; i < dt.Rows.Count; i++)
            {
                jsonBuilder.Append("{");for(intj =0; j < dt.Columns.Count; j++)
                {
                    jsonBuilder.Append("\"");
                    jsonBuilder.Append(dt.Columns[j].ColumnName);
                    jsonBuilder.Append("\":\"");
                    jsonBuilder.Append(dt.Rows[i][j].ToString());
                    jsonBuilder.Append("\",");
                }
                jsonBuilder.Remove(jsonBuilder.Length-1,1);
                jsonBuilder.Append("},");
            }
            jsonBuilder.Remove(jsonBuilder.Length-1,1);
            jsonBuilder.Append("]");
            jsonBuilder.Append("})");returnjsonBuilder.ToString();
        }publicstaticstringGetTreeJsonByDataSet(DataSet ds)
        {returnGetTreeJsonByDataTable(ds.Tables[0]);
        }publicstaticstringGetTreeJsonByDataTable(DataTable dataTable)
        {
            DataTable dt=FormatDataTableForTree(dataTable);
            StringBuilder jsonBuilder=newStringBuilder();
            jsonBuilder.Append("[");for(inti =0; i < dt.Rows.Count; i++)
            {
                jsonBuilder.Append("{");for(intj =0; j < dt.Columns.Count; j++)
                {
                    jsonBuilder.Append("\'");if(dt.Columns[j].ColumnName =="leaf")
                    {stringleafValue =dt.Rows[i][j].ToString();if(!string.IsNullOrEmpty(leafValue))
                        {
                            jsonBuilder.Append(dt.Columns[j].ColumnName);
                            jsonBuilder.Append("\':\'");
                            jsonBuilder.Append(dt.Rows[i][j].ToString());
                            jsonBuilder.Append("\',");
                        }else{
                            jsonBuilder.Remove(jsonBuilder.Length-1,1);
                        }
                    }elseif(dt.Columns[j].ColumnName =="customUrl")
                    {
                        jsonBuilder.Remove(jsonBuilder.Length-1,1);
                        jsonBuilder.Append(dt.Columns[j].ColumnName);
                        jsonBuilder.Append(":\'");
                        jsonBuilder.Append(dt.Rows[i][j].ToString());
                        jsonBuilder.Append("\',");
                    }else{
                        jsonBuilder.Append(dt.Columns[j].ColumnName);
                        jsonBuilder.Append("\':\'");
                        jsonBuilder.Append(dt.Rows[i][j].ToString());
                        jsonBuilder.Append("\',");
                    }
                }
                jsonBuilder.Remove(jsonBuilder.Length-1,1);
                jsonBuilder.Append("},");
            }
            jsonBuilder.Remove(jsonBuilder.Length-1,1);
            jsonBuilder.Append("]");returnjsonBuilder.ToString();
        }privatestaticDataTable FormatDataTableForTree(DataTable dt)
        {
            DataTable dtTree=newDataTable();
            dtTree.Columns.Add("id",typeof(string));
            dtTree.Columns.Add("text",typeof(string));
            dtTree.Columns.Add("leaf",typeof(string));
            dtTree.Columns.Add("cls",typeof(string));
            dtTree.Columns.Add("customUrl",typeof(string));
            dtTree.AcceptChanges();for(inti =0; i < dt.Rows.Count; i++)
            {
                DataRow drTree=dtTree.NewRow();
                drTree["id"] = dt.Rows[i]["id"].ToString();
                drTree["text"] = dt.Rows[i]["text"].ToString();if(dt.Rows[i]["leaf"].ToString() =="Y")
                {
                    drTree["leaf"] ="true";
                    drTree["cls"] ="file";
                }else{
                    drTree["cls"] ="folder";
                }
                drTree["customUrl"] = dt.Rows[i]["customUrl"].ToString();
                dtTree.Rows.Add(drTree);
            }returndtTree;
        }
  
    }///<summary>///动态JSON解析///add yuangang by 2015-05-19///</summary>publicclassDynamicJsonObject : System.Dynamic.DynamicObject
    {privateIDictionary<string,object> Dictionary {get;set; }publicDynamicJsonObject(IDictionary<string,object>dictionary)
        {this.Dictionary =dictionary;
        }publicoverrideboolTryGetMember(System.Dynamic.GetMemberBinder binder,outobjectresult)
        {
            result=this.Dictionary[binder.Name];if(resultisIDictionary<string,object>)
            {
                result=newDynamicJsonObject(resultasIDictionary<string,object>);
            }elseif(resultisArrayList && (resultasArrayList)isIDictionary<string,object>)
            {
                result=newList<DynamicJsonObject>((resultasArrayList).ToArray().Select(x =>newDynamicJsonObject(xasIDictionary<string,object>)));
            }elseif(resultisArrayList)
            {
                result=newList<object>((resultasArrayList).ToArray());
            }returnthis.Dictionary.ContainsKey(binder.Name);
        }
    }///<summary>///动态JSON转换///add yuangang by 2015-05-19///</summary>publicclassDynamicJsonConverter : JavaScriptConverter
    {publicoverrideobjectDeserialize(IDictionary<string,object>dictionary, Type type, JavaScriptSerializer serializer)
        {if(dictionary ==null)thrownewArgumentNullException("dictionary");if(type ==typeof(object))
            {returnnewDynamicJsonObject(dictionary);
            }returnnull;
        }publicoverrideIDictionary<string,object> Serialize(objectobj, JavaScriptSerializer serializer)
        {thrownewNotImplementedException();
        }publicoverrideIEnumerable<Type>SupportedTypes
        {get{returnnewSystem.Collections.ObjectModel.ReadOnlyCollection<Type>(newList<Type>(newType[] {typeof(object) })); }
        }
    }
}

**-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------**
**二、JsonHelper：****json的辅助类**
usingSystem.Collections.Generic;usingSystem.Text;usingNewtonsoft.Json;usingSystem.Runtime.Serialization.Json;usingSystem.IO;namespaceCommon
{///<summary>///提供了一个关于json的辅助类///</summary>publicclassJsonHelper
    {\#regionMethod///<summary>///类对像转换成json格式///</summary>///<returns></returns>publicstaticstringToJson(objectt)
        {returnJsonConvert.SerializeObject(t, Newtonsoft.Json.Formatting.Indented,newJsonSerializerSettings { NullValueHandling =NullValueHandling.Include });
        }///<summary>///类对像转换成json格式///</summary>///<param name="t"></param>///<param name="HasNullIgnore">是否忽略NULL值</param>///<returns></returns>publicstaticstringToJson(objectt,boolHasNullIgnore)
        {if(HasNullIgnore)returnJsonConvert.SerializeObject(t, Newtonsoft.Json.Formatting.Indented,newJsonSerializerSettings { NullValueHandling =NullValueHandling.Ignore });elsereturnToJson(t);
        }///<summary>///json格式转换///</summary>///<typeparam name="T"></typeparam>///<param name="strJson"></param>///<returns></returns>publicstaticT FromJson<T>(stringstrJson)whereT :class{if(!strJson.IsNullOrEmpty())returnJsonConvert.DeserializeObject<T>(strJson);returnnull;
        }///<summary>///功能描述：将List转换为Json///</summary>///<param name="a"></param>///<returns></returns>publicstaticstringListToJson(IList<object>a)
        {
            DataContractJsonSerializer json=newDataContractJsonSerializer(a.GetType());stringszJson ="";//序列化using(MemoryStream stream =newMemoryStream())
            {
                json.WriteObject(stream, a);
                szJson=Encoding.UTF8.GetString(stream.ToArray());
            }returnszJson;
        }\#endregion\#regionProperty///<summary>///数据状态///</summary>publicstringStatus {get;set; }///<summary>///提示信息///</summary>publicstringMsg {get;set; }///<summary>///回传URL///</summary>publicstringReUrl {get;set; }///<summary>///数据包///</summary>publicobjectData {get;set; }\#endregion}
}
**-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------**
**三、JsonSplit：判断字符串是否为Json**
usingSystem;usingSystem.Collections.Generic;usingSystem.Linq;usingSystem.Web;namespaceCommon
{///<summary>///判断字符串是否为JSON///</summary>publicclassJsonSplit
    {privatestaticboolIsJsonStart(refstringjson)
        {if(!string.IsNullOrEmpty(json))
            {
                json= json.Trim('\r','\n','');if(json.Length >1)
                {chars = json[0];chare = json[json.Length -1];return(s =='{'&& e =='}') || (s =='['&& e ==']');
                }
            }returnfalse;
        }publicstaticboolIsJson(stringjson)
        {interrIndex;returnIsJson(json,outerrIndex);
        }publicstaticboolIsJson(stringjson,outinterrIndex)
        {
            errIndex=0;if(IsJsonStart(refjson))
            {
                CharState cs=newCharState();charc;for(inti =0; i < json.Length; i++)
                {
                    c=json[i];if(SetCharState(c,refcs) && cs.childrenStart)//设置关键符号状态。{stringitem =json.Substring(i);interr;intlength = GetValueLength(item,true,outerr);
                        cs.childrenStart=false;if(err >0)
                        {
                            errIndex= i +err;returnfalse;
                        }
                        i= i + length -1;
                    }if(cs.isError)
                    {
                        errIndex=i;returnfalse;
                    }
                }return!cs.arrayStart && !cs.jsonStart;
            }returnfalse;
        }///<summary>///获取值的长度（当Json值嵌套以"{"或"["开头时）///</summary>privatestaticintGetValueLength(stringjson,boolbreakOnErr,outinterrIndex)
        {
            errIndex=0;intlen =0;if(!string.IsNullOrEmpty(json))
            {
                CharState cs=newCharState();charc;for(inti =0; i < json.Length; i++)
                {
                    c=json[i];if(!SetCharState(c,refcs))//设置关键符号状态。{if(!cs.jsonStart && !cs.arrayStart)//json结束，又不是数组，则退出。{break;
                        }
                    }elseif(cs.childrenStart)//正常字符，值状态下。{intlength = GetValueLength(json.Substring(i), breakOnErr,outerrIndex);//递归子值，返回一个长度。。。cs.childrenStart =false;
                        cs.valueStart=0;//cs.state = 0;i = i + length -1;
                    }if(breakOnErr &&cs.isError)
                    {
                        errIndex=i;returni;
                    }if(!cs.jsonStart && !cs.arrayStart)//记录当前结束位置。{
                        len= i +1;//长度比索引+1break;
                    }
                }
            }returnlen;
        }///<summary>///字符状态///</summary>privateclassCharState
        {internalbooljsonStart =false;//以 "{"开始了...internalboolsetDicValue =false;//可以设置字典值了。internalboolescapeChar =false;//以"\"转义符号开始了///<summary>///数组开始【仅第一开头才算】，值嵌套的以【childrenStart】来标识。///</summary>internalboolarrayStart =false;//以"[" 符号开始了internalboolchildrenStart =false;//子级嵌套开始了。///<summary>///【0 初始状态，或 遇到“,”逗号】；【1 遇到“：”冒号】///</summary>internalintstate =0;///<summary>///【-1 取值结束】【0 未开始】【1 无引号开始】【2 单引号开始】【3 双引号开始】///</summary>internalintkeyStart =0;///<summary>///【-1 取值结束】【0 未开始】【1 无引号开始】【2 单引号开始】【3 双引号开始】///</summary>internalintvalueStart =0;internalboolisError =false;//是否语法错误。internalvoidCheckIsError(charc)//只当成一级处理（因为GetLength会递归到每一个子项处理）{if(keyStart >1|| valueStart >1)
                {return;
                }//示例 ["aa",{"bbbb":123,"fff","ddd"}]switch(c)
                {case'{'://[{ "[{A}]":[{"[{B}]":3,"m":"C"}]}]isError = jsonStart && state ==0;//重复开始错误 同时不是值处理。break;case'}':
                        isError= !jsonStart || (keyStart !=0&& state ==0);//重复结束错误 或者 提前结束{"aa"}。正常的有{}break;case'[':
                        isError= arrayStart && state ==0;//重复开始错误break;case']':
                        isError= !arrayStart || jsonStart;//重复开始错误 或者 Json 未结束break;case'"':case'\'':
                        isError= !(jsonStart || arrayStart);//json 或数组开始。if(!isError)
                        {//重复开始 [""",{"" "}]isError = (state ==0&& keyStart == -1) || (state ==1&& valueStart == -1);
                        }if(!isError && arrayStart && !jsonStart && c =='\'')//['aa',{}]{
                            isError=true;
                        }break;case':':
                        isError= !jsonStart || state ==1;//重复出现。break;case',':
                        isError= !(jsonStart || arrayStart);//json 或数组开始。if(!isError)
                        {if(jsonStart)
                            {
                                isError= state ==0|| (state ==1&& valueStart >1);//重复出现。}elseif(arrayStart)//["aa,] [,]  [{},{}]{
                                isError= keyStart ==0&& !setDicValue;
                            }
                        }break;case'':case'\r':case'\n'://[ "a",\r\n{} ]case'\0':case'\t':break;default://值开头。。isError = (!jsonStart && !arrayStart) || (state ==0&& keyStart == -1) || (valueStart == -1&& state ==1);//break;
                }//if (isError)//{//}}
        }///<summary>///设置字符状态(返回true则为关键词，返回false则当为普通字符处理）///</summary>privatestaticboolSetCharState(charc,refCharState cs)
        {
            cs.CheckIsError(c);switch(c)
            {case'{'://[{ "[{A}]":[{"[{B}]":3,"m":"C"}]}]\#region大括号if(cs.keyStart <=0&& cs.valueStart <=0)
                    {
                        cs.keyStart=0;
                        cs.valueStart=0;if(cs.jsonStart && cs.state ==1)
                        {
                            cs.childrenStart=true;
                        }else{
                            cs.state=0;
                        }
                        cs.jsonStart=true;//开始。returntrue;
                    }\#endregionbreak;case'}':\#region大括号结束if(cs.keyStart <=0&& cs.valueStart <2&&cs.jsonStart)
                    {
                        cs.jsonStart=false;//正常结束。cs.state =0;
                        cs.keyStart=0;
                        cs.valueStart=0;
                        cs.setDicValue=true;returntrue;
                    }//cs.isError = !cs.jsonStart && cs.state == 0;\#endregionbreak;case'[':\#region中括号开始if(!cs.jsonStart)
                    {
                        cs.arrayStart=true;returntrue;
                    }elseif(cs.jsonStart && cs.state ==1)
                    {
                        cs.childrenStart=true;returntrue;
                    }\#endregionbreak;case']':\#region中括号结束if(cs.arrayStart && !cs.jsonStart && cs.keyStart <=2&& cs.valueStart <=0)//[{},333]//这样结束。{
                        cs.keyStart=0;
                        cs.valueStart=0;
                        cs.arrayStart=false;returntrue;
                    }\#endregionbreak;case'"':case'\'':\#region引号if(cs.jsonStart ||cs.arrayStart)
                    {if(cs.state ==0)//key阶段,有可能是数组["aa",{}]{if(cs.keyStart <=0)
                            {
                                cs.keyStart= (c =='"'?3:2);returntrue;
                            }elseif((cs.keyStart ==2&& c =='\'') || (cs.keyStart ==3&& c =='"'))
                            {if(!cs.escapeChar)
                                {
                                    cs.keyStart= -1;returntrue;
                                }else{
                                    cs.escapeChar=false;
                                }
                            }
                        }elseif(cs.state ==1&& cs.jsonStart)//值阶段必须是Json开始了。{if(cs.valueStart <=0)
                            {
                                cs.valueStart= (c =='"'?3:2);returntrue;
                            }elseif((cs.valueStart ==2&& c =='\'') || (cs.valueStart ==3&& c =='"'))
                            {if(!cs.escapeChar)
                                {
                                    cs.valueStart= -1;returntrue;
                                }else{
                                    cs.escapeChar=false;
                                }
                            }
                        }
                    }\#endregionbreak;case':':\#region冒号if(cs.jsonStart && cs.keyStart <2&& cs.valueStart <2&& cs.state ==0)
                    {if(cs.keyStart ==1)
                        {
                            cs.keyStart= -1;
                        }
                        cs.state=1;returntrue;
                    }//cs.isError = !cs.jsonStart || (cs.keyStart < 2 && cs.valueStart < 2 && cs.state == 1);\#endregionbreak;case',':\#region逗号//["aa",{aa:12,}]if(cs.jsonStart)
                    {if(cs.keyStart <2&& cs.valueStart <2&& cs.state ==1)
                        {
                            cs.state=0;
                            cs.keyStart=0;
                            cs.valueStart=0;//if (cs.valueStart == 1)//{//cs.valueStart = 0;//}cs.setDicValue =true;returntrue;
                        }
                    }elseif(cs.arrayStart && cs.keyStart <=2)
                    {
                        cs.keyStart=0;//if (cs.keyStart == 1)//{//cs.keyStart = -1;//}returntrue;
                    }\#endregionbreak;case'':case'\r':case'\n'://[ "a",\r\n{} ]case'\0':case'\t':if(cs.keyStart <=0&& cs.valueStart <=0)//cs.jsonStart &&{returntrue;//跳过空格。}break;default://值开头。。if(c =='\\')//转义符号{if(cs.escapeChar)
                        {
                            cs.escapeChar=false;
                        }else{
                            cs.escapeChar=true;returntrue;
                        }
                    }else{
                        cs.escapeChar=false;
                    }if(cs.jsonStart || cs.arrayStart)//Json 或数组开始了。{if(cs.keyStart <=0&& cs.state ==0)
                        {
                            cs.keyStart=1;//无引号的}elseif(cs.valueStart <=0&& cs.state ==1&& cs.jsonStart)//只有Json开始才有值。{
                            cs.valueStart=1;//无引号的}
                    }break;
            }returnfalse;
        }
    }
}
**-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------**
**四、AjaxResult：前台Ajax请求的统一返回结果类**
usingSystem.Web.Script.Serialization;namespaceCommon
{///<summary>///前台Ajax请求的统一返回结果类///</summary>publicclassAjaxResult
    {privateAjaxResult()
        {
        }privatebooliserror =false;///<summary>///是否产生错误///</summary>publicboolIsError {get{returniserror; } }///<summary>///错误信息，或者成功信息///</summary>publicstringMessage {get;set; }///<summary>///成功可能时返回的数据///</summary>publicobjectData {get;set; }\#regionErrorpublicstaticAjaxResult Error()
        {returnnewAjaxResult()
            {
                iserror=true};
        }publicstaticAjaxResult Error(stringmessage)
        {returnnewAjaxResult()
            {
                iserror=true,
                Message=message
            };
        }\#endregion\#regionSuccesspublicstaticAjaxResult Success()
        {returnnewAjaxResult()
            {
                iserror=false};
        }publicstaticAjaxResult Success(stringmessage)
        {returnnewAjaxResult()
            {
                iserror=false,
                Message=message
            };
        }publicstaticAjaxResult Success(objectdata)
        {returnnewAjaxResult()
            {
                iserror=false,
                Data=data
            };
        }publicstaticAjaxResult Success(objectdata,stringmessage)
        {returnnewAjaxResult()
            {
                iserror=false,
                Data=data,
                Message=message
            };
        }\#endregion///<summary>///序列化///</summary>///<returns></returns>publicoverridestringToString()
        {returnnewJavaScriptSerializer().Serialize(this);
        }
    }
}

常用的一些Json操作类，留备。会继续为大家分享一些 常用的 公共帮助类。
原创文章 转载请尊重劳动成果[http://yuangang.cnblogs.com](http://yuangang.cnblogs.com/)





