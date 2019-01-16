

#ifndef __IDCARD_H__
#define __IDCARD_H__

struct idcard {
  char gender[32]; // 性别
  char name[32]; // 名字
  char number[32];// 号码
  char birthday[32];// 出生日期
  char race[32];// 民族
  char address[128]; //地址
  char side[32];// front: 人像面back: 国徽面
  int type; // 证件类型。返回1，代表是身份证
};


int idcard_curr(char* aa) { return 0; }


#if 0
1~6位为地区代码，其中1、2位数为各省级政府的代码3、4位数为地、市级政府的代码5、6位数为县、区级政府代码
7~10位为出生年份4位
11~12位为出生月份
13~14位为出生日期第
15~17位为顺序号，为县、区级政府所辖派出所的分配码，每个派出所分配码位10个连续号码，
例如“150—159”，其中单数为男性分配码，双数为女性分配码，
如遇同年同月同日有两人以上时顺延第二、第三、第四、第五个分配码。
18位为效验位（识别码），通过复杂公式算出，普遍采用计算机自动生成。

身份证号码的编码含义：
数字编码实际应用于很多领域，比如：身份证号码、学号、车牌号、邮政编码、电话区号、银行帐号等等。
身份证号码共18位：AAAAAABBBBBBCCCD
1、号码的结构
公民身份号码是特征组合码，由十七位数字本体码和一位校验码组成。排列顺序从左至右依次为：六位数字地址码，八位数字出生日期码，三位数字顺序码和一位数字校验码。

