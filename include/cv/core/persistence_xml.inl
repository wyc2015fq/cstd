
/****************************************************************************************\
*                                       XML Parser                                       *
\****************************************************************************************/

#define CC_XML_INSIDE_COMMENT 1
#define CC_XML_INSIDE_TAG 2
#define CC_XML_INSIDE_DIRECTIVE 3

static char*
icvXMLSkipSpaces( CTextInfo* fs, const char* ptr, int mode )
{
    int level = 0;

    for(;;)
    {
        char c;
        ptr--;

        if( mode == CC_XML_INSIDE_COMMENT )
        {
            do c = *++ptr;
            while( cc_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

            if( c == '-' )
            {
                assert( ptr[1] == '-' && ptr[2] == '>' );
                mode = 0;
                ptr += 3;
            }
        }
        else if( mode == CC_XML_INSIDE_DIRECTIVE )
        {
            // !!!NOTE!!! This is not quite correct, but should work in most cases
            do
            {
                c = *++ptr;
                level += c == '<';
                level -= c == '>';
                if( level < 0 )
                    return (char*)ptr;
            } while( cc_isprint_or_tab(c) );
        }
        else
        {
            do c = *++ptr;
            while( c == ' ' || c == '\t' );

            if( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
            {
                if( mode != 0 )
                    CC_PARSE_ERROR( "Comments are not allowed here" );
                mode = CC_XML_INSIDE_COMMENT;
                ptr += 4;
            }
            else if( cc_isprint(c) )
                break;
        }

        if( !cc_isprint(*ptr) )
        {
            {
                int l = (int)strlen(ptr);
                if( ptr[l-1] != '\n' && ptr[l-1] != '\r' )
                    CC_PARSE_ERROR( "Too long string or a last string w/o newline" );
            }
            fs->lineno++;
        }
    }
    return (char*)ptr;
}


static void icvXMLGetMultilineStringContent(CTextInfo* fs, const char* ptr, char* &beg, char* &end)
{
    ptr = icvXMLSkipSpaces(fs, ptr, CC_XML_INSIDE_TAG);
    beg = (char*)ptr;
    end = (char*)ptr;

    if ( *beg == '<' )
        return; /* end of string */

    /* find end */
    while( cc_isprint(*ptr) ) /* no check for base64 string */
        ++ ptr;
    if ( *ptr == '\0' )
        CC_PARSE_ERROR( "Unexpected end of line" );

    end = (char*)ptr;
}


static char* icvXMLParseBase64(CTextInfo* fs, const char* ptr, CFileNode * node)
{
    char * beg = 0;
    char * end = 0;
#if 0
    icvXMLGetMultilineStringContent(fs, ptr, beg, end);
    if (beg >= end)
        return end; // CC_PARSE_ERROR("Empty Binary Data");

    /* calc (decoded) total_byte_size from header */
    std::string dt;
    {
        if (end - beg < (int)(ENCODED_HEADER_SIZE))
            CC_PARSE_ERROR("Unrecognized Base64 header");

        std::vector<char> header(HEADER_SIZE + 1, ' ');
        base64_decode(beg, header.data(), 0U, ENCODED_HEADER_SIZE);
        if ( !read_base64_header(header, dt) || dt.empty() )
            CC_PARSE_ERROR("Invalid `dt` in Base64 header");

        beg += ENCODED_HEADER_SIZE;
    }

    /* get all Base64 data */
    std::string base64_buffer; // not an efficient way.
    base64_buffer.reserve( PARSER_BASE64_BUFFER_SIZE );
    while( beg < end )
    {
        base64_buffer.append( beg, end );
        beg = end;
        icvXMLGetMultilineStringContent( fs, beg, beg, end );
    }
    if ( !base64_valid(base64_buffer.data(), 0U, base64_buffer.size()) )
        CC_PARSE_ERROR( "Invalid Base64 data." );

    /* alloc buffer for all decoded data(include header) */
    std::vector<uchar> binary_buffer( base64_decode_buffer_size(base64_buffer.size()) );
    int total_byte_size = (int)(
        base64_decode_buffer_size( base64_buffer.size(), base64_buffer.data(), false )
        );
    {
        Base64ContextParser parser(binary_buffer.data(), binary_buffer.size() );
        const uchar * buffer_beg = reinterpret_cast<const uchar *>( base64_buffer.data() );
        const uchar * buffer_end = buffer_beg + base64_buffer.size();
        parser.read( buffer_beg, buffer_end );
        parser.flush();
    }

    /* save as CvSeq */
    int elem_size = ::icvCalcStructSize(dt, 0);
    if (total_byte_size % elem_size != 0)
        CC_PARSE_ERROR("data size not matches elememt size");
    int elem_cnt = total_byte_size / elem_size;

    node->tag = CC_NODE_NONE;
    int struct_flags = CC_NODE_SEQ;
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
#endif
}

#if 0

static char*
icvXMLParseTag( CTextInfo* fs, const char* ptr, CStringHashNode** _tag,
                vstr_t* _list, int* _tag_type );

static char*
icvXMLParseValue( CTextInfo* fs, const char* ptr, CFileNode** node)
{
    CFileNode *elem = *node;
    int have_space = 1, is_simple = 1;

    for(;;)
    {
        char c = *ptr, d;
        char* endptr;

        if( cc_isspace(c) || c == '\0' || (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
        {
            ptr = icvXMLSkipSpaces( fs, ptr, 0 );
            have_space = 1;
            c = *ptr;
        }

        d = ptr[1];

        if( c =='<' || c == '\0' )
        {
            CStringHashNode *key = 0, *key2 = 0;
            vstr_t list[1] = {0};
            int tag_type = 0;
            const char* type_name = 0;
            int elem_type = CC_NODE_NONE;

            if( d == '/' || c == '\0' )
                break;

            ptr = icvXMLParseTag( fs, ptr, &key, list, &tag_type );

            if( tag_type == CC_XML_DIRECTIVE_TAG )
                CC_PARSE_ERROR( "Directive tags are not allowed here" );
            if( tag_type == CC_XML_EMPTY_TAG )
                CC_PARSE_ERROR( "Empty tags are not supported" );

            assert( tag_type == CC_XML_OPENING_TAG );

            /* for base64 string */
            bool is_binary_string = false;
#if 0
            type_name = cvAttrValue( list, "type_id" );
            if( type_name )
            {
                if( strcmp( type_name, "str" ) == 0 )
                    elem_type = CC_NODE_STRING;
                else if( strcmp( type_name, "map" ) == 0 )
                    elem_type = CC_NODE_MAP;
                else if( strcmp( type_name, "seq" ) == 0 )
                    elem_type = CC_NODE_SEQ;
                else if (strcmp(type_name, "binary") == 0)
                {
                    elem_type = CC_NODE_NONE;
                    is_binary_string = true;
                }
                else
                {
                    info = cvFindType( type_name );
                    if( info )
                        elem_type = CC_NODE_USER;
                }
            }
#endif
            //is_noname = key->str.len == 1 && key->str.ptr[0] == '_';

            elem = cvGetFileNode( fs, node, key->str.ptr, key->str.len, 1 );
            ptr = icvXMLParseValue( fs, ptr, &elem->sub);
            ptr = icvXMLParseTag( fs, ptr, &key2, list, &tag_type );

            have_space = 1;
            vstr_free(list);
        }
        else
        {
            if( !have_space )
                CC_PARSE_ERROR( "There should be space between literals" );

            elem = *node;
#if 0
            if( value_type != CC_NODE_STRING &&
                (cc_isdigit(c) || ((c == '-' || c == '+') &&
                (cc_isdigit(d) || d == '.')) || (c == '.' && cc_isalnum(d))) ) // a number
            {
                double fval;
                int ival;
                endptr = ptr + (c == '-' || c == '+');
                while( cc_isdigit(*endptr) )
                    endptr++;
                if( *endptr == '.' || *endptr == 'e' )
                {
                    fval = icc_strtod( fs, ptr, &endptr );
                    /*if( endptr == ptr || cc_isalpha(*endptr) )
                        icvProcessSpecialDouble( fs, ptr, &fval, &endptr ));*/
                    elem->tag = CC_NODE_REAL;
                    elem->data.f = fval;
                }
                else
                {
                    ival = (int)strtol( ptr, &endptr, 0 );
                    elem->tag = CC_NODE_INT;
                    elem->data.i = ival;
                }

                if( endptr == ptr )
                    CC_PARSE_ERROR( "Invalid numeric value (inconsistent explicit type specification?)" );

                ptr = endptr;
            }
            else
            {
                // string
                char buf[CC_FS_MAX_LEN+16];
                int i = 0, len, is_quoted = 0;
                elem->tag = CC_NODE_STRING;
                if( c == '\"' )
                    is_quoted = 1;
                else
                    --ptr;

                for( ;; )
                {
                    c = *++ptr;
                    if( !cc_isalnum(c) )
                    {
                        if( c == '\"' )
                        {
                            if( !is_quoted )
                                CC_PARSE_ERROR( "Literal \" is not allowed within a string. Use &quot;" );
                            ++ptr;
                            break;
                        }
                        else if( !cc_isprint(c) || c == '<' || (!is_quoted && cc_isspace(c)))
                        {
                            if( is_quoted )
                                CC_PARSE_ERROR( "Closing \" is expected" );
                            break;
                        }
                        else if( c == '\'' || c == '>' )
                        {
                            CC_PARSE_ERROR( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                        }
                        else if( c == '&' )
                        {
                            if( *++ptr == '#' )
                            {
                                int val, base = 10;
                                ptr++;
                                if( *ptr == 'x' )
                                {
                                    base = 16;
                                    ptr++;
                                }
                                val = (int)strtol( ptr, &endptr, base );
                                if( (unsigned)val > (unsigned)255 ||
                                    !endptr || *endptr != ';' )
                                    CC_PARSE_ERROR( "Invalid numeric value in the string" );
                                c = (char)val;
                            }
                            else
                            {
                                endptr = ptr;
                                do c = *++endptr;
                                while( cc_isalnum(c) );
                                if( c != ';' )
                                    CC_PARSE_ERROR( "Invalid character in the symbol entity name" );
                                len = (int)(endptr - ptr);
                                if( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                    c = '<';
                                else if( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                    c = '>';
                                else if( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                    c = '&';
                                else if( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                    c = '\'';
                                else if( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                    c = '\"';
                                else
                                {
                                    memcpy( buf + i, ptr-1, len + 2 );
                                    i += len + 2;
                                }
                            }
                            ptr = endptr;
                        }
                    }
                    buf[i++] = c;
                    if( i >= CC_FS_MAX_LEN )
                        CC_PARSE_ERROR( "Too long string literal" );
                }
                elem->data.str = cvMemStorageAllocString( fs->memstorage, buf, i );
            }
#endif

            have_space = 0;
        }
    }

    return (char*)ptr;
}


static char*
icvXMLParseTag( CTextInfo* fs, const char* ptr, CStringHashNode** _tag,
                vstr_t*** _list, int* _tag_type )
{
    int tag_type = 0;
    CStringHashNode* tagname = 0;
    vstr_t* *first = 0, *last = 0;
    int count = 0, max_count = 4;
    int attr_buf_size = (max_count*2 + 1)*sizeof(char*) + sizeof(vstr_t*);
    const char* endptr;
    char c;
    int have_space;

    if( *ptr == '\0' )
        CC_PARSE_ERROR( "Preliminary end of the stream" );

    if( *ptr != '<' )
        CC_PARSE_ERROR( "Tag should start with \'<\'" );

    ptr++;
    if( cc_isalnum(*ptr) || *ptr == '_' )
        tag_type = CC_XML_OPENING_TAG;
    else if( *ptr == '/' )
    {
        tag_type = CC_XML_CLOSING_TAG;
        ptr++;
    }
    else if( *ptr == '?' )
    {
        tag_type = CC_XML_HEADER_TAG;
        ptr++;
    }
    else if( *ptr == '!' )
    {
        tag_type = CC_XML_DIRECTIVE_TAG;
        assert( ptr[1] != '-' || ptr[2] != '-' );
        ptr++;
    }
    else
        CC_PARSE_ERROR( "Unknown tag type" );

    for(;;)
    {
        CStringHashNode* attrname;

        if( !cc_isalpha(*ptr) && *ptr != '_' )
            CC_PARSE_ERROR( "Name should start with a letter or underscore" );

        endptr = ptr - 1;
        do c = *++endptr;
        while( cc_isalnum(c) || c == '_' || c == '-' );

        attrname = cvGetHashedKey( fs, ptr, (int)(endptr - ptr), 1 );
        ptr = endptr;

        if( !tagname )
            tagname = attrname;
        else
        {
            if( tag_type == CC_XML_CLOSING_TAG )
                CC_PARSE_ERROR( "Closing tag should not contain any attributes" );

            if( !last || count >= max_count )
            {
                vstr_t** chunk;

                chunk = (vstr_t**)cvMemStorageAlloc( fs->memstorage, attr_buf_size );
                memset( chunk, 0, attr_buf_size );
                chunk->attr = (const char**)(chunk + 1);
                count = 0;
                if( !last )
                    first = last = chunk;
                else
                    last = last->next = chunk;
            }
            last->attr[count*2] = attrname->str.ptr;
        }

        if( last )
        {
            CFileNode stub;

            if( *ptr != '=' )
            {
                ptr = icvXMLSkipSpaces( fs, ptr, CC_XML_INSIDE_TAG );
                if( *ptr != '=' )
                    CC_PARSE_ERROR( "Attribute name should be followed by \'=\'" );
            }

            c = *++ptr;
            if( c != '\"' && c != '\'' )
            {
                ptr = icvXMLSkipSpaces( fs, ptr, CC_XML_INSIDE_TAG );
                if( *ptr != '\"' && *ptr != '\'' )
                    CC_PARSE_ERROR( "Attribute value should be put into single or double quotes" );
            }

            ptr = icvXMLParseValue( fs, ptr, &stub, CC_NODE_STRING );
            assert( stub.tag == CC_NODE_STRING );
            last->attr[count*2+1] = stub.data.str.ptr;
            count++;
        }

        c = *ptr;
        have_space = cc_isspace(c) || c == '\0';

        if( c != '>' )
        {
            ptr = icvXMLSkipSpaces( fs, ptr, CC_XML_INSIDE_TAG );
            c = *ptr;
        }

        if( c == '>' )
        {
            if( tag_type == CC_XML_HEADER_TAG )
                CC_PARSE_ERROR( "Invalid closing tag for <?xml ..." );
            ptr++;
            break;
        }
        else if( c == '?' && tag_type == CC_XML_HEADER_TAG )
        {
            if( ptr[1] != '>'  )
                CC_PARSE_ERROR( "Invalid closing tag for <?xml ..." );
            ptr += 2;
            break;
        }
        else if( c == '/' && ptr[1] == '>' && tag_type == CC_XML_OPENING_TAG )
        {
            tag_type = CC_XML_EMPTY_TAG;
            ptr += 2;
            break;
        }

        if( !have_space )
            CC_PARSE_ERROR( "There should be space between attributes" );
    }

    *_tag = tagname;
    *_tag_type = tag_type;
    *_list = first;

    return ptr;
}

static void
icvXMLParse( const str_t* fs )
{
    char* ptr = fs->s;
    CStringHashNode *key = 0, *key2 = 0;
    vstr_t** list = 0;
    int tag_type = 0;

    // CC_XML_INSIDE_TAG is used to prohibit leading comments
    ptr = icvXMLSkipSpaces( fs, ptr, CC_XML_INSIDE_TAG );

    if( memcmp( ptr, "<?xml", 5 ) != 0 )
        CC_PARSE_ERROR( "Valid XML should start with \'<?xml ...?>\'" );

    ptr = icvXMLParseTag( fs, ptr, &key, &list, &tag_type );

    while( *ptr != '\0' )
    {
        ptr = icvXMLSkipSpaces( fs, ptr, 0 );

        if( *ptr != '\0' )
        {
            CFileNode* root_node;
            ptr = icvXMLParseTag( fs, ptr, &key, &list, &tag_type );
            if( tag_type != CC_XML_OPENING_TAG ||
                strcmp(key->str.ptr,"opencc_storage") != 0 )
                CC_PARSE_ERROR( "<opencc_storage> tag is missing" );

            root_node = (CFileNode*)cvSeqPush( fs->roots, 0 );
            ptr = icvXMLParseValue( fs, ptr, root_node, CC_NODE_NONE );
            ptr = icvXMLParseTag( fs, ptr, &key2, &list, &tag_type );
            if( tag_type != CC_XML_CLOSING_TAG || key != key2 )
                CC_PARSE_ERROR( "</opencc_storage> tag is missing" );
            ptr = icvXMLSkipSpaces( fs, ptr, 0 );
        }
    }

    assert( fs->dummy_eof != 0 );
}

#endif

/****************************************************************************************\
*                                       XML Emitter                                      *
\****************************************************************************************/

#define icvXMLFlush icvFSFlush

static void
icvXMLWriteTag( CFileStorage* fs, const char* key, int tag_type, const str_t* attr, int attr_size )
{
    int i;

    if( key && key[0] == '\0' )
        key = 0;

    if( !key )
        key = "_";
    else if( key[0] == '_' && key[1] == '\0' )
        CC_Error( CC_StsBadArg, "A single _ is a reserved tag name" );

    if( tag_type == CC_XML_OPENING_TAG ) {
      fs->new_line = 1;
      icvWriteIndent(fs);
    }
    icvPutc(fs, '<');
    if( tag_type == CC_XML_CLOSING_TAG )
    {
        if( attr )
            CC_Error( CC_StsBadArg, "Closing tag should not include any attributes" );
        icvPutc(fs, '/');
    }

    if( !cc_isalpha(key[0]) && key[0] != '_' )
        CC_Error( CC_StsBadArg, "Key should start with a letter or _" );

    icvPuts(fs, key);

    if (attr) {
        for( i=0; i<attr_size-1; i += 2 )
        {
            int len0 = (int)(attr[i+0].l);
            int len1 = (int)(attr[i+1].l);

            icvPutc(fs, ' ');
            icvWrite(fs, attr[i+0].s, len0 );
            icvPutc(fs, '=');
            icvPutc(fs, '\"');
            icvWrite(fs, attr[i+1].s, len1 );
            icvPutc(fs, '\"');
        }
    }

    if( tag_type == CC_XML_EMPTY_TAG )
        icvPutc(fs, '/');
    icvPutc(fs, '>');
}


static void
icvXMLWriteStructBegin( CFileStorage* fs, const char* key, const char* type_name CC_DEFAULT(0))
{
    str_t attr[10] = {0};
    int idx = 0, attr_size = 0;

    if ( type_name && *type_name == '\0' )
        type_name = 0;

    if( type_name )
    {
        attr[idx++] = STR1("type_id");
        attr[idx++] = STR1(type_name);
    }
    attr_size = idx;

    icvXMLWriteTag( fs, key, CC_XML_OPENING_TAG, attr, attr_size );
}


static void
icvXMLWriteStructEnd( CFileStorage* fs )
{
    icvXMLWriteTag( fs, fs->struct_tag.ptr, CC_XML_CLOSING_TAG, NULL, 0 );
}

static void
icvXMLWriteScalar( CFileStorage* fs, const char* key, const char* data, int len )
{
  if (key) {
    icvXMLWriteTag( fs, key, CC_XML_OPENING_TAG, 0, 0 );
    icvWrite( fs, data, len );
    icvXMLWriteTag( fs, key, CC_XML_CLOSING_TAG, 0, 0 );
  } else {
    icvWrite( fs, data, len );
  }
}


static void
icvXMLWrite( CFileStorage* fs, const char* key, const char* data )
{
    int buf_len = (int)strlen(data);
    icvXMLWriteScalar( fs, key, data, buf_len );
}


static void
icvXMLWriteInt( CFileStorage* fs, const char* key, int value )
{
    char buf[128], *ptr = icc_itoa( value, buf, 10 );
    int len = (int)strlen(ptr);
    icvXMLWriteScalar( fs, key, ptr, len );
}


static void
icvXMLWriteReal( CFileStorage* fs, const char* key, double value )
{
    char buf[128];
    int len = (int)strlen( icvDoubleToString( buf, value ));
    icvXMLWriteScalar( fs, key, buf, len );
}


static void
icvXMLWriteString( CFileStorage* fs, const char* key, const char* str, int quote )
{
    char buf[CC_FS_MAX_LEN*6+16];
    char* data = (char*)str;
    int i, len;

    if( !str )
        CC_Error( CC_StsNullPtr, "Null string pointer" );

    len = (int)strlen(str);
    if( len > CC_FS_MAX_LEN )
        CC_Error( CC_StsBadArg, "The written string is too long" );

    if( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
    {
        int need_quote = quote || len == 0;
        data = buf;
        *data++ = '\"';
        for( i = 0; i < len; i++ )
        {
            char c = str[i];

            if( (uchar)c >= 128 || c == ' ' )
            {
                *data++ = c;
                need_quote = 1;
            }
            else if( !cc_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
            {
                *data++ = '&';
                if( c == '<' )
                {
                    memcpy(data, "lt", 2);
                    data += 2;
                }
                else if( c == '>' )
                {
                    memcpy(data, "gt", 2);
                    data += 2;
                }
                else if( c == '&' )
                {
                    memcpy(data, "amp", 3);
                    data += 3;
                }
                else if( c == '\'' )
                {
                    memcpy(data, "apos", 4);
                    data += 4;
                }
                else if( c == '\"' )
                {
                    memcpy( data, "quot", 4);
                    data += 4;
                }
                else
                {
                    sprintf( data, "#x%02x", (uchar)c );
                    data += 4;
                }
                *data++ = ';';
                need_quote = 1;
            }
            else
                *data++ = c;
        }
        if( !need_quote && (cc_isdigit(str[0]) ||
            str[0] == '+' || str[0] == '-' || str[0] == '.' ))
            need_quote = 1;

        if( need_quote )
            *data++ = '\"';
        len = (int)(data - buf) - !need_quote;
        *data++ = '\0';
        data = buf + !need_quote;
    }

    icvXMLWriteScalar( fs, key, data, len );
}


static void
icvXMLWriteComment( CFileStorage* fs, const char* comment, int eol_comment )
{
    int multiline;
    const char* eol;

    if( !comment )
        CC_Error( CC_StsNullPtr, "Null comment" );

    if( strstr(comment, "--") != 0 )
        CC_Error( CC_StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

    eol = strchr(comment, '\n');
    multiline = eol != 0;

    // icvPutc(fs, ' ');

    if( !multiline )
    {
        icvPuts(fs, "<!-- ");
        icvPuts(fs, comment);
        icvPuts(fs, " -->");
    }
    else
    {
        icvPuts(fs, "<!--");
        while( comment )
        {
            if( eol )
            {
                icvWrite( fs, comment, eol - comment );
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            else
            {
                icvPuts( fs, comment );
            }
        }
        icvPuts( fs, "-->" );
    }
}
