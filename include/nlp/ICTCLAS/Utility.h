//////////////////////////////////////////////////////////////////////
//ICTCLAS简介：计算所汉语词法分析系统ICTCLAS(Institute of Computing Technology, Chinese Lexical Analysis System)，
//             功能有：中文分词；词性标注；未登录词识别。
//             分词正确率高达97.58%(973专家评测结果)，
//             未登录词识别召回率均高于90%，其中中国人名的识别召回率接近98%;
//             处理速度为31.5Kbytes/s。
//著作权：  Copyright?2002-2005中科院计算所 职务著作权人：张华平 刘群
//遵循协议：自然语言处理开放资源许可证1.0
//Email: zhanghp@software.ict.ac.cn
//Homepage:www.nlp.org.cn;mtgroup.ict.ac.cn
/****************************************************************************
 *
 * Copyright (c) 2000, 2001
 *     Machine Group
 *     Software Research Lab.
 *     Institute of Computing Tech.
 *     Chinese Academy of Sciences
 *     All rights reserved.
 *
 * This file is the confidential and proprietary property of
 * Institute of Computing Tech. and the posession or use of this file requires
 * a written license from the author.
 * Filename: Utility.h
 * Abstract:
 *           Utility functions for Chinese Language Processing
 * Author:   Kevin Zhang
 *          (zhanghp@software.ict.ac.cn)
 * Date:     2002-1-8
 *
 * Notes:
 *
 *
 ****************************************************************************/
#if !defined(AFX_CHINESE_UTILITY_H__B6D7EA03_7BCD_46AD_B38C_D8033ACD5813__INCLUDED_)
#define AFX_CHINESE_UTILITY_H__B6D7EA03_7BCD_46AD_B38C_D8033ACD5813__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  CT_SENTENCE_BEGIN 1//Sentence begin 
#define  CT_SENTENCE_END   4//Sentence ending
#define  CT_SINGLE  5//SINGLE byte
#define  CT_DELIMITER CT_SINGLE+1//delimiter
#define  CT_CHINESE   CT_SINGLE+2//Chinese Char
#define  CT_LETTER    CT_SINGLE+3//HanYu Pinyin
#define  CT_NUM       CT_SINGLE+4//HanYu Pinyin
#define  CT_INDEX     CT_SINGLE+5//HanYu Pinyin
#define  CT_OTHER     CT_SINGLE+12//Other
#define POSTFIX_SINGLE "坝邦堡杯城池村单岛道堤店洞渡队法峰府冈港阁宫沟国海号河湖环集江奖礁角街井郡坑口矿里岭楼路门盟庙弄牌派坡铺旗桥区渠泉人山省市水寺塔台滩坛堂厅亭屯湾文屋溪峡县线乡巷型洋窑营屿语园苑院闸寨站镇州庄族陂庵町"
#define POSTFIX_MUTIPLE {"半岛","草原","城市","大堤","大公国","大桥","地区","帝国","渡槽","港口","高速公路","高原","公路","公园","共和国","谷地","广场","国道","海峡","胡同","机场","集镇","教区","街道","口岸","码头","煤矿","牧场","农场","盆地","平原","丘陵","群岛","沙漠","沙洲","山脉","山丘","水库","隧道","特区","铁路","新村","雪峰","盐场","盐湖","渔场","直辖市","自治区","自治县","自治州",""}

