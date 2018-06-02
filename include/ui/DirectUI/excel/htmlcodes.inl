
#include <windows.h>
typedef struct _ColTable {
  char *szName;
  COLORREF colVal;
}ColTable;
static ColTable g_ColTable[] =
  { {"AliceBlue" , RGB( 240, 248, 255 ) },
    {"AntiqueWhite" , RGB( 250, 235, 215 ) },
    {"Aqua" , RGB( 0, 255, 255 ) },
    {"Aquamarine" , RGB( 127, 255, 212 ) },
    {"Azure" , RGB( 240, 255, 255 ) },
    {"Beige" , RGB( 245, 245, 220 ) },
    {"Bisque" , RGB( 255, 228, 196 ) },
    {"Black" , RGB( 0, 0, 0 ) },
    {"BlanchedAlmond" , RGB( 255, 255, 205 ) },
    {"Blue" , RGB( 0, 0, 255 ) },
    {"BlueViolet" , RGB( 138, 43, 226 ) },
    {"Brown" , RGB( 165, 42, 42 ) },
    {"Burlywood" , RGB( 222, 184, 135 ) },
    {"CadetBlue" , RGB( 95, 158, 160 ) },
    {"Chartreuse" , RGB( 127, 255, 0 ) },
    {"Chocolate" , RGB( 210, 105, 30 ) },
    {"Coral" , RGB( 255, 127, 80 ) },
    {"CornflowerBlue" , RGB( 100, 149, 237 ) },
    {"Cornsilk" , RGB( 255, 248, 220 ) },
    {"Crimson" , RGB( 220, 20, 60 ) },
    {"Cyan" , RGB( 0, 255, 255 ) },
    {"DarkBlue" , RGB( 0, 0, 139 ) },
    {"DarkCyan" , RGB( 0, 139, 139 ) },
    {"DarkGoldenRod" , RGB( 184, 134, 11 ) },
    {"DarkGray" , RGB( 169, 169, 169 ) },
    {"DarkGreen" , RGB( 0, 100, 0 ) },
    {"DarkKhaki" , RGB( 189, 183, 107 ) },
    {"DarkMagenta" , RGB( 139, 0, 139 ) },
    {"DarkOliveGreen" , RGB( 85, 107, 47 ) },
    {"DarkOrange" , RGB( 255, 140, 0 ) },
    {"DarkOrchid" , RGB( 153, 50, 204 ) },
    {"DarkRed" , RGB( 139, 0, 0 ) },
    {"DarkSalmon" , RGB( 233, 150, 122 ) },
    {"DarkSeaGreen" , RGB( 143, 188, 143 ) },
    {"DarkSlateBlue" , RGB( 72, 61, 139 ) },
    {"DarkSlateGray" , RGB( 47, 79, 79 ) },
    {"DarkTurquoise" , RGB( 0, 206, 209 ) },
    {"DarkViolet" , RGB( 148, 0, 211 ) },
    {"DeepPink" , RGB( 255, 20, 147 ) },
    {"DeepSkyBlue" , RGB( 0, 191, 255 ) },
    {"DimGray" , RGB( 105, 105, 105 ) },
    {"DodgerBlue" , RGB( 30, 144, 255 ) },
    {"FireBrick" , RGB( 178, 34, 34 ) },
    {"FloralWhite" , RGB( 255, 250, 240 ) },
    {"ForestGreen" , RGB( 34, 139, 34 ) },
    {"Fuchsia" , RGB( 255, 0, 255 ) },
    {"Gainsboro" , RGB( 220, 220, 220 ) },
    {"GhostWhite" , RGB( 248, 248, 255 ) },
    {"Gold" , RGB( 255, 215, 0 ) },
    {"GoldenRod" , RGB( 218, 165, 32 ) },
    {"Gray" , RGB( 127, 127, 127 ) },
    {"Green" , RGB( 0, 128, 0 ) },
    {"GreenYellow" , RGB( 173, 255, 47 ) },
    {"HoneyDew" , RGB( 240, 255, 240 ) },
    {"HotPink" , RGB( 255, 105, 180 ) },
    {"IndianRed" , RGB( 205, 92, 92 ) },
    {"Indigo" , RGB( 75, 0, 130 ) },
    {"Ivory" , RGB( 255, 255, 240 ) },
    {"Khaki" , RGB( 240, 230, 140 ) },
    {"Lavender" , RGB( 230, 230, 250 ) },
    {"LavenderBlush" , RGB( 255, 240, 245 ) },
    {"Lawngreen" , RGB( 124, 252, 0 ) },
    {"LemonChiffon" , RGB( 255, 250, 205 ) },
    {"LightBlue" , RGB( 173, 216, 230 ) },
    {"LightCoral" , RGB( 240, 128, 128 ) },
    {"LightCyan" , RGB( 224, 255, 255 ) },
    {"LightGoldenRodYellow", RGB( 250, 250, 210 ) },
    {"LightGreen" , RGB( 144, 238, 144 ) },
    {"LightGrey" , RGB( 211, 211, 211 ) },
    {"LightPink" , RGB( 255, 182, 193 ) },
    {"LightSalmon" , RGB( 255, 160, 122 ) },
    {"LightSeaGreen" , RGB( 32, 178, 170 ) },
    {"LightSkyBlue" , RGB( 135, 206, 250 ) },
    {"LightSlateGray" , RGB( 119, 136, 153 ) },
    {"LightSteelBlue" , RGB( 176, 196, 222 ) },
    {"LightYellow" , RGB( 255, 255, 224 ) },
    {"Lime" , RGB( 0, 255, 0 ) },
    {"LimeGreen" , RGB( 50, 205, 50 ) },
    {"Linen" , RGB( 250, 240, 230 ) },
    {"Magenta" , RGB( 255, 0, 255 ) },
    {"Maroon" , RGB( 128, 0, 0 ) },
    {"MediumAquamarine" , RGB( 102, 205, 170 ) },
    {"MediumBlue" , RGB( 0, 0, 205 ) },
    {"MediumOrchid" , RGB( 186, 85, 211 ) },
    {"MediumPurple" , RGB( 147, 112, 219 ) },
    {"MediumSeaGreen" , RGB( 60, 179, 113 ) },
    {"MediumSlateBlue" , RGB( 123, 104, 238 ) },
    {"MediumSpringGreen" , RGB( 0, 250, 154 ) },
    {"MediumTurquoise" , RGB( 72, 209, 204 ) },
    {"MediumVioletRed" , RGB( 199, 21, 133 ) },
    {"MidnightBlue" , RGB( 25, 25, 112 ) },
    {"MintCream" , RGB( 245, 255, 250 ) },
    {"MistyRose" , RGB( 255, 228, 225 ) },
    {"Moccasin" , RGB( 255, 228, 181 ) },
    {"NavajoWhite" , RGB( 255, 222, 173 ) },
    {"Navy" , RGB( 0, 0, 128 ) },
    {"OldLace" , RGB( 253, 245, 230 ) },
    {"Olive" , RGB( 128, 128, 0 ) },
    {"OliveDrab" , RGB( 107, 142, 35 ) },
    {"Orange" , RGB( 255, 165, 0 ) },
    {"OrangeRed" , RGB( 255, 69, 0 ) },
    {"Orchid" , RGB( 218, 112, 214 ) },
    {"PaleGoldenRod" , RGB( 238, 232, 170 ) },
    {"PaleGreen" , RGB( 152, 251, 152 ) },
    {"PaleTurquoise" , RGB( 175, 238, 238 ) },
    {"PaleVioletRed" , RGB( 219, 112, 147 ) },
    {"PapayaWhip" , RGB( 255, 239, 213 ) },
    {"PeachPuff" , RGB( 255, 218, 185 ) },
    {"Peru" , RGB( 205, 133, 63 ) },
    {"Pink" , RGB( 255, 192, 203 ) },
    {"Plum" , RGB( 221, 160, 221 ) },
    {"PowderBlue" , RGB( 176, 224, 230 ) },
    {"Purple" , RGB( 128, 0, 128 ) },
    {"Red" , RGB( 255, 0, 0 ) },
    {"RosyBrown" , RGB( 188, 143, 143 ) },
    {"RoyalBlue" , RGB( 65, 105, 225 ) },
    {"SaddleBrown" , RGB( 139, 69, 19 ) },
    {"Salmon" , RGB( 250, 128, 114 ) },
    {"SandyBrown" , RGB( 244, 164, 96 ) },
    {"SeaGreen" , RGB( 46, 139, 87 ) },
    {"SeaShell" , RGB( 255, 245, 238 ) },
    {"Sienna" , RGB( 160, 82, 45 ) },
    {"Silver" , RGB( 192, 192, 192 ) },
    {"SkyBlue" , RGB( 135, 206, 235 ) },
    {"SlateBlue" , RGB( 106, 90, 205 ) },
    {"SlateGray" , RGB( 112, 128, 144 ) },
    {"Snow" , RGB( 255, 250, 250 ) },
    {"SpringGreen" , RGB( 0, 255, 127 ) },
    {"SteelBlue" , RGB( 70, 130, 180 ) },
    {"Tan" , RGB( 210, 180, 140 ) },
    {"Teal" , RGB( 0, 128, 128 ) },
    {"Thistle" , RGB( 216, 191, 216 ) },
    {"Tomato" , RGB( 255, 99, 71 ) },
    {"Turquoise" , RGB( 64, 224, 208 ) },
    {"Violet" , RGB( 238, 130, 238 ) },
    {"Wheat" , RGB( 245, 222, 179 ) },
    {"White" , RGB( 255, 255, 255 ) },
    {"WhiteSmoke" , RGB( 245, 245, 245 ) },
    {"Yellow" , RGB( 255, 255, 0 ) },
    {"YellowGreen" , RGB( 139, 205, 50 ) },
    {NULL, 0}
  };
