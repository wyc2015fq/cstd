
typedef unsigned char byte;

int RGBToXYZ(const byte* From, byte* To, int Length)
{
  const byte* End = From + Length * 3;
  int x, y, z;
  byte Red, Green, Blue;

  if (Length < 1) {
    return 0;
  }

  while (From != End) {
    Blue = *From;
    Green = *(From + 1);
    Red = *(From + 2);
    x = (Blue * 199049 + Green * 394494 + Red * 455033 + 524288) >> 20;
    y = (Blue * 75675 + Green * 749900 + Red * 223002 + 524288) >> 20;
    z = (Blue * 915161 + Green * 114795 + Red * 18621 + 524288) >> 20;
    *To = (byte)x;
    *(To + 1) = (byte)y;
    *(To + 2) = (byte)z;
    From += 3;
    To += 3;
  }

  return 0;
}
int XYZToRGB(const byte* From, byte* To, int Length)
{
  int X, Y, Z;
  int Blue, Green, Red;
  const byte* End = From + Length * 3;

  while (From != End) {
    X = *From;
    Y = *(From + 1);
    Z = *(From + 2);
    Blue = (X * 55460 - Y * 213955 + Z * 1207070) >> 20;
    Green = (X * -965985 + Y * 1967119 + Z * 47442) >> 20;       // x * -965985 和 -x * 965985 在反汇编后是不一样的，后者多了个neg指令
    Red = (X * 3229543 - Y * 1611819 - Z * 569148) >> 20;

    if (Red > 255) {
      Red = 255;
    }
    else if (Red < 0) {
      Red = 0;  // 这里需要判断，因为RGB空间所有的颜色转换到YCbCr后，并不是填充满了0-255的范围的，反转过去就会存在一些溢出的点。
    }

    if (Green > 255) {
      Green = 255;
    }
    else if (Green < 0) {
      Green = 0;  // 编译后应该比三目运算符的效率高
    }

    if (Blue > 255) {
      Blue = 255;
    }
    else if (Blue < 0) {
      Blue = 0;
    }

    *(To) = (byte)Blue;
    *(To + 1) = (byte)Green;
    *(To + 2) = (byte)Red;
    From += 3;
    To += 3;
  }

  return 0;
}


#define LABXRF  0.433953f         // RGB转XYZ的校正后系数(浮点类型）
#define LABXGF  0.376219f
#define LABXBF  0.189828f
#define LABYRF  0.212671f
#define LABYGF  0.715160f
#define LABYBF  0.072169f
#define LABZRF  0.017758f
#define LABZGF  0.109477f
#define LABZBF  0.872765f

#define LABRXF  3.0799327f        // XYZ转RGB的校正后系数(浮点类型）
#define LABRYF  -1.53715f
#define LABRZF  -0.542782f
#define LABGXF  -0.921235f
#define LABGYF  1.875991f
#define LABGZF  0.04524426f
#define LABBXF  0.0528909755f
#define LABBYF  -0.204043f
#define LABBZF  1.15115158f

#define Shift  10
#define HalfShiftValue (1 << (Shift - 1))

#define LABXRI  (int)(LABXRF * (1 << Shift) + 0.5)      // RGB转XYZ的校正后系数
#define LABXGI  (int)(LABXGF * (1 << Shift) + 0.5)
#define LABXBI  (1 << Shift) - LABXRI - LABXGI          //保证三个系数之和为(1 << Shift)
#define LABYRI  (int)(LABYRF * (1 << Shift) + 0.5)
#define LABYGI  (int)(LABYGF * (1 << Shift) + 0.5)
#define LABYBI  (1 << Shift) - LABYRI - LABYGI
#define LABZRI  (int)(LABZRF * (1 << Shift) + 0.5)
#define LABZGI  (int)(LABZGF * (1 << Shift) + 0.5)
#define LABZBI  (1 << Shift) - LABZRI - LABZGI

