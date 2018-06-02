
#ifndef OPENCC_CORE_TRAITS_HPP
#define OPENCC_CORE_TRAITS_HPP

template<typename _Tp> class DataType
{
public:
    typedef _Tp         value_type;
    typedef value_type  work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 1,
           type          = CC_NUL,
           channels     = 1,
           fmt          = 0,
           typecn = CC_MAKETYPECN(type, channels)
         };
};

template<> class DataType<bool>
{
public:
    typedef bool        value_type;
    typedef int         work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 0,
           type          = CC_8U,
           channels     = 1,
           fmt          = (int)'u',
           typecn         = CC_MAKETYPECN(type, channels)
         };
};

template<> class DataType<uchar>
{
public:
    typedef uchar       value_type;
    typedef int         work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 0,
           type          = CC_8U,
           channels     = 1,
           fmt          = (int)'u',
           typecn         = CC_MAKETYPECN(type, channels)
         };
};

template<> class DataType<schar>
{
public:
    typedef schar       value_type;
    typedef int         work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 0,
           type          = CC_8S,
           channels     = 1,
           fmt          = (int)'c',
           typecn         = CC_MAKETYPECN(type, channels)
         };
};

template<> class DataType<char>
{
public:
    typedef schar       value_type;
    typedef int         work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 0,
           type          = CC_8S,
           channels     = 1,
           fmt          = (int)'c',
           typecn         = CC_MAKETYPECN(type, channels)
         };
};

template<> class DataType<ushort>
{
public:
    typedef ushort      value_type;
    typedef int         work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 0,
           type          = CC_16U,
           channels     = 1,
           fmt          = (int)'w',
           typecn         = CC_MAKETYPECN(type, channels)
         };
};

template<> class DataType<short>
{
public:
    typedef short       value_type;
    typedef int         work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 0,
           type          = CC_16S,
           channels     = 1,
           fmt          = (int)'s',
           typecn         = CC_MAKETYPECN(type, channels)
         };
};

template<> class DataType<int>
{
public:
    typedef int         value_type;
    typedef value_type  work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 0,
           type          = CC_32S,
           channels     = 1,
           fmt          = (int)'i',
           typecn         = CC_MAKETYPECN(type, channels)
         };
};

template<> class DataType<float>
{
public:
    typedef float       value_type;
    typedef value_type  work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 0,
           type          = CC_32F,
           channels     = 1,
           fmt          = (int)'f',
           typecn         = CC_MAKETYPECN(type, channels)
         };
};

template<> class DataType<double>
{
public:
    typedef double      value_type;
    typedef value_type  work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 0,
           type          = CC_64F,
           channels     = 1,
           fmt          = (int)'d',
           typecn         = CC_MAKETYPECN(type, channels)
         };
};


/** @brief A helper class for DataType

The class is specialized for each fundamental numerical data typecn supported by OpenCV. It provides
DataDepth<T>::value constant.
*/
template<typename _Tp> class DataDepth
{
public:
    enum
    {
        value = DataType<_Tp>::depth,
        fmt   = DataType<_Tp>::fmt
    };
};



template<int _depth> class TypeDepth
{
    enum { depth = CC_USRTYPE1 };
    typedef void value_type;
};

template<> class TypeDepth<CC_8U>
{
    enum { depth = CC_8U };
    typedef uchar value_type;
};

template<> class TypeDepth<CC_8S>
{
    enum { depth = CC_8S };
    typedef schar value_type;
};

template<> class TypeDepth<CC_16U>
{
    enum { depth = CC_16U };
    typedef ushort value_type;
};

template<> class TypeDepth<CC_16S>
{
    enum { depth = CC_16S };
    typedef short value_type;
};

template<> class TypeDepth<CC_32S>
{
    enum { depth = CC_32S };
    typedef int value_type;
};

template<> class TypeDepth<CC_32F>
{
    enum { depth = CC_32F };
    typedef float value_type;
};

template<> class TypeDepth<CC_64F>
{
    enum { depth = CC_64F };
    typedef double value_type;
};

//! @}


#endif // OPENCC_CORE_TRAITS_HPP