#define  TRANS_ENGLISH   "·—阿埃艾爱安昂敖奥澳笆芭巴白拜班邦保堡鲍北贝本比毕彼别波玻博勃伯泊卜布才采仓查差柴彻川茨慈次达大戴代丹旦但当道德得的登迪狄蒂帝丁东杜敦多额俄厄鄂恩尔伐法范菲芬费佛夫福弗甫噶盖干冈哥戈革葛格各根古瓜哈海罕翰汗汉豪合河赫亨侯呼胡华霍基吉及加贾坚简杰金京久居君喀卡凯坎康考柯科可克肯库奎拉喇莱来兰郎朗劳勒雷累楞黎理李里莉丽历利立力连廉良列烈林隆卢虏鲁路伦仑罗洛玛马买麦迈曼茅茂梅门蒙盟米蜜密敏明摩莫墨默姆木穆那娜纳乃奈南内尼年涅宁纽努诺欧帕潘畔庞培佩彭皮平泼普其契恰强乔切钦沁泉让热荣肉儒瑞若萨塞赛桑瑟森莎沙山善绍舍圣施诗石什史士守斯司丝苏素索塔泰坦汤唐陶特提汀图土吐托陀瓦万王旺威韦维魏温文翁沃乌吾武伍西锡希喜夏相香歇谢辛新牙雅亚彦尧叶依伊衣宜义因音英雍尤于约宰泽增詹珍治中仲朱诸卓孜祖佐伽娅尕腓滕济嘉津赖莲琳律略慕妮聂裴浦奇齐琴茹珊卫欣逊札哲智兹芙汶迦珀琪梵斐胥黛"
#define  TRANS_RUSSIAN   "·阿安奥巴比彼波布察茨大德得丁杜尔法夫伏甫盖格哈基加坚捷金卡科可克库拉莱兰勒雷里历利连列卢鲁罗洛马梅蒙米姆娜涅宁诺帕泼普奇齐乔切日萨色山申什斯索塔坦特托娃维文乌西希谢亚耶叶依伊以扎佐柴达登蒂戈果海赫华霍吉季津柯理琳玛曼穆纳尼契钦丘桑沙舍泰图瓦万雅卓兹"
#define  TRANS_JAPANESE  "安奥八白百邦保北倍本比滨博步部彩菜仓昌长朝池赤川船淳次村大代岛稻道德地典渡尔繁饭风福冈高工宫古谷关广桂贵好浩和合河黑横恒宏后户荒绘吉纪佳加见健江介金今进井静敬靖久酒菊俊康可克口梨理里礼栗丽利立凉良林玲铃柳隆鹿麻玛美萌弥敏木纳南男内鸟宁朋片平崎齐千前浅桥琴青清庆秋丘曲泉仁忍日荣若三森纱杉山善上伸神圣石实矢世市室水顺司松泰桃藤天田土万望尾未文武五舞西细夏宪相小孝新星行雄秀雅亚岩杨洋阳遥野也叶一伊衣逸义益樱永由有佑宇羽郁渊元垣原远月悦早造则泽增扎宅章昭沼真政枝知之植智治中忠仲竹助椎子佐阪坂堀荻菅薰浜濑鸠筱"
//Translation type
#define  TT_ENGLISH  0
#define  TT_RUSSIAN  1
#define  TT_JAPANESE  2
//Seperator type
#define  SEPERATOR_C_SENTENCE "。！？：；…"
#define  SEPERATOR_C_SUB_SENTENCE "、，（）“”‘’"
#define  SEPERATOR_E_SENTENCE "!?:;"
#define  SEPERATOR_E_SUB_SENTENCE ",()\042'"
#define  SEPERATOR_LINK "\n\r 　"
//Sentence begin and ending string
#define SENTENCE_BEGIN "始##始"
#define SENTENCE_END "末##末"
//Seperator between two words
#define WORD_SEGMENTER "@"


/*********************************************************************
 *
 *  Func Name  : GB2312_Generate
 *
 *  Description:  Generate the GB2312 List file
 *
 *
 *  Parameters : sFilename: the file name for the output GB2312 List
 *
 *  Returns    : bool
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2002-1-8
 *********************************************************************/
bool GB2312_Generate(char* sFileName)
{
  FILE* fp;
  unsigned int i, j;

  if ((fp = fopen(sFileName, "wt")) == NULL) {
    return false;  //fail while opening the file
  }

  for (i = 161; i < 255; i++)
    for (j = 161; j < 255; j++) {
      fprintf(fp, "%c%c,%d,%d\n", i, j, i, j);
    }

  fclose(fp);
  return true;
}
/*********************************************************************
 *
 *  Func Name  : CC_Generate
 *
 *  Description:  Generate the Chinese Char List file
 *
 *
 *  Parameters : sFilename: the file name for the output CC List
 *
 *  Returns    : bool
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2002-1-8
 *********************************************************************/
