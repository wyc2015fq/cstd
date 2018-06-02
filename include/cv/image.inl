
/* ////////////////////////////////////////////////////////////////////
//
//  C++ classes for image and matrices
//
// */

/////////////////////////////// CvImage implementation //////////////////////////////////

static CvLoadImageFunc load_image = 0;
static CvLoadImageMFunc load_image_m = 0;
static CvSaveImageFunc save_image = 0;
static CvShowImageFunc show_image = 0;

static bool
icvIsXmlOrYaml(const char* filename)
{
    const char* suffix = strrchr(filename, '.');
    return suffix &&
        strcmp(suffix, ".xml") == 0 ||
        strcmp(suffix, ".Xml") == 0 ||
        strcmp(suffix, ".XML") == 0 ||
        strcmp(suffix, ".yml") == 0 ||
        strcmp(suffix, ".Yml") == 0 ||
        strcmp(suffix, ".YML") == 0 ||
        strcmp(suffix, ".yaml") == 0 ||
        strcmp(suffix, ".Yaml") == 0 ||
        strcmp(suffix, ".YAML") == 0;
}


static img_t*
icvRetrieveImage(void* obj)
{
    img_t* img = 0;

    CC_FUNCNAME("icvRetrieveImage");

    __BEGIN__;

    if(CC_IS_IMAGE(obj))
        img = (img_t*)obj;
    else if(CC_IS_MAT(obj))
    {
        img_t* m = obj;
        CC_CALL(img = cvCreateImageHeader(cSize(m->cols,m->rows),
                        CC_MAT_TYPE(m->tid), CC_MAT_CN(m->tid)));
        cvSetData(img, m->tt.data, m->step);
        img->imageDataOrigin = (char*)m->refcount;
        m->tt.data = 0; m->step = 0;
        cvReleaseMat(&m);
    }
    else if(obj)
    {
        cvRelease(&obj);
        CC_ERROR(CC_StsUnsupportedFormat, "The object is neither an image, nor a matrix");
    }

    __END__;

    return img;
}


bool CvImage::load(const char* filename, const char* imgname, int color)
{
    img_t* img = 0;

    CC_FUNCNAME("CvImage::read");

    __BEGIN__;

    if(icvIsXmlOrYaml(filename))
    {
        img = icvRetrieveImage(cvLoad(filename,0,imgname));
        if((img->nChannels > 1) != (color == 0))
            CC_ERROR(CC_StsNotImplemented,
            "RGB<->Grayscale conversion is not implemented for images stored in XML/YAML");
        /*{
            img_t* temp_img = 0;
            CC_CALL(temp_img = cvCreateImage(cvGetSize(img), img->depth, color > 0 ? 3 : 1));
            cvCvtColor(img, temp_img, color > 0 ? CC_GRAY2BGR : CC_BGR2GRAY);
            cvReleaseImage(&img);
            img = temp_img;
        }*/
    }
    else
    {
        if(load_image)
            img = load_image(filename, color);
        else
            CC_ERROR(CC_StsNotImplemented,
            "Loading an image stored in such a format requires HigGUI.\n"
            "Link it to your program and call any function from it\n");
    }

    attach(img);

    __END__;

    return img != 0;
}


bool CvImage::read(CvFileStorage* fs, const char* mapname, const char* imgname)
{
    void* obj = 0;
    img_t* img = 0;

    if(mapname)
    {
        CvFileNode* mapnode = cvGetFileNodeByName(fs, 0, mapname);
        if(!mapnode)
            obj = cvReadByName(fs, mapnode, imgname);
    }
    else
        obj = cvReadByName(fs, 0, imgname);

    img = icvRetrieveImage(obj);
    attach(img);
    return img != 0;
}


bool CvImage::read(CvFileStorage* fs, const char* seqname, int idx)
{
    void* obj = 0;
    img_t* img = 0;
    CvFileNode* seqnode = seqname ?
        cvGetFileNodeByName(fs, 0, seqname) : cvGetRootFileNode(fs,0);

    if(seqnode && CC_NODE_IS_SEQ(seqnode->tag))
        obj = cvRead(fs, (CvFileNode*)cvGetSeqElem(seqnode->tt.seq, idx));
    img = icvRetrieveImage(obj);
    attach(img);
    return img != 0;
}


void CvImage::save(const char* filename, const char* imgname)
{
    CC_FUNCNAME("CvImage::write");
    __BEGIN__;

    if(!image)
        return;
    if(icvIsXmlOrYaml(filename))
        cvSave(filename, image, imgname);
    else
    {
        if(save_image)
            save_image(filename, image);
        else
            CC_ERROR(CC_StsNotImplemented,
            "Saving an image in such a format requires HigGUI.\n"
            "Link it to your program and call any function from it\n");
    }

    __END__;
}


void CvImage::write(CvFileStorage* fs, const char* imgname)
{
    if(image)
        cvWrite(fs, imgname, image);
}


void CvImage::show(const char* window_name)
{
    CC_FUNCNAME("CvMatrix::show");

    __BEGIN__;

    if(image)
    {
        if(!show_image)
            CC_ERROR(CC_StsNotImplemented,
            "CvImage::show method requires HighGUI.\n"
            "Link it to your program and call any function from it\n");
        show_image(window_name, image);
    }

    __END__;
}


/////////////////////////////// CvMatrix implementation //////////////////////////////////

