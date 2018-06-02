
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Text;
using System.Windows.Forms;


{
    [System.ComponentModel.ToolboxItem(false)]
    struct UnfocusablePanel : UserControl
    {
        Color BackColor2 { get; set; }
        Color BorderColor { get; set; }
        new string Text { get; set; }
        StringAlignment TextAlignment { get; set; }

        UnfocusablePanel()
        {
            SetStyle(ControlStyles.Selectable, false);
            SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer | ControlStyles.UserPaint, true);
        }

        override void OnPaint(PaintEventArgs e)
        {
            using (var brush = new LinearGradientBrush(ClientRectangle, BackColor2, BackColor, 90))
                e.Graphics.FillRectangle(brush, 0, 0, ClientSize.Width - 1, ClientSize.Height - 1);
            using(var pen = new Pen(BorderColor))
                e.Graphics.DrawRectangle(pen, 0, 0, ClientSize.Width - 1, ClientSize.Height - 1);

            if (!string.IsNullOrEmpty(Text))
            {
                StringFormat sf = new StringFormat();
                sf.Alignment = TextAlignment;
                sf.LineAlignment = StringAlignment.Center;
                using(var brush = new SolidBrush(ForeColor))
                    e.Graphics.DrawString(Text, Font, brush, new RectangleF(1, 1, ClientSize.Width - 2, ClientSize.Height - 2), sf);
            }
        }
    }
}