#define LABRXI  (int)(LABRXF * (1 << Shift) + 0.5)        // XYZ转RGB的校正后系数
#define LABRYI  (int)(LABRYF * (1 << Shift) + 0.5)
#define LABRZI  (1 << Shift) - LABRXI - LABRYI            //保证三个系数之和为(1 << Shift)
#define LABGXI  (int)(LABGXF * (1 << Shift) + 0.5)
#define LABGYI  (int)(LABGYF * (1 << Shift) + 0.5)
#define LABGZI  (1 << Shift) - LABGXI - LABGYI
#define LABBXI  (int)(LABBXF * (1 << Shift) + 0.5)
#define LABBYI  (int)(LABBYF * (1 << Shift) + 0.5)
#define LABBZI  (1 << Shift) - LABBXI - LABBYI

#define ScaleLC  (int)(16 * 2.55 * (1 << Shift) + 0.5)
#define ScaleLT  (int)(116 * 2.55 + 0.5)

#define Threshold  (int)(6 * 6 * 6.0f / (29 * 29 * 29) * 255 * 4 + 0.5)

#define Div116  (1.0f / 116 * (100.0F / 255))
#define Div500  (1.0f / 500)
#define Div200  (1.0f / 200)
#define Add16   (16.0f / 116)
#define ThresoldF  (6.0f / 29)
#define MulT       (3 * (6 * 6.0f) / (29 * 29))
#define Sub4Div29  (4.0f / 29)

static int LabTab[(1024 * 4)];
static int TabX[(256 * 256 * 4)];
static int TabY[(256 * 4)];
static int TabZ[(256 * 256 * 4)];

static int RGBLAB_table_init()
{
  int I, J, Index = 0;
  float X, Y, Z, T;

  for (I = 0; I < 1024; I++) {
    if (I > Threshold) {
      LabTab[I] = (int)(pow((float)I / 1020, 1.0F / 3) * (1 << Shift) + 0.5);
    }
    else {
      LabTab[I] = (int)((29 * 29.0 * I / (6 * 6 * 3 * 1020) + 4.0 / 29) * (1 << Shift) + 0.5);
    }
  }

  for (I = 0; I < 256; I++) {
    T = I * Div116 + Add16;

    if (T > ThresoldF) {
      Y = T * T * T;
    }
    else {
      Y = MulT * (T - Sub4Div29);
    }

    TabY[I] = (int)(Y * 255 + 0.5);

    for (J = 0; J < 256; J++) {
      X = T + Div500 * (J - 128);

      if (X > ThresoldF) {
        X = X * X * X;
      }
      else {
        X = MulT * (X - Sub4Div29);
      }

      TabX[Index] = (int)(X * 255 + 0.5);

      Z = T - Div200 * (J - 128);

      if (Z > ThresoldF) {
        Z = Z * Z * Z;
      }
      else {
        Z = MulT * (Z - Sub4Div29);
      }

      TabZ[Index] = (int)(Z * 255 + 0.5);
      Index++;
    }
  }

  return 0;
}

int RGBToLAB(const byte* From, byte* To, int Length)
{
  const byte* End = From + Length * 3;
  int X, Y, Z, L, A, B;
  byte Red, Green, Blue;
  static int RGBLAB_table_inited = 0;

  if (RGBLAB_table_inited == 0) {
    RGBLAB_table_inited = 1;
    RGBLAB_table_init();
  }

  if (Length < 1) {
    return 0;
  }

  while (From != End) {
    Blue = *From;
    Green = *(From + 1);
    Red = *(From + 2);
    X = (Blue * LABXBI + Green * LABXGI + Red * LABXRI + HalfShiftValue) >> (Shift - 2);
    Y = (Blue * LABYBI + Green * LABYGI + Red * LABYRI + HalfShiftValue) >> (Shift - 2);
    Z = (Blue * LABZBI + Green * LABZGI + Red * LABZRI + HalfShiftValue) >> (Shift - 2);
    X = LabTab[X];
    Y = LabTab[Y];
    Z = LabTab[Z];
    L = ((ScaleLT * Y - ScaleLC + HalfShiftValue) >> Shift);
    A = ((500 * (X - Y) + HalfShiftValue) >> Shift) + 128;
    B = ((200 * (Y - Z) + HalfShiftValue) >> Shift) + 128;
    *To = (byte)L;
    *(To + 1) = (byte)A;
    *(To + 2) = (byte)B;
    From += 3;
    To += 3;
  }

  return 0;
}