2、地址码(前六位数）
  表示编码对象常住户口所在县(市、旗、区)的行政区划代码，按GB / T2260的规定执行。

  3、出生日期码（第七位至十四位）
  表示编码对象出生的年、月、日，按GB / T7408的规定执行，年、月、日代码之间不用分隔符。

  4、顺序码（第十五位至十七位）
  表示在同一地址码所标识的区域范围内，对同年、同月、同日出生的人编定的顺序号，顺序码的奇数分配给男性，偶数分配给女性。

  5、校验码（第十八位数）
  作为尾号的校验码，是由号码编制单位按统一的公式计算出来的，如果某人的尾号是0－9，都不会出现X，但如果尾号是10，那么就得用X来代替，因为如果用10做尾号，那么此人的身份证就变成了19位。X是罗马数字的10，用X来代替10，可以保证公民的身份证符合国家标准。


#endif


  // 省级政府的代码
#define PROVINCE_DEF(DEF) \
DEF(北京市,11) \
DEF(天津市,12) \
DEF(河北省,13) \
DEF(山西省,14) \
DEF(内蒙古自治区,15) \
DEF(辽宁省,21) \
DEF(吉林省,22) \
DEF(黑龙江省,23) \
DEF(上海市,31) \
DEF(江苏省,32) \
DEF(浙江省,33) \
DEF(安徽省,34) \
DEF(福建省,35) \
DEF(江西省,36) \
DEF(山东省,37) \
DEF(河南省,41) \
DEF(湖北省,42) \
DEF(湖南省,43) \
DEF(广东省,44) \
DEF(广西壮族自治区,45) \
DEF(海南省,46) \
DEF(重庆市,50) \
DEF(四川省,51) \
DEF(贵州省,52) \
DEF(云南省,53) \
DEF(西藏自治区,54) \
DEF(陕西省,61) \
DEF(甘肃省,62) \
DEF(青海省,63) \
DEF(宁夏回族自治区,64) \
DEF(新疆维吾尔自治区,65) \
DEF(台湾省,71) \
DEF(香港特别行政区,81) \
DEF(澳门特别行政区,82)


int myatoi(const char* number, int n) {
  char buf[64];
  if (n < 64) {
    memcpy(buf, number, n);
    buf[n] = 0;
    int k = atoi(buf);
    return k;
  }
  return 0;
}

  int idcard_get_info(const char* number, idcard* out) {
  int len = strlen(number);
  if (len != 18) return 0;
  int k;
  if (1) {
    k = myatoi(number + 6, 4);
    if (k < 1800 || k>3000) return 0;
    k = myatoi(number + 10, 2);
    if (k>13) return 0;
    k = myatoi(number + 12, 2);
    if (k>33) return 0;
  }
  memcpy(out->birthday, number + 6, 8);
  out->birthday[8] = 0;
  k = myatoi(number +14, 3);
  const char* gender = k & 1 ? "男" : "女";
  strcpy(out->gender, gender);
  return 1;
}

#if 0
/**
* 验证身份证
* @param IDStr
* @return "1" 代表合法的身份证 ,其他值代表错误信息
* @throws ParseException
*/
static String IDCardValidate(String IDStr) {
  String tipInfo = "1";// 记录错误信息 
  String Ai = "";

  if (null == IDStr || IDStr.trim().isEmpty())
    return "身份证号码长度应该为15位或18位。";

  // 判断号码的长度 15位或18位 
  if (IDStr.length() != 15 && IDStr.length() != 18) {
    tipInfo = "身份证号码长度应该为15位或18位。";
    return tipInfo;
  }
  // 18位身份证前17位位数字，如果是15位的身份证则所有号码都为数字 
  if (IDStr.length() == 18) {
    Ai = IDStr.substring(0, 17);
  }
  else if (IDStr.length() == 15) {
    Ai = IDStr.substring(0, 6) + "19" + IDStr.substring(6, 15);
  }
  if (isNumeric(Ai) == false) {
    tipInfo = "身份证15位号码都应为数字 ; 18位号码除最后一位外，都应为数字。";
    return tipInfo;
  }
  // 判断出生年月是否有效 
  String strYear = Ai.substring(6, 10);// 年份 
  String strMonth = Ai.substring(10, 12);// 月份 
  String strDay = Ai.substring(12, 14);// 日期 
  if (isDate(strYear + "-" + strMonth + "-" + strDay) == false) {
    tipInfo = "身份证出生日期无效。";
    return tipInfo;
  }
  GregorianCalendar gc = new GregorianCalendar();
  SimpleDateFormat s = new SimpleDateFormat("yyyy-MM-dd");
  try {
    if ((gc.get(Calendar.YEAR) - Integer.parseInt(strYear)) > 150
      || (gc.getTime().getTime() - s.parse(strYear + "-" + strMonth + "-" + strDay).getTime()) < 0) {
      tipInfo = "身份证生日不在有效范围。";
      return tipInfo;
    }
  }
  catch (NumberFormatException e) {
    e.printStackTrace();
  }
  catch (java.text.ParseException e) {
    e.printStackTrace();
  }
  if (Integer.parseInt(strMonth) > 12 || Integer.parseInt(strMonth) == 0) {
    tipInfo = "身份证月份无效";
    return tipInfo;
  }
  if (Integer.parseInt(strDay) > 31 || Integer.parseInt(strDay) == 0) {
    tipInfo = "身份证日期无效";
    return tipInfo;
  }
  // 判断地区码是否有效 
  Hashtable areacode = GetAreaCode();
  // 如果身份证前两位的地区码不在Hashtable，则地区码有误 
  if (areacode.get(Ai.substring(0, 2)) == null) {
    tipInfo = "身份证地区编码错误。";
    return tipInfo;
  }
  //判断第18位校验码是否正确
  if (isVarifyCode(Ai, IDStr) == false) {
    tipInfo = "身份证校验码无效，不是合法的身份证号码";
    return tipInfo;
  }
  return tipInfo;
}

/*
* 判断第18位校验码是否正确 第18位校验码的计算方式： 1. 对前17位数字本体码加权求和 公式为：S = Sum(Ai * Wi), i =
* 0, ... , 16 其中Ai表示第i个位置上的身份证号码数字值，Wi表示第i位置上的加权因子，其各位对应的值依次为： 7 9 10 5 8 4
* 2 1 6 3 7 9 10 5 8 4 2 2. 用11对计算结果取模 Y = mod(S, 11) 3. 根据模的值得到对应的校验码
* 对应关系为： Y值： 0 1 2 3 4 5 6 7 8 9 10 校验码： 1 0 X 9 8 7 6 5 4 3 2
*/
private static int isVarifyCode(String Ai, String IDStr) {
  String[] VarifyCode = { "1", "0", "X", "9", "8", "7", "6", "5", "4", "3", "2" };
  String[] Wi = { "7", "9", "10", "5", "8", "4", "2", "1", "6", "3", "7", "9", "10", "5", "8", "4", "2" };
  int sum = 0;
  for (int i = 0; i < 17; i++) {
    sum = sum + Integer.parseInt(String.valueOf(Ai.charAt(i))) * Integer.parseInt(Wi[i]);
  }
  int modValue = sum % 11;
  String strVerifyCode = VarifyCode[modValue];
  Ai = Ai + strVerifyCode;
  if (IDStr.length() == 18) {
    if (Ai.equals(IDStr) == false) {
      return false;
    }
  }
  return true;
}

/**
* 将所有地址编码保存在一个Hashtable中
*
* @return Hashtable 对象
*/
private static Hashtable GetAreaCode() {
  Hashtable hashtable = new Hashtable();
  hashtable.put("11", "北京");
  hashtable.put("12", "天津");
  hashtable.put("13", "河北");
  hashtable.put("14", "山西");
  hashtable.put("15", "内蒙古");
  hashtable.put("21", "辽宁");
  hashtable.put("22", "吉林");
  hashtable.put("23", "黑龙江");
  hashtable.put("31", "上海");
  hashtable.put("32", "江苏");
  hashtable.put("33", "浙江");
  hashtable.put("34", "安徽");
  hashtable.put("35", "福建");
  hashtable.put("36", "江西");
  hashtable.put("37", "山东");
  hashtable.put("41", "河南");
  hashtable.put("42", "湖北");
  hashtable.put("43", "湖南");
  hashtable.put("44", "广东");
  hashtable.put("45", "广西");
  hashtable.put("46", "海南");
  hashtable.put("50", "重庆");
  hashtable.put("51", "四川");
  hashtable.put("52", "贵州");
  hashtable.put("53", "云南");
  hashtable.put("54", "西藏");
  hashtable.put("61", "陕西");
  hashtable.put("62", "甘肃");
  hashtable.put("63", "青海");
  hashtable.put("64", "宁夏");
  hashtable.put("65", "新疆");
  hashtable.put("71", "台湾");
  hashtable.put("81", "香港");
  hashtable.put("82", "澳门");
  hashtable.put("91", "国外");
  return hashtable;
}


/**
* 判断字符串是否为数字,0-9重复0次或者多次
*
* @param strnum
* @return
*/
private static int isNumeric(String strnum) {
  Pattern pattern = Pattern.compile("[0-9]*");
  Matcher isNum = pattern.matcher(strnum);
  if (isNum.matches()) {
    return true;
  }
  else {
    return false;
  }
}

/**
* 功能：判断字符串出生日期是否符合正则表达式：包括年月日，闰年、平年和每月31天、30天和闰月的28天或者29天
*
* @param string
* @return
*/
static int isDate(String strDate) {
  Pattern pattern = Pattern.compile(
    "^((\\d{2}(([02468][048])|([13579][26]))[\\-\\/\\s]?((((0?[13578])|(1[02]))[\\-\\/\\s]?((0?[1-9])|([1-2][0-9])|(3[01])))|(((0?[469])|(11))[\\-\\/\\s]?((0?[1-9])|([1-2][0-9])|(30)))|(0?2[\\-\\/\\s]?((0?[1-9])|([1-2][0-9])))))|(\\d{2}(([02468][1235679])|([13579][01345789]))[\\-\\/\\s]?((((0?[13578])|(1[02]))[\\-\\/\\s]?((0?[1-9])|([1-2][0-9])|(3[01])))|(((0?[469])|(11))[\\-\\/\\s]?((0?[1-9])|([1-2][0-9])|(30)))|(0?2[\\-\\/\\s]?((0?[1-9])|(1[0-9])|(2[0-8]))))))?$");
  Matcher m = pattern.matcher(strDate);
  if (m.matches()) {
    return true;
  }
  else {
    return false;
  }
}

#endif
#endif // __IDCARD_H__
