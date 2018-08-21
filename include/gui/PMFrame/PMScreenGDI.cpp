#include "StdAfx.h"
#include "PMScreenGDI.h"
#include "PMSpaceAlgorithm.h"


#define LAFERNOTE "//********************************************************************************\r\n\
//文件：lafer语法 <by feiren>  2012-11-22 12:39\r\n\
//说明：\r\n\
//  1、点用法：     POINT:宽度,颜色(R,G,B),坐标(x,y);  例如：POINT:1.1,(166,166,166),(0,0,-1.1,0);\r\n\
//  2、线用法：     LINE:宽度,颜色(R,G,B),坐标(x,y,x1,y1); \r\n\
//  3、折线用法：   POLYLINE:宽度,颜色(R,G,B),坐标(x,y,x1,y1,x2,y2,...); \r\n\
//  4、三角形用法： TRIANGLE:宽度,颜色(R,G,B),填充颜色(R,G,B),坐标(x,y,x1,y1,x2,y2); \r\n\
//  5、矩形用法：   RECTANGLE:宽度,颜色(R,G,B),填充颜色(R,G,B),坐标(x,y,x1,y1); \r\n\
//  6、渐变色矩形用法:SMOOTINGRECT:方向(0或-0),填充颜色1(R,G,B),填充颜色2(R,G,B),坐标(x,y,x1,y1); \r\n\
//  7、多边形用法： POLYGON:宽度,颜色(R,G,B),填充颜色(R,G,B),坐标(x,y,x1,y1,x2,y2,...); \r\n\
//  8、椭圆用法：   ELLIPSE:宽度,颜色(R,G,B),填充颜色(R,G,B),坐标(x,y,x1,y1); \r\n\
//  9、扇形用法：   PIE:宽度,颜色(R,G,B),填充颜色(R,G,B),坐标(x,y,x1,y1,xr1,yr1,xr2,yr2); \r\n\
//	宽度说明：0为不绘制线条，0~1为按照宽度比例计算宽度，-1~0为按高度比例绘制宽度，其余为实际宽度\r\n\
//  颜色说明：R、G、B在[0~255]之间，如果为-0为不绘制颜色\r\n\
//  坐标说明：x——0为x原点，-0为x右边原点，(0~1)为左边开始的宽度比例坐标，(-1~0)为右边开始的宽度比例坐标，其余为实际坐标\r\n\
//            y——0为y原点，-0为y下边原点，(0~1)为上边开始的宽度比例坐标，(-1~0)为下边开始的宽度比例坐标，其余为实际坐标\r\n\
//********************************************************************************\r\n\r\n"

PMPicture::PMPicture():mhBitmap(NULL)//,mJson(NULL)
{
	
}

PMPicture::~PMPicture()
{
	ReleaseBuffer();
}

PMVoid PMPicture::ReleaseBuffer()
{
	if(mhBitmap)
	{
		DeleteObject(mhBitmap);
		mhBitmap=NULL;
	}
}

PMVoid PMPictures::ClearPictures()
{
	for (vector<PMPicture*>::iterator it = mPictures.begin(); it != mPictures.end(); it++)
	{
		delete *it;
	}
	mPictures.clear();
}

PMPictures::PMPictures()
{

}

PMPictures::~PMPictures()
{	
	ClearPictures();
}

PMPicture* PMPictures::operator[]( PMInt index )
{
	if(index>=0&&index<(PMInt)mPictures.size())
	{
		return mPictures[index];
	}
	return NULL;
}


std::string PMLaferLexer::GetJsonValue( TJSON &laferJson )
{
	if(laferJson.IsArray())
	{
		string strValue;
		for (TJSON::iterator it = laferJson.begin(); it != laferJson.end(); it++)
		{
			TJSON &json = *it;
			strValue+=json==0&&(PMInt)json.GetDefine()==1?"-0":json.ToString();
		}
		return strValue;
	}else
	{
		return laferJson==0&&(PMInt)laferJson.GetDefine()==1?"-0":laferJson.ToString();
	}
}

std::string PMLaferLexer::GetJsonColor( TJSON &laferJson )
{
	PMInt color = laferJson.ToInt();
	PMChar buffer[30]={0};
	return (color==0&&(PMInt)laferJson.GetDefine()==1?"-0,-0,-0":(string(itoa(GetRValue(color),buffer,10))+","+itoa(GetGValue(color),buffer,10)+","+itoa(GetBValue(color),buffer,10))) ;
}

std::string PMLaferLexer::CreateLaferCode( TJSON &laferJson )
{
	string LaferCode = LAFERNOTE;
	for (TJSON::iterator it = laferJson.begin(); it != laferJson.end(); it++)
	{
		TJSON &jnEntity = (*it);
		LaferCode+="ENTITY "+jnEntity.GetName()+"\r\n{\r\n";
		LaferCode+=EntityToCode(jnEntity);
		LaferCode+="}\r\n";
	}
	return LaferCode;
}

std::string PMLaferLexer::EntityToCode( TJSON &laferJson )
{
	string LaferCode ;
	string name;
	int index =0;
	for (TJSON::iterator it = laferJson.begin(); it != laferJson.end(); it++)
	{
		TJSON &json = (*it);
		name = json.GetName();
		if(name=="FRAME")
		{
			LaferCode+="\t//"+PMSpaceAlgorithm::itoa(index)+"、第"+PMSpaceAlgorithm::GetChineseNumber(index+1)+"帧\r\n\tFRAME ";
			LaferCode+=FrameToCode(*it);
			index++;
		}else
		{		
			LaferCode+="\t//边框的大小\r\n\t"+name+"(";
			LaferCode+=GetJsonValue(json[0])+",";
			LaferCode+=GetJsonValue(json[1])+",";
			LaferCode+=GetJsonValue(json[2])+",";
			LaferCode+=GetJsonValue(json[3])+");\r\n";
		}
	}
	return LaferCode;
}

std::string PMLaferLexer::FrameToCode( TJSON &laferJson )
{
	string LaferCode= laferJson.GetName() + "\r\n\t{\r\n";
	for (TJSON::iterator it = laferJson.begin(); it != laferJson.end(); it++)
	{
		LaferCode+=BodyToCode(*it);
	}
	LaferCode += "\t}\r\n";
	return LaferCode;
}

std::string PMLaferLexer::BodyToCode( TJSON &laferJson )
{
	string LaferCode="\t\t";
	LaferCode += laferJson.GetName() + "\r\n\t\t{\r\n";
	for (TJSON::iterator it = laferJson.begin(); it != laferJson.end(); it++)
	{
		TJSON &json = (*it);
		switch(json[0].ToInt())
		{
		case PMENTITY_POINT:
			LaferCode += PointToCode(json);
			break;
		case PMENTITY_LINE:
			LaferCode += LineToCode(json);
			break;
		case PMENTITY_POLYLINE:
			LaferCode += PolyLineToCode(json);
			break;
		case PMENTITY_TRIANGLE:
			LaferCode += TriangleToCode(json);
			break;
		case PMENTITY_RECTANGLE:
			LaferCode += RectangleToCode(json);
			break;
		case PMENTITY_POLYGON:
			LaferCode += PolygonToCode(json);
			break;
		case PMENTITY_ELLIPSE:
			LaferCode += EllipseToCode(json);
			break;
		case PMENTITY_PIE:
			LaferCode += PieToCode(json);
			break;
		}
	}
	LaferCode += "\t\t}\r\n";
	return LaferCode;
}

