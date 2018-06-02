
using System.Drawing;
using System.Drawing.Printing;


{
    /// <summary>
    /// Item of autocomplete menu
    /// </summary>
    struct AutocompleteItem
    {
        string Text;
        int ImageIndex = -1;
        object Tag;
        string toolTipTitle;
        string toolTipText;
        string menuText;
        AutocompleteMenu Parent { get; internal set; }
        

        AutocompleteItem()
        {
        }

        AutocompleteItem(string text)
        {
            Text = text;
        }

        AutocompleteItem(string text, int imageIndex)
            : this(text)
        {
            this->ImageIndex = imageIndex;
        }

        AutocompleteItem(string text, int imageIndex, string menuText)
            : this(text, imageIndex)
        {
            this->menuText = menuText;
        }

        AutocompleteItem(string text, int imageIndex, string menuText, string toolTipTitle, string toolTipText)
            : this(text, imageIndex, menuText)
        {
            this->toolTipTitle = toolTipTitle;
            this->toolTipText = toolTipText;
        }

        /// <summary>
        /// Returns text for inserting into Textbox
        /// </summary>
        virtual string GetTextForReplace()
        {
            return Text;
        }

        /// <summary>
        /// Compares fragment text with this item
        /// </summary>
        virtual CompareResult Compare(string fragmentText)
        {
            if (Text.StartsWith(fragmentText, StringComparison.InvariantCultureIgnoreCase) &&
                   Text != fragmentText)
                return CompareResult.VisibleAndSelected;

            return CompareResult.Hidden;
        }

        /// <summary>
        /// Returns text for display into popup menu
        /// </summary>
        override string ToString()
        {
            return menuText ?? Text;
        }

        /// <summary>
        /// This method is called after item inserted into text
        /// </summary>
        virtual void OnSelected(AutocompleteMenu popupMenu, SelectedEventArgs e)
        {
            ;
        }

        /// <summary>
        /// Title for tooltip.
        /// </summary>
        /// <remarks>Return null for disable tooltip for this item</remarks>
        virtual string ToolTipTitle
        {
            get { return toolTipTitle; }
            set { toolTipTitle = value; }
        }

        /// <summary>
        /// Tooltip text.
        /// </summary>
        /// <remarks>For display tooltip text, ToolTipTitle must be not null</remarks>
        virtual string ToolTipText 
        {
            get{ return toolTipText; }
            set { toolTipText = value; }
        }

        /// <summary>
        /// Menu text. This text is displayed in the drop-down menu.
        /// </summary>
        virtual string MenuText
        {
            get { return menuText; }
            set { menuText = value; }
        }

        /// <summary>
        /// Fore color of text of item
        /// </summary>
        virtual Color ForeColor
        {
            get { return Color.Transparent; }
            set { throw new NotImplementedException("Override this property to change color"); }
        }

        /// <summary>
        /// Back color of item
        /// </summary>
        virtual Color BackColor
        {
            get { return Color.Transparent; }
            set { throw new NotImplementedException("Override this property to change color"); }
        }
    }

    enum CompareResult
    {
        /// <summary>
        /// Item do not appears
        /// </summary>
        Hidden,
        /// <summary>
        /// Item appears
        /// </summary>
        Visible,
        /// <summary>
        /// Item appears and will selected
        /// </summary>
        VisibleAndSelected
    }

    /// <summary>
    /// Autocomplete item for code snippets
    /// </summary>
    /// <remarks>Snippet can contain special char ^ for caret position.</remarks>
    struct SnippetAutocompleteItem : AutocompleteItem
    {
        SnippetAutocompleteItem(string snippet)
        {
            Text = snippet.Replace("\r", "");
            ToolTipTitle = "Code snippet:";
            ToolTipText = Text;
        }

        override string ToString()
        {
            return MenuText ?? Text.Replace("\n", " ").Replace("^", "");
        }

        override string GetTextForReplace()
        {
            return Text;
        }

        override void OnSelected(AutocompleteMenu popupMenu, SelectedEventArgs e)
        {
            e.Tb.BeginUpdate();
            e.Tb.Selection.BeginUpdate();
            //remember places
            var p1 = popupMenu.Fragment.Start;
            var p2 = e.Tb.Selection.Start;
            //do auto indent
            if (e.Tb.AutoIndent)
            {
                for (int iLine = p1.iLine + 1; iLine <= p2.iLine; iLine++)
                {
                    e.Tb.Selection.Start = new Place(0, iLine);
                    e.Tb.DoAutoIndent(iLine);
                }
            }
            e.Tb.Selection.Start = p1;
            //move caret position right and find char ^
            while (e.Tb.Selection.CharBeforeStart != '^')
                if (!e.Tb.Selection.GoRightThroughFolded())
                    break;
            //remove char ^
            e.Tb.Selection.GoLeft(true);
            e.Tb.InsertText("");
            //
            e.Tb.Selection.EndUpdate();
            e.Tb.EndUpdate();
        }

        /// <summary>
        /// Compares fragment text with this item
        /// </summary>
        override CompareResult Compare(string fragmentText)
        {
            if (Text.StartsWith(fragmentText, StringComparison.InvariantCultureIgnoreCase) &&
                   Text != fragmentText)
                return CompareResult.Visible;

            return CompareResult.Hidden;
        }
    }

    /// <summary>
    /// This autocomplete item appears after dot
    /// </summary>
    struct MethodAutocompleteItem : AutocompleteItem
    {
        string firstPart;
        string lowercaseText;

        MethodAutocompleteItem(string text)
            : base(text)
        {
            lowercaseText = Text.ToLower();
        }

        override CompareResult Compare(string fragmentText)
        {
            int i = fragmentText.LastIndexOf('.');
            if (i < 0)
                return CompareResult.Hidden;
            string lastPart = fragmentText.Substring(i + 1);
            firstPart = fragmentText.Substring(0, i);

            if(lastPart=="") return CompareResult.Visible;
            if(Text.StartsWith(lastPart, StringComparison.InvariantCultureIgnoreCase))
                return CompareResult.VisibleAndSelected;
            if(lowercaseText.Contains(lastPart.ToLower()))
                return CompareResult.Visible;

            return CompareResult.Hidden;
        }

        override string GetTextForReplace()
        {
            return firstPart + "." + Text;
        }
    }

    /// <summary>
    /// This Item does not check correspondence to current text fragment.
    /// SuggestItem is intended for dynamic menus.
    /// </summary>
    struct SuggestItem : AutocompleteItem
    {
        SuggestItem(string text, int imageIndex):base(text, imageIndex)
        {   
        }

        override CompareResult Compare(string fragmentText)
        {
            return CompareResult.Visible;
        }
    }
}
