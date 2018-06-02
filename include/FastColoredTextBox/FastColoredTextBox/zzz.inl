
RTFStyleDescriptor* Style::GetRTF() 
{
  return new RTFStyleDescriptor();
}

virtual void Style::AddVisualMarker(FastColoredTextBox* tb, StyleVisualMarker* marker)
{
  tb->AddVisualMarker(marker);
}
static Size Style::GetSizeOfRange(Range* range)
{
  return Size((range->End.iChar - range->Start.iChar) * range->tb.CharWidth, range->tb.CharHeight);
}
RTFStyleDescriptor* TextStyle::GetRTF()
{
  RTFStyleDescriptor result;
  
#if SUPORT_IS
  if (BackgroundBrush is SolidBrush)
    result.BackColor = (BackgroundBrush as SolidBrush).Color;
  
  if (ForeBrush is SolidBrush)
    result.ForeColor = (ForeBrush as SolidBrush).Color;
#endif
  
  if ((FontStyle & FontStyle.Bold) != 0)
    result.AdditionalTags += "\b";
  if ((FontStyle & FontStyle.Italic) != 0)
    result.AdditionalTags += "\i";
  if ((FontStyle & FontStyle.Strikeout) != 0)
    result.AdditionalTags += "\strike";
  if ((FontStyle & FontStyle.Underline) != 0)
    result.AdditionalTags += "\ul";
  
  return result;
}

void TextStyle::Draw(Graphics* gr, Point position, Range* range)
{
  //draw background
  if (BackgroundBrush != null)
    FillRectangle(gr, BackgroundBrush, position.X, position.Y, (range.End.iChar - range.Start.iChar) * range.tb.CharWidth, range.tb.CharHeight);
  //draw chars
  using(var f = new Font(range.tb.Font, FontStyle))
  {
    Line line = range.tb[range.Start.iLine];
    float dx = range.tb.CharWidth;
    float y = position.Y + range.tb.LineInterval/2;
    float x = position.X - range.tb.CharWidth/3;
    
    if (ForeBrush == null)
      ForeBrush = new SolidBrush(range.tb.ForeColor);
    
    if (range.tb.ImeAllowed)
    {
      //IME mode
      for (int i = range.Start.iChar; i < range.End.iChar; i++)
      {
        SizeF size = FastColoredTextBox.GetCharSize(f, line[i].c);
        
        var gs = gr->Save();
        float k = size.Width > range.tb.CharWidth + 1 ? range.tb.CharWidth/size.Width : 1;
        gr->TranslateTransform(x, y + (1 - k)*range.tb.CharHeight/2);
        gr->ScaleTransform(k, (float) Math.Sqrt(k));
        gr->DrawString(line[i].c.ToString(), f, ForeBrush, 0, 0, stringFormat);
        gr->Restore(gs);
        x += dx;
      }
    }
    else
    {
      //classic mode 
      for (int i = range.Start.iChar; i < range.End.iChar; i++)
      {
        //draw char
        gr->DrawString(line[i].c.ToString(), f, ForeBrush, x, y, stringFormat);
        x += dx;
      }
    }
  }
}
string TextStyle::GetCSS()
{
  string result = "";
  
  if (BackgroundBrush is SolidBrush)
  {
    var s =  ExportToHTML.GetColorAsString((BackgroundBrush as SolidBrush).Color);
    if (s != "")
      result += "background-color:" + s + ";";
  }
  if (ForeBrush is SolidBrush)
  {
    var s = ExportToHTML.GetColorAsString((ForeBrush as SolidBrush).Color);
    if (s != "")
      result += "color:" + s + ";";
  }
  if ((FontStyle & FontStyle.Bold) != 0)
    result += "font-weight:bold;";
  if ((FontStyle & FontStyle.Italic) != 0)
    result += "font-style:oblique;";
  if ((FontStyle & FontStyle.Strikeout) != 0)
    result += "text-decoration:line-through;";
  if ((FontStyle & FontStyle.Underline) != 0)
    result += "text-decoration:underline;";
  
  return result;
}