std::string PMLaferLexer::PointToCode( TJSON &laferJson )
{
	string LaferCode = "\t\t\tPOINT:";
	LaferCode+=GetJsonValue(laferJson[1])+",("+GetJsonColor(laferJson[2])+"),("+GetJsonValue(laferJson[3])+","+GetJsonValue(laferJson[4])+");\r\n";
	return LaferCode;
}

std::string PMLaferLexer::LineToCode( TJSON &laferJson )
{
	string LaferCode = "\t\t\tLINE:";
	LaferCode+=GetJsonValue(laferJson[1])+",("+GetJsonColor(laferJson[2])+"),("+GetJsonValue(laferJson[3])+
		","+GetJsonValue(laferJson[4])+","+GetJsonValue(laferJson[5])+","+GetJsonValue(laferJson[6])+");\r\n";
	return LaferCode;
}

std::string PMLaferLexer::PolyLineToCode( TJSON &laferJson )
{
	string LaferCode = "\t\t\tPOLYLINE:";
	LaferCode+=GetJsonValue(laferJson[1])+",("+GetJsonColor(laferJson[2])+"),("+GetJsonValue(laferJson[3]);
	for(TJSON::iterator it = laferJson.begin()+4;it!=laferJson.end();it++)
	{
		LaferCode+=","+GetJsonValue(*it);
	}
	LaferCode+=");\r\n";
	return LaferCode;
}

std::string PMLaferLexer::TriangleToCode( TJSON &laferJson )
{
	string LaferCode = "\t\t\tTRIANGLE:";
	LaferCode+=GetJsonValue(laferJson[1])+",("+GetJsonColor(laferJson[2])+"),("+GetJsonColor(laferJson[3])+"),("+
		GetJsonValue(laferJson[4])+","+GetJsonValue(laferJson[5])+","+GetJsonValue(laferJson[6])+","+GetJsonValue(laferJson[7])+","+
		GetJsonValue(laferJson[8])+","+GetJsonValue(laferJson[9])+");\r\n";
	return LaferCode;
}

std::string PMLaferLexer::RectangleToCode( TJSON &laferJson )
{
	string LaferCode = "\t\t\tRECTANGLE:";
	LaferCode+=GetJsonValue(laferJson[1])+",("+GetJsonColor(laferJson[2])+"),("+GetJsonColor(laferJson[3])+"),("+
		GetJsonValue(laferJson[4])+","+GetJsonValue(laferJson[5])+","+GetJsonValue(laferJson[6])+","+GetJsonValue(laferJson[7])+");\r\n";
	return LaferCode;
}

std::string PMLaferLexer::SmootingRectToCode( TJSON &laferJson )
{
	string LaferCode = "\t\t\tSMOOTINGRECT:";
	LaferCode+=GetJsonValue(laferJson[1])+",("+GetJsonColor(laferJson[2])+"),("+GetJsonColor(laferJson[3])+"),("+
		GetJsonValue(laferJson[4])+","+GetJsonValue(laferJson[5])+","+GetJsonValue(laferJson[6])+","+GetJsonValue(laferJson[7])+");\r\n";
	return LaferCode;
}

std::string PMLaferLexer::EllipseToCode( TJSON &laferJson )
{
	string LaferCode = "\t\t\tELLIPSE:";
	LaferCode+=GetJsonValue(laferJson[1])+",("+GetJsonColor(laferJson[2])+"),("+GetJsonColor(laferJson[3])+"),("+
		GetJsonValue(laferJson[4])+","+GetJsonValue(laferJson[5])+","+GetJsonValue(laferJson[6])+","+GetJsonValue(laferJson[7])+");\r\n";
	return LaferCode;
}

std::string PMLaferLexer::PolygonToCode( TJSON &laferJson )
{
	string LaferCode = "\t\t\tPOLYGON:";
	LaferCode+=GetJsonValue(laferJson[1])+",("+GetJsonColor(laferJson[2])+"),("+GetJsonColor(laferJson[3])+"),("+GetJsonValue(laferJson[4]);
	for(TJSON::iterator it = laferJson.begin()+5;it!=laferJson.end();it++)
	{
		LaferCode+=","+GetJsonValue(*it);
	}
	LaferCode+=");\r\n";
	return LaferCode;
}

std::string PMLaferLexer::PieToCode( TJSON &laferJson )
{
	string LaferCode = "\t\t\tPIE:";
	LaferCode+=GetJsonValue(laferJson[1])+",("+GetJsonColor(laferJson[2])+"),("+GetJsonColor(laferJson[3])+"),("+
		GetJsonValue(laferJson[4])+","+GetJsonValue(laferJson[5])+","+GetJsonValue(laferJson[6])+","+GetJsonValue(laferJson[7])+","+
		GetJsonValue(laferJson[8])+","+GetJsonValue(laferJson[9])+","+GetJsonValue(laferJson[10])+","+GetJsonValue(laferJson[11])+");\r\n";
	return LaferCode;
}

PMVoid PMLaferLexer::SetJsonValue( TJSON &laferJson,TWordLex &lex )
{
	string strValue = lex.GetWord();
	if (lex.IsExpression())
	{
		PMInt i=0;
		TExpressionLex ExpLex(strValue.c_str());
		strValue = ExpLex.GetWord();
		while (strValue!="")
		{
			SetJsonValue(laferJson[i++],strValue);
			strValue = ExpLex.GetWord();
		}
	}else
	{
		SetJsonValue(laferJson,strValue);
	}
}


PMVoid PMLaferLexer::SetJsonValue( TJSON &laferJson,const string &strValue )
{
	PMDouble dValue=PMSpaceAlgorithm::atof(strValue.c_str());
	if(PMSpaceAlgorithm::IsInteger(dValue))
	{
		laferJson = (PMInt)dValue;
		if(strValue=="-0")
		{
			laferJson.SetDefine((PMVoid*)1);
		}
	}else
	{
		laferJson = dValue;
	}
}


PMVoid PMLaferLexer::SetJsonColor( TJSON &laferJson,TWordLex &lex )
{
	string R=lex.GetWord();
	string G=lex.GetWord();
	string B=lex.GetWord();
	if(R=="-0"||G=="-0"||B=="-0")
	{
		laferJson = 0;
		laferJson.SetDefine((PMVoid*)1);
	}else
	{
		PMInt color = RGB(PMSpaceAlgorithm::atoi(R),PMSpaceAlgorithm::atoi(G),PMSpaceAlgorithm::atoi(B));
		laferJson = color;
	}
}

TJSON * PMLaferLexer::CreateLaferJson( const string &laferdata )
{
	TJSON *JSON=new TJSON();
	TWordLex lex(laferdata.c_str());
	string word = lex.GetWord();
	while (word!="")
	{
		ToUpper(word);
		if(word=="ENTITY")
		{
			ParseEntity(*JSON,lex);
		}
		word = lex.GetWord();
	}
	return JSON;
}