int LABToRGB(const byte* From, byte* To, int Length)
{
  const byte* End = From + Length * 3;
  int L, A, B, X, Y, Z;
  int Blue, Green, Red;

  if (Length < 1) {
    return 0;
  }

  while (From != End) {
    L = *(From);
    A = *(From + 1);
    B = *(From + 2);
    X = TabX[L * 256 + A];      // *256编译后会自动优化为移位的
    Y = TabY[L];
    Z = TabZ[L * 256 + B];
    Blue = (X * LABBXI + Y * LABBYI + Z * LABBZI + HalfShiftValue) >> Shift;
    Green = (X * LABGXI + Y * LABGYI + Z * LABGZI + HalfShiftValue) >> Shift;
    Red = (X * LABRXI + Y * LABRYI + Z * LABRZI + HalfShiftValue) >> Shift;

    if (Red > 255) {
      Red = 255;
    }
    else if (Red < 0) {
      Red = 0;
    }

    if (Green > 255) {
      Green = 255;
    }
    else if (Green < 0) {
      Green = 0;  // 需要有这个判断
    }

    if (Blue > 255) {
      Blue = 255;
    }
    else if (Blue < 0) {
      Blue = 0;
    }

    *(To) = (byte)Blue;
    *(To + 1) = (byte)Green;
    *(To + 2) = (byte)Red;
    From += 3;
    To += 3;
  }

  return 0;
}


#define YCbCrYRF  0.299F              // RGB转YCbCr的系数(浮点类型）
#define YCbCrYGF  0.587F
#define YCbCrYBF  0.114F
#define YCbCrCbRF  -0.168736F
#define YCbCrCbGF  -0.331264F
#define YCbCrCbBF  0.500000F
#define YCbCrCrRF  0.500000F
#define YCbCrCrGF  -0.418688F
#define YCbCrCrBF  -0.081312F

#define RGBRYF  1.00000F            // YCbCr转RGB的系数(浮点类型）
#define RGBRCbF  0.0000F
#define RGBRCrF  1.40200F
#define RGBGYF  1.00000F
#define RGBGCbF  -0.34414F
#define RGBGCrF  -0.71414F
#define RGBBYF  1.00000F
#define RGBBCbF  1.77200F
#define RGBBCrF  0.00000F

#undef  Shift
#undef HalfShiftValue

#define Shift  20
#define HalfShiftValue  (1 << (Shift - 1))

#define YCbCrYRI   (int)(YCbCrYRF * (1 << Shift) + 0.5)         // RGB转YCbCr的系数(整数类型）
#define YCbCrYGI   (int)(YCbCrYGF * (1 << Shift) + 0.5)
#define YCbCrYBI   (int)(YCbCrYBF * (1 << Shift) + 0.5)
#define YCbCrCbRI  (int)(YCbCrCbRF * (1 << Shift) + 0.5)
#define YCbCrCbGI  (int)(YCbCrCbGF * (1 << Shift) + 0.5)
#define YCbCrCbBI  (int)(YCbCrCbBF * (1 << Shift) + 0.5)
#define YCbCrCrRI  (int)(YCbCrCrRF * (1 << Shift) + 0.5)
#define YCbCrCrGI  (int)(YCbCrCrGF * (1 << Shift) + 0.5)
#define YCbCrCrBI  (int)(YCbCrCrBF * (1 << Shift) + 0.5)

#define RGBRYI   (int)(RGBRYF * (1 << Shift) + 0.5)              // YCbCr转RGB的系数(整数类型）
#define RGBRCbI  (int)(RGBRCbF * (1 << Shift) + 0.5)
#define RGBRCrI  (int)(RGBRCrF * (1 << Shift) + 0.5)
#define RGBGYI   (int)(RGBGYF * (1 << Shift) + 0.5)
#define RGBGCbI  (int)(RGBGCbF * (1 << Shift) + 0.5)
#define RGBGCrI  (int)(RGBGCrF * (1 << Shift) + 0.5)
#define RGBBYI   (int)(RGBBYF * (1 << Shift) + 0.5)
#define RGBBCbI  (int)(RGBBCbF * (1 << Shift) + 0.5)
#define RGBBCrI  (int)(RGBBCrF * (1 << Shift) + 0.5)

