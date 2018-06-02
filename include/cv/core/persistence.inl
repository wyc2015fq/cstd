
//#include "cfile.h"

//typedef void* gzFile;
//#include <vector>

#if USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 0
#  endif
#  include <zlib.h>
#endif

/****************************************************************************************\
*                            Common macros and type definitions                          *
\****************************************************************************************/

//class Base64Writer;

enum State
{
    Uncertain,
    NotUse,
    InUse,
};


#define CC_XML_OPENING_TAG 1
#define CC_XML_CLOSING_TAG 2
#define CC_XML_EMPTY_TAG 3
#define CC_XML_HEADER_TAG 4
#define CC_XML_DIRECTIVE_TAG 5


static const size_t HEADER_SIZE         = 24U;
static const size_t ENCODED_HEADER_SIZE = 32U;

/* base64 */

typedef uchar uint8_t;

#if 0
extern uint8_t const base64_padding;
extern uint8_t const base64_mapping[65];
extern uint8_t const base64_demapping[127];

size_t base64_encode(uint8_t const * src, uint8_t * dst, size_t off,      size_t cnt);
size_t base64_encode(   char const * src,    char * dst, size_t off CC_DEFAULT(0U), size_t cnt CC_DEFAULT(0U));

size_t base64_decode(uint8_t const * src, uint8_t * dst, size_t off,      size_t cnt);
size_t base64_decode(   char const * src,    char * dst, size_t off CC_DEFAULT(0U), size_t cnt CC_DEFAULT(0U));

bool   base64_valid (uint8_t const * src, size_t off,      size_t cnt);
bool   base64_valid (   char const * src, size_t off CC_DEFAULT(0U), size_t cnt CC_DEFAULT(0U));

size_t base64_encode_buffer_size(size_t cnt, bool is_end_with_zero CC_DEFAULT(true));

size_t base64_decode_buffer_size(size_t cnt, bool is_end_with_zero CC_DEFAULT(true));
size_t base64_decode_buffer_size(size_t cnt, char  const * src, bool is_end_with_zero CC_DEFAULT(true));
size_t base64_decode_buffer_size(size_t cnt, uchar const * src, bool is_end_with_zero CC_DEFAULT(true));

/* binary */

template<typename _uint_t>      inline size_t to_binary(_uint_t       val, uchar * cur);
template<>                      inline size_t to_binary(double        val, uchar * cur);
template<>                      inline size_t to_binary(float         val, uchar * cur);
template<typename _primitive_t> inline size_t to_binary(uchar const * val, uchar * cur);

template<typename _uint_t>      inline size_t binary_to(uchar const * cur, _uint_t & val);
template<>                      inline size_t binary_to(uchar const * cur, double  & val);
template<>                      inline size_t binary_to(uchar const * cur, float   & val);
template<typename _primitive_t> inline size_t binary_to(uchar const * cur, uchar   * val);

class RawDataToBinaryConvertor;

class BinaryToCvSeqConvertor;
/* class */

class Base64ContextParser
{
public:
    explicit Base64ContextParser(uchar * buffer, size_t size);
    ~Base64ContextParser();
    Base64ContextParser & read(const uchar * beg, const uchar * end);
    bool flush();
private:
    enum { BUFFER_LEN = 120U };
    uchar * dst_cur;
    uchar * dst_end;
    std::vector<uchar> base64_buffer;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_end;
    std::vector<uchar> binary_buffer;
};

class Base64ContextEmitter;

class Base64Writer
{
public:
    Base64Writer(::CFileStorage * fs);
    ~Base64Writer();
    void write(const void* _data, size_t len, const char* dt);
    template<typename _to_binary_convertor_t> void write(_to_binary_convertor_t & convertor, const char* dt);

private:
    void check_dt(const char* dt);

private:

    Base64ContextEmitter * emitter;
    std::string data_type_string;
};

/* other */

std::string make_base64_header(const char * dt);

bool read_base64_header(const char* header, std::string & dt);

//void make_seq(void * binary_data, int elem_cnt, const char * dt, CvSeq & seq);

/* sample */

void cvWriteRawDataBase64(::CFileStorage* fs, const void* _data, int len, const char* dt);

#endif

static void icvPutc( CFileStorage* fs, int c )
{
    stream_putc( fs->file, c );
}

static void icvWrite( CFileStorage* fs, const void* buf, int len )
{
    stream_write( fs->file, buf, len );
}

static void icvPuts( CFileStorage* fs, const char* str )
{
    stream_puts( fs->file, str );
}

static char* icvGets( CFileStorage* fs, char* str, int maxCount )
{
    return stream_gets( str, maxCount, fs->file );
}

static int icvEof( CFileStorage* fs )
{
        return stream_eof(fs->file);
}

static void icvCloseFile( CFileStorage* fs )
{
    stream_close( fs->file );
}

static void icvRewind( CFileStorage* fs )
{
  stream_rewind(fs->file);
}