CvMatrix::CvMatrix(int rows, int cols, int type, CvMemStorage* storage, bool alloc_data)
{
    if(storage)
    {
        matrix = cvMemStorageAlloc(storage, sizeof(*matrix));
        cvInitMatHeader(matrix, rows, cols, type, alloc_data ?
            cvMemStorageAlloc(storage, rows*cols*CC_ELEM_SIZE(type)) : 0);
    }
    else
        matrix = 0;
}

static img_t*
icvRetrieveMatrix(void* obj)
{
    img_t* m = 0;

    CC_FUNCNAME("icvRetrieveMatrix");

    __BEGIN__;

    if(CC_IS_MAT(obj))
        m = obj;
    else if(CC_IS_IMAGE(obj))
    {
        img_t* img = (img_t*)obj;
        img_t hdr, *src = cvGetMat(img, &hdr);
        CC_CALL(m = cvCreateMat(src->rows, src->cols, src->tid));
        CC_CALL(cvCopy(src, m));
        cvReleaseImage(&img);
    }
    else if(obj)
    {
        cvRelease(&obj);
        CC_ERROR(CC_StsUnsupportedFormat, "The object is neither an image, nor a matrix");
    }

    __END__;

    return m;
}


bool CvMatrix::load(const char* filename, const char* matname, int color)
{
    img_t* m = 0;

    CC_FUNCNAME("CvMatrix::read");

    __BEGIN__;

    if(icvIsXmlOrYaml(filename))
    {
        m = icvRetrieveMatrix(cvLoad(filename,0,matname));

        if((CC_MAT_CN(m->tid) > 1) != (color == 0))
            CC_ERROR(CC_StsNotImplemented,
            "RGB<->Grayscale conversion is not implemented for matrices stored in XML/YAML");
        /*{
            img_t* temp_mat;
            CC_CALL(temp_mat = cvCreateMat(m->rows, m->cols,
                CC_MAKETYPE(CC_MAT_TYPE(m->tid), color > 0 ? 3 : 1)));
            cvCvtColor(m, temp_mat, color > 0 ? CC_GRAY2BGR : CC_BGR2GRAY);
            cvReleaseMat(&m);
            m = temp_mat;
        }*/
    }
    else
    {
        if(load_image_m)
            m = load_image_m(filename, color);
        else
            CC_ERROR(CC_StsNotImplemented,
            "Loading an image stored in such a format requires HigGUI.\n"
            "Link it to your program and call any function from it\n");
    }

    set(m, false);

    __END__;

    return m != 0;
}


bool CvMatrix::read(CvFileStorage* fs, const char* mapname, const char* matname)
{
    void* obj = 0;
    img_t* m = 0;

    if(mapname)
    {
        CvFileNode* mapnode = cvGetFileNodeByName(fs, 0, mapname);
        if(!mapnode)
            obj = cvReadByName(fs, mapnode, matname);
    }
    else
        obj = cvReadByName(fs, 0, matname);

    m = icvRetrieveMatrix(obj);
    set(m, false);
    return m != 0;
}


bool CvMatrix::read(CvFileStorage* fs, const char* seqname, int idx)
{
    void* obj = 0;
    img_t* m = 0;
    CvFileNode* seqnode = seqname ?
        cvGetFileNodeByName(fs, 0, seqname) : cvGetRootFileNode(fs,0);

    if(seqnode && CC_NODE_IS_SEQ(seqnode->tag))
        obj = cvRead(fs, (CvFileNode*)cvGetSeqElem(seqnode->tt.seq, idx));
    m = icvRetrieveMatrix(obj);
    set(m, false);
    return m != 0;
}


void CvMatrix::save(const char* filename, const char* matname)
{
    CC_FUNCNAME("CvMatrix::write");
    __BEGIN__;

    if(!matrix)
        return;
    if(icvIsXmlOrYaml(filename))
        cvSave(filename, matrix, matname);
    else
    {
        if(save_image)
            save_image(filename, matrix);
        else
            CC_ERROR(CC_StsNotImplemented,
            "Saving a matrixe in such a format requires HigGUI.\n"
            "Link it to your program and call any function from it\n");
    }

    __END__;
}


void CvMatrix::write(CvFileStorage* fs, const char* matname)
{
    if(matrix)
        cvWrite(fs, matname, matrix);
}


void CvMatrix::show(const char* window_name)
{
    CC_FUNCNAME("CvMatrix::show");

    __BEGIN__;

    if(matrix)
    {
        if(!show_image)
            CC_ERROR(CC_StsNotImplemented,
            "CvMatrix::show method requires HighGUI.\n"
            "Link it to your program and call any function from it\n");
        show_image(window_name, matrix);
    }

    __END__;
}


CC_IMPL int
cvSetImageIOFunctions(CvLoadImageFunc _load_image, CvLoadImageMFunc _load_image_m,
                       CvSaveImageFunc _save_image, CvShowImageFunc _show_image)
{
    load_image = _load_image;
    load_image_m = _load_image_m;
    save_image = _save_image;
    show_image = _show_image;
    return 1;
}


/*void main(void)
{
    CvImage a(cSize(300,200),8,3), b(cSize(300,200),8,3);
    CRNG rng = cRNG(-1);

    CC_SET_IMAGE_IO_FUNCTIONS();

    cvNamedWindow("test", 1);
    //cvZero(a);
    cvZero(b);
    cvRandArr(&rng, a, CC_RAND_UNI, cScalarAll(0), cScalarAll(100));
    cvCircle(b, cPoint(100,100), 70, cScalar(0,255,0), -1, CC_AA, 0);
    cvAdd(a, b, a);
    a.show("test");

    WaitKey();
}*/



