



    /// <summary>
    /// Line index and char index
    /// </summary>
    struct Place : IEquatable<Place>
    {
        int iChar;
        int iLine;

        Place() {
          iChar = 0;
          iLine = 0;
        }
        Place(int iChar, int iLine)
        {
            this->iChar = iChar;
            this->iLine = iLine;
        }

        void Offset(int dx, int dy)
        {
            iChar += dx;
            iLine += dy;
        }

        bool Equals(Place other)
        {
            return iChar == other.iChar && iLine == other.iLine;
        }

        template <typename T>
        bool Equals(const T& obj)
        {
            return false;
        }

        int GetHashCode()
        {
          return ::GetHashCode(iChar) ^ ::GetHashCode(iLine);
        }

        static bool operator !=(Place p1, Place p2)
        {
            return !p1.Equals(p2);
        }

        static bool operator ==(Place p1, Place p2)
        {
            return p1.Equals(p2);
        }

        static bool operator <(Place p1, Place p2)
        {
            if (p1.iLine < p2.iLine) return true;
            if (p1.iLine > p2.iLine) return false;
            if (p1.iChar < p2.iChar) return true;
            return false;
        }

        static bool operator <=(Place p1, Place p2)
        {
            if (p1.Equals(p2)) return true;
            if (p1.iLine < p2.iLine) return true;
            if (p1.iLine > p2.iLine) return false;
            if (p1.iChar < p2.iChar) return true;
            return false;
        }

        static bool operator >(Place p1, Place p2)
        {
            if (p1.iLine > p2.iLine) return true;
            if (p1.iLine < p2.iLine) return false;
            if (p1.iChar > p2.iChar) return true;
            return false;
        }

        static bool operator >=(Place p1, Place p2)
        {
            if (p1.Equals(p2)) return true;
            if (p1.iLine > p2.iLine) return true;
            if (p1.iLine < p2.iLine) return false;
            if (p1.iChar > p2.iChar) return true;
            return false;
        }

        static Place operator +(Place p1, Place p2)
        {
            return Place(p1.iChar + p2.iChar, p1.iLine + p2.iLine);
        }

        static Place getEmpty()
        {
            return Place();
        }

        string ToString()
        {
            return string("(") + iChar + "," + iLine + ")";
        }
    };

