


    /// <summary>
    /// Style of chars
    /// </summary>
    /// <remarks>This is base struct for all text and design renderers</remarks>
    struct Style : IDisposable
    {
        /// <summary>
        /// This style is exported to outer formats (HTML for example)
        /// </summary>
      bool IsExportable;
        /// <summary>
        /// Occurs when user click on StyleVisualMarker joined to this style 
        /// </summary>
        void (*VisualMarkerClick)(FastColoredTextBox* tb, VisualMarkerEventArgs* args);

        /// <summary>
        /// Constructor
        /// </summary>
        Style()
        {
            IsExportable = true;
        }

        /// <summary>
        /// Renders given range of text
        /// </summary>
        /// <param name="gr">Graphics object</param>
        /// <param name="position">Position of the range in absolute control coordinates</param>
        /// <param name="range">Rendering range of text</param>
        virtual void Draw(Graphics* gr, Point position, Range* range) = 0;

        /// <summary>
        /// Occurs when user click on StyleVisualMarker joined to this style 
        /// </summary>
        virtual void OnVisualMarkerClick(FastColoredTextBox* tb, VisualMarkerEventArgs* args)
        {
            if (VisualMarkerClick != null)
                VisualMarkerClick(tb, args);
        }

        /// <summary>
        /// Shows VisualMarker
        /// Call this method in Draw method, when you need to show VisualMarker for your style
        /// </summary>
        virtual void AddVisualMarker(FastColoredTextBox* tb, StyleVisualMarker* marker);

        static Size GetSizeOfRange(Range* range);

        static void GetRoundedRectangle(GraphicsPath* gp, Rectangle rect, int d)
        {
            PathReset(gp);
            PathAddArc(gp, rect.X, rect.Y, d, d, 180, 90);
            PathAddArc(gp, rect.X + rect.Width - d, rect.Y, d, d, 270, 90);
            PathAddArc(gp, rect.X + rect.Width - d, rect.Y + rect.Height - d, d, d, 0, 90);
            PathAddArc(gp, rect.X, rect.Y + rect.Height - d, d, d, 90, 90);
            PathAddLine(gp, rect.X, rect.Y + rect.Height - d, rect.X, rect.Y + d / 2);
        }

        virtual void Dispose()
        {
            ;
        }

        /// <summary>
        /// Returns CSS for export to HTML
        /// </summary>
        /// <returns></returns>
        virtual string GetCSS()
        {
            return "";
        }

        /// <summary>
        /// Returns RTF descriptor for export to RTF
        /// </summary>
        /// <returns></returns>
        virtual RTFStyleDescriptor* GetRTF();
    };

    /// <summary>
    /// Style for chars rendering
    /// This renderer can draws chars, with defined fore and back colors
    /// </summary>
    struct TextStyle : Style
    {
        Brush ForeBrush[1];
        Brush BackgroundBrush[1];
        FontStyle FontStyle;
        //public readonly Font Font;
        StringFormat stringFormat[1];

        TextStyle(Brush* foreBrush, Brush* backgroundBrush, FontStyle fontStyle)
        {
            *this->ForeBrush = *foreBrush;
            *this->BackgroundBrush = *backgroundBrush;
            this->FontStyle = fontStyle;
            //stringFormat = new StringFormat(StringFormatFlagsMeasureTrailingSpaces);
        }
        
        virtual void Draw(Graphics* gr, Point position, Range* range);
        
        string GetCSS();
        
        virtual RTFStyleDescriptor* GetRTF();
    };

    /// <summary>
    /// Renderer for folded block
    /// </summary>
    struct FoldedBlockStyle : TextStyle
    {
        FoldedBlockStyle(Brush* foreBrush, Brush* backgroundBrush, FontStyle fontStyle):
            TextStyle(foreBrush, backgroundBrush, fontStyle)
        {
        }

        virtual void Draw(Graphics* gr, Point position, Range* range);
    };

    /// <summary>
    /// Renderer for selected area
    /// </summary>
    struct SelectionStyle : Style
    {
        Brush* BackgroundBrush;
        Brush* ForegroundBrush;

        SelectionStyle(Brush* backgroundBrush, Brush* foregroundBrush = null)
        {
            this->BackgroundBrush = backgroundBrush;
            this->ForegroundBrush = foregroundBrush;
            this->IsExportable = false;
        }
        
        virtual void Draw(Graphics* gr, Point position, Range* range);
    };

    /// <summary>
    /// Marker style
    /// Draws background color for text
    /// </summary>
    struct MarkerStyle : Style
    {
        Brush* BackgroundBrush;

        MarkerStyle(Brush* backgroundBrush)
        {
            this->BackgroundBrush = backgroundBrush;
            IsExportable = true;
        }
        
        virtual void Draw(Graphics* gr, Point position, Range* range);
        
        virtual string GetCSS();
    };

    /// <summary>
    /// Draws small rectangle for popup menu
    /// </summary>
    struct ShortcutStyle : Style
    {
        Pen* borderPen;

        ShortcutStyle(Pen* borderPen)
        {
            this->borderPen = borderPen;
        }
        
        virtual void Draw(Graphics* gr, Point position, Range* range);
    };

    /// <summary>
    /// This style draws a wavy line below a given text range.
    /// </summary>
    /// <remarks>Thanks for Yallie</remarks>
    struct WavyLineStyle : Style
    {
        Pen Pen1[1];

        WavyLineStyle(int alpha, Color color)
        {
          PenSetColor(Pen1, FromArgb(alpha, color));
        }

        virtual void Draw(Graphics* gr, Point pos, Range* range)
        {
            Size size = GetSizeOfRange(range);
            Point start = iPoint(pos.X, pos.Y + size.Height - 1);
            Point end = iPoint(pos.X + size.Width, pos.Y + size.Height - 1);
            DrawWavyLine(gr, start, end);
        }

        void DrawWavyLine(Graphics* gr, Point start, Point end)
        {
            if (end.X - start.X < 2)
            {
                DrawLine(gr, Pen1, start, end);
                return;
            }

            int offset = -1;
            Point* points = NULL;
            int count = end.X-start.X + 1;
            myrealloc(&points, count);

            for (int i = start.X; i <= end.X; i += 2)
            {
                points[i - start.X] = iPoint(i, start.Y + offset);
                offset = -offset;
            }

            DrawLines(gr, Pen1, points, count);
        }

        virtual void Dispose()
        {
          Style::Dispose();

            //if (Pen1 != null) Pen1->Dispose();
        }
    };

    /// <summary>
    /// This style is used to mark range of text as ReadOnly block
    /// </summary>
    /// <remarks>You can inherite this style to add visual effects of readonly text</remarks>
    struct ReadOnlyStyle : Style
    {
        ReadOnlyStyle()
        {
            IsExportable = false;
        }

        virtual void Draw(Graphics* gr, Point position, Range* range)
        {
            //
        }
    };

