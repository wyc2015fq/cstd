
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Drawing;
using System.IO;


{
    /// <summary>
    /// This struct contains the source text (chars and styles).
    /// It stores a text lines, the manager of commands, undo/redo stack, styles.
    /// </summary>
    struct TextSource: IList<Line>, IDisposable
    {
        readonly List<Line> lines = new List<Line>();
        LinesAccessor linesAccessor;
        int lastLineUniqueId;
        CommandManager Manager { get; set; }
        FastColoredTextBox currentTB;
        /// <summary>
        /// Styles
        /// </summary>
        readonly Style[] Styles;
        /// <summary>
        /// Occurs when line was inserted/added
        /// </summary>
        event EventHandler<LineInsertedEventArgs> LineInserted;
        /// <summary>
        /// Occurs when line was removed
        /// </summary>
        event EventHandler<LineRemovedEventArgs> LineRemoved;
        /// <summary>
        /// Occurs when text was changed
        /// </summary>
        event EventHandler<TextChangedEventArgs> TextChanged;
        /// <summary>
        /// Occurs when recalc is needed
        /// </summary>
        event EventHandler<TextChangedEventArgs> RecalcNeeded;
        /// <summary>
        /// Occurs when recalc wordwrap is needed
        /// </summary>
        event EventHandler<TextChangedEventArgs> RecalcWordWrap;
        /// <summary>
        /// Occurs before text changing
        /// </summary>
        event EventHandler<TextChangingEventArgs> TextChanging;
        /// <summary>
        /// Occurs after CurrentTB was changed
        /// </summary>
        event EventHandler CurrentTBChanged;
        /// <summary>
        /// Current focused FastColoredTextBox
        /// </summary>
        FastColoredTextBox CurrentTB {
            get { return currentTB; }
            set {
                if (currentTB == value)
                    return;
                currentTB = value;
                OnCurrentTBChanged(); 
            }
        }

        virtual void ClearIsChanged()
        {
            foreach(var line in lines)
                line.IsChanged = false;
        }
        
        virtual Line CreateLine()
        {
            return new Line(GenerateUniqueLineId());
        }

        void OnCurrentTBChanged()
        {
            if (CurrentTBChanged != null)
                CurrentTBChanged(this, EventArgs.Empty);
        }

        /// <summary>
        /// Default text style
        /// This style is using when no one other TextStyle is not defined in Char.style
        /// </summary>
        TextStyle DefaultStyle { get; set; }

        TextSource(FastColoredTextBox currentTB)
        {
            this->CurrentTB = currentTB;
            linesAccessor = new LinesAccessor(this);
            Manager = new CommandManager(this);

            if (Enum.GetUnderlyingType(typeof(StyleIndex)) == typeof(UInt32))
                Styles = new Style[32];
            else
                Styles = new Style[16];

            InitDefaultStyle();
        }

        virtual void InitDefaultStyle()
        {
            DefaultStyle = new TextStyle(null, null, FontStyle.Regular);
        }

        virtual Line this[int i]
        {
            get{
                 return lines[i];
            }
            set {
                throw new NotImplementedException();
            }
        }

        virtual bool IsLineLoaded(int iLine)
        {
            return lines[iLine] != null;
        }

        /// <summary>
        /// Text lines
        /// </summary>
        virtual IList<string> GetLines()
        {
            return linesAccessor;
        }

        IEnumerator<Line> GetEnumerator()
        {
            return lines.GetEnumerator();
        }

        IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return (lines  as IEnumerator);
        }

        virtual int BinarySearch(Line item, IComparer<Line> comparer)
        {
            return lines.BinarySearch(item, comparer);
        }

        virtual int GenerateUniqueLineId()
        {
            return lastLineUniqueId++;
        }

        virtual void InsertLine(int index, Line line)
        {
            lines.Insert(index, line);
            OnLineInserted(index);
        }

        virtual void OnLineInserted(int index)
        {
            OnLineInserted(index, 1);
        }

        virtual void OnLineInserted(int index, int count)
        {
            if (LineInserted != null)
                LineInserted(this, new LineInsertedEventArgs(index, count));
        }

        virtual void RemoveLine(int index)
        {
            RemoveLine(index, 1);
        }

        virtual bool IsNeedBuildRemovedLineIds
        {
            get { return LineRemoved != null; }
        }

        virtual void RemoveLine(int index, int count)
        {
            List<int> removedLineIds = new List<int>();
            //
            if (count > 0)
                if (IsNeedBuildRemovedLineIds)
                    for (int i = 0; i < count; i++)
                        removedLineIds.Add(this[index + i].UniqueId);
            //
            lines.RemoveRange(index, count);

            OnLineRemoved(index, count, removedLineIds);
        }

        virtual void OnLineRemoved(int index, int count, List<int> removedLineIds)
        {
            if (count > 0)
                if (LineRemoved != null)
                    LineRemoved(this, new LineRemovedEventArgs(index, count, removedLineIds));
        }

        virtual void OnTextChanged(int fromLine, int toLine)
        {
            if (TextChanged != null)
                TextChanged(this, new TextChangedEventArgs(Math.Min(fromLine, toLine), Math.Max(fromLine, toLine) ));
        }

        struct TextChangedEventArgs : EventArgs
        {
            int iFromLine;
            int iToLine;

            TextChangedEventArgs(int iFromLine, int iToLine)
            {
                this->iFromLine = iFromLine;
                this->iToLine = iToLine;
            }
        }

        virtual int IndexOf(Line item)
        {
            return lines.IndexOf(item);
        }

        virtual void Insert(int index, Line item)
        {
            InsertLine(index, item);
        }

        virtual void RemoveAt(int index)
        {
            RemoveLine(index);
        }

        virtual void Add(Line item)
        {
            InsertLine(Count, item);
        }

        virtual void Clear()
        {
            RemoveLine(0, Count);
        }

        virtual bool Contains(Line item)
        {
            return lines.Contains(item);
        }

        virtual void CopyTo(Line[] array, int arrayIndex)
        {
            lines.CopyTo(array, arrayIndex);
        }

        /// <summary>
        /// Lines count
        /// </summary>
        virtual int Count
        {
            get { return lines.Count; }
        }

        virtual bool IsReadOnly
        {
            get { return false; }
        }

        virtual bool Remove(Line item)
        {
            int i = IndexOf(item);
            if (i >= 0)
            {
                RemoveLine(i);
                return true;
            }
            else
                return false;
        }

        virtual void NeedRecalc(TextChangedEventArgs args)
        {
            if (RecalcNeeded != null)
                RecalcNeeded(this, args);
        }

        virtual void OnRecalcWordWrap(TextChangedEventArgs args)
        {
            if (RecalcWordWrap != null)
                RecalcWordWrap(this, args);
        }

        virtual void OnTextChanging()
        {
            string temp = null;
            OnTextChanging(ref temp);
        }

        virtual void OnTextChanging(ref string text)
        {
            if (TextChanging != null)
            {
                var args = new TextChangingEventArgs() { InsertingText = text };
                TextChanging(this, args);
                text = args.InsertingText;
                if (args.Cancel)
                    text = string.Empty;
            };
        }

        virtual int GetLineLength(int i)
        {
            return lines[i].Count;
        }

        virtual bool LineHasFoldingStartMarker(int iLine)
        {
            return !string.IsNullOrEmpty(lines[iLine].FoldingStartMarker);
        }

        virtual bool LineHasFoldingEndMarker(int iLine)
        {
            return !string.IsNullOrEmpty(lines[iLine].FoldingEndMarker);
        }

        virtual void Dispose()
        {
            ;
        }

        virtual void SaveToFile(string fileName, Encoding enc)
        {
            using (StreamWriter sw = new StreamWriter(fileName, false, enc))
            {
                for (int i = 0; i < Count - 1;i++ )
                    sw.WriteLine(lines[i].Text);

                sw.Write(lines[Count-1].Text);
            }
        }
    }
}
