
# 一个Nuance 的语音识别的例子 -  芝麻软件工作室的专栏 - CSDN博客


2012年08月09日 10:42:51[seven-soft](https://me.csdn.net/softn)阅读数：1956


\#include "stdafx.h"
\#include "GetToken.h"

\#ifdef _DEBUG
\#define new DEBUG_NEW
\#undef THIS_FILE
staticcharTHIS_FILE[] = __FILE__;
\#endif
/////////////////////////////////////////////////////////////////////////////
// The one and only application object
//CWinApp theApp;
//using namespace std;
\#ifdef __UNICODE__
\#undef __UNICODE__
\#endif
//\#define __UNICODE__
\#ifdef __UNICODE__
\#define dictName "dict.uni"
\#define alpha "freepy.uni"
\#else//gb2312
\#define dictName "dict.gb"
\#define alpha "freepy.gb"
\#endif
constintmaxlength = 256;
\#define dictSize 0x10000
constcharpyIndex[]  ="pyindex";

BOOLISHanzi(char*src)
{
intasc=*src;
if(asc<0)  asc+=256;
if(asc>127)
returnTRUE;
returnFALSE;
}
char* GetPhoneticByWideString(WCHAR* str,constWORD* dict,constchar* pyTable)
{
charphonetic[10];
char* widestr = (char*)str;
char*returnvalue=NULL;
BOOLhz;
hz=FALSE;
while(*widestr != 0)
{
memset(phonetic,0,10);
boolbEnglish =true;
if(*widestr <='Z'&& *widestr >='A')
{
if(hz)
{
*phonetic=' ';
*(phonetic+1) = *widestr +'a'-'A';
*(phonetic+2) = NULL;
}else
{
*phonetic = *widestr +'a'-'A';
*(phonetic+1) = NULL;
}
hz=FALSE;
widestr ++;
}
elseif(*widestr <='z'&& *widestr >='a'|| *widestr ==' ')
{
if(hz&&*widestr !=' ')
{
*(phonetic)=' ';
*(phonetic+1) = *widestr;
*(phonetic+2) = NULL;
}
else
{
*phonetic = *widestr;
*(phonetic+1) = NULL;
}
hz=FALSE;
widestr ++;
}
elseif(*widestr >='0'&& *widestr <='9')
{
/*          *phonetic = *widestr;
*(phonetic+1) = NULL;
hz=FALSE;
widestr ++;
*/
GetDigits(*widestr,phonetic);
hz=TRUE;
bEnglish =false;
widestr++;
}
elseif(ISHanzi(widestr))
{
if(-1 == GetPhoneticByHanZi(*(WCHAR*)widestr,dict,pyTable,phonetic))
returnNULL;
widestr ++;
if(!((*widestr <='Z'&& *widestr >='A') || (*widestr <='a'&& *widestr >='a') || *widestr ==' '))
widestr ++;
hz=TRUE;
bEnglish =false;
}else
{
widestr ++;
continue;
}
if(!returnvalue)
{
inti = strlen(phonetic);
returnvalue = strdup(phonetic);
}
else
{
if(!bEnglish && returnvalue[strlen(returnvalue)-1] !=' ')
{
returnvalue  = (char*)realloc(returnvalue,strlen(returnvalue)+strlen(phonetic)+2);
strcat(returnvalue," ");
}
else
{
returnvalue  = (char*)realloc(returnvalue,strlen(returnvalue)+strlen(phonetic)+1);
}
strcat(returnvalue,phonetic);
}
}
returnreturnvalue;
}
voidLoadDictAndPYTable(WORD* pDict,char** pPyTable)
{
CFile dictFile(dictName,CFile::modeRead|CFile::typeBinary);
//for (int i = 0;i<DICTSIZE;I++){ (a if a="wch" (m<lstr){ while tabfile='fopen("table.txt","r+b");' tabfile; * FILE destfile='fopen("big.txt","w+b");' destfile; BigSource="Big5Source;" char* GBString="outString;" 输出字符串 char[lstr]; outString="new" lstr="bigStrlen+1;" m="0;" long lstr; b; char unsigned a; Big5Source="new" else } NULL); NULL, bigStrlen, Big5Source, -1, WidecharStr, 0, WideCharToMultiByte(936, MultiByteToWideChar(950,0,SourceText,bigStrlen,WidecharStr,bigStrlen); { (strstr(SourceText,?big5?)!="NULL)" bigStrlen+1); memset(Big5Source, (bigStrlen+1)*sizeof(WCHAR)); memset(WidecharStr, String WideChar WCHAR[bigStrlen+1]; WidecharStr="new" WCHAR Big5字符串 char[bigStrlen+1]; LPSTR 源字符串长度 bigStrlen="strlen(SourceText);" *SourceText) Big2GB(char ?);} strcpy(returnvalue,?ling2 (digits="='1')" strcpy(returnvalue,?jiu3 strcpy(returnvalue,?ba1 strcpy(returnvalue,?qi1 strcpy(returnvalue,?liu4 strcpy(returnvalue,?wu3 strcpy(returnvalue,?si4 strcpy(returnvalue,?san1 strcpy(returnvalue,?er4 strcpy(returnvalue,?yi1 returnvalue) digits,char* GetDigits(char void count; return count++; *(lpStrArr+count*wMaxArrSize+i-cursor+1)="_T('/0');" _tcsncpy(lpStrArr+count*wMaxArrSize,lpBuf+cursor,i-cursor+1); ){ ?) *(lpBuf+i)!="_T('" && wBufLen-1 if(i="=" cursor="0,count=0,wBufLen;" *(lpStrArr+count*wMaxArrSize+i-cursor)="_T('/0');" _tcsncpy(lpStrArr+count*wMaxArrSize,lpBuf+cursor,i-cursor); *(lpBuf+i-1)!="_T('" if(i!="0" _T(?/t?)) *(lpBuf+i)="=" || _T(? if(*(lpBuf+i)="=" (i="0;i<wBufLen;i++){" for wBufLen="strlen(lpBuf);" WORD i; int wMaxArrSize) lpStrArr,WORD lpBuf,LPTSTR String2Array(LPTSTR 1; phonetic[len+1]="0;" phonetic[len]="shengdiao+0x30;" len="strlen(phonetic);" strcpy(phonetic,pyTable+index-shengdiao); 8; % shengdiao="index" -1; (!index) ); !="NULL" pyTable ASSERT(phonetic!="NULL" phonetic) *pyTable,char* index,const GetPYByIndex(int 返回只为* *根据索引值获得汉字的发音,为上函数的逆函数 find/n?); printf(?not break; 415) (count++="=" +="8;" index="(char*)" while(strcmp(index,phonetic)!="0)" count="0;" pyTable; pyTable) void* phonetic,const GetIndexByPY(char* 顺序查找，可以修改为这般查找* +音调 返回为索引植="音标序号*8" pyTable为存储音标的表， *phonetic为要查的音标,如ang， GetPYByIndex(dict[a],pyTable,phonetic); ; short phonetic pyTable,char* dict,const wch,const GetPhoneticByHanZi(WCHAR +音调phonetic* 返回为该汉字的拼音代码="音标序号*8" pyTable为字典数组的起始地址， *从一个汉字取得数组下标 pyFile.Read(*pPyTable,4096); pyFile(pyIndex,CFile::modeRead|CFile::typeBinary); CFile *pPyTable="(char*)malloc(4096);" (!*pPyTable) TRACE(dictName); dictFile.ReadHuge(pDict,dictSize*2);>=0x81 && a <=0xfe)
{
b=*(Big5Source+1);
if(b<0x7e) b=b - 0x40;
if(b>=0x80) b = b - 0x80 + 0x7e - 0x40 + 1;
fseek(tabfile, 2 * ((a -0x81) * (0xfe - 0x80 + 1 + 0x7e - 0x40 + 1 ) + b), SEEK_SET);
fread(& a,1,1,tabfile);
fread(& b,1,1,tabfile);
fwrite(&a,1,1,destfile);
fwrite(&b,1,1,destfile);
*outString=a;
*(outString+1)=b;
outString+=2;
Big5Source+=2;
m+=2;
}
else
{
fwrite(&a,1,1,destfile);
*outString=a;
outString++;
Big5Source++;
m++;
}
}
fclose(destfile);
fclose(tabfile);
Big5Source=BigSource;
outString[lstr-1]='/0';
delete[] Big5Source;
delete[] WidecharStr;
returnGBString;
}
\#include "stdafx.h"
\#include "GrammarThread.h"
\#include "GetToken.h"
\#include "HandleDataThread.h"
\#include "process.h"
\#ifdef _DEBUG
\#define new DEBUG_NEW
\#undef THIS_FILE
staticcharTHIS_FILE[] = __FILE__;
\#endif
\#define MAX_STRING_PARAM_LEN 300
/////////////////////////////////////////////////////////////////////////////
// CGrammarThread
usingnamespacestd;
externWORDdictArray[0X10000];
externchar* pyTable;
voidShowMsg(CString msg);

