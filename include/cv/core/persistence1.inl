
CC_IMPL void
cvStartReadRawData( const CFileStorage* fs, const CFileNode* src, CvSeqReader* reader )
{
    int node_type;
    CC_CHECK_FILE_STORAGE( fs );

    if( !src || !reader )
        CC_Error( CC_StsNullPtr, "Null pointer to source file node or reader" );

    node_type = CC_NODE_TYPE(src->tag);
    if( node_type == CC_NODE_INT || node_type == CC_NODE_REAL )
    {
        // emulate reading from 1-element sequence
        reader->ptr = (schar*)src;
        reader->block_max = reader->ptr + sizeof(*src)*2;
        reader->block_min = reader->ptr;
        reader->seq = 0;
    }
    else if( node_type == CC_NODE_SEQ )
    {
        cvStartReadSeq( src->data.seq, reader, 0 );
    }
    else if( node_type == CC_NODE_NONE )
    {
        memset( reader, 0, sizeof(*reader) );
    }
    else
        CC_Error( CC_StsBadArg, "The file node should be a numerical scalar or a sequence" );
}


CC_IMPL void
cvReadRawDataSlice( const CFileStorage* fs, CvSeqReader* reader,
                    int len, void* _data, const char* dt )
{
    char* data0 = (char*)_data;
    int fmt_pairs[CC_FS_MAX_FMT_PAIRS*2], k = 0, fmt_pair_count;
    int i = 0, offset = 0, count = 0;

    CC_CHECK_FILE_STORAGE( fs );

    if( !reader || !data0 )
        CC_Error( CC_StsNullPtr, "Null pointer to reader or destination array" );

    if( !reader->seq && len != 1 )
        CC_Error( CC_StsBadSize, "The readed sequence is a scalar, thus len must be 1" );

    fmt_pair_count = icvDecodeFormat( dt, fmt_pairs, CC_FS_MAX_FMT_PAIRS );

    for(;;)
    {
        for( k = 0; k < fmt_pair_count; k++ )
        {
            int elem_type = fmt_pairs[k*2+1];
            int elem_size = CC_ELEM_SIZE(elem_type);
            char* data;

            count = fmt_pairs[k*2];
            offset = cvAlign( offset, elem_size );
            data = data0 + offset;

            for( i = 0; i < count; i++ )
            {
                CFileNode* node = (CFileNode*)reader->ptr;
                if( CC_NODE_IS_INT(node->tag) )
                {
                    int ival = node->data.i;

                    switch( elem_type )
                    {
                    case CC_8U:
                        *(uchar*)data = saturate_cast<uchar>(ival);
                        data++;
                        break;
                    case CC_8S:
                        *(char*)data = saturate_cast<schar>(ival);
                        data++;
                        break;
                    case CC_16U:
                        *(ushort*)data = saturate_cast<ushort>(ival);
                        data += sizeof(ushort);
                        break;
                    case CC_16S:
                        *(short*)data = saturate_cast<short>(ival);
                        data += sizeof(short);
                        break;
                    case CC_32S:
                        *(int*)data = ival;
                        data += sizeof(int);
                        break;
                    case CC_32F:
                        *(float*)data = (float)ival;
                        data += sizeof(float);
                        break;
                    case CC_64F:
                        *(double*)data = (double)ival;
                        data += sizeof(double);
                        break;
                    case CC_USRTYPE1: /* reference */
                        *(size_t*)data = ival;
                        data += sizeof(size_t);
                        break;
                    default:
                        CC_Error( CC_StsUnsupportedFormat, "Unsupported type" );
                        return;
                    }
                }
                else if( CC_NODE_IS_REAL(node->tag) )
                {
                    double fval = node->data.f;
                    int ival;

                    switch( elem_type )
                    {
                    case CC_8U:
                        ival = cRound(fval);
                        *(uchar*)data = saturate_cast<uchar>(ival);
                        data++;
                        break;
                    case CC_8S:
                        ival = cRound(fval);
                        *(char*)data = saturate_cast<schar>(ival);
                        data++;
                        break;
                    case CC_16U:
                        ival = cRound(fval);
                        *(ushort*)data = saturate_cast<ushort>(ival);
                        data += sizeof(ushort);
                        break;
                    case CC_16S:
                        ival = cRound(fval);
                        *(short*)data = saturate_cast<short>(ival);
                        data += sizeof(short);
                        break;
                    case CC_32S:
                        ival = cRound(fval);
                        *(int*)data = ival;
                        data += sizeof(int);
                        break;
                    case CC_32F:
                        *(float*)data = (float)fval;
                        data += sizeof(float);
                        break;
                    case CC_64F:
                        *(double*)data = fval;
                        data += sizeof(double);
                        break;
                    case CC_USRTYPE1: /* reference */
                        ival = cRound(fval);
                        *(size_t*)data = ival;
                        data += sizeof(size_t);
                        break;
                    default:
                        CC_Error( CC_StsUnsupportedFormat, "Unsupported type" );
                        return;
                    }
                }
                else
                    CC_Error( CC_StsError,
                    "The sequence element is not a numerical scalar" );

                CC_NEXT_SEQ_ELEM( sizeof(CFileNode), *reader );
                if( !--len )
                    goto end_loop;
            }

            offset = (int)(data - data0);
        }
    }

end_loop:
    if( i != count - 1 || k != fmt_pair_count - 1 )
        CC_Error( CC_StsBadSize,
        "The sequence slice does not fit an integer number of records" );

    if( !reader->seq )
        reader->ptr -= sizeof(CFileNode);
}


CC_IMPL void
cvReadRawData( const CFileStorage* fs, const CFileNode* src,
               void* data, const char* dt )
{
    CvSeqReader reader;

    if( !src || !data )
        CC_Error( CC_StsNullPtr, "Null pointers to source file node or destination array" );

    cvStartReadRawData( fs, src, &reader );
    cvReadRawDataSlice( fs, &reader, CC_NODE_IS_SEQ(src->tag) ?
                        src->data.seq->total : 1, data, dt );
}


static void
icvWriteFileNode( CFileStorage* fs, const char* name, const CFileNode* node );

static void
icvWriteCollection( CFileStorage* fs, const CFileNode* node )
{
    int i, total = node->data.seq->total;
    int elem_size = node->data.seq->elem_size;
    int is_map = CC_NODE_IS_MAP(node->tag);
    CvSeqReader reader;

    cvStartReadSeq( node->data.seq, &reader, 0 );

    for( i = 0; i < total; i++ )
    {
        CFileMapNode* elem = (CFileMapNode*)reader.ptr;
        if( !is_map || CC_IS_SET_ELEM(elem) )
        {
            const char* name = is_map ? elem->key->str.ptr : 0;
            icvWriteFileNode( fs, name, &elem->value );
        }
        CC_NEXT_SEQ_ELEM( elem_size, reader );
    }
}

static void
icvWriteFileNode( CFileStorage* fs, const char* name, const CFileNode* node )
{
    switch( CC_NODE_TYPE(node->tag) )
    {
    case CC_NODE_INT:
        fs->write_int( fs, name, node->data.i );
        break;
    case CC_NODE_REAL:
        fs->write_real( fs, name, node->data.f );
        break;
    case CC_NODE_STR:
        fs->write_string( fs, name, node->data.str.ptr, 0 );
        break;
    case CC_NODE_SEQ:
    case CC_NODE_MAP:
        cvWriteStructBegin( fs, name, CC_NODE_TYPE(node->tag) +
                (CC_NODE_SEQ_IS_SIMPLE(node->data.seq) ? CC_NODE_FLOW : 0),
                node->info ? node->info->tid_name : 0 );
        icvWriteCollection( fs, node );
        cvWriteStructEnd( fs );
        break;
    case CC_NODE_NONE:
        cvWriteStructBegin( fs, name, CC_NODE_SEQ, 0 );
        cvWriteStructEnd( fs );
        break;
    default:
        CC_Error( CC_StsBadFlag, "Unknown type of file node" );
    }
}


CC_IMPL void
cvWriteFileNode( CFileStorage* fs, const char* new_node_name,
                 const CFileNode* node, int embed )
{
    CFileStorage* dst = 0;
    CC_CHECK_OUTPUT_FILE_STORAGE(fs);

    if( !node )
        return;

    if( CC_NODE_IS_COLLECTION(node->tag) && embed )
    {
        icvWriteCollection( fs, node );
    }
    else
    {
        icvWriteFileNode( fs, new_node_name, node );
    }
    /*
    int i, stream_count;
    stream_count = fs->roots->total;
    for( i = 0; i < stream_count; i++ )
    {
        CFileNode* node = (CFileNode*)cvGetSeqElem( fs->roots, i, 0 );
        icvDumpCollection( dst, node );
        if( i < stream_count - 1 )
            dst->start_next_stream( dst );
    }*/
    cvReleaseFileStorage( &dst );
}


CC_IMPL const char*
cvGetFileNodeName( const CFileNode* file_node )
{
    return file_node && CC_NODE_HAS_NAME(file_node->tag) ?
        ((CFileMapNode*)file_node)->key->str.ptr : 0;
}

/****************************************************************************************\
*                          Reading/Writing etc. for standard types                       *
\****************************************************************************************/


static int
icvFileNodeSeqLen( CFileNode* node )
{
    return CC_NODE_IS_COLLECTION(node->tag) ? node->data.seq->total :
        CC_NODE_TYPE(node->tag) != CC_NODE_NONE;
}


static void*
icvReadMat( CFileStorage* fs, CFileNode* node )
{
    void* ptr = 0;
    img_t* mat;
    const char* dt;
    CFileNode* data;
    int rows, cols, elem_type;

    rows = cvReadIntByName( fs, node, "rows", -1 );
    cols = cvReadIntByName( fs, node, "cols", -1 );
    dt = cvReadStringByName( fs, node, "dt", 0 );

    if( rows < 0 || cols < 0 || !dt )
        CC_Error( CC_StsError, "Some of essential matrix attributes are absent" );

    elem_type = icvDecodeSimpleFormat( dt );

    data = cvGetFileNodeByName( fs, node, "data" );
    if( !data )
        CC_Error( CC_StsError, "The matrix data is not found in file storage" );

    int nelems = icvFileNodeSeqLen( data );
    if( nelems > 0 && nelems != rows*cols*CC_MAT_CN(elem_type) )
        CC_Error( CC_StsUnmatchedSizes,
                 "The matrix size does not match to the number of stored elements" );

    if( nelems > 0 )
    {
        mat = cvCreateMat( rows, cols, elem_type );
        cvReadRawData( fs, data, mat->data.ptr, dt );
    }
    else
        mat = cvCreateMatHeader( rows, cols, elem_type );

    ptr = mat;
    return ptr;
}


/******************************* CvMatND ******************************/

static int
icvIsMatND( const void* ptr )
{
    return CC_IS_MATND_HDR(ptr);
}


static void
icvWriteMatND( CFileStorage* fs, const char* name,
               const void* struct_ptr, vstr_t* /*attr*/ )
{
    CvMatND* mat = (CvMatND*)struct_ptr;
    CvMatND stub;
    CvNArrayIterator iterator;
    int dims, sizes[CC_MAX_DIM];
    char dt[16];

    assert( CC_IS_MATND_HDR(mat) );

    cvWriteStructBegin( fs, name, CC_NODE_MAP, CC_TYPE_NAME_MATND );
    dims = cvGetDims( mat, sizes );
    cvWriteStructBegin( fs, "sizes", CC_NODE_SEQ + CC_NODE_FLOW );
    cvWriteRawData( fs, sizes, dims, "i" );
    cvWriteStructEnd( fs );
    cvWriteString( fs, "dt", icvEncodeFormat( cvGetElemType(mat), dt ), 0 );
    cvWriteStructBegin( fs, "data", CC_NODE_SEQ + CC_NODE_FLOW );

    if( mat->dim[0].size > 0 && mat->data.ptr )
    {
        cvInitNArrayIterator( 1, (img_t**)&mat, 0, &stub, &iterator );

        do
            cvWriteRawData( fs, iterator.ptr[0], iterator.size.width, dt );
        while( cvNextNArraySlice( &iterator ));
    }
    cvWriteStructEnd( fs );
    cvWriteStructEnd( fs );
}


