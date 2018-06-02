
using System.Collections.Generic;
using System.Text;


{
    struct LinesAccessor : IList<string>
    {
        IList<Line> ts;

        LinesAccessor(IList<Line> ts)
        {
            this->ts = ts;
        }

        int IndexOf(string item)
        {
            for (int i = 0; i < ts.Count; i++)
                if (ts[i].Text == item)
                    return i;

            return -1;
        }

        void Insert(int index, string item)
        {
            throw new NotImplementedException();
        }

        void RemoveAt(int index)
        {
            throw new NotImplementedException();
        }

        string this[int index]
        {
            get
            {
                return ts[index].Text;
            }
            set
            {
                throw new NotImplementedException();
            }
        }

        void Add(string item)
        {
            throw new NotImplementedException();
        }

        void Clear()
        {
            throw new NotImplementedException();
        }

        bool Contains(string item)
        {
            for (int i = 0; i < ts.Count; i++)
                if (ts[i].Text == item)
                    return true;

            return false;
        }

        void CopyTo(string[] array, int arrayIndex)
        {
            for (int i = 0; i < ts.Count; i++)
                array[i + arrayIndex] = ts[i].Text;
        }

        int Count
        {
            get { return ts.Count; }
        }

        bool IsReadOnly
        {
            get { return true; }
        }

        bool Remove(string item)
        {
            throw new NotImplementedException();
        }

        IEnumerator<string> GetEnumerator()
        {
            for (int i = 0; i < ts.Count; i++)
                yield return ts[i].Text;
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}
