#include "cstd.h"
#include "str.h"
#include "cfile.h"
int fmtcode_range(str_t* v, int n)
{
    int i, j, k, maxpos = 0, minlen = 10000, maxpos2 = 0;
    int pos[256];
    int pos1[256];
    int pos2[256];
    int len[256];
    char buf[256] = {0};
    memset(buf, ' ', 256);
    ASSERT(n < 256);
    for (i = 0; i < n; ++i)
    {
        char* s = v[i].s;
        pos[i] = v[i].l;
        len[i] = 0;
        pos1[i] = 0;
        pos2[i] = 0;
        for (j = 0; j < v[i].l - 1; ++j)
        {
            if (s[j] == '/' && s[j + 1] == '/')
            {
                pos[i] = j;
                for (; (k = pos[i] - 1 - len[i]) >= 0 && ' ' == s[k]; len[i]++);
                minlen = MIN(minlen, len[i]);
                pos2[i] = pos1[i] = pos[i] - len[i];
                for (; pos2[i] > 0 && ' ' != s[pos2[i] - 1]; pos2[i]--);
                maxpos2 = MAX(maxpos2, pos2[i]);
                maxpos = MAX(maxpos, pos[i] - pos2[i]);
                break;
            }
        }
    }
    for (i = 0; i < n; ++i)
    {
        j = pos[i] - len[i];
        k = maxpos - pos[i] + len[i] - minlen;
        if (pos2[i] < pos1[i])
        {
            if (1)
            {
                k = (maxpos2 + maxpos - pos[i]);
                str_ins(v + i, pos[i], 0, buf, maxpos - pos[i] + pos2[i]);
                str_ins(v + i, pos2[i], 0, buf, maxpos2 - pos2[i]);
            }
            else
            {
                str_ins(v + i, j, len[i], buf, 1);
                str_ins(v + i, pos2[i], 0, buf, k);
            }
        }
        else
        {
            str_ins(v + i, j, len[i], buf, k + 1);
        }
        //printf("%s\n", v[i].s);
    }
    return 0;
}
int fmtcode_all(str_t* v, int n)
{
    int i, j;
    int pre_hascode = 0, pre_hascom = 0;
    int beg = 0, end = 0;
    for (i = 0; i < n; ++i)
    {
        char* s = v[i].s;
        int hascode = 0, hascom = 0;
        int l = v[i].l;
        for (j = 0; j < l; ++j)
        {
            if (j < l - 1 && s[j] == '/' && s[j + 1] == '/')
            {
                hascom = 1;
            }
            if (s[j] != ' ' && s[j] != '\t')
            {
                hascode = 1;
            }
        }
        if (!pre_hascom && hascode && hascom)
        {
            end = beg = i;
        }
        if (pre_hascom && pre_hascode && !hascom)
        {
            end = i;
            if (end - beg > 1)
            {
                fmtcode_range(v + beg, end - beg);
            }
        }
        pre_hascom = hascom;
        pre_hascode = hascode;
    }
    return 0;
}
int fmtcode(char* code, str_t* out)
{
    vstr_t sv[1] = {0};
    int l = strlen(code);
    strv_split_str(sv, STR1(code), "\n", "\r\n", 0);
    fmtcode_all(sv->v, sv->n);
    out->l = 0;
    strv_merge(sv, out, "\r\n");
    strv_free(sv);
    return 0;
}
enum
{
    TOK_NULL,
    TOK_NUMBER,
    TOK_STRING,
    TOK_IDENTIFIER,
    TOK_COMMENT_LINE,
    TOK_COMMENT_BLOCK,
    TOK_KEYWORD, // return|break|if|else|switch|for|case|int|double|float|char|unsigned|
    TOK_OPERATOR, //
    TOK_OPERATOR_STRUCT, // . ->
    TOK_DEFINE, //
    TOK_PUNCTUATION, //
    TOK_BEG3, //{
    TOK_END3, //}
    TOK_BEG1, //{
    TOK_END1, //}
    TOK_BEG2, //{
    TOK_END2, //}
    TOK_SEMICOLON, //;
    TOK_COMMA, //,
};
typedef struct
{
    int beg;
    int end;
    int type;
} tok_t;
//#define TOK_INIT(_TOK, TYPE, BEG, END)  {((_TOK)->type = TYPE, (_TOK)->beg = BEG, (_TOK)->end = END); {char buf[256]={0};strncpy(buf, (char*)s+BEG, END-BEG); printf("%s\n", buf); }}
#define TOK_INIT(_TOK, TYPE, BEG, END)  {((_TOK)->type = TYPE, (_TOK)->beg = BEG, (_TOK)->end = END);}
int tok(const char* s, int i, int n, tok_t* t)
{
    {
    }
    return 0;
}
int code_parse(char* code, int n, tok_t* out, int nout)
{
    int i = 0, j = 0;
    static unsigned char typemap[256] = {0};
    uchar* s = (uchar*)code;
    tok_t* outbeg = out;
    tok_t* outend = out + nout;
    if (0 == typemap['*'])
    {
        char* s;
        int i;
        s = "+-*/:?<>=%!~^";
        for (i = 0; s[i]; ++i)
        {
            typemap[s[i]] = TOK_OPERATOR;
        }
        s = "{}[](),;.";
        for (i = 0; s[i]; ++i)
        {
            typemap[s[i]] = TOK_PUNCTUATION;
        }
        for (i = 'a'; i <= 'z'; ++i)
        {
            typemap[i] = TOK_IDENTIFIER;
        }
        for (i = 'A'; i <= 'Z'; ++i)
        {
            typemap[i] = TOK_IDENTIFIER;
        }
        for (i = '0'; i <= '9'; ++i)
        {
            typemap[i] = TOK_NUMBER;
        }
        typemap['\"'] = TOK_STRING;
        typemap['_'] = TOK_IDENTIFIER;
        typemap['{'] = TOK_BEG3;
        typemap['}'] = TOK_END3;
        typemap['('] = TOK_BEG1;
        typemap[')'] = TOK_END1;
        typemap['['] = TOK_BEG2;
        typemap[']'] = TOK_END2;
        typemap[';'] = TOK_SEMICOLON;
        typemap['#'] = TOK_DEFINE;
        typemap[','] = TOK_COMMA;
    }
    for (; i < n && out < outend;)
    {
        int t, beg;
        t = typemap[s[i++]];
        if (!t)
        {
            continue;
        }
        beg = i - 1;
        switch (t)
        {
        case TOK_STRING:
            for (; i < n && ('\"' != s[i] || '\\' == s[i - 1]); ++i);
            ++i;
            TOK_INIT(out, TOK_STRING, beg, i);
            break;
        case TOK_IDENTIFIER:
        case TOK_NUMBER:
            if (TOK_NUMBER == t || (i < n && ('-' == s[i] || '+' == s[i])))
            {
                for (; i < n && TOK_NUMBER == typemap[s[i]]; ++i);
                if ('.' == s[i] || 'x'==s[i] || 'X'==s[i])
                {
                    for (; i < n && TOK_NUMBER == typemap[s[i]]; ++i);
                }
                else
                {
                    for (; i < n && ('f' == s[i] || 'F' == s[i]); ++i);
                }
                TOK_INIT(out, TOK_NUMBER, beg, i);
            }
            else
            {
                for (; i < n && (t = typemap[s[i]]) && (TOK_IDENTIFIER == t || TOK_NUMBER == t); ++i);
                TOK_INIT(out, TOK_IDENTIFIER, beg, i);
            }
            break;
        case TOK_DEFINE:
            for (; i < n && ('\\' == s[i-1]) || ('\n' != s[i] && '\r' != s[i]); ++i);
            TOK_INIT(out, TOK_DEFINE, beg, i);
            break;
        case TOK_OPERATOR:
            if ('.'==s[i-1])
            {
                TOK_INIT(out, TOK_OPERATOR_STRUCT, beg, i);
            }
            else if (i<n && '-'==s[i-1]&&'>'==s[i])
            {
                ++i;
                TOK_INIT(out, TOK_OPERATOR_STRUCT, beg, i);
            }
            else if ('/' == s[i - 1] && ('/' == s[i] || '*' == s[i]))
            {
                if ('*' == s[i])
                {
                    for (; i < n && ('/' != s[i] || '*' != s[i - 1]); ++i);
                    TOK_INIT(out, TOK_COMMENT_BLOCK, beg, i);
                }
                else
                {
                    for (; i < n && ('\n' != s[i]); ++i);
                    TOK_INIT(out, TOK_COMMENT_LINE, beg, i);
                }
            }
            else
            {
                for (; i < n && TOK_OPERATOR == typemap[s[i]]; ++i);
                TOK_INIT(out, TOK_OPERATOR, beg, i);
            }
            break;
        default:
            TOK_INIT(out, t, beg, i);
            break;
        }
        ++out;
    }
    return out-outbeg;
}
int break_line(char* out, int j, int outlen, int deep3, int nspace)
{
    int n = j+deep3*nspace;
    ASSERT(n < outlen);
    for (; j<n; )
    {
        out[j++] = ' ';
    }
    return n;
}
#define NEWLINE()  (out[j++] = '\n', linepos = j)
#define INDENT()  (j = break_line(out, j, outlen, deep3, nspace))
int code_put(const char* src, const tok_t* tok, int ntok, char* out, int outlen)
{
    tok_t pre = {0};
    int i, j=0, deep3=0, len, deep1=0, linepos=0, indef = -1;
    int nspace = 4;
    for (i=0; i<ntok && j<outlen; ++i)
    {
        switch (tok[i].type)
        {
        case TOK_BEG1:
            ++deep1;
            break;
        case TOK_END1:
            --deep1;
            break;
        case TOK_BEG3:
            deep1=0;
            ++deep3;
            if (TOK_OPERATOR==pre.type)
            {
                indef = deep3;
            }
            else
            {
                if (1)
                {
                    NEWLINE();
                }
                else
                {
                    if (pre.type==TOK_END1)
                    {
                        out[j++] = ' ';
                    }
                }
            }
            break;
        case TOK_END3:
            deep1=0;
            --deep3;
            //ASSERT(deep3>=0);
            //NEWLINE();
            break;
        case TOK_DEFINE:
            break;
        case TOK_COMMENT_BLOCK:
            NEWLINE();
            break;
        case TOK_OPERATOR:
            if (TOK_END2==pre.type || TOK_IDENTIFIER==pre.type||TOK_NUMBER==pre.type)
            {
                out[j++] = ' ';
            }
            break;
        case TOK_IDENTIFIER:
        case TOK_NUMBER:
            if (TOK_IDENTIFIER==pre.type||TOK_NUMBER==pre.type)
            {
                out[j++] = ' ';
            }
            break;
        default:
            ASSERT(j<outlen);
            if (pre.type!=TOK_PUNCTUATION && tok[i].type!=TOK_PUNCTUATION)
            {
                //out[j++] = ' ';
            }
        }
        if (linepos==j)
        {
            switch (tok[i].type)
            {
            case TOK_BEG3:
                --deep3;
                INDENT();
                ++deep3;
                break;
            case TOK_DEFINE:
                break;
            default:
                INDENT();
                break;
            }
        }
        strncpy(out+j, src+tok[i].beg, len = (tok[i].end-tok[i].beg));
        j += len;
        switch (tok[i].type)
        {
        case TOK_COMMENT_BLOCK:
        case TOK_COMMENT_LINE:
            NEWLINE();
            break;
        case TOK_OPERATOR:
            if (TOK_END2==pre.type || TOK_IDENTIFIER==pre.type||TOK_NUMBER==pre.type)
            {
                out[j++] = ' ';
            }
            break;
        case TOK_COMMA:
            out[j++] = ' ';
            break;
        case TOK_DEFINE:
            NEWLINE();
            break;
        case TOK_SEMICOLON:
            if (0==deep1)
            {
                NEWLINE();
            }
            else
            {
                out[j++] = ' ';
            }
            break;
        case TOK_BEG3:
            if (indef!=deep3)
            {
                NEWLINE();
            }
            break;
        case TOK_END3:
            ASSERT(deep3>=0);
            if (indef!=(deep3+1))
            {
                NEWLINE();
                if (0==deep3)
                {
                    NEWLINE();
                }
            }
            else
            {
                indef = -1;
            }
            break;
        }
        pre = tok[i];
    }
    return j;
}
int test_fmtcode()
{
    str_t s[2] = {0};
    str_t* s1 = s + 1;
    if (1)
    {
        char* instr = sys_get_clipboard_text_static();
        fmtcode(instr, s);
        sys_set_clipboard_text(s->s, s->l);
    }
    if (0)
    {
        char* path;
        int i;
        dirlist_t dl[1] = {0};
        path = "E:/code/cstd/include/slgdev/slgdev";
        path = "E:/code/cstd/include/slgdev/SLGServer";
        path = "E:/code/cstd/include/slgdev";
        //_chdir(path);
        dirlist(dl, path, "inl|h|cpp|c|inl", FF_FULLNAME);
        for (i = 0; i < dl->n; ++i)
        {
            str_load(dl->v[i].name, s1);
            fmtcode(s1->s, s);
            str_save(dl->v[i].name, s);
        }
    }
    if (1)
    {
        char* path;
        int i;
        dirlist_t dl[1] = {0};
        path = "E:/code/cstd/include/slgdev/slgdev";
        path = "E:/code/cstd/include/slgdev/SLGServer";
        path = "E:/code/cstd/include/slgdev";
        tok_t* out = NULL;
        int nout = 0, kout = 0;
        //_chdir(path);
        dirlist(dl, path, "inl|h|cpp|c|inl", FF_FULLNAME);
        for (i = 0; i < dl->n; ++i)
        {
            str_load(dl->v[i].name, s1);
            str_setsize(s, s1->l*2+1000);
            nout = s1->l;
            MYREALLOC(out, nout);
            printf("%s\n", dl->v[i].name);
            kout = code_parse(s1->s, s1->l, out, nout);
            s->l = code_put(s1->s, out, kout, s->s, s->l);
            str_save("C:\\aaa.inl", s);
            str_save("C:\\aaa.inl", s);
        }
    }
    str_frees(s, countof(s));
    return 0;
}

