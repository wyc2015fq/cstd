
	int stoi(const char* str, int pos, int base, int* result)
	{
		const char* sptr = str + pos;
		char *end;
		errno = 0;
		*result = strtol(sptr, &end, base);

    if (sptr == end) {
			//throw invalid_argument("invalid stoi argument");
    }
    if (errno == ERANGE) {
			//throw out_of_range("stoi argument out of range");
    }
    
    pos = end - str;
		return pos;
	}
  
  int wtoi(const wchar_t* str, int pos, int base, int* result)
  {
    const wchar_t* sptr = str + pos;
    wchar_t *end;
    errno = 0;
    *result = wcstol(sptr, &end, base);
    
    if (sptr == end) {
      //throw invalid_argument("invalid stoi argument");
    }
    if (errno == ERANGE) {
      //throw out_of_range("stoi argument out of range");
    }
    
    pos = end - str;
    return pos;
  }
  
  int stol(const char* str, int pos, int base, long* result)
  {
    const char* sptr = str + pos;
    char *end;
    errno = 0;
    *result = strtol(sptr, &end, base);
    
    if (sptr == end) {
      //throw invalid_argument("invalid stoi argument");
    }
    if (errno == ERANGE) {
      //throw out_of_range("stoi argument out of range");
    }
    
    pos = end - str;
    return pos;
  }
  
  int wtol(const wchar_t* str, int pos, int base, long* result)
  {
    const wchar_t* sptr = str + pos;
    wchar_t *end;
    errno = 0;
    *result = wcstol(sptr, &end, base);
    
    if (sptr == end) {
      //throw invalid_argument("invalid stoi argument");
    }
    if (errno == ERANGE) {
      //throw out_of_range("stoi argument out of range");
    }
    
    pos = end - str;
    return pos;
  }
  
	int stoul(const char* str, int pos, int base, unsigned long* result)
	{
		const char* sptr = str + pos;
		char *end;
		errno = 0;
		*result = strtoul(sptr, &end, base);

    if (sptr == end) {
			//throw invalid_argument("invalid stoi argument");
    }
    if (errno == ERANGE) {
			//throw out_of_range("stoi argument out of range");
    }
    
    pos = end - str;
		return pos;
	}
  
  int wtoul(const wchar_t* str, int pos, int base, unsigned long* result)
  {
    const wchar_t* sptr = str + pos;
    wchar_t *end;
    errno = 0;
    *result = wcstoul(sptr, &end, base);
    
    if (sptr == end) {
      //throw invalid_argument("invalid stoi argument");
    }
    if (errno == ERANGE) {
      //throw out_of_range("stoi argument out of range");
    }
    
    pos = end - str;
    return pos;
  }
  
	int stof(const char* str, int pos, float* result)
  {
		const char* sptr = str + pos;
		errno = 0;
		char *end;
		*result = (float)strtod(sptr, &end);

    if (sptr == end) {
			//throw invalid_argument("invalid stod argument");
    }
    if (errno == ERANGE) {
			//throw out_of_range("stof argument out of range");
    }
    pos = end - str;
		return pos;
	}
  
	int wtof(const wchar_t* str, int pos, float* result)
  {
		const wchar_t* sptr = str + pos;
		errno = 0;
		wchar_t *end;
		*result = (float)wcstod(sptr, &end);

    if (sptr == end) {
			//throw invalid_argument("invalid stod argument");
    }
    if (errno == ERANGE) {
			//throw out_of_range("stof argument out of range");
    }
    pos = end - str;
		return pos;
	}
  
  int stod(const char* str, int pos, double* result)
  {
    const char* sptr = str + pos;
    errno = 0;
    char *end;
    *result = strtod(sptr, &end);
    
    if (sptr == end) {
      //throw invalid_argument("invalid stod argument");
    }
    if (errno == ERANGE) {
      //throw out_of_range("stof argument out of range");
    }
    pos = end - str;
    return pos;
  }
  
  int wtod(const wchar_t* str, int pos, double* result)
  {
    const wchar_t* sptr = str + pos;
    errno = 0;
    wchar_t *end;
    *result = wcstod(sptr, &end);
    
    if (sptr == end) {
      //throw invalid_argument("invalid stod argument");
    }
    if (errno == ERANGE) {
      //throw out_of_range("stof argument out of range");
    }
    pos = end - str;
    return pos;
  }
  