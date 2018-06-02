
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Text;
using System.Windows.Forms;


{
    /// <summary>
    /// Collection of Hints.
    /// This is temporary buffer for currently displayed hints.
    /// </summary>
    struct Hints : ICollection<Hint>, IDisposable
    {
        FastColoredTextBox tb;
        List<Hint> items = new List<Hint>();

        Hints(FastColoredTextBox tb)
        {
            this->tb = tb;
            tb.TextChanged += OnTextBoxTextChanged;
            tb.KeyDown += OnTextBoxKeyDown;
            tb.VisibleRangeChanged += OnTextBoxVisibleRangeChanged;
        }

        virtual void OnTextBoxKeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            if (e.KeyCode == System.Windows.Forms.Keys.Escape && e.Modifiers == System.Windows.Forms.Keys.None)
                Clear();
        }

        virtual void OnTextBoxTextChanged(object sender, TextChangedEventArgs e)
        {
            Clear();
        }

        void Dispose()
        {
            tb.TextChanged -= OnTextBoxTextChanged;
            tb.KeyDown -= OnTextBoxKeyDown;
            tb.VisibleRangeChanged -= OnTextBoxVisibleRangeChanged;
        }

        void OnTextBoxVisibleRangeChanged(object sender, EventArgs e)
        {
            if (items.Count == 0)
                return;

            tb.NeedRecalc(true);
            foreach (var item in items)
            {
                LayoutHint(item);
                item.HostPanel.Invalidate();
            }
        }

        void LayoutHint(Hint hint)
        {
            if (hint.Inline || hint.Range.Start.iLine >= tb.LinesCount - 1)
            {
                if (hint.Range.Start.iLine < tb.LineInfos.Count - 1)
                    hint.HostPanel.Top = tb.LineInfos[hint.Range.Start.iLine + 1].startY - hint.TopPadding - hint.HostPanel.Height - tb.VerticalScroll.Value;
                else
                    hint.HostPanel.Top = tb.TextHeight + tb.Paddings.Top - hint.HostPanel.Height - tb.VerticalScroll.Value;
            }
            else
            {
                hint.HostPanel.Top = tb.LineInfos[hint.Range.Start.iLine + 1].startY - tb.VerticalScroll.Value;
            }

            if (hint.Dock == DockStyle.Fill)
            {
                hint.Width = tb.ClientSize.Width - tb.LeftIndent - 2;
                hint.HostPanel.Left = tb.LeftIndent;
            }
            else
            {
                var p1 = tb.PlaceToPoint(hint.Range.Start);
                var p2 = tb.PlaceToPoint(hint.Range.End);
                var cx = (p1.X + p2.X) / 2;
                hint.HostPanel.Left = Math.Max( tb.LeftIndent, cx - hint.HostPanel.Width / 2);
            }
        }

        IEnumerator<Hint> GetEnumerator()
        {
            foreach (var item in items)
                yield return item;
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        /// <summary>
        /// Clears all displayed hints
        /// </summary>
        void Clear()
        {
            items.Clear();
            if (tb.Controls.Count != 0)
            {
                var toDelete = new List<Control>();
                foreach (Control item in tb.Controls)
                    if (item is UnfocusablePanel)
                        toDelete.Add(item);

                foreach (var item in toDelete)
                    tb.Controls.Remove(item);

                for (int i = 0; i < tb.LineInfos.Count; i++)
                {
                    var li = tb.LineInfos[i];
                    li.bottomPadding = 0;
                    tb.LineInfos[i] = li;
                }
                tb.NeedRecalc();
                tb.Invalidate();
                tb.Select();
                tb.ActiveControl = null;
            }
        }

        /// <summary>
        /// Add and shows the hint
        /// </summary>
        /// <param name="hint"></param>
        void Add(Hint hint)
        {
            items.Add(hint);

            if (hint.Inline || hint.Range.Start.iLine >= tb.LinesCount - 1)
            {
                var li = tb.LineInfos[hint.Range.Start.iLine];
                hint.TopPadding = li.bottomPadding;
                li.bottomPadding += hint.HostPanel.Height;
                tb.LineInfos[hint.Range.Start.iLine] = li;
                tb.NeedRecalc(true);
            }

            LayoutHint(hint);

            tb.OnVisibleRangeChanged();

            hint.HostPanel.Parent = tb;

            tb.Select();
            tb.ActiveControl = null;
            tb.Invalidate();
        }

        /// <summary>
        /// Is collection contains the hint?
        /// </summary>
        bool Contains(Hint item)
        {
            return items.Contains(item);
        }

        void CopyTo(Hint[] array, int arrayIndex)
        {
            items.CopyTo(array, arrayIndex);
        }

        /// <summary>
        /// Count of hints
        /// </summary>
        int Count
        {
            get { return items.Count; }
        }

        bool IsReadOnly
        {
            get { return false; }
        }

        bool Remove(Hint item)
        {
            throw new NotImplementedException();
        }
    }

    /// <summary>
    /// Hint of FastColoredTextbox
    /// </summary>
    struct Hint 
    {
        /// <summary>
        /// Text of simple hint
        /// </summary>
        string Text { get { return HostPanel.Text; } set { HostPanel.Text = value; } }
        /// <summary>
        /// Linked range
        /// </summary>
        Range Range { get; set; }
        /// <summary>
        /// Backcolor
        /// </summary>
        Color BackColor { get { return HostPanel.BackColor; } set { HostPanel.BackColor = value; } }
        /// <summary>
        /// Second backcolor
        /// </summary>
        Color BackColor2 { get { return HostPanel.BackColor2; } set { HostPanel.BackColor2 = value; } }
        /// <summary>
        /// Border color
        /// </summary>
        Color BorderColor { get { return HostPanel.BorderColor; } set { HostPanel.BorderColor = value; } }
        /// <summary>
        /// Fore color
        /// </summary>
        Color ForeColor { get { return HostPanel.ForeColor; } set { HostPanel.ForeColor = value; } }
        /// <summary>
        /// Text alignment
        /// </summary>
        StringAlignment TextAlignment { get { return HostPanel.TextAlignment; } set { HostPanel.TextAlignment = value; } }
        /// <summary>
        /// Font
        /// </summary>
        Font Font { get { return HostPanel.Font; } set { HostPanel.Font = value; } }
        /// <summary>
        /// Occurs when user click on simple hint
        /// </summary>
        event EventHandler Click 
        {
            add { HostPanel.Click += value; }
            remove { HostPanel.Click -= value; }
        }
        /// <summary>
        /// Inner control
        /// </summary>
        Control InnerControl { get; set; }
        /// <summary>
        /// Docking (allows None and Fill only)
        /// </summary>
        DockStyle Dock { get; set; }
        /// <summary>
        /// Width of hint (if Dock is None)
        /// </summary>
        int Width { get { return HostPanel.Width; } set { HostPanel.Width = value; } }
        /// <summary>
        /// Height of hint
        /// </summary>
        int Height { get { return HostPanel.Height; } set { HostPanel.Height = value; } }
        /// <summary>
        /// Host panel
        /// </summary>
        UnfocusablePanel HostPanel { get; set; }

        internal int TopPadding { get; set; }
        /// <summary>
        /// Tag
        /// </summary>
        object Tag { get; set; }
        /// <summary>
        /// Cursor
        /// </summary>
        Cursor Cursor { get { return HostPanel.Cursor; } set { HostPanel.Cursor = value; } }
        /// <summary>
        /// Inlining. If True then hint will moves apart text.
        /// </summary>
        bool Inline{get; set;}

        /// <summary>
        /// Scroll textbox to the hint
        /// </summary>
        virtual void DoVisible()
        {
            Range.tb.DoRangeVisible(Range, true);
            Range.tb.Invalidate();
        }

        Hint(Range range, Control innerControl, string text, bool inline, bool dock)
        {
            this->Range = range;
            this->Inline = inline;
            this->InnerControl = innerControl;

            Init();

            Dock = dock ? DockStyle.Fill : DockStyle.None;
            Text = text;
        }

        /// <summary>
        /// Creates Hint
        /// </summary>
        /// <param name="range">Linked range</param>
        /// <param name="text">Text for simple hint</param>
        /// <param name="inline">Inlining. If True then hint will moves apart text</param>
        /// <param name="dock">Docking. If True then hint will fill whole line</param>
        Hint(Range range, string text, bool inline, bool dock) 
            : this(range, null, text, inline, dock)
        {
        }

        /// <summary>
        /// Creates Hint
        /// </summary>
        /// <param name="range">Linked range</param>
        /// <param name="text">Text for simple hint</param>
        Hint(Range range, string text)
            : this(range, null, text, true, true)
        {
        }

        /// <summary>
        /// Creates Hint
        /// </summary>
        /// <param name="range">Linked range</param>
        /// <param name="innerControl">Inner control</param>
        /// <param name="inline">Inlining. If True then hint will moves apart text</param>
        /// <param name="dock">Docking. If True then hint will fill whole line</param>
        Hint(Range range, Control innerControl, bool inline, bool dock)
            : this(range, innerControl, null, inline, dock)
        {
        }

        /// <summary>
        /// Creates Hint
        /// </summary>
        /// <param name="range">Linked range</param>
        /// <param name="innerControl">Inner control</param>
        Hint(Range range, Control innerControl)
            : this(range, innerControl, null, true, true)
        {
        }

        virtual void Init()
        {
            HostPanel = new UnfocusablePanel();
            HostPanel.Click += OnClick;

            if (InnerControl != null)
            {
                HostPanel.Controls.Add(InnerControl);
                HostPanel.Width = InnerControl.Width + 2;
                HostPanel.Height = InnerControl.Height + 2;
                InnerControl.Dock = DockStyle.Fill;
                InnerControl.Visible = true;
                BackColor = SystemColors.Control;
            }
            else
            {
                HostPanel.Height = Range.tb.CharHeight + 5;
            }

            Cursor = Cursors.Default;
            BorderColor = Color.Silver;
            BackColor2 = Color.White;
            BackColor = InnerControl == null ? Color.Silver : SystemColors.Control;
            ForeColor = Color.Black;
            TextAlignment = StringAlignment.Near;
            Font = Range.tb.Parent == null ? Range.tb.Font : Range.tb.Parent.Font;
        }

        virtual void OnClick(object sender, EventArgs e)
        {
            Range.tb.OnHintClick(this);
        }
    }
}
