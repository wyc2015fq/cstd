


namespace FastColoredTextBoxNS
{

#ifdef Styles32
  /// <summary>
  /// Style index mask (32 styles)
  /// </summary>
  enum StyleIndex
  {
    StyleIndexNone = 0,
    StyleIndexStyle0 = 0x1,
    StyleIndexStyle1 = 0x2,
    StyleIndexStyle2 = 0x4,
    StyleIndexStyle3 = 0x8,
    StyleIndexStyle4 = 0x10,
    StyleIndexStyle5 = 0x20,
    StyleIndexStyle6 = 0x40,
    StyleIndexStyle7 = 0x80,
    StyleIndexStyle8 = 0x100,
    StyleIndexStyle9 = 0x200,
    StyleIndexStyle10 = 0x400,
    StyleIndexStyle11 = 0x800,
    StyleIndexStyle12 = 0x1000,
    StyleIndexStyle13 = 0x2000,
    StyleIndexStyle14 = 0x4000,
    StyleIndexStyle15 = 0x8000,

    StyleIndexStyle16 = 0x10000,
    StyleIndexStyle17 = 0x20000,
    StyleIndexStyle18 = 0x40000,
    StyleIndexStyle19 = 0x80000,
    StyleIndexStyle20 = 0x100000,
    StyleIndexStyle21 = 0x200000,
    StyleIndexStyle22 = 0x400000,
    StyleIndexStyle23 = 0x800000,
    StyleIndexStyle24 = 0x1000000,
    StyleIndexStyle25 = 0x2000000,
    StyleIndexStyle26 = 0x4000000,
    StyleIndexStyle27 = 0x8000000,
    StyleIndexStyle28 = 0x10000000,
    StyleIndexStyle29 = 0x20000000,
    StyleIndexStyle30 = 0x40000000,
    StyleIndexStyle31 = 0x80000000,
    StyleIndexAll = 0xffffffff
  };
#else
  /// <summary>
  /// Style index mask (16 styles)
  /// </summary>
  
  enum StyleIndex
  {
    StyleIndexNone = 0,
    StyleIndexStyle0 = 0x1,
    StyleIndexStyle1 = 0x2,
    StyleIndexStyle2 = 0x4,
    StyleIndexStyle3 = 0x8,
    StyleIndexStyle4 = 0x10,
    StyleIndexStyle5 = 0x20,
    StyleIndexStyle6 = 0x40,
    StyleIndexStyle7 = 0x80,
    StyleIndexStyle8 = 0x100,
    StyleIndexStyle9 = 0x200,
    StyleIndexStyle10 = 0x400,
    StyleIndexStyle11 = 0x800,
    StyleIndexStyle12 = 0x1000,
    StyleIndexStyle13 = 0x2000,
    StyleIndexStyle14 = 0x4000,
    StyleIndexStyle15 = 0x8000,
    StyleIndexAll = 0xffff
  };
#endif


    /// <summary>
    /// Char and style
    /// </summary>
    struct Char
    {
        /// <summary>
        /// Unicode character
        /// </summary>
        wchar_t c;
        /// <summary>
        /// Style bit mask
        /// </summary>
        /// <remarks>Bit 1 in position n means that this char will rendering by FastColoredTextBox.Styles[n]</remarks>
        StyleIndex style;

        Char(char c)
        {
            this->c = c;
            style = StyleIndexNone;
        }
    };
}