CC_IMPL void icvWriteIndent(CFileStorage* fs) {
  if (fs->new_line) {
    int i;
    const char* ispace = "                             ";
    int nspace = strlen(ispace);
    int n = fs->write_stack_top*2;
    icvPutc(fs, '\n');
    for (i=0; i<n; ) {
      int k = MIN(nspace, n-i);
      icvWrite(fs, ispace, k);
      i += k;
    }
    fs->new_line = 0;
  }
}

#define CC_YML_INDENT  3
#define CC_XML_INDENT  2
#define CC_YML_INDENT_FLOW  1
#define CC_FS_MAX_LEN 4096

#define CC_FILE_STORAGE ('Y' + ('A' << 8) + ('M' << 16) + ('L' << 24))
#define CC_IS_FILE_STORAGE(fs) ((fs) != 0 && (fs)->flags == CC_FILE_STORAGE)

#define CC_CHECK_FILE_STORAGE(fs)                       \
{                                                       \
    if( !CC_IS_FILE_STORAGE(fs) )                       \
        CC_Error( (fs) ? CC_StsBadArg : CC_StsNullPtr,  \
                  "Invalid pointer to file storage" );  \
}

#define CC_CHECK_OUTPUT_FILE_STORAGE(fs)                \
{                                                       \
    CC_CHECK_FILE_STORAGE(fs);                          \
    if( !fs->write_mode )                               \
        CC_Error( CC_StsError, "The file storage is opened for reading" ); \
}

CC_IMPL const str_t*
cvAttrValue( const vstr_t* attr, const char* attr_name )
{
    int i;
    str_t s1 = STR1(attr_name);
    for ( i=0; i<attr->n; i+=2 )
    {
        if( str_cmp( s1, attr->v[i], 1 ) == 0 )
            return attr->v+i+1;
    }

    return 0;
}


static CGenericHash*
cvCreateMap( CGenericHash* map, int flags, int header_size, int elem_size, int start_tab_size )
{
    if( header_size < (int)sizeof(CGenericHash) )
        CC_Error( CC_StsBadSize, "Too small map header_size" );

    if( start_tab_size <= 0 )
        start_tab_size = 16;

    map->tab_size = start_tab_size;
    MYREALLOC( map->table, start_tab_size );
    MEMSET( map->table, 0, start_tab_size );

    return map;
}

#define CC_PARSE_ERROR( errmsg )                                    \
    icvParseError( fs, CC_Func, (errmsg), __FILE__, __LINE__ )

static void
icvParseError( CTextInfo* fs, const char* func_name,
               const char* err_msg, const char* source_file, int source_line )
{
    char buf[1<<10];
    sprintf( buf, "%s(%d): (%d) %s", source_file, source_line, fs->lineno, err_msg );
    CC_Error( CC_StsParseError, buf );
}

inline char* icvFSResizeWriteBuffer( CFileStorage* fs, const char* ptr, int len )
{
    return (char*)ptr;
}


static char*
icvFSFlush( CFileStorage* fs )
{
    return 0;
}


static void
icvClose( CFileStorage* fs )
{
}

#define MEM_BLOCK_ALLOC_STRING(storage, ptr, len)   mem_block_alloc_string(&storage, ptr, len, __FILE__, __LINE__)
CC_IMPL str_t
mem_block_alloc_string( mem_block_t** storage, const char* ptr, int len, const char* file, int line )
{
    str_t str = {0};

    str.len = len >= 0 ? len : (int)strlen(ptr);
    str.ptr = (char*)mem_block_alloc( storage, 0, str.len + 1, file, line );
    memcpy( str.ptr, ptr, str.len );
    str.ptr[str.len] = '\0';

    return str;
}


#define CC_HASHVAL_SCALE 33

static unsigned cvStringHash(const char* str, int len) {
    unsigned hashval = 0;
    int i;
    if( len < 0 )
    {
        for( i = 0; str[i] != '\0'; i++ )
            hashval = hashval*CC_HASHVAL_SCALE + (unsigned char)str[i];
        len = i;
    }
    else for( i = 0; i < len; i++ )
        hashval = hashval*CC_HASHVAL_SCALE + (unsigned char)str[i];
  return hashval;
}

CC_IMPL CStringHashNode*
cvGetHashedKey( CGenericHash* str_hash, const char* str, int len CC_DEFAULT(-1), int create_missing CC_DEFAULT(0) )
{
    CStringHashNode* node = 0;
    unsigned hashval = 0;
    int i, tab_size;

    if( !str_hash )
        return 0;

    CStringHash* map = str_hash;

    hashval = cvStringHash(str, len);
    hashval &= INT_MAX;
    tab_size = map->tab_size;
    if( (tab_size & (tab_size - 1)) == 0 )
        i = (int)(hashval & (tab_size - 1));
    else
        i = (int)(hashval % tab_size);

    for( node = (CStringHashNode*)(map->table[i]); node != 0; node = node->next )
    {
        if( node->hashval == hashval &&
            node->str.len == len &&
            memcmp( node->str.ptr, str, len ) == 0 )
            break;
    }

    if( !node && create_missing )
    {
        node = MEM_BLOCK_ALLOC( CStringHashNode, map->storage, 0, 1);
        node->hashval = hashval;
        node->str = MEM_BLOCK_ALLOC_STRING( map->storage, str, len );
        node->next = (CStringHashNode*)(map->table[i]);
        map->table[i] = node;
    }

    return node;
}


