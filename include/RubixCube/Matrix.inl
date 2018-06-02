
// N Choose M - Compute the number of ways a subset of N items can be
// selected from a set of M items.  N must be greater than M.

// The formula for N choose M is:
// N! / (M! * (N-M)!)

// The N! / (N-M)! portion can be computed iteratively as:
// N * (N-1) * (N-2) * (N-3) * ... * (N-M+1)

// The M! portion can be divided out iteratively as:
// 1, 2, 3 ... M
// (i.e. first divide partial result by 1, then by 2, then by 3,...)

// Note that both require M iterations allowing the entire calculation to be
// performed within a single loop.  Note also that we want to keep the
// numerator as large as possible during this calculation to avoid truncation
// error during the division.  This is accomplished by performing the
// multiplication in descending order and the division in ascending order.

// Since N choose M is equivalent to N choose (N-M) we can take advantage of
// this property to optimize the calculation in cases where M > N/2 since
// using the equivalent form results in a smaller M recalling that M is the
// number of loop iterations.

// Note, the performance of this function could be improved by using a table
// of precomputed results (i.e. NChooseM[N][M]).

int NChooseM(int N, int M)
{
  // Iterates from N down to M+1 to  compute N! / (N-M)!
  if (N < M) {
    return 0;  // N must be a subset of M
  }

  if (M > N / 2) {
    M = N - M;  // Optimization
  }

  int Result = 1;            // Holds the result of N choose M

  // Work on the N! / (N-M)! part, Divide out the M! part
  for (int MFact = 1; N > M; N--, MFact++) {
    Result = (Result * N) / MFact;
  }

  return Result;
}

//
// The following pair of permutation algorithms are based on a description in
// Knuth's "Fundamental Algorithms Volume 2: Seminumerical Algorithms" p64.

// PermutationToOrdinal - Given a permutation contained in a vector of length
// n, compute a unique ordinal in the range (0,...n!-1).

// This algorithm is based on the notion of a "factorial number system".
// An ordinal can be thought of as a number with a variable base, the sum of
// factorial terms, where each term is the product of a factorial and an
// associated coefficent.  It generates the coefficients by finding the
// position of the largest, second largest, third largest, etc. elements in
// the permutation vector.  Each time it finds the ith largest element, it
// exchanges that with the element at location i.  Thus there are i
// possibilities for the position of the ith largest element.  This process
// yields the i coefficients.

// Note: The length of the vector is currently limited to 12 elements.

WORD PermutToOrdinal(const BYTE vector[], char n)
{
  int Limit, Coeff_i, i;
  BYTE vect[12], t;       // Limits n <= 12
  WORD Ordinal;

  ASSERT(n <= 12);

  // Make a copy of the permutation vector
  memcpy(vect, vector, n);

  for (Limit = n, Ordinal = 0; --Limit > 0;) {

    // Find the maximum up to the current limit
    for (i = 1, t = vect[0], Coeff_i = 0; i <= Limit; i++) {
      if (vect[i] > t) {
        t = vect[i], Coeff_i = i;
      }
    }

    // Accumulate result
    Ordinal += Ordinal * Limit + Coeff_i;

    // Exchange elements
    t = vect[Limit], vect[Limit] = vect[Coeff_i], vect[Coeff_i] = t;
  }

  return Ordinal;
}

//
// OrdinalToPermutation - Given an ordinal in the range (0,...n!-1) compute a
// unique permutation of n items.

// This algorithm is essentially the above algorithm run backwards.  It uses
// modulo arithmetic and division to produce the coefficients that drive the
// exchanges.

void OrdinalToPermut(WORD Ordinal, BYTE vector[], char n, BYTE offset)
{
  int i, Coeff_i, ip;
  BYTE t;

  // Construct an inital permutation
  for (i = 0; i < n; i++) {
    vector[i] = BYTE(i) + offset;
  }

  for (i = 1; i < n; i++) {
    ip = i + 1;

    // Compute the coefficent
    Coeff_i = Ordinal % ip;
    // Divide out current "factorial number base"
    Ordinal /= ip;

    // Exchange elements
    t = vector[i], vector[i] = vector[Coeff_i], vector[Coeff_i] = t;
  }
}



// Correlation between colors and faces
// Give the default color corresponding to a facet number
inline BYTE defColor(BYTE no)//W Y R O B G
{
  static const BYTE cv[] = {4, 5, 1, 0, 3, 2};
  return no < 6 ? cv[no] : 0xFF;
}

inline BYTE defNoColor(BYTE noSide)
{
  return defColor(noSide / 9);
}

typedef BYTE t_COLORS;  // signature color: each bit represents a color
// ie : bit0=> color 0...
inline t_COLORS cvrtColor(BYTE color)
{
  return (color < 6) ? (1 << color) : 0;
}

inline BYTE colorCvrt(t_COLORS cvrt)
{
  BYTE c = 0xff;

  while (cvrt & 0x3f) {
    c++, cvrt >>= 1;
  }

  return c;
}

// the signature color of a cubie is the 'or' (|) of the signatures of its
// visible faces. Each cubie of a Rubik's Cube has a different signature.

// count the colors in a signature color of a cube
inline BYTE colorsCount(t_COLORS cvrt)
{
  BYTE c = 0;

  while (cvrt) {
    c += cvrt & 1, cvrt >>= 1;
  }

  return c;
}

// the center cubie has zero color, the cubies, centers sides, have one
// color, the edge cubies have two colors and the corner cubies have
// three colors

// for debug, visualize colors in a signature (in ascii)
inline int outColors(t_COLORS cvrt, char* out, int len)
{
  if (len > 6) {
    int i;
    memset(out, '-', 6);
    out[6] = 0;

    for (i = 0; i < 6; i++, cvrt >>= 1) if (cvrt & 1) {
        out[i] = '0' + i;
      }
  }

  return 6;
}





// Facets colors of the Rubik's Cube

static COLOR defaultColors[6] = {
  _RGB(255, 128, 0), // ORANGE   Right (Droite     O orange)
  _RGB(255, 0, 0),   // RED      Left  (Gauche     R rouge)
  _RGB(0, 192, 0),   // GREEN    Back  (Posterieur G vert)
  _RGB(0, 0, 255),   // BLUE     Front (Avant      B bleu)
  _RGB(255, 255, 255), // WHITE    Up    (Haut       W blanc)
  _RGB(255, 255, 0)  // YELLOW   Down  (Bas        Y jaune)
};

static COLOR UsedColors[6] = {
  defaultColors[0], defaultColors[1], defaultColors[2],
  defaultColors[3], defaultColors[4], defaultColors[5]
};

static COLOR BrushColors[6] = {
  UsedColors[0], UsedColors[1], UsedColors[2],
  UsedColors[3], UsedColors[4], UsedColors[5],
};



// Compute the color from the number (0..5)

COLOR RGBDrawColor(BYTE color)         // number

{
  return color < 6 ? UsedColors[color] : _RGB(127, 127, 127);
}



// Compute the text color from the background color

COLOR RGBTextColor(BYTE color)         // background color

{
  if (color < 6) {
    COLOR rgb = UsedColors[color];
    int lum = (6969 * BYTE(rgb) + 23434 * BYTE(rgb >> 8) + 2365 * BYTE(rgb >> 16)) / 32768;

    return (lum > 80) ? _RGB(0, 0, 0) : _RGB(255, 255, 255);
  }
  else {
    return _RGB(0, 0, 0);
  }
}


COLOR GetBrush(BYTE color)

{
  COLOR brush = (_RGB(127, 127, 127));
  return (color < 6) ? BrushColors[color] : brush;
}

void SetBrush(BYTE color,              // number of color (0..5)
    COLOR rgb)            // color

{
  UsedColors[color] = rgb;
  BrushColors[color] = (rgb);
}

void SetDefaultColors()

{
  for (BYTE i = 0; i < 6; i++) {
    SetBrush(i, UsedColors[i] = defaultColors[i]);
  }
}

void ColorsExchange(BYTE u, BYTE v)

{
  COLOR rgb_u = UsedColors[u];
  SetBrush(u, UsedColors[v]);
  SetBrush(v, rgb_u);
}



// Reading the colors in the register

void InitColors()

{
#if 0
  LPBYTE pData;
  UINT size;

  ::AfxGetApp()->GetProfileBinary("Settings", "Colors", &pData, &size);

  SetDefaultColors();

  if (pData) {
    if (size > sizeof(UsedColors)) {
      size = sizeof(UsedColors);
    }

    size /= 4;

    for (BYTE i = 0; i < size; i++) {
      SetBrush(i, PDWORD(pData)[i]);
    }

    delete [] pData;
  }

#endif
}



// Memorization of the colors in the register

void SaveRegColors()

{
#if 0
  ::AfxGetApp()->WriteProfileBinary("Settings", "Colors",
      (BYTE*)UsedColors, sizeof(UsedColors));
#endif
}

// Update the colors table from a string
BOOL ReadColors(const char* msg)

{

  if (strlen(msg) >= 36 && 0 == str_ncmp(msg, "COLORS", 6)) {
    SetDefaultColors();

    for (int i = 0, n = 6; i < 6; i++) {
      static const char* decod = ("0123456789ABCDEF");
      COLOR color = 0;

      for (int j = 0, ix; j < 6; j++) {
        if ((ix = str_findchr(decod, msg[n++])) < 0) {
          return FALSE;
        }
        else {
          color = color * 16 + ix;
        }
      }

      SetBrush(i, color);
    }

    return TRUE;
  }

  return FALSE;
}



// Output of the table of the 6 colors coded in hexadecimal

int WriteColors(char* buf, int len)
{
  _snprintf(buf, len, "COLORS%06X%06X%06X%06X%06X%06X", UsedColors[0], UsedColors[1],
      UsedColors[2], UsedColors[3], UsedColors[4], UsedColors[5]);

  return 0;
}



//
typedef unsigned short t_Mask;
// cube position :
// position Cube(bits9..14) | orientation Cube(bits0..5)
// posCube 2 bits per axe
// orCube : no 1 to 24 include
// or commande :
// mask(bits9..15) | rep(bit8) | plan(bits0..5)

// calculate the position of a cube 0..26
inline t_Mask cubePos(BYTE n)
{
  return ((n % 3 + 1) << 9) | (((n % 9) / 3 + 1) << 11) | (((n % 27) / 9 + 1) << 13);
}
// Convert the position of a cube in his number 0 a 26
inline BYTE cubeNo(t_Mask c)
{
  return BYTE(((c >> 9) & 3) + ((c >> 11) & 3) * 3 + ((c >> 13) & 3) * 9 - 13);
}