typedef struct _HTMLCODES {
  char szCode[ 10 ];
  char Code;
}HTMLCODES;
#pragma warning(disable: 4305)
#pragma warning(disable: 4309)
HTMLCODES g_HTMLCodes[] = //
{ {"&quot;", '"'},
{"&amp;", '&'},
{"&lt;", '<'},
{"&gt;", '>'},
{"&nbsp;", ' '},
{"&iexcl;", '?'},
{"&cent;", '?'},
{"&pound;", '?'},
{"&curren;", '?'},
{"&yen;", '?'},
{"&brvbar;", '?'},
{"&sect;", '?'},
{"&uml;", '?'},
{"&copy;", '?'},
{"&ordf;", '?'},
{"&laquo;", '?'},
{"&not;", '?'},
{"&shy;", '?'},
{"&reg;", '?'},
{"&macr;", '?'},
{"&deg;", '?'},
{"&plusmn;", '?'},
{"&sup2;", '?'},
{"&sup3;", '?'},
{"&acute;", '?'},
{"&micro;", '?'},
{"&para;", '?'},
{"&middot;", '?'},
{"&cedil;", '?'},
{"&sup1;", '?'},
{"&ordm;", '?'},
{"&raquo;", '?'},
{"&frac14;", '?'},
{"&frac12;", '?'},
{"&frac34;", '?'},
{"&iquest;", '?'},
{"&Agrave;", '?'},
{"&Aacute;", '?'},
{"&Acirc;", '?'},
{"&Atilde;", '?'},
{"&Auml;", '?'},
{"&Aring;", '?'},
{"&AElig;", '?'},
{"&Ccedil;", '?'},
{"&Egrave;", '?'},
{"&Eacute;", '?'},
{"&Ecirc;", '?'},
{"&Euml;", '?'},
{"&Igrave;", '?'},
{"&Iacute;", '?'},
{"&Icirc;", '?'},
{"&Iuml;", '?'},
{"&ETH;", '?'},
{"&Ntilde;", '?'},
{"&Ograve;", '?'},
{"&Oacute;", '?'},
{"&Ocirc;", '?'},
{"&Otilde;", '?'},
{"&Ouml;", '?'},
{"&times;", '?'},
{"&Oslash;", '?'},
{"&Ugrave;", '?'},
{"&Uacute;", '?'},
{"&Ucirc;", '?'},
{"&Uuml;", '?'},
{"&Yacute;", '?'},
{"&THORN;", '?'},
{"&szlig;", '?'},
{"&agrave;", '?'},
{"&aacute;", '?'},
{"&acirc;", '?'},
{"&atilde;", '?'},
{"&auml;", '?'},
{"&aring;", '?'},
{"&aelig;", '?'},
{"&ccedil;", '?'},
{"&egrave;", '?'},
{"&eacute;", '?'},
{"&ecirc;", '?'},
{"&euml;", '?'},
{"&igrave;", '?'},
{"&iacute;", '?'},
{"&icirc;", '?'},
{"&iuml;", '?'},
{"&eth;", '?'},
{"&ntilde;", '?'},
{"&ograve;", '?'},
{"&oacute;", '?'},
{"&ocirc;", '?'},
{"&otilde;", '?'},
{"&ouml;", '?'},
{"&divide;", '?'},
{"&oslash;", '?'},
{"&ugrave;", '?'},
{"&uacute;", '?'},
{"&ucirc;", '?'},
{"&uuml;", '?'},
{"&yacute;", '?'},
{"&thorn;", '?'},
{"&yuml;", 'ÿ'},
{"&OElig;", 'E'},
{"&oelig;", 'e'},
{"&Scaron;", 'S'},
{"&scaron;", 's'},
{"&Yuml;", 'ÿ'},
{"&fnof;", 'f'},
{"&circ;", '^'},
{"&tilde;", '~'},
{"&ndash;", '-'},
{"&mdash;", '-'},
{"&lsquo;", '?'},
{"&rsquo;", '?'},
{"&sbquo;", ','},
{"&ldquo;", '\"'},
{"&rdquo;", '\"'},
{"&bdquo;", '?'},
{"&dagger;", '?'},
{"&Dagger;", '?'},
{"&bull;", '?'},
{"&hellip;", '?'},
{"&permil;", '?'},
{"&lsaquo;", '?'},
{"&rsaquo;", '?'},
{"&euro;", '€'},
{"&trade;", '?'},
{{( char ) 0x97, ( char ) 0x00}, ' -'},
{{( char ) 0xA0, ( char ) 0x00}, ' '},
{"", 0}
                          };