CC_IMPL CFileNode*
cvGetFileNode( CTextInfo* fs, CFileNode** _map_node, const char* key, int keylen, int create_missing )
{
    int attempts = 1;
    CFileNode* prev = NULL;
    unsigned hashval = cvStringHash(key, keylen);
    keylen = keylen>0 ? keylen : strlen(key);

    if( !key )
        CC_Error( CC_StsNullPtr, "Null key element" );

    for( ; (*_map_node); _map_node = &(*_map_node)->next )
    {
        prev = (*_map_node);
        if( (*_map_node)->hashval == hashval && 0==memcpy((*_map_node)->str.s, key, keylen) )
        {
            if( !create_missing )
            {
                return (*_map_node);
            }
            CC_PARSE_ERROR( "Duplicated key" );
        }

    }

    if( (*_map_node) == NULL && create_missing )
    {
        (*_map_node) = MEM_BLOCK_ALLOC( CFileNode, fs->storage, 0, 1);
        (*_map_node)->str = MEM_BLOCK_ALLOC_STRING(fs->storage, key, keylen);
        (*_map_node)->next = NULL;
        //(*_map_node)->prev = prev;
        if (prev) {
          prev->next = (*_map_node);
        }
    }

    return (*_map_node);
}


CC_IMPL CFileNode*
cvGetFileNodeByName( const CTextInfo* fs, const CFileNode* _map_node, const char* str )
{
    return cvGetFileNode((CTextInfo*)fs, (CFileNode**)&_map_node, str, -1, false);
}

