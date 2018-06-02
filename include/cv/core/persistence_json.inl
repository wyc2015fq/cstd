

/****************************************************************************************\
*                                       JSON Parser                                      *
\****************************************************************************************/

static char*
icvJSONSkipSpaces( CFileStorage* fs, const char* ptr )
{
    bool is_eof = false;
    bool is_completed = false;

    while ( is_eof == false && is_completed == false )
    {
        switch ( *ptr )
        {
        /* comment */
        case '/' : {
            ptr++;
            if ( *ptr == '\0' )
            {
                ptr = icvGets( fs, fs->buffer_start, (int)(fs->buffer_end - fs->buffer_start) );
                if ( !ptr ) { is_eof = true; break; }
            }

            if ( *ptr == '/' )
            {
                while ( *ptr != '\n' && *ptr != '\r' )
                {
                    if ( *ptr == '\0' )
                    {
                        ptr = icvGets( fs, fs->buffer_start, (int)(fs->buffer_end - fs->buffer_start) );
                        if ( !ptr ) { is_eof = true; break; }
                    }
                    else
                    {
                        ptr++;
                    }
                }
            }
            else if ( *ptr == '*' )
            {
                ptr++;
                for (;;)
                {
                    if ( *ptr == '\0' )
                    {
                        ptr = icvGets( fs, fs->buffer_start, (int)(fs->buffer_end - fs->buffer_start) );
                        if ( !ptr ) { is_eof = true; break; }
                    }
                    else if ( *ptr == '*' )
                    {
                        ptr++;
                        if ( *ptr == '\0' )
                        {
                            ptr = icvGets( fs, fs->buffer_start, (int)(fs->buffer_end - fs->buffer_start) );
                            if ( !ptr ) { is_eof = true; break; }
                        }
                        if ( *ptr == '/' )
                        {
                            ptr++;
                            break;
                        }
                    }
                    else
                    {
                        ptr++;
                    }
                }
            }
            else
            {
                CC_PARSE_ERROR( "Not supported escape character" );
            }
        } break;
        /* whitespace */
        case '\t':
        case ' ' : {
            ptr++;
        } break;
        /* newline || end mark */
        case '\0':
        case '\n':
        case '\r': {
            ptr = icvGets( fs, fs->buffer_start, (int)(fs->buffer_end - fs->buffer_start) );
            if ( !ptr ) { is_eof = true; break; }
        } break;
        /* other character */
        default: {
            if ( !cc_isprint(*ptr) )
                CC_PARSE_ERROR( "Invalid character in the stream" );
            is_completed = true;
        } break;
        }
    }

    if ( is_eof )
    {
        ptr = fs->buffer_start;
        *ptr = '\0';
        fs->dummy_eof = 1;
    }
    else if ( !is_completed )
    {
        /* should not be executed */
        ptr = 0;
        fs->dummy_eof = 1;
        CC_PARSE_ERROR( "Abort at parse time" );
    }
    return ptr;
}


static char* icvJSONParseKey( CFileStorage* fs, const char* ptr, CFileNode* map, CFileNode** value_placeholder )
{
    if( *ptr != '"' )
        CC_PARSE_ERROR( "Key must start with \'\"\'" );

    char * beg = ptr + 1;
    char * end = beg;

    do ++ptr;
    while( cc_isprint(*ptr) && *ptr != '"' );

    if( *ptr != '"' )
        CC_PARSE_ERROR( "Key must end with \'\"\'" );

    end = ptr;
    ptr++;
    ptr = icvJSONSkipSpaces( fs, ptr );
    if ( ptr == 0 || fs->dummy_eof )
        return 0;

    if( *ptr != ':' )
        CC_PARSE_ERROR( "Missing \':\' between key and value" );

    /* [beg, end) */
    if( end <= beg )
        CC_PARSE_ERROR( "Key is empty" );

    if ( end - beg == 7u && memcmp(beg, "type_id", 7u) == 0 )
    {
        *value_placeholder = 0;
    }
    else
    {
        CStringHashNode* str_hash_node = cvGetHashedKey( fs, beg, (int)(end - beg), 1 );
        *value_placeholder = cvGetFileNode( fs, map, str_hash_node, 1 );
    }

    ptr++;
    return ptr;
}

