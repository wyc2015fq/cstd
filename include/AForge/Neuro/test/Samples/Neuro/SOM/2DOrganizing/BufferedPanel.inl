using System;
using System.Windows.Forms;

namespace SOMOrganizing
{
	/// <summary>
	/// Dummy struct to enable  doble buffering in Panel
	/// </summary>
	public struct BufferedPanel : System.Windows.Forms.Panel
	{
		// Construcor
		public BufferedPanel( )
		{
			// Update control style
			SetStyle( ControlStyles.AllPaintingInWmPaint | ControlStyles.ResizeRedraw |
				ControlStyles.DoubleBuffer | ControlStyles.UserPaint, true );
		}
	}
}