bool CC_Generate(char* sFileName)
{
  FILE* fp;
  unsigned int i, j;

  if ((fp = fopen(sFileName, "wt")) == NULL) {
    return false;  //fail while opening the file
  }

  for (i = 176; i < 255; i++)
    for (j = 161; j < 255; j++) {
      fprintf(fp, "%c%c,%d,%d\n", i, j, i, j);
    }

  fclose(fp);
  return true;
}
/*********************************************************************
 *
 *  Func Name  : CC_Find
 *
 *  Description: Find a Chinese sub-string in the Chinese String
 *
 *
 *  Parameters :  string:Null-terminated string to search
 *
 *            strCharSet:Null-terminated string to search for
 *
 *  Returns    : char *
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2002-1-8
 *********************************************************************/
char* CC_Find(const char* string, const char* strCharSet)
{
  char* cp = strstr(string, strCharSet);

  if (cp != NULL && (cp - string) % 2 == 1) {
    return NULL;
  }

  return cp;
}
/*********************************************************************
 *
 *  Func Name  : charType
 *
 *  Description: Judge the type of sChar or (sChar,sChar+1)
 *
 *
 *  Parameters : sFilename: the file name for the output CC List
 *
 *  Returns    : int : the type of char
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2002-1-8
 *********************************************************************/
int charType(unsigned char* sChar)
{
  if (*sChar < 128) {
    if (strchr("\042!,.?()[]{}+=", (int)*sChar)) {
      return CT_DELIMITER;
    }

    return CT_SINGLE;
  }
  else if (*sChar == 162) {
    return CT_INDEX;
  }
  else if (*sChar == 163 && *(sChar + 1) > 175 && *(sChar + 1) < 186) {
    return CT_NUM;
  }
  else if (*sChar == 163 && (*(sChar + 1) >= 193 && *(sChar + 1) <= 218 || *(sChar + 1) >= 225 && *(sChar + 1) <= 250)) {
    return CT_LETTER;
  }
  else if (*sChar == 161 || *sChar == 163) {
    return CT_DELIMITER;
  }
  else if (*sChar >= 176 && *sChar <= 247) {
    return CT_CHINESE;
  }
  else {
    return CT_OTHER;
  }
}
/*********************************************************************
 *
 *  Func Name  : GetCCPrefix
 *
 *  Description: Get the max Prefix string made up of Chinese Char
 *
 *
 *  Parameters : sSentence: the original sentence which includes Chinese or Non-Chinese char
 *
 *  Returns    : the end of the sub-sentence
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2002-1-8
 *********************************************************************/
unsigned int  GetCCPrefix(unsigned char* sSentence)
{
  unsigned int nLen = strlen((const char*)sSentence), nCurPos = 0;

  while (nCurPos < nLen && sSentence[nCurPos] > 175 && sSentence[nCurPos] < 248) {
    nCurPos += 2; //Get next Chinese Char
  }

  return nCurPos;
}
/*********************************************************************
 *
 *  Func Name  : IsAllSingleByte
 *
 *  Description: Judge the string is all made up of Single Byte Char
 *
 *
 *  Parameters : sSentence: the original sentence which includes Chinese or Non-Chinese char
 *
 *  Returns    : the end of the sub-sentence
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2002-1-24
 *********************************************************************/
bool IsAllChinese(unsigned char* sString)
{
  unsigned int nLen = strlen((const char*)sString), i = 0;

  while (i < nLen - 1 && sString[i] < 248 && sString[i] > 175) {
    i += 2;
  }

  if (i < nLen) {
    return false;
  }

  return true;
}
/*********************************************************************
 *
 *  Func Name  : IsAllNonChinese
 *
 *  Description: Judge the string is all made up of Single Byte Char
 *
 *
 *  Parameters : sSentence: the original sentence which includes Chinese or Non-Chinese char
 *
 *  Returns    : the end of the sub-sentence
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2002-1-24
 *********************************************************************/
bool IsAllNonChinese(unsigned char* sString)
{
  unsigned int nLen = strlen((const char*)sString), i = 0;

  while (i < nLen) {
    if (sString[i] < 248 && sString[i] > 175) {
      return false;
    }

    if (sString[i] > 128) {
      i += 2;
    }
    else {
      i += 1;
    }
  }

  return true;
}
/*********************************************************************
 *
 *  Func Name  : IsAllSingleByte
 *
 *  Description: Judge the string is all made up of Single Byte Char
 *
 *
 *  Parameters : sSentence: the original sentence which includes Chinese or Non-Chinese char
 *
 *  Returns    : the end of the sub-sentence
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2002-1-24
 *********************************************************************/
