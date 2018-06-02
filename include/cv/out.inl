
    enum { STATE_PROLOGUE, STATE_EPILOGUE, STATE_INTERLUDE,
           STATE_ROW_OPEN, STATE_ROW_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_VALUE, STATE_FINISHED,
           STATE_LINE_SEPARATOR, STATE_CN_SEPARATOR, STATE_VALUE_SEPARATOR };
    enum {BRACE_ROW_OPEN = 0, BRACE_ROW_CLOSE = 1, BRACE_ROW_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

struct CFormatted
{
    char floatFormat[8];
    char buf[32];   // enough for double with precision up to 20

    const img_t* mtx;
    int mcn; // == mtx CC_MAT_CN()
    bool singleLine;
    bool alignOrder;    // true when cn first order

    int state;
    int row;
    int col;
    int cn;

    int prec32f;
    int prec64f;
    int multiline;
    char prologue[256];
    char epilogue[256];
    char braces[5];
    int (*valueToStr)(struct CFormatted* s);
};


static int valueToStr8u(CFormatted* s)  { return snprintf(s->buf, 32, "%3d", (int)img_at(uchar, s->mtx, s->row, s->col)[s->cn]); }
static int valueToStr8s(CFormatted* s)  { return snprintf(s->buf, 32, "%3d", (int)img_at(schar, s->mtx, s->row, s->col)[s->cn]); }
static int valueToStr16u(CFormatted* s) { return snprintf(s->buf, 32, "%d", (int)img_at(ushort, s->mtx, s->row, s->col)[s->cn]); }
static int valueToStr16s(CFormatted* s) { return snprintf(s->buf, 32, "%d", (int)img_at(short, s->mtx, s->row, s->col)[s->cn]); }
static int valueToStr32s(CFormatted* s) { return snprintf(s->buf, 32, "%d", (int)img_at(int, s->mtx, s->row, s->col)[s->cn]); }
static int valueToStr32f(CFormatted* s) { return snprintf(s->buf, 32, s->floatFormat, img_at(float, s->mtx, s->row, s->col)[s->cn]); }
static int valueToStr64f(CFormatted* s) { return snprintf(s->buf, 32, s->floatFormat, img_at(double, s->mtx, s->row, s->col)[s->cn]); }
static int valueToStrOther(CFormatted* s) { return s->buf[0] = 0; }

static int Formatted_init(CFormatted* s, const char* pl, const char* el, const img_t* mtx, char br[5], bool aOrder)
{
    //CC_Assert(mtx.dims <= 2);
    s->prec32f = (6);
    s->prec64f = (8);
    s->multiline = (true);
    
    bool sLine = mtx->h == 1 || !s->multiline;
    int precision = mtx->tid == CC_64F ? s->prec64f : s->prec32f;

    strcpy(s->prologue, pl);
    strcpy(s->epilogue, el);
    s->mtx = mtx;
    s->mcn = CC_MAT_CN(mtx);
    memcpy(s->braces, br, 5);
    s->state = STATE_PROLOGUE;
    s->singleLine = sLine;
    s->alignOrder = aOrder;
    s->row = s->col = s->cn =0;

    if (precision < 0)
    {
        s->floatFormat[0] = '%';
        s->floatFormat[1] = 'a';
        s->floatFormat[2] = 0;
    }
    else
    {
        snprintf(s->floatFormat, countof(s->floatFormat), "%%%d.%dg", 10, MIN(precision, 20));
    }

    switch(mtx->tid)
    {
        case CC_8U:  s->valueToStr = valueToStr8u; break;
        case CC_8S:  s->valueToStr = valueToStr8s; break;
        case CC_16U: s->valueToStr = valueToStr16u; break;
        case CC_16S: s->valueToStr = valueToStr16s; break;
        case CC_32S: s->valueToStr = valueToStr32s; break;
        case CC_32F: s->valueToStr = valueToStr32f; break;
        case CC_64F: s->valueToStr = valueToStr64f; break;
        default:     s->valueToStr = valueToStrOther; break;
    }
    s->state = STATE_PROLOGUE;
    return 0;
}

static void Formatted_reset(CFormatted* s)
{
    s->state = STATE_PROLOGUE;
}

static const char* Formatted_next(CFormatted* s)
{
    switch(s->state)
    {
        case STATE_PROLOGUE:
            s->row = 0;
            if (imempty(s->mtx))
                s->state = STATE_EPILOGUE;
            else if (s->alignOrder)
                s->state = STATE_INTERLUDE;
            else
                s->state = STATE_ROW_OPEN;
            return s->prologue;
        case STATE_INTERLUDE:
            s->state = STATE_ROW_OPEN;
            if (s->row >= s->mtx->h)
            {
                if (++s->cn >= s->mcn)
                {
                    s->state = STATE_EPILOGUE;
                    s->buf[0] = 0;
                    return s->buf;
                }
                else
                    s->row = 0;
                sprintf(s->buf, "\n(:, :, %d) = \n", s->cn+1);
                return s->buf;
            }
            sprintf(s->buf, "(:, :, %d) = \n", s->cn+1);
            return s->buf;
        case STATE_EPILOGUE:
            s->state = STATE_FINISHED;
            return s->epilogue;
        case STATE_ROW_OPEN:
            s->col = 0;
            s->state = STATE_CN_OPEN;
            {
                size_t pos = 0;
                if (s->row > 0)
                    while(pos < strlen(s->prologue) && pos < sizeof(s->buf) - 2)
                        s->buf[pos++] = ' ';
                if (s->braces[BRACE_ROW_OPEN])
                  s->buf[pos++] = s->braces[BRACE_ROW_OPEN];
                if(!pos)
                    return Formatted_next(s);
                s->buf[pos] = 0;
            }
            return s->buf;
        case STATE_ROW_CLOSE:
            s->state = STATE_LINE_SEPARATOR;
            ++s->row;
            if (s->braces[BRACE_ROW_CLOSE])
            {
                s->buf[0] = s->braces[BRACE_ROW_CLOSE];
                s->buf[1] = s->row < s->mtx->h ? ',' : '\0';
                s->buf[2] = 0;
                return s->buf;
            }
            else if(s->braces[BRACE_ROW_SEP] && s->row < s->mtx->h)
            {
                s->buf[0] = s->braces[BRACE_ROW_SEP];
                s->buf[1] = 0;
                return s->buf;
            }
            return Formatted_next(s);
        case STATE_CN_OPEN:
            s->state = STATE_VALUE;
            if (!s->alignOrder)
                s->cn = 0;
            if (s->mcn > 1 && s->braces[BRACE_CN_OPEN])
            {
                s->buf[0] = s->braces[BRACE_CN_OPEN];
                s->buf[1] = 0;
                return s->buf;
            }
            return Formatted_next(s);
        case STATE_CN_CLOSE:
            ++s->col;
            if (s->col >= s->mtx->w)
                s->state = STATE_ROW_CLOSE;
            else
                s->state = STATE_CN_SEPARATOR;
            if (s->mcn > 1 && s->braces[BRACE_CN_CLOSE])
            {
                s->buf[0] = s->braces[BRACE_CN_CLOSE];
                s->buf[1] = 0;
                return s->buf;
            }
            return Formatted_next(s);
        case STATE_VALUE:
            s->valueToStr(s);
            s->state = STATE_CN_CLOSE;
            if (s->alignOrder)
                return s->buf;
            if (++s->cn < s->mcn)
                s->state = STATE_VALUE_SEPARATOR;
            return s->buf;
        case STATE_FINISHED:
            return 0;
        case STATE_LINE_SEPARATOR:
            if (s->row >= s->mtx->h)
            {
                if (s->alignOrder)
                    s->state = STATE_INTERLUDE;
                else
                    s->state = STATE_EPILOGUE;
                return Formatted_next(s);
            }
            s->state = STATE_ROW_OPEN;
            s->buf[0] = s->singleLine ? ' ' : '\n';
            s->buf[1] = 0;
            return s->buf;
        case STATE_CN_SEPARATOR:
            s->state = STATE_CN_OPEN;
            s->buf[0] = ',';
            s->buf[1] = ' ';
            s->buf[2] = 0;
            return s->buf;
        case STATE_VALUE_SEPARATOR:
            s->state = STATE_VALUE;
            s->buf[0] = ',';
            s->buf[1] = ' ';
            s->buf[2] = 0;
            return s->buf;
    }
    return 0;
}

static int DefaultFormatter_init(CFormatted* s, const img_t* mtx) {
    char braces[5] = {'\0', '\0', ';', '\0', '\0'};
    return Formatted_init(s, "[", "]", mtx, &*braces, false);
}
static int MatlabFormatter_init(CFormatted* s, const img_t* mtx) {
    char braces[5] = {'\0', '\0', ';', '\0', '\0'};
    return Formatted_init(s, "", "", mtx, &*braces, true);
}
static int PythonFormatter_init(CFormatted* s, const img_t* mtx) {
    char braces[5] = {'[', ']', ',', '[', ']'};
    if (mtx->w == 1)
        braces[0] = braces[1] = '\0';
    return Formatted_init(s, "[", "]", mtx, &*braces, false);
}
static int NumpyFormatter_init(CFormatted* s, const img_t* mtx) {
    static const char* numpyTypes[] =
    {
        "uint8", "int8", "uint16", "int16", "int32", "float32", "float64", "uint64"
    };
    char braces[5] = {'[', ']', ',', '[', ']'};
    if (mtx->w == 1)
        braces[0] = braces[1] = '\0';
    char el[256];
    snprintf(el, 256, "], dtype='%s')", numpyTypes[mtx->tid]);
    return Formatted_init(s, "array([", el, mtx, &*braces, false);
}
static int CSVFormatter_init(CFormatted* s, const img_t* mtx) {
    char braces[5] = {'\0', '\0', '\0', '\0', '\0'};
    return Formatted_init(s, "", mtx->h > 1 ? ("\n") : "", mtx, &*braces, false);
}
static int CFormatter_init(CFormatted* s, const img_t* mtx) {
    char braces[5] = {'\0', '\0', ',', '\0', '\0'};
    return Formatted_init(s, "{", "}", mtx, &*braces, false);
};

typedef enum {
  CC_OUT_FMT_DEFAULT = 0,
  CC_OUT_FMT_MATLAB  = 1,
  CC_OUT_FMT_CSV     = 2,
  CC_OUT_FMT_PYTHON  = 3,
  CC_OUT_FMT_NUMPY   = 4,
  CC_OUT_FMT_C       = 5
} CvOutFmt;
static int Formatter_init(CFormatted* s, const img_t* mtx, CvOutFmt fmt CC_DEFAULT(CC_OUT_FMT_DEFAULT))
{
    switch(fmt)
    {
        case CC_OUT_FMT_DEFAULT:
            return DefaultFormatter_init(s, mtx);
        case CC_OUT_FMT_MATLAB:
            return MatlabFormatter_init(s, mtx);
        case CC_OUT_FMT_CSV:
            return CSVFormatter_init(s, mtx);
        case CC_OUT_FMT_PYTHON:
            return PythonFormatter_init(s, mtx);
        case CC_OUT_FMT_NUMPY:
            return NumpyFormatter_init(s, mtx);
        case CC_OUT_FMT_C:
            return CFormatter_init(s, mtx);
    }
    return DefaultFormatter_init(s, mtx);
}
static FILE* cvOutMat(const img_t* mat, FILE* pf CC_DEFAULT(stdout), CvOutFmt fmt CC_DEFAULT(CC_OUT_FMT_DEFAULT)) {
    CFormatted ss[1] = {0};
    Formatter_init(ss, mat, fmt);
    const char* p = NULL;
    for (;p = Formatted_next(ss);) {
      fputs(p, pf);
    }
    fputs("\n", pf);
    return pf;
}
static void cvOutMatToFile(const img_t* mat, const char* name, const char* fn) {
  FILE* pf = fopen(fn, "wb");
  if (pf) {
    fprintf(pf, "%s[%d][%d][%d] = \n", name, mat->h, mat->w, CC_MAT_CN(mat));
    cvOutMat(mat, pf);
    fclose(pf);
  }
}