PMVoid PMLaferLexer::ParseEntity( TJSON &laferJson,TWordLex &lex )
{
	string name = lex.GetWord();
	TJSON &jnEntity =  laferJson[name];
	string word = lex.GetWord();
	while (word!=""&&word!="}")
	{
		ToUpper(word);
		if(word=="FRAME")
		{
			ParseFrame(jnEntity,lex);
		}else
		{
			TJSON &laferRect = jnEntity[word];
			ParseRect(laferRect,lex);
		}
		word = lex.GetWord();
	}
}

PMVoid PMLaferLexer::ParseRect( TJSON &laferJson,TWordLex &lex )
{
	SetJsonValue(laferJson[0],lex);
	SetJsonValue(laferJson[1],lex);
	SetJsonValue(laferJson[2],lex);
	SetJsonValue(laferJson[3],lex);
}

PMVoid PMLaferLexer::ParseFrame( TJSON &laferJson,TWordLex &lex )
{
	string name = lex.GetWord();
	TJSON &jnEntity =  laferJson[name];
	string word = lex.GetWord();
	while (word!=""&&word!="}")
	{
		ToUpper(word);
		if(word=="LINE"||word=="FILL")
		{
			TJSON &jnBorder=  jnEntity[word];
			ParseBody(jnBorder,lex);
		}
		word = lex.GetWord();
	}
}

PMVoid PMLaferLexer::ParseBody( TJSON &laferJson,TWordLex &lex )
{
	string word = lex.GetWord();
	int i=0;
	while (word!=""&&word!="}")
	{
		ToUpper(word);
		if(word=="LINE")
		{
			ParseLine(laferJson[i++],lex);
		}else if(word=="RECTANGLE")
		{
			ParseRectangle(laferJson[i++],lex);
		}else if(word=="ELLIPSE")
		{
			ParseEclipse(laferJson[i++],lex);
		}else if(word=="TRIANGLE")
		{
			ParseTriangle(laferJson[i++],lex);
		}else if(word=="POLYGON")
		{
			ParsePolygon(laferJson[i++],lex);
		}else if(word=="POLYLINE")
		{
			ParsePolyLine(laferJson[i++],lex);
		}else if(word=="PIE")
		{
			ParsePie(laferJson[i++],lex);
		}else if(word=="POINT")//ENTITY
		{
			ParsePoint(laferJson[i++],lex);
		}else if(word=="SMOOTINGRECT")
		{
			ParseSmootingRect(laferJson[i++],lex);
		}
		word = lex.GetWord();
	}
}