CGrammarThread::CGrammarThread()
{
}
CGrammarThread::CGrammarThread(NuanceConfig &config,
DispatcherInterface & d,
DBDescriptor * db_desc,CHandleDataThread* _p):   m_pConfig(&config), m_pDispatcher(&d), m_pRcEngine(NULL),// m_kb(NULL),
m_pDynagram_Db_Desc(db_desc)
{
m_parent=_p;
NuanceStatus   status;
for(inti=0;i<MAX_SLOT_CNT;I++) m_pRcEngine- { CGrammarThread::~CGrammarThread() } -------- NotifiableAdapter from inherited WarnForUnhandledNotifications(); m_uRecord_ID="0;"m_uRecognition_ID="0;"m_specdtmf="0;"; m_termno="\#"m_iddtime="0;"m_firsttime="0;"m_dtmflen="40;"m_dtmfreason="0;"m_dtmfcnt="0;"memset(m_dtmfbuf,0,200); m_uPlay_Prompt_ID="0;"m_playreason="0;"h_recordevent="CreateEvent(NULL,FALSE,TRUE,NULL);"h_parameterevent="CreateEvent(NULL,FALSE,TRUE,NULL);"h_recognevent="CreateEvent(NULL,FALSE,TRUE,NULL);"h_dtmfevent="CreateEvent(NULL,FALSE,TRUE,NULL);"h_playevent="CreateEvent(NULL,FALSE,TRUE,NULL);"h_insertevent="CreateEvent(NULL,FALSE,FALSE,NULL);"h_closedbevent="CreateEvent(NULL,FALSE,FALSE,NULL);"h_delevent="CreateEvent(NULL,FALSE,FALSE,NULL);"h_queryevent="CreateEvent(NULL,FALSE,FALSE,NULL);"h_newevent="CreateEvent(NULL,FALSE,FALSE,NULL);"return; !="status)"(NUANCE_OKifstatus="NLGetSlotNamesFromConfig(m_pConfig,slotname);"m_pRcEngine) (NULL="="status); *this, *m_pDispatcher, RCEngine(m_pConfig, m_pRcEngine="new"NLFreeValue(slotname); NLFreeValue(slotstr); strcpy(slotnamelist[i],slot); slotnamelist[i]="NULL;"NLGetStringFromValue(slotstr,slot,200);for(i="0;i<slen;i++)"slotstr="NLNewValue(&status);"NLValue* slotname="NLNewValue(&status);"slot[200];charslen="0;"int>Abort(NullID);
if(m_pDynagram_Db_Desc) {
DeleteDBDescriptor(m_pDynagram_Db_Desc);
}
m_pDynagram_Db_Desc = NULL;
deletem_pRcEngine;
m_pRcEngine = NULL;
CloseHandle(h_newevent);
CloseHandle(h_queryevent);
CloseHandle(h_delevent);
CloseHandle(h_closedbevent);
CloseHandle(h_insertevent);
CloseHandle(h_playevent);
CloseHandle(h_dtmfevent);
CloseHandle(h_recognevent);
CloseHandle(h_parameterevent);
CloseHandle(h_recordevent);
for(inti=0;slotnamelist[i]!=NULL;i++)
{
free(slotnamelist[i]);
}
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleCallTransferFailed(CallTransferFailedNotificationconst& n)
{
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleNuanceDBRecordDeleted(NuanceDBRecordDeletedNotificationconst& n)
{
NuanceStatus status = n.GetStatus();
if(NUANCE_OK != status) {
}else
{
CString msg;
msg="记录删除";
ShowMsg(msg);
}
SetEvent(h_delevent);
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleNuanceDBRecordModified(NuanceDBRecordModifiedNotificationconst& n)
{
CString msg;
NuanceStatus status = n.GetStatus();
if(NUANCE_OK != status) {
msg.Format("Dynamic Grammar Modified 失败:%s",n.GetErrorString());
ShowMsg(msg);
}else
{
msg="Dynamic Grammar Modified";
ShowMsg(msg);
}
SetEvent(h_newevent);
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleNuanceDBRecordCreated(NuanceDBRecordCreatedNotificationconst& n)
{
CString msg;
NuanceStatus status = n.GetStatus();
if(NUANCE_OK != status) {
msg.Format("Dynamic Grammar Create 失败:%s",n.GetErrorString());
ShowMsg(msg);
}else
{
msg="Dynamic Grammar Create";
ShowMsg(msg);
}
SetEvent(h_newevent);
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleConferenceCallStarted(ConferenceCallStartedNotificationconst& n)
{
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleConferenceEnded(ConferenceEndedNotificationconst& n)
{
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleDTMFTones(DTMFTonesNotificationconst& dn)
{
charconst*m_cNum = dn.GetTones();
printf("have gotten dtmf :%c/r/n",*m_cNum);
if(m_uRecord_ID>0)
{
if(m_termdtmf!='a'&&m_termdtmf!='A')
{
if(*m_cNum==m_termdtmf)
StopRecord();
}else
{
StopRecord();
}
}
if(m_dtmfstate==1)
{
if(m_uRecognition_ID>0)
StopRecognize(8);
StopPrompt();
if(*m_cNum==m_termno)
{
m_dtmfreason=1;
m_dtmfstate=0;
return;
}
memcpy(m_dtmfbuf+m_dtmfcnt,m_cNum,strlen(m_cNum));
m_dtmfcnt+=strlen(m_cNum);
time(&m_dt);
if(m_dtmfcnt==1&&*m_cNum==m_specdtmf)
{
m_dtmfreason=1;
m_dtmfstate=0;
}elseif(m_dtmfcnt==m_dtmflen)
{
m_dtmfreason=1;
m_dtmfstate=0;
}
}
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleAcknowledgment (AcknowledgmentNotificationconst& an)
{
CString strfname;
strfname.Format("%s",an.GetRequestTypeAsString(an.GetRequestType()));
NuanceStatus status = an.GetStatus();
if(NUANCE_OK != status) {
switch(an.GetRequestType())
{
caseAcknowledgmentNotification::RequestType::StartRecording:
m_recordreason=2;
m_uRecord_ID=0;
SetEvent(h_recordevent);
break;
caseAcknowledgmentNotification::RequestType::RecognizeUtterance:
m_uRecognition_ID=0;
SetEvent(h_recognevent);
break;
caseAcknowledgmentNotification::RequestType::SetParameter:
SetEvent(h_parameterevent);
break;
caseAcknowledgmentNotification::RequestType::PlayPrompts:
if(m_dtmfstate==1) time(&m_ft);
m_uPlay_Prompt_ID=0;
m_parent->RemoveAllWav();
ShowMsg("***playback Stop for error");
SetEvent(h_playevent);
break;
caseAcknowledgmentNotification::RequestType::DeleteRecord:
SetEvent(h_delevent);
break;
caseAcknowledgmentNotification::RequestType::CloseDatabase:
SetEvent(h_closedbevent);
break;
caseAcknowledgmentNotification::RequestType::NewDynamicGrammar:
caseAcknowledgmentNotification::RequestType::NewDynamicGrammarFromGSL:
caseAcknowledgmentNotification::RequestType::NewDynamicGrammarFromPhraseList:
caseAcknowledgmentNotification::RequestType::AddPhraseListToDynamicGrammar:
caseAcknowledgmentNotification::RequestType::AddPhraseToDynamicGrammar:
caseAcknowledgmentNotification::RequestType::RemovePhraseFromDynamicGrammar:
SetEvent(h_newevent);
break;
caseAcknowledgmentNotification::RequestType::InsertDynamicGrammar:
SetEvent(h_insertevent);
break;
caseAcknowledgmentNotification::RequestType::QueryDynamicGrammarContents:
caseAcknowledgmentNotification::RequestType::QueryRecordExists:
SetEvent(h_queryevent);
break;
}
}
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleInitializationCompleted (InitializationCompletedNotificationconst& icn)
{
StateDynagramOpenDB();
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandlePlaybackStopped (PlaybackStoppedNotificationconst& pcn)
{
PlaybackStoppedNotification::Reason reason = pcn.GetReason();
switch(reason)
{
//完全播放结束
casePlaybackStoppedNotification::PLAYBACK_COMPLETED:
{
m_playreason=1;
}
break;
//若是调用了 Abort 函数 或 挂断电话
casePlaybackStoppedNotification::PLAYBACK_ABORTED:
{
m_playreason=2;
}
break;
casePlaybackStoppedNotification::HANG_UP:
{
m_playreason=3;
}
break;
default:
{
m_playreason=4;
}
}//end of case
if(m_dtmfstate==1) time(&m_ft);
m_uPlay_Prompt_ID=0;
m_parent->RemoveAllWav();
ShowMsg("***playback Stop");
SetEvent(h_playevent);
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleRecordingStopped (RecordingStoppedNotificationconst& rsn)
{
m_uRecord_ID=0;
SetEvent(h_recordevent);
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleStartOfSpeech (StartOfSpeechNotificationconst& sosn)
{
printf("^^^^^^^^^HandleStartOfSpeech/r/n");
if(m_dtmfstate==1)  StopDtmf(8);
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleEndOfSpeech (EndOfSpeechNotificationconst& eosn)
{
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandlePartialResult (PartialResultNotificationconst& prn)
{
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleRecognitionStopped (RecognitionStoppedNotificationconst& rsn)
{
//取的识别 ID
unsigned id = rsn.GetUtteranceID();
//判断识别结束原因
intiReason = rsn.GetReason();
switch(iReason)
{
//识别顺利完成
caseRecognitionStoppedNotification::COMPLETED:
{
RecResultconst* rr = rsn.GetRecResult();
RecognizeNotify((RecResult*)rr);
}
break;
//挂断电话
caseRecognitionStoppedNotification::HANG_UP:
{
}
break;
//调用 Abort
caseRecognitionStoppedNotification::ABORTED:
{
ShowMsg("00000000000000000");
}
break;
default:
{
ShowMsg("111111111111111111111");
}
}
ShowMsg("Recognize Stop");
m_uRecognition_ID=0;
SetEvent(h_recognevent);
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleIncomingCall (IncomingCallNotificationconst& icn)
{
ShowMsg("Incoming call--------------");
strcpy(m_parent->m_callingID,icn.GetCallerID());
strcpy(m_parent->m_calledID,icn.GetDID());
unsigned id = icn.GetCallID();
m_uAnswer_Call_ID=id;
m_pRcEngine->AnswerCall(m_uAnswer_Call_ID);
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleOutgoingCallConnected (OutgoingCallConnectedNotificationconst& occn)
{
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleCallAnswered (CallAnsweredNotificationconst& can)
{
ShowMsg("Call Answered--------------");
m_parent->m_callstate=1;
m_parent->PostThreadMessage(WM_CALL_CONNECT,0,0);
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleCallEnded (CallEndedNotificationconst& cen)
{
ShowMsg("Call Ended--------------");
m_parent->m_callstate=2;
StopAll();
m_parent->PostThreadMessage(WM_CALL_DISCONNECT,0,0);
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleNuanceDBOpened (NuanceDBOpenedNotificationconst& ndbon)
{
// check status
NuanceStatus status = ndbon.GetStatus();
if(NUANCE_OK != status) {
CString msg="OpenDB 失败";
ShowMsg(msg);
m_pRcEngine->Abort(NullID);
return;
}
unsigned id = ndbon.GetID();
if(m_uDynagram_Db_ID != id) {
CString msg="OpenDB ID 失败";
ShowMsg(msg);
m_pRcEngine->Abort(NullID);
return;
}
CString msg="Open DB 成功";
ShowMsg(msg);
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleNuanceDBClosed (NuanceDBClosedNotificationconst& ndbcn)
{
NuanceStatus status = ndbcn.GetStatus();
if(NUANCE_OK != status) {
CString msg="CloseDB 失败";
ShowMsg(msg);
}else
{
CString msg="CloseDB 成功";
ShowMsg(msg);
}
SetEvent(h_closedbevent);
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleNuanceDBRecordExists (NuanceDBRecordExistsNotificationconst& ndben)
{
NuanceStatus status = ndben.GetStatus();
if(NUANCE_OK != status) {
CString msg="Grammar 记录不存在";
ShowMsg(msg);
r_exist=false;
}else
{
CString msg="Grammar 记录存在";
ShowMsg(msg);
r_exist=true;
}
SetEvent(h_queryevent);
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleDynamicGrammarInserted (DynamicGrammarInsertedNotificationconst& dgin)
{
NuanceStatus status = dgin.GetStatus();
if(NUANCE_OK != status) {
ShowMsg("Grammar insert 失败");
}else
{
ShowMsg("Grammar insert 成功");
}
SetEvent(h_insertevent);
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleParameterSet (ParameterSetNotificationconst& psn)
{
SetEvent(h_parameterevent);
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleParameterGotten (ParameterGottenNotificationconst& pgn)
{
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleException (ExceptionNotificationconst& en)
{
}
void
CGrammarThread::
StateDynagramOpenDB (void)
{
m_uDynagram_Db_ID = m_pRcEngine->GetUniqueID();
m_pRcEngine->OpenDynamicGrammarDatabase(m_pDynagram_Db_Desc, m_uDynagram_Db_ID);
}

intCGrammarThread::NewDynaGrammar(char*key,char*text,booloverwrite)
{
boolt_ret=false;
PhraseList* m_pPhrase_List;
m_pPhrase_List=NewPhraseList();
char* token = strtok(text,"|");
while( token != NULL )
{
CString tmpstr;
tmpstr.Format("%s",token);
boolret=CreatePhraseList(m_pPhrase_List,(char*)(LPCTSTR)tmpstr);
if(ret) t_ret=ret;
token = strtok( NULL,"|");
}
if(!t_ret)
{
SetEvent(h_newevent);
DeletePhraseList(m_pPhrase_List);
return1;
}
unsigned m_uDynagram_New_ID = m_pRcEngine->GetUniqueID();
if(overwrite)
{
ResetEvent(h_newevent);
m_pRcEngine->NewDynamicGrammar(m_uDynagram_Db_ID,key,
m_uDynagram_New_ID,m_pPhrase_List,NULL,1);
}else
{
if(QueryRecord(key))
{
ResetEvent(h_newevent);
m_pRcEngine->AddPhraseListToDynamicGrammar(m_uDynagram_Db_ID,key,
m_uDynagram_New_ID,m_pPhrase_List,NULL);
}
else
{
ResetEvent(h_newevent);
m_pRcEngine->NewDynamicGrammar(m_uDynagram_Db_ID,key,
m_uDynagram_New_ID,m_pPhrase_List,NULL,1);
}
}
DeletePhraseList(m_pPhrase_List);
WaitForSingleObject(h_newevent,INFINITE);
return0;
}
CString ParseGrammar(CString src)
{
CString result="";
CString tmp="";
charch;
boolhaso=false;
for(inti=0;i<SRC.GETLENGTH();I++) { } ; slotname=""charintif(i="=0)"break;if(!mrun) i++; slotname.TrimRight(); slotname.TrimLeft(); ntext.TrimRight(); ntext.TrimLeft(); ntext=""ptext.TrimRight(); ptext.TrimLeft(); ptext=""stext.TrimRight(); stext.TrimLeft(); stext=""pos1='srctext.Find("*",0);'pos2="0;"ttext=""}elsemrun="true;"if(pos1<0) CStringwhile(1) i="0;"boolsrctext.Format(?%s?,text); srctext; status; NuanceStatus *text) *p_list, CGrammarThread::CreatePhraseList(PhraseList result;return?); result.TrimRight(? tmp=""free(phFood); result+="tmp;"?,phFood); tmp.Format(?%sif(phFood) pyTable); *phFood="GetPhoneticByWideString((WCHAR*)(char*)(LPCTSTR)tmp,dictArray,"tmp.Format(??(%s)if(haso) )if(tmp!=""tmp+="ch;"haso="true;"if(ch="='[')"ch="src.GetAt(i);">3)break;
}
if(stext=="")returnfalse;
boolsrcz=false;
if(ntext.Left(2)=="c:"||ntext.Left(2)=="C:")
{
ntext=ntext.Right(ntext.GetLength()-2);
srcz=true;
}
if(ntext=="")
{
char*phFood =GetPhoneticByWideString((WCHAR*)(char*)(LPCTSTR)stext,dictArray,
pyTable);
if(!phFood)
{
returnfalse;
}
ntext.Format("%s",phFood);
free(phFood);
}else
{
if(srcz)
{
ntext=ParseGrammar(ntext);
srcz=false;
}
}
if(ptext=="")
{
ptext=ntext;
ptext.Replace(" ","");
}
CString ctmp=ntext;
ntext="("+ctmp+")";
ctmp=stext;
if(slotname=="")
{
slotname="peoplename";
}
stext="{<"+ slotname+" /""+ctmp+"/">}";
status = PhraseListAdd(
p_list,// phrase list pointer
ptext,// used to reference phrase in db
ntext,// actual text to pronounce
stext,// nl entry
1.0// probability
);
if(status == NUANCE_OK)
returntrue;
returnfalse;
}

boolCGrammarThread::QueryRecord(char*key)
{
ResetEvent(h_queryevent);
r_exist=false;
unsigned m_uDynagram_Query_ID = m_pRcEngine->GetUniqueID();
m_pRcEngine->QueryRecordExists(m_uDynagram_Db_ID,key,m_uDynagram_Query_ID);
WaitForSingleObject(h_queryevent,INFINITE);
returnr_exist;
}
voidCGrammarThread::RemovePhrase(char*key,char*phraseid)
{
char* token = strtok(phraseid,"|");
while( token != NULL )
{
if(QueryDGrmContents(key,token))
{
ResetEvent(h_newevent);
unsigned m_uDynagram_Remove_ID = m_pRcEngine->GetUniqueID();
m_pRcEngine->RemovePhraseFromDynamicGrammar(m_uDynagram_Db_ID,key,m_uDynagram_Remove_ID,token);
WaitForSingleObject(h_newevent,INFINITE);
}
token = strtok( NULL,"|");
}
}
boolCGrammarThread::QueryDGrmContents(char*key,char*phraseid)
{
ResetEvent(h_queryevent);
r_exist=false;
unsigned m_uDynagram_Query_ID = m_pRcEngine->GetUniqueID();
m_pRcEngine->QueryDynamicGrammarContents(m_uDynagram_Db_ID,key,m_uDynagram_Query_ID,phraseid);
WaitForSingleObject(h_queryevent,INFINITE);
returnr_exist;
}
voidNUANCE_MEMBER_FUNCTION
CGrammarThread::HandleNuanceDBRecordContents(NuanceDBRecordContentsNotificationconst& n)
{
NuanceStatus status = n.GetStatus();
if(NUANCE_OK != status) {
ShowMsg("Grammar中内容不存在");
r_exist=false;
}else
{
char* str=(char*)n.GetPhraseString();
if(strlen(str)>0)
{
//--避免CString 的长度限制,所以没用ShowMsg
cout<<"Grammar中内容存在:"<<ENDL; { } }elsem_uDynagram_Del_ID="m_pRcEngine-"unsigned ResetEvent(h_delevent); *key) CGrammarThread::DeleteGrm(charvoidSetEvent(h_queryevent); r_exist="true;"ShowMsg(?Grammar中内容不存在?); cout<<str<<endl;>GetUniqueID();
m_pRcEngine->DeleteRecord(m_uDynagram_Db_ID,key,m_uDynagram_Del_ID);
WaitForSingleObject(h_delevent,INFINITE);
}
voidCGrammarThread::CloseDB()
{
ResetEvent(h_closedbevent);
m_pRcEngine->CloseDatabase(m_uDynagram_Db_ID);
WaitForSingleObject(h_closedbevent,INFINITE);
}
voidCGrammarThread::InsertGrammer(char*key,char*lable,booltp)
{
ResetEvent(h_insertevent);
unsigned m_uDynagram_Insert_ID = m_pRcEngine->GetUniqueID();
m_pRcEngine->InsertDynamicGrammar(m_uDynagram_Db_ID,key,m_uDynagram_Insert_ID,lable,((tp)?DURATION_CALL:DURATION_PERMANENT));
WaitForSingleObject(h_insertevent,INFINITE);
}
BYTECGrammarThread::StatePrompt(bool_wait)
{
/*for(int i=0;i<M_PARENT->m_currentprompt;i++)
{   ShowMsg("******");
ShowMsg(m_parent->m_playlist[i]);
}
*/
if(m_parent->m_currentprompt<=0)
{
if(m_dtmfstate==1) time(&m_ft);
SetEvent(h_playevent);
return1;
}
m_playreason=0;
ResetEvent(h_playevent);
m_uPlay_Prompt_ID= m_pRcEngine->GetUniqueID();
m_pRcEngine->PlayPrompts(m_parent->m_playlist,m_uPlay_Prompt_ID);
if(_wait) WaitForSingleObject(h_playevent,INFINITE);
returnm_playreason;
}
voidDtmfThread(LPVOIDlp)
{
CGrammarThread* p=(CGrammarThread*)lp;
while(p->m_dtmfstate)
{
if(p->m_firsttime>0&&p->m_dtmfcnt==0&&p->m_uPlay_Prompt_ID==0)
{
time_tnt;
time(&nt);
if(nt-p->m_ft>=p->m_firsttime)
{
p->m_dtmfreason=1;
p->m_dtmfstate=0;
break;
}
}
if(p->m_iddtime>0&&p->m_dtmfcnt>0)
{
time_tnt;
time(&nt);
if(nt-p->m_dt>=p->m_iddtime)
{
p->m_dtmfreason=1;
p->m_dtmfstate=0;
break;
}
}
Sleep(1);
}
p->StopPrompt();
SetEvent(p->h_dtmfevent);
}
BYTECGrammarThread::RunDtmf(intdtmflen,intfirsttime,intiddtime,chartermno,charspecdtmf)
{
ResetEvent(h_dtmfevent);
memset(m_dtmfbuf,0,40);
m_dtmfcnt=0;
m_dtmfreason=0;
m_dtmflen=dtmflen;
m_firsttime=firsttime;
m_iddtime=iddtime;
m_termno=termno;
m_specdtmf=specdtmf;
time(&m_dt);
m_dtmfstate=1;
StatePrompt(false);
_beginthread(DtmfThread,0,this);
WaitForSingleObject(h_dtmfevent,INFINITE);
WaitForSingleObject(h_playevent,INFINITE);
returnm_dtmfreason;
}
voidCGrammarThread::StopDtmf(BYTE_r)
{
m_dtmfreason=_r;
m_dtmfstate=0;
}
voidCGrammarThread::StopPrompt()
{
if(m_uPlay_Prompt_ID>0)
{
m_pRcEngine->AbortPlayback(m_uPlay_Prompt_ID);
}else
{
SetEvent(h_playevent);
}
}
BYTECGrammarThread::RecognizeUttr(boolhotword,WORDnbest,WORDnospeechtimeout,
WORDtoomuchms,WORDtooslowms,WORDrejthreshold,
char* resultbuf,char*pText,char*_grammar,
bool_overwrite,char*key,intdtmflen,
intiddtime,chartermno,charspecdtmf)
{
BYTEret=0;
m_resultbuf=resultbuf;
intm_nbest=nbest;
doublem_nospeechtimeout=nospeechtimeout*1.00;
intm_toomuchms=toomuchms;
intm_tooslowms=tooslowms;
intm_rejthreshold=rejthreshold;
SetParameters("rec.NumNBest",&m_nbest,0);
SetParameters("client.NoSpeechTimeoutSecs",&m_nospeechtimeout,1);
SetParameters("rec.ConfidenceRejectionThreshold",&m_rejthreshold,0);
charglable[150];
charf_grammar[150];
chargkey[150];
strcpy(glable,"DynaPeople0");
if(key) strcpy(gkey,key);
elsesprintf(gkey,"nasr%d",m_parent->m_index);
if(_grammar) strcpy(f_grammar,_grammar);
elsestrcpy(f_grammar,".PEOPLE0");

if(!_grammar)
{
if(strlen(pText)>0)
{
intret=NewDynaGrammar(gkey,pText,_overwrite);
if(ret!=0)
{
return2;
}
}
InsertGrammer(gkey,glable);
}
ResetEvent(h_recognevent);
m_uRecognition_ID=m_pRcEngine->GetUniqueID();
m_pRcEngine->RecognizeUtterance(f_grammar,m_uRecognition_ID);
if(dtmflen<=0)
{
StatePrompt(false);
WaitForSingleObject(h_playevent,INFINITE);
WaitForSingleObject(h_recognevent,INFINITE);
}
else
{
ret=RunDtmf(dtmflen, 0,iddtime, termno, specdtmf);
WaitForSingleObject(h_recognevent,INFINITE);
if(ret!=8)
{
strcpy(resultbuf,m_dtmfbuf);
}
}
return1;
}
voidCGrammarThread::SetParameters(char*pname,void*value,BYTEvtype)
{
ResetEvent(h_parameterevent);
m_uParameter_ID=m_pRcEngine->GetUniqueID();
switch(vtype)
{
case0://INT
m_pRcEngine->SetParameter(pname,*((int*)value),m_uParameter_ID);
break;
case1://double
m_pRcEngine->SetParameter(pname,*((double*)value),m_uParameter_ID);
break;
case2://char*
m_pRcEngine->SetParameter(pname,*((char**)value),m_uParameter_ID);
break;
}
WaitForSingleObject(h_parameterevent,INFINITE);
}

HRESULTCGrammarThread::RecognizeNotify(RecResult *rr)
{
//判断结果是否合法，记录在  rec_result_type 中
//jb 2001.12.07
NuanceStatus   status;
RecResultType  rec_result_type;
status = RecResultGetType(rr,&rec_result_type);
if(NUANCE_OK != status)
{
strcpy(m_resultbuf,"error");
return0;
}
//若是 不说话超时，则重播刚才的 Prompt
if(rec_result_type == NO_SPEECH_TIMEOUT)
{
m_resultbuf[0]=0;
return0;
}
//若识别不成功
if(rec_result_type != RECOGNITION)
{
strcpy(m_resultbuf,"nomatch");
return0;
}

// 得到结果的个数
intnum_results;
status = RecResultNumAnswers(rr, &num_results);
// nlresult for natural language interpretion
m_resultbuf[0]=0;
// 取得每一个结果
for(intindex = 0; index < num_results; ++index)
{
NLResult *            m_pNl_Result=NULL;
m_pNl_Result = NLInitializeResult(&status);
charresult_buf[MAX_STRING_PARAM_LEN];
intconfidence_score;//识别结果的确定性分数
// 得到结果的 string 型
status = RecResultString(rr, index, result_buf,sizeof(result_buf));
if(NUANCE_OK != status)
{
strcpy(m_resultbuf,"error");
return0;
}
// 得到结果的 Confidence
status = RecResultOverallConfidence(rr, index, &confidence_score);
if(NUANCE_OK != status)
{
strcpy(m_resultbuf,"error");
return0;
}

status = RecResultNLResult(rr, index, m_pNl_Result);
// 得到 NL Result 的个数
intnum_nl_results =
NLGetNumberOfInterpretations(m_pNl_Result, &status);
if(NUANCE_OK != status)
{
strcpy(m_resultbuf,"error");
return0;
}
for(intnl_index = 0; nl_index < num_nl_results; nl_index++)
{
//激活当前的 NL Result
status = NLMakeIthInterpretationActive(m_pNl_Result,
nl_index);
if(NUANCE_OK != status)
{
strcpy(m_resultbuf,"error");
return0;
}
charnlvalue[300];
for(inti=0;slotnamelist[i]!=NULL;i++)
{
status = NLGetSlotValueAsString(m_pNl_Result,slotnamelist[i],nlvalue,300);
if(nlvalue[0]!=0)
{
strcat(m_resultbuf,nlvalue);
strcat(m_resultbuf,";");
}
}
}
NLFreeResult(m_pNl_Result);
}
m_resultbuf[strlen(m_resultbuf)-1]=0;
return0;
}
voidCGrammarThread::StopRecognize(BYTEr)
{
if(m_uRecognition_ID>0)
{
m_pRcEngine->Abort(m_uRecognition_ID);
}else
{
SetEvent(h_recognevent);
}
}

voidCGrammarThread::StopAll()
{
StopPrompt();
StopRecord();
StopRecognize(0);
if(m_dtmfstate==1) StopDtmf(0);
}
voidCheckRecordTime(LPVOIDlp)
{
CGrammarThread* p=(CGrammarThread*)lp;
time_tft;
time(&ft);
while(p->m_uRecord_ID>0)
{
time_tnt;
time(&nt);
if(nt-ft>=p->m_maxtime)
{
p->StopRecord();
break;
}
Sleep(1);
}
}
BYTECGrammarThread::StateRecord(char* filename,BOOLbeep,intmaxtime,intstime,CString dtmf,inttype,CString bgwav,intmtype)
{
m_recordreason=1;
m_maxtime=maxtime;
m_termdtmf=dtmf.GetAt(0);
ResetEvent(h_recordevent);
m_uRecord_ID=m_pRcEngine->GetUniqueID();
//m_pRcEngine->RecordUtterance();
m_pRcEngine->StartRecording(filename,m_uRecord_ID);
if(m_maxtime>0)
{
_beginthread(CheckRecordTime,0,this);
}
WaitForSingleObject(h_recordevent,INFINITE);
returnm_recordreason;
}
voidCGrammarThread::StopRecord()
{
if(m_uRecord_ID>0)
{
m_pRcEngine->StopRecording(m_uRecord_ID);
}else
{
SetEvent(h_recordevent);
}
}
voidCGrammarThread::StateDisconnect()
{
m_pRcEngine->Hangup();
}
voidCGrammarThread::StateTransfer(char*dest,char*src)
{
m_pRcEngine->TransferCall(dest,m_uAnswer_Call_ID);
Sleep(1000);
StateDisconnect();
}
\#include "stdafx.h"
\#include "HandleDataThread.h"
\#include "process.h"

\#include "datastr.h"
\#ifdef _DEBUG
\#define new DEBUG_NEW
\#undef THIS_FILE
staticcharTHIS_FILE[] = __FILE__;
\#endif
usingnamespacestd;
charm_tempwavdir[250];
/////////////////////////////////////////////////////////////////////////////
// CHandleDataThread
voidShowMsg(CString msg);
voidDispatherThread(void* lp);
CString ParseParString(char* src,char*pname,boollast=false);
charParseParChar(char* src,char*pname);
intParseParInt(char* src,char*pname);
BOOLParseParBool(char* src,char*pname);
externCONFIG_PARAM config_params[MAX_HANDLE_THREADS];
externHANDLEm_endevents [MAX_HANDLE_THREADS];
IMPLEMENT_DYNCREATE(CHandleDataThread, CWinThread)
BOOLMyPostThreadMessage(DWORDid,UINTtmsg,WPARAMwParam,LPARAMlParam);
BOOLMyPostThreadMessage(DWORDid,UINTtmsg,WPARAMwParam,LPARAMlParam)
{
if(!(::PostThreadMessage(id,tmsg,wParam,lParam)))
{
NOTIFY_MESSAGE* ps=(NOTIFY_MESSAGE*)lParam;
deleteps;
returnFALSE;
}
returnTRUE;
}
charParseParChar(char* src,char*pname)
{
CString ret=ParseParString(src,pname);
if(ret.GetLength()>0)
return(char)ret.GetAt(0);
return0;
}
BOOLParseParBool(char* src,char*pname)
{
CString ret=ParseParString(src,pname);
ret.TrimLeft();
ret.TrimRight();
if(ret.CompareNoCase("true")==0)
returnTRUE;
returnFALSE;
}

CString ParseParString(char* src,char*pname,boollast)
{
CString ret="";
CString strsrc="";
strsrc.Format("%s",src);
intpstart=0;
intpos=0;
while(pstart<STRSRC.GETLENGTH())if(ret.GetLength() ret="strsrc.Mid(pstart+strlen(pname)+1);"CString { src,char*pname) ParseParInt(char*int} ret;returnpstart="pos+2;"break;elseif(last)if(tpname.CompareNoCase(CString(pname)+?:?)="=0)"tpname="tmpName.Left(strlen(pname)+1);")if(tmpName!=""tmpName="strsrc.Mid(pstart,pos-pstart);"pos='strsrc.Find("@@",pstart);'if(pos="=-1)">0)
{
returnatoi(ret);
}
return0;
}
DWORDexecmdline(char* cmdline)
{
DWORDResult=100;
STARTUPINFO  StartupInfo={0};
PROCESS_INFORMATION ProcessInfo;
StartupInfo.cb =sizeof(StartupInfo);
StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
StartupInfo.wShowWindow = SW_HIDE;
if(CreateProcess(NULL,cmdline,
NULL,NULL,FALSE,
0,NULL,NULL,
&StartupInfo,&ProcessInfo
))
{
WaitForSingleObject(ProcessInfo.hProcess,INFINITE);
GetExitCodeProcess(ProcessInfo.hProcess,&Result);
}
returnResult;
}
CHandleDataThread::CHandleDataThread()
{
}
CHandleDataThread::CHandleDataThread(int_index)
{
m_callstate=0;
m_currentprompt=0;
for(inti=0;i<MAX_PROMPT;I++) {int}returnp_rc- CHandleDataThread::ExitInstance() TRUE; CHandleDataThread::InitInstance()BOOLCHandleDataThread::~CHandleDataThread() _beginthread(DispatherThread,0,this); m_index="_index;"m_eevent="CreateEvent(NULL,FALSE,FALSE,NULL);"sprintf(m_desc,?%d?,_index); m_playlist[i]="0;">CloseDB();
deletep_rc;
WaitForSingleObject(m_eevent,INFINITE);
CloseHandle(m_eevent);
SetEvent(m_endevents[m_index]);
returnCWinThread::ExitInstance();
}
BEGIN_MESSAGE_MAP(CHandleDataThread, CWinThread)
//{{AFX_MSG_MAP(CHandleDataThread)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
ON_THREAD_MESSAGE(IVR_NOTIFY,OnXmlMsgEvent)
ON_THREAD_MESSAGE(WM_HANDLE_PACKAGE,HandleData)
ON_THREAD_MESSAGE(WM_CALL_CONNECT,OnCallConnect)
ON_THREAD_MESSAGE(WM_CALL_DISCONNECT,OnCallDisConnect)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CHandleDataThread message handlers
voidCHandleDataThread::HandleData(WPARAMwPa,LPARAMlPa)
{
}
voidCHandleDataThread::OnXmlMsgEvent(WPARAMwPa,LPARAMlPa)
{
CString Msgstring;
NOTIFY_MESSAGE* p=(NOTIFY_MESSAGE*)lPa;
if(m_callstate==0||m_callstate==2)
{
if(p->command!=DEBUG_MESSAGE)
{
deletep;
TerminalBrowser();
return;
}else
{
ShowMsg(p->szData);
deletep;
TerminalBrowser();
return;
}
}
switch(p->command)
{
caseDEBUG_MESSAGE:
{
ShowMsg(p->szData);
CString msgx=p->szData;
deletep;
if(msgx=="vxml thread quit...")
{
p_rc->StateDisconnect();
}
}
break;
caseOUTPUT_AUDIO:
{
Msgstring.Format("OUTPUT_AUDIO:%s",p->szData);
ShowMsg((char*)(LPCTSTR)Msgstring);
m_playlist[m_currentprompt]=(char*)malloc(strlen(p->szData)+1);
strcpy(m_playlist[m_currentprompt],p->szData);
m_currentprompt++;
deletep;
NOTIFY_MESSAGE* ps=newNOTIFY_MESSAGE;
ps->command=ACTION_COMPLETE;
MyPostThreadMessage(m_browserThreadId,IVR_NOTIFY,0,LPARAM(ps));
}
break;
caseEDIT_WAV:
Msgstring.Format("EDIT_WAV:%s",p->szData);
ShowMsg((char*)(LPCTSTR)Msgstring);
break;
caseACTION_CHAT:
Msgstring.Format("ACTON_CHAT:%s",p->szData);
ShowMsg((char*)(LPCTSTR)Msgstring);
break;
caseWAV_INFO:
Msgstring.Format("WAV_INFO:%s",p->szData);
ShowMsg((char*)(LPCTSTR)Msgstring);
break;
caseACTION_EXEC:
{
Msgstring.Format("ACTION_EXEC:%s",p->szData);
ShowMsg((char*)(LPCTSTR)Msgstring);
CString cmd=ParseParString(p->szData,"cmd");
CString params=ParseParString(p->szData,"params");
deletep;
cmd=cmd +" "+ params;
intret=(int)execmdline((char*)(LPCTSTR)cmd);
CString dStr;
dStr.Format("%d",ret);
NOTIFY_MESSAGE* ps=newNOTIFY_MESSAGE;
ps->command=ACTION_COMPLETE;
ps->szData=newchar[dStr.GetLength()+1];
strcpy(ps->szData,dStr);
MyPostThreadMessage(m_browserThreadId,IVR_NOTIFY,0,LPARAM(ps));
}
break;
caseACTION_CVTWAVE:
{
Msgstring.Format("ACTION_CVTWAVE:%s",p->szData);
ShowMsg((char*)(LPCTSTR)Msgstring);
CString action=ParseParString(p->szData,"action");
CString src=ParseParString(p->szData,"src");
CString dest=ParseParString(p->szData,"dest");
deletep;
CString dStr="error";
action.MakeLower();
FILE* fp=NULL;
if((fp=fopen((char*)(LPCTSTR)src,"r"))!=NULL)
{
fclose(fp);
if(action=="p8k16b2a8k8b")
{
}
elseif(action=="a8k8b2p8k16b")
{
}
dStr="success";
}
NOTIFY_MESSAGE* ps=newNOTIFY_MESSAGE;
ps->command=ACTION_COMPLETE;
ps->szData=newchar[dStr.GetLength()+1];
strcpy(ps->szData,dStr);
MyPostThreadMessage(m_browserThreadId,IVR_NOTIFY,0,LPARAM(ps));
}
break;
caseOUTPUT_CATWAV:
Msgstring.Format("OUTPUT_CATWAV:%s",p->szData);
ShowMsg((char*)(LPCTSTR)Msgstring);
break;
caseOUTPUT_DISCONNECT:
ShowMsg("OUTPUT_DISCONNECT");
deletep;
PlayPrompt(true);
p_rc->StateDisconnect();
break;
caseINPUT_DTMF_M:
{
Msgstring.Format("INPUT_DTMF_M:%s",p->szData);
ShowMsg((char*)(LPCTSTR)Msgstring);
Handle_DTMF_M(p);
}
break;
caseINPUT_ENROLL:
{
Msgstring.Format("INPUT_ENROLL:%s",p->szData);
ShowMsg((char*)(LPCTSTR)Msgstring);
}
break;
caseCOPY_DYNAGRAMMAR:
{
Msgstring.Format("COPY_DYNAGRAMMAR:%s",p->szData);
ShowMsg((char*)(LPCTSTR)Msgstring);
}
break;
caseACTION_DYNAGRAMMAR:
{
Msgstring.Format("ACTION_DYNAGRAMMAR:%s",p->szData);
ShowMsg((char*)(LPCTSTR)Msgstring);
}
break;
caseINPUT_SPOKEN_RECORD:
{
Msgstring.Format("INPUT_SPOKEN_RECORD:%s",p->szData);
ShowMsg((char*)(LPCTSTR)Msgstring);
Hand_INPUT_SPOKEN_RECORD(p);
}
break;
caseSTART_PROMPT:
{
deletep;
ShowMsg("START_PROMPT");
PlayPrompt(true);
NOTIFY_MESSAGE* ps=newNOTIFY_MESSAGE;
ps->command=ACTION_COMPLETE;
MyPostThreadMessage(m_browserThreadId,IVR_NOTIFY,0,LPARAM(ps));
}
break;
caseOUTPUT_DIALOUT:
Msgstring.Format("OUTPUT_DIALOUT:%s",p->szData);
ShowMsg((char*)(LPCTSTR)Msgstring);
break;
caseOUTPUT_TRANSFER:
Msgstring.Format("OUTPUT_TRANSFER:%s",p->szData);
ShowMsg((char*)(LPCTSTR)Msgstring);
Hand_TRANSFER(p);
break;
caseSEND_SMS:
Msgstring.Format("SEND_SMS:%s",p->szData);
ShowMsg((char*)(LPCTSTR)Msgstring);
break;
caseEMAIL_SMTP:
Msgstring.Format("EMAIL_SMTP:%s",p->szData);
ShowMsg((char*)(LPCTSTR)Msgstring);
break;
caseTTS_FILE:
Msgstring.Format("TTS:%s",p->szData);
ShowMsg((char*)(LPCTSTR)Msgstring);
break;
caseINPUT_SR_M:
{
Msgstring.Format("INPUT_SR_M:%s",p->szData);
ShowMsg((char*)(LPCTSTR)Msgstring);
Handle_INPUT_SR_M(p);
}
break;
caseINPUT_SRDTMF_M:
{
Msgstring.Format("INPUT_SRDTMF_M:%s",p->szData);
ShowMsg((char*)(LPCTSTR)Msgstring);
Hand_INPUT_SRDTMF_M(p);
}
break;
default:
deletep;
break;
}
}
voidDispatherThread(void* lp)
{
CHandleDataThread* p=(CHandleDataThread*)lp;
p->InitRc();
}
boolCHandleDataThread::InitRc()
{
CString msg;
NuanceStatus status;
NuanceConfig * config =
NuanceConfigBuildFromCommandLine(&(config_params[m_index].argc), config_params[m_index].argv, 1, &status);
if( (NULL == config) || (NUANCE_OK != status) ) {
msg.Format("T%d: NuanceConfigBuildFromCommandLine 失败",m_index);
ShowMsg(msg);
return0;
}
intsilent_flag = 1;
DBDescriptor * dynagram_db_desc =
NewDBDescriptorFromCommandLine(&(config_params[m_index].argc), config_params[m_index].argv,"", &silent_flag);
Dispatcher * d =newDispatcher(config, status);
if(NUANCE_OK != status) {
msg.Format("T%d: Dispatcher create 失败",m_index);
ShowMsg(msg);
deleted;
NuanceConfigFree(config);
return0;
}
p_rc=newCGrammarThread(*config,*d,dynagram_db_desc,this);
msg.Format("T%d: Dispatcher listening......",m_index);
ShowMsg(msg);
SetEvent(m_endevents[m_index]);
d->Dispatch();
deleted;
d = NULL;
NuanceConfigFree(config);
config = NULL;
msg.Format("T%d: Dispatcher Close......",m_index);
ShowMsg(msg);
msg.Empty();
SetEvent(m_eevent);
returntrue;
}
voidsss(LPVOIDlp)
{
CHandleDataThread* p=(CHandleDataThread*)lp;
Sleep(10000);
p->p_rc->StopPrompt();
}
voidCHandleDataThread::PlayPrompt(bool_wait)
{
p_rc->StatePrompt(_wait);
}
voidCHandleDataThread::CreateBrowser()
{
CreateBrowserThread(m_homepage,m_nThreadID,&m_browserThreadId,m_callingID,m_calledID,m_desc,m_tempwavdir);
}
voidCHandleDataThread::OnCallConnect(WPARAMwPa,LPARAMlPa)
{
RemoveAllWav();
CreateBrowser();
}
voidCHandleDataThread::RemoveAllWav()
{
for(inti=0;i<M_CURRENTPROMPT;I++) {int} m_playlist[i]="NULL;"len="ParseParInt(p-"*p) CHandleDataThread::Handle_DTMF_M(NOTIFY_MESSAGEvoidm_currentprompt="0;"; free(m_playlist[i])>szData,"len");
intfinalsilence=ParseParInt(p->szData,"finalsilence");
intfirsttimeout=ParseParInt(p->szData,"firsttime");
chartermdtmf=ParseParChar(p->szData,"termdtmf");
charsdtmf=ParseParChar(p->szData,"sdtmf");
if(finalsilence==0) finalsilence=4;
if(len==0) len=40;
deletep;
BYTEret=p_rc->RunDtmf(len,firsttimeout,finalsilence,termdtmf,sdtmf);
if(ret==1)
{
ShowMsg(p_rc->m_dtmfbuf);
NOTIFY_MESSAGE* ps=newNOTIFY_MESSAGE;
ps->command=ACTION_COMPLETE;
ps->szData=newchar[p_rc->m_dtmfcnt+1];
strcpy(ps->szData,p_rc->m_dtmfbuf);
MyPostThreadMessage(m_browserThreadId,IVR_NOTIFY,0,LPARAM(ps));
}
}
voidCHandleDataThread::Handle_INPUT_SR_M(NOTIFY_MESSAGE *p)
{
CString gkey=ParseParString(p->szData,"gkey");
BOOL_overwrite=ParseParBool(p->szData,"gover");
CString grammar=ParseParString(p->szData,"grammar");
WORDnbest=(WORD)ParseParInt(p->szData,"nbest");
if(nbest==0) nbest=1;
WORDnospeechtimeout=(WORD)ParseParInt(p->szData,"nospeechtimeout");
if(nospeechtimeout==0) nospeechtimeout=7;
WORDtoomuchms=(WORD)ParseParInt(p->szData,"toomuchms");
if(toomuchms==0) toomuchms=60;
WORDtooslowms=(WORD)ParseParInt(p->szData,"tooslowms");
if(tooslowms==0) tooslowms=0;
WORDrejthreshold=(WORD)ParseParInt(p->szData,"rejthreshold");
if(rejthreshold==0) rejthreshold=38;
BOOLhot=(WORD)ParseParBool(p->szData,"hotword");
boolhotword=false;
if(hot) hotword=true;

deletep;
charr[100];
memset(r,0,100);
strcpy(r,"error");
BYTEret;
if(grammar.Left(1)==".")
ret=p_rc->RecognizeUttr(hotword,nbest,  nospeechtimeout, toomuchms, tooslowms,  rejthreshold,r,NULL,(char*)(LPCTSTR)grammar,(_overwrite?true:false),((gkey.GetLength()>0)?((char*)(LPCTSTR)gkey):NULL),0,10,'\#');
else
ret=p_rc->RecognizeUttr(hotword,nbest,  nospeechtimeout, toomuchms, tooslowms,  rejthreshold,r,(char*)(LPCTSTR)grammar,NULL,(_overwrite?true:false),((gkey.GetLength()>0)?((char*)(LPCTSTR)gkey):NULL),0,10,'\#');
ShowMsg(r);
NOTIFY_MESSAGE* ps=newNOTIFY_MESSAGE;
ps->command=ACTION_COMPLETE;
ps->szData=newchar[strlen(r)+1];
strcpy(ps->szData,r);
MyPostThreadMessage(m_browserThreadId,IVR_NOTIFY,0,LPARAM(ps));
}

voidCHandleDataThread::Hand_INPUT_SRDTMF_M(NOTIFY_MESSAGE *p)
{
CString gkey=ParseParString(p->szData,"gkey");
BOOL_overwrite=ParseParBool(p->szData,"gover");
CString grammar=ParseParString(p->szData,"grammar");
intlen=ParseParInt(p->szData,"len");
intfinalsilence=ParseParInt(p->szData,"finalsilence");
intfirsttimeout=ParseParInt(p->szData,"firsttime");
chartermdtmf=ParseParChar(p->szData,"termdtmf");
charsdtmf=ParseParChar(p->szData,"sdtmf");
if(finalsilence==0) finalsilence=4;
if(len==0) len=40;
WORDnbest=(WORD)ParseParInt(p->szData,"nbest");
if(nbest==0) nbest=1;
WORDnospeechtimeout=(WORD)ParseParInt(p->szData,"nospeechtimeout");
if(nospeechtimeout==0) nospeechtimeout=7;
WORDtoomuchms=(WORD)ParseParInt(p->szData,"toomuchms");
if(toomuchms==0) toomuchms=60;
WORDtooslowms=(WORD)ParseParInt(p->szData,"tooslowms");
if(tooslowms==0) tooslowms=0;
WORDrejthreshold=(WORD)ParseParInt(p->szData,"rejthreshold");
if(rejthreshold==0) rejthreshold=38;
BOOLhot=(WORD)ParseParBool(p->szData,"hotword");
boolhotword=false;
if(hot) hotword=true;
deletep;
charr[100];
memset(r,0,100);
strcpy(r,"error");
BYTEret;
if(grammar.Left(1)==".")
ret=p_rc->RecognizeUttr(hotword,nbest,  nospeechtimeout, toomuchms, tooslowms,  rejthreshold,r,NULL,(char*)(LPCTSTR)grammar,(_overwrite?true:false),((gkey.GetLength()>0)?((char*)(LPCTSTR)gkey):NULL),len,finalsilence,termdtmf,sdtmf);
else
ret=p_rc->RecognizeUttr(hotword,nbest,  nospeechtimeout, toomuchms, tooslowms,  rejthreshold,r,(char*)(LPCTSTR)grammar,NULL,(_overwrite?true:false),((gkey.GetLength()>0)?((char*)(LPCTSTR)gkey):NULL),len,finalsilence,termdtmf,sdtmf);
ShowMsg(r);
NOTIFY_MESSAGE* ps=newNOTIFY_MESSAGE;
ps->command=ACTION_COMPLETE;
ps->szData=newchar[strlen(r)+1];
strcpy(ps->szData,r);
MyPostThreadMessage(m_browserThreadId,IVR_NOTIFY,0,LPARAM(ps));
}
voidCHandleDataThread::OnCallDisConnect(WPARAMwPa,LPARAMlPa)
{
printf("disconnected/r/n");
TerminalBrowser();
}
voidCHandleDataThread::Hand_INPUT_SPOKEN_RECORD(NOTIFY_MESSAGE *p)
{
intmaxtime=ParseParInt(p->szData,"maxtime");
intiddtime=ParseParInt(p->szData,"finalsilence");
CString dtmf=ParseParString(p->szData,"dtmfterm");
BOOLbeep=ParseParBool(p->szData,"beep");
CString type=ParseParString(p->szData,"type");
CString bgmusic=ParseParString(p->szData,"bgmusic");
intmixtype=ParseParInt(p->szData,"mixtype");
intrtype=0;
if(dtmf=="") dtmf="a";
if(type=="audio/vox") rtype=1;
deletep;
PlayPrompt(true);
CString filename=GenuniqueRecFileName(rtype);
BYTEret=p_rc->StateRecord((char*)(LPCTSTR)filename,beep,maxtime,iddtime,dtmf,rtype,bgmusic,mixtype);
if(ret!=1) filename="";
NOTIFY_MESSAGE* ps=newNOTIFY_MESSAGE;
ps->command=ACTION_COMPLETE;
ps->szData=newchar[filename.GetLength()+1];
strcpy(ps->szData,filename);
MyPostThreadMessage(m_browserThreadId,IVR_NOTIFY,0,LPARAM(ps));
}
CString CHandleDataThread::GenuniqueRecFileName(inttype)
{
intindex=0;
CString _filter="";
if(type==0) _filter="wav";
else_filter="vox";
time_t_t=0;
time(&_t);
CString _newfile;
CString _tmpdir("e://app");
_tmpdir.TrimRight("//");
_newfile.Format("%s//rec%d%ld.%s",_tmpdir,m_index,_t,_filter);
CFileFind file;
while(file.FindFile(_newfile))
{
index+=1;
_newfile.Format("%s//rec%d%ld%d.%s",_tmpdir,m_index,_t,index,_filter);
file.Close ();
}
return_newfile;
}
voidCHandleDataThread::Hand_TRANSFER(NOTIFY_MESSAGE *p)
{
CString dest=ParseParString(p->szData,"dest");
BOOLbridge=ParseParBool(p->szData,"bridge");
intchindex=ParseParInt(p->szData,"chindex");
intdelay=ParseParInt(p->szData,"delay");
CString src=ParseParString(p->szData,"src");
deletep;
if(!bridge)
{
}else
{
}
p_rc->StateTransfer((char*)(LPCTSTR)dest,(char*)(LPCTSTR)src);
}
voidCHandleDataThread::TerminalBrowser()
{
if(m_browserThreadId!=0)
{
::PostThreadMessage(m_browserThreadId,WM_QUIT,0,0);
m_browserThreadId=0;
}
}