static char* icvJSONParseValue( CFileStorage* fs, const char* ptr, CFileNode* node )
{
    ptr = icvJSONSkipSpaces( fs, ptr );
    if ( ptr == 0 || fs->dummy_eof )
        CC_PARSE_ERROR( "Unexpected End-Of-File" );

    memset( node, 0, sizeof(*node) );

    if ( *ptr == '"' )
    {   /* must be string or Base64 string */
        ptr++;
        char * beg = ptr;
        size_t len = 0u;
        for ( ; (cc_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
            len++;

        if ( len >= 8u && memcmp( beg, "$base64$", 8u ) == 0 )
        {   /**************** Base64 string ****************/
            ptr = beg += 8;

            std::string base64_buffer;
            base64_buffer.reserve( PARSER_BASE64_BUFFER_SIZE );

            bool is_matching = false;
            while ( !is_matching )
            {
                switch ( *ptr )
                {
                case '\0':
                {
                    base64_buffer.append( beg, ptr );

                    ptr = icvGets( fs, fs->buffer_start, (int)(fs->buffer_end - fs->buffer_start) );
                    if ( !ptr )
                        CC_PARSE_ERROR( "'\"' - right-quote of string is missing" );

                    beg = ptr;
                    break;
                }
                case '\"':
                {
                    base64_buffer.append( beg, ptr );
                    beg = ptr;
                    is_matching = true;
                    break;
                }
                case '\n':
                case '\r':
                {
                    CC_PARSE_ERROR( "'\"' - right-quote of string is missing" );
                    break;
                }
                default:
                {
                    ptr++;
                    break;
                }
                }
            }

            if ( *ptr != '\"' )
                CC_PARSE_ERROR( "'\"' - right-quote of string is missing" );
            else
                ptr++;

            if ( base64_buffer.size() >= ENCODED_HEADER_SIZE )
            {
                const char * base64_beg = base64_buffer.data();
                const char * base64_end = base64_beg + base64_buffer.size();

                /* get dt from header */
                std::string dt;
                {
                    std::vector<char> header(HEADER_SIZE + 1, ' ');
                    base64_decode(base64_beg, header.data(), 0U, ENCODED_HEADER_SIZE);
                    if ( !read_base64_header(header, dt) || dt.empty() )
                        CC_PARSE_ERROR("Invalid `dt` in Base64 header");
                }

                /* set base64_beg to beginning of base64 data */
                base64_beg = &base64_buffer.at( ENCODED_HEADER_SIZE );

                if ( base64_buffer.size() > ENCODED_HEADER_SIZE )
                {
                    if ( !base64_valid( base64_beg, 0U, base64_end - base64_beg ) )
                        CC_PARSE_ERROR( "Invalid Base64 data." );

                    /* buffer for decoded data(exclude header) */
                    std::vector<uchar> binary_buffer( base64_decode_buffer_size(base64_end - base64_beg) );
                    int total_byte_size = (int)(
                        base64_decode_buffer_size( base64_end - base64_beg, base64_beg, false )
                        );
                    {
                        Base64ContextParser parser(binary_buffer.data(), binary_buffer.size() );
                        const uchar * binary_beg = reinterpret_cast<const uchar *>( base64_beg );
                        const uchar * binary_end = binary_beg + (base64_end - base64_beg);
                        parser.read( binary_beg, binary_end );
                        parser.flush();
                    }

                    /* save as CvSeq */
                    int elem_size = ::icvCalcStructSize(dt, 0);
                    if (total_byte_size % elem_size != 0)
                        CC_PARSE_ERROR("Byte size not match elememt size");
                    int elem_cnt = total_byte_size / elem_size;

                    /* after icvFSCreateCollection, node->tag == struct_flags */
                    icvFSCreateCollection(fs, CC_NODE_FLOW | CC_NODE_SEQ, node);
                    make_seq(binary_buffer.data(), elem_cnt, dt, *node->data.seq);
                }
                else
                {
                    /* empty */
                    icvFSCreateCollection(fs, CC_NODE_FLOW | CC_NODE_SEQ, node);
                }
            }
            else if ( base64_buffer.empty() )
            {
                /* empty */
                icvFSCreateCollection(fs, CC_NODE_FLOW | CC_NODE_SEQ, node);
            }
            else
            {
                CC_PARSE_ERROR("Unrecognized Base64 header");
            }
        }
        else
        {   /**************** normal string ****************/
            std::string string_buffer;
            string_buffer.reserve( PARSER_BASE64_BUFFER_SIZE );

            ptr = beg;
            bool is_matching = false;
            while ( !is_matching )
            {
                switch ( *ptr )
                {
                case '\\':
                {
                    string_buffer.append( beg, ptr );
                    ptr++;
                    switch ( *ptr )
                    {
                    case '\\':
                    case '\"':
                    case '\'': { string_buffer.append( 1u, *ptr ); break; }
                    case 'n' : { string_buffer.append( 1u, '\n' ); break; }
                    case 'r' : { string_buffer.append( 1u, '\r' ); break; }
                    case 't' : { string_buffer.append( 1u, '\t' ); break; }
                    case 'b' : { string_buffer.append( 1u, '\b' ); break; }
                    case 'f' : { string_buffer.append( 1u, '\f' ); break; }
                    case 'u' : { CC_PARSE_ERROR( "'\\uXXXX' currently not supported" ); }
                    default  : { CC_PARSE_ERROR( "Invalid escape character" ); }
                        break;
                    }
                    ptr++;
                    beg = ptr;
                    break;
                }
                case '\0':
                {
                    string_buffer.append( beg, ptr );

                    ptr = icvGets( fs, fs->buffer_start, (int)(fs->buffer_end - fs->buffer_start) );
                    if ( !ptr )
                        CC_PARSE_ERROR( "'\"' - right-quote of string is missing" );

                    beg = ptr;
                    break;
                }
                case '\"':
                {
                    string_buffer.append( beg, ptr );
                    beg = ptr;
                    is_matching = true;
                    break;
                }
                case '\n':
                case '\r':
                {
                    CC_PARSE_ERROR( "'\"' - right-quote of string is missing" );
                    break;
                }
                default:
                {
                    ptr++;
                    break;
                }
                }
            }

            if ( *ptr != '\"' )
                CC_PARSE_ERROR( "'\"' - right-quote of string is missing" );
            else
                ptr++;

            node->data.str = cvMemStorageAllocString
            (
                fs->memstorage,
                string_buffer,
                (int)(string_buffer.size())
            );
            node->tag = CC_NODE_STRING;
        }
    }
    else if ( cc_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
    {    /**************** number ****************/
        char * beg = ptr;
        if ( *ptr == '+' || *ptr == '-' )
            ptr++;
        while( cc_isdigit(*ptr) )
            ptr++;
        if (*ptr == '.' || *ptr == 'e')
        {
            node->data.f = icc_strtod( fs, beg, &ptr );
            node->tag = CC_NODE_REAL;
        }
        else
        {
            node->data.i = (int)(strtol( beg, &ptr, 0 ));
            node->tag = CC_NODE_INT;
        }

        if ( beg >= ptr )
            CC_PARSE_ERROR( "Invalid numeric value (inconsistent explicit type specification?)" );
    }
    else
    {    /**************** other data ****************/
        const char * beg = ptr;
        size_t len = 0u;
        for ( ; cc_isalpha(*ptr) && len <= 6u; ptr++ )
            len++;

        if ( len >= 4u && memcmp( beg, "null", 4u ) == 0 )
        {
            CC_PARSE_ERROR( "Value 'null' is not supported by this parser" );
        }
        else if ( len >= 4u && memcmp( beg, "true", 4u ) == 0 )
        {
            node->data.i = 1;
            node->tag = CC_NODE_INT;
        }
        else if ( len >= 5u && memcmp( beg, "false", 5u ) == 0 )
        {
            node->data.i = 0;
            node->tag = CC_NODE_INT;
        }
        else
        {
            CC_PARSE_ERROR( "Unrecognized value" );
        }
        ptr++;
    }

    return ptr;
}

static char* icvJSONParseSeq( CFileStorage* fs, const char* ptr, CFileNode* node );
static char* icvJSONParseMap( CFileStorage* fs, const char* ptr, CFileNode* node );

static char* icvJSONParseSeq( CFileStorage* fs, const char* ptr, CFileNode* node )
{
    if ( *ptr != '[' )
        CC_PARSE_ERROR( "'[' - left-brace of seq is missing" );
    else
        ptr++;

    memset( node, 0, sizeof(*node) );
    icvFSCreateCollection( fs, CC_NODE_SEQ, node );

    for (;;)
    {
        ptr = icvJSONSkipSpaces( fs, ptr );
        if ( ptr == 0 || fs->dummy_eof )
            break;

        if ( *ptr != ']' )
        {
            CFileNode* child = (CFileNode*)cvSeqPush( node->data.seq, 0 );

            if ( *ptr == '[' )
                ptr = icvJSONParseSeq( fs, ptr, child );
            else if ( *ptr == '{' )
                ptr = icvJSONParseMap( fs, ptr, child );
            else
                ptr = icvJSONParseValue( fs, ptr, child );
        }

        ptr = icvJSONSkipSpaces( fs, ptr );
        if ( ptr == 0 || fs->dummy_eof )
            break;

        if ( *ptr == ',' )
            ptr++;
        else if ( *ptr == ']' )
            break;
        else
            CC_PARSE_ERROR( "Unexpected character" );
    }

    if ( *ptr != ']' )
        CC_PARSE_ERROR( "']' - right-brace of seq is missing" );
    else
        ptr++;

    return ptr;
}

static char* icvJSONParseMap( CFileStorage* fs, const char* ptr, CFileNode* node )
{
    if ( *ptr != '{' )
        CC_PARSE_ERROR( "'{' - left-brace of map is missing" );
    else
        ptr++;

    memset( node, 0, sizeof(*node) );
    icvFSCreateCollection( fs, CC_NODE_MAP, node );

    for ( ;; )
    {
        ptr = icvJSONSkipSpaces( fs, ptr );
        if ( ptr == 0 || fs->dummy_eof )
            break;

        if ( *ptr == '"' )
        {
            CFileNode* child = 0;
            ptr = icvJSONParseKey( fs, ptr, node, &child );
            ptr = icvJSONSkipSpaces( fs, ptr );
            if ( ptr == 0 || fs->dummy_eof )
                break;

            if ( child == 0 )
            {   /* type_id */
                CFileNode tmp;
                ptr = icvJSONParseValue( fs, ptr, &tmp );
                if ( CC_NODE_IS_STRING(tmp.tag) )
                {
                    node->info = cvFindType( tmp.data.str.ptr );
                    if ( node->info )
                        node->tag |= CC_NODE_USER;
                    // delete tmp.data.str
                }
                else
                {
                    CC_PARSE_ERROR( "\"type_id\" should be of type string" );
                }
            }
            else
            {   /* normal */
                if ( *ptr == '[' )
                    ptr = icvJSONParseSeq( fs, ptr, child );
                else if ( *ptr == '{' )
                    ptr = icvJSONParseMap( fs, ptr, child );
                else
                    ptr = icvJSONParseValue( fs, ptr, child );
            }
        }

        ptr = icvJSONSkipSpaces( fs, ptr );
        if ( ptr == 0 || fs->dummy_eof )
            break;

        if ( *ptr == ',' )
            ptr++;
        else if ( *ptr == '}' )
            break;
        else
            CC_PARSE_ERROR( "Unexpected character" );
    }

    if ( *ptr != '}' )
        CC_PARSE_ERROR( "'}' - right-brace of map is missing" );
    else
        ptr++;

    return ptr;
}


static void
icvJSONParse( CFileStorage* fs )
{
    char* ptr = fs->buffer_start;
    ptr = icvJSONSkipSpaces( fs, ptr );
    if ( ptr == 0 || fs->dummy_eof )
        return;

    if ( *ptr == '{' )//}
    {
        CFileNode* root_node = (CFileNode*)cvSeqPush( fs->roots, 0 );
        ptr = icvJSONParseMap( fs, ptr, root_node );
    }
    else if ( *ptr == '[' )
    {
        CFileNode* root_node = (CFileNode*)cvSeqPush( fs->roots, 0 );
        ptr = icvJSONParseSeq( fs, ptr, root_node );
    }
    else
    {
        CC_PARSE_ERROR( "left-brace of top level is missing" );
    }

    if ( fs->dummy_eof != 0 )
        CC_PARSE_ERROR( "Unexpected End-Of-File" );
}


/****************************************************************************************\
*                                       JSON Emitter                                     *
\****************************************************************************************/

static void
icvJSONWrite( CFileStorage* fs, const char* key, const char* data )
{
    /* check write_struct */

    check_if_write_struct_is_delayed( fs );
    if ( fs->state_of_writing_base64 == Uncertain )
    {
        switch_to_Base64_state( fs, NotUse );
    }
    else if ( fs->state_of_writing_base64 == InUse )
    {
        CC_Error( CC_StsError, "At present, output Base64 data only." );
    }

    /* check parameters */

    size_t key_len = 0u;
    if( key && *key == '\0' )
        key = 0;
    if ( key )
    {
        key_len = strlen(key);
        if ( key_len == 0u )
            CC_Error( CC_StsBadArg, "The key is an empty" );
        else if ( (int)(key_len) > CC_FS_MAX_LEN )
            CC_Error( CC_StsBadArg, "The key is too long" );
    }

    size_t data_len = 0u;
    if ( data )
        data_len = strlen(data);

    int struct_flags = fs->struct_flags;
    if( CC_NODE_IS_COLLECTION(struct_flags) )
    {
        if ( (CC_NODE_IS_MAP(struct_flags) ^ (key != 0)) )
            CC_Error( CC_StsBadArg, "An attempt to add element without a key to a map, "
                                    "or add element with key to sequence" );
    } else {
        fs->is_first = 0;
        struct_flags = CC_NODE_EMPTY | (key ? CC_NODE_MAP : CC_NODE_SEQ);
    }

    /* start to write */

    char* ptr = 0;

    if( CC_NODE_IS_FLOW(struct_flags) )
    {
        int new_offset;
        ptr = fs->buffer;
        if( !CC_NODE_IS_EMPTY(struct_flags) )
            *ptr++ = ',';
        new_offset = (int)(ptr - fs->buffer_start + key_len + data_len);
        if( new_offset > fs->wrap_margin && new_offset - fs->struct_indent > 10 )
        {
            fs->buffer = ptr;
            ptr = icvFSFlush(fs);
        }
        else
            *ptr++ = ' ';
    }
    else
    {
        if ( !CC_NODE_IS_EMPTY(struct_flags) )
        {
            ptr = fs->buffer;
            *ptr++ = ',';
            *ptr++ = '\n';
            *ptr++ = '\0';
            ::icvPuts( fs, fs->buffer_start );
            ptr = fs->buffer = fs->buffer_start;
        }
        ptr = icvFSFlush(fs);
    }

    if( key )
    {
        if( !cc_isalpha(key[0]) && key[0] != '_' )
            CC_Error( CC_StsBadArg, "Key must start with a letter or _" );

        ptr = icvFSResizeWriteBuffer( fs, ptr, (int)(key_len) );
        *ptr++ = '\"';

        for( size_t i = 0u; i < key_len; i++ )
        {
            char c = key[i];

            ptr[i] = c;
            if( !cc_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                CC_Error( CC_StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
        }

        ptr += key_len;
        *ptr++ = '\"';
        *ptr++ = ':';
        *ptr++ = ' ';
    }

    if( data )
    {
        ptr = icvFSResizeWriteBuffer( fs, ptr, (int)(data_len) );
        memcpy( ptr, data, data_len );
        ptr += data_len;
    }

    fs->buffer = ptr;
    fs->struct_flags = struct_flags & ~CC_NODE_EMPTY;
}


static void
icvJSONStartWriteStruct( CFileStorage* fs, const char* key, int struct_flags,
                        const char* type_name CC_DEFAULT(0))
{
    int parent_flags;
    char data[CC_FS_MAX_LEN + 1024];

    struct_flags = (struct_flags & (CC_NODE_TYPE_MASK|CC_NODE_FLOW)) | CC_NODE_EMPTY;
    if( !CC_NODE_IS_COLLECTION(struct_flags))
        CC_Error( CC_StsBadArg,
        "Some collection type - CC_NODE_SEQ or CC_NODE_MAP, must be specified" );

    if ( type_name && *type_name == '\0' )
        type_name = 0;

    bool has_type_id = false;
    bool is_real_collection = true;
    if (type_name && memcmp(type_name, "binary", 6) == 0)
    {
        struct_flags = CC_NODE_STR;
        data[0] = '\0';
        is_real_collection = false;
    }
    else if( type_name )
    {
        has_type_id = true;
    }

    if ( is_real_collection )
    {
        char c = CC_NODE_IS_MAP(struct_flags) ? '{' : '[';
        data[0] = c;
        data[1] = '\0';
    }

    icvJSONWrite( fs, key, data );

    parent_flags = fs->struct_flags;
    cvSeqPush( fs->write_stack, &parent_flags );
    fs->struct_flags = struct_flags;
    fs->struct_indent += 4;

    if ( has_type_id )
        fs->write_string( fs, "type_id", type_name, 1 );
}


static void
icvJSONEndWriteStruct( CFileStorage* fs )
{
    if( fs->write_stack->total == 0 )
        CC_Error( CC_StsError, "EndWriteStruct w/o matching StartWriteStruct" );

    int parent_flags = 0;
    int struct_flags = fs->struct_flags;
    cvSeqPop( fs->write_stack, &parent_flags );
    fs->struct_indent -= 4;
    fs->struct_flags = parent_flags & ~CC_NODE_EMPTY;
    assert( fs->struct_indent >= 0 );

    if ( CC_NODE_IS_COLLECTION(struct_flags) )
    {
        if ( !CC_NODE_IS_FLOW(struct_flags) )
        {
            if ( fs->buffer <= fs->buffer_start + fs->space )
            {
                /* some bad code for base64_writer... */
                *fs->buffer++ = '\n';
                *fs->buffer++ = '\0';
                icvPuts( fs, fs->buffer_start );
                fs->buffer = fs->buffer_start;
            }
            icvFSFlush(fs);
        }

        char* ptr = fs->buffer;
        if( ptr > fs->buffer_start + fs->struct_indent && !CC_NODE_IS_EMPTY(struct_flags) )
            *ptr++ = ' ';
        *ptr++ = CC_NODE_IS_MAP(struct_flags) ? '}' : ']';
        fs->buffer = ptr;
    }
}


static void
icvJSONStartNextStream( CFileStorage* fs )
{
    if( !fs->is_first )
    {
        while( fs->write_stack->total > 0 )
            icvJSONEndWriteStruct(fs);

        fs->struct_indent = 4;
        icvFSFlush(fs);
        fs->buffer = fs->buffer_start;
    }
}


static void
icvJSONWriteInt( CFileStorage* fs, const char* key, int value )
{
    char buf[128];
    icvJSONWrite( fs, key, icc_itoa( value, buf, 10 ));
}


static void
icvJSONWriteReal( CFileStorage* fs, const char* key, double value )
{
    char buf[128];
    icvJSONWrite( fs, key, icvDoubleToString( buf, value ));
}


static void
icvJSONWriteString( CFileStorage* fs, const char* key,
                   const char* str, int quote CC_DEFAULT(0))
{
    char buf[CC_FS_MAX_LEN*4+16];
    char* data = (char*)str;
    int i, len;

    if( !str )
        CC_Error( CC_StsNullPtr, "Null string pointer" );

    len = (int)strlen(str);
    if( len > CC_FS_MAX_LEN )
        CC_Error( CC_StsBadArg, "The written string is too long" );

    if( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
    {
        int need_quote = 1;
        data = buf;
        *data++ = '\"';
        for( i = 0; i < len; i++ )
        {
            char c = str[i];

            switch ( c )
            {
            case '\\':
            case '\"':
            case '\'': { *data++ = '\\'; *data++ = c;   break; }
            case '\n': { *data++ = '\\'; *data++ = 'n'; break; }
            case '\r': { *data++ = '\\'; *data++ = 'r'; break; }
            case '\t': { *data++ = '\\'; *data++ = 't'; break; }
            case '\b': { *data++ = '\\'; *data++ = 'b'; break; }
            case '\f': { *data++ = '\\'; *data++ = 'f'; break; }
            default  : { *data++ = c; }
                break;
            }
        }

        *data++ = '\"';
        *data++ = '\0';
        data = buf + !need_quote;
    }

    icvJSONWrite( fs, key, data );
}


static void
icvJSONWriteComment( CFileStorage* fs, const char* comment, int eol_comment )
{
    if( !comment )
        CC_Error( CC_StsNullPtr, "Null comment" );

    int         len = (int)(strlen(comment));
    char*       ptr = fs->buffer;
    const char* eol = strchr(comment, '\n');
    bool  multiline = eol != 0;

    if( !eol_comment || multiline || fs->buffer_end - ptr < len || ptr == fs->buffer_start )
        ptr = icvFSFlush( fs );
    else
        *ptr++ = ' ';

    while( comment )
    {
        *ptr++ = '/';
        *ptr++ = '/';
        *ptr++ = ' ';
        if( eol )
        {
            ptr = icvFSResizeWriteBuffer( fs, ptr, (int)(eol - comment) + 1 );
            memcpy( ptr, comment, eol - comment + 1 );
            fs->buffer = ptr + (eol - comment);
            comment = eol + 1;
            eol = strchr( comment, '\n' );
        }
        else
        {
            len = (int)strlen(comment);
            ptr = icvFSResizeWriteBuffer( fs, ptr, len );
            memcpy( ptr, comment, len );
            fs->buffer = ptr + len;
            comment = 0;
        }
        ptr = icvFSFlush( fs );
    }
}

#endif