static void*
icvReadMatND( CFileStorage* fs, CFileNode* node )
{
    void* ptr = 0;
    CvMatND* mat;
    const char* dt;
    CFileNode* data;
    CFileNode* sizes_node;
    int sizes[CC_MAX_DIM], dims, elem_type;
    int i, total_size;

    sizes_node = cvGetFileNodeByName( fs, node, "sizes" );
    dt = cvReadStringByName( fs, node, "dt", 0 );

    if( !sizes_node || !dt )
        CC_Error( CC_StsError, "Some of essential matrix attributes are absent" );

    dims = CC_NODE_IS_SEQ(sizes_node->tag) ? sizes_node->data.seq->total :
           CC_NODE_IS_INT(sizes_node->tag) ? 1 : -1;

    if( dims <= 0 || dims > CC_MAX_DIM )
        CC_Error( CC_StsParseError, "Could not determine the matrix dimensionality" );

    cvReadRawData( fs, sizes_node, sizes, "i" );
    elem_type = icvDecodeSimpleFormat( dt );

    data = cvGetFileNodeByName( fs, node, "data" );
    if( !data )
        CC_Error( CC_StsError, "The matrix data is not found in file storage" );



    for( total_size = CC_MAT_CN(elem_type), i = 0; i < dims; i++ )
        total_size *= sizes[i];

    int nelems = icvFileNodeSeqLen( data );

    if( nelems > 0 && nelems != total_size )
        CC_Error( CC_StsUnmatchedSizes,
                 "The matrix size does not match to the number of stored elements" );

    if( nelems > 0 )
    {
        mat = cvCreateMatND( dims, sizes, elem_type );
        cvReadRawData( fs, data, mat->data.ptr, dt );
    }
    else
        mat = cvCreateMatNDHeader( dims, sizes, elem_type );

    ptr = mat;
    return ptr;
}


/******************************* CvSparseMat ******************************/

static int
icvIsSparseMat( const void* ptr )
{
    return CC_IS_SPARSE_MAT(ptr);
}


static int
icvSortIdxCmpFunc( const void* _a, const void* _b, void* userdata )
{
    int i, dims = *(int*)userdata;
    const int* a = *(const int**)_a;
    const int* b = *(const int**)_b;

    for( i = 0; i < dims; i++ )
    {
        int delta = a[i] - b[i];
        if( delta )
            return delta;
    }

    return 0;
}


static void
icvWriteSparseMat( CFileStorage* fs, const char* name,
                   const void* struct_ptr, vstr_t* /*attr*/ )
{
    CvMemStorage* memstorage = 0;
    const CvSparseMat* mat = (const CvSparseMat*)struct_ptr;
    CvSparseMatIterator iterator;
    CvSparseNode* node;
    CvSeq* elements;
    CvSeqReader reader;
    int i, dims;
    int *prev_idx = 0;
    char dt[16];

    assert( CC_IS_SPARSE_MAT(mat) );

    memstorage = cvCreateMemStorage();

    cvWriteStructBegin( fs, name, CC_NODE_MAP, CC_TYPE_NAME_SPARSE_MAT );
    dims = cvGetDims( mat, 0 );

    cvWriteStructBegin( fs, "sizes", CC_NODE_SEQ + CC_NODE_FLOW );
    cvWriteRawData( fs, mat->size, dims, "i" );
    cvWriteStructEnd( fs );
    cvWriteString( fs, "dt", icvEncodeFormat( CC_MAT_TYPE(mat->tid), dt ), 0 );
    cvWriteStructBegin( fs, "data", CC_NODE_SEQ + CC_NODE_FLOW );

    elements = cvCreateSeq( CC_SEQ_ELTYPE_PTR, sizeof(CvSeq), sizeof(int*), memstorage );

    node = cvInitSparseMatIterator( mat, &iterator );
    while( node )
    {
        int* idx = CC_NODE_IDX( mat, node );
        cvSeqPush( elements, &idx );
        node = cvGetNextSparseNode( &iterator );
    }

    cvSeqSort( elements, icvSortIdxCmpFunc, &dims );
    cvStartReadSeq( elements, &reader, 0 );

    for( i = 0; i < elements->total; i++ )
    {
        int* idx;
        void* val;
        int k = 0;

        CC_READ_SEQ_ELEM( idx, reader );
        if( i > 0 )
        {
            for( ; idx[k] == prev_idx[k]; k++ )
                assert( k < dims );
            if( k < dims - 1 )
                fs->write_int( fs, 0, k - dims + 1 );
        }
        for( ; k < dims; k++ )
            fs->write_int( fs, 0, idx[k] );
        prev_idx = idx;

        node = (CvSparseNode*)((uchar*)idx - mat->idxoffset );
        val = CC_NODE_VAL( mat, node );

        cvWriteRawData( fs, val, 1, dt );
    }

    cvWriteStructEnd( fs );
    cvWriteStructEnd( fs );
    cvReleaseMemStorage( &memstorage );
}


static void*
icvReadSparseMat( CFileStorage* fs, CFileNode* node )
{
    void* ptr = 0;
    CvSparseMat* mat;
    const char* dt;
    CFileNode* data;
    CFileNode* sizes_node;
    CvSeqReader reader;
    CvSeq* elements;
    int sizes[CC_MAX_DIM_HEAP], dims, elem_type, cn;
    int i;

    sizes_node = cvGetFileNodeByName( fs, node, "sizes" );
    dt = cvReadStringByName( fs, node, "dt", 0 );

    if( !sizes_node || !dt )
        CC_Error( CC_StsError, "Some of essential matrix attributes are absent" );

    dims = CC_NODE_IS_SEQ(sizes_node->tag) ? sizes_node->data.seq->total :
           CC_NODE_IS_INT(sizes_node->tag) ? 1 : -1;

    if( dims <= 0 || dims > CC_MAX_DIM_HEAP )
        CC_Error( CC_StsParseError, "Could not determine sparse matrix dimensionality" );

    cvReadRawData( fs, sizes_node, sizes, "i" );
    elem_type = icvDecodeSimpleFormat( dt );

    data = cvGetFileNodeByName( fs, node, "data" );
    if( !data || !CC_NODE_IS_SEQ(data->tag) )
        CC_Error( CC_StsError, "The matrix data is not found in file storage" );

    mat = cvCreateSparseMat( dims, sizes, elem_type );

    cn = CC_MAT_CN(elem_type);
    int idx[CC_MAX_DIM_HEAP];
    elements = data->data.seq;
    cvStartReadRawData( fs, data, &reader );

    for( i = 0; i < elements->total; )
    {
        CFileNode* elem = (CFileNode*)reader.ptr;
        uchar* val;
        int k;
        if( !CC_NODE_IS_INT(elem->tag ))
            CC_Error( CC_StsParseError, "Sparse matrix data is corrupted" );
        k = elem->data.i;
        if( i > 0 && k >= 0 )
            idx[dims-1] = k;
        else
        {
            if( i > 0 )
                k = dims + k - 1;
            else
                idx[0] = k, k = 1;
            for( ; k < dims; k++ )
            {
                CC_NEXT_SEQ_ELEM( elements->elem_size, reader );
                i++;
                elem = (CFileNode*)reader.ptr;
                if( !CC_NODE_IS_INT(elem->tag ) || elem->data.i < 0 )
                    CC_Error( CC_StsParseError, "Sparse matrix data is corrupted" );
                idx[k] = elem->data.i;
            }
        }
        CC_NEXT_SEQ_ELEM( elements->elem_size, reader );
        i++;
        val = cvPtrND( mat, idx, 0, 1, 0 );
        cvReadRawDataSlice( fs, &reader, cn, val, dt );
        i += cn;
    }

    ptr = mat;
    return ptr;
}


/******************************* img_t ******************************/

static int
icvIsImage( const void* ptr )
{
    return CC_IS_IMAGE_HDR(ptr);
}

static void
icvWriteImage( CFileStorage* fs, const char* name,
               const void* struct_ptr, vstr_t* /*attr*/ )
{
    const img_t* image = (const img_t*)struct_ptr;
    char dt_buf[16], *dt;
    CSize size;
    int y, depth;

    assert( CC_IS_IMAGE(image) );

    if( image->dataOrder == IMG_DATA_ORDER_PLANE )
        CC_Error( CC_StsUnsupportedFormat,
        "Images with planar data layout are not supported" );

    cvWriteStructBegin( fs, name, CC_NODE_MAP, CC_TYPE_NAME_IMAGE );
    cvWriteInt( fs, "width", image->width );
    cvWriteInt( fs, "height", image->height );
    cvWriteString( fs, "origin", image->origin == IMG_ORIGIN_TL
                   ? "top-left" : "bottom-left", 0 );
    cvWriteString( fs, "layout", image->dataOrder == IMG_DATA_ORDER_PLANE
                   ? "planar" : "interleaved", 0 );
    if( image->roi )
    {
        cvWriteStructBegin( fs, "roi", CC_NODE_MAP + CC_NODE_FLOW );
        cvWriteInt( fs, "x", image->roi->xOffset );
        cvWriteInt( fs, "y", image->roi->yOffset );
        cvWriteInt( fs, "width", image->roi->width );
        cvWriteInt( fs, "height", image->roi->height );
        cvWriteInt( fs, "coi", image->roi->coi );
        cvWriteStructEnd( fs );
    }

    depth = IPL2CC_DEPTH(image->depth);
    sprintf( dt_buf, "%d%c", image->nChannels, icvTypeSymbol[depth] );
    dt = dt_buf + (dt_buf[2] == '\0' && dt_buf[0] == '1');
    cvWriteString( fs, "dt", dt, 0 );

    size = cSize(image->width, image->height);
    if( size.width*image->nChannels*CC_ELEM_SIZE(depth) == image->widthStep )
    {
        size.width *= size.height;
        size.height = 1;
    }

    cvWriteStructBegin( fs, "data", CC_NODE_SEQ + CC_NODE_FLOW );
    for( y = 0; y < size.height; y++ )
        cvWriteRawData( fs, image->imageData + y*image->widthStep, size.width, dt );
    cvWriteStructEnd( fs );
    cvWriteStructEnd( fs );
}


static void*
icvReadImage( CFileStorage* fs, CFileNode* node )
{
    void* ptr = 0;
    img_t* image;
    const char* dt;
    CFileNode* data;
    CFileNode* roi_node;
    CvSeqReader reader;
    CRect roi;
    int y, width, height, elem_type, coi, depth;
    const char* origin, *data_order;

    width = cvReadIntByName( fs, node, "width", 0 );
    height = cvReadIntByName( fs, node, "height", 0 );
    dt = cvReadStringByName( fs, node, "dt", 0 );
    origin = cvReadStringByName( fs, node, "origin", 0 );

    if( width == 0 || height == 0 || dt == 0 || origin == 0 )
        CC_Error( CC_StsError, "Some of essential image attributes are absent" );

    elem_type = icvDecodeSimpleFormat( dt );
    data_order = cvReadStringByName( fs, node, "layout", "interleaved" );
    if( strcmp( data_order, "interleaved" ) != 0 )
        CC_Error( CC_StsError, "Only interleaved images can be read" );

    data = cvGetFileNodeByName( fs, node, "data" );
    if( !data )
        CC_Error( CC_StsError, "The image data is not found in file storage" );

    if( icvFileNodeSeqLen( data ) != width*height*CC_MAT_CN(elem_type) )
        CC_Error( CC_StsUnmatchedSizes,
        "The matrix size does not match to the number of stored elements" );

    depth = cvIplDepth(elem_type);
    image = cvCreateImage( cSize(width,height), depth, CC_MAT_CN(elem_type) );

    roi_node = cvGetFileNodeByName( fs, node, "roi" );
    if( roi_node )
    {
        roi.x = cvReadIntByName( fs, roi_node, "x", 0 );
        roi.y = cvReadIntByName( fs, roi_node, "y", 0 );
        roi.width = cvReadIntByName( fs, roi_node, "width", 0 );
        roi.height = cvReadIntByName( fs, roi_node, "height", 0 );
        coi = cvReadIntByName( fs, roi_node, "coi", 0 );

        cvSetImageROI( image, roi );
        cvSetImageCOI( image, coi );
    }

    if( width*CC_ELEM_SIZE(elem_type) == image->widthStep )
    {
        width *= height;
        height = 1;
    }

    width *= CC_MAT_CN(elem_type);
    cvStartReadRawData( fs, data, &reader );
    for( y = 0; y < height; y++ )
    {
        cvReadRawDataSlice( fs, &reader, width,
            image->imageData + y*image->widthStep, dt );
    }

    ptr = image;
    return ptr;
}


/******************************* CvSeq ******************************/

static int
icvIsSeq( const void* ptr )
{
    return CC_IS_SEQ(ptr);
}


static void
icvReleaseSeq( void** ptr )
{
    if( !ptr )
        CC_Error( CC_StsNullPtr, "NULL double pointer" );
    *ptr = 0; // it's impossible now to release seq, so just clear the pointer
}


static void*
icvCloneSeq( const void* ptr )
{
    return cvSeqSlice( (CvSeq*)ptr, CC_WHOLE_SEQ,
                       0 /* use the same storage as for the original sequence */, 1 );
}