PMVoid PMLaferLexer::ParsePoint( TJSON &laferJson,TWordLex &lex )
{
	PMInt i = 1; 
	laferJson[0]=PMENTITY_POINT; 
	SetJsonValue(laferJson[i++],lex);
	SetJsonColor(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
}

PMVoid PMLaferLexer::ParseLine( TJSON &laferJson,TWordLex &lex )
{
	PMInt i = 1; 
	laferJson[0]=PMENTITY_LINE; 
	SetJsonValue(laferJson[i++],lex);
	SetJsonColor(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
}

PMVoid PMLaferLexer::ParsePolyLine( TJSON &laferJson,TWordLex &lex )
{
	PMInt i = 1; 
	laferJson[0]=PMENTITY_POLYLINE; 
	SetJsonValue(laferJson[i++],lex);
	SetJsonColor(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	lex.SetReturnSymbol(";");
	string word = lex.GetWord();
	while (word!=""&&word!=";")
	{
		SetJsonValue(laferJson[i++],lex);
		SetJsonValue(laferJson[i++],lex);
		word = lex.GetWord(false);
	}
	lex.SetReturnSymbol(NULL);
}

PMVoid PMLaferLexer::ParseTriangle( TJSON &laferJson,TWordLex &lex )
{
	PMInt i = 1; 
	laferJson[0]=PMENTITY_TRIANGLE; 
	SetJsonValue(laferJson[i++],lex);
	SetJsonColor(laferJson[i++],lex);
	SetJsonColor(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
}

PMVoid PMLaferLexer::ParseRectangle( TJSON &laferJson,TWordLex &lex )
{
	PMInt i = 1; 
	laferJson[0]=PMENTITY_RECTANGLE; 
	SetJsonValue(laferJson[i++],lex);
	SetJsonColor(laferJson[i++],lex);
	SetJsonColor(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
}

PMVoid PMLaferLexer::ParseSmootingRect( TJSON &laferJson,TWordLex &lex )
{
	PMInt i = 1; 
	laferJson[0]=PMENTITY_SMOOTINGRECT; 
	SetJsonValue(laferJson[i++],lex);
	SetJsonColor(laferJson[i++],lex);
	SetJsonColor(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
}

PMVoid PMLaferLexer::ParseEclipse( TJSON &laferJson,TWordLex &lex )
{
	PMInt i = 1; 
	laferJson[0]=PMENTITY_ELLIPSE; 
	SetJsonValue(laferJson[i++],lex);
	SetJsonColor(laferJson[i++],lex);
	SetJsonColor(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
}

PMVoid PMLaferLexer::ParsePolygon( TJSON &laferJson,TWordLex &lex )
{
	PMInt i = 1; 
	laferJson[0]=PMENTITY_POLYGON; 
	SetJsonValue(laferJson[i++],lex);
	SetJsonColor(laferJson[i++],lex);
	SetJsonColor(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	lex.SetReturnSymbol(";");	
	string word = lex.GetWord();
	while (word!=""&&word!=";")
	{
		SetJsonValue(laferJson[i++],lex);
		SetJsonValue(laferJson[i++],lex);
		word = lex.GetWord(false);
	}
	lex.SetReturnSymbol(NULL);
}

PMVoid PMLaferLexer::ParsePie( TJSON &laferJson,TWordLex &lex )
{
	PMInt i=1;
	laferJson[0]=PMENTITY_PIE; 
	SetJsonValue(laferJson[i++],lex);
	SetJsonColor(laferJson[i++],lex);
	SetJsonColor(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
	SetJsonValue(laferJson[i++],lex);
}
HINSTANCE ScreenGDI::mHinstance;
ScreenGDI::ScreenGDI():mHWnd(NULL),mhPen(NULL),mhFont(NULL),
					mhBrush(NULL),mOldBrushColor(0),mVAxtra(0),mHAxtra(0)
{
	
}


PMVoid ScreenGDI::Initialize( HWND hWnd,const RECT &ClientRect )
{
	ReleaseBuffer();
	mHWnd= hWnd;
	mHdc =::GetDC(hWnd);
	RECT RectWin;
	GetClientRect(mHWnd,&RectWin);
	Assign(mWindowRect,RectWin);
	Assign(mRect,ClientRect);
	InitBuffer();
	mNullPen = (HPEN)GetStockObject(NULL_PEN);
	mNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
}

ScreenGDI::~ScreenGDI(PMVoid)
{
	ReleaseBuffer();
}

PMVoid ScreenGDI::SetCurrentDC( HDC currentDC )
{
	mCurrentHDC = currentDC;
}

HDC ScreenGDI::GetDC()
{
	return mCurrentHDC;
}

PMVoid ScreenGDI::SetPen( PMColor color,PMInt width ,PMInt style )
{
	if(!mOldPendata.Equal(mCurrentHDC,color,width,style))
	{
		mOldPendata.Set(mCurrentHDC,color,width,style);
		HPEN pen = mhPen;	
		LOGBRUSH lb;
		lb.lbStyle = BS_SOLID;
		lb.lbColor = color;
		lb.lbHatch = 1;
		mhPen = ExtCreatePen(PS_GEOMETRIC | PS_ENDCAP_SQUARE |style, width, &lb, 0, NULL); 
		SelectObject(mCurrentHDC,mhPen);
		if(pen)DeleteObject(pen);
	}else
	{
		SelectObject(mCurrentHDC,mhPen);
	}
}

PMVoid ScreenGDI::SetHatchPen( PMColor color,PMInt width ,PMInt style )
{
	mOldPendata.Set(mCurrentHDC,color,width,style);
	HPEN pen = mhPen;	
	LOGBRUSH lb;
	lb.lbStyle = BS_HATCHED;
	lb.lbColor = color;
	lb.lbHatch = style; 
	mhPen = ExtCreatePen(PS_GEOMETRIC , width, &lb, 0, NULL); 
	SelectObject(mCurrentHDC,mhPen);
	if(pen)DeleteObject(pen);
}

PMVoid ScreenGDI::SetSystemFont()
{	
	HFONT hfont = mhFont;
	SIZE  size;
	mhFont =CreateFontA(13,0,0,2700,10,false,false,false,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"宋体");
	SelectObject(mCurrentHDC,mhFont);
	SetTextColor(mCurrentHDC,RGB(39,65,62));
	if(hfont)DeleteObject(hfont);
	GetTextExtentPoint(mCurrentHDC," ",1,&size);
	mFontWidth  = size.cx;
	mFontHeight = size.cy;
}

PMVoid ScreenGDI::SetFont( TJSON *jnFont )
{
	if(jnFont->IsInt())
	{
		SetTextColor(mCurrentHDC,jnFont->ToInt());
		return;
	}
//	0字体颜色
//	或者
// 	[
// 		"宋体",//字体名称:face name
// 		12,//字体大小
// 		1,//粗体
// 		0,//倾斜
// 		0,//删除线
// 		1,//下划线
// 		0//字体颜色
// 	]
	mJnFont = jnFont;
	HFONT	hfont = mhFont;
	SIZE	size;
	PMLong  lfHeight		= jnFont->At(1).ToInt();
	PMLong  lfWeight		= jnFont->At(2).ToBool()?FW_BOLD:0; 	 
	PMByte  lfItalic		= jnFont->At(3).ToBool();
	PMByte  lfUnderline		= jnFont->At(5).ToBool();
	PMByte  lfStrikeOut		= jnFont->At(4).ToBool(); 
	PMColor lfColor			= jnFont->At(6).ToInt(); 
	PMChar *lfFaceName		= jnFont->At(0).ToCString(); 
	mhFont =CreateFontA(lfHeight,0,0,0,lfWeight,lfItalic,lfUnderline,lfStrikeOut,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,lfFaceName);
	SelectObject(mCurrentHDC,mhFont);
	SetTextColor(mCurrentHDC,lfColor);
	if(hfont)DeleteObject(hfont);
	GetTextExtentPoint(mCurrentHDC," ",1,&size);
	mFontWidth  = size.cx;
	mFontHeight = size.cy + mVAxtra;
	CalcFontWidth();
}

PMVoid ScreenGDI::CalcFontWidth()
{
	SIZE	size;
	PMChar  word[4]={0};
	for (PMChar c = 1; c<127;c++)
	{
		word[0]=c;
		word[1]=0;
		GetTextExtentPoint(mCurrentHDC,word,strlen(word),&size);
		mFontTags[c] =size.cx;
	}
	mFontTags['	'] = TABLENGHT * mFontWidth;
	GetTextExtentPoint(mCurrentHDC,"黎",strlen("黎"),&size);
	mWideFontWidth =size.cx;
}

PMInt ScreenGDI::GetStringWidth( const PMChar *data,PMInt len )
{
	PMInt Result = 0;
	PMInt i = 0;
	while (i<len)
	{
		if (data[i]<0)
		{
			Result += mWideFontWidth;
			i+=2;
		}else
		{
			Result += mFontTags[data[i]];
			i++;
		}
	}
	return Result;
}

PMVoid ScreenGDI::SetNullPen()
{
	SelectObject(mCurrentHDC,mNullPen);
}

PMVoid ScreenGDI::SetBrush( PMColor color )
{
	//if(mOldBrushColor!=color)
	//{
		mOldBrushColor = color;
		HBRUSH brush = mhBrush;	
		mhBrush = CreateSolidBrush(color);
		SelectObject(mCurrentHDC,mhBrush);
		if(brush)DeleteObject(brush);
	//}	
}

PMVoid ScreenGDI::SetHatchBrush( PMInt fnStyle,PMColor color /*= RGB(255,255,255)*/ )
{
	mOldBrushColor = color;
	HBRUSH brush = mhBrush;	
	mhBrush = CreateHatchBrush(fnStyle,color);
	SelectObject(mCurrentHDC,mhBrush);
	if(brush)DeleteObject(brush);
}

PMVoid ScreenGDI::SetPictureBrush( PMBitmap hbmp )
{
	HBRUSH brush = mhBrush;	
	mhBrush = CreatePatternBrush(hbmp);
	SelectObject(mCurrentHDC,mhBrush);
	if(brush)DeleteObject(brush);
}

PMVoid ScreenGDI::WindowResize( const RECT &clientRect )
{
	ReleaseBuffer();
	mHdc = ::GetDC(mHWnd);
	RECT RectWin;
	GetClientRect(mHWnd,&RectWin);
	Assign(mWindowRect,RectWin);
	Assign(mRect,clientRect);
	InitBuffer();
}

PMVoid ScreenGDI::WindowResize( const IntRect &clientRect )
{
	ReleaseBuffer();
	mHdc = ::GetDC(mHWnd);
	RECT RectWin;
	GetClientRect(mHWnd,&RectWin);
	Assign(mWindowRect,RectWin);
	mRect=clientRect;
	InitBuffer();
}

PMVoid ScreenGDI::DrawFrame( const IntRect *rect/*=NULL*/ )
{
	const IntRect *clipRect = rect?rect:&mRect;
	BitBlt(mHdc,clipRect->left,clipRect->top,clipRect->right,clipRect->bottom,mCurrentHDC,0,0,SRCCOPY);
}

PMVoid ScreenGDI::DrawLine( const IntPoint &start,const IntPoint &end )
{
	POINT oldPoint;
	MoveToEx(mCurrentHDC,start.x,start.y,&oldPoint);
	LineTo(mCurrentHDC,end.x,end.y);
}

PMVoid ScreenGDI::DrawLine( const IntPoints &points )
{
	if (points.size()>=2)
	{
		POINT oldPoint;
		IntPoints::const_iterator iter = points.begin();
		MoveToEx(mCurrentHDC,iter->x,iter->y,&oldPoint);
		for (iter++;iter!=points.end();iter++)
		{
			LineTo(mCurrentHDC,iter->x,iter->y);
		}		
	}
}

PMVoid ScreenGDI::DrawDoubleLine( const IntPoint &start,const IntPoint &end )
{
	IntPoint ptStart	= PMSpaceAlgorithm::GetPerpendicularPoint(start,end,0.5);
	IntPoint ptEnd		= PMSpaceAlgorithm::GetPerpendicularPoint(end,start,0.5);
	DrawLine(ptStart,ptEnd);
	ptStart				= PMSpaceAlgorithm::GetPerpendicularPoint(start,end,-0.5);
	ptEnd				= PMSpaceAlgorithm::GetPerpendicularPoint(end,start,-0.5);
	DrawLine(ptStart,ptEnd);
}

PMVoid ScreenGDI::InitBuffer()//mRect必须是从0,0开始
{
	mHdcBitmap			= CreateCompatibleDC(mHdc);	
	mHdcPicture			= CreateCompatibleDC(mHdc);	
	mCurrentHDC			= mHdcBitmap;
	mBitmap				= CreateCompatibleBitmap(mHdc,mRect.width(),mRect.height());	
	mhWhiteBrush		= (HBRUSH)	GetStockObject(WHITE_BRUSH);
	mhWhitePen			= (HPEN)	GetStockObject(WHITE_PEN);
	mhSystemFont		= (HFONT)	GetStockObject(SYSTEM_FIXED_FONT);
	mOldBitmap			= (HBITMAP) SelectObject(mCurrentHDC,mBitmap);	
	mhOldBrush			= (HBRUSH)	SelectObject(mCurrentHDC,mhWhiteBrush);
	mhOldPen			= (HPEN)	SelectObject(mCurrentHDC,mhWhitePen);
	mhOldFont			= (HFONT)	SelectObject(mCurrentHDC,mhSystemFont);
	mhOldPictureBrush	= (HBRUSH)	SelectObject(mCurrentHDC,mhWhiteBrush);
	mhOldPicturePen		= (HPEN)	SelectObject(mCurrentHDC,mhWhitePen);
	Rectangle(mHdcBitmap,mRect.left,mRect.top,mRect.right,mRect.bottom);
	SetBkMode(mCurrentHDC,TRANSPARENT);
	//SetBkMode(mHdc,TRANSPARENT);
}

PMVoid ScreenGDI::ReleaseBuffer()
{	
	SelectObject(mHdcPicture,mhOldPictureBrush);
	SelectObject(mHdcPicture,mhOldPicturePen);
	SelectObject(mHdcBitmap,mhOldBrush);
	SelectObject(mHdcBitmap,mhOldPen);
	SelectObject(mHdcBitmap,mOldBitmap);
	DeleteObject(mBitmap);
	DeleteDC(mHdcBitmap);
	DeleteDC(mHdcPicture);
	ReleaseDC(mHWnd,mHdc);
	if(mhPen)
	{
		DeleteObject(mhPen);
		mhPen=NULL;
	}
	if(mhBrush)
	{
		DeleteObject(mhBrush);
		mhBrush=NULL;
	}
	if(mhFont)
	{
		DeleteObject(mhFont);
		mhFont=NULL;
	}
}

PMVoid ScreenGDI::FillRetangle( const IntRect &rect )
{
	//HPEN pen = CreatePen(0,1,mOldBrushColor);
	SelectObject(mCurrentHDC,mNullPen);
	Rectangle(mCurrentHDC,rect.left,rect.top,rect.right+1,rect.bottom+1);//不知道为什么，Rectangle总是在下一个顶点少画1
	SelectObject(mCurrentHDC,mhPen);
	//DeleteObject(pen);
}

PMVoid ScreenGDI::DrawRetangle( const IntRect &rect )
{
	POINT oldPoint;
	MoveToEx(mCurrentHDC,rect.left,rect.top,&oldPoint);
	LineTo(mCurrentHDC,rect.right,rect.top);
	LineTo(mCurrentHDC,rect.right,rect.bottom);
	LineTo(mCurrentHDC,rect.left,rect.bottom);
	LineTo(mCurrentHDC,rect.left,rect.top);
}

PMVoid ScreenGDI::Retangle( const IntRect &rect )
{
	Rectangle(mCurrentHDC,rect.left,rect.top,rect.right+1,rect.bottom+1);//不知道为什么，Rectangle总是在下一个顶点少画1
}

PMVoid ScreenGDI::ShowNumber( PMInt number,PMInt group/*=0*/ )
{
	string str = PMSpaceAlgorithm::itoa(number);
	//::TextOutA(mCurrentHDC,33,20+group*18,str.c_str(),str.size());
	::TextOutA(mHdc,33,20+group*18,str.c_str(),str.size());
}

PMVoid ScreenGDI::TextOutFormat( const PMChar *data,PMInt len,const IntRect &rect,PMInt format )
{
	PMInt x= rect.left,y= rect.top;
	SIZE  size;
	GetTextExtentPoint(mCurrentHDC,data,len,&size);
	if(format&TF_VCENTER)
	{
		y = (rect.top + rect.bottom)/ 2 - size.cy/2;
	}else if(format&TF_VBOTTOM)
	{
		y = rect.bottom - size.cy;
	}

	if(format&TF_HCENTER)
	{
		x = (rect.left + rect.right)/2 -  size.cx/2;
	}else if(format&TF_HRIGHT)
	{
		x = rect.right -  size.cx;
	}
	::TextOutA(mCurrentHDC,x,y,data,len);
}

PMVoid ScreenGDI::DrawPicture( const PMPicture *picture,const IntRect &rect )
{
	HGDIOBJ hOldObject = SelectObject(mHdcPicture,picture->mhBitmap);
	BitBlt(mCurrentHDC,rect.left,rect.top,rect.width()+1,rect.height()+1,mHdcPicture,0,0,SRCCOPY);
	SelectObject(mHdcPicture,hOldObject);
}

PMPicture* ScreenGDI::CreatePictur( const IntSize &pictureSize,PMColor color )
{
	PMPicture *pPicture = new PMPicture;
	HBITMAP hbitmap = CreateCompatibleBitmap(mHdc,pictureSize.width,pictureSize.height);	
	HBITMAP holdbitmap = (HBITMAP)SelectObject(mHdcPicture,hbitmap);
	HBRUSH brush		= (HBRUSH)CreateSolidBrush(color);
	HBRUSH oldBrush		= (HBRUSH) SelectObject(mHdcPicture,brush);
	Rectangle(mHdcPicture,-1,-1,pictureSize.width+1,pictureSize.height+1)	;	
	SelectObject(mHdcPicture,oldBrush);	
	SelectObject(mHdcPicture,holdbitmap);	
	DeleteObject(brush);
	pPicture->mhBitmap = hbitmap;
	pPicture->mSize = pictureSize;
	return pPicture;
}

PMPicture* ScreenGDI::CreateLaferPicture( const IntSize &pictureSize, const string &laferdata,const string &name )
{
	PMLaferLexer Lexer;
	TJSON *pLaferJson = Lexer.CreateLaferJson(laferdata);
	PMPicture* picture = CreateLaferPicture(pictureSize,*pLaferJson,name);
	TJSON::DeleteJSON(pLaferJson);
	return picture;
}

PMPicture* ScreenGDI::CreateLaferPicture( const IntSize &pictureSize,TJSON &laferJson,const string &name )
{	
	
	TJSON* jnFrame = laferJson.Find(name);
	if(jnFrame)
	{
		PushPicture(pictureSize);
		DrawLaferBody(*jnFrame);
		return PopPicture();
	}
	return NULL;
}

PMPicture* ScreenGDI::CreateLaferFillPicture( const IntSize &pictureSize, TJSON &laferJson,const string &name )
{
	TJSON* jnFrame = laferJson.Find(name);
	PMPicture *picture = NULL;
	if(jnFrame)
	{
		PushPicture(pictureSize);
		TJSON* jnFill = jnFrame->Find(GRID_CONST_FILL);
		if(jnFill)
		{
			DrawLaferObjects(*jnFill);
		}
		picture = PopPicture();
		//picture->mJson = jnFrame->Find(GRID_CONST_LINE);
	}
	return picture;
}

PMVoid ScreenGDI::DrawLaferFrame( TJSON &laferJson,const IntRect &laferRect, const string &name )
{
	mLaferRect = laferRect;
	TJSON* jnFrame = laferJson.Find(name);
	if(jnFrame)
	{
		DrawLaferBody(*jnFrame);
	}
}

PMVoid ScreenGDI::DrawLaferLineFrame( TJSON &laferJson,const IntRect &laferRect, const string &name )
{
	mLaferRect = laferRect;
	TJSON* jnFrame = laferJson.Find(name);
	if(jnFrame)
	{
		TJSON* jnLine = jnFrame->Find(GRID_CONST_LINE);
		if(jnLine)
		{
			DrawLaferObjects(*jnLine);
		}
	}
}

TJSON * ScreenGDI::CreateLaferJson( const string &laferdata )
{
	PMLaferLexer lfLexer;	
	return lfLexer.CreateLaferJson(laferdata);
}

std::string ScreenGDI::CreateLaferCode( TJSON &laferJson )
{
	PMLaferLexer lfLexer;	
	return lfLexer.CreateLaferCode(laferJson);
}

IntRect ScreenGDI::GetRect( TJSON &laferJson,const string &name )
{
	IntRect rect;
	TJSON *json = laferJson.Find(name);
	if(json)
	{
		rect.left	= GetValueX((*json)[0]);
		rect.top	= GetValueY((*json)[1]);
		rect.right	= GetValueX((*json)[2]);
		rect.bottom = GetValueY((*json)[3]);
	}
	return rect;
}

PMInt ScreenGDI::GetValueX( TJSON &laferJson )
{
	if(laferJson.IsArray())
	{		 
		TJSON::iterator it = laferJson.begin();
		PMInt data = GetValueX(*it);
		PMDouble x ;
		for (it++; it != laferJson.end(); it++)
		{
			x =it->ToDouble();
			if(x>=1||x<=-1)
			{
				data+=(PMInt)x;
			}else
			{
				data+=(PMInt)(x*mLaferRect.width());
			}
		}
		return data;
	}else
	{
		PMDouble x =laferJson.ToDouble();
		if((PMInt)laferJson.GetDefine()==1)
		{
			return mLaferRect.right;
		}else if(x>=1)
		{
			return mLaferRect.left+(PMInt)x;
		}else if(x<=-1)
		{
			return mLaferRect.right+(PMInt)x;
		}else if(x>=0)
		{
			return mLaferRect.left+(PMInt)(x*mLaferRect.width());
		}else
		{
			return mLaferRect.right+(PMInt)(x*mLaferRect.width());
		}
	}
}

PMInt ScreenGDI::GetValueY( TJSON &laferJson )
{
	if(laferJson.IsArray())
	{		 
		TJSON::iterator it = laferJson.begin();
		PMInt data = GetValueY(*it);
		PMDouble x ;
		for (it++; it != laferJson.end(); it++)
		{
			x =it->ToDouble();
			if(x>=1||x<=-1)
			{
				data+=(PMInt)x;
			}else
			{
				data+=(PMInt)(x*mLaferRect.height());
			}
		}
		return data;
	}else
	{
		PMDouble x =laferJson.ToDouble();
		if((PMInt)laferJson.GetDefine()==1)
		{
			return mLaferRect.bottom;
		}else if(x>=1)
		{
			return mLaferRect.top+(PMInt)x;
		}else if(x<=-1)
		{
			return mLaferRect.bottom+(PMInt)x;
		}else if(x>=0)
		{
			return mLaferRect.top+(PMInt)(x*mLaferRect.height());
		}else
		{
			return mLaferRect.bottom+(PMInt)(x*mLaferRect.height());
		}
	}
}

PMInt ScreenGDI::GetWidthValue( TJSON &laferJson)
{
	PMDouble x =laferJson.ToDouble();
	if(x>=1)
	{
		return (PMInt)x;
	}else if(x<=-1)
	{
		return (PMInt)-x;
	}else if(x>=0)
	{
		return (PMInt)(x*mLaferRect.width());
	}else
	{
		return (PMInt)(-x*mLaferRect.height());
	}
}

PMVoid ScreenGDI::DrawLaferBody( TJSON &laferJson )
{
	for (TJSON::iterator it = laferJson.begin(); it != laferJson.end(); it++)
	{
		DrawLaferObjects(*it);
	}	
}

PMVoid ScreenGDI::DrawLaferObjects( TJSON &laferJson )
{
	for (TJSON::iterator iter = laferJson.begin(); iter != laferJson.end(); iter++)
	{
		TJSON &json = (*iter);
		switch(json[0].ToInt())
		{
		case PMENTITY_POINT:
			DrawLaferPoint(json);
			break;
		case PMENTITY_LINE:
			DrawLaferLine(json);
			break;
		case PMENTITY_POLYLINE:
			DrawLaferPolyLine(json);
			break;
		case PMENTITY_TRIANGLE:
			DrawLaferTriangle(json);
			break;
		case PMENTITY_RECTANGLE:
			DrawLaferRectangle(json);
			break;
		case PMENTITY_POLYGON:
			DrawLaferPolygon(json);
			break;
		case PMENTITY_ELLIPSE:
			DrawLaferEllipse(json);
			break;
		case PMENTITY_PIE:
			DrawLaferPie(json);
			break;
		case PMENTITY_SMOOTINGRECT:
			DrawLaferSmootingRect(json);
			break;
		}
	}
}

PMVoid ScreenGDI::DrawLaferPoint( TJSON &laferJson )
{
	PMInt painResult = SetParameter(laferJson[1],laferJson[2],&laferJson[2]);
	if(painResult==1||painResult==3)
	{
		PMInt w = laferJson[1].ToInt()/2;
		PMInt centerX = GetValueX(laferJson[3]);
		PMInt centerY = GetValueY(laferJson[4]);
		if(w==0)
		{
			SetPixel(mCurrentHDC,centerX,centerY,laferJson[2].ToInt());
		}else
		{
			Ellipse(mCurrentHDC,centerX-w,centerY-w,centerX+w,centerY+w);
		}
		
	}	
}

PMVoid ScreenGDI::DrawLaferLine( TJSON &laferJson )
{
	if(SetParameter(laferJson[1],laferJson[2]))
	{
		POINT point;
// 		PMInt x=GetValueX(laferJson[3]),y=GetValueY(laferJson[4]);
// 		PMInt x1=GetValueX(laferJson[5]),y1=GetValueY(laferJson[6]);
		MoveToEx(mCurrentHDC,GetValueX(laferJson[3]),GetValueY(laferJson[4]),&point);
		LineTo(mCurrentHDC,GetValueX(laferJson[5]),GetValueY(laferJson[6]));
	}	
}

PMVoid ScreenGDI::DrawLaferPolyLine( TJSON &laferJson )
{
	if(SetParameter(laferJson[1],laferJson[2]))
	{
		POINT point;
		MoveToEx(mCurrentHDC,GetValueX(laferJson[3]),GetValueY(laferJson[4]),&point);
		TJSON::iterator end = laferJson.end(),endLast = laferJson.end() -1;
		for (TJSON::iterator it = laferJson.begin()+5; it!=end&&it!=endLast;it=it+2)
		{
			LineTo(mCurrentHDC,GetValueX(*it),GetValueY(*(it+1)));
		}
	}	
}

PMVoid ScreenGDI::DrawLaferRectangle( TJSON &laferJson )
{
	PMInt painResult = SetParameter(laferJson[1],laferJson[2],&laferJson[3]);//0、画NONE。1、画边框。2、填充。3、画边框并填充
	if(painResult==2)
	{
		Rectangle(mCurrentHDC,GetValueX(laferJson[4]),GetValueY(laferJson[5]),
			GetValueX(laferJson[6])+2,GetValueY(laferJson[7])+2);					//不知道为什么，总是会少画2
	}else if(painResult>0)
	{
		Rectangle(mCurrentHDC,GetValueX(laferJson[4]),GetValueY(laferJson[5]),
			GetValueX(laferJson[6])+1,GetValueY(laferJson[7])+1);					//不知道为什么，总是会少画1
	}
}

PMVoid ScreenGDI::DrawLaferSmootingRect( TJSON &laferJson )
{
	PMInt colorFrom = laferJson[2].ToInt();
	PMInt colorTo	= laferJson[3].ToInt();
	PMInt R=GetRValue(colorFrom),G=GetGValue(colorFrom),B=GetBValue(colorFrom);
	PMInt r=GetRValue(colorTo),g=GetGValue(colorTo),b=GetBValue(colorTo);
	IntRect rect(GetValueX(laferJson[4]),GetValueY(laferJson[5]),GetValueX(laferJson[6]),GetValueY(laferJson[7]));
	HPEN pen = CreatePen(PS_SOLID,1,colorFrom);
	HPEN oldPen = (HPEN)SelectObject(mCurrentHDC,pen);
	POINT pt;
	if((PMInt)laferJson[1].GetDefine()==1)//竖直方向渐变
	{		
		PMInt width = rect.width();		
		PMDouble dr=((PMDouble)r-R)/width,dg=((PMDouble)g-G)/width,db=((PMDouble)b-B)/width;
		for (int i=0;i<=width;i++)
		{
			MoveToEx(mCurrentHDC,rect.left+i,rect.top,&pt);
			LineTo(mCurrentHDC,rect.left+i,rect.bottom+1);
			DeleteObject(pen);
			pen = CreatePen(0,1,RGB(R+dr*i,G+dg*i,B+db*i));
			SelectObject(mCurrentHDC,pen);
		}		
	}else								 //水平方向渐变
	{
		PMInt height = rect.height();		
		PMDouble dr=((PMDouble)r-R)/height,dg=((PMDouble)g-G)/height,db=((PMDouble)b-B)/height;
		for (int i=0;i<=height;i++)
		{
			MoveToEx(mCurrentHDC,rect.left,rect.top+i,&pt);
			LineTo(mCurrentHDC,rect.right+1,rect.top+i);
			DeleteObject(pen);
			pen = CreatePen(0,1,RGB(R+dr*i,G+dg*i,B+db*i));
			SelectObject(mCurrentHDC,pen);
		}	
	}
	DeleteObject(pen);
	SelectObject(mCurrentHDC,oldPen);
}

PMVoid ScreenGDI::DrawLaferTriangle( TJSON &laferJson )
{
	DrawLaferPolygon(laferJson);
}

PMVoid ScreenGDI::DrawLaferPolygon( TJSON &laferJson )
{
	if(SetParameter(laferJson[1],laferJson[2],&laferJson[3]))
	{
		PMInt childCount = (laferJson.GetChildCount()-4)/2;
		if(childCount>=3)
		{
			POINT *pts = new POINT[childCount];
			TJSON::iterator end = laferJson.end(),endLast = laferJson.end() -1;
			childCount=0;
			for (TJSON::iterator it = laferJson.begin()+4; it!=end&&it!=endLast;it=it+2)
			{
				pts[childCount].x = GetValueX(*it);
				pts[childCount++].y = GetValueY(*(it+1));
			}	
			Polygon(mCurrentHDC,pts,childCount);
			delete[] pts;
		}				
	}
}

PMVoid ScreenGDI::DrawLaferEllipse( TJSON &laferJson )
{
	PMInt painResult = SetParameter(laferJson[1],laferJson[2],&laferJson[3]);//0、画NONE。1、画边框。2、填充。3、画边框并填充
	if(painResult==2)
	{
		Ellipse(mCurrentHDC,GetValueX(laferJson[4]),GetValueY(laferJson[5]),
			GetValueX(laferJson[6])+2,GetValueX(laferJson[7])+2);					//不知道为什么，总是会少画2
	}else if(painResult>0)
	{
		Ellipse(mCurrentHDC,GetValueX(laferJson[4]),GetValueY(laferJson[5]),
			GetValueX(laferJson[6])+1,GetValueY(laferJson[7])+1);					//不知道为什么，总是会少画1
	}
}

PMVoid ScreenGDI::DrawLaferPie( TJSON &laferJson )
{
	if(SetParameter(laferJson[1],laferJson[2],&laferJson[3]))
	{
		Pie(mCurrentHDC,GetValueX(laferJson[4]),GetValueY(laferJson[5]),GetValueX(laferJson[6]),GetValueY(laferJson[7]),
			GetValueX(laferJson[8]),GetValueY(laferJson[9]),GetValueX(laferJson[10]),GetValueY(laferJson[11]));					
	}	
}


PMInt ScreenGDI::SetParameter( TJSON &wJson,TJSON &cJson,TJSON *bgcJson )
{
	PMInt painResult = 0;//0、画NONE。1、画边框。2、填充。3、画边框并填充
	PMInt w = GetWidthValue(wJson);
	if(w!=0)
	{
		if((PMInt)cJson.GetDefine()!=1)
		{
			SetPen(cJson.ToInt(),w);
			painResult = 1;
		}else
		{
			SelectObject(mCurrentHDC,mNullPen);
		}		
	}else if(bgcJson)
	{
		SelectObject(mCurrentHDC,mNullPen);
	}
	if(bgcJson)
	{	
		if((PMInt)bgcJson->GetDefine()!=1)
		{
			SetBrush(bgcJson->ToInt());
			painResult = painResult==1?3:2;
		}else if(painResult == 1)
		{
			SelectObject(mCurrentHDC,mNullBrush);
		}
	}
	return painResult;
}

PMVoid ScreenGDI::PushPicture(const IntSize &pictureSize)
{
	PMPicture *pPicture = new PMPicture;
	mObjects[0] = pPicture;
	pPicture->mSize = pictureSize;
	HBITMAP hbitmap = CreateCompatibleBitmap(mHdc,pPicture->mSize.width,pPicture->mSize.height);	
	HBITMAP holdbitmap = (HBITMAP)SelectObject(mHdcPicture,hbitmap);
	mObjects[1] = holdbitmap;	
	pPicture->mhBitmap = hbitmap;	
	mObjects[2] = mCurrentHDC;
	mCurrentHDC= mHdcPicture;
	mObjects[3] = (PMVoid*)mOldPendata.mColor;
	mObjects[4] = (PMVoid*)mOldPendata.mWidth;
	mObjects[5] = (PMVoid*)mOldPendata.mStyle;
	mObjects[6] = mhPen;
	mObjects[7] = (PMVoid*)mOldBrushColor;
	mObjects[8] = mhBrush;
	mObjects[9] = (PMVoid*)mLaferRect.left;
	mObjects[10] = (PMVoid*)mLaferRect.top;
	mObjects[11] = (PMVoid*)mLaferRect.right;
	mObjects[12] = (PMVoid*)mLaferRect.bottom;
	mObjects[13] = (PMVoid*)mOldPendata.mHDC;

	mObjects[14] = (PMVoid*)mhFont;

	mLaferRect.set(0,0,pPicture->mSize.width-1,pPicture->mSize.height-1); //从0开始必须-1
	mhPen = NULL;
	mhBrush = NULL;
}

PMPicture* ScreenGDI::PopPicture()
{
	SelectObject(mHdcPicture,(HBITMAP)mObjects[1]);	
	mCurrentHDC = (HDC)mObjects[2];
	if(mhPen)  DeleteObject(mhPen);
	if(mhFont) DeleteObject(mhFont);
	if(mhBrush)DeleteObject(mhBrush);
	mOldPendata.mColor		 = (PMColor)mObjects[3];
	mOldPendata.mWidth		 = (PMInt)  mObjects[4];
	mOldPendata.mStyle		 = (PMInt)  mObjects[5];
	mhPen					 = (HPEN)   mObjects[6];
	mOldBrushColor			 = (PMColor)mObjects[7];
	mhBrush					 = (HBRUSH) mObjects[8];

	mLaferRect.left			 = (PMInt)  mObjects[9];
	mLaferRect.top			 = (PMInt)  mObjects[10];
	mLaferRect.right		 = (PMInt)  mObjects[11];
	mLaferRect.bottom		 = (PMInt)  mObjects[12];
	mOldPendata.mHDC		 = (HDC)    mObjects[13];
	mhFont					 = (HFONT)  mObjects[14];
	return (PMPicture*)mObjects[0];
}

PMVoid ScreenGDI::LineExactly( PMInt x1,PMInt y1,PMInt x2,PMInt y2,PMInt width )
{
	int dx1;
	int dx2;
	if(width%2==0)
	{
		dx1=width/2;
		dx2=dx1-1;
	}else
	{
		dx2=width/2;
		dx1=dx2;
	}
	if(x1==x2)
	{
		if(y1<y2)
		{
			y1+=dx1;
			y2-=dx2;
		}else
		{
			y2+=dx1;
			y1-=dx2;
		}
	}else
	{
		if(x1<x2)
		{
			x1+=dx1;
			x2-=dx2;
		}else
		{
			x2+=dx1;
			x1-=dx2;
		}		
	}	
	POINT ptt;
	MoveToEx(mCurrentHDC,x1,y1,&ptt);
	LineTo(mCurrentHDC,x2,y2);
}

PMVoid ScreenGDI::SetInstance( HINSTANCE hInstance )
{
	mHinstance = hInstance;
}

PMVoid ScreenGDI::SetTextAxtra( PMInt VAxtra,PMInt HAxtra )
{
	mVAxtra = VAxtra;
	mHAxtra = HAxtra;
	SetTextCharacterExtra(mCurrentHDC,HAxtra);
}

PMVoid ScreenGDI::ClearBackGround(PMInt iColor)
{
	HBRUSH hBrush;
	HPEN   hPen;
	if(iColor == -1)
	{
		hBrush = mhWhiteBrush;
		hPen   = mhWhitePen;
	}else
	{
		hBrush = CreateSolidBrush(iColor);
		hPen   = CreatePen(PS_SOLID,1,iColor);
	}
	HBRUSH hOldBrush		= (HBRUSH)	SelectObject(mCurrentHDC,hBrush);
	HPEN hOldPen			= (HPEN)	SelectObject(mCurrentHDC,hPen);
	Rectangle(mCurrentHDC,mRect.left,mRect.top,mRect.right,mRect.bottom);
	SelectObject(mCurrentHDC,hOldBrush);
	SelectObject(mCurrentHDC,hOldPen);
	if(iColor!=-1)
	{
		DeleteObject(hBrush);
		DeleteObject(hPen);
	}
}


PMBool PenData::Equal(HDC hdc, PMColor iColor, PMInt iWidth, PMInt iStyle )
{
	return mHDC==hdc &&iColor==mColor&&iWidth==mWidth&&iStyle==mStyle;
}

PMVoid PenData::Set(HDC hdc, PMColor iColor, PMInt iWidth, PMInt iStyle )
{
	mHDC   = hdc;
	mColor = iColor;
	mWidth = iWidth;
	mStyle = iStyle;
}

PMColor TColorFormula::GetColor( PMColor sourceColor )
{
	BYTE R =GetRValue(sourceColor);
	BYTE G =GetGValue(sourceColor);
	BYTE B =GetBValue(sourceColor);
	R = R + RBase - R*RNumerator/255;
	G = G + GBase - G*GNumerator/255;
	B = B + BBase - B*BNumerator/255;
	return RGB(R,G,B);
}

PMVoid TColorFormula::Initialize( TJSON *json )
{
	if(json)
	{
		RBase = json->At(0).ToInt();
		RNumerator = json->At(1).ToInt();
		GBase = json->At(2).ToInt();
		GNumerator = json->At(3).ToInt();
		BBase = json->At(4).ToInt();
		BNumerator = json->At(5).ToInt();
	}
}

TColorFormula * TColorFormula::GetSingleton()
{
	static TColorFormula ColorFormula;
	return &ColorFormula;
}