bool IsAllSingleByte(unsigned char* sString)
{
  unsigned int nLen = strlen((const char*)sString), i = 0;

  while (i < nLen && sString[i] < 128) {
    i++;
  }

  if (i < nLen) {
    return false;
  }

  return true;
}
/*********************************************************************
 *
 *  Func Name  : IsAllNum
 *
 *  Description: Judge the string is all made up of Num Char
 *
 *
 *  Parameters : sSentence: the original sentence which includes Chinese or Non-Chinese char
 *
 *  Returns    : the end of the sub-sentence
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2002-1-24
 *********************************************************************/
bool IsAllNum(unsigned char* sString)
{

  unsigned int nLen = strlen((const char*)sString), i = 0;
  char sChar[3];
  sChar[2] = 0;

  if (i < nLen) { //Get prefix such as + -
    sChar[0] = sString[i++];

    if (sChar[0] < 0) { //Get first char
      sChar[1] = sString[i++];
    }
    else {
      sChar[1] = 0;
    }

    if (!strstr("±+—-＋", sChar)) {
      i = 0;
    }
  }

  while (i < nLen - 1 && sString[i] == 163 && sString[i + 1] > 175 && sString[i + 1] < 186) {
    i += 2;
  }

  if (i < nLen) { //Get middle delimiter such as .
    sChar[0] = sString[i++];

    if (sChar[0] < 0) { //Get first char
      sChar[1] = sString[i++];
    }
    else {
      sChar[1] = 0;
    }

    if (CC_Find("∶·．／", sChar) || sChar[0] == '.' || sChar[0] == '/') {
      //98．1％
      while (i < nLen - 1 && sString[i] == 163 && sString[i + 1] > 175 && sString[i + 1] < 186) {
        i += 2;
      }
    }
    else {
      i -= strlen(sChar);
    }
  }

  if (i >= nLen) {
    return true;
  }

  while (i < nLen && sString[i] > '0' - 1 && sString[i] < '9' + 1) {
    //single byte number char
    i += 1;
  }

  if (i < nLen) { //Get middle delimiter such as .
    sChar[0] = sString[i++];

    if (sChar[0] < 0) { //Get first char
      sChar[1] = sString[i++];
    }
    else {
      sChar[1] = 0;
    }

    if (CC_Find("∶·．／", sChar) || sChar[0] == '.' || sChar[0] == '/') {
      //98．1％
      while (i < nLen && sString[i] > '0' - 1 && sString[i] < '9' + 1) {
        i += 1;
      }
    }
    else {
      i -= strlen(sChar);
    }
  }

  if (i < nLen) { //Get middle delimiter such as .
    sChar[0] = sString[i++];

    if (sChar[0] < 0) { //Get first char
      sChar[1] = sString[i++];
    }
    else {
      sChar[1] = 0;
    }

    if (!CC_Find("百千万亿佰仟％‰", sChar) && sChar[0] != '%') {
      i -= strlen(sChar);
    }
  }

  if (i >= nLen) {
    return true;
  }

  return false;
}
/*********************************************************************
 *
 *  Func Name  : IsAllIndex
 *
 *  Description: Judge the string is all made up of Index Num Char
 *
 *
 *  Parameters : sSentence: the original sentence which includes Chinese or Non-Chinese char
 *
 *  Returns    : the end of the sub-sentence
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2002-1-24
 *********************************************************************/
bool IsAllIndex(unsigned char* sString)
{
  unsigned int nLen = strlen((const char*)sString), i = 0;

  while (i < nLen - 1 && sString[i] == 162) {
    i += 2;
  }

  if (i >= nLen) {
    return true;
  }

  while (i < nLen && (sString[i] > 'A' - 1 && sString[i] < 'Z' + 1) || (sString[i] > 'a' - 1 && sString[i] < 'z' + 1)) {
    //single byte number char
    i += 1;
  }

  if (i < nLen) {
    return false;
  }

  return true;

}
/*********************************************************************
 *
 *  Func Name  : IsAllLetter
 *
 *  Description: Judge the string is all made up of Letter Char
 *
 *
 *  Parameters : sSentence: the original sentence which includes Chinese or Non-Chinese char
 *
 *  Returns    : the end of the sub-sentence
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2002-1-24
 *********************************************************************/
