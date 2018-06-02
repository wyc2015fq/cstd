
#include "sys/datetime.inl"
    /// <summary>
    /// Line of text
    /// </summary>
    struct Line : IList<Char>
    {
        /// <summary>
        /// Chars count
        /// </summary>
        int Count;

        Char* chars;

        string FoldingStartMarker;
        string FoldingEndMarker;
        /// <summary>
        /// Text of line was changed
        /// </summary>
        bool IsChanged;
        /// <summary>
        /// Time of last visit of caret in this line
        /// </summary>
        /// <remarks>This property can be used for forward/backward navigating</remarks>
        uint64 LastVisit;
        /// <summary>
        /// Background brush.
        /// </summary>
        Brush BackgroundBrush;
        /// <summary>
        /// Unique ID
        /// </summary>
        int UniqueId;
        /// <summary>
        /// Count of needed start spaces for AutoIndent
        /// </summary>
        int AutoIndentSpacesNeededCount;

        Line(int uid)
        {
            this->UniqueId = uid;
            chars = NULL;
        }


        /// <summary>
        /// Clears style of chars, delete folding markers
        /// </summary>
        void ClearStyle(StyleIndex styleIndex)
        {
            FoldingStartMarker = null;
            FoldingEndMarker = null;
            for (int i = 0; i < Count; i++)
            {
                Char& c = chars[i];
                c.style &= ~styleIndex;
                chars[i] = c;
            }
        }

        /// <summary>
        /// Text of the line
        /// </summary>
        virtual int getText(ushort* wbuf, int len)
        {
          int i;
          len = MIN(len, Count);
          for (i=0; i<len; ++i) {
            wbuf[i] = chars[i].c;
          }
          return Count;
        }
        
        virtual int getText(char* buf, int len)
        {
          int i;
          len = MIN(len, Count);
          ushort* wbuf = NULL;
          MYREALLOC(wbuf, len+1);
          for (i=0; i<len; ++i) {
            wbuf[i] = chars[i].c;
          }
          sys_wc2mb(CE_GB2312, wbuf, len, buf, len);
          FREE(wbuf);
          return Count;
        }

        /// <summary>
        /// Clears folding markers
        /// </summary>
        void ClearFoldingMarkers()
        {
            FoldingStartMarker = null;
            FoldingEndMarker = null;
        }

        /// <summary>
        /// Count of start spaces
        /// </summary>
        int getStartSpacesCount()
        {
          int spacesCount = 0;
          for (int i = 0; i < Count; i++) {
            if (chars[i].c == L' ')
              spacesCount++;
            else
              break;
          }
          return spacesCount;
        }
        
        Char& get(int index) {
          return chars[index];
        }
        void set(int index, const Char& value) {
          chars[index] = value;
        }
        
#if 0
        int IndexOf(Char item)
        {
            return chars.IndexOf(item);
        }

        void Insert(int index, Char item)
        {
            chars.Insert(index, item);
        }

        void RemoveAt(int index)
        {
            chars.RemoveAt(index);
        }

        void Add(Char item)
        {
            chars.Add(item);
        }

        void Clear()
        {
            chars.Clear();
        }

        bool Contains(Char item)
        {
            return chars.Contains(item);
        }

        void CopyTo(Char* array, int arrayIndex)
        {
            chars.CopyTo(array, arrayIndex);
        }

        bool IsReadOnly
        {
            get {  return false; }
        }

        bool Remove(Char item)
        {
            return chars.Remove(item);
        }

        IEnumerator<Char> GetEnumerator()
        {
            return chars.GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return chars.GetEnumerator() as System.Collections.IEnumerator;
        }

        virtual void RemoveRange(int index, int count)
        {
            if (index >= Count)
                return;
            chars.RemoveRange(index, Math.Min(Count - index, count));
        }

        virtual void TrimExcess()
        {
            chars.TrimExcess();
        }

        virtual void AddRange(IEnumerable<Char> collection)
        {
            chars.AddRange(collection);
        }
    };

    struct LineInfo
    {
        List<int> cutOffPositions;
        //Y coordinate of line on screen
        internal int startY;
        internal int bottomPadding;
        //indent of secondary wordwrap strings (in chars)
        internal int wordWrapIndent;
        /// <summary>
        /// Visible state
        /// </summary>
        VisibleState VisibleState;

        LineInfo(int startY)
        {
            cutOffPositions = null;
            VisibleState = VisibleState.Visible;
            this->startY = startY;
            bottomPadding = 0;
            wordWrapIndent = 0;
        }
        /// <summary>
        /// Positions for wordwrap cutoffs
        /// </summary>
        List<int> CutOffPositions
        {
            get
            {
                if (cutOffPositions == null)
                    cutOffPositions = new List<int>();
                return cutOffPositions;
            }
        }

        /// <summary>
        /// Count of wordwrap string count for this line
        /// </summary>
        int WordWrapStringsCount
        {
            get
            {
                switch (VisibleState)
                {
                    case VisibleState.Visible:
                         if (cutOffPositions == null)
                            return 1;
                         else
                            return cutOffPositions.Count + 1;
                    case VisibleState.Hidden: return 0;
                    case VisibleState.StartOfHiddenBlock: return 1;
                }

                return 0;
            }
        }

        internal int GetWordWrapStringStartPosition(int iWordWrapLine)
        {
            return iWordWrapLine == 0 ? 0 : CutOffPositions[iWordWrapLine - 1];
        }

        internal int GetWordWrapStringFinishPosition(int iWordWrapLine, Line line)
        {
            if (WordWrapStringsCount <= 0)
                return 0;
            return iWordWrapLine == WordWrapStringsCount - 1 ? line.Count - 1 : CutOffPositions[iWordWrapLine] - 1;
        }

        /// <summary>
        /// Gets index of wordwrap string for given char position
        /// </summary>
        int GetWordWrapStringIndex(int iChar)
        {
            if (cutOffPositions == null || cutOffPositions.Count == 0) return 0;
            for (int i = 0; i < cutOffPositions.Count; i++)
                if (cutOffPositions[i] >/*>=*/ iChar)
                    return i;
            return cutOffPositions.Count;
        }
#endif
    };

    enum VisibleState
    {
        Visible, StartOfHiddenBlock, Hidden
    };

    enum IndentMarker
    {
        None,
        Increased,
        Decreased
    };