int RGBToYCbCr(const byte* From, byte* To, int Length)
{
  const byte* End = From + Length * 3;
  int Red, Green, Blue;

  if (Length < 1) {
    return 0;
  }

  while (From != End) {
    Blue = *From;
    Green = *(From + 1);
    Red = *(From + 2);
    // 无需判断是否存在溢出，因为测试过整个RGB空间的所有颜色值，无颜色存在溢出
    *To = (byte)((YCbCrYRI * Red + YCbCrYGI * Green + YCbCrYBI * Blue + HalfShiftValue) >> Shift);
    *(To + 1) = (byte)(128 + ((YCbCrCbRI * Red + YCbCrCbGI * Green + YCbCrCbBI * Blue + HalfShiftValue) >> Shift));
    *(To + 2) = (byte)(128 + ((YCbCrCrRI * Red + YCbCrCrGI * Green + YCbCrCrBI * Blue + HalfShiftValue) >> Shift));
    From += 3;
    To += 3;
  }

  return 0;
}

int YCbCrToRGB2(const byte* From, byte* To, int Length)
{
  const byte* End = From + Length * 3;
  int Red, Green, Blue;
  int Y, Cb, Cr;

  if (Length < 1) {
    return 0;
  }

  while (From != End) {
    Y = *From;
    Cb = *(From + 1) - 128;
    Cr = *(From + 2) - 128;
    Red = Y + ((RGBRCrI * Cr + HalfShiftValue) >> Shift);
    Green = Y + ((RGBGCbI * Cb + RGBGCrI * Cr + HalfShiftValue) >> Shift);
    Blue = Y + ((RGBBCbI * Cb + HalfShiftValue) >> Shift);

    if (Red > 255) {
      Red = 255;
    }
    else if (Red < 0) {
      Red = 0;
    }

    if (Green > 255) {
      Green = 255;
    }
    else if (Green < 0) {
      Green = 0;  // 编译后应该比三目运算符的效率高
    }

    if (Blue > 255) {
      Blue = 255;
    }
    else if (Blue < 0) {
      Blue = 0;
    }

    *To = (byte)Blue;                                               // 由于不是一一对应的，需要判断是否越界
    *(To + 1) = (byte)Green;
    *(To + 2) = (byte)Red;
    From += 3;
    To += 3;
  }

  return 0;
}




// http://en.wikipedia.org/wiki/YDbDr
//const float YDbDrYRF = 0.299F;              // RGB转YDbDr的系数(浮点类型）
//const float YDbDrYGF = 0.587F;
//const float YDbDrYBF = 0.114F;
//const float YDbDrDbRF = -0.450F;
//const float YDbDrDbGF = -0.883F;
//const float YDbDrDbBF = 1.333F;
//const float YDbDrDrRF = -1.333F;
//const float YDbDrDrGF = 1.116F;
//const float YDbDrDrBF = 0.217F;

//const float RGBRYF = 1.00000F;            // YDbDr转RGB的系数(浮点类型）
//const float RGBRDbF = 0.000092303716148F;
//const float RGBRDrF = -0.525912630661865F;
//const float RGBGYF = 1.00000F;
//const float RGBGDbF = -0.129132898890509F;
//const float RGBGDrF = 0.267899328207599F;
//const float RGBBYF = 1.00000F;
//const float RGBBDbF = 0.664679059978955F;
//const float RGBBDrF = -0.000079202543533F;


#define YDbDrYRF  0.299F              // RGB转YDbDr的系数(浮点类型）
#define YDbDrYGF  0.587F
#define YDbDrYBF  0.114F
#define YDbDrDbRF  -0.1688F
#define YDbDrDbGF  -0.3312F
#define YDbDrDbBF  0.5F
#define YDbDrDrRF  -0.5F
#define YDbDrDrGF  0.4186F
#define YDbDrDrBF  0.0814F

#define RGBRYF  1.00000F            // YDbDr转RGB的系数(浮点类型）
#define RGBRDbF  0.0002460817072494899F
#define RGBRDrF  -1.402083073344533F
#define RGBGYF  1.00000F
#define RGBGDbF  -0.344268308442098F
#define RGBGDrF  0.714219609001458F
#define RGBBYF  1.00000F
#define RGBBDbF  1.772034373903893F
#define RGBBDrF  0.0002111539810593343F

