# ms speech sdk51 TTS使用 - 专注于数据挖掘算法研究和应用 - CSDN博客





2008年05月13日 15:28:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4240








                1.下载speech sdk5.1,安装;2.下载语言包，安装;3.vc环境中包含相应的头文件和lib  头文件一般在Tools/options/Directories中加上speech sdk5.1安装路径下include   对于Lib我喜欢直接从拷贝到工程路径下使用。  下面例子分别给出了中英的读并导出录音文件。4.#include <sapi.h>#include <sphelper.h>#pragma comment(lib,"ole32.lib")  //CoInitialize CoCreateInstance需要调用ole32.dll#pragma comment(lib,"sapi.lib")   //sapi.lib在SDK的lib目录,必需正确配置int main(int argc, char* argv[]){  //COM初始化：    if   (FAILED(::CoInitialize(NULL)))    return   0; ISpVoice* pVoice=NULL; HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice,(void **)&pVoice); if(SUCCEEDED(hr)) {  /*pVoice->Speak(L"Hello World",SPF_DEFAULT,NULL);*/  /*IEnumSpObjectTokens *pSpEnumTokens=NULL;  hr=SpEnumTokens(SPCAT_VOICES,L"Language=804",NULL,&pSpEnumTokens);  if(SUCCEEDED(hr))  {   ISpObjectToken *pSpToken = NULL;   while(SUCCEEDED(pSpEnumTokens->Next(1, &pSpToken, NULL)) && pSpToken != NULL)   {    pVoice->SetVoice(pSpToken);    pVoice->Speak(L"我们都是中国人",SPF_DEFAULT,NULL);    pSpToken->Release();   }   pSpEnumTokens->Release();  }*/  /*ISpObjectToken *pSpToken = NULL;  if (SUCCEEDED(SpFindBestToken(SPCAT_VOICES, L"Language=804",NULL,&pSpToken)))  {   pVoice->SetVoice(pSpToken);   pVoice->Speak(L"我在广州电信上班",SPF_DEFAULT,NULL);   pSpToken->Release();  }*/  /*CComPtr<ISpStream> cpWavStream;   CComPtr<ISpStreamFormat> cpOldStream;   CSpStreamFormat OriginalFmt;   pVoice->GetOutputStream( &cpOldStream );   OriginalFmt.AssignFormat(cpOldStream);   hr = SPBindToFile( L"D://output.wav",                  SPFM_CREATE_ALWAYS,         &cpWavStream,         &OriginalFmt.FormatId(),          OriginalFmt.WaveFormatExPtr() );   if( SUCCEEDED( hr ) )   {    pVoice->SetOutput(cpWavStream,TRUE);    WCHAR WTX[] = L"<VOICE REQUIRED='' NAME='Microsoft Mary'/> text to wave";    pVoice->Speak(WTX, SPF_IS_XML, NULL);   }*/  CComPtr<ISpStream> cpWavStream;   CComPtr<ISpStreamFormat> cpOldStream;   CSpStreamFormat OriginalFmt;   pVoice->GetOutputStream( &cpOldStream );   OriginalFmt.AssignFormat(cpOldStream);   hr = SPBindToFile( L"D://output.wav",                  SPFM_CREATE_ALWAYS,         &cpWavStream,         &OriginalFmt.FormatId(),          OriginalFmt.WaveFormatExPtr() );  ISpObjectToken *pSpToken = NULL;  if (SUCCEEDED(SpFindBestToken(SPCAT_VOICES, L"Language=804",NULL,&pSpToken)))  {   pVoice->SetVoice(pSpToken);   pVoice->SetOutput(cpWavStream,TRUE);   pVoice->Speak(L"我们都是中国人",SPF_DEFAULT,NULL);   pSpToken->Release();  } } pVoice->Release(); //释放COM资源： ::CoUninitialize();     return true;   }            


