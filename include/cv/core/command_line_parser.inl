
typedef struct CommandLineParserParams
{
    str_t help_message[1];
    str_t keys[1];
    str_t value_type[1];
    str_t def_value[1];
} CommandLineParserParams;

#if 0

struct CommandLineParser
{
    bool error;
    char* error_message;
    char* about_message;
    char* path_to_app;
    char* app_name;
};


void CommandLineParser::getByName(const char* name, bool space_delete, int type, void* dst) const
{
    try
    {
        for (size_t i = 0; i < impl->data.size(); i++)
        {
            for (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                if (name == impl->data[i].keys[j])
                {
                    String v = impl->data[i].def_value;
                    if (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has it a default value
                    if((v.empty() && type != Param::STRING) || v == noneValue) {
                        impl->error = true;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        return;
                    }

                    from_str(v, type, dst);
                    return;
                }
            }
        }
    }
    catch (Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        return;
    }

    CC_Error_(CC_StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


void CommandLineParser::getByIndex(int index, bool space_delete, int type, void* dst) const
{
    try
    {
        for (size_t i = 0; i < impl->data.size(); i++)
        {
            if (impl->data[i].number == index)
            {
                String v = impl->data[i].def_value;
                if (space_delete == true) v = cat_string(v);

                // the key was neither specified nor has it a default value
                if((v.empty() && type != Param::STRING) || v == noneValue) {
                    impl->error = true;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", index);
                    return;
                }
                from_str(v, type, dst);
                return;
            }
        }
    }
    catch(Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + format("Parameter #%d: ", index) + e.err + "\n";
        return;
    }

    CC_Error_(CC_StsBadArg, ("undeclared position %d requested", index));
}

static bool cmp_params(const CommandLineParserParams & p1, const CommandLineParserParams & p2)
{
    if (p1.number < p2.number)
        return true;

    if (p1.number > p2.number)
        return false;

    return p1.keys[0].compare(p2.keys[0]) < 0;
}

CommandLineParser::~CommandLineParser()
{
    if (CC_XADD(&impl->refcount, -1) == 1)
        delete impl;
}

CommandLineParser::CommandLineParser(const CommandLineParser& parser)
{
    impl = parser.impl;
    CC_XADD(&impl->refcount, 1);
}

CommandLineParser& CommandLineParser::operator = (const CommandLineParser& parser)
{
    if( this != &parser )
    {
        if(CC_XADD(&impl->refcount, -1) == 1)
            delete impl;
        impl = parser.impl;
        CC_XADD(&impl->refcount, 1);
    }
    return *this;
}

void CommandLineParser::about(const char* message)
{
    impl->about_message = message;
}

void CommandLineParser::Impl::apply_params(const char* key, const char* value)
{
    for (size_t i = 0; i < data.size(); i++)
    {
        for (size_t k = 0; k < data[i].keys.size(); k++)
        {
            if (key.compare(data[i].keys[k]) == 0)
            {
                data[i].def_value = value;
                break;
            }
        }
    }
}

void CommandLineParser::Impl::apply_params(int i, String value)
{
    for (size_t j = 0; j < data.size(); j++)
    {
        if (data[j].number == i)
        {
            data[j].def_value = value;
            break;
        }
    }
}

void CommandLineParser::Impl::sort_params()
{
    for (size_t i = 0; i < data.size(); i++)
    {
        std::sort(data[i].keys.begin(), data[i].keys.end());
    }

    std::sort (data.begin(), data.end(), cmp_params);
}

String CommandLineParser::getPathToApplication() const
{
    return impl->path_to_app;
}

bool CommandLineParser::has(const char* name) const
{
    for (size_t i = 0; i < impl->data.size(); i++)
    {
        for (size_t j = 0; j < impl->data[i].keys.size(); j++)
        {
            if (name == impl->data[i].keys[j])
            {
                const String v = cat_string(impl->data[i].def_value);
                return !v.empty() && v != noneValue;
            }
        }
    }

    CC_Error_(CC_StsBadArg, ("undeclared key '%s' requested", name.c_str()));
    return false;
}

bool CommandLineParser::check() const
{
    return impl->error == false;
}

void CommandLineParser::printErrors() const
{
    if (impl->error)
    {
        printf("\nERRORS:\n%s\n", impl->error_message.c_str());
        fflush(stdout);
    }
}

void CommandLineParser::printMessage() const
{
    if (impl->about_message != "")
        printf("%s\n", impl->about_message.c_str());

    printf("Usage: %s [params] ", impl->app_name.c_str());

    for (size_t i = 0; i < impl->data.size(); i++)
    {
        if (impl->data[i].number > -1)
        {
            String name = impl->data[i].keys[0].substr(1, impl->data[i].keys[0].length() - 1);
            printf("%s ", name.c_str());
        }
    }

    printf("\n\n");

    for (size_t i = 0; i < impl->data.size(); i++)
    {
        if (impl->data[i].number == -1)
        {
            printf("\t");
            for (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                String k = impl->data[i].keys[j];
                if (k.length() > 1)
                {
                    printf("--");
                }
                else
                {
                    printf("-");
                }
                printf("%s", k.c_str());

                if (j != impl->data[i].keys.size() - 1)
                {
                    printf(", ");
                }
            }
            String dv = cat_string(impl->data[i].def_value);
            if (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
    printf("\n");

    for (size_t i = 0; i < impl->data.size(); i++)
    {
        if (impl->data[i].number != -1)
        {
            printf("\t");
            String k = impl->data[i].keys[0];
            k = k.substr(1, k.length() - 1);

            printf("%s", k.c_str());

            String dv = cat_string(impl->data[i].def_value);
            if (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
}

std::vector<String> CommandLineParser::Impl::split_string(const char* _str, char symbol, bool create_empty_item) const
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    while (!str.empty())
    {
        if (str[0] == symbol)
        {
            if (!word.empty() || create_empty_item)
            {
                vec.push_back(word);
                word = "";
            }
        }
        else
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    if (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    return vec;
}



#endif

static int split_range_string(const char* s, int i, int l, char fs, char ss, str_t* ps)
{
    ps->s = NULL;
    ps->l = 0;
    for (;i<l && s[i]!=fs;++i);
    if (i<l) {
      ps->s = (char*)s+i+1;
      for (;i<l && s[i]!=ss;++i);
      if (i<l) {
        ps->l = (s+i)-ps->s;
        ++i;
      }
    }

    // CC_ERROR(CC_StsParseError, "error in split_range_string");
    return i;
}

static CommandLineParserParams* CommandLineParserParams_find(CommandLineParserParams* pa, int n, const str_t* key) {
  int i;
  for (i=0; i<n; ++i) {
    int k = cstr_split_find(pa[i].keys->s, 0, pa[i].keys->l, " \t", " \t", key->s, key->l);
    if (k>=0) {
      return pa + i;
    }
  }
  return NULL;
}

static int cvCommandLineParserV(int argc, const char* const argv[], const char* keys, va_list ap)
{
  char path_to_app[256] = {0};
  char* app_name = GetFileNameExt(argv[0]);
  CommandLineParserParams pa[30] = {0};
  CommandLineParserParams* p;
  GetFilePathCopy(argv[0], path_to_app, countof(path_to_app));
  
  // parse keys
  int i=0, k=0;
  int l = strlen(keys);
  for (; i < l;) {
    p = pa + k;
    str_t ps[1] = {0};
    int j=0;
    i = split_range_string(keys, i, l, '{', '}', ps);
    cstr_split(ps->s, &j, ps->l, "|", " \t", 0, p->keys);
    if (j<ps->l) {
      cstr_split(ps->s, &j, ps->l, "|", " \t", 0, p->value_type);
      cstr_split(ps->s, &j, ps->l, "|", " \t", 0, p->def_value);
      cstr_split(ps->s, &j, ps->l, "|", " \t", 0, p->help_message);
      if (p->def_value->l==0) {
        *p->def_value = STR1("0");
      }
      ++k;
    }
  }
  for (i=1; i < argc;++i) {
    const char* s = argv[i];
    if (s[0] == '-') {
      ++s;
      l = strlen(s);
      if (l>0) {
        int j;
        str_t key[1] = {0};
        s += s[0] == '-';
        cstr_split(s, &j, l, "=", " \t", 0, key);
        p = CommandLineParserParams_find(pa, k, key);
        if (p) {
          if (j<l) {
            cstr_split(s, &j, l, "=", " \t", 0, p->def_value);
          } else if ((i+1)<argc && argv[i+1]) {
            *p->def_value = _cstrtrim_c(argv[i+1], -1, " \t");
          } else {
            *p->def_value = STR1("1");
          }
        }
      }
    }
  }
  
  for (i=0; i < k; ++i)
  {
    p = pa + i;
    switch(p->value_type->s[0]) {
    default:
    case 'h':
    case 'd':
      *va_arg(ap, int*) = atoi(p->def_value->s);
      break;
    case 'f':
      *va_arg(ap, double*) = atof(p->def_value->s);
      break;
    case 's':
      {
        char* pstr = va_arg(ap, char*);
        strncpy(pstr, p->def_value->s, p->def_value->l);
        pstr[p->def_value->l] = 0;
      }
      break;
    }
  }
  
  return k;
}

static int cvCommandLineParser(int argc, const char* const argv[], const char* keys, ...)
{
    int ret = 0;
    va_list arglist;
    va_start(arglist, keys);
    ret = cvCommandLineParserV(argc, argv, keys, arglist);
    va_end(arglist);
    return ret;
}