bool IsAllLetter(unsigned char* sString)
{
  unsigned int nLen = strlen((const char*)sString), i = 0;

  while (i < nLen - 1 && sString[i] == 163 && ((sString[i + 1] >= 193 && sString[i + 1] <= 218) || (sString[i + 1] >= 225 && sString[i + 1] <= 250))) {
    i += 2;
  }

  if (i < nLen) {
    return false;
  }

  return true;
}
/*********************************************************************
 *
 *  Func Name  : IsAllDelimiter
 *
 *  Description: Judge the string is all made up of Delimiter
 *
 *
 *  Parameters : sSentence: the original sentence which includes Chinese or Non-Chinese char
 *
 *  Returns    : the end of the sub-sentence
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2002-1-24
 *********************************************************************/
bool IsAllDelimiter(unsigned char* sString)
{
  unsigned int nLen = strlen((const char*)sString), i = 0;

  while (i < nLen - 1 && (sString[i] == 161 || sString[i] == 163)) {
    i += 2;
  }

  if (i < nLen) {
    return false;
  }

  return true;
}
/*********************************************************************
 *
 *  Func Name  : BinarySearch
 *
 *  Description: Lookup the index of nVal in the table nTable which length is nTableLen
 *
 *  Parameters : nPOS: the POS value
 *
 *  Returns    : the index value
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2002-1-25
 *********************************************************************/
int BinarySearch(int nVal, int* nTable, int nTableLen)
{
  int nStart = 0, nEnd = nTableLen - 1, nMid = (nStart + nEnd) / 2;

  while (nStart <= nEnd) { //Binary search
    if (nTable[nMid] == nVal) {
      return nMid;//find it
    }
    else if (nTable[nMid] < nVal) {
      nStart = nMid + 1;
    }
    else {
      nEnd = nMid - 1;
    }

    nMid = (nStart + nEnd) / 2;
  }

  return -1;//Can not find it;
}
/*********************************************************************
 *
 *  Func Name  : GetCharCount
 *
 *  Description: Get the count of char which is in sWord and in sCharSet
 *
 *  Parameters : sWord: the word
 *
 *  Returns    : COUNT
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2002-5-21
 *********************************************************************/
int GetCharCount(const char* sCharSet, const char* sWord)
{
  unsigned int  k = 0;
  char tchar[3];
  int nCount = 0;
  tchar[2] = 0;

  while (k < strlen(sWord)) {
    tchar[0] = sWord[k];
    tchar[1] = 0;

    if (sWord[k] < 0) {
      tchar[1] = sWord[k + 1];
      k += 1;
    }

    k += 1;

    if ((tchar[0] < 0 && CC_Find(sCharSet, tchar)) || strchr(sCharSet, tchar[0])) {
      nCount++;
    }
  }

  return nCount;
}
/*********************************************************************
 *
 *  Func Name  : GetForeignCharCount
 *
 *  Description:
 *
 *  Parameters : sWord: the word
 *
 *  Returns    : the index value
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2002-4-4
 *              2.Modify  2002-5-21
 *********************************************************************/
int GetForeignCharCount(const char* sWord)
{
  unsigned int nForeignCount, nCount;
  nForeignCount = GetCharCount(TRANS_ENGLISH, sWord); //English char counnts
  nCount = GetCharCount(TRANS_JAPANESE, sWord); //Japan char counnts

  if (nForeignCount <= nCount) {
    nForeignCount = nCount;
  }

  nCount = GetCharCount(TRANS_RUSSIAN, sWord); //Russian char counnts

  if (nForeignCount <= nCount) {
    nForeignCount = nCount;
  }

  return nForeignCount;
}
/*********************************************************************
 *
 *  Func Name  : IsForeign
 *
 *  Description: Decide whether the word is not a Non-fereign word
 *
 *  Parameters : sWord: the word
 *
 *  Returns    : the index value
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2002-1-26
 *********************************************************************/
