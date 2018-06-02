
/****************************************************************************************\
*                                       YAML Parser                                      *
\****************************************************************************************/

static char*
icvYMLSkipSpaces( CTextInfo* fs, const char* ptr, int min_indent, int max_comment_indent )
{
    for(;;)
    {
        while( *ptr == ' ' )
            ptr++;
        if( *ptr == '#' )
        {
            //if( ptr - fs->buffer_start > max_comment_indent )                return ptr;
            // *ptr = '\0';
        }
        else if( cc_isprint(*ptr) )
        {
            //if( ptr - fs->buffer_start < min_indent )                CC_PARSE_ERROR( "Incorrect indentation" );
            break;
        }
        else if( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
        {
#if 0
            ptr = icvGets( fs, fs->buffer_start, (int)(fs->buffer_end - fs->buffer_start) );
            if( !ptr )
            {
                // emulate end of stream
                ptr = fs->buffer_start;
                ptr[0] = ptr[1] = ptr[2] = '.';
                ptr[3] = '\0';
                fs->dummy_eof = 1;
                break;
            }
            else
#endif
            {
                int l = (int)strlen(ptr);
                if( ptr[l-1] != '\n' && ptr[l-1] != '\r' )
                    printf( "Too long string or a last string w/o newline" );
            }

            fs->lineno++;
        }
        else
            CC_PARSE_ERROR( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
    }

    return (char*)ptr;
}


static void icvYMLGetMultilineStringContent(CTextInfo* fs,
    char* ptr, int indent, char* &beg, char* &end)
{
    ptr = icvYMLSkipSpaces(fs, ptr, 0, INT_MAX);
    beg = ptr;
    end = ptr;

#if 0
    if (ptr - fs->buffer_start != indent)
        return ; /* end of string */
#endif

    /* find end */
    while(cc_isprint(*ptr)) /* no check for base64 string */
        ++ ptr;
    if (*ptr == '\0')
        CC_PARSE_ERROR("Unexpected end of line");

    end = ptr;
}

static int icvCalcStructSize( const char* dt, int initial_size );

#if 0
static char* icvYMLParseBase64(CFileStorage* fs, const char* ptr, int indent, CFileNode * node)
{
    char * beg = 0;
    char * end = 0;

    icvYMLGetMultilineStringContent(fs, ptr, indent, beg, end);
    if (beg >= end)
        return end; // CC_PARSE_ERROR("Empty Binary Data");

    /* calc (decoded) total_byte_size from header */
    std::string dt;
    {
        if (end - beg < (int)(ENCODED_HEADER_SIZE))
            CC_PARSE_ERROR("Unrecognized Base64 header");

        char header[HEADER_SIZE + 2];
        memset(header, countof(header), ' ');
        header[countof(header)-1]=0;
        base64_decode(beg, header, 0U, ENCODED_HEADER_SIZE);
        if ( !read_base64_header(header, dt) || dt.empty() )
            CC_PARSE_ERROR("Invalid `dt` in Base64 header");

        beg += ENCODED_HEADER_SIZE;
    }

    /* get all Base64 data */
    std::string base64_buffer;
    base64_buffer.reserve( PARSER_BASE64_BUFFER_SIZE );
    while( beg < end )
    {
        base64_buffer.append( beg, end );
        beg = end;
        icvYMLGetMultilineStringContent( fs, beg, indent, beg, end );
    }
    if ( !base64_valid(base64_buffer.data(), 0U, base64_buffer.size()) )
        CC_PARSE_ERROR( "Invalid Base64 data." );

    /* buffer for decoded data(exclude header) */
    std::vector<uchar> binary_buffer( base64_decode_buffer_size(base64_buffer.size()) );
    int total_byte_size = (int)(
        base64_decode_buffer_size( base64_buffer.size(), base64_buffer.data(), false )
        );
    {
        Base64ContextParser parser(binary_buffer.begin(), binary_buffer.size() );
        const uchar * buffer_beg = reinterpret_cast<const uchar *>( base64_buffer.data() );
        const uchar * buffer_end = buffer_beg + base64_buffer.size();
        parser.read( buffer_beg, buffer_end );
        parser.flush();
    }

    /* save as CvSeq */
    int elem_size = ::icvCalcStructSize(dt.c_str(), 0);
    if (total_byte_size % elem_size != 0)
        CC_PARSE_ERROR("Byte size not match elememt size");
    int elem_cnt = total_byte_size / elem_size;

    node->tag = CC_NODE_NONE;
    int struct_flags = CC_NODE_FLOW | CC_NODE_SEQ;
    /* after icvFSCreateCollection, node->tag == struct_flags */
    icvFSCreateCollection(fs, struct_flags, node);
    make_seq(binary_buffer.data(), elem_cnt, dt, *node->data.seq);

    if (fs->dummy_eof) {
        /* end of file */
        return fs->buffer_start;
    } else {
        /* end of line */
        return end;
    }
}
#endif


static char*
icvYMLParseKey( CTextInfo* fs, const char* ptr,
                CFileNode* map_node, CFileNode** value_placeholder )
{
    char c;
    char *endptr = (char*)ptr - 1, *saveptr;

    if( *ptr == '-' )
        CC_PARSE_ERROR( "Key may not start with \'-\'" );

    do c = *++endptr;
    while( cc_isprint(c) && c != ':' );

    if( c != ':' )
        CC_PARSE_ERROR( "Missing \':\'" );

    saveptr = endptr + 1;
    do c = *--endptr;
    while( c == ' ' );

    ++endptr;
    if( endptr == ptr )
        CC_PARSE_ERROR( "An empty key" );


    *value_placeholder = map_node;
    cvGetFileNode( fs, value_placeholder, ptr, (int)(endptr - ptr), 1 );
    ptr = saveptr;

    return (char*)ptr;
}


static char*
icvYMLParseValue( CFileStorage* fs, const char* ptr, CFileNode* node,
                  int min_indent )
{
    //char buf[CC_FS_MAX_LEN + 1024];
    char* endptr = 0;
    char c = ptr[0], d = ptr[1];
    //int len;
    bool is_binary_string = false;

    memset( node, 0, sizeof(*node) );

#if 0
    if( c == '!' ) // handle explicit type specification
    {
        if( d == '!' || d == '^' )
        {
            ptr++;
            value_type |= CC_NODE_USER;
        }

        endptr = (char*)ptr++;
        do d = *++endptr;
        while( cc_isprint(d) && d != ' ' );
        len = (int)(endptr - ptr);
        if( len == 0 )
            CC_PARSE_ERROR( "Empty type name" );
        d = *endptr;
        // *endptr = '\0';

        if( len == 3 && !CC_NODE_IS_USER(value_type) )
        {
            if( memcmp( ptr, "str", 3 ) == 0 )
                value_type = CC_NODE_STRING;
            else if( memcmp( ptr, "int", 3 ) == 0 )
                value_type = CC_NODE_INT;
            else if( memcmp( ptr, "seq", 3 ) == 0 )
                value_type = CC_NODE_SEQ;
            else if( memcmp( ptr, "map", 3 ) == 0 )
                value_type = CC_NODE_MAP;
        }
        else if( len == 5 && !CC_NODE_IS_USER(value_type) )
        {
            if( memcmp( ptr, "float", 5 ) == 0 )
                value_type = CC_NODE_REAL;
        }
#if 0
        else if (len == 6 && CC_NODE_IS_USER(value_type))
        {
            if( memcmp( ptr, "binary", 6 ) == 0 ) {
                value_type = CC_NODE_SEQ;
                is_binary_string = true;

                /* for ignore '|' */

                /**** operation with endptr ****/
                *endptr = d;

                do {
                    d = *++endptr;
                    if (d == '|')
                        break;
                } while (d == ' ');

                d = *++endptr;
                *endptr = '\0';
            }
        }
        else if( CC_NODE_IS_USER(value_type) )
        {
            node->info = cvFindType( ptr );
            if( !node->info )
                node->tag &= ~CC_NODE_USER;
        }

        *endptr = d;
#endif
        ptr = icvYMLSkipSpaces( fs, endptr, min_indent, INT_MAX );

        c = *ptr;

        if( !CC_NODE_IS_USER(value_type) )
        {
            if (value_type == CC_NODE_STRING && c != '\'' && c != '\"')
                goto force_string;
            if( value_type == CC_NODE_INT )
                goto force_int;
            if( value_type == CC_NODE_REAL )
                goto force_real;
        }
    }
    if (is_binary_string)
    {
        /* for base64 string */
        int indent = (int)(ptr - fs->buffer_start);
        ptr = icvYMLParseBase64(fs, ptr, indent, node);
    }
    else if( cc_isdigit(c) ||
        ((c == '-' || c == '+') && (cc_isdigit(d) || d == '.')) ||
        (c == '.' && cc_isalnum(d))) // a number
    {
        double fval;
        int ival;
        endptr = (char*)ptr + (c == '-' || c == '+');
        while( cc_isdigit(*endptr) )
            endptr++;
        if( *endptr == '.' || *endptr == 'e' )
        {
force_real:
            fval = icc_strtod( ptr, &endptr );
            /*if( endptr == ptr || cc_isalpha(*endptr) )
                icvProcessSpecialDouble( fs, endptr, &fval, &endptr ));*/

            node->tag = CC_NODE_REAL;
            //node->data.f = fval;
        }
        else
        {
force_int:
            ival = (int)strtol( ptr, &endptr, 0 );
            node->tag = CC_NODE_INT;
            node->data.i = ival;
        }

        if( !endptr || endptr == ptr )
            CC_PARSE_ERROR( "Invalid numeric value (inconsistent explicit type specification?)" );

        ptr = endptr;
    }
    else if( c == '\'' || c == '\"' ) // an explicit string
    {
        node->tag = CC_NODE_STRING;
        if( c == '\'' )
            for( len = 0; len < CC_FS_MAX_LEN; )
            {
                c = *++ptr;
                if( cc_isalnum(c) || (c != '\'' && cc_isprint(c)))
                    buf[len++] = c;
                else if( c == '\'' )
                {
                    c = *++ptr;
                    if( c != '\'' )
                        break;
                    buf[len++] = c;
                }
                else
                    CC_PARSE_ERROR( "Invalid character" );
            }
        else
            for( len = 0; len < CC_FS_MAX_LEN; )
            {
                c = *++ptr;
                if( cc_isalnum(c) || (c != '\\' && c != '\"' && cc_isprint(c)))
                    buf[len++] = c;
                else if( c == '\"' )
                {
                    ++ptr;
                    break;
                }
                else if( c == '\\' )
                {
                    d = *++ptr;
                    if( d == '\'' )
                        buf[len++] = d;
                    else if( d == '\"' || d == '\\' || d == '\'' )
                        buf[len++] = d;
                    else if( d == 'n' )
                        buf[len++] = '\n';
                    else if( d == 'r' )
                        buf[len++] = '\r';
                    else if( d == 't' )
                        buf[len++] = '\t';
                    else if( d == 'x' || (cc_isdigit(d) && d < '8') )
                    {
                        int val, is_hex = d == 'x';
                        c = ptr[3];
                        ptr[3] = '\0';
                        val = (int)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                        ptr[3] = c;
                        if( endptr == ptr + is_hex )
                            buf[len++] = 'x';
                        else
                        {
                            buf[len++] = (char)val;
                            ptr = endptr;
                        }
                    }
                }
                else
                    CC_PARSE_ERROR( "Invalid character" );
            }

        if( len >= CC_FS_MAX_LEN )
            CC_PARSE_ERROR( "Too long string literal" );

        node->data.str = cvMemStorageAllocString( fs->memstorage, buf, len );
    }
    else if( c == '[' || c == '{' ) // collection as a flow
    {
        int new_min_indent = min_indent + !is_parent_flow;
        int struct_flags = CC_NODE_FLOW + (c == '{' ? CC_NODE_MAP : CC_NODE_SEQ);
        int is_simple = 1;

        icvFSCreateCollection( fs, CC_NODE_TYPE(struct_flags) +
                                        (node->info ? CC_NODE_USER : 0), node );

        d = c == '[' ? ']' : '}';

        for( ++ptr ;;)
        {
            CFileNode* elem = 0;

            ptr = icvYMLSkipSpaces( fs, ptr, new_min_indent, INT_MAX );
            if( *ptr == '}' || *ptr == ']' )
            {
                if( *ptr != d )
                    CC_PARSE_ERROR( "The wrong closing bracket" );
                ptr++;
                break;
            }

            if( node->data.seq->total != 0 )
            {
                if( *ptr != ',' )
                    CC_PARSE_ERROR( "Missing , between the elements" );
                ptr = icvYMLSkipSpaces( fs, ptr + 1, new_min_indent, INT_MAX );
            }

            if( CC_NODE_IS_MAP(struct_flags) )
            {
                ptr = icvYMLParseKey( fs, ptr, node, &elem );
                ptr = icvYMLSkipSpaces( fs, ptr, new_min_indent, INT_MAX );
            }
            else
            {
                if( *ptr == ']' )
                    break;
                elem = (CFileNode*)cvSeqPush( node->data.seq, 0 );
            }
            ptr = icvYMLParseValue( fs, ptr, elem, struct_flags, new_min_indent );
            if( CC_NODE_IS_MAP(struct_flags) )
                elem->tag |= CC_NODE_NAMED;
            is_simple &= !CC_NODE_IS_COLLECTION(elem->tag);
        }
        node->data.seq->flags |= is_simple ? CC_NODE_SEQ_SIMPLE : 0;
    }
    else
    {
        int indent, struct_flags, is_simple;

        if( is_parent_flow || c != '-' )
        {
            // implicit (one-line) string or nested block-style collection
            if( !is_parent_flow )
            {
                if( c == '?' )
                    CC_PARSE_ERROR( "Complex keys are not supported" );
                if( c == '|' || c == '>' )
                    CC_PARSE_ERROR( "Multi-line text literals are not supported" );
            }

force_string:
            endptr = ptr - 1;

            do c = *++endptr;
            while( cc_isprint(c) &&
                   (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                   (is_parent_flow || c != ':' || value_type == CC_NODE_STRING));

            if( endptr == ptr )
                CC_PARSE_ERROR( "Invalid character" );

            if( is_parent_flow || c != ':' )
            {
                char* str_end = endptr;
                node->tag = CC_NODE_STRING;
                // strip spaces in the end of string
                do c = *--str_end;
                while( str_end > ptr && c == ' ' );
                str_end++;
                node->data.str = cvMemStorageAllocString( fs->memstorage, ptr, (int)(str_end - ptr) );
                ptr = endptr;
                return ptr;
            }
            struct_flags = CC_NODE_MAP;
        }
        else
            struct_flags = CC_NODE_SEQ;

        icvFSCreateCollection( fs, struct_flags +
                    (node->info ? CC_NODE_USER : 0), node );

        indent = (int)(ptr - fs->buffer_start);
        is_simple = 1;

        for(;;)
        {
            CFileNode* elem = 0;

            if( CC_NODE_IS_MAP(struct_flags) )
            {
                ptr = icvYMLParseKey( fs, ptr, node, &elem );
            }
            else
            {
                c = *ptr++;
                if( c != '-' )
                    CC_PARSE_ERROR( "Block sequence elements must be preceded with \'-\'" );

                elem = (CFileNode*)cvSeqPush( node->data.seq, 0 );
            }

            ptr = icvYMLSkipSpaces( fs, ptr, indent + 1, INT_MAX );
            ptr = icvYMLParseValue( fs, ptr, elem, struct_flags, indent + 1 );
            if( CC_NODE_IS_MAP(struct_flags) )
                elem->tag |= CC_NODE_NAMED;
            is_simple &= !CC_NODE_IS_COLLECTION(elem->tag);

            ptr = icvYMLSkipSpaces( fs, ptr, 0, INT_MAX );
            if( ptr - fs->buffer_start != indent )
            {
                if( ptr - fs->buffer_start < indent )
                    break;
                else
                    CC_PARSE_ERROR( "Incorrect indentation" );
            }
            if( memcmp( ptr, "...", 3 ) == 0 )
                break;
        }

        node->data.seq->flags |= is_simple ? CC_NODE_SEQ_SIMPLE : 0;
    }
#endif

    return (char*)ptr;
}


static void
icvYMLParse( CFileStorage* fs, const char* ptr )
{
    int is_first = 1;
#if 0
    for(;;)
    {
        // 0. skip leading comments and directives  and ...
        // 1. reach the first item
        for(;;)
        {
            ptr = icvYMLSkipSpaces( fs, ptr, 0, INT_MAX );
            if( !ptr )
                return;

            if( *ptr == '%' )
            {
                if( memcmp( ptr, "%YAML", 5 ) == 0 &&
                    memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                    memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    CC_PARSE_ERROR( "Unsupported YAML version (it must be 1.x)" );
                
            }
            else if( *ptr == '-' )
            {
                if( memcmp(ptr, "---", 3) == 0 )
                {
                    ptr += 3;
                    break;
                }
                else if( is_first )
                    break;
            }
            else if( cc_isalnum(*ptr) || *ptr=='_')
            {
                if( !is_first )
                    CC_PARSE_ERROR( "The YAML streams must start with '---', except the first one" );
                break;
            }
            else if( fs->dummy_eof )
                break;
            else
                CC_PARSE_ERROR( "Invalid or unsupported syntax" );
        }

        ptr = icvYMLSkipSpaces( fs, ptr, 0, INT_MAX );
        if( memcmp( ptr, "...", 3 ) != 0 )
        {
            // 2. parse the collection
            CFileNode* root_node = (CFileNode*)cvSeqPush( fs->roots, 0 );

            ptr = icvYMLParseValue( fs, ptr, root_node, CC_NODE_NONE, 0 );
            if( !CC_NODE_IS_COLLECTION(root_node->tag) )
                CC_PARSE_ERROR( "Only collections as YAML streams are supported by this parser" );

            // 3. parse until the end of file or next collection
            ptr = icvYMLSkipSpaces( fs, ptr, 0, INT_MAX );
            if( !ptr )
                return;
        }

        if( fs->dummy_eof )
            break;
        ptr += 3;
        is_first = 0;
    }
#endif
}


/****************************************************************************************\
*                                       YAML Emitter                                     *
\****************************************************************************************/

#if 0
static void
icvYMLWrite( CFileStorage* fs, const char* key, const char* data )
{
    //check_if_write_struct_is_delayed( fs );
    if ( fs->state_of_writing_base64 == Uncertain )
    {
        switch_to_Base64_state( fs, NotUse );
    }
    else if ( fs->state_of_writing_base64 == InUse )
    {
        CC_Error( CC_StsError, "At present, output Base64 data only." );
    }

    int i, keylen = 0;
    int datalen = 0;
    int struct_flags;
    char* ptr;

    struct_flags = fs->struct_flags;

    if( key && key[0] == '\0' )
        key = 0;

    if( CC_NODE_IS_COLLECTION(struct_flags) )
    {
        if( (CC_NODE_IS_MAP(struct_flags) ^ (key != 0)) )
            CC_Error( CC_StsBadArg, "An attempt to add element without a key to a map, "
                                    "or add element with key to sequence" );
    }
    else
    {
        fs->is_first = 0;
        struct_flags = CC_NODE_EMPTY | (key ? CC_NODE_MAP : CC_NODE_SEQ);
    }

    if( key )
    {
        keylen = (int)strlen(key);
        if( keylen == 0 )
            CC_Error( CC_StsBadArg, "The key is an empty" );

        if( keylen > CC_FS_MAX_LEN )
            CC_Error( CC_StsBadArg, "The key is too long" );
    }

    if( data )
        datalen = (int)strlen(data);

    if( CC_NODE_IS_FLOW(struct_flags) )
    {
        int new_offset;
        ptr = fs->buffer;
        if( !CC_NODE_IS_EMPTY(struct_flags) )
            *ptr++ = ',';
        new_offset = (int)(ptr - fs->buffer_start) + keylen + datalen;
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
        ptr = icvFSFlush(fs);
        if( !CC_NODE_IS_MAP(struct_flags) )
        {
            *ptr++ = '-';
            if( data )
                *ptr++ = ' ';
        }
    }

    if( key )
    {
        if( !cc_isalpha(key[0]) && key[0] != '_' )
            CC_Error( CC_StsBadArg, "Key must start with a letter or _" );

        ptr = icvFSResizeWriteBuffer( fs, ptr, keylen );

        for( i = 0; i < keylen; i++ )
        {
            char c = key[i];

            ptr[i] = c;
            if( !cc_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                CC_Error( CC_StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
        }

        ptr += keylen;
        *ptr++ = ':';
        if( !CC_NODE_IS_FLOW(struct_flags) && data )
            *ptr++ = ' ';
    }

    if( data )
    {
        ptr = icvFSResizeWriteBuffer( fs, ptr, datalen );
        memcpy( ptr, data, datalen );
        ptr += datalen;
    }

    fs->buffer = ptr;
    fs->struct_flags = struct_flags & ~CC_NODE_EMPTY;
}


static void
icvYMLStartWriteStruct( CFileStorage* fs, const char* key, int struct_flags,
                        const char* type_name CC_DEFAULT(0))
{
    int parent_flags;
    char buf[CC_FS_MAX_LEN + 1024];
    const char* data = 0;

    if ( type_name && *type_name == '\0' )
        type_name = 0;

    struct_flags = (struct_flags & (CC_NODE_TYPE_MASK|CC_NODE_FLOW)) | CC_NODE_EMPTY;
    if( !CC_NODE_IS_COLLECTION(struct_flags))
        CC_Error( CC_StsBadArg,
        "Some collection type - CC_NODE_SEQ or CC_NODE_MAP, must be specified" );

    if (type_name && memcmp(type_name, "binary", 6) == 0)
    {
        /* reset struct flag. in order not to print ']' */
        struct_flags = CC_NODE_SEQ;
        sprintf(buf, "!!binary |");
        data = buf;
    }
    else if( CC_NODE_IS_FLOW(struct_flags))
    {
        char c = CC_NODE_IS_MAP(struct_flags) ? '{' : '[';
        struct_flags |= CC_NODE_FLOW;

        if( type_name )
            sprintf( buf, "!!%s %c", type_name, c );
        else
        {
            buf[0] = c;
            buf[1] = '\0';
        }
        data = buf;
    }
    else if( type_name )
    {
        sprintf( buf, "!!%s", type_name );
        data = buf;
    }

    icvYMLWrite( fs, key, data );

    parent_flags = fs->struct_flags;
    cvSeqPush( fs->write_stack, &parent_flags );
    fs->struct_flags = struct_flags;

    if( !CC_NODE_IS_FLOW(parent_flags) )
        fs->struct_indent += CC_YML_INDENT + CC_NODE_IS_FLOW(struct_flags);
}


static void
icvYMLEndWriteStruct( CFileStorage* fs )
{
    int parent_flags = 0, struct_flags;
    char* ptr;

    struct_flags = fs->struct_flags;
    if( fs->write_stack->total == 0 )
        CC_Error( CC_StsError, "EndWriteStruct w/o matching StartWriteStruct" );

    cvSeqPop( fs->write_stack, &parent_flags );

    if( CC_NODE_IS_FLOW(struct_flags) )
    {
        ptr = fs->buffer;
        if( ptr > fs->buffer_start + fs->struct_indent && !CC_NODE_IS_EMPTY(struct_flags) )
            *ptr++ = ' ';
        *ptr++ = CC_NODE_IS_MAP(struct_flags) ? '}' : ']';
        fs->buffer = ptr;
    }
    else if( CC_NODE_IS_EMPTY(struct_flags) )
    {
        ptr = icvFSFlush(fs);
        memcpy( ptr, CC_NODE_IS_MAP(struct_flags) ? "{}" : "[]", 2 );
        fs->buffer = ptr + 2;
    }

    if( !CC_NODE_IS_FLOW(parent_flags) )
        fs->struct_indent -= CC_YML_INDENT + CC_NODE_IS_FLOW(struct_flags);
    assert( fs->struct_indent >= 0 );

    fs->struct_flags = parent_flags;
}


static void
icvYMLStartNextStream( CFileStorage* fs )
{
    if( !fs->is_first )
    {
        while( fs->write_stack->total > 0 )
            icvYMLEndWriteStruct(fs);

        fs->struct_indent = 0;
        icvFSFlush(fs);
        icvPuts( fs, "...\n" );
        icvPuts( fs, "---\n" );
        fs->buffer = fs->buffer_start;
    }
}


static void
icvYMLWriteInt( CFileStorage* fs, const char* key, int value )
{
    char buf[128];
    icvYMLWrite( fs, key, icc_itoa( value, buf, 10 ));
}


static void
icvYMLWriteReal( CFileStorage* fs, const char* key, double value )
{
    char buf[128];
    icvYMLWrite( fs, key, icvDoubleToString( buf, value ));
}


static void
icvYMLWriteString( CFileStorage* fs, const char* key,
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
        int need_quote = quote || len == 0 || str[0] == ' ';
        data = buf;
        *data++ = '\"';
        for( i = 0; i < len; i++ )
        {
            char c = str[i];

            if( !need_quote && !cc_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                need_quote = 1;

            if( !cc_isalnum(c) && (!cc_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
            {
                *data++ = '\\';
                if( cc_isprint(c) )
                    *data++ = c;
                else if( c == '\n' )
                    *data++ = 'n';
                else if( c == '\r' )
                    *data++ = 'r';
                else if( c == '\t' )
                    *data++ = 't';
                else
                {
                    sprintf( data, "x%02x", c );
                    data += 3;
                }
            }
            else
                *data++ = c;
        }
        if( !need_quote && (cc_isdigit(str[0]) ||
            str[0] == '+' || str[0] == '-' || str[0] == '.' ))
            need_quote = 1;

        if( need_quote )
            *data++ = '\"';
        *data++ = '\0';
        data = buf + !need_quote;
    }

    icvYMLWrite( fs, key, data );
}


static void
icvYMLWriteComment( CFileStorage* fs, const char* comment, int eol_comment )
{
    int len; //, indent;
    int multiline;
    const char* eol;
    char* ptr;

    if( !comment )
        CC_Error( CC_StsNullPtr, "Null comment" );

    len = (int)strlen(comment);
    eol = strchr(comment, '\n');
    multiline = eol != 0;
    ptr = fs->buffer;

    if( !eol_comment || multiline ||
        fs->buffer_end - ptr < len || ptr == fs->buffer_start )
        ptr = icvFSFlush( fs );
    else
        *ptr++ = ' ';

    while( comment )
    {
        *ptr++ = '#';
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