char *ConvertFromHTMLCodes( char *szText ) {
  int i, j, k;
  for ( k = j = 0;szText[ j ];j++ ) {
    for ( i = 0;g_HTMLCodes[ i ].Code;i++ ) {
      if ( !strncmp( &szText[ j ], g_HTMLCodes[ i ].szCode, strlen( g_HTMLCodes[ i ].szCode ) ) ) {
        szText[ k++ ] = g_HTMLCodes[ i ].Code;
        j += strlen( g_HTMLCodes[ i ].szCode ) - 1;
        break;
      }
    }
    if ( !g_HTMLCodes[ i ].Code ) {
      if ( !strnicmp( &szText[ j ], "&#", 2 ) ) {
        j += 2;
        szText[ k++ ] = atoi( &szText[ j ] );
        while ( szText[ j ] && szText[ j ] != ';' && szText[ j ] != ' ' )
          j++;
        if ( !szText[ j ] )
          break;
      } else
        szText[ k++ ] = szText[ j ];
    }
  }
  szText[ k ] = 0;
  return szText;
}
char *ConvertToHTMLCodes( char *szText ) {
  int nLen = 0;
  int i, j, k;
  char *szRet;
  for ( i = 0;szText[ i ];i++ ) {
    for ( j = 0;g_HTMLCodes[ j ].Code;j++ )
      if ( szText[ i ] == g_HTMLCodes[ j ].Code )
        break;
    if ( g_HTMLCodes[ j ].Code )
      nLen += strlen( g_HTMLCodes[ j ].szCode );
    else
      nLen++;
  }
  szRet = (char*)malloc( nLen + 1 );
  k = 0;
  for ( i = 0;szText[ i ];i++ ) {
    for ( j = 0;g_HTMLCodes[ j ].Code;j++ )
      if ( szText[ i ] == g_HTMLCodes[ j ].Code )
        break;
    if ( g_HTMLCodes[ j ].Code ) {
      strcpy( &szRet[ k ], g_HTMLCodes[ j ].szCode );
      k += strlen( g_HTMLCodes[ j ].szCode );
    } else
      szRet[ k++ ] = szText[ i ];
  }
  szRet[ k ] = 0;
  return szRet;
}
struct _UTF8 {
  unsigned char Code;
  unsigned char szCode[ 3 ];
} g_Utf8[] =//
  { {160, {0xc2, 0xa0, 0}},
    {161, {0xc2, 0xa1, 0}},
    {162, {0xc2, 0xa2, 0}},
    {163, {0xc2, 0xa3, 0}},
    {164, {0xc2, 0xa4, 0}},
    {165, {0xc2, 0xa5, 0}},
    {166, {0xc2, 0xa6, 0}},
    {167, {0xc2, 0xa7, 0}},
    {168, {0xc2, 0xa8, 0}},
    {169, {0xc2, 0xa9, 0}},
    {170, {0xc2, 0xaa, 0}},
    {171, {0xc2, 0xab, 0}},
    {172, {0xc2, 0xac, 0}},
    {173, {0xc2, 0xad, 0}},
    {174, {0xc2, 0xae, 0}},
    {175, {0xc2, 0xaf, 0}},
    {176, {0xc2, 0xb0, 0}},
    {177, {0xc2, 0xb1, 0}},
    {178, {0xc2, 0xb2, 0}},
    {179, {0xc2, 0xb3, 0}},
    {180, {0xc2, 0xb4, 0}},
    {181, {0xc2, 0xb5, 0}},
    {182, {0xc2, 0xb6, 0}},
    {183, {0xc2, 0xb7, 0}},
    {184, {0xc2, 0xb8, 0}},
    {185, {0xc2, 0xb9, 0}},
    {186, {0xc2, 0xba, 0}},
    {187, {0xc2, 0xbb, 0}},
    {188, {0xc2, 0xbc, 0}},
    {189, {0xc2, 0xbd, 0}},
    {190, {0xc2, 0xbe, 0}},
    {191, {0xc2, 0xbf, 0}},
    {192, {0xc3, 0x80, 0}},
    {193, {0xc3, 0x81, 0}},
    {194, {0xc3, 0x82, 0}},
    {195, {0xc3, 0x83, 0}},
    {196, {0xc3, 0x84, 0}},
    {197, {0xc3, 0x85, 0}},
    {198, {0xc3, 0x86, 0}},
    {199, {0xc3, 0x87, 0}},
    {200, {0xc3, 0x88, 0}},
    {201, {0xc3, 0x89, 0}},
    {202, {0xc3, 0x8a, 0}},
    {203, {0xc3, 0x8b, 0}},
    {204, {0xc3, 0x8c, 0}},
    {205, {0xc3, 0x8d, 0}},
    {206, {0xc3, 0x8e, 0}},
    {207, {0xc3, 0x8f, 0}},
    {208, {0xc3, 0x90, 0}},
    {209, {0xc3, 0x91, 0}},
    {210, {0xc3, 0x92, 0}},
    {211, {0xc3, 0x93, 0}},
    {212, {0xc3, 0x94, 0}},
    {213, {0xc3, 0x95, 0}},
    {214, {0xc3, 0x96, 0}},
    {215, {0xc3, 0x97, 0}},
    {216, {0xc3, 0x98, 0}},
    {217, {0xc3, 0x99, 0}},
    {218, {0xc3, 0x9a, 0}},
    {219, {0xc3, 0x9b, 0}},
    {220, {0xc3, 0x9c, 0}},
    {221, {0xc3, 0x9d, 0}},
    {222, {0xc3, 0x9e, 0}},
    {223, {0xc3, 0x9f, 0}},
    {224, {0xc3, 0xa0, 0}},
    {225, {0xc3, 0xa1, 0}},
    {226, {0xc3, 0xa2, 0}},
    {227, {0xc3, 0xa3, 0}},
    {228, {0xc3, 0xa4, 0}},
    {229, {0xc3, 0xa5, 0}},
    {230, {0xc3, 0xa6, 0}},
    {231, {0xc3, 0xa7, 0}},
    {232, {0xc3, 0xa8, 0}},
    {233, {0xc3, 0xa9, 0}},
    {234, {0xc3, 0xaa, 0}},
    {235, {0xc3, 0xab, 0}},
    {236, {0xc3, 0xac, 0}},
    {237, {0xc3, 0xad, 0}},
    {238, {0xc3, 0xae, 0}},
    {239, {0xc3, 0xaf, 0}},
    {240, {0xc3, 0xb0, 0}},
    {241, {0xc3, 0xb1, 0}},
    {242, {0xc3, 0xb2, 0}},
    {243, {0xc3, 0xb3, 0}},
    {244, {0xc3, 0xb4, 0}},
    {245, {0xc3, 0xb5, 0}},
    {246, {0xc3, 0xb6, 0}},
    {247, {0xc3, 0xb7, 0}},
    {248, {0xc3, 0xb8, 0}},
    {249, {0xc3, 0xb9, 0}},
    {250, {0xc3, 0xba, 0}},
    {251, {0xc3, 0xbb, 0}},
    {252, {0xc3, 0xbc, 0}},
    {253, {0xc3, 0xbd, 0}},
    {254, {0xc3, 0xbe, 0}},
    {255, {0xc3, 0xbf, 0}},
    {0, {0, 0}}
  };