static void
icvWriteHeaderData( CFileStorage* fs, const CvSeq* seq,
                    vstr_t** attr, int initial_header_size )
{
    char header_dt_buf[128];
    const char* header_dt = cvAttrValue( attr, "header_dt" );

    if( header_dt )
    {
        int dt_header_size;
        dt_header_size = icvCalcElemSize( header_dt, initial_header_size );
        if( dt_header_size > seq->header_size )
            CC_Error( CC_StsUnmatchedSizes,
            "The size of header calculated from \"header_dt\" is greater than header_size" );
    }
    else if( seq->header_size > initial_header_size )
    {
        if( CC_IS_SEQ(seq) && CC_IS_SEQ_POINT_SET(seq) &&
            seq->header_size == sizeof(CvPoint2DSeq) &&
            seq->elem_size == sizeof(int)*2 )
        {
            CvPoint2DSeq* point_seq = (CvPoint2DSeq*)seq;

            cvWriteStructBegin( fs, "rect", CC_NODE_MAP + CC_NODE_FLOW );
            cvWriteInt( fs, "x", point_seq->rect.x );
            cvWriteInt( fs, "y", point_seq->rect.y );
            cvWriteInt( fs, "width", point_seq->rect.width );
            cvWriteInt( fs, "height", point_seq->rect.height );
            cvWriteStructEnd( fs );
            cvWriteInt( fs, "color", point_seq->color );
        }
        else if( CC_IS_SEQ(seq) && CC_IS_SEQ_CHAIN(seq) &&
                 CC_MAT_TYPE(seq->flags) == CC_8UC1 )
        {
            CChain* chain = (CChain*)seq;

            cvWriteStructBegin( fs, "origin", CC_NODE_MAP + CC_NODE_FLOW );
            cvWriteInt( fs, "x", chain->origin.x );
            cvWriteInt( fs, "y", chain->origin.y );
            cvWriteStructEnd( fs );
        }
        else
        {
            unsigned extra_size = seq->header_size - initial_header_size;
            // a heuristic to provide nice defaults for sequences of int's & float's
            if( extra_size % sizeof(int) == 0 )
                sprintf( header_dt_buf, "%ui", (unsigned)(extra_size/sizeof(int)) );
            else
                sprintf( header_dt_buf, "%uu", extra_size );
            header_dt = header_dt_buf;
        }
    }

    if( header_dt )
    {
        cvWriteString( fs, "header_dt", header_dt, 0 );
        cvWriteStructBegin( fs, "header_user_data", CC_NODE_SEQ + CC_NODE_FLOW );
        cvWriteRawData( fs, (uchar*)seq + sizeof(CvSeq), 1, header_dt );
        cvWriteStructEnd( fs );
    }
}


static char*
icvGetFormat( const CvSeq* seq, const char* dt_key, vstr_t** attr,
              int initial_elem_size, char* dt_buf )
{
    char* dt = 0;
    dt = (char*)cvAttrValue( attr, dt_key );

    if( dt )
    {
        int dt_elem_size;
        dt_elem_size = icvCalcElemSize( dt, initial_elem_size );
        if( dt_elem_size != seq->elem_size )
            CC_Error( CC_StsUnmatchedSizes,
            "The size of element calculated from \"dt\" and "
            "the elem_size do not match" );
    }
    else if( CC_MAT_TYPE(seq->flags) != 0 || seq->elem_size == 1 )
    {
        if( CC_ELEM_SIZE(seq->flags) != seq->elem_size )
            CC_Error( CC_StsUnmatchedSizes,
            "Size of sequence element (elem_size) is inconsistent with seq->flags" );
        dt = icvEncodeFormat( CC_MAT_TYPE(seq->flags), dt_buf );
    }
    else if( seq->elem_size > initial_elem_size )
    {
        unsigned extra_elem_size = seq->elem_size - initial_elem_size;
        // a heuristic to provide nice defaults for sequences of int's & float's
        if( extra_elem_size % sizeof(int) == 0 )
            sprintf( dt_buf, "%ui", (unsigned)(extra_elem_size/sizeof(int)) );
        else
            sprintf( dt_buf, "%uu", extra_elem_size );
        dt = dt_buf;
    }

    return dt;
}


static void
icvWriteSeq( CFileStorage* fs, const char* name,
             const void* struct_ptr,
             vstr_t* attr, int level )
{
    const CvSeq* seq = (CvSeq*)struct_ptr;
    CvSeqBlock* block;
    char buf[128];
    char dt_buf[128], *dt;

    assert( CC_IS_SEQ( seq ));
    cvWriteStructBegin( fs, name, CC_NODE_MAP, CC_TYPE_NAME_SEQ );

    if( level >= 0 )
        cvWriteInt( fs, "level", level );

    dt = icvGetFormat( seq, "dt", &attr, 0, dt_buf );

    strcpy(buf, "");
    if( CC_IS_SEQ_CLOSED(seq) )
        strcat(buf, " closed");
    if( CC_IS_SEQ_HOLE(seq) )
        strcat(buf, " hole");
    if( CC_IS_SEQ_CURVE(seq) )
        strcat(buf, " curve");
    if( CC_SEQ_ELTYPE(seq) == 0 && seq->elem_size != 1 )
        strcat(buf, " untyped");

    cvWriteString( fs, "flags", buf + (buf[0] ? 1 : 0), 1 );

    cvWriteInt( fs, "count", seq->total );

    cvWriteString( fs, "dt", dt, 0 );

    icvWriteHeaderData( fs, seq, &attr, sizeof(CvSeq) );
    cvWriteStructBegin( fs, "data", CC_NODE_SEQ + CC_NODE_FLOW );

    for( block = seq->first; block; block = block->next )
    {
        cvWriteRawData( fs, block->data, block->count, dt );
        if( block == seq->first->prev )
            break;
    }
    cvWriteStructEnd( fs );
    cvWriteStructEnd( fs );
}


static void
icvWriteSeqTree( CFileStorage* fs, const char* name,
                 const void* struct_ptr, vstr_t* attr )
{
    const CvSeq* seq = (CvSeq*)struct_ptr;
    const char* recursive_value = cvAttrValue( &attr, "recursive" );
    int is_recursive = recursive_value &&
                       strcmp(recursive_value,"0") != 0 &&
                       strcmp(recursive_value,"false") != 0 &&
                       strcmp(recursive_value,"False") != 0 &&
                       strcmp(recursive_value,"FALSE") != 0;

    assert( CC_IS_SEQ( seq ));

    if( !is_recursive )
    {
        icvWriteSeq( fs, name, seq, attr, -1 );
    }
    else
    {
        CvTreeNodeIterator tree_iterator;

        cvWriteStructBegin( fs, name, CC_NODE_MAP, CC_TYPE_NAME_SEQ_TREE );
        cvWriteStructBegin( fs, "sequences", CC_NODE_SEQ );
        cvInitTreeNodeIterator( &tree_iterator, seq, INT_MAX );

        for(;;)
        {
            if( !tree_iterator.node )
                break;
            icvWriteSeq( fs, 0, tree_iterator.node, attr, tree_iterator.level );
            cvNextTreeNode( &tree_iterator );
        }

        cvWriteStructEnd( fs );
        cvWriteStructEnd( fs );
    }
}


static void*
icvReadSeq( CFileStorage* fs, CFileNode* node )
{
    void* ptr = 0;
    CvSeq* seq;
    CvSeqBlock* block;
    CFileNode *data, *header_node, *rect_node, *origin_node;
    CvSeqReader reader;
    int total, flags;
    int elem_size, header_size = sizeof(CvSeq);
    int fmt_pairs[CC_FS_MAX_FMT_PAIRS], i, fmt_pair_count;
    int items_per_elem = 0;
    const char* flags_str;
    const char* header_dt;
    const char* dt;
    char* endptr = 0;

    flags_str = cvReadStringByName( fs, node, "flags", 0 );
    total = cvReadIntByName( fs, node, "count", -1 );
    dt = cvReadStringByName( fs, node, "dt", 0 );

    if( !flags_str || total == -1 || !dt )
        CC_Error( CC_StsError, "Some of essential sequence attributes are absent" );

    flags = CC_SEQ_MAGIC_VAL;

    if( cc_isdigit(flags_str[0]) )
    {
        const int OLD_SEQ_ELTYPE_BITS = 9;
        const int OLD_SEQ_ELTYPE_MASK = (1 << OLD_SEQ_ELTYPE_BITS) - 1;
        const int OLD_SEQ_KIND_BITS = 3;
        const int OLD_SEQ_KIND_MASK = ((1 << OLD_SEQ_KIND_BITS) - 1) << OLD_SEQ_ELTYPE_BITS;
        const int OLD_SEQ_KIND_CURVE = 1 << OLD_SEQ_ELTYPE_BITS;
        const int OLD_SEQ_FLAG_SHIFT = OLD_SEQ_KIND_BITS + OLD_SEQ_ELTYPE_BITS;
        const int OLD_SEQ_FLAG_CLOSED = 1 << OLD_SEQ_FLAG_SHIFT;
        const int OLD_SEQ_FLAG_HOLE = 8 << OLD_SEQ_FLAG_SHIFT;

        int flags0 = (int)strtol( flags_str, &endptr, 16 );
        if( endptr == flags_str || (flags0 & CC_MAGIC_MASK) != CC_SEQ_MAGIC_VAL )
            CC_Error( CC_StsError, "The sequence flags are invalid" );
        if( (flags0 & OLD_SEQ_KIND_MASK) == OLD_SEQ_KIND_CURVE )
            flags |= CC_SEQ_KIND_CURVE;
        if( flags0 & OLD_SEQ_FLAG_CLOSED )
            flags |= CC_SEQ_FLAG_CLOSED;
        if( flags0 & OLD_SEQ_FLAG_HOLE )
            flags |= CC_SEQ_FLAG_HOLE;
        flags |= flags0 & OLD_SEQ_ELTYPE_MASK;
    }
    else
    {
        if( strstr(flags_str, "curve") )
            flags |= CC_SEQ_KIND_CURVE;
        if( strstr(flags_str, "closed") )
            flags |= CC_SEQ_FLAG_CLOSED;
        if( strstr(flags_str, "hole") )
            flags |= CC_SEQ_FLAG_HOLE;
        if( !strstr(flags_str, "untyped") )
        {
            try
            {
                flags |= icvDecodeSimpleFormat(dt);
            }
            catch(...)
            {
            }
        }
    }

    header_dt = cvReadStringByName( fs, node, "header_dt", 0 );
    header_node = cvGetFileNodeByName( fs, node, "header_user_data" );

    if( (header_dt != 0) ^ (header_node != 0) )
        CC_Error( CC_StsError,
        "One of \"header_dt\" and \"header_user_data\" is there, while the other is not" );

    rect_node = cvGetFileNodeByName( fs, node, "rect" );
    origin_node = cvGetFileNodeByName( fs, node, "origin" );

    if( (header_node != 0) + (rect_node != 0) + (origin_node != 0) > 1 )
        CC_Error( CC_StsError, "Only one of \"header_user_data\", \"rect\" and \"origin\" tags may occur" );

    if( header_dt )
    {
        header_size = icvCalcElemSize( header_dt, header_size );
    }
    else if( rect_node )
        header_size = sizeof(CvPoint2DSeq);
    else if( origin_node )
        header_size = sizeof(CChain);

    elem_size = icvCalcElemSize( dt, 0 );
    seq = cvCreateSeq( flags, header_size, elem_size, fs->dststorage );

    if( header_node )
    {
        cvReadRawData( fs, header_node, (char*)seq + sizeof(CvSeq), header_dt );
    }
    else if( rect_node )
    {
        CvPoint2DSeq* point_seq = (CvPoint2DSeq*)seq;
        point_seq->rect.x = cvReadIntByName( fs, rect_node, "x", 0 );
        point_seq->rect.y = cvReadIntByName( fs, rect_node, "y", 0 );
        point_seq->rect.width = cvReadIntByName( fs, rect_node, "width", 0 );
        point_seq->rect.height = cvReadIntByName( fs, rect_node, "height", 0 );
        point_seq->color = cvReadIntByName( fs, node, "color", 0 );
    }
    else if( origin_node )
    {
        CChain* chain = (CChain*)seq;
        chain->origin.x = cvReadIntByName( fs, origin_node, "x", 0 );
        chain->origin.y = cvReadIntByName( fs, origin_node, "y", 0 );
    }

    cvSeqPushMulti( seq, 0, total, 0 );
    fmt_pair_count = icvDecodeFormat( dt, fmt_pairs, CC_FS_MAX_FMT_PAIRS );
    fmt_pair_count *= 2;
    for( i = 0; i < fmt_pair_count; i += 2 )
        items_per_elem += fmt_pairs[i];

    data = cvGetFileNodeByName( fs, node, "data" );
    if( !data )
        CC_Error( CC_StsError, "The image data is not found in file storage" );

    if( icvFileNodeSeqLen( data ) != total*items_per_elem )
        CC_Error( CC_StsError, "The number of stored elements does not match to \"count\"" );

    cvStartReadRawData( fs, data, &reader );
    for( block = seq->first; block; block = block->next )
    {
        int delta = block->count*items_per_elem;
        cvReadRawDataSlice( fs, &reader, delta, block->data, dt );
        if( block == seq->first->prev )
            break;
    }

    ptr = seq;
    return ptr;
}