// test cube positions belonging to a plane
inline bool CubeChk(t_Mask cube, t_Mask m)
{
  WORD c = (m & 0x8000) ? 0x2A00 : 0x7E00;
  return ((cube ^ c)& m & 0x7E00) == 0;
}

inline int binConvert(BYTE d, char* msg, int len)
{
  int i = 0;

  while (d) {
    msg[i++] = ('0' + (d & 1));
    d >>= 1;
  }

  msg[i] = 0;
  return i;
}


//
typedef BYTE t_Command;

inline int CmdCubeCount(t_Command cmd)
{
  return ((cmd & 0x1F) / 6 == 2) ? 2 : 1;
}


//
// Reading and converting a command line

typedef enum {
  Move_R,  Move_L,  Move_B,  Move_F,  Move_U,  Move_D,     // faces names (x=> R-L, y => B-F, z=> U-D
  Move_cR, Move_cL, Move_cB, Move_cF, Move_cU, Move_cD,    // faces rotation CW
  Move_cR2, Move_cL2, Move_cB2, Move_cF2, Move_cU2, Move_cD2, // faces rotation 180 degrees
  Move_cRi, Move_cLi, Move_cBi, Move_cFi, Move_cUi, Move_cDi, // faces rotation CCW
  Move_cSc = 0x20, Move_cFull = 0x40, Move_cRot = 0x80, Move_CR = 1, Move_Init = 5
} Move;

inline const char* CmdConvertOneCmd(t_Command c)
{
  static const char* acode[24] = {
    "",  "\r\n", ":",  "",   "#",  "@",
    "R",  "L",  "B",  "F",  "U",  "D",
    "R2", "L2", "B2", "F2", "U2", "D2",
    "R'", "L'", "B'", "F'", "U'", "D'"
  };
  return acode[c & 0x1F];
}

int CCmdString_Init(char* buf, int len, t_Command cmd)
{
  ASSERT(len > 1);
  buf[0] = (char)cmd;
  buf[1] = 0;
  return 0;
}
int CCmdString_Init2(char* buf, int len, const t_Command table[], int length)
{
  STRcpy(buf, len, (const char*)table, length);
  return 0;
}


// Encodes a sequence of deformation the Rubik's Cube

t_Mask Command(short axe,     // 0..2 => x, y or z
    short plan,    // 0..2 => No slice, 3 => full Cube
    bool neg)      // CCW rotation
{
  t_Mask m = ((plan & 3) == 3) ? (0x0001 << (axe * 2)) | 0x0100 :
      ((0x603 & (plan + 0x0601)) << (axe << 1));
  return neg ? m | 0x8000 : m;
}



// Extract the rotation axis of a deformation order of the Rubik's Cube
short GetAxe(t_Mask cmd)
{
  short axe;

  for (axe = 0; axe < 3; axe++, cmd >>= 2) {
    if (cmd & 3) {
      break;
    }
  }

  return axe;
}



// Calculate the command mask
t_Mask CmdCubeSet(t_Command cmd)
{
  BYTE face = (cmd & 0x1F) % 6, or = (cmd & 0x1F) / 6, axe = (face / 2) & 3;
  t_Mask out;

  if ((cmd & 0x80) || cmd < 6) {
    return 0;  // multiple rotations
  }
  else if (cmd & 0x40) {
    out = (1 << (axe * 2)) | 0x0100;  // full cube
  }
  else if (cmd & 0x20) {
    out = 0x0602 << (axe * 2);  // slice 2
  }
  else if (face & 1) {
    out = 0x0601 << (axe * 2);  // slice 1
  }
  else {
    out = 0x0603 << (axe * 2);  // slice 3
  }

  if (face & 1) {
    out ^= 0x8000;
  }

  if ( or == 3) {
    out ^= 0x8000;
  }

  return out;
}



// Explode complex orders of rotation

t_Command CmdTranslate(t_Command& cmd)

{
  t_Command c = cmd;

  if (c & 0x80) {
    if (c & 0x40) {
      // CCW order
      for (int i = 4; i >= 0; i -= 2) {
        if (c = ((cmd >> i) & 3)) {
          cmd &= ~(3 << i);
          return i + c * 6 + 0x41;
        }
      }
    }
    else {
      // CW order
      for (int i = 0; i < 6; i += 2) {
        if (c = ((cmd >> i) & 3)) {
          cmd &= ~(3 << i);
          return i + c * 6 + 0x40;
        }
      }
    }
  }

  cmd = 0;
  return c;
}


static bool SearchCommand(const char* msg,   // line in a string
    int ptr)     // begining of the line in a string
{
  for (int i = ptr; i < strlen(msg); i++) {
    static const char* code = ("\r\n/rlbfudRLBFUD"); // char to detect
    char c = msg[i];
    int ix = str_findchr(code, c);

    if (ix < 0) {
      continue;
    }
    else if (ix < 3) {
      break;  // end of line or comments
    }
    else {
      return true;  // ok, char of commands
    }
  }

  return false;
}


// Invalidate commands line in a text

int InvalidCommands(const char* string, char* out, int len)
{
  int i;
  strncpy(out, string, len);

  for (i = strlen(string) - 1; --i >= 0;) {
    if (STR_cmp(string + i, 2, "\r\n", 2) == 0) {
      // new line detected
      if (SearchCommand(string, i + 2)) {
        STRins(out, -1, i + 2, 0, "/ ", -1, out, len);  // insert a comments marker
      }
    }
  }

  // search at the begin of the string
  if (SearchCommand(string, 0)) {
    STRins(out, -1, 0, 0, "/ ", -1, out, len);
  }

  // terminate the text lines
  //out.TrimLeft(), out.TrimRight();
  STRtrim(out, -1, " ");

  if (!strlen(out) && STR_cmp_last_n(out, -1, "\r\n", 2, 2) != 0) {
    STRcat(out, len, "\r\n", 2);
  }

  return 0;
}


//static const CString acode("rlbfudRLBFUD>*/\n");
static const char* acode = ("rlbfudRLBFUD>*/\n");   // command chars