static char*
icvDoubleToString( char* buf, double value )
{
    suf64_t val;
    unsigned ieee754_hi;

    val.f = value;
    ieee754_hi = (unsigned)(val.u >> 32);

    if( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        int ivalue = cRound(value);
        if( ivalue == value )
            sprintf( buf, "%d.", ivalue );
        else
        {
            static const char* fmt = "%.16e";
            char* ptr = buf;
            sprintf( buf, fmt, value );
            if( *ptr == '+' || *ptr == '-' )
                ptr++;
            for( ; cc_isdigit(*ptr); ptr++ )
                ;
            if( *ptr == ',' )
                *ptr = '.';
        }
    }
    else
    {
        unsigned ieee754_lo = (unsigned)val.u;
        if( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            strcpy( buf, ".Nan" );
        else
            strcpy( buf, (int)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    return buf;
}


static char*
icvFloatToString( char* buf, float value )
{
    suf32_t val;
    unsigned ieee754;
    val.f = value;
    ieee754 = val.u;

    if( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        int ivalue = cRound(value);
        if( ivalue == value )
            sprintf( buf, "%d.", ivalue );
        else
        {
            static const char* fmt = "%.8e";
            char* ptr = buf;
            sprintf( buf, fmt, value );
            if( *ptr == '+' || *ptr == '-' )
                ptr++;
            for( ; cc_isdigit(*ptr); ptr++ )
                ;
            if( *ptr == ',' )
                *ptr = '.';
        }
    }
    else
    {
        if( (ieee754 & 0x7fffffff) != 0x7f800000 )
            strcpy( buf, ".Nan" );
        else
            strcpy( buf, (int)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    return buf;
}


static void
icvProcessSpecialDouble( const char* buf, double* value, char** endptr )
{
    char c = buf[0];
    int inf_hi = 0x7ff00000;

    if( c == '-' || c == '+' )
    {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    if( c != '.' )
        printf( "Bad format of floating-point constant" );

    union{double d; uint64 i;} v;
    v.d = 0.;
    if( toupper(buf[1]) == 'I' && toupper(buf[2]) == 'N' && toupper(buf[3]) == 'F' )
        v.i = (uint64)inf_hi << 32;
    else if( toupper(buf[1]) == 'N' && toupper(buf[2]) == 'A' && toupper(buf[3]) == 'N' )
        v.i = (uint64)-1;
    else
        printf( "Bad format of floating-point constant" );
    *value = v.d;

    *endptr = (char*)buf + 4;
}


static double icc_strtod( const char* ptr, char** endptr )
{
    double fval = strtod( ptr, endptr );
    if( **endptr == '.' )
    {
        char* dot_pos = *endptr;
        *dot_pos = ',';
        double fval2 = strtod( ptr, endptr );
        *dot_pos = '.';
        if( *endptr > dot_pos )
            fval = fval2;
        else
            *endptr = dot_pos;
    }

    if( *endptr == ptr || cc_isalpha(**endptr) )
        icvProcessSpecialDouble( ptr, &fval, endptr );

    return fval;
}

// this function will convert "aa?bb&cc&dd" to {"aa", "bb", "cc", "dd"}
static int analyze_file_name( const char* file_name, str_t* result, int max_result )
{
    static const char not_file_name       = '\n';
    static const char parameter_begin     = '?';
    static const char parameter_separator = '&';
    int i=0;

    if ( strchr(file_name, not_file_name) != NULL )
        return 0;

    char* beg = (char*)strrchr(file_name, parameter_begin);

    if ( beg != NULL )
    {
        char* end = (char*)file_name + strlen(file_name);
        result[i++] = STR2(file_name, beg-file_name);
        beg++;
        char* param_beg = beg, *param_end = beg;
        for ( ; param_end < end && i<max_result; param_beg = param_end + 1 )
        {
            param_end = strchr( param_beg, parameter_separator );
            if ( (param_end == NULL || param_end != param_beg) && param_beg + 1U < end )
            {
                result[i++] = STR2( param_beg, param_end - param_beg );
            }
        }
    }

    return i;
}

static bool is_param_exist( const str_t* params, int params_size, const str_t param )
{
    if ( params_size < 2U )
        return false;

    return strv_find(params, params_size, param, 0, 0)>=0;
}

#if 0
static void switch_to_Base64_state( CFileStorage* fs, State state )
{
    const char * err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    const char * err_unable_to_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    switch (fs->state_of_writing_base64)
    {
    case Uncertain:
        switch (state)
        {
        case InUse:
            CC_DbgAssert( fs->base64_writer == 0 );
            fs->base64_writer = new Base64Writer( fs );
            break;
        case Uncertain:
            break;
        case NotUse:
            break;
        default:
            CC_Error( CC_StsError, err_unkonwn_state );
            break;
        }
        break;
    case InUse:
        switch (state)
        {
        case InUse:
        case NotUse:
            CC_Error( CC_StsError, err_unable_to_switch );
            break;
        case Uncertain:
            delete fs->base64_writer;
            fs->base64_writer = 0;
            break;
        default:
            CC_Error( CC_StsError, err_unkonwn_state );
            break;
        }
        break;
    case NotUse:
        switch (state)
        {
        case InUse:
        case NotUse:
            CC_Error( CC_StsError, err_unable_to_switch );
            break;
        case Uncertain:
            break;
        default:
            CC_Error( CC_StsError, err_unkonwn_state );
            break;
        }
        break;
    default:
        CC_Error( CC_StsError, err_unkonwn_state );
        break;
    }

    fs->state_of_writing_base64 = state;

}


static void check_if_write_struct_is_delayed( CFileStorage* fs, bool change_type_to_base64 = false )
{
    if ( fs->is_write_struct_delayed )
    {
        /* save data to prevent recursive call errors */
        std::string struct_key;
        std::string type_name;
        int struct_flags = fs->delayed_struct_flags;

        if ( fs->delayed_struct_key != 0 && *fs->delayed_struct_key != '\0' )
        {
            struct_key.assign(fs->delayed_struct_key);
        }
        if ( fs->delayed_type_name != 0 && *fs->delayed_type_name != '\0' )
        {
            type_name.assign(fs->delayed_type_name);
        }

        /* reset */
        delete[] fs->delayed_struct_key;
        delete[] fs->delayed_type_name;
        fs->delayed_struct_key   = 0;
        fs->delayed_struct_flags = 0;
        fs->delayed_type_name    = 0;

        fs->is_write_struct_delayed = false;

        /* call */
        if ( change_type_to_base64 )
        {
            fs->start_write_struct( fs, struct_key, struct_flags, "binary");
            if ( fs->state_of_writing_base64 != Uncertain )
                switch_to_Base64_state( fs, Uncertain );
            switch_to_Base64_state( fs, InUse );
        }
        else
        {
            fs->start_write_struct( fs, struct_key, struct_flags, type_name);
            if ( fs->state_of_writing_base64 != Uncertain )
                switch_to_Base64_state( fs, Uncertain );
            switch_to_Base64_state( fs, NotUse );
        }
    }
}

static void make_write_struct_delayed(CFileStorage* fs,const char* key, int struct_flags,const char* type_name )
{
    CC_Assert( fs->is_write_struct_delayed == false );
    CC_DbgAssert( fs->delayed_struct_key   == 0 );
    CC_DbgAssert( fs->delayed_struct_flags == 0 );
    CC_DbgAssert( fs->delayed_type_name    == 0 );

    fs->delayed_struct_flags = struct_flags;

    if ( key != 0 )
    {
        fs->delayed_struct_key = new char[strlen(key) + 1U];
        strcpy(fs->delayed_struct_key, key);
    }

    if ( type_name != 0 )
    {
        fs->delayed_type_name = new char[strlen(type_name) + 1U];
        strcpy(fs->delayed_type_name, type_name);
    }

    fs->is_write_struct_delayed = true;
}
#endif


static const size_t PARSER_BASE64_BUFFER_SIZE = 1024U * 1024U / 8U;


#include "persistence_yml.inl"
#include "persistence_xml.inl"
//#include "persistence_json.inl"

/****************************************************************************************\
*                              Common High-Level Functions                               *
\****************************************************************************************/

#if 0
// , 
CC_IMPL void
adsf( const char* query, bool mem, bool write_mode, bool append ) {
    const char * filename = query;
    int params_size = 0;
    str_t params[20] = {0};
    char buf[1100];
    bool write_base64 = (write_mode || append) && (flags & CC_STORAGE_BASE64) != 0;
    if ( !mem )
    {
        params_size = analyze_file_name( query, params, countof(params) );
        if ( params_size>0 ) {
            filename = strncpy( buf, params[0].s, MIN(params[0].l, countof(buf)-1) );
        }

        if ( write_base64 == false && is_param_exist( params, params_size, STR1("base64") ) )
            write_base64 = (write_mode || append);
    }
    size_t fnamelen = 0;
    if( !filename || filename[0] == '\0' )
    {
        if( !write_mode )
            CC_Error( CC_StsNullPtr, mem ? "NULL or empty filename" : "NULL or empty buffer" );
        mem = true;
    }
    else
        fnamelen = strlen(filename);

    if( !mem )
    {
        //fs->filename = (char*)cvMemStorageAlloc( fs->memstorage, fnamelen+1 );
        //strcpy( fs->filename, filename );

        char* dot_pos = strrchr(filename, '.');
        char compression = '\0';

        if( dot_pos && dot_pos[1] == 'g' && dot_pos[2] == 'z' &&
            (dot_pos[3] == '\0' || (cc_isdigit(dot_pos[3]) && dot_pos[4] == '\0')) )
        {
            if( append )
            {
                cvReleaseFileStorage( &fs );
                CC_Error(CC_StsNotImplemented, "Appending data to compressed file is not implemented" );
            }
            isGZ = true;
            compression = dot_pos[3];
            if( compression )
                dot_pos[3] = '\0', fnamelen--;
        }

        if( !isGZ )
        {
            fstream_init(fs->file, fopen(filename, !fs->write_mode ? "rt" : !append ? "wt" : "a+t") );
            if( !fs->file->x )
                return NULL;
        }
        else
        {
            cvReleaseFileStorage( &fs );
            CC_Error(CC_StsNotImplemented, "There is no compressed file storage support in this configuration");
        }
    }
    if( fs->write_mode )
    {
        if( fmt == CC_STORAGE_FORMAT_AUTO && filename )
        {
            const char* dot_pos = strrchr( filename, '.' );
            fs->fmt
                = cc_strcasecmp( dot_pos, ".xml" )
                ? CC_STORAGE_FORMAT_XML
                : cc_strcasecmp( dot_pos, ".json" )
                ? CC_STORAGE_FORMAT_JSON
                : CC_STORAGE_FORMAT_YAML
                ;
        }
        else if ( fmt != CC_STORAGE_FORMAT_AUTO )
        {
            fs->fmt = fmt;
        }
        else
        {
            fs->fmt = CC_STORAGE_FORMAT_XML;
        }
    }
}
#endif

CC_IMPL void
cvFileStorageEnd( CFileStorage* fs ) {
    icvPuts( fs, "<\\opencc_storage>" );
}


CC_IMPL void
cvFileStorageBegin( CFileStorage* fs, int flags, const char* encoding = NULL )
{
    int default_block_size = 1 << 18;
    bool append = (flags & 3) == CC_STORAGE_APPEND;
    bool mem = (flags & CC_STORAGE_MEMORY) != 0;
    bool write_mode = (flags & 3) != 0;
    bool isGZ = false;
    char buf[1100];

    write_mode = true;
    if( mem && append )
        CC_Error( CC_StsBadFlag, "CC_STORAGE_APPEND and CC_STORAGE_MEMORY are not currently compatible" );

    //fs->memstorage = cvCreateMemStorage( default_block_size );
    //fs->dststorage = dststorage ? dststorage : fs->memstorage;

    fs->write_mode = write_mode;
    fs->struct_indent = 0;


    {
        int fmt = flags & CC_STORAGE_FORMAT_MASK;

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))

        if( append )
            stream_seek( fs->file, 0, SEEK_END );

        fmt = CC_STORAGE_FORMAT_XML;
        if( fmt == CC_STORAGE_FORMAT_XML )
        {
            static CFileStruct icvXML[1] = {
#define FUNDEF(ret, name, args)  icvXML ## name,
  CFILESTRUCT_FUNDEFDEF(FUNDEF)
#undef FUNDEF
            };
            fs->vfun = icvXML;
            if( encoding )
            {
                CC_Assert( strlen(encoding) < 1000 );
                snprintf(buf, countof(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                icvPuts( fs, buf );
            }
            else
                icvPuts( fs, "<?xml version=\"1.0\"?>\n" );
            icvPuts( fs, "<opencc_storage>" );
            
        }
#if 0
        else if( fs->fmt == CC_STORAGE_FORMAT_YAML )
        {
            if( !append )
                icvPuts( fs, "%YAML:1.0\n---\n" );
            else
                icvPuts( fs, "...\n---\n" );
            fs->start_write_struct = icvYMLStartWriteStruct;
            fs->end_write_struct = icvYMLEndWriteStruct;
            fs->write_int = icvYMLWriteInt;
            fs->write_real = icvYMLWriteReal;
            fs->write_string = icvYMLWriteString;
            fs->write_comment = icvYMLWriteComment;
            fs->start_next_stream = icvYMLStartNextStream;
        }
        else if( fs->fmt == CC_STORAGE_FORMAT_JSON )
        {
            if( !append )
                icvPuts( fs, "{\n" );
            else
            {
                bool valid = false;
                long roffset = 0;
                for ( ;
                      fseek( fs->file, roffset, SEEK_END ) == 0;
                      roffset -= 1 )
                {//{
                    const char end_mark = '}';
                    if ( fgetc( fs->file ) == end_mark )
                    {
                        fseek( fs->file, roffset, SEEK_END );
                        valid = true;
                        break;
                    }
                }

                if ( valid )
                {
                    icvCloseFile( fs );
                    fs->file = fopen( fs->filename, "r+t" );
                    fseek( fs->file, roffset, SEEK_END );
                    fputs( ",", fs->file );
                }
                else
                {
                    CC_Error( CC_StsError, "Could not find '}' in the end of file.\n" );
                }
            }
            fs->struct_indent = 4;
            fs->start_write_struct = icvJSONStartWriteStruct;
            fs->end_write_struct = icvJSONEndWriteStruct;
            fs->write_int = icvJSONWriteInt;
            fs->write_real = icvJSONWriteReal;
            fs->write_string = icvJSONWriteString;
            fs->write_comment = icvJSONWriteComment;
            fs->start_next_stream = icvJSONStartNextStream;
        }
#endif
    }

}


CC_IMPL int
cvGetStorageFormat(const str_t* s) {
  size_t buf_size = 1 << 20;
  const char* yaml_signature = "%YAML";
  const char* json_signature = "{";//}
  const char* xml_signature  = "<?xml";
  char buf[16] = {0};
  memcpy( buf, s->s, MIN(s->l, sizeof(buf)-2) );
  char* bufPtr = cc_skip_BOM(buf);
  size_t bufOffset = bufPtr - buf;
  int fmt = 0;
  
  if(strncmp( bufPtr, yaml_signature, strlen(yaml_signature) ) == 0)
    fmt = CC_STORAGE_FORMAT_YAML;
  else if(strncmp( bufPtr, json_signature, strlen(json_signature) ) == 0)
    fmt = CC_STORAGE_FORMAT_JSON;
  else if(strncmp( bufPtr, xml_signature, strlen(xml_signature) ) == 0)
    fmt = CC_STORAGE_FORMAT_XML;
  else if(s->l  == bufOffset)
    CC_Error(CC_BADARG_ERR, "Input file is empty");
  else
    CC_Error(CC_BADARG_ERR, "Unsupported file storage format");
  
  //mode = cvGetErrMode();
  //cvSetErrMode( CC_ErrModeSilent );
  //switch (fmt) {
  //case CC_STORAGE_FORMAT_XML : { icvXMLParse ( fs ); break; }
  //case CC_STORAGE_FORMAT_YAML: { icvYMLParse ( fs ); break; }
  //case CC_STORAGE_FORMAT_JSON: { icvJSONParse( fs ); break; }
  //default: break;    }
  return fmt;
}

CC_IMPL void
cvWriteStructBegin( CFileStorage* fs, const char* key, 
                    const char* type_name CC_DEFAULT(NULL), vstr_t* *attributes CC_DEFAULT(NULL) )
{
  CXMLStackRecord* parent;
  fs->vfun->WriteStructBegin( fs, key, type_name );
  parent = fs->write_stack + fs->write_stack_top++;
  
  fs->struct_indent += CC_XML_INDENT;
  
  if( key )
  {
    fs->struct_tag = STR1( key );
  }
  else
  {
    fs->struct_tag.ptr = 0;
    fs->struct_tag.len = 0;
  }
  parent->struct_indent = fs->struct_indent;
  parent->struct_tag = fs->struct_tag;
}


CC_IMPL void
cvWriteStructEnd( CFileStorage* fs )
{
  CXMLStackRecord *parent;
  if( fs->write_stack_top <= 0 )
      CC_Error( CC_StsError, "An extra closing tag" );
  --fs->write_stack_top;

  parent = fs->write_stack + fs->write_stack_top;

  fs->struct_indent = parent->struct_indent;
  fs->struct_tag = parent->struct_tag;

  fs->vfun->WriteStructEnd( fs );
}


CC_IMPL void
cvWriteInt( CFileStorage* fs, const char* key, int value )
{
  fs->vfun->WriteInt( fs, key, value );
}


CC_IMPL void
cvWriteReal( CFileStorage* fs, const char* key, double value )
{
    fs->vfun->WriteReal( fs, key, value );
}


CC_IMPL void
cvWriteString( CFileStorage* fs, const char* key, const char* value, int quote CC_DEFAULT(0) )
{
    fs->vfun->WriteString( fs, key, value, quote );
}


CC_IMPL void
cvWriteComment( CFileStorage* fs, const char* comment, int eol_comment )
{
    fs->vfun->WriteComment( fs, comment, eol_comment );
}


CC_IMPL void
cvStartNextStream( CFileStorage* fs )
{
  while( fs->write_stack_top > 0 ) {
    cvWriteStructEnd(fs);
  }
  cvWriteComment(fs, "next stream", 0);
}

static const char icvTypeSymbol[] = "ucwsifdr";

#define CC_FS_MAX_FMT_PAIRS  128


static char*
icvEncodeFormat( TypeId elem_type, int cn, char* dt )
{
    sprintf( dt, "%d%c", cn, icvTypeSymbol[elem_type] );
    return dt + ( dt[2] == '\0' && dt[0] == '1' );
}

static int
icvDecodeFormat( const char* dt, int* fmt_pairs, int max_len )
{
    int fmt_pair_count = 0;
    int i = 0, k = 0, len = dt ? (int)strlen(dt) : 0;

    if( !dt || !len )
        return 0;

    assert( fmt_pairs != 0 && max_len > 0 );
    fmt_pairs[0] = 0;
    max_len *= 2;

    for( ; k < len; k++ )
    {
        char c = dt[k];

        if( cc_isdigit(c) )
        {
            int count = c - '0';
            if( cc_isdigit(dt[k+1]) )
            {
                char* endptr = 0;
                count = (int)strtol( dt+k, &endptr, 10 );
                k = (int)(endptr - dt) - 1;
            }

            if( count <= 0 )
                CC_Error( CC_StsBadArg, "Invalid data type specification" );

            fmt_pairs[i] = count;
        }
        else
        {
            const char* pos = strchr( icvTypeSymbol, c );
            if( !pos )
                CC_Error( CC_StsBadArg, "Invalid data type specification" );
            if( fmt_pairs[i] == 0 )
                fmt_pairs[i] = 1;
            fmt_pairs[i+1] = (int)(pos - icvTypeSymbol);
            if( i > 0 && fmt_pairs[i+1] == fmt_pairs[i-1] )
                fmt_pairs[i-2] += fmt_pairs[i];
            else
            {
                i += 2;
                if( i >= max_len )
                    CC_Error( CC_StsBadArg, "Too long data type specification" );
            }
            fmt_pairs[i] = 0;
        }
    }

    fmt_pair_count = i/2;
    return fmt_pair_count;
}


static int
icvCalcElemSize( const char* dt, int initial_size )
{
    int size = 0;
    int fmt_pairs[CC_FS_MAX_FMT_PAIRS];
    int i, fmt_pair_count;
    int comp_size;

    fmt_pair_count = icvDecodeFormat( dt, fmt_pairs, CC_FS_MAX_FMT_PAIRS );
    fmt_pair_count *= 2;
    for( i = 0, size = initial_size; i < fmt_pair_count; i += 2 )
    {
        TypeId type = (TypeId)fmt_pairs[i+1];
        comp_size = CC_TYPE_SIZE(type);
        size = cvAlign( size, comp_size );
        size += comp_size * fmt_pairs[i];
    }
    if( initial_size == 0 )
    {
        comp_size = CC_TYPE_SIZE((TypeId)fmt_pairs[1]);
        size = cvAlign( size, comp_size );
    }
    return size;
}


static int
icvCalcStructSize( const char* dt, int initial_size )
{
    int size = icvCalcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    for ( const char * type = dt; *type != '\0'; type++ ) {
        switch ( *type )
        {
        case 'u': { elem_max_size = MAX( elem_max_size, sizeof(uchar ) ); break; }
        case 'c': { elem_max_size = MAX( elem_max_size, sizeof(schar ) ); break; }
        case 'w': { elem_max_size = MAX( elem_max_size, sizeof(ushort) ); break; }
        case 's': { elem_max_size = MAX( elem_max_size, sizeof(short ) ); break; }
        case 'i': { elem_max_size = MAX( elem_max_size, sizeof(int   ) ); break; }
        case 'f': { elem_max_size = MAX( elem_max_size, sizeof(float ) ); break; }
        case 'd': { elem_max_size = MAX( elem_max_size, sizeof(double) ); break; }
        default: break;
        }
    }
    size = cvAlign( size, (int)(elem_max_size) );
    return size;
}


static int
icvDecodeSimpleFormat( const char* dt )
{
    int elem_type = -1;
    int fmt_pairs[CC_FS_MAX_FMT_PAIRS], fmt_pair_count;

    fmt_pair_count = icvDecodeFormat( dt, fmt_pairs, CC_FS_MAX_FMT_PAIRS );
    if( fmt_pair_count != 1 || fmt_pairs[0] > 4 )
        CC_Error( CC_StsError, "Too complex format for the matrix" );

    elem_type = CC_MAKETYPECN( (TypeId)fmt_pairs[1], fmt_pairs[0] );

    return elem_type;
}

CC_IMPL void
cvWriteRawData( CFileStorage* fs, const void* _data, int len, const char* dt )
{
    const char* data0 = (const char*)_data;
    int offset = 0;
    int fmt_pairs[CC_FS_MAX_FMT_PAIRS*2], k, fmt_pair_count;
    char buf[256] = "";

    if( len < 0 )
        CC_Error( CC_StsOutOfRange, "Negative number of elements" );

    fmt_pair_count = icvDecodeFormat( dt, fmt_pairs, CC_FS_MAX_FMT_PAIRS );

    if( !len )
        return;

    if( !data0 )
        CC_Error( CC_StsNullPtr, "Null data pointer" );

    if( fmt_pair_count == 1 )
    {
        fmt_pairs[0] *= len;
        len = 1;
    }

    for(;len--;)
    {
        for( k = 0; k < fmt_pair_count; k++ )
        {
            int i, count = fmt_pairs[k*2];
            TypeId elem_type = (TypeId)fmt_pairs[k*2+1];
            int elem_size = CC_TYPE_SIZE(elem_type);
            const char* data, *ptr;

            offset = cvAlign( offset, elem_size );
            data = data0 + offset;

            for( i = 0; i < count; i++ )
            {
                switch( elem_type )
                {
                case CC_8U:
                    ptr = icc_itoa( *(uchar*)data, buf, 10 );
                    data++;
                    break;
                case CC_8S:
                    ptr = icc_itoa( *(char*)data, buf, 10 );
                    data++;
                    break;
                case CC_16U:
                    ptr = icc_itoa( *(ushort*)data, buf, 10 );
                    data += sizeof(ushort);
                    break;
                case CC_16S:
                    ptr = icc_itoa( *(short*)data, buf, 10 );
                    data += sizeof(short);
                    break;
                case CC_32U:
                case CC_32S:
                    ptr = icc_itoa( *(int*)data, buf, 10 );
                    data += sizeof(int);
                    break;
                case CC_32F:
                    ptr = icvFloatToString( buf, *(float*)data );
                    data += sizeof(float);
                    break;
                case CC_64F:
                    ptr = icvDoubleToString( buf, *(double*)data );
                    data += sizeof(double);
                    break;
                default:
                    CC_Error( CC_StsUnsupportedFormat, "Unsupported type" );
                    return;
                }
                
                fs->vfun->Write( fs, 0, ptr );
            }

            offset = (int)(data - data0);
        }
    }
}

#define CC_TYPE_NAME_MAT "opencv-matrix"
#define CC_TYPE_NAME_MATND "opencv-nd-matrix"
#define CC_TYPE_NAME_SPARSE_MAT "opencv-sparse-matrix"
#define CC_TYPE_NAME_IMAGE "opencv-image"
#define CC_TYPE_NAME_SEQ "opencv-sequence"
#define CC_TYPE_NAME_SEQ_TREE "opencv-sequence-tree"
#define CC_TYPE_NAME_GRAPH "opencv-graph"

/******************************* img_t ******************************/

static void
cvWriteMat( CFileStorage* fs, const char* name,
             const img_t* struct_ptr, vstr_t* attr )
{
    const img_t* mat = (const img_t*)struct_ptr;
    char dt[16];
    CSize size;
    int y;

    cvWriteStructBegin( fs, name, CC_TYPE_NAME_MAT );
    cvWriteInt( fs, "rows", mat->rows );
    cvWriteInt( fs, "cols", mat->cols );
    cvWriteString( fs, "dt", icvEncodeFormat( CC_MAT_DEPTH(mat), CC_MAT_CN(mat), dt ), 0 );
    cvWriteStructBegin( fs, "data" );

    size = cvGetSize(mat);
    if( size.height > 0 && size.width > 0 && mat->data )
    {
        if( CC_IS_MAT_CONT(mat) )
        {
            size.width *= size.height;
            size.height = 1;
        }

        for( y = 0; y < size.height; y++ )
            cvWriteRawData( fs, mat->data + (size_t)y*mat->step, size.width, dt );
    }
    cvWriteStructEnd( fs );
    cvWriteStructEnd( fs );
}
// #include "persistence1.inl"