static void*
icvReadSeqTree( CFileStorage* fs, CFileNode* node )
{
    void* ptr = 0;
    CFileNode *sequences_node = cvGetFileNodeByName( fs, node, "sequences" );
    CvSeq* sequences;
    CvSeq* root = 0;
    CvSeq* parent = 0;
    CvSeq* prev_seq = 0;
    CvSeqReader reader;
    int i, total;
    int prev_level = 0;

    if( !sequences_node || !CC_NODE_IS_SEQ(sequences_node->tag) )
        CC_Error( CC_StsParseError,
        "opencv-sequence-tree instance should contain a field \"sequences\" that should be a sequence" );

    sequences = sequences_node->data.seq;
    total = sequences->total;

    cvStartReadSeq( sequences, &reader, 0 );
    for( i = 0; i < total; i++ )
    {
        CFileNode* elem = (CFileNode*)reader.ptr;
        CvSeq* seq;
        int level;
        seq = (CvSeq*)cvRead( fs, elem );
        level = cvReadIntByName( fs, elem, "level", -1 );
        if( level < 0 )
            CC_Error( CC_StsParseError, "All the sequence tree nodes should contain \"level\" field" );
        if( !root )
            root = seq;
        if( level > prev_level )
        {
            assert( level == prev_level + 1 );
            parent = prev_seq;
            prev_seq = 0;
            if( parent )
                parent->v_next = seq;
        }
        else if( level < prev_level )
        {
            for( ; prev_level > level; prev_level-- )
                prev_seq = prev_seq->v_prev;
            parent = prev_seq->v_prev;
        }
        seq->h_prev = prev_seq;
        if( prev_seq )
            prev_seq->h_next = seq;
        seq->v_prev = parent;
        prev_seq = seq;
        prev_level = level;
        CC_NEXT_SEQ_ELEM( sequences->elem_size, reader );
    }

    ptr = root;
    return ptr;
}

/******************************* CvGraph ******************************/

static int
icvIsGraph( const void* ptr )
{
    return CC_IS_GRAPH(ptr);
}


static void
icvReleaseGraph( void** ptr )
{
    if( !ptr )
        CC_Error( CC_StsNullPtr, "NULL double pointer" );

    *ptr = 0; // it's impossible now to release graph, so just clear the pointer
}


static void*
icvCloneGraph( const void* ptr )
{
    return cvCloneGraph( (const CvGraph*)ptr, 0 );
}


static void
icvWriteGraph( CFileStorage* fs, const char* name,
               const void* struct_ptr, vstr_t* attr )
{
    int* flag_buf = 0;
    char* write_buf = 0;
    const CvGraph* graph = (const CvGraph*)struct_ptr;
    CvSeqReader reader;
    char buf[128];
    int i, k, vtx_count, edge_count;
    char vtx_dt_buf[128], *vtx_dt;
    char edge_dt_buf[128], *edge_dt;
    int write_buf_size;

    assert( CC_IS_GRAPH(graph) );
    vtx_count = cvGraphGetVtxCount( graph );
    edge_count = cvGraphGetEdgeCount( graph );
    flag_buf = (int*)cAlloc( vtx_count*sizeof(flag_buf[0]));

    // count vertices
    cvStartReadSeq( (CvSeq*)graph, &reader );
    for( i = 0, k = 0; i < graph->total; i++ )
    {
        if( CC_IS_SET_ELEM( reader.ptr ))
        {
            CvGraphVtx* vtx = (CvGraphVtx*)reader.ptr;
            flag_buf[k] = vtx->flags;
            vtx->flags = k++;
        }
        CC_NEXT_SEQ_ELEM( graph->elem_size, reader );
    }

    // write header
    cvWriteStructBegin( fs, name, CC_NODE_MAP, CC_TYPE_NAME_GRAPH );

    cvWriteString(fs, "flags", CC_IS_GRAPH_ORIENTED(graph) ? "oriented" : "", 1);

    cvWriteInt( fs, "vertex_count", vtx_count );
    vtx_dt = icvGetFormat( (CvSeq*)graph, "vertex_dt",
                    &attr, sizeof(CvGraphVtx), vtx_dt_buf );
    if( vtx_dt )
        cvWriteString( fs, "vertex_dt", vtx_dt, 0 );

    cvWriteInt( fs, "edge_count", edge_count );
    edge_dt = icvGetFormat( (CvSeq*)graph->edges, "edge_dt",
                                &attr, sizeof(CvGraphEdge), buf );
    sprintf( edge_dt_buf, "2if%s", edge_dt ? edge_dt : "" );
    edge_dt = edge_dt_buf;
    cvWriteString( fs, "edge_dt", edge_dt, 0 );

    icvWriteHeaderData( fs, (CvSeq*)graph, &attr, sizeof(CvGraph) );

    write_buf_size = MAX( 3*graph->elem_size, 1 << 16 );
    write_buf_size = MAX( 3*graph->edges->elem_size, write_buf_size );
    write_buf = (char*)cAlloc( write_buf_size );

    // as vertices and edges are written in similar way,
    // do it as a parametrized 2-iteration loop
    for( k = 0; k < 2; k++ )
    {
        const char* dt = k == 0 ? vtx_dt : edge_dt;
        if( dt )
        {
            CvSet* data = k == 0 ? (CvSet*)graph : graph->edges;
            int elem_size = data->elem_size;
            int write_elem_size = icvCalcElemSize( dt, 0 );
            char* src_ptr = write_buf;
            int write_max = write_buf_size / write_elem_size, write_count = 0;

            // alignment of user part of the edge data following 2if
            int edge_user_align = sizeof(float);

            if( k == 1 )
            {
                int fmt_pairs[CC_FS_MAX_FMT_PAIRS], fmt_pair_count;
                fmt_pair_count = icvDecodeFormat( dt, fmt_pairs, CC_FS_MAX_FMT_PAIRS );
                if( fmt_pair_count > 2 && CC_ELEM_SIZE(fmt_pairs[2*2+1]) >= (int)sizeof(double))
                    edge_user_align = sizeof(double);
            }

            cvWriteStructBegin( fs, k == 0 ? "vertices" : "edges",
                                CC_NODE_SEQ + CC_NODE_FLOW );
            cvStartReadSeq( (CvSeq*)data, &reader );
            for( i = 0; i < data->total; i++ )
            {
                if( CC_IS_SET_ELEM( reader.ptr ))
                {
                    if( k == 0 ) // vertices
                        memcpy( src_ptr, reader.ptr + sizeof(CvGraphVtx), write_elem_size );
                    else
                    {
                        CvGraphEdge* edge = (CvGraphEdge*)reader.ptr;
                        src_ptr = (char*)cvAlignPtr( src_ptr, sizeof(int) );
                        ((int*)src_ptr)[0] = edge->vtx[0]->flags;
                        ((int*)src_ptr)[1] = edge->vtx[1]->flags;
                        *(float*)(src_ptr + sizeof(int)*2) = edge->weight;
                        if( elem_size > (int)sizeof(CvGraphEdge) )
                        {
                            char* src_ptr2 = (char*)cvAlignPtr( src_ptr + 2*sizeof(int)
                                                + sizeof(float), edge_user_align );
                            memcpy( src_ptr2, edge + 1, elem_size - sizeof(CvGraphEdge) );
                        }
                    }
                    src_ptr += write_elem_size;
                    if( ++write_count >= write_max )
                    {
                        cvWriteRawData( fs, write_buf, write_count, dt );
                        write_count = 0;
                        src_ptr = write_buf;
                    }
                }
                CC_NEXT_SEQ_ELEM( data->elem_size, reader );
            }

            if( write_count > 0 )
                cvWriteRawData( fs, write_buf, write_count, dt );
            cvWriteStructEnd( fs );
        }
    }

    cvWriteStructEnd( fs );

    // final stage. restore the graph flags
    cvStartReadSeq( (CvSeq*)graph, &reader );
    vtx_count = 0;
    for( i = 0; i < graph->total; i++ )
    {
        if( CC_IS_SET_ELEM( reader.ptr ))
            ((CvGraphVtx*)reader.ptr)->flags = flag_buf[vtx_count++];
        CC_NEXT_SEQ_ELEM( graph->elem_size, reader );
    }

    cFree( &write_buf );
    cFree( &flag_buf );
}


static void*
icvReadGraph( CFileStorage* fs, CFileNode* node )
{
    void* ptr = 0;
    char* read_buf = 0;
    CvGraphVtx** vtx_buf = 0;
    CvGraph* graph;
    CFileNode *header_node, *vtx_node, *edge_node;
    int flags, vtx_count, edge_count;
    int vtx_size = sizeof(CvGraphVtx), edge_size, header_size = sizeof(CvGraph);
    int src_vtx_size = 0, src_edge_size;
    int fmt_pairs[CC_FS_MAX_FMT_PAIRS], fmt_pair_count;
    int vtx_items_per_elem = 0, edge_items_per_elem = 0;
    int edge_user_align = sizeof(float);
    int read_buf_size;
    int i, k;
    const char* flags_str;
    const char* header_dt;
    const char* vtx_dt;
    const char* edge_dt;
    char* endptr = 0;

    flags_str = cvReadStringByName( fs, node, "flags", 0 );
    vtx_dt = cvReadStringByName( fs, node, "vertex_dt", 0 );
    edge_dt = cvReadStringByName( fs, node, "edge_dt", 0 );
    vtx_count = cvReadIntByName( fs, node, "vertex_count", -1 );
    edge_count = cvReadIntByName( fs, node, "edge_count", -1 );

    if( !flags_str || vtx_count == -1 || edge_count == -1 || !edge_dt )
        CC_Error( CC_StsError, "Some of essential graph attributes are absent" );

    flags = CC_SET_MAGIC_VAL + CC_GRAPH;

    if( isxdigit(flags_str[0]) )
    {
        const int OLD_SEQ_ELTYPE_BITS = 9;
        const int OLD_SEQ_KIND_BITS = 3;
        const int OLD_SEQ_FLAG_SHIFT = OLD_SEQ_KIND_BITS + OLD_SEQ_ELTYPE_BITS;
        const int OLD_GRAPH_FLAG_ORIENTED = 1 << OLD_SEQ_FLAG_SHIFT;

        int flags0 = (int)strtol( flags_str, &endptr, 16 );
        if( endptr == flags_str || (flags0 & CC_MAGIC_MASK) != CC_SET_MAGIC_VAL )
            CC_Error( CC_StsError, "The sequence flags are invalid" );
        if( flags0 & OLD_GRAPH_FLAG_ORIENTED )
            flags |= CC_GRAPH_FLAG_ORIENTED;
    }
    else
    {
        if( strstr(flags_str, "oriented") )
            flags |= CC_GRAPH_FLAG_ORIENTED;
    }

    header_dt = cvReadStringByName( fs, node, "header_dt", 0 );
    header_node = cvGetFileNodeByName( fs, node, "header_user_data" );

    if( (header_dt != 0) ^ (header_node != 0) )
        CC_Error( CC_StsError,
        "One of \"header_dt\" and \"header_user_data\" is there, while the other is not" );

    if( header_dt )
        header_size = icvCalcElemSize( header_dt, header_size );

    if( vtx_dt )
    {
        src_vtx_size = icvCalcElemSize( vtx_dt, 0 );
        vtx_size = icvCalcElemSize( vtx_dt, vtx_size );
        fmt_pair_count = icvDecodeFormat( edge_dt,
                            fmt_pairs, CC_FS_MAX_FMT_PAIRS );
        fmt_pair_count *= 2;
        for( i = 0; i < fmt_pair_count; i += 2 )
            vtx_items_per_elem += fmt_pairs[i];
    }

    {
        char dst_edge_dt_buf[128];
        const char* dst_edge_dt = 0;

        fmt_pair_count = icvDecodeFormat( edge_dt,
                            fmt_pairs, CC_FS_MAX_FMT_PAIRS );
        if( fmt_pair_count < 2 ||
            fmt_pairs[0] != 2 || fmt_pairs[1] != CC_32S ||
            fmt_pairs[2] < 1 || fmt_pairs[3] != CC_32F )
            CC_Error( CC_StsBadArg,
            "Graph edges should start with 2 integers and a float" );

        // alignment of user part of the edge data following 2if
        if( fmt_pair_count > 2 && CC_ELEM_SIZE(fmt_pairs[5]) >= (int)sizeof(double))
            edge_user_align = sizeof(double);

        fmt_pair_count *= 2;
        for( i = 0; i < fmt_pair_count; i += 2 )
            edge_items_per_elem += fmt_pairs[i];

        if( edge_dt[2] == 'f' || (edge_dt[2] == '1' && edge_dt[3] == 'f') )
            dst_edge_dt = edge_dt + 3 + cc_isdigit(edge_dt[2]);
        else
        {
            int val = (int)strtol( edge_dt + 2, &endptr, 10 );
            sprintf( dst_edge_dt_buf, "%df%s", val-1, endptr );
            dst_edge_dt = dst_edge_dt_buf;
        }

        edge_size = icvCalcElemSize( dst_edge_dt, sizeof(CvGraphEdge) );
        src_edge_size = icvCalcElemSize( edge_dt, 0 );
    }

    graph = cvCreateGraph( flags, header_size, vtx_size, edge_size, fs->dststorage );

    if( header_node )
        cvReadRawData( fs, header_node, (char*)graph + sizeof(CvGraph), header_dt );

    read_buf_size = MAX( src_vtx_size*3, 1 << 16 );
    read_buf_size = MAX( src_edge_size*3, read_buf_size );
    read_buf = (char*)cAlloc( read_buf_size );
    vtx_buf = (CvGraphVtx**)cAlloc( vtx_count * sizeof(vtx_buf[0]) );

    vtx_node = cvGetFileNodeByName( fs, node, "vertices" );
    edge_node = cvGetFileNodeByName( fs, node, "edges" );
    if( !edge_node )
        CC_Error( CC_StsBadArg, "No edges data" );
    if( vtx_dt && !vtx_node )
        CC_Error( CC_StsBadArg, "No vertices data" );

    // as vertices and edges are read in similar way,
    // do it as a parametrized 2-iteration loop
    for( k = 0; k < 2; k++ )
    {
        const char* dt = k == 0 ? vtx_dt : edge_dt;
        int elem_size = k == 0 ? vtx_size : edge_size;
        int src_elem_size = k == 0 ? src_vtx_size : src_edge_size;
        int items_per_elem = k == 0 ? vtx_items_per_elem : edge_items_per_elem;
        int elem_count = k == 0 ? vtx_count : edge_count;
        char* dst_ptr = read_buf;
        int read_max = read_buf_size /MAX(src_elem_size, 1), read_count = 0;
        CvSeqReader reader;
        if(dt)
            cvStartReadRawData( fs, k == 0 ? vtx_node : edge_node, &reader );

        for( i = 0; i < elem_count; i++ )
        {
            if( read_count == 0 && dt )
            {
                int count = MIN( elem_count - i, read_max )*items_per_elem;
                cvReadRawDataSlice( fs, &reader, count, read_buf, dt );
                read_count = count;
                dst_ptr = read_buf;
            }

            if( k == 0 )
            {
                CvGraphVtx* vtx;
                cvGraphAddVtx( graph, 0, &vtx );
                vtx_buf[i] = vtx;
                if( dt )
                    memcpy( vtx + 1, dst_ptr, src_elem_size );
            }
            else
            {
                CvGraphEdge* edge = 0;
                int vtx1 = ((int*)dst_ptr)[0];
                int vtx2 = ((int*)dst_ptr)[1];
                int result;

                if( (unsigned)vtx1 >= (unsigned)vtx_count ||
                    (unsigned)vtx2 >= (unsigned)vtx_count )
                    CC_Error( CC_StsOutOfRange,
                    "Some of stored vertex indices are out of range" );

                result = cvGraphAddEdgeByPtr( graph,
                    vtx_buf[vtx1], vtx_buf[vtx2], 0, &edge );

                if( result == 0 )
                    CC_Error( CC_StsBadArg, "Duplicated edge has occured" );

                edge->weight = *(float*)(dst_ptr + sizeof(int)*2);
                if( elem_size > (int)sizeof(CvGraphEdge) )
                {
                    char* dst_ptr2 = (char*)cvAlignPtr( dst_ptr + sizeof(int)*2 +
                                                sizeof(float), edge_user_align );
                    memcpy( edge + 1, dst_ptr2, elem_size - sizeof(CvGraphEdge) );
                }
            }

            dst_ptr += src_elem_size;
            read_count--;
        }
    }

    ptr = graph;
    cFree( &read_buf );
    cFree( &vtx_buf );

    return ptr;
}

