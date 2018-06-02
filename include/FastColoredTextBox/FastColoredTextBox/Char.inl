

/// <summary>
/// Char and style
/// </summary>
struct Char
{
  /// <summary>
  /// Unicode character
  /// </summary>
  unsigned short c;
  /// <summary>
  /// Style bit mask
  /// </summary>
  /// <remarks>Bit 1 in position n means that this char will rendering by FastColoredTextBox.Styles[n]</remarks>
  unsigned short style;
  
  Char(char c)
  {
    this->c = c;
    style = StyleIndex_None;
  }
};
