#ifndef _MATHTOOLS_INL_
#define _MATHTOOLS_INL_

/// <summary>
///   Set of mathematical tools.
/// </summary>

//#region Framework-wide random number generator
//private static Random random = new Random();

/// <summary>
///   Gets a reference to the random number generator used
///   internally by the Accord.NET classes and methods.
/// </summary>
//static Random Random { get { return random; } }

/// <summary>
///   Sets a random seed for the internal number generator.
/// </summary>
/// <remarks>
///   This method is only available in the debug version
///   of the framework.
/// </remarks>
static void SetRandomSeed(int seed)
{
    srand(seed);
}

/// <summary>
///   Returns the next power of 2 after the input value x.
/// </summary>
/// <param name="x">Input value x.</param>
/// <returns>Returns the next power of 2 after the input value x.</returns>
static int NextPowerOf2(int x)
{
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return ++x;
}

/// <summary>
///   Returns the previous power of 2 after the input value x.
/// </summary>
/// <param name="x">Input value x.</param>
/// <returns>Returns the previous power of 2 after the input value x.</returns>
static int PreviousPowerOf2(int x)
{
    return NextPowerOf2(x + 1) / 2;
}


/// <summary>
///   Hypotenuse calculus without overflow/underflow
/// </summary>
/// <param name="a">first value</param>
/// <param name="b">second value</param>
/// <returns>The hypotenuse Sqrt(a^2 + b^2)</returns>
static double Hypotenuse(double a, double b)
{
    double r = 0.0;
    double absA = fabs(a);
    double absB = fabs(b);

    if (absA > absB)
    {
        r = b / a;
        r = absA * sqrt(1 + r * r);
    }
    else if (b != 0)
    {
        r = a / b;
        r = absB * sqrt(1 + r * r);
    }

    return r;
}

/// <summary>
///   Gets the proper modulus operation for
///   a integer x and modulo m.
/// </summary>
static int Mod(int x, int m)
{
  int r;
    if (m < 0) m = -m;
    r = x % m;
    return r < 0 ? r + m : r;
}

typedef struct IntRange {
  int Length;
  int Min;
  int Max;
} IntRange;

typedef struct DoubleRange {
  double Length;
  double Min;
  double Max;
} DoubleRange;

/// <summary>
///   Converts the value x (which is measured in the scale
///   'from') to another value measured in the scale 'to'.
/// </summary>
static int iScale(IntRange from, IntRange to, int x)
{
    if (from.Length == 0) return 0;
    return (to.Length) * (x - from.Min) / from.Length + to.Min;
}

/// <summary>
///   Converts the value x (which is measured in the scale
///   'from') to another value measured in the scale 'to'.
/// </summary>
static double dScale(DoubleRange from, DoubleRange to, double x)
{
    if (from.Length == 0) return 0;
    return (to.Length) * (x - from.Min) / from.Length + to.Min;
}

/// <summary>
///   Converts the value x (which is measured in the scale
///   'from') to another value measured in the scale 'to'.
/// </summary>
static double Scale(double fromMin, double fromMax, double toMin, double toMax, double x)
{
    if (fromMax - fromMin == 0) return 0;
    return (toMax - toMin) * (x - fromMin) / (fromMax - fromMin) + toMin;
}

/// <summary>
///   Returns the hyperbolic arc cosine of the specified value.
/// </summary>
static double Acosh(double x)
{
    if (x < 1.0)
        ArgumentOutOfRangeException("x");
    return log(x + sqrt(x * x - 1));
}

/// <summary>
/// Returns the hyperbolic arc sine of the specified value.
/// </summary>
static double Asinh(double d)
{
    double x;
    int sign;

    if (d == 0.0)
        return d;

    if (d < 0.0)
    {
        sign = -1;
        x = -d;
    }
    else
    {
        sign = 1;
        x = d;
    }
    return sign * log(x + sqrt(x * x + 1));
}

/// <summary>
/// Returns the hyperbolic arc tangent of the specified value.
/// </summary>
static double Atanh(double d)
{
    if (d > 1.0 || d < -1.0)
        ArgumentOutOfRangeException("d");
    return 0.5 * log((1.0 + d) / (1.0 - d));
}



/// <summary>
///   Returns the factorial falling power of the specified value.
/// </summary>
static int FactorialPower(int value, int degree)
{
    int i, t = value;
    for (i = 0; i < degree; i++)
        t *= degree--;
    return t;
}

/// <summary>
///   Truncated power function.
/// </summary>
static double TruncatedPower(double value, double degree)
{
    double x = pow(value, degree);
    return (x > 0) ? x : 0.0;
}


#endif // _MATHTOOLS_INL_