/****************************************************************************************\
*                                    RTTI Functions                                      *
\****************************************************************************************/

CvTypeInfo *CvType::first = 0, *CvType::last = 0;

CvType::CvType( const char* type_name,
                CvIsInstanceFunc is_instance, CvReleaseFunc release,
                CvReadFunc read, CvWriteFunc write, CvCloneFunc clone )
{
    CvTypeInfo _info;
    _info.flags = 0;
    _info.header_size = sizeof(_info);
    _info.type_name = type_name;
    _info.prev = _info.next = 0;
    _info.is_instance = is_instance;
    _info.release = release;
    _info.clone = clone;
    _info.read = read;
    _info.write = write;

    cvRegisterType( &_info );
    info = first;
}


CvType::~CvType()
{
    cvUnregisterType( info->tid_name );
}


CvType seq_type( CC_TYPE_NAME_SEQ, icvIsSeq, icvReleaseSeq, icvReadSeq,
                 icvWriteSeqTree /* this is the entry point for
                 writing a single sequence too */, icvCloneSeq );

CvType seq_tree_type( CC_TYPE_NAME_SEQ_TREE, icvIsSeq, icvReleaseSeq,
                      icvReadSeqTree, icvWriteSeqTree, icvCloneSeq );

CvType seq_graph_type( CC_TYPE_NAME_GRAPH, icvIsGraph, icvReleaseGraph,
                       icvReadGraph, icvWriteGraph, icvCloneGraph );

CvType sparse_mat_type( CC_TYPE_NAME_SPARSE_MAT, icvIsSparseMat,
                        (CvReleaseFunc)cvReleaseSparseMat, icvReadSparseMat,
                        icvWriteSparseMat, (CvCloneFunc)cvCloneSparseMat );

CvType image_type( CC_TYPE_NAME_IMAGE, icvIsImage, (CvReleaseFunc)cvReleaseImage,
                   icvReadImage, icvWriteImage, (CvCloneFunc)cvCloneImage );

CvType mat_type( CC_TYPE_NAME_MAT, icvIsMat, (CvReleaseFunc)cvReleaseMat,
                 icvReadMat, icvWriteMat, (CvCloneFunc)cvCloneMat );

CvType matnd_type( CC_TYPE_NAME_MATND, icvIsMatND, (CvReleaseFunc)cvReleaseMatND,
                   icvReadMatND, icvWriteMatND, (CvCloneFunc)cvCloneMatND );

CC_IMPL  void
cvRegisterType( const CvTypeInfo* _info )
{
    CvTypeInfo* info = 0;
    int i, len;
    char c;

    //if( !CvType::first )
    //    icvCreateStandardTypes();

    if( !_info || _info->header_size != sizeof(CvTypeInfo) )
        CC_Error( CC_StsBadSize, "Invalid type info" );

    if( !_info->is_instance || !_info->release ||
        !_info->read || !_info->write )
        CC_Error( CC_StsNullPtr,
        "Some of required function pointers "
        "(is_instance, release, read or write) are NULL");

    c = _info->tid_name[0];
    if( !cc_isalpha(c) && c != '_' )
        CC_Error( CC_StsBadArg, "Type name should start with a letter or _" );

    len = (int)strlen(_info->tid_name);

    for( i = 0; i < len; i++ )
    {
        c = _info->tid_name[i];
        if( !cc_isalnum(c) && c != '-' && c != '_' )
            CC_Error( CC_StsBadArg,
            "Type name should contain only letters, digits, - and _" );
    }

    info = (CvTypeInfo*)cAlloc( sizeof(*info) + len + 1 );

    *info = *_info;
    info->tid_name = (char*)(info + 1);
    memcpy( (char*)info->tid_name, _info->tid_name, len + 1 );

    info->flags = 0;
    info->next = CvType::first;
    info->prev = 0;
    if( CvType::first )
        CvType::first->prev = info;
    else
        CvType::last = info;
    CvType::first = info;
}



CC_IMPL CvTypeInfo*
cvFindType( const char* type_name )
{
    CTypeInfo* info = 0;

    if (type_name)
      for( info = CType::first; info != 0; info = info->next )
        if( strcmp( info->tid_name, type_name ) == 0 )
      break;

    return info;
}


CC_IMPL CvTypeInfo*
cvTypeOf( const void* struct_ptr )
{
    CvTypeInfo* info = 0;

    if( struct_ptr )
    {
        for( info = CvType::first; info != 0; info = info->next )
            if( info->is_instance( struct_ptr ))
                break;
    }

    return info;
}


/* universal functions */
CC_IMPL void
cvRelease( void** struct_ptr )
{
    CvTypeInfo* info;

    if( !struct_ptr )
        CC_Error( CC_StsNullPtr, "NULL double pointer" );

    if( *struct_ptr )
    {
        info = cvTypeOf( *struct_ptr );
        if( !info )
            CC_Error( CC_StsError, "Unknown object type" );
        if( !info->release )
            CC_Error( CC_StsError, "release function pointer is NULL" );

        info->release( struct_ptr );
        *struct_ptr = 0;
    }
}


void* cvClone( const void* struct_ptr )
{
    void* struct_copy = 0;
    CvTypeInfo* info;

    if( !struct_ptr )
        CC_Error( CC_StsNullPtr, "NULL structure pointer" );

    info = cvTypeOf( struct_ptr );
    if( !info )
        CC_Error( CC_StsError, "Unknown object type" );
    if( !info->clone )
        CC_Error( CC_StsError, "clone function pointer is NULL" );

    struct_copy = info->clone( struct_ptr );
    return struct_copy;
}


/* reads matrix, image, sequence, graph etc. */
CC_IMPL void*
cvRead( CFileStorage* fs, CFileNode* node, vstr_t** list )
{
    void* obj = 0;
    CC_CHECK_FILE_STORAGE( fs );

    if( !node )
        return 0;

    if( !CC_NODE_IS_USER(node->tag) || !node->info )
        CC_Error( CC_StsError, "The node does not represent a user object (unknown type?)" );

    obj = node->info->read( fs, node );
    if( list )
        *list = cvAttrList(0,0);

    return obj;
}


/* writes matrix, image, sequence, graph etc. */
CC_IMPL void
cvWrite( CFileStorage* fs, const char* name,
         const void* ptr, vstr_t* attributes )
{
    CvTypeInfo* info;

    CC_CHECK_OUTPUT_FILE_STORAGE( fs );

    if( !ptr )
        CC_Error( CC_StsNullPtr, "Null pointer to the written object" );

    info = cvTypeOf( ptr );
    if( !info )
        CC_Error( CC_StsBadArg, "Unknown object" );

    if( !info->write )
        CC_Error( CC_StsBadArg, "The object does not have write function" );

    info->write( fs, name, ptr, attributes );
}


/* simple API for reading/writing data */
CC_IMPL void
cvSave( const char* filename, const void* struct_ptr,
        const char* _name, const char* comment, vstr_t* attributes )
{
    CFileStorage* fs = 0;

    if( !struct_ptr )
        CC_Error( CC_StsNullPtr, "NULL object pointer" );

    fs = cvOpenFileStorage( filename, 0, CC_STORAGE_WRITE );
    if( !fs )
        CC_Error( CC_StsError, "Could not open the file storage. Check the path and permissions" );

    String name = _name ? String(_name) : FileStorage::getDefaultObjectName(filename);

    if( comment )
        cvWriteComment( fs, comment, 0 );
    cvWrite( fs, name, struct_ptr, attributes );
    cvReleaseFileStorage( &fs );
}

CC_IMPL void*
cvLoad( const char* filename, CvMemStorage* memstorage,
        const char* name, const char** _real_name )
{
    void* ptr = 0;
    const char* real_name = 0;
    FileStorage fs(cvOpenFileStorage(filename, memstorage, CC_STORAGE_READ));

    CFileNode* node = 0;

    if( !fs.isOpened() )
        return 0;

    if( name )
    {
        node = cvGetFileNodeByName( *fs, 0, name );
    }
    else
    {
        int i, k;
        for( k = 0; k < (*fs)->roots->total; k++ )
        {
            CvSeq* seq;
            CvSeqReader reader;

            node = (CFileNode*)cvGetSeqElem( (*fs)->roots, k );
            if( !CC_NODE_IS_MAP( node->tag ))
                return 0;
            seq = node->data.seq;
            node = 0;

            cvStartReadSeq( seq, &reader, 0 );

            // find the first element in the map
            for( i = 0; i < seq->total; i++ )
            {
                if( CC_IS_SET_ELEM( reader.ptr ))
                {
                    node = (CFileNode*)reader.ptr;
                    goto stop_search;
                }
                CC_NEXT_SEQ_ELEM( seq->elem_size, reader );
            }
        }

stop_search:
        ;
    }

    if( !node )
        CC_Error( CC_StsObjectNotFound, "Could not find the/an object in file storage" );

    real_name = cvGetFileNodeName( node );
    ptr = cvRead( *fs, node, 0 );

    // sanity check
    if( !memstorage && (CC_IS_SEQ( ptr ) || CC_IS_SET( ptr )) )
        CC_Error( CC_StsNullPtr,
        "NULL memory storage is passed - the loaded dynamic structure can not be stored" );

    if( cvGetErrStatus() < 0 )
    {
        cvRelease( (void**)&ptr );
        real_name = 0;
    }

    if( _real_name)
    {
    if (real_name)
    {
        *_real_name = (const char*)cAlloc(strlen(real_name));
            memcpy((void*)*_real_name, real_name, strlen(real_name));
    } else {
        *_real_name = 0;
    }
    }

    return ptr;
}


///////////////////////// new C++ interface for CFileStorage ///////////////////////////