#define Shift  20
#define HalfShiftValue  (1 << (Shift - 1))

#define YDbDrYRI  (int)(YDbDrYRF * (1 << Shift) + 0.5)         // RGB转YDbDr的系数(整数类型）
#define YDbDrYGI  (int)(YDbDrYGF * (1 << Shift) + 0.5)
#define YDbDrYBI  (int)(YDbDrYBF * (1 << Shift) + 0.5)
#define YDbDrDbRI  (int)(YDbDrDbRF * (1 << Shift) + 0.5)
#define YDbDrDbGI  (int)(YDbDrDbGF * (1 << Shift) + 0.5)
#define YDbDrDbBI  (int)(YDbDrDbBF * (1 << Shift) + 0.5)
#define YDbDrDrRI  (int)(YDbDrDrRF * (1 << Shift) + 0.5)
#define YDbDrDrGI  (int)(YDbDrDrGF * (1 << Shift) + 0.5)
#define YDbDrDrBI  (int)(YDbDrDrBF * (1 << Shift) + 0.5)

#define RGBRYI  (int)(RGBRYF * (1 << Shift) + 0.5)              // YDbDr转RGB的系数(整数类型）
#define RGBRDbI  (int)(RGBRDbF * (1 << Shift) + 0.5)
#define RGBRDrI  (int)(RGBRDrF * (1 << Shift) + 0.5)
#define RGBGYI  (int)(RGBGYF * (1 << Shift) + 0.5)
#define RGBGDbI  (int)(RGBGDbF * (1 << Shift) + 0.5)
#define RGBGDrI  (int)(RGBGDrF * (1 << Shift) + 0.5)
#define RGBBYI  (int)(RGBBYF * (1 << Shift) + 0.5)
#define RGBBDbI  (int)(RGBBDbF * (1 << Shift) + 0.5)
#define RGBBDrI  (int)(RGBBDrF * (1 << Shift) + 0.5)

int RGBToYDbDr(const byte* From, byte* To, int Length)
{
  const byte* End = From + Length * 3;
  int Red, Green, Blue;

  if (Length < 1) {
    return 0;
  }

  while (From != End) {
    Blue = *From;
    Green = *(From + 1);
    Red = *(From + 2);
    // 无需判断是否存在溢出，因为测试过整个RGB空间的所有颜色值，无颜色存在溢出
    *To = (byte)((YDbDrYRI * Red + YDbDrYGI * Green + YDbDrYBI * Blue + HalfShiftValue) >> Shift);                      // YDbDr和YUV的Y相同
    *(To + 1) = (byte)(128 + ((YDbDrDbRI * Red + YDbDrDbGI * Green + YDbDrDbBI * Blue + HalfShiftValue) >> Shift));
    *(To + 2) = (byte)(128 + ((YDbDrDrRI * Red + YDbDrDrGI * Green + YDbDrDrBI * Blue + HalfShiftValue) >> Shift));
    From += 3;
    To += 3;
  }

  return 0;
}

int YDbDrToRGB(const byte* From, byte* To, int Length)
{
  const byte* End = From + Length * 3;
  int Red, Green, Blue;
  int Y, Db, Dr;

  if (Length < 1) {
    return 0;
  }

  while (From != End) {
    Y = *From;
    Db = *(From + 1) - 128;
    Dr = *(From + 2) - 128;
    Red = Y + ((RGBRDbI * Db + RGBRDrI * Dr + HalfShiftValue) >> Shift);
    Green = Y + ((RGBGDbI * Db + RGBGDrI * Dr + HalfShiftValue) >> Shift);
    Blue = Y + ((RGBBDbI * Db + RGBBDrI * Dr + HalfShiftValue) >> Shift);

    *To = (byte)CLAMP255(Blue);
    *(To + 1) = (byte)CLAMP255(Green);
    *(To + 2) = (byte)CLAMP255(Red);
    From += 3;
    To += 3;
  }

  return 0;
}


#undef Threshold