void FoldedBlockStyle::Draw(Graphics* gr, Point position, Range* range);
{
  if (range.End.iChar > range.Start.iChar)
  {
    base.Draw(gr, position, range);
    
    int firstNonSpaceSymbolX = position.X;
    
    //find first non space symbol
    for (int i = range.Start.iChar; i < range.End.iChar; i++)
      if (range.tb[range.Start.iLine][i].c != ' ')
        break;
      else
        firstNonSpaceSymbolX += range.tb.CharWidth;
      
      //create marker
      range.tb.AddVisualMarker(new FoldedAreaMarker(range.Start.iLine, new Rectangle(firstNonSpaceSymbolX, position.Y, position.X + (range.End.iChar - range.Start.iChar) * range.tb.CharWidth - firstNonSpaceSymbolX, range.tb.CharHeight)));
  }
  else
  {
    //draw '...'
    using(Font f = new Font(range.tb.Font, FontStyle))
      gr->DrawString("...", f, ForeBrush, range.tb.LeftIndent, position.Y - 2);
    //create marker
    range.tb.AddVisualMarker(new FoldedAreaMarker(range.Start.iLine, new Rectangle(range.tb.LeftIndent + 2, position.Y, 2 * range.tb.CharHeight, range.tb.CharHeight)));
  }
}


virtual void SelectionStyle::Draw(Graphics* gr, Point position, Range* range);
{
  //draw background
  if (BackgroundBrush != null)
  {
    gr->SmoothingMode = SmoothingMode.None;
    var rect = new Rectangle(position.X, position.Y, (range.End.iChar - range.Start.iChar) * range.tb.CharWidth, range.tb.CharHeight);
    if (rect.Width == 0)
      return;
    FillRectangle(gr, BackgroundBrush, rect);
    //
    if (ForegroundBrush != null)
    {
      //draw text
      gr->SmoothingMode = SmoothingMode.AntiAlias;
      
      var r = new Range(range.tb, range.Start.iChar, range.Start.iLine,
        Math.Min(range.tb[range.End.iLine].Count, range.End.iChar), range.End.iLine);
      using (var style = new TextStyle(ForegroundBrush, null, FontStyle.Regular))
        style.Draw(gr, new Point(position.X, position.Y - 1), r);
    }
  }
}

virtual void MarkerStyle::Draw(Graphics* gr, Point position, Range* range)
{
  //draw background
  if (BackgroundBrush != null)
  {
    Rectangle rect(position.X, position.Y, (range->End.iChar - range->Start.iChar) * range->tb->CharWidth, range->tb->CharHeight);
    if (rect.Width == 0)
      return;
    FillRectangle(gr, BackgroundBrush, rect);
  }
}

virtual string MarkerStyle::GetCSS()
{
  string result = "";
  
  if (BackgroundBrush is SolidBrush)
  {
    var s = ExportToHTML.GetColorAsString((BackgroundBrush as SolidBrush).Color);
    if (s != "")
      result += "background-color:" + s + ";";
  }
  
  return result;
}


virtual void ShortcutStyle::Draw(Graphics* gr, Point position, Range* range)
{
  //get last char coordinates
  Point p = range.tb.PlaceToPoint(range.End);
  //draw small square under char
  Rectangle rect = new Rectangle(p.X - 5, p.Y + range.tb.CharHeight - 2, 4, 3);
  gr->FillPath(Brushes.White, GetRoundedRectangle(rect, 1));
  gr->DrawPath(borderPen, GetRoundedRectangle(rect, 1));
  //add visual marker for handle mouse events
  AddVisualMarker(range.tb, new StyleVisualMarker(new Rectangle(p.X-range.tb.CharWidth, p.Y, range.tb.CharWidth, range.tb.CharHeight), this));
}