static void getElemSize( const char* fmt, size_t& elemSize, size_t& cn )
{
    const char* dt = fmt;
    cn = 1;
    if( cc_isdigit(dt[0]) )
    {
        cn = dt[0] - '0';
        dt++;
    }
    char c = dt[0];
    elemSize = cn*(c == 'u' || c == 'c' ? sizeof(uchar) : c == 'w' || c == 's' ? sizeof(ushort) :
        c == 'i' ? sizeof(int) : c == 'f' ? sizeof(float) : c == 'd' ? sizeof(double) :
        c == 'r' ? sizeof(void*) : (size_t)0);
}

FileStorage& operator << (FileStorage& fs, const char* str)
{
    CC_INSTRUMENT_REGION()

    enum { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        VALUE_EXPECTED = FileStorage::VALUE_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    const char* _str = str;
    if( !fs.isOpened() || !_str )
        return fs;
    if( *_str == '}' || *_str == ']' )
    {
        if( fs.structs.empty() )
            CC_Error_( CC_StsError, ("Extra closing '%c'", *_str) );
        if( (*_str == ']' ? '[' : '{') != fs.structs.back() )
            CC_Error_( CC_StsError,
            ("The closing '%c' does not match the opening '%c'", *_str, fs.structs.back()));
        fs.structs.pop_back();
        fs.state = fs.structs.empty() || fs.structs.back() == '{' ?
            INSIDE_MAP + NAME_EXPECTED : VALUE_EXPECTED;
        cvWriteStructEnd( *fs );
        fs.elname = String();
    }
    else if( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        if (!cc_isalpha(*_str) && *_str != '_')
            CC_Error_( CC_StsError, ("Incorrect element name %s", _str) );
        fs.elname = str;
        fs.state = VALUE_EXPECTED + INSIDE_MAP;
    }
    else if( (fs.state & 3) == VALUE_EXPECTED )
    {
        if( *_str == '{' || *_str == '[' )
        {
            fs.structs.push_back(*_str);
            int flags = *_str++ == '{' ? CC_NODE_MAP : CC_NODE_SEQ;
            fs.state = flags == CC_NODE_MAP ? INSIDE_MAP +
                NAME_EXPECTED : VALUE_EXPECTED;
            if( *_str == ':' )
            {
                flags |= CC_NODE_FLOW;
                _str++;
            }
            cvWriteStructBegin( *fs, fs.elname.size() > 0 ? fs.elname : 0,
                flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        else
        {
            write( fs, fs.elname, (_str[0] == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            if( fs.state == INSIDE_MAP + VALUE_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    else
        CC_Error( CC_StsError, "Invalid fs.state" );
    return fs;
}


void FileStorage::writeRaw( const char* fmt, const uchar* vec, size_t len )
{
    if( !isOpened() )
        return;
    size_t elemSize, cn;
    getElemSize( fmt, elemSize, cn );
    CC_Assert( len % elemSize == 0 );
    cvWriteRawData( fs, vec, (int)(len/elemSize), fmt);
}


void FileStorage::writeObj( const char* name, const void* obj )
{
    if( !isOpened() )
        return;
    cvWrite( fs, name.size() > 0 ? name : 0, obj );
}

void FileStorage::write( const char* name, double val )
{
    *this << name << val;
}

void FileStorage::write( const char* name, const char* val )
{
    *this << name << val;
}

void FileStorage::write( const char* name, const img_t* val )
{
    *this << name << val;
}

void FileStorage::writeComment( const char* comment, bool append )
{
    cvWriteComment(fs, comment, append ? 1 : 0);
}

FileNode FileStorage::operator[](const char* nodename) const
{
    return FileNode(fs, cvGetFileNodeByName(fs, 0, nodename));
}

FileNode FileStorage::operator[](const char* nodename) const
{
    return FileNode(fs, cvGetFileNodeByName(fs, 0, nodename));
}

FileNode FileNode::operator[](const char* nodename) const
{
    return FileNode(fs, cvGetFileNodeByName(fs, node, nodename));
}

FileNode FileNode::operator[](const char* nodename) const
{
    return FileNode(fs, cvGetFileNodeByName(fs, node, nodename));
}

FileNode FileNode::operator[](int i) const
{
    return isSeq() ? FileNode(fs, (CFileNode*)cvGetSeqElem(node->data.seq, i)) :
        i == 0 ? *this : FileNode();
}

String FileNode::name() const
{
    const char* str;
    return !node || (str = cvGetFileNodeName(node)) == 0 ? String() : String(str);
}

void* FileNode::readObj() const
{
    if( !fs || !node )
        return 0;
    return cvRead( (CFileStorage*)fs, (CFileNode*)node );
}

FileNodeIterator::FileNodeIterator()
{
    fs = 0;
    container = 0;
    reader.ptr = 0;
    remaining = 0;
}

FileNodeIterator::FileNodeIterator(const CFileStorage* _fs,
                                   const CFileNode* _node, size_t _ofs)
{
    if( _fs && _node && CC_NODE_TYPE(_node->tag) != CC_NODE_NONE )
    {
        int node_type = _node->tag & FileNode::TYPE_MASK;
        fs = _fs;
        container = _node;
        if( !(_node->tag & FileNode::USER) && (node_type == FileNode::SEQ || node_type == FileNode::MAP) )
        {
            cvStartReadSeq( _node->data.seq, (CvSeqReader*)&reader );
            remaining = FileNode(_fs, _node).size();
        }
        else
        {
            reader.ptr = (schar*)_node;
            reader.seq = 0;
            remaining = 1;
        }
        (*this) += (int)_ofs;
    }
    else
    {
        fs = 0;
        container = 0;
        reader.ptr = 0;
        remaining = 0;
    }
}

FileNodeIterator::FileNodeIterator(const FileNodeIterator& it)
{
    fs = it.fs;
    container = it.container;
    reader = it.reader;
    remaining = it.remaining;
}

FileNodeIterator& FileNodeIterator::operator ++()
{
    if( remaining > 0 )
    {
        if( reader.seq )
        {
            if( ((reader).ptr += (((CvSeq*)reader.seq)->elem_size)) >= (reader).block_max )
            {
                cvChangeSeqBlock( (CvSeqReader*)&(reader), 1 );
            }
        }
        remaining--;
    }
    return *this;
}

FileNodeIterator FileNodeIterator::operator ++(int)
{
    FileNodeIterator it = *this;
    ++(*this);
    return it;
}

FileNodeIterator& FileNodeIterator::operator --()
{
    if( remaining < FileNode(fs, container).size() )
    {
        if( reader.seq )
        {
            if( ((reader).ptr -= (((CvSeq*)reader.seq)->elem_size)) < (reader).block_min )
            {
                cvChangeSeqBlock( (CvSeqReader*)&(reader), -1 );
            }
        }
        remaining++;
    }
    return *this;
}

FileNodeIterator FileNodeIterator::operator --(int)
{
    FileNodeIterator it = *this;
    --(*this);
    return it;
}

FileNodeIterator& FileNodeIterator::operator += (int ofs)
{
    if( ofs == 0 )
        return *this;
    if( ofs > 0 )
        ofs = MIN(ofs, (int)remaining);
    else
    {
        size_t count = FileNode(fs, container).size();
        ofs = (int)(remaining - MIN(remaining - ofs, count));
    }
    remaining -= ofs;
    if( reader.seq )
        cvSetSeqReaderPos( (CvSeqReader*)&reader, ofs, 1 );
    return *this;
}

FileNodeIterator& FileNodeIterator::operator -= (int ofs)
{
    return operator += (-ofs);
}


FileNodeIterator& FileNodeIterator::readRaw( const char* fmt, uchar* vec, size_t maxCount )
{
    if( fs && container && remaining > 0 )
    {
        size_t elem_size, cn;
        getElemSize( fmt, elem_size, cn );
        CC_Assert( elem_size > 0 );
        size_t count = MIN(remaining, maxCount);

        if( reader.seq )
        {
            cvReadRawDataSlice( fs, (CvSeqReader*)&reader, (int)count, vec, fmt );
            remaining -= count*cn;
        }
        else
        {
            cvReadRawData( fs, container, vec, fmt );
            remaining = 0;
        }
    }
    return *this;
}


void write( FileStorage& fs, const char* name, int value )
{ cvWriteInt( *fs, name.size() ? name : 0, value ); }

void write( FileStorage& fs, const char* name, float value )
{ cvWriteReal( *fs, name.size() ? name : 0, value ); }

void write( FileStorage& fs, const char* name, double value )
{ cvWriteReal( *fs, name.size() ? name : 0, value ); }

void write( FileStorage& fs, const char* name, const char* value )
{ cvWriteString( *fs, name.size() ? name : 0, value ); }

void writeScalar(FileStorage& fs, int value )
{ cvWriteInt( *fs, 0, value ); }

void writeScalar(FileStorage& fs, float value )
{ cvWriteReal( *fs, 0, value ); }

void writeScalar(FileStorage& fs, double value )
{ cvWriteReal( *fs, 0, value ); }

void writeScalar(FileStorage& fs, const char* value )
{ cvWriteString( *fs, 0, value ); }


void write( FileStorage& fs, const char* name, const img_t& value )
{
    if( value.dims <= 2 )
    {
        img_t mat = value;
        cvWrite( *fs, name.size() ? name : 0, &mat );
    }
    else
    {
        CvMatND mat = value;
        cvWrite( *fs, name.size() ? name : 0, &mat );
    }
}

// TODO: the 4 functions below need to be implemented more efficiently
void write( FileStorage& fs, const char* name, const SparseMat& value )
{
    Ptr<CvSparseMat> mat(cvCreateSparseMat(value));
    cvWrite( *fs, name.size() ? name : 0, mat );
}


internal::WriteStructContext::WriteStructContext(FileStorage& _fs,
    const char* name, int flags, const char* typeName) : fs(&_fs)
{
    cvWriteStructBegin(**fs, !name.empty() ? name : 0, flags,
                       !typeName.empty() ? typeName : 0);
    fs->elname = String();
    if ((flags & FileNode::TYPE_MASK) == FileNode::SEQ)
    {
        fs->state = FileStorage::VALUE_EXPECTED;
        fs->structs.push_back('[');
    }
    else
    {
        fs->state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        fs->structs.push_back('{');
    }
}

internal::WriteStructContext::~WriteStructContext()
{
    cvWriteStructEnd(**fs);
    fs->structs.pop_back();
    fs->state = fs->structs.empty() || fs->structs.back() == '{' ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::VALUE_EXPECTED;
    fs->elname = String();
}


void read( const FileNode& node, img_t& mat, const img_t& default_mat )
{
    if( node.empty() )
    {
        default_mat.copyTo(mat);
        return;
    }
    void* obj = cvRead((CFileStorage*)node.fs, (CFileNode*)*node);
    if(CC_IS_MAT_HDR_Z(obj))
    {
        cvarrToMat(obj).copyTo(mat);
        cvReleaseMat((img_t**)&obj);
    }
    else if(CC_IS_MATND_HDR(obj))
    {
        cvarrToMat(obj).copyTo(mat);
        cvReleaseMatND((CvMatND**)&obj);
    }
    else
    {
        cvRelease(&obj);
        CC_Error(CC_StsBadArg, "Unknown array type");
    }
}

void read( const FileNode& node, SparseMat& mat, const SparseMat& default_mat )
{
    if( node.empty() )
    {
        default_mat.copyTo(mat);
        return;
    }
    Ptr<CvSparseMat> m((CvSparseMat*)cvRead((CFileStorage*)node.fs, (CFileNode*)*node));
    CC_Assert(CC_IS_SPARSE_MAT(m));
    m->copyToSparseMat(mat);
}

void write(FileStorage& fs, const char* objname, const std::vector<KeyPoint>& keypoints)
{
    internal::WriteStructContext ws(fs, objname, CC_NODE_SEQ + CC_NODE_FLOW);

    int i, npoints = (int)keypoints.size();
    for( i = 0; i < npoints; i++ )
    {
        const KeyPoint& kpt = keypoints[i];
        write(fs, kpt.pt.x);
        write(fs, kpt.pt.y);
        write(fs, kpt.size);
        write(fs, kpt.angle);
        write(fs, kpt.response);
        write(fs, kpt.octave);
        write(fs, kpt.class_id);
    }
}


void read(const FileNode& node, std::vector<KeyPoint>& keypoints)
{
    keypoints.resize(0);
    FileNodeIterator it = node.begin(), it_end = node.end();
    for( ; it != it_end; )
    {
        KeyPoint kpt;
        it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
        keypoints.push_back(kpt);
    }
}


void write(FileStorage& fs, const char* objname, const std::vector<DMatch>& matches)
{
    internal::WriteStructContext ws(fs, objname, CC_NODE_SEQ + CC_NODE_FLOW);

    int i, n = (int)matches.size();
    for( i = 0; i < n; i++ )
    {
        const DMatch& m = matches[i];
        write(fs, m.queryIdx);
        write(fs, m.trainIdx);
        write(fs, m.imgIdx);
        write(fs, m.distance);
    }
}

void read(const FileNode& node, std::vector<DMatch>& matches)
{
    matches.resize(0);
    FileNodeIterator it = node.begin(), it_end = node.end();
    for( ; it != it_end; )
    {
        DMatch m;
        it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
        matches.push_back(m);
    }
}


int FileNode::type() const { return !node ? NONE : (node->tag & TYPE_MASK); }
bool FileNode::isNamed() const { return !node ? false : (node->tag & NAMED) != 0; }

size_t FileNode::size() const
{
    int t = type();
    return t == MAP ? (size_t)((CvSet*)node->data.map)->active_count :
        t == SEQ ? (size_t)node->data.seq->total : (size_t)!isNone();
}

void read(const FileNode& node, int& value, int default_value)
{
    value = !node.node ? default_value :
    CC_NODE_IS_INT(node.node->tag) ? node.node->data.i :
    CC_NODE_IS_REAL(node.node->tag) ? cRound(node.node->data.f) : 0x7fffffff;
}

void read(const FileNode& node, float& value, float default_value)
{
    value = !node.node ? default_value :
        CC_NODE_IS_INT(node.node->tag) ? (float)node.node->data.i :
        CC_NODE_IS_REAL(node.node->tag) ? (float)node.node->data.f : 1e30f;
}

void read(const FileNode& node, double& value, double default_value)
{
    value = !node.node ? default_value :
        CC_NODE_IS_INT(node.node->tag) ? (double)node.node->data.i :
        CC_NODE_IS_REAL(node.node->tag) ? node.node->data.f : 1e300;
}

void read(const FileNode& node, char* value, const char* default_value)
{
    value = !node.node ? default_value : CC_NODE_IS_STRING(node.node->tag) ? String(node.node->data.str.ptr) : String();
}



/****************************************************************************
 * Newly added for Base64
 *
 *
 ***************************************************************************/


/****************************************************************************
 * constant
 ***************************************************************************/

#if CHAR_BIT != 8
#error "`char` should be 8 bit."
#endif

uint8_t const base64_mapping[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

uint8_t const base64_padding = '=';

uint8_t const base64_demapping[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0, 62,  0,  0,  0, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,  0,  0,
    0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,  0,  0,  0,  0,  0, 26, 27, 28,
    29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
    49, 50, 51,  0,  0,  0,  0,
};

/*    `base64_demapping` above is generated in this way:
 *    `````````````````````````````````````````````````````````````````````
 *  std::string mapping((const char *)base64_mapping);
 *    for (auto ch = 0; ch < 127; ch++) {
 *        auto i = mapping.find(ch);
 *        printf("%3u, ", (i != std::string::npos ? i : 0));
 *    }
 *    putchar('\n');
 *    `````````````````````````````````````````````````````````````````````
 */

/****************************************************************************
 * function
 ***************************************************************************/

size_t base64_encode(uint8_t const * src, uint8_t * dst, size_t off, size_t cnt)
{
    if (!src || !dst || !cnt)
        return 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t const * src_beg = src + off;
    uint8_t const * src_cur = src_beg;
    uint8_t const * src_end = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    while (src_cur < src_end) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        uint8_t _0 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_1 & 0xF0U) >> 4U | (_2 & 0x03U) << 4U];
        *dst_cur++ = base64_mapping[(_0 & 0xC0U) >> 6U | (_1 & 0x0FU) << 2U];
        *dst_cur++ = base64_mapping[ _0 & 0x3FU];
    }

    /* remainder part */
    size_t rst = src_beg + cnt - src_cur;
    if (rst == 1U) {
        uint8_t _2 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U];
    } else if (rst == 2U) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U | (_1 & 0xF0U) >> 4U];
        *dst_cur++ = base64_mapping[(_1 & 0x0FU) << 2U];
    }

    /* padding */
    switch (rst)
    {
    case 1U: *dst_cur++ = base64_padding;
    case 2U: *dst_cur++ = base64_padding;
    default: *dst_cur   = 0;
        break;
    }

    return static_cast<size_t>(dst_cur - dst_beg);
}

size_t base64_encode(char const * src, char * dst, size_t off, size_t cnt)
{
    if (cnt == 0U)
        cnt = std::strlen(src);

    return base64_encode
    (
        reinterpret_cast<uint8_t const *>(src),
        reinterpret_cast<uint8_t       *>(dst),
        off,
        cnt
    );
}

size_t base64_decode(uint8_t const * src, uint8_t * dst, size_t off, size_t cnt)
{
    /* check parameters */
    if (!src || !dst || !cnt)
        return 0U;
    if (cnt & 0x3U)
        return 0U;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t const * src_beg = src + off;
    uint8_t const * src_cur = src_beg;
    uint8_t const * src_end = src_cur + cnt;

    /* start decoding */
    while (src_cur < src_end) {
        uint8_t d50 = base64_demapping[*src_cur++];
        uint8_t c50 = base64_demapping[*src_cur++];
        uint8_t b50 = base64_demapping[*src_cur++];
        uint8_t a50 = base64_demapping[*src_cur++];

        uint8_t b10 = b50 & 0x03U;
        uint8_t b52 = b50 & 0x3CU;
        uint8_t c30 = c50 & 0x0FU;
        uint8_t c54 = c50 & 0x30U;

        *dst_cur++ = (d50 << 2U) | (c54 >> 4U);
        *dst_cur++ = (c30 << 4U) | (b52 >> 2U);
        *dst_cur++ = (b10 << 6U) | (a50 >> 0U);
    }

    *dst_cur = 0;
    return size_t(dst_cur - dst_beg);
}

size_t base64_decode(char const * src, char * dst, size_t off, size_t cnt)
{
    if (cnt == 0U)
        cnt = std::strlen(src);

    return base64_decode
    (
        reinterpret_cast<uint8_t const *>(src),
        reinterpret_cast<uint8_t       *>(dst),
        off,
        cnt
    );
}

bool base64_valid(uint8_t const * src, size_t off, size_t cnt)
{
    /* check parameters */
    if (src == 0 || src + off == 0)
        return false;
    if (cnt == 0U)
        cnt = std::strlen(reinterpret_cast<char const *>(src));
    if (cnt == 0U)
        return false;
    if (cnt & 0x3U)
        return false;

    /* initialize beginning and end */
    uint8_t const * beg = src + off;
    uint8_t const * end = beg + cnt;

    /* skip padding */
    if (*(end - 1U) == base64_padding) {
        end--;
        if (*(end - 1U) == base64_padding)
            end--;
    }

    /* find illegal characters */
    for (uint8_t const * iter = beg; iter < end; iter++)
        if (*iter > 126U || (!base64_demapping[(uint8_t)*iter] && *iter != base64_mapping[0]))
            return false;

    return true;
}

bool base64_valid(char const * src, size_t off, size_t cnt)
{
    if (cnt == 0U)
        cnt = std::strlen(src);

    return base64_valid(reinterpret_cast<uint8_t const *>(src), off, cnt);
}

size_t base64_encode_buffer_size(size_t cnt, bool is_end_with_zero)
{
    size_t additional = static_cast<size_t>(is_end_with_zero == true);
    return (cnt + 2U) / 3U * 4U + additional;
}

size_t base64_decode_buffer_size(size_t cnt, bool is_end_with_zero)
{
    size_t additional = static_cast<size_t>(is_end_with_zero == true);
    return cnt / 4U * 3U + additional;
}

size_t base64_decode_buffer_size(size_t cnt, char  const * src, bool is_end_with_zero)
{
    return base64_decode_buffer_size(cnt, reinterpret_cast<uchar const *>(src), is_end_with_zero);
}

size_t base64_decode_buffer_size(size_t cnt, uchar const * src, bool is_end_with_zero)
{
    size_t padding_cnt = 0U;
    for (uchar const * ptr = src + cnt - 1U; *ptr == base64_padding; ptr--)
        padding_cnt ++;
    return base64_decode_buffer_size(cnt, is_end_with_zero) - padding_cnt;
}

/****************************************************************************
 * to_binary && binary_to
 ***************************************************************************/

template<typename _uint_t> inline size_t 
to_binary(_uint_t val, uchar * cur)
{
    size_t delta = CHAR_BIT;
    size_t cnt = sizeof(_uint_t);
    while (cnt --> static_cast<size_t>(0U)) {
        *cur++ = static_cast<uchar>(val);
        val >>= delta;
    }
    return sizeof(_uint_t);
}

template<> inline size_t to_binary(double val, uchar * cur)
{
    suf64_t bit64;
    bit64.f = val;
    return to_binary(bit64.u, cur);
}

template<> inline size_t to_binary(float val, uchar * cur)
{
    suf32_t bit32;
    bit32.f = val;
    return to_binary(bit32.u, cur);
}

template<typename _primitive_t> inline size_t 
to_binary(uchar const * val, uchar * cur)
{
    return to_binary<_primitive_t>(*reinterpret_cast<_primitive_t const *>(val), cur);
}


template<typename _uint_t> inline size_t 
binary_to(uchar const * cur, _uint_t & val)
{
    val = static_cast<_uint_t>(0);
    for (size_t i = static_cast<size_t>(0U); i < sizeof(_uint_t); i++)
        val |= (static_cast<_uint_t>(*cur++) << (i * CHAR_BIT));
    return sizeof(_uint_t);
}

template<> inline size_t binary_to(uchar const * cur, double & val)
{
    suf64_t bit64;
    binary_to(cur, bit64.u);
    val = bit64.f;
    return sizeof(val);
}

template<> inline size_t binary_to(uchar const * cur, float & val)
{
    suf32_t bit32;
    binary_to(cur, bit32.u);
    val = bit32.f;
    return sizeof(val);
}

template<typename _primitive_t> inline size_t 
binary_to(uchar const * cur, uchar * val)
{
    return binary_to<_primitive_t>(cur, *reinterpret_cast<_primitive_t *>(val));
}

/****************************************************************************
 * others
 ***************************************************************************/

std::string make_base64_header(const char * dt)
{
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string buffer(oss.str());
    CC_Assert(buffer.size() < HEADER_SIZE);

    buffer.reserve(HEADER_SIZE);
    while (buffer.size() < HEADER_SIZE)
        buffer += ' ';

    return buffer;
}

bool read_base64_header(const char* header, std::string & dt)
{
    std::istringstream iss(header);
    return static_cast<bool>(iss >> dt);
}

/****************************************************************************
 * Parser
 ***************************************************************************/

Base64ContextParser::Base64ContextParser(uchar * buffer, size_t size)
    : dst_cur(buffer)
    , dst_end(buffer + size)
    , base64_buffer(BUFFER_LEN)
    , src_beg(0)
    , src_cur(0)
    , src_end(0)
    , binary_buffer(base64_encode_buffer_size(BUFFER_LEN))
{
    src_beg = binary_buffer.data();
    src_cur = src_beg;
    src_end = src_beg + BUFFER_LEN;
}

Base64ContextParser::~Base64ContextParser()
{
    /* encode the rest binary data to base64 buffer */
    if (src_cur != src_beg)
        flush();
}

Base64ContextParser & Base64ContextParser::
read(const uchar * beg, const uchar * end)
{
    if (beg >= end)
        return *this;

    while (beg < end) {
        /* collect binary data and copy to binary buffer */
        size_t len = MIN(end - beg, src_end - src_cur);
        std::memcpy(src_cur, beg, len);
        beg     += len;
        src_cur += len;

        if (src_cur >= src_end) {
            /* binary buffer is full. */
            /* decode it send result to dst */

            CC_Assert(flush());    /* check for base64_valid */
        }
    }

    return *this;
}

bool Base64ContextParser::flush()
{
    if ( !base64_valid(src_beg, 0U, src_cur - src_beg) )
        return false;

    if ( src_cur == src_beg )
        return true;

    uchar * buffer = binary_buffer.data();
    size_t len = base64_decode(src_beg, buffer, 0U, src_cur - src_beg);
    src_cur = src_beg;

    /* unexpected error */
    CC_Assert(len != 0);

    /* buffer is full */
    CC_Assert(dst_cur + len < dst_end);

    if (dst_cur + len < dst_end) {
        /* send data to dst */
        std::memcpy(dst_cur, buffer, len);
        dst_cur += len;
    }

    return true;
}

/****************************************************************************
 * Emitter
 ***************************************************************************/

/* A decorator for CFileStorage
 * - no copyable
 * - not safe for now
 * - move constructor may be needed if C++11
 */
class Base64ContextEmitter
{
public:
    explicit Base64ContextEmitter(CFileStorage * fs)
        : file_storage(fs)
        , binary_buffer(BUFFER_LEN)
        , base64_buffer(base64_encode_buffer_size(BUFFER_LEN))
        , src_beg(0)
        , src_cur(0)
        , src_end(0)
    {
        src_beg = binary_buffer.data();
        src_end = src_beg + BUFFER_LEN;
        src_cur = src_beg;

        CC_CHECK_OUTPUT_FILE_STORAGE(fs);

        if ( fs->fmt == CC_STORAGE_FORMAT_JSON )
        {
            /* clean and break buffer */
            *fs->buffer++ = '\0';
            ::icvPuts( fs, fs->buffer_start );
            fs->buffer = fs->buffer_start;
            memset( file_storage->buffer_start, 0, (int)(file_storage->space) );
            ::icvPuts( fs, "\"$base64$" );
        }
        else
        {
            ::icvFSFlush(file_storage);
        }
    }

    ~Base64ContextEmitter()
    {
        /* cleaning */
        if (src_cur != src_beg)
            flush();    /* encode the rest binary data to base64 buffer */

        if ( file_storage->fmt == CC_STORAGE_FORMAT_JSON )
        {
            /* clean and break buffer  */
            ::icvPuts(file_storage, "\"");
            file_storage->buffer = file_storage->buffer_start;
            ::icvFSFlush( file_storage );
            memset( file_storage->buffer_start, 0, (int)(file_storage->space) );
            file_storage->buffer = file_storage->buffer_start;
        }
    }

    Base64ContextEmitter & write(const uchar * beg, const uchar * end)
    {
        if (beg >= end)
            return *this;

        while (beg < end) {
            /* collect binary data and copy to binary buffer */
            size_t len = MIN(end - beg, src_end - src_cur);
           std::memcpy(src_cur, beg, len);
            beg     += len;
            src_cur += len;

            if (src_cur >= src_end) {
                /* binary buffer is full. */
                /* encode it to base64 and send result to fs */
                flush();
            }
        }

        return *this;
    }

    /*
     * a convertor must provide :
     * - `operator >> (uchar * & dst)` for writting current binary data to `dst` and moving to next data.
     * - `operator bool` for checking if current loaction is valid and not the end.
     */
    template<typename _to_binary_convertor_t> inline
    Base64ContextEmitter & write(_to_binary_convertor_t & convertor)
    {
        static const size_t BUFFER_MAX_LEN = 1024U;

        std::vector<uchar> buffer(BUFFER_MAX_LEN);
        uchar * beg = buffer.data();
        uchar * end = beg;

        while (convertor) {
            convertor >> end;
            write(beg, end);
            end = beg;
        }

        return *this;
    }

    bool flush()
    {
        /* controll line width, so on. */
        size_t len = base64_encode(src_beg, base64_buffer.data(), 0U, src_cur - src_beg);
        if (len == 0U)
            return false;

        src_cur = src_beg;
        {
            if ( file_storage->fmt == CC_STORAGE_FORMAT_JSON )
            {
                ::icvPuts(file_storage, (const char*)base64_buffer.data());
            }
            else
            {
                const char newline[] = "\n";
                char space[80];
                int ident = file_storage->struct_indent;
                memset(space, ' ', (int)(ident));
                space[ident] = '\0';

                ::icvPuts(file_storage, space);
                ::icvPuts(file_storage, (const char*)base64_buffer.data());
                ::icvPuts(file_storage, newline);
                ::icvFSFlush(file_storage);
            }

        }

        return true;
    }

private:
    /* because of Base64, we must keep its length a multiple of 3 */
    static const size_t BUFFER_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

private:
    CFileStorage * file_storage;

    std::vector<uchar> binary_buffer;
    std::vector<uchar> base64_buffer;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_end;
};


class RawDataToBinaryConvertor
{
public:

    RawDataToBinaryConvertor(const void* src, int len, const char* dt)
        : beg(reinterpret_cast<const uchar *>(src))
        , cur(0)
        , end(0)
    {
        CC_Assert(src);
        CC_Assert(dt);
        CC_Assert(len > 0);

        /* calc step and to_binary_funcs */
        make_to_binary_funcs(dt);

        end = beg;
        cur = beg;

        step = ::icvCalcStructSize(dt, 0);
        end = beg + step * static_cast<size_t>(len);
    }

    inline RawDataToBinaryConvertor & operator >>(uchar * & dst)
    {
        CC_DbgAssert(*this);

        for (size_t i = 0U, n = to_binary_funcs.size(); i < n; i++) {
            elem_to_binary_t & pack = to_binary_funcs[i];
            pack.func(cur + pack.offset, dst + pack.offset);
        }
        cur += step;
        dst += step;

        return *this;
    }

    inline operator bool() const
    {
        return cur < end;
    }

private:
    typedef size_t(*to_binary_t)(const uchar *, uchar *);
    struct elem_to_binary_t
    {
        size_t      offset;
        to_binary_t func;
    };

private:
    void make_to_binary_funcs(const char* dt)
    {
        size_t cnt = 0;
        size_t offset = 0;
        char type = '\0';

        std::istringstream iss(dt);
        while (!iss.eof()) {
            if (!(iss >> cnt)) {
                iss.clear();
                cnt = 1;
            }
            CC_Assert(cnt > 0U);
            if (!(iss >> type))
                break;

            while (cnt-- > 0)
            {
                elem_to_binary_t pack;

                size_t size = 0;
                switch (type)
                {
                case 'u':
                case 'c':
                    size = sizeof(uchar);
                    pack.func = to_binary<uchar>;
                    break;
                case 'w':
                case 's':
                    size = sizeof(ushort);
                    pack.func = to_binary<ushort>;
                    break;
                case 'i':
                    size = sizeof(uint);
                    pack.func = to_binary<uint>;
                    break;
                case 'f':
                    size = sizeof(float);
                    pack.func = to_binary<float>;
                    break;
                case 'd':
                    size = sizeof(double);
                    pack.func = to_binary<double>;
                    break;
                case 'r':
                default: { CC_Assert(!"type not support"); break; }
                };

                offset = static_cast<size_t>(cvAlign((int)(offset), (int)(size)));
                pack.offset = offset;
                offset += size;

                to_binary_funcs.push_back(pack);
            }
        }

        CC_Assert(iss.eof());
    }

private:
    const uchar * beg;
    const uchar * cur;
    const uchar * end;

    size_t step;
    std::vector<elem_to_binary_t> to_binary_funcs;
};

class BinaryToCvSeqConvertor
{
public:
    BinaryToCvSeqConvertor(const void* src, int len, const char* dt)
        : cur(reinterpret_cast<const uchar *>(src))
        , beg(reinterpret_cast<const uchar *>(src))
        , end(reinterpret_cast<const uchar *>(src))
    {
        CC_Assert(src);
        CC_Assert(dt);
        CC_Assert(len >= 0);

        /* calc binary_to_funcs */
        make_funcs(dt);
        functor_iter = binary_to_funcs.begin();

        step = ::icvCalcStructSize(dt, 0);
        end = beg + step * static_cast<size_t>(len);
    }

    inline BinaryToCvSeqConvertor & operator >> (CFileNode & dst)
    {
        CC_DbgAssert(*this);

        /* get current data */
        union
        {
            uchar mem[sizeof(double)];
            uchar  u;
            char   b;
            ushort w;
            short  s;
            int    i;
            float  f;
            double d;
        } buffer; /* for GCC -Wstrict-aliasing */
        std::memset(buffer.mem, 0, sizeof(buffer));
        functor_iter->func(cur + functor_iter->offset, buffer.mem);

        /* set node::data */
        switch (functor_iter->cc_type)
        {
        case CC_8U : { dst.data.i = saturate_cast<int>   (buffer.u); break;}
        case CC_8S : { dst.data.i = saturate_cast<int>   (buffer.b); break;}
        case CC_16U: { dst.data.i = saturate_cast<int>   (buffer.w); break;}
        case CC_16S: { dst.data.i = saturate_cast<int>   (buffer.s); break;}
        case CC_32S: { dst.data.i = saturate_cast<int>   (buffer.i); break;}
        case CC_32F: { dst.data.f = saturate_cast<double>(buffer.f); break;}
        case CC_64F: { dst.data.f = saturate_cast<double>(buffer.d); break;}
        default: break;
        }

        /* set node::tag */
        switch (functor_iter->cc_type)
        {
        case CC_8U :
        case CC_8S :
        case CC_16U:
        case CC_16S:
        case CC_32S: { dst.tag = CC_NODE_INT; /*std::printf("%i,", dst.data.i);*/ break; }
        case CC_32F:
        case CC_64F: { dst.tag = CC_NODE_REAL; /*std::printf("%.1f,", dst.data.f);*/ break; }
        default: break;
        }

        /* check if end */
        if (++functor_iter == binary_to_funcs.end()) {
            functor_iter = binary_to_funcs.begin();
            cur += step;
        }

        return *this;
    }

    inline operator bool() const
    {
        return cur < end;
    }

private:
    typedef size_t(*binary_to_t)(uchar const *, uchar *);
    struct binary_to_filenode_t
    {
        size_t      cc_type;
        size_t      offset;
        binary_to_t func;
    };

private:
    void make_funcs(const char* dt)
    {
        size_t cnt = 0;
        char type = '\0';
        size_t offset = 0;

        std::istringstream iss(dt);
        while (!iss.eof()) {
            if (!(iss >> cnt)) {
                iss.clear();
                cnt = 1;
            }
            CC_Assert(cnt > 0U);
            if (!(iss >> type))
                break;

            while (cnt-- > 0)
            {
                binary_to_filenode_t pack;

                /* set func and offset */
                size_t size = 0;
                switch (type)
                {
                case 'u':
                case 'c':
                    size      = sizeof(uchar);
                    pack.func = binary_to<uchar>;
                    break;
                case 'w':
                case 's':
                    size      = sizeof(ushort);
                    pack.func = binary_to<ushort>;
                    break;
                case 'i':
                    size      = sizeof(uint);
                    pack.func = binary_to<uint>;
                    break;
                case 'f':
                    size      = sizeof(float);
                    pack.func = binary_to<float>;
                    break;
                case 'd':
                    size      = sizeof(double);
                    pack.func = binary_to<double>;
                    break;
                case 'r':
                default:  { CC_Assert(!"type not support"); break; }
                }; // need a better way for outputting error.

                offset = static_cast<size_t>(cvAlign((int)(offset), (int)(size)));
                pack.offset = offset;
                offset += size;

                /* set type */
                switch (type)
                {
                case 'u': { pack.cc_type = CC_8U ; break; }
                case 'c': { pack.cc_type = CC_8S ; break; }
                case 'w': { pack.cc_type = CC_16U; break; }
                case 's': { pack.cc_type = CC_16S; break; }
                case 'i': { pack.cc_type = CC_32S; break; }
                case 'f': { pack.cc_type = CC_32F; break; }
                case 'd': { pack.cc_type = CC_64F; break; }
                case 'r':
                default:  { CC_Assert(!"type is not support"); break; }
                } // need a better way for outputting error.

                binary_to_funcs.push_back(pack);
            }
        }

        CC_Assert(iss.eof());
        CC_Assert(binary_to_funcs.size());
    }

private:

    const uchar * cur;
    const uchar * beg;
    const uchar * end;

    size_t step;
    std::vector<binary_to_filenode_t> binary_to_funcs;
    std::vector<binary_to_filenode_t>::iterator functor_iter;
};



/****************************************************************************
 * Wapper
 ***************************************************************************/


Base64Writer::Base64Writer(::CFileStorage * fs)
    : emitter(new Base64ContextEmitter(fs))
    , data_type_string()
{
    CC_CHECK_OUTPUT_FILE_STORAGE(fs);
}

void Base64Writer::write(const void* _data, size_t len, const char* dt)
{
    check_dt(dt);

    RawDataToBinaryConvertor convertor(
        _data, (int)(len), data_type_string
    );
    emitter->write(convertor);
}

template<typename _to_binary_convertor_t> inline
void Base64Writer::write(_to_binary_convertor_t & convertor, const char* dt)
{
    check_dt(dt);
    emitter->write(convertor);
}

Base64Writer::~Base64Writer()
{
    delete emitter;
}

void Base64Writer::check_dt(const char* dt)
{
    if ( dt == 0 )
        CC_Error( CC_StsBadArg, "Invalid \'dt\'." );
    else if (data_type_string.empty()) {
        data_type_string = dt;

        /* output header */
        std::string buffer = make_base64_header(dt);
        const uchar * beg = reinterpret_cast<const uchar *>(buffer.data());
        const uchar * end = beg + buffer.size();

        emitter->write(beg, end);
    } else if ( data_type_string != dt )
        CC_Error( CC_StsBadArg, "\'dt\' does not match." );
}


void make_seq(void * binary, int elem_cnt, const char * dt, ::CvSeq & seq)
{
    ::CFileNode node;
    node.info = 0;
    BinaryToCvSeqConvertor convertor(binary, elem_cnt, dt);
    while (convertor) {
        convertor >> node;
        cvSeqPush(&seq, &node);
    }
}

void cvWriteRawDataBase64(::CFileStorage* fs, const void* _data, int len, const char* dt)
{
    CC_Assert(fs);
    CC_CHECK_OUTPUT_FILE_STORAGE(fs);

    check_if_write_struct_is_delayed( fs, true );

    if ( fs->state_of_writing_base64 == Uncertain )
    {
        switch_to_Base64_state( fs, InUse );
    }
    else if ( fs->state_of_writing_base64 != InUse )
    {
        CC_Error( CC_StsError, "Base64 should not be used at present." );
    }

    fs->base64_writer->write(_data, len, dt);
}

/****************************************************************************
 * Interface
 ***************************************************************************/

CC_IMPL void cvWriteRawDataBase64(::CFileStorage* fs, const void* _data, int len, const char* dt)
{
    ::cvWriteRawDataBase64(fs, _data, len, dt);
}

#endif