char *ConvertFromUtf8Codes( char *szText ) {
  int i, j, k;
  for ( k = j = 0;szText[ j ];j++ ) {
    for ( i = 0;g_Utf8[ i ].Code;i++ ) {
      if ( !strncmp( &szText[ j ], ( char* ) g_Utf8[ i ].szCode, strlen( ( char* ) g_Utf8[ i ].szCode ) ) ) {
        szText[ k++ ] = g_Utf8[ i ].Code;
        j += strlen( ( char* ) g_Utf8[ i ].szCode ) - 1;
        break;
      }
    }
    if ( !g_Utf8[ i ].Code )
      szText[ k++ ] = szText[ j ];
  }
  szText[ k ] = 0;
  return szText;
}
char *ConvertToUtfCodes( char *szText ) {
  int nLen = 0;
  int i, j, k;
  char *szRet;
  for ( i = 0;szText[ i ];i++ ) {
    for ( j = 0;g_Utf8[ j ].Code;j++ )
      if ( ( unsigned char ) szText[ i ] == g_Utf8[ j ].Code )
        break;
    if ( g_Utf8[ j ].Code )
      nLen += strlen( ( char* ) g_Utf8[ j ].szCode );
    else
      nLen++;
  }
  szRet = (char*)malloc( nLen + 1 );
  k = 0;
  for ( i = 0;szText[ i ];i++ ) {
    for ( j = 0;g_Utf8[ j ].Code;j++ )
      if ( ( unsigned char ) szText[ i ] == g_Utf8[ j ].Code )
        break;
    if ( g_Utf8[ j ].Code ) {
      strcpy( &szRet[ k ], ( char* ) g_Utf8[ j ].szCode );
      k += strlen( ( char* ) g_Utf8[ j ].szCode );
    } else
      szRet[ k++ ] = szText[ i ];
  }
  szRet[ k ] = 0;
  return szRet;
}