bool IsForeign(const char* sWord)
{
  int nForeignCount = GetForeignCharCount(sWord), nCharCount = strlen(sWord);

  if (nCharCount > 2 || nForeignCount >= 1 * nCharCount / 2) {
    return true;
  }

  return false;
}
/*********************************************************************
 *
 *  Func Name  : IsAllForeign
 *
 *  Description: Decide whether the word is not a Non-fereign word
 *
 *  Parameters : sWord: the word
 *
 *  Returns    : the index value
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2002-3-25
 *********************************************************************/
bool IsAllForeign(const char* sWord)
{
  unsigned int nForeignCount = (unsigned int)GetForeignCharCount(sWord);

  if (2 * nForeignCount == strlen(sWord)) {
    return true;
  }

  return false;
}
/*********************************************************************
 *
 *  Func Name  : IsForeign
 *
 *  Description: Decide whether the word is Chinese Num word
 *
 *  Parameters : sWord: the word
 *
 *  Returns    : the index value
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2002-1-26
 *********************************************************************/
bool IsAllChineseNum(char* sWord)
{
  //百分之五点六的人早上八点十八分起床
  unsigned int  k;
  char tchar[3];
  char ChineseNum[] = "零○一二两三四五六七八九十廿百千万亿壹贰叁肆伍陆柒捌玖拾佰仟∶·．／点"; //
  char sPrefix[] = "几数第上成";

  for (k = 0; k < strlen(sWord); k += 2) {
    strncpy(tchar, sWord + k, 2) ;
    tchar[2] = '\0';

    if (strncmp(sWord + k, "分之", 4) == 0) { //百分之五
      k += 2;
      continue;
    }

    if (!CC_Find(ChineseNum, tchar) && !(k == 0 && CC_Find(sPrefix, tchar))) {
      return false;
    }
  }

  return true;
}
/*********************************************************************
 *
 *  Func Name  : GetForeignCharCount
 *
 *  Description: Return the foreign type
 *
 *  Parameters : sWord: the word
 *
 *  Returns    : the index value
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2002-4-4
 *              2.Modify  2002-5-21
 *********************************************************************/
int GetForeignType(char* sWord)
{
  unsigned int nForeignCount, nCount, nType = TT_ENGLISH;
  nForeignCount = GetCharCount(TRANS_ENGLISH, sWord); //English char counnts
  nCount = GetCharCount(TRANS_RUSSIAN, sWord); //Russian char counnts

  if (nForeignCount < nCount) {
    nForeignCount = nCount;
    nType = TT_RUSSIAN;
  }

  nCount = GetCharCount(TRANS_JAPANESE, sWord); //Japan char counnts

  if (nForeignCount < nCount) {
    nForeignCount = nCount;
    nType = TT_JAPANESE;
  }

  return nType;
}
bool PostfixSplit(char* sWord, char* sWordRet, char* sPostfix)
{
  char sSinglePostfix[] = POSTFIX_SINGLE;
  char sMultiPostfix[][9] = POSTFIX_MUTIPLE;
  unsigned int nPostfixLen = 0, nWordLen = strlen(sWord);
  int i = 0;

  while (sMultiPostfix[i][0] != 0 && strncmp(sWord + nWordLen - strlen(sMultiPostfix[i]), sMultiPostfix[i], strlen(sMultiPostfix[i])) != 0) {
    //Try to get the postfix of an address
    i++;
  }

  strcpy(sPostfix, sMultiPostfix[i]);
  nPostfixLen = strlen(sMultiPostfix[i]); //Get the length of place postfix

  if (nPostfixLen == 0) {
    sPostfix[2] = 0;
    strncpy(sPostfix, sWord + nWordLen - 2, 2);

    if (CC_Find(sSinglePostfix, sPostfix)) {
      nPostfixLen = 2;
    }
  }

  strncpy(sWordRet, sWord, nWordLen - nPostfixLen);
  sWordRet[nWordLen - nPostfixLen] = 0; //Get the place name which have erasing the postfix
  sPostfix[nPostfixLen] = 0;
  return true;
}

#endif // !defined(AFX_CHINESE_UTILITY_H__B6D7EA03_7BCD_46AD_B38C_D8033ACD5813__INCLUDED_)