// Convert ascii commands in order codes
int CmdExtract(char* buf, int buflen, const char* src, // source input
    int cur,   // 0 first item to be processed
    int* pLast)   // null end index out
{
  int len = strlen(src), beg = cur, flag = 0;
  bool frst = true;

  // the command interpreter, idles up to encounter an order
  while (cur < len) {
    char cmd = src[cur++];
    int code = str_findchr(acode, cmd);    // code( -1..15 )

    switch (code) {
    case 12 :
      if (flag == 0) {
        flag = 0x20;  // '>'
        continue;
      }

    case 13 :
      if (flag == 0) {
        flag = 0x40;  // '*'
        continue;
      }

    case -1 :
      break;                    // invalid char, next !

      // '/' start of comments
    case 14 :
      while (cur < len && src[cur++] != ('\n')) ;

      //'\n' new line
    case 15 :
      if (pLast) {
        *pLast = cur;
        return 0;
      }
      else {
        frst = true;
      }

      break;

    default :
      ASSERT(code >= 0 && code < 12);
      // is a command, the code is between 0 and 11

      if (code < 6) {
        code += 18;  // CCW (lowercase)
      }

      // post fixed orders
      if (cur < len) {
        char u = src[cur++];   // next char

        if (u == '\'' || u == '’'' || u == '3') {
            code = (code + 12) % 24;   // CCW
          } else if (u == ('2') || cmd == u) {
            code = (code % 6) + 12;    // 180 degrees
          } else {
            cur--;
          }
          }

            // generate the new command
            STRcat_c(buf, 256, code | flag);
          }
            flag = 0;                           // désarmer le préfixe
            beg = cur;                          // new command begin
          }
            // if (Right(1) != 1) *this += char(1);
            if (pLast) *pLast = cur;
            return 0;
          }



            // Convert an ascii command in the order code

            t_Command CmdExtractOne(const char* src,// source input
            int cur,    // first item to be processed
            int &last)     // end index out

            {
            int len = strlen(src), flag = 0;

            while (cur < len) {
            char cmd = src[cur++];
            int code = str_findchr(acode, cmd);    // code( -1..15 )

            switch (code) {
            case 12 : if (flag == 0) { flag = 0x20; break; }   // ' > '
            case 13 : if (flag == 0) { flag = 0x40; break; }   // '*'
            case -1 : last = cur; return 0;     // invalid char, we pass !

            case 14 : while (cur < len && src[cur++] != ('\n')) ;
            case 15 : last = cur; return 0;     // comments, we pass !

            default :
            ASSERT(code >= 0 && code < 12);
            // is a command, the code is between 0 and 11

            if (code < 6) code += 18;        // CCW (lowercase)

            // post fixed orders
            if (cur < len) {
            char u = src[cur++];   // next char

            if (u == char('\'') || u == char('’'') || u == char('3'))
                  code = (code + 12) % 24;   // CCW
                  else if (u == char('2') || cmd == u)
                  code = (code % 6) + 12;    // 180 degrees
                  else
                  cur--;
                }

                  last = cur;
                  // generate the new command
                  return code | flag;
                }
                }
                  last = cur;
                  return 0;
                }


                  // Convert command in the ascii codes
                  int CmdConvert(char* text, int textlen, const char* buf, bool cr)
                  {
                  int len = strlen(buf), n, cnt = 0;

                  for (int i = 0, ol = 0; i < len; i++) {
                  t_Command c = (t_Command)buf[i];

                  if (strlen(text) > ol && c != 1) {
                  if (strlen(text) - ol < 70) STRcat_c(text, textlen, ' ');
                  else STRcat(text, textlen, CmdConvertOneCmd(1), -1), ol = strlen(text);
                }

                  if (c < 6) {

                  // special codes
                  if (cr || c != 1) STRcat(text, textlen, CmdConvertOneCmd(c), -1);
                  switch (c) {
                  case 1 : ol = strlen(text); break;
                  case 2 : STRcat(text, textlen, buf+i + 1, 8), i += 8; break;
                  case 5 : STRcat(text, textlen, buf+i + 1, 27), i += 27; break; }
                } else if (c & 0x80) {
                  // Treatment of combined rotations along the three axes
                  text += char('[');
                      do {
                      BYTE cx = CmdTranslate(c);

                      if (cx) {
                      STRcat(text, textlen, "*", 1);
                      STRcat(text, textlen, CmdConvertOneCmd(cx), -1);
                    }
                    } while (c != 0);
                      text += char(']');
                  cnt++;
                } else {

                  // Treatment of slide rotations
                  static const char pre[] = ">* ";

                  ASSERT((c & 0x9F) < 24);
                  if (n = (c >> 5)) text += pre[n - 1];
                  STRcat(text, textlen, CmdConvertOneCmd(c), -1);
                  cnt++;
                }
                }

                  if (cr) {
                  if (cnt > 0) {
                  char tmp[32];
                  // add commands count at end of string
                  _snprintf(tmp, 32, " (%d)", cnt);
                  STRcat(text, textlen, tmp, -1);
                } else {
                  STRcpy(text, textlen, "/ Empty\r\n", -1);
                }
                }
                  return 0;
                }


                  inline static char Combine(char a, char b)
                  { return (((a &= 0x1F) / 6 + (b & 0x1F) / 6) & 3) * 6 + a % 6; }
                  inline static char getFace(char a)
                  { return (a & 0x1F) % 6 + (a & 0x60); }


                  // Optimize a command line

                  int CmdOptimize(char* buf)
                  {
                  int len = strlen(buf), i, j;
                  t_Command f2 = -1, f1 = -1, f0, o, cur;

                  for (i = j = 0; i < len; i++) {
                  cur = buf[i];     // first order

                  if (cur < 6 || (cur & 0x80)) {
                  ASSERT(cur != 5);
                  f2 = f1 = -1, buf[j++] = cur; continue;
                }

                  f0 = getFace(cur);       // face one
                  if (f0 == f1) {
                  // two consecutive orders are for the same face (and even Axis)
                  o = Combine(cur, buf[j - 1]);
                  if (o < 6) {
                  // two consecutive orders vanish
                  --j, f1 = f2, f2 = -1;
                } else {
                  // two consecutive orders are combined into one
                  buf[j - 1] = o | (cur & 0x60);
                }
                } else if (((f0 ^ f1)& 0x7E) != 0) {
                  // this order is not in the same axis as the previous
                  f2 = -1, f1 = f0, buf[j++] = cur;
                } else if (f0 == f2) {
                  // three consecutive orders are in the same axis and the first and
                  // third relate to the same face
                  o = Combine(cur, buf[j - 2]);
                  if (o < 6) {
                  // the first and third vanish
                  buf[j - 2] = buf[j - 1];
                  f1 = f2, f2 = -1, j--;
                } else {
                  // the first and third are combined in the only one
                  buf[j - 2] = o | (cur & 0x60);
                }
                } else {
                  // This order is in the same axis as the previous one but not the
                  // same face.
                  f2 = f1, f1 = f0, buf[j++] = cur;
                  // Note : insofar as there are only two sides per axis, as three
                  // levels to follow along the same axis, they must lead
                  // optimization, because both are the same face.
                }
                }
                  if (j < i) {
                  //Delete(j, i - j);
                  buf[j] = 0;
                }
                  return 0;
                }



                  t_Command toCommand(BYTE axe, BYTE cubeNo)
                  {
                  // cubes no
                  // 24 25 26          axe x, R - L
                  // upper layer  21  U 23          axe y, B - F
                  // 18 19 20          axe z, U - D

                  // 15  B 17
                  // middle layer   L 13  R
                  // 9  F 11

                  // 6  7  8
                  // bottom layer   3  D  5
                  // 0  1  2
                  // typedef enum {R,L,B,F,U,D};            // faces names
                  // using namespace char*;

                  static BYTE scmdTbl[3][27] = {
                  // axe x, R - L
                  // 0  1      2   3  4      5   6  7      8   9  10     11  12 13
                  {Move_cL,Move_cL|Move_cSc,Move_cRi,Move_cL,Move_cL|Move_cSc,Move_cRi,Move_cL,Move_cL|Move_cSc,Move_cRi,Move_cL,Move_cL|Move_cSc,Move_cRi,0, 0,
                  // 14 15 16     17  18 19     20  21 22     23  24 25     26
                  0, Move_cL,Move_cL|Move_cSc,Move_cRi,Move_cL,Move_cL|Move_cSc,Move_cRi,Move_cL,Move_cL|Move_cSc,Move_cRi,Move_cL,Move_cL|Move_cSc,Move_cRi},
                  // axe y B - F
                  // 0  1  2  3      4      5      6   7   8   9  10 11 12     13
                  {Move_cF,Move_cF,Move_cF,Move_cF|Move_cSc,Move_cF|Move_cSc,Move_cF|Move_cSc,Move_cBi,Move_cBi,Move_cBi,Move_cF,0, Move_cF,Move_cF|Move_cSc,0,
                  // 14     15  16 17  18 19 20 21     22     23     24  25  26
                  Move_cF|Move_cSc,Move_cBi,0, Move_cBi,Move_cF,Move_cF,Move_cF,Move_cF|Move_cSc,Move_cF|Move_cSc,Move_cF|Move_cSc,Move_cBi,Move_cBi,Move_cBi},
                  // axe z U - D
                  // 0  1  2  3  4  5  6  7  8  9      10     11     12     13
                  {Move_cD,Move_cD,Move_cD,Move_cD,0, Move_cD,Move_cD,Move_cD,Move_cD,Move_cD|Move_cSc,Move_cD|Move_cSc,Move_cD|Move_cSc,Move_cD|Move_cSc,0,
                  // 14     15     16     17     18  19  20  21  22 23  24  25  26
                  Move_cD|Move_cSc,Move_cD|Move_cSc,Move_cD|Move_cSc,Move_cD|Move_cSc,Move_cUi,Move_cUi,Move_cUi,Move_cUi,0, Move_cUi,Move_cUi,Move_cUi,Move_cUi}
                };

                  return (cubeNo < 27)? scmdTbl[axe][cubeNo]: 0;
                }


                  // Convert of an command line to debug in hexa

                  int CmdHexaDump(const char* buf, char* out, int len)
                  {
                  int i;
                  char tmp[32];
                  t_Command cmd;

                  for (i = 0; i < strlen(buf); i++) {
                  if ((cmd = buf[i]) == 1) STRcat(out, len, "\r\n", 2);
                  else {
                  _snprintf(tmp, 32, " %02x", cmd), STRcat(out, len, tmp, -1);
                }
                }

                  return 0;
                }


                  // CubeUtilities.cpp: implementation of the CubeUtilities utilities.
                  //
                  // BYTE scmd : compressed representation of a command
                  // bits 0,1 : axe + 1 : x => 1, y => 2, z => 3, 0=>null command,
                  // bits 2,3 : plan 0 => 0, 4 => 1, 8 => 2, 0xC => all plans,
                  // bits 4,5,6: provision
                  // bit  7   : sens rotation 0=> CW, 1=> CCW */

                  inline t_Mask Command(BYTE scmd)
                  {
                  return (scmd & 3) ? Command(--scmd & 0x03, (scmd & 0x0C) >> 2, (scmd & 0x80) != 0) : 0;
                }

                  inline BYTE PlanMed(BYTE scmd)
                  {
                  return (scmd & 0xF3) ^ 0x04;
                }

                  // { return (scmd & 0xF3) ^ ((scmd & 0x0C0)? 0x84: 0x04); }
                  inline BYTE PlanAll(BYTE scmd)
                  {
                  return scmd | 0x0C;
                }

                  // { return (scmd & 0x0C)? (scmd | 0x0C) ^ 0x80: (scmd | 0x0C); }
                  inline BYTE ShortCmd(short axe, short plan, bool neg)
                  {
                  return BYTE(++axe & 3) | BYTE((plan & 3) << 2) | (neg ? 0x80 : 0);
                }
                  //
                  // Gives the cubie number with that default colors signature
                  extern BYTE GetNoCube(t_COLORS cvrt);

                  // Gives the number of the cubie with this visible facet
                  extern BYTE side2NoCube(BYTE noSide);

                  // prochain cube ayant par defaut (au moins) les couleurs donnees
                  extern BYTE NextCube(t_COLORS cvrt, BYTE nbc, BYTE noCube = 0xff);


                  //
                  // conversion entre texte et table de 54 facettes


                  extern int SetSideText(const char* input, BYTE facets[54], const char* colorsId = "ORGBWYDRBULF");
                  extern int GetSideText(char* out, int len, const BYTE facets[54], const char* colorsId = "ORGBWYDRBULF");

                  // representation of the table of the facets in held at flat
                  extern void ComputRCSides(const IRECT& rect, IRECT rectSides[54]);
                  extern void DrawRCSides(img_t* im, const IRECT* pclip, const IRECT& rect, const BYTE facets[54]);

                  extern int ToSingmasterString(char* out, int len, const BYTE facets[54], const char* colorsId = "      UDLRFB");
                  extern bool SingmasterToFacets(const char* msg, BYTE facets[54]);

                  // descripteur de face 2D
                  typedef struct {
                  t_Mask plan;
                  char x, y;
                } t_Side;


                  //
                  typedef DWORD t_CUBES;  // each bit represents a cubie number

                  const t_CUBES ALL_CUBES = (1 << 27) - 1;

                  inline t_CUBES cvrtCube(BYTE noCube)   // convert cubie number in bit
                  {
                  return (noCube < 27) ? (1 << noCube) : 0;
                }
                  inline BYTE cubeCvrt(t_CUBES cubes)    // return the last cubie number
                  {
                  BYTE c = 0xff;

                  while (cubes) {
                  c++, cubes >>= 1;
                }

                  return c;
                }
                  inline BYTE cubesCount(t_CUBES cubes)  // counts the cubies
                  {
                  BYTE c = 0;

                  while (cubes) {
                  c += BYTE(cubes & 1), cubes >>= 1;
                }

                  return c;
                }

                  // provide all cubes having the same number of visible facets
                  extern t_CUBES GetCubesVSC(BYTE nbc);
                  // provide all cubies with facets to update
                  extern t_CUBES GetUpdCubes(const BYTE facets[54], BYTE nb = 0);
                  // provide all cubie without this colors
                  extern t_CUBES CubesWithoutColors(t_COLORS colors, const BYTE facets[54]);


                  //
                  typedef DWORD t_GROUP;  // the first 3 bytes contain the number of facets
                  // (0 .. 53) or colors (0 .. 5). The unused bytes
                  // are set to 0xff.

                  inline t_GROUP GROUP(BYTE a = 0xff, BYTE b = 0xff, BYTE c = 0xff)
                  {
                  return a | (b << 8) | (c << 16) | 0xFF000000;
                }

                  inline BYTE BYTE_0(t_GROUP gr)
                  {
                  return BYTE(gr);
                }
                  inline BYTE BYTE_1(t_GROUP gr)
                  {
                  return BYTE(gr >> 8);
                }
                  inline BYTE BYTE_2(t_GROUP gr)
                  {
                  return BYTE(gr >> 16);
                }

                  const t_GROUP GROUP_EMPTY = DWORD(-1);
                  const t_GROUP CENTER_FACES = GROUP(4, 4 + 2 * 9, 4 + 4 * 9);

                  BYTE GetCenterColor(t_GROUP cubeColors, BYTE face);

                  // Count the components (colors, dimensions,...) contained in a t_GROUP
                  inline BYTE SidesCount(t_GROUP group)
                  {
                  return ((BYTE_0(group) != 0xff) ? 1 : 0)
                  + ((BYTE_1(group) != 0xff) ? 1 : 0) + ((BYTE_2(group) != 0xff) ? 1 : 0);
                }

                  // Compute the color signature corresponding to a color group
                  inline t_COLORS cvrtColors(t_GROUP colors)
                  {
                  return cvrtColor(BYTE_0(colors)) |
                  cvrtColor(BYTE_1(colors)) | cvrtColor(BYTE_2(colors));
                }

                  // Give all visible facets belonging to a cubie
                  extern t_GROUP GetNoSides(BYTE noCube);
                  // Give default colors corresponding to the numbers of facets
                  extern t_GROUP RefColors(t_GROUP noSides);

                  // Give all facets of the same cubie that the facet 'side '
                  inline t_GROUP GetAllSides(BYTE side)
                  {
                  return GetNoSides(side2NoCube(side));
                }

                  // Put the defined colors in the facet table
                  extern void SetSideColors(t_GROUP noSides, t_GROUP noColors, BYTE facets[54]);
                  // Extract numbers of facets filled in,
                  // allowing the calculation of the orientation
                  extern t_GROUP GetOrientSides(const BYTE facets[54]);
                  // Provide the colors corresponding to the numbers of facets
                  extern t_GROUP GetSideColors(t_GROUP noSides, const BYTE facets[54]);

                  extern t_COLORS GetFreeColors(const BYTE facets[54], BYTE nbc = 0);
                  extern BYTE ColorCount(BYTE color, const BYTE facets[54], BYTE nbc = 0);
                  extern BYTE ColorsCount(const BYTE facets[54]);
                  extern bool IsFreeColors(t_COLORS cvrt, const BYTE facets[54], BYTE nbc);
                  extern bool FreeColorUpdate(BYTE nbc, BYTE facets[54]);

                  // center faces colors
                  inline BYTE ExtractSidesColors(BYTE no, t_GROUP colors)
                  {
                  return (no & 1) ^ BYTE(colors >> ((no / 2) * 8));
                }

                  // Switch the colors or facets in the variable t_GROUP, corresponding to
                  // rotations of a legal cubie for edge or corner.
                  inline t_GROUP Rotate(t_GROUP colors)
                  {
                  BYTE b;

                  if ((b = BYTE_2(colors)) != 0xff) {
                  return (colors << 8) | b | 0xff000000;  // 3 couleurs
                }

                  if ((b = BYTE_1(colors)) != 0xff) {
                  return (colors << 8) | b | 0xffff0000;  // 2 couleurs
                }

                  return colors;
                }                 // 1 ou 0 couleur : on ne fait rien

                  // Check the color group and counts the number of rotation from the origin
                  extern char CheckSidesColors(t_GROUP colors, BYTE* pNoCube = NULL);

                  // Calculates the parity of the number of permutations of cubie space
                  extern BYTE PermutationsParity(const BYTE cubes[], BYTE len);

                  extern int GetErrorMessage(char* out, int len, int err, const char* cr = "\n");

                  // Complete the first missing color
                  extern t_GROUP FillColor(t_GROUP colors, BYTE color);
                  // Change the color defined by the facet number, provides the corresponding
                  // color group
                  extern t_GROUP FillColor(BYTE side, BYTE color, const BYTE facets[54]);

                  inline void CubeSidesColors(BYTE colors[6], const BYTE facets[54])
                  {
                  for (int i = 0; i < 6; i++) {
                  colors[i] = facets[i * 9 + 4];
                }
                }


                  //
                  // Rotations of the facets center of face


                  inline DWORD SetCenters(int rR, int rL, int rB, int rF, int rU, int rD)
                  {
                  return rR | (rL << 4) | (rB << 8) | (rF << 12) | (rU << 16) | (rD << 20);
                }

                  extern bool IsCentersLegal(DWORD center);
                  extern int GetMsgCenter(char* out, int len, DWORD centers, int face = -1);
                  extern int CenterFaceOrient(DWORD centers, int face);
                  extern DWORD SetCenterFaceOrient(DWORD centers, int face, int orient);

                  inline DWORD NegateCenters(DWORD centers)
                  {
                  return ((0x00444444 - centers) & 0x00333333);
                }

                  inline bool IsChgSign(DWORD c)
                  {
                  return ((c & 0x00111111) != 0);
                }

                  DWORD CenterTranslate(DWORD centers, const BYTE oper[6], WORD sgn = 0);

                  //

                  // DEBUG fonctions
                  // convertit la couleur en un char ASCII
                  inline int color2Str(BYTE color)
                  {
                  static const char* c = ("ORGBWY");
                  return color < 6 ? c[color] : (' ? ');
                }
                  // convertit un groupe de couleurs en chaine ASCII
                  inline int colors2Str(char* out, int len, t_GROUP colors)
                  {
                  out[0] = color2Str(BYTE(colors >> 16));
                  out[1] = color2Str(BYTE(colors >> 8));
                  out[2] = color2Str(BYTE(colors));
                  out[3] = 0;
                  return 0;
                }

                  // conversion d'un ID resource string en sa chaine ASCII correspondante
          inline const char * String(int id, char * out, int len) {
          static struct {
            int id;
            char* str;
          } aa[] = {
#include "strs.txt"
          };
          int i;

          for (i = 0; i < countof(aa); ++i) {
            if (id == aa[i].id) {
              return aa[i].str;
            }
          }

          return "";
        }



            // Convert an ASCII string into a table of 54 colors

            int SetSideText(const char * input,    // ASCII string input
                BYTE facets[54],          // output table 54 facets colors
                const char * colorsId)       // 6 color letters + 6 color faces

        {
          ASSERT(strlen(colorsId) == 12);
          int len = strlen(input), i, n, x, k;
          char codesF[10];
          char codesC[10];
          STRmid_cpy(colorsId, -1, 6, -1, codesF, 10);
          STRmid_cpy(colorsId, -1, 0, 6, codesC, 10);

          // init the output buffer
          for (i = 0; i < 54; i++) {
            facets[i] = 0xFF;
          }

          // decode the input string
          for (i = k = 0, n = -1; i < len && k < 54; i++) {
            if (n < 0) {
              // beginning of a face
              if ((x = str_findchr(codesF, input[i])) < 0) {
                continue;
              }

              n = x * 9;

              if (input[i + 1] != ':') {
                return -5;  // missing ':' after side indicator
              }

            }
            else {
              // face running
              if ((x = str_findchr(codesC, input[i])) < 0) {
                continue;
              }

              if (facets[n] != 0xff) {
                return -6;  // double definition of face
              }

              facets[n] = BYTE(x), k++;

              if ((++n % 9) == 0) {
                n = -1;
              }
            }
          }

          return (k == 54) ? 0 : -7; // facets are not informed
        }



        // Convert a table of 54 colors in to an ascii string

        const char * GetSideText(const BYTE facets[54],     // table of facets colors
            const char * colorsId)      // 6 color letters + 6 color faces

        {
          ASSERT(strlen(colorsId) >= 12);
          const char* text;

          // for each face of the Rubik's Cube
          for (int i = 0, n = 0; i < 6; i++) {
            if (i > 0) {
              text += char(' ');
            }

            text += colorsId[i + 6];
            text += char(':');

            // compute the colors for the 9 facets
            for (int j = 0; j < 9; j++, n++) {
              BYTE c = facets[n];
              text += (c < 6) ? colorsId[c] : char('?');
            }
          }

          return text;
        }



        // Trace the faces of the Rubik's Cube in developed flat
        // from a table of 54 colors of facets

        void DrawRCSides(img_t * im, const IRECT * pclip,              // drawing DC
            const IRECT & rect,      // rectangle for the drawing
            const BYTE facets[54])   // facet colors table

        {
          IRECT rectSides[54];

          // calculate the rectangles of the 54 facets
          ComputRCSides(rect, rectSides);

          for (int n = 0; n < 54; n++) {
            COLOR clr = (GetBrush(facets[n]));

            imdraw_rect(im, pclip, rectSides[n], clr, 0, 0, 0);
            // draw this rectangles
          }
        }



        // Calculate the squares of the sides of the Rubik's Cube developed flat

        void ComputRCSides(const IRECT & rect, // drawing space available
            IRECT rectSides[54])// faces rectangles table

        {
          // 0-U
          // 2-L  4-F  3-R  5-B
          // 1-D          faces order :  U     D     L     R     F     B
          static const POINT positions[6] = {{1, 0}, {1, 2}, {0, 1}, {2, 1}, {1, 1}, {3, 1}};
          int h = (RCH(&rect) - 10) / 9, e = (RCH(&rect) - (9 * h) - 8) / 2;
          int orgx = (rect.l + rect.r - 12 * h - 3 * e) / 2;
          int i, n, x, y;

          // calculate the rectangle faces
          for (i = 0, n = 0; i < 6; i++) {
            const POINT& org = positions[i];
            int ss = h * 3 + e;
            int ox = orgx + org.x * ss, oy = rect.t + 5 + org.y * ss;

            for (y = 0; y < 3; y++) {
              for (x = 0; x < 3; x++, n++) {
                int x0 = ox + x * h, y0 = oy + y * h;

                rectSides[n] = iRECT(x0, y0, x0 + h - 1, y0 + h - 1);
              }
            }
          }
        }



        // Place the colors corresponding to the facets

        void SetSideColors(t_GROUP noSides,       // facets involved
            t_GROUP noColors,      // corresponding colors
            BYTE facets[54])       // table of 54 colors to update

        {
          BYTE noSide;

          while ((noSide = BYTE(noSides)) < 54) {
            facets[noSide] = BYTE(noColors);
            noSides = 0xFF000000 | (noSides >> 8);
            noColors >>= 8;
          }
        }



        // Determines the colors corresponding to the facets

        t_GROUP GetSideColors(t_GROUP noSides,       // facets involved
            const BYTE facets[54])    // table of 54 colors

        {
          union {
            t_GROUP dw;
            BYTE b[4];
          } data;
          BYTE noSide;

          data.dw = noSides;

          for (int i = 0; i < 4 && (noSide = data.b[i]) < 54; i++) {
            data.b[i] = facets[ noSide ];
          }

          return data.dw;                           // output colors
        }

        //
        enum Facelet {       // Facelet indices
          // 0  1  2  3  4  5  6  7  8   9 10 11 12 13 14 15 16 17
          u0, u1, u2, u3, u4, u5, u6, u7, u8, d0, d1, d2, d3, d4, d5, d6, d7, d8, // Up / Down
          // 18 19 20 21 22 23 24 25 26  27 28 29 30 31 32 33 34 35
          l0, l1, l2, l3, l4, l5, l6, l7, l8, r0, r1, r2, r3, r4, r5, r6, r7, r8, // Left / Right
          // 36 37 38 39 40 41 42 43 44  45 46 47 48 49 50 51 52 53
          f0, f1, f2, f3, f4, f5, f6, f7, f8, b0, b1, b2, b3, b4, b5, b6, b7, b8
        };// Front / Back

        // order of faces:   0=Up,   1=Down,  2=Left, 3=Right, 4=Front, 5=Back
        // colors of faces:  White=4,Yellow=5,Red=1,  Orange=0,Blue=3,  Green=2

        // Up                        couleurs   W           W = U, Y = D,
        // Left  Front  Right  Back        faces    R B O G       R = L, O = R,
        // Down                      par defaut Y           B = F, G = B

        // u0 u1 u2              c26                 no Cubes  24 25 26
        // c21      u3 u4 u5       c23                          couche  21  U 23
        // u6 u7 u8                                superieure  18 19 20
        // c18   c19    c20
        // l0 l1 l2    f0 f1 f2     r0 r1 r2     b0 b1 b2          couche  15  B 17
        // l3 l4 l5 c9 f3 f4 f5 c11 r3 r4 r5 c17 b3 b4 b5         mediane   L 13  R
        // l6 l7 l8    f6 f7 f8     r6 r7 r8     b6 b7 b8                   9  F 11
        // c0    c1    c2
        // d0 d1 d2                                    couche   6  7  8
        // c3       d3 d4 d5        c5                      inferieure   3  D  5
        // d6 d7 d8              c8                             0  1  2

        static const t_GROUP sidesTbl[27] = {
          GROUP(d0, l8, f6),   // DLF cube 0
          GROUP(d1, f7),       // DF  cube 1
          GROUP(d2, f8, r6),   // DFR cube 2
          GROUP(d3, l7),       // DL  cube 3
          GROUP(d4),           // D   cube 4
          GROUP(d5, r7),       // DR  cube 5
          GROUP(d6, b8, l6),   // DBL cube 6
          GROUP(d7, b7),       // DB  cube 7
          GROUP(d8, r8, b6),   // DRB cube 8
          GROUP(l5, f3),       // LF  cube 9
          GROUP(f4),           // F   cube 10
          GROUP(r3, f5),       // RF  cube 11
          GROUP(r4),           // L   cube 12
          GROUP_EMPTY,         // cube 13  central cube has no face
          GROUP(l4),           // R   cube 14
          GROUP(l3, b5),       // LB  cube 15
          GROUP(b4),           // B   cube 16
          GROUP(r5, b3),       // RB  cube 17
          GROUP(u6, f0, l2),   // UFL cube 18
          GROUP(u7, f1),      // UF  cube 19
          GROUP(u8, r0, f2),   // URF cube 20
          GROUP(u3, l1),       // UL  cube 21
          GROUP(u4),           // U   cube 22
          GROUP(u5, r1),       // UR  cube 23
          GROUP(u0, l0, b2),   // ULB cube 24
          GROUP(u1, b1),       // UB  cube 25
          GROUP(u2, b0, r2)
        }; // UBR cube 26

        // Tables and creation programs

        // table of the signature color for the cubes
        static BYTE refCvrt[64] = {0, };

        static void makeRefCvrt() {
          // provides the translation between the signature color and the number of
          // the cube
          for (int i = 0; i < 64; i++) {
            refCvrt[i] = 0xFF;
          }

          for (int n = 0; n < 27; n++) {
            refCvrt[ cvrtColors(RefColors(sidesTbl[n])) ] = BYTE(n);
          }
        }

        // conversion from number face to number cube
        static BYTE cvNoSide[54] = {0, };

        static void makeCvNoSide() {
          for (BYTE noCube = 0; noCube < 27; noCube++) {
            t_GROUP dw = sidesTbl[noCube];
            BYTE b;

            while ((b = BYTE(dw)) < 54) {
              cvNoSide[b] = noCube, dw >>= 8;
            }
          }
        }

        // cubies bits ranked by number of facets
        static t_CUBES cubesTbl[4] = {0, 0, 0, 0};

        static void makeCubesTbl() {
          for (BYTE noCube = 0; noCube < 27; noCube++) {
            cubesTbl[SidesCount(sidesTbl[noCube])] |= cvrtCube(noCube);
          }
        }



        BYTE GetCenterColor(t_GROUP cubeColors,// colors of the 3 faces Guidelines
        BYTE face) {       // face to search a color
          // colors : 0 1 2 3 4 5    faces : R,  L,  B,  F,  U,  D
          static const BYTE facetTable[6] = { 9,  8,  17, 16, 0,  1 };
          ASSERT(face < 6);
          BYTE fx = facetTable[face];

          return BYTE((cubeColors >> (fx & 0x18)) ^ (fx & 1));
        }

        // Defines the number of a cube from the signature color
        // of this visible faces
        BYTE GetNoCube(t_COLORS cvrt) {
          if (refCvrt[0] == 0) {
            makeRefCvrt();
          }

          return cvrt < 0x40 ? refCvrt[cvrt] : 0xff;
        }



        // Convertion du no de face en no de cube appartenant

        BYTE side2NoCube(BYTE noSide)


        {
          if (cvNoSide[0] == 0) {
            makeCvNoSide();
          }

          return noSide < 54 ? cvNoSide[noSide] : 0xff;
        }



        // Donne le prochain cube possedant ces couleurs
        // le jeu de couleurs peut etre partiel

        BYTE NextCube(t_COLORS cvrt, BYTE nbc, BYTE noCube)


        {
          ASSERT(colorsCount(cvrt) <= nbc);

          while (++noCube < 27) {
            t_GROUP noSides = sidesTbl[noCube];

            if (SidesCount(noSides) != nbc) {
              continue;
            }

            BYTE rCvrt = cvrtColors(RefColors(noSides));

            if ((cvrt & rCvrt) == cvrt) {
              return noCube;
            }
          }

          return 0xff;
        }



        // List the numbers of visible faces of a givend cube
        // (0 to 3 faces visible)

        t_GROUP GetNoSides(BYTE noCube)


        {
          return noCube < 27 ? sidesTbl[noCube] : GROUP_EMPTY;
        }



        // Gives the default color of a list of visible faces

        t_GROUP RefColors(t_GROUP noSides)


        {
          union {
            t_GROUP dw;
            BYTE b[4];
          } data;

          data.dw = noSides;

          for (int i = 0; i < 4; i++) {
            data.b[i] = defNoColor(data.b[i]);
          }

          return data.dw;
        }



        // Extract from the list the two central facets filled in
        // allowing to calculate the orientation of a Rubik's Cube.
        // Do not verify that other facets can indicate
        // a different direction.

        t_GROUP GetOrientSides(const BYTE facets[54])


        {
          // analyze aspects of face centers
          for (UINT n = 4, m; n < 54; n += 9) {
            if (facets[n] >= 6) {
              continue;
            }

            // jumps (possibly) the facet opposite to the first
            m = ((n / 9) | 1) * 9 + 4;

            // search the next
            while ((m += 9) < 54) {
              if (facets[m] < 6)
                // second facet found
              {
                return GROUP(n, m);
              }
            }

            break;
          }

          return GROUP_EMPTY;              // not found
        }

        //
        // The definiton string of the Identity cube in Singmaster notation is
        static const BYTE SingmasterEdgeToFacelet[24] = {
          // edges UF UR UB UL DF DR DB DL FR FL BR BL
          u7, f1, u5, r1, u1, b1, u3, l1, d1, f7, d5, r7, d7, b7, d3, l7, f5, r3, f3, l5, b3, r5, b5, l3
        };
        static const BYTE SingmasterCornerToFacelet[24] = {
          // corners UFR URB UBL ULF DRF DFL DLB DBR
          u8, f2, r0, u2, r2, b0, u0, b2, l0, u6, l2, f0, d2, r6, f8, d0, f6, l8, d6, l6, b8, d8, b6, r8
        };


        int ToSingmasterString(char * string, int len, const BYTE facets[54], // faces order UDLRFB
            const char * colorsId)     // 6 letters of colors + 6 of faces

        {
          char colors[6];
          int i, j;

          memset(string, char(' '), 67);
          string[67] = 0;

          // get colors of the center of the faces
          for (i = 0; i < 6; i++) {
            colors[facets[i * 9 + 4]] = colorsId[i + 6];
          }

          for (i = j = 0; i < 24; i++) {
            string[j++] = colors[facets[SingmasterEdgeToFacelet[i]]];

            // add spaces if necessary
            if (i % 2 == 1) {
              j++;
            }
          }

          for (i = 0; i < 24; i++) {
            string[j++] = colors[facets[SingmasterCornerToFacelet[i]]];

            // add spaces if necessary
            if (i % 3 == 2) {
              j++;
            }
          }

          string[j] = 0;
          return j;
        }



        bool SingmasterToFacets(const char * msg, BYTE facets[54])

        {
          static const BYTE colors[6] = {4, 5, 1, 0, 3, 2}; // Singmaster colors convert
          static const char* sgm = ("UDLRFB(/\n\r");    // Singmaster faces names
          int i, j, ix;

          for (i = 0; i < 54; i++) {
            facets[i] = 0xFF;
          }

          // set center of faces colors
          for (ix = 0; ix < 6; ix++) {
            facets[ix * 9 + 4] = colors[ix];
          }

          // convert Singmaster message
          for (i = 0, j = 0; i < strlen(msg); i++) {
            if ((ix = str_findchr(sgm, msg[i])) < 0) {
              continue;
            }
            else if (ix >= 6) {
              break;
            }

            if (j < 24) {
              facets[SingmasterEdgeToFacelet[j]] = colors[ix];
            }
            else if (j < 48) {
              facets[SingmasterCornerToFacelet[j - 24]] = colors[ix];
            }

            j++;
          }

          return (j == 48);
        }



        // Test whether the colors of this cubie are legal

        char CheckSidesColors(t_GROUP colors,  // colors has identified
            BYTE * pNoCube)  // cube corresponding to the colors


        {
          t_COLORS cvrt = cvrtColors(colors);
          BYTE noCube = GetNoCube(cvrt);   // cubie corresponding to this signature

          if (colorsCount(cvrt) != SidesCount(colors) || noCube >= 27) {
            return -2;  // illegal combination of colors
          }

          if (pNoCube) {
            *pNoCube = noCube;  // cubie number ouput
          }

          t_GROUP refColors = RefColors(sidesTbl[noCube]);
          ASSERT(cvrt == cvrtColors(refColors));

          // Research the number in rotations required to get the color group
          // corresponding to this cubie
          for (char n = 0; n < 3; n++, colors = Rotate(colors))
            if (colors == refColors) {
              return n;  // number in rotations required (0,1 ou 0,1,2)
            }

          // for a edge cube in 2 colors there are only two possible combinations
          // and legal, for a corner cube with 3 colors there are 6 possible
          // combinations of which only 3 are legal. The program also works for a
          // center face facet with one color !

          return -3;        // legal combination of colors, but disordered
        }



        // Calculate the number of permutations in places of cubies

        BYTE PermutationsParity(const BYTE cubes[],  // cubies no table
            BYTE len)            // table size

        {
          BYTE count = 0;

          // Permutation parity can be computed by counting the number of reversals
          // in the permutation sequence, - i.e., the number of pairs (p,q) such
          // that p>q and p precedes q. Then determine if the result is even or odd.
          // Do this for both edges (EPP) and corners (CPP). A configuration is
          // reachable if EPP=CPP or EPP+CPP is odd. (August/September cube.lovers -
          // Vanderschel/Saxe)
          for (BYTE p = 1; p < len; p++)
            for (BYTE q = p, k = cubes[p - 1]; q < len; q++)
              if (k > cubes[q]) {
                count++;
              }

          return count;
        }



        // Complete la premiere couleur manquante

        t_GROUP FillColor(t_GROUP colors, BYTE color)


        {
          union {
            t_GROUP dw;
            BYTE b[4];
          } c;

          c.dw = colors;

              for (int i = 0; i < 3; i++)
            if (c.b[i] == 0xff) {
              c.b[i] = color;
              return c.dw;
            }

          return 0;   // il n'y a pas de couleur a completer
        }

        // Change the color defined by the facet number
        t_GROUP FillColor(BYTE side, BYTE color, const BYTE facets[54]) {
          union {
            t_GROUP dw;
            BYTE b[4];
          } colors, nSides;
          int i;

          // extract all visible facets of the cube for the designated facet
          nSides.dw = GetAllSides(side);
              // the facets colors
              colors.dw = GetSideColors(nSides.dw, facets);

              // search the facet in all facets of the cube
          for (i = 0; nSides.b[i] != side; i++) {
            ASSERT(i < 2);
          }

          // change the color
          colors.b[i] = color;
          // give all the colors of the cube changed
          return colors.dw;
        }



        // Provide all cubes having the same number of visible facets

        t_CUBES GetCubesVSC(BYTE nbc) {
          if (cubesTbl[0] == 0) {
            makeCubesTbl();
          }

          return cubesTbl[nbc & 3];
        }



        // Definit la liste des cubes a completer

        t_CUBES GetUpdCubes(const BYTE facets[54], BYTE nb)


        {
          t_CUBES cubes = 0;

          for (BYTE i = 0; i < 54; i++)
            if (facets[i] == 0xff) {
              cubes |= cvrtCube(side2NoCube(i));
            }

          if (nb > 0) {
            // retire de la liste les cubes possedant un nombre de couleur
            // a completer different de la requete
            for (BYTE noCube = 0; noCube < 27; noCube++) {
              t_CUBES cube = cvrtCube(noCube);

              if (cubes & cube) {
                // cube a completer
                t_GROUP nSides = GetNoSides(noCube);
                t_GROUP colors = GetSideColors(nSides, facets);

                if (SidesCount(nSides) - SidesCount(colors) != nb) {
                  cubes ^= cube;
                }
              }
            }
          }

          return cubes;
        }



        // Definit la liste des cubes ne possedant pas ces couleurs

        t_CUBES CubesWithoutColors(t_COLORS cvrt, const BYTE facets[54])


        {
          t_CUBES cubes = 0;

          for (BYTE noCube = 0; noCube < 27; noCube++) {
            BYTE icvrt = cvrtColors(GetSideColors(GetNoSides(noCube), facets));

            if ((cvrt & icvrt) == 0) {
              cubes |= (1 << noCube);
            }
          }

          return cubes;
        }



        // Comptabilise la presence d'une couleur

        BYTE ColorCount(BYTE color,      // couleur a comptabiliser ou 0xff
            const BYTE facets[54], // table des couleurs des facettes
            BYTE nbc)        // type 0=>tous, ou 1,2,3 facettes/cube

        {
          BYTE count = 0;

          if (nbc == 0) {
            // comptabilise l'usage d'une couleur sur toutes les facettes
            for (int i = 0; i < 54; i++)
              if (facets[i] == color) {
                count++;
              }

          }
          else {
            for (UINT noCube = 0; noCube < 27; noCube++) {
              t_GROUP noSides = sidesTbl[noCube];

              // selectionne les cubes par nombre de facettes visibles
              if (SidesCount(noSides) == nbc) {
                union {
                  t_GROUP dw;
                  BYTE b[4];
                } colors;

                // compabilise l'usage d'une couleur dans les cubes selectionnes
                colors.dw = GetSideColors(noSides, facets);

                    for (UINT i = 0; i < nbc; i++)
                  if (colors.b[i] == color) {
                    count++;
                  }
              }
            }
          }

          return count;
        }



        // Counts the filled in facets

        BYTE ColorsCount(const BYTE facets[54]) // facets colors table


        {
          BYTE i, count;

          for (i = 0, count = 0; i < 54; i++) if (facets[i] < 6) {
              count++;
            }

          return count;
        }



        // Definit la liste des couleurs à completer

        t_COLORS GetFreeColors(const BYTE facets[54],// table des couleurs facettes
            BYTE nbc)     // type 0=>tous, ou 1,2,3 facettes/cube


        {
          static const BYTE max[4] = {9, 1, 4, 4};
          t_COLORS cvrt = 0;

          for (BYTE color = 0; color < 6; color++)
            if (ColorCount(color, facets, nbc) < max[nbc & 3]) {
              cvrt |= cvrtColor(color);
            }

          return cvrt;
        }



        // Definit si un groupe de couleurs est deja utilise

        bool IsFreeColors(t_COLORS cvrt, // groupe de couleurs
            const BYTE facets[54],// liste des 54 facettes a tester
            BYTE nbc)      // nbre de facettes du cube elementaire


        {
          if (nbc == 0) {
            // parmi toutes les facettes sauf les facettes centrales
            for (int i = 0; i < 54; i++) {
              if (i % 9 == 4) {
                continue;
              }

              cvrt &= ~cvrtColor(facets[i]);

              if (cvrt == 0) {
                return true;
              }
            }
          }
          else if (colorsCount(cvrt) == nbc) {
            // toutes les couleurs du type donne definies dans le groupe
            for (UINT noCube = 0; noCube < 27; noCube++) {
              t_GROUP noSides = sidesTbl[noCube];

              // selectionne les cubes par nombre de facettes visibles
              if (SidesCount(noSides) == nbc
              && cvrt == cvrtColors(GetSideColors(noSides, facets))) {
                return true;
              }
            }
          }
          else {
            // parmi les couleurs du type donne definies dans le groupe
            for (UINT noCube = 0; noCube < 27; noCube++) {
              t_GROUP noSides = sidesTbl[noCube];

              // selectionne les cubes par nombre de facettes visibles
              if (SidesCount(noSides) == nbc) {
                BYTE icvrt = cvrtColors(GetSideColors(noSides, facets));

                // test si le groupe de couleurs est utilise sur ce cube
                if ((icvrt & cvrt) == cvrt) {
                  return true;
                }
              }
            }
          }

          return false;
        }



        // Lorsqu'il ne manque qu'une couleur pour un type de facettes
        // complete la couleur

        bool FreeColorUpdate(BYTE nbc,      // type facette: nbre facettes visibles
            BYTE facets[54])  // liste des 54 facettes a completer


        {
          BYTE color, cvrt, side;

          ASSERT(nbc != 0 && nbc <= 3);

          // liste des couleurs inutilisees
          cvrt = GetFreeColors(facets, nbc);

          if (colorsCount(cvrt) != 1) {
            return false;
          }

          // si cette couleur est unique, complete les facettes de ce type
          color = colorCvrt(cvrt);

          for (side = 0; side < 54; side++)
            if (facets[side] >= 6 && SidesCount(GetAllSides(side)) == nbc) {
              facets[side] = color;
            }

          return true;
        }

        // Rotation of the facets side center //



        // Tests the state of rotation of the facets center side
        // of the Rubik's Cube

        bool IsCentersLegal(DWORD centers)

        {
          DWORD counts = 0;

          for (centers &= 0x00FFFFFF; centers; centers >>= 4) {
            counts += centers;
          }

          return (counts & 1) == 0;
        }


        static const char* codeCf = ("RLBFUD"); // names for the facets center of side

        inline static int FindFace(int face) {
          int f = str_findchr(codeCf, char(face));
          return f < 0 ? face : f;
        }

        static int GetMsgCenters(char * msg, int len, DWORD centers, int face, bool all, bool space) {
          static const char* orient[4] = { NULL, "+", "++", "-" }; // rotations

          ASSERT(face >= 0 && face < 6);

          centers = (centers >> (face * 4)) & 3;

          if (centers) {
            _snprintf(msg, len, "%c%s", codeCf[face], orient[centers]);
          }
          else if (all) {
            STRmid_cpy(codeCf, -1, face, face + 1, msg, len);
          }

          if (space && msg[0] != 0) {
            //msg.Insert(0, char(' '));
            STRins(msg, -1, 0, 0, " ", 1, msg, len);
          }

          return 0;
        }

        // Convert the rotation of all center side of the Rubik's Cube
        int GetMsgCenter(char * msg, int len, DWORD centers,         // counts of centers
            int face)       // face number or all

        {
          int i;
          char tmp[32];

          msg[0] = 0;
          face = FindFace(face);

          if (face >= 0 && face < 6) {
            GetMsgCenters(msg, len, centers, face, true, false);
          }
          else {
            for (i = 0; i < 6; i++) {
              GetMsgCenters(tmp, 32, centers, i, false, msg[0] != 0);
              STRcat(msg, len, tmp, -1);
            }
          }

          return 0;
        }



        // Calculate the rotation of one center side of the Rubik's Cube

        int CenterFaceOrient(DWORD centers,          // counts of centers
            int face)       // face number

        {
          face = FindFace(face);

          if (face >= 0 && face < 6) {
            return (centers >> (face * 4)) & 3;
          }

          return -1;
        }



        // Changes the rotation of one center side of the Rubik's Cube

        DWORD SetCenterFaceOrient(DWORD centers,     // counts of centers
            int face,        // face number
            int orient) // new count

        {
          face = FindFace(face);

          if (face >= 0 && face < 6) {
            orient &= 3;
            centers &= ~(0x0F  << (face * 4));
            centers |= (orient << (face * 4));
          }

          return centers;
        }



        // Change the location of counts of the central facets

        DWORD CenterTranslate(DWORD centers,         // facets center of face
            const BYTE oper[6],  // permutation order
            WORD sgn)       // sign change order

        {
          DWORD out = 0, t;

          for (int i = 0; i < 6; i++, centers >>= 4) {
            t = 3 & ((sgn >> i) & 1 ? 4 - centers : centers), out |= t << (oper[i] * 4);
          }

          return out;
        }



        // Error messages on the construction of a Rubik's Cube

        int GetErrorMessage(char * msg, int len, int er, const char * cr) {
          if (er < 0) {
            static const char* pChar[] = {
              // errors in construction of a Rubik's Cube
              "Central facets of faces illegal.",                   //-1
              "Color combination of a cube illegal.",               //-2
              "color scheme of a cube invalid.",                    //-3
              "Color combinations of cubes in two.",                //-4
              // Inability to read a definition color faceted
              "Color definition format of facets invalid.",         //-5
              "Double color definition of a face.",                 //-6
              "Color facets are not indicated.",                    //-7
              "No solution is found."
            };                             //-8

            if (er >= -8) {
              STRcat2(msg, len, pChar[1 - er], -1, cr, -1);
            }
            else {
              _snprintf(msg, len, "error %d.%s", -er, cr);
            }
          }
          else {
            // organizational mistakes of color facets
            if (er & 1) {
              STRcat2(msg, len, "Invalid edge orientation parity.", -1, cr, -1);
            }

            if (er & 2) {
              STRcat2(msg, len, "Invalid corner orientation parity.", -1, cr, -1);
            }

            if (er & 4) {
              STRcat2(msg, len, "Invalid total permutation parity.", -1, cr, -1);
            }

            if (er & 8) {
              STRcat2(msg, len, "Illegal orientation of central facets.", -1, cr, -1);
            }
          }

          if (msg[0] == 0 && er != 0) {
            _snprintf(msg, len, "error %02x.%s", er, cr);
          }

          return 0;
        }



        // Read a ascii string in a file

        char * ReadText(const char * fileName) {
          FILE* file;
          char* text = NULL;

          if (file = fopen(fileName, "rb")) {
            int len = fsize(file);
            text  = MALLOC(char, len + 1);
            text[len] = 0;
            fread(text, 1, len, file);
          }

          return text;
        }



        // Add a library path to the file name

        int FixePath(char * path, int len, const char * fileName) {
          // Computs file name of the solver tables
          GetModuleFileName(NULL, path, len);
          strcpy(STRrchrs(path, -1, "\\/") + 1, fileName);
          //PathRemoveFileSpec(path);
          //PathAppend(path, fileName);
          return 0;
        }


        //
        typedef BYTE t_Matrix;  // Value ranging from 1 to 24 inclusive, representing
        // the 24 possible orientations of a cube (modulo 90
        // degrees in x, y, z axis).

        const t_Matrix MatrixUnit = 1;   // initial orientation (x = 0, y = 0, z = 0)
        const t_Matrix MatrixInvalid = 0;// invalid value

        //extern t_Matrix MatrixInvert(t_Matrix mat);
        //extern t_Matrix MatrixProduct(t_Matrix x, t_Matrix y);
        //extern t_Matrix MatrixOrientation(short axe, bool neg = false);
        //extern t_Matrix MatrixPack(const char* mat);

        inline static BYTE InvertReq(BYTE rq) {
          return (rq & 1) ? rq ^ 2 : rq;
        }

        //#define PI     (3.14159265359f)

        static BYTE tabCmdRot[32] = {0};
        static BYTE tabMatOut[32] = {0};
        static BYTE tabOrient[6] = {0};
        static BYTE tabMatInp[128] = {0};

        static t_Matrix invertTbl[25] = {0, 0};
        static t_Matrix producTbl[25 * 25] = {0};

        // rotation plan in the matrix   0, x  1, y  2, z
        static const BYTE rotPlan[6] = { 1, 2, 2, 0, 0, 1 };

        // Tables generation


        inline void M33_Set(float * m_d, float fl) {
          m_d[0 * 3 + 0] = m_d[1 * 3 + 1] = m_d[2 * 3 + 2] = fl;
          m_d[1 * 3 + 0] = m_d[2 * 3 + 0] = m_d[0 * 3 + 1] = m_d[2 * 3 + 1] = m_d[0 * 3 + 2] = m_d[1 * 3 + 2] = 0.0f;
        }
#define M33_Cpy(m0, m1)  memcpy(m0, m1, sizeof(float)*9)

        BYTE M33_setMatPack(const float * m_d) {
          int i, j, k, m, t, u;
          float min, max, tmp;

          for (m = 0, k = -1, i = 3; --i >= 0; m <<= 1) {
            max = .0f, min = .0f;

            for (j = 0; j < 3; j++) {
              tmp = m_d[i * 3 + j];

              if (tmp > max) {
                max = tmp, t = j;
              }
              else if (tmp < min) {
                min = tmp, u = j;
              }
            }

            if (max > -min && k != t) {
              k = (k << 2) | t;
            }
            else if (k != u) {
              m |= 8, k = (k << 2) | u;
            }
            else {
              k = (k << 2) | t;
            }
          }

          // signes des 3 coefs + indices de x et y sont combines
          return BYTE(m) | BYTE(k) & 0x0F;
        }

        static char entryCnt(BYTE b) { // count the number of command
          // (an order holds a 2-bit field)
          char n;

          for (n = 0; b != 0; b >>= 2) {
            if (b & 3) {
              n += ((b & 3) == 2) ? 3 : 2;  // promotes the 90 degrees
            }
          }

          return n;
        }


        // Rotation de quart de tour, dans le sens desire et suivant un axe donne

        float * M33_Rotate90(float * m_d, short axe, // 0=> x, 1=> y, 2=> z
        bool neg) {  // sens
          int a, b;
          float t;

          // calcul du plan de rotation
          if (neg) {
            a = rotPlan[axe * 2 + 1], b = rotPlan[axe * 2 + 0];
          }
          else {
            a = rotPlan[axe * 2 + 0], b = rotPlan[axe * 2 + 1];
          }

          // rotation
          t = m_d[0 * 3 + a], m_d[0 * 3 + a] = m_d[0 * 3 + b], m_d[0 * 3 + b] = -t;
          t = m_d[1 * 3 + a], m_d[1 * 3 + a] = m_d[1 * 3 + b], m_d[1 * 3 + b] = -t;
          t = m_d[2 * 3 + a], m_d[2 * 3 + a] = m_d[2 * 3 + b], m_d[2 * 3 + b] = -t;

          return m_d;
        }

        void initTabCmdRot() {
          float m0[9] = {0}, m1[9] = {0}, m2[9] = {0};
          BYTE a, c, i, j, k, u, v;
          M33_Set(m0, 1.f);
          M33_Set(m1, 1.f);
          M33_Set(m2, 1.f);

          // initial value of tables
          for (i = 0; i < 32; i++) {
            tabCmdRot[i] = tabMatOut[i] = 0xFF;
          }

          for (i = 0; i < 128; i++) {
            tabMatInp[i] = 0xFF;
          }

          // for the 4 directions on the three axes
          for (u = MatrixUnit, i = 0; i < 4; i++) {
            for (M33_Cpy(m1, m0), j = 0; j < 4; j++) {
              for (M33_Cpy(m2, m1), k = 0; k < 4; k++) {

                // creating tables and unpacking compaction matrices
                v = M33_setMatPack(m2);
                ASSERT(v < 128);

                if ((a = tabMatInp[v]) == 0xFF) {
                  tabMatInp[v] = a = u++, tabMatOut[a] = v;
                }

                ASSERT(a < 25);

                // choose the order of rotation the shortest
                c = i | (j << 2) | (k << 4);// 00kkjjii => 00zzyyxx

                if (entryCnt(c) < entryCnt(tabCmdRot[a])) {
                  tabCmdRot[a] = c;
                }

                // define the ordes of rotation of 90 degrees on the sides
                if ((j | k) == 0 && (i & 1)) {
                  tabOrient[i == 1 ? 0 : 1] = a;
                }
                else if ((i | k) == 0 && (j & 1)) {
                  tabOrient[j == 1 ? 2 : 3] = a;
                }
                else if ((i | j) == 0 && (k & 1)) {
                  tabOrient[k == 1 ? 4 : 5] = a;
                }

                M33_Rotate90(m2, 2, false);  // z
              }

              M33_Rotate90(m1, 1, false);     // y
            }

            M33_Rotate90(m0, 0, false);        // x
          }

          TRACE("initTabCmdRot %d [%d,%d,%d,%d,%d,%d]\n",
              u, tabOrient[0], tabOrient[1], tabOrient[2],
              tabOrient[3], tabOrient[4], tabOrient[5]);
        }


        void M33_M33(float * m_d, t_Matrix mat, float size) {
          // init a zero toute la matrice
          M33_Set(m_d, 0.0f);

          if (tabMatInp[0] == 0) {
            initTabCmdRot();
          }

          // extrait les indices des coefs
          BYTE om = tabMatOut[mat & 0x1F];
          ASSERT(om != 0xFF);

          BYTE x = 3 & om, y = 3 & (om >> 2), z = 3 - x - y;

          m_d[0 * 3 + x] = (om & 0x10) ? -size : size;
          m_d[1 * 3 + y] = (om & 0x20) ? -size : size;
          m_d[2 * 3 + z] = (om & 0x40) ? -size : size;
        }


        // Compact the contents of the matrix (unitary)
        t_Matrix MatrixPack(const char * mat) {
          int i, j;
          BYTE k;  // two bits for the indice of coef value (0, 1, 2)
          BYTE m;  // one bit per sign 0x10=> x, 0x20=> y, 0x40=> z

          if (tabMatInp[0] == 0) {
            initTabCmdRot();
          }

          // search the indices of the 3 parameters
          for (i = 3, m = k = 0; --i >= 0; m <<= 1) {
            for (j = 0; mat[i * 3 + j] == 0; j++) {
              ASSERT(j < 3);
            }

            k = (k << 2) | BYTE(j);    // indice du coef (0,1,2) code sur 2 bits

            if (mat[i * 3 + j] < 0) {
              m |= 8;  // coef negatif
            }
          }

          i = m | (k & 0x0F);  // only two indices are used (x et y) + the 3 signes
          return tabMatInp[i]; // this is the address of the table 0..127.
        }



        // Calcule le produit inverse de deux matrices 3 * 3

        static void MatrixIProduct(const float * Myz, const float * Mxy,
        float * Mxz) {                 // resultat
          int i, j;

          for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
              Mxz[i * 3 + j] = Mxy[0 * 3 + i] * Myz[j * 3 + 0] + Mxy[1 * 3 + i] * Myz[j * 3 + 1] + Mxy[2 * 3 + i] * Myz[j * 3 + 2];
            }
          }
        }



        BYTE CmdRot(t_Matrix mat) {
          if (tabCmdRot[0] == 0) {
            initTabCmdRot();
          }

          return tabCmdRot[mat & 0x1F];
        }

        // Compacte le contenu de la matrice (unitaire)

        t_Matrix M33_Pack(const float * m_d) {
          if (tabMatInp[0] == 0) {
            initTabCmdRot();
          }

          return tabMatInp[M33_setMatPack(m_d)];
        }

        inline t_Command CmdGetRotation(const float * m_d, bool invert /* = false*/) {
          t_Matrix m = M33_Pack(m_d);
          return (m != 1) ? CmdRot(M33_Pack(m_d)) | (invert ? 0xC0 : 0x80) : 0;
        }
        // Calcule le produit de deux matrices 3 * 3
        static void MatrixProduct(const float * Myz, const float * Mxy, float * Mxz) { // resultat
          int i, j;
          float _tmp[9] = {0};
          float* tmp = (Myz == Mxz || Mxy == Mxz) ? _tmp : Mxz;

          for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
              tmp[i * 3 + j] = Mxy[i * 3 + 0] * Myz[0 * 3 + j] + Mxy[i * 3 + 1] * Myz[1 * 3 + j] + Mxy[i * 3 + 2] * Myz[2 * 3 + j];
            }
          }

          if (tmp != Mxz) {
            memcpy(Mxz, tmp, sizeof(*tmp) * 9);
          }
        }


        void M33_M33(float * m_d, t_Matrix mat, const float * matrix, float size) {
          float m[9] = {0};
          M33_M33(m, mat, size);

          if (matrix) {
            float r[9] = {0};
            MatrixProduct(matrix, m, r);
            M33_Cpy(m_d, r);
          }
          else {
            M33_Cpy(m_d, m);
          }
        }


        t_Matrix MatrixOrientation(short axe, bool neg) {
          ASSERT(axe >= 0 && axe < 3);
          return tabOrient[axe * 2 + neg ? 1 : 0];
        }


        void M33_Invert(float * m_d) {
          float t;
          int a, b;

          // echange entre ligne et colonne
          for (a = 0; a < 3; a++) {
            for (b = 0; b < a; b++) {
              t = m_d[a * 3 + b], m_d[a * 3 + b] = m_d[b * 3 + a], m_d[b * 3 + a] = t;
            }
          }
        }


        static void makeInvertTbl() {
          // set invalid values
          for (t_Matrix n = 0; n < 25; n++) {
            producTbl[n * 25 + 0] = producTbl[0 * 25 + n] = 0;
          }

          invertTbl[0] = 0;

          for (t_Matrix y = 1; y < 25; y++) {
            float m[9] = {0};
            M33_M33(m, y, 1.f);

            // product table
            for (t_Matrix x = 1; x < 25; x++) {
              float m1[9] = {0};
              M33_M33(m1, x, m, 1.f);
              producTbl[x * 25 + y] = M33_Pack(m1);
            }

            // invert table
            M33_Invert(m);
            invertTbl[y] = M33_Pack(m);
          }
        }

        t_Matrix MatrixInvert(t_Matrix mat) {
          if (invertTbl[1] == 0) {
            makeInvertTbl();
          }

          ASSERT(mat < 25);
          return invertTbl[mat];
        }



        t_Matrix MatrixProduct(t_Matrix x, t_Matrix y) {
          if (invertTbl[1] == 0) {
            makeInvertTbl();
          }

          ASSERT(x < 25 && y < 25);
          return producTbl[x * 25 + y];
        }


        void M33_M33(float * m_d, const float * pMyz, const float * pMxy) {
          MatrixProduct(pMyz, pMxy, m_d);
        }




        // Assure une rotatation progressive suivant un axe

        void M33_Rotate(float * m_d, short axe,    // axe rotation 0=> x, 1=> y, 2=> z
        short nb) {    // nombre d'increments
          float t, inc = (float)PI * 0.0005f;// increment sin(a) equivaut a l'angle pour les
          int a, b;                   // petits angles on peut confondre angle et sin

          // calcul du plan rotation
          if (nb < 0) {
            a = rotPlan[axe * 2 + 1], b = rotPlan[axe * 2 + 0], nb = -nb;
          }
          else {
            a = rotPlan[axe * 2 + 0], b = rotPlan[axe * 2 + 1];
          }

          while (--nb >= 0) {           // rotation de la matrice dans plan (a, b)
            t = m_d[0 * 3 + a], m_d[0 * 3 + a] += inc * m_d[0 * 3 + b], m_d[0 * 3 + b] -= inc * t;
            t = m_d[1 * 3 + a], m_d[1 * 3 + a] += inc * m_d[1 * 3 + b], m_d[1 * 3 + b] -= inc * t;
            t = m_d[2 * 3 + a], m_d[2 * 3 + a] += inc * m_d[2 * 3 + b], m_d[2 * 3 + b] -= inc * t;
          }
        }




        void M33_Rotate90Req(float * m_d, BYTE rq) // bits0,1: 0=> 0, 1=> 90, 2=> 180, 3=>270
        // bits2,3 : axe 0=> x, 1=> y, 2=> z

        {
          if (rq & 3) {
            // angle de rotation different de 0 degre
            short axe = (rq >> 2) & 3;

            if (rq & 1) {
              M33_Rotate90(m_d, axe, (rq & 2) == 0);  // plus ou moins 90 degres
            }
            else if (rq & 2) {
              M33_Rotate90(m_d, axe, true), M33_Rotate90(m_d, axe, true);  // 180 degres
            }
          }
        }



        void M33_Rotate90Mask(float * m_d, t_Mask mask, bool dbl) {
          if (mask & 0x003F) {
            short axe = GetAxe(mask);
            bool neg = (mask & 0x8000) != 0;

            M33_Rotate90(m_d, axe, neg);

            if (dbl) {
              M33_Rotate90(m_d, axe, neg);
            }
          }
        }



        // Assure la rotation de la matrice correspondant a la commande

        void M33_CmdSet(float * m_d, t_Command cmd) {
          if (cmd & 0x80) {
            // rotations multiples du Rubik's Cube complet
            if (cmd & 0x40) {
              // rotation negative suivant les 3 axes

              for (char axe = 2, or; axe >= 0; axe--)
                if ( or = ((cmd >> (axe * 2)) & 3)) {
                  M33_Rotate90Req(m_d, 4 - or + axe * 4);
                }
            }
            else {
              // rotation positive suivant les 3 axes

              for (char axe = 0, or; axe < 3; axe++) {
                if ( or = ((cmd >> (axe * 2)) & 3)) {
                  M33_Rotate90Req(m_d, or + axe * 4);
                }
              }
            }
          }
          else if (cmd >= 6) {
            t_Mask move = CmdCubeSet(cmd);

            M33_Set(m_d, 1.0f);
            M33_Rotate90Mask(m_d, move, (cmd & 0x1F) / 6 == 2);
            // ordre de rotations de faces
            // BYTE face = (cmd & 0x1F) % 6, or = (cmd & 0x1F) / 6, axe = face / 2;

            // axe x: R - L, axe y: F - B, axe z: U - D
            // if (face & 1) or = (4 - or) & 3;
            // Rotate90Req(or + axe * 4);
          }
        }


        void M33_RotateCmd(float * m_d, t_Mask cmd,   // ordre de rotation (sens, axe)
            short nb)      // amplitude de la rotation
        // (en nbre increments)
        {
          if (cmd & 0x003F) {
            if (cmd & 0x8000) {
              nb = -nb;
            }

            M33_Rotate(m_d, GetAxe(cmd), nb);
          }
        }

        // conversion d'un deplacement souris en rotation du cube
        void M33_PosRotate(float * m_d, int x, int y) {
          int nx = x, ny = y;
          short axe = 2;

          if (x < 0) {
            x = -x;
          }

          if (y < 0) {
            y = -y;
          }

          if (x > y) {
            axe = 0;
          }
          else {
            nx = ny;
          }

          M33_Rotate(m_d, axe, nx * 5);
        }



        DWORD M33_ToCommand(const float * m_d, t_Matrix or , bool neg) {
          BYTE rot = CmdRot( or), ng = (neg) ? 1 : 3;
          DWORD out = 0;
          DWORD out2;

          ASSERT(rot != 0xFF);

          if (rot == 0) {
            return out;  // commande nulle
          }

          for (BYTE axe = 1; rot != 0; axe++, rot >>= 2) {
            if (rot & 3) {
              out = (out << 8) | (axe | 0x0C);

              if ((rot & 3) == 2) {
                out = (out << 8) | BYTE(out);  // double commande
              }
              else if ((rot & 3) == ng) {
                out |= 0x80;  // commande negative
              }
            }
          }

          if (neg) {
            return out;
          }

          out2 = 0;      // inverse l'ordre des commandes

          while (out) {
            out2 = (out2 << 8) | BYTE(out), out >>= 8;
          }

          return out2;
        }



        // debug : matrix display orientation or position

        int M33_DumpMat(char * msg, int len, t_Matrix mat, const char * text) {
          BYTE om = tabMatOut[mat & 0x1F];
          UINT x = om & 3, y = (om >> 2) & 3;
          char m[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

          if (om == 0xFF) {
            _snprintf(msg, len, "%s  **** err **** %02X **** ", text, mat);
          }
          else {
            m[0 * 3 + x]      = (om & 0x10) ? -1 : 1;
            m[1 * 3 + y]      = (om & 0x20) ? -1 : 1;
            m[2 * 3 + 3 - x - y] = (om & 0x40) ? -1 : 1;

            _snprintf(msg, len, "%s fr=%2d  %3d%3d%3d  %3d%3d%3d  %3d%3d%3d ", text, mat,
                m[0 * 3 + 0], m[0 * 3 + 1], m[0 * 3 + 2], m[1 * 3 + 0], m[1 * 3 + 1],
                m[1 * 3 + 2], m[2 * 3 + 0], m[2 * 3 + 1], m[2 * 3 + 2]);
          }

          return 0;
        }

        int M33_Dump(const float * m_d, char * msg, int len, short mode) {
          int i;
          char tmp[256];
          int msglen = 0;

          if (mode & 1) {
            for (i = 0; i < 3; i++) {
              _snprintf(tmp, 256, " %7.4f %7.4f %7.4f\n", m_d[i * 3 + 0], m_d[i * 3 + 1], m_d[i * 3 + 2]);
              msglen = STRcat(msg, len, tmp, -1);
            }
          }

          if (mode & 2) {
            M33_DumpMat(tmp, 256, M33_Pack(m_d), " t_Matrix ");
            msglen = STRcat2(msg, len, tmp, -1, "\n", 1);
          }

          return msglen;
        }
