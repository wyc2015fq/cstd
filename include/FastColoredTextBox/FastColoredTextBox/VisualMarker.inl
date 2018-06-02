
struct VisualMarker
{
  VisualMarker() {}
  Rectangle rectangle;
  
  VisualMarker(Rectangle rectangle)
  {
    this->rectangle = rectangle;
  }
  
  virtual void Draw(Graphics* gr, Pen pen)
  {
  }
  
  virtual Cursor Cursor()
  {
    return Cursors.Hand;
  }
};

struct CollapseFoldingMarker: VisualMarker
{
  typedef VisualMarker base;
  int iLine;
  
  CollapseFoldingMarker(int iLine, Rectangle rectangle)
    : base(rectangle)
  {
    this->iLine = iLine;
  }
  
  virtual void Draw(Graphics* gr, Pen* pen, Brush* backgroundBrush, Pen* forePen)
  {
    //draw minus
    FillRectangle(gr, backgroundBrush, rectangle);
    DrawRectangle(gr, pen, rectangle);
    DrawLine(gr, forePen, rectangle.X + 2, rectangle.Y + rectangle.Height / 2, rectangle.GetRight() - 2, rectangle.Y + rectangle.Height / 2);
  }
};

struct ExpandFoldingMarker : VisualMarker
{
  int iLine;
  typedef VisualMarker base;
  
  ExpandFoldingMarker(int iLine, Rectangle rectangle)
    : base(rectangle)
  {
    this->iLine = iLine;
  }
  
  void Draw(Graphics* gr, Pen* pen,  Brush* backgroundBrush, Pen* forePen)
  {
    //draw plus
    FillRectangle(gr, backgroundBrush, rectangle);
    DrawRectangle(gr, pen, rectangle);
    DrawLine(gr, forePen, rectangle.X + 2, rectangle.Y + rectangle.Height / 2, rectangle.GetRight() - 2, rectangle.Y + rectangle.Height / 2);
    DrawLine(gr, forePen, rectangle.X + rectangle.Width / 2, rectangle.Y + 2, rectangle.X + rectangle.Width / 2, rectangle.GetBottom() - 2);
  }
};

struct FoldedAreaMarker : VisualMarker
{
  readonly int iLine;
  typedef VisualMarker base;
  
  FoldedAreaMarker(int iLine, Rectangle rectangle)
    : base(rectangle)
  {
    this->iLine = iLine;
  }
  
  void Draw(Graphics* gr, Pen* pen)
  {
    DrawRectangle(gr, pen, rectangle);
  }
};

struct StyleVisualMarker : VisualMarker
{
  typedef VisualMarker base;
  Style* Style;
  
  StyleVisualMarker() {}
  StyleVisualMarker(Rectangle rectangle, Style* style)
    : base(rectangle)
  {
    this->Style = style;
  }
};

struct VisualMarkerEventArgs : MouseEventArgs
{
  typedef MouseEventArgs base;
  Style* Style;
  StyleVisualMarker Marker;
  
  VisualMarkerEventArgs(Style* style, StyleVisualMarker marker, MouseEventArgs args)
    : base(args.Button, args.Clicks, args.X, args.Y, args.Delta)
  {
    this->Style = style;
    this->Marker = marker;
  }
};

