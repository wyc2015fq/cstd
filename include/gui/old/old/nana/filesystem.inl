

#if defined(NANA_WINDOWS)
    #include <windows.h>

    #if defined(NANA_MINGW)
        #ifndef _WIN32_IE
            #define _WIN32_IE 0x0500
        #endif
    #endif

	#include <shlobj.h>
#elif defined(NANA_POSIX)
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <dirent.h>
	#include <errno.h>
	#include <unistd.h>
	#include <stdlib.h>
#endif

enum
{
  file_none = 0,   ///< has not been determined or an error occurred while trying to determine
  file_not_found = -1, ///< Pseudo-type: file was not found. Is not considered an error
  file_regular = 1,
  file_directory = 2  ,
  file_symlink =3, ///< Symbolic link file
  file_block =4,  ///< Block special file
  file_character= 5 ,  ///< Character special file
  file_fifo = 6 ,  ///< FIFO or pipe file
  file_socket =7,
  file_unknown= 8  ///< The file does exist, but is of an operating system dependent type not covered by any of the other
};

enum 
{
  perms_none = 0,		///< There are no permissions set for the file.
  perms_all = 0x1FF,		///< owner_all | group_all | others_all 
  perms_mask = 0xFFF,		///< all | set_uid | set_gid | sticky_bit.
  perms_unknown = 0xFFFF	///<  not known, such as when a file_status object is created without specifying the permissions
};

#if defined(NANA_WINDOWS)
#define def_root  "C:"
#define def_rootstr = "C:\\"
#define def_rootname = "Local Drive(C:)"
#elif defined(NANA_LINUX)
#define def_root  "/"
#define def_rootstr  "/"
#define def_rootname  "Root/"
#endif

		char* path_user(char* path, int len)
		{
      path[0] = 0;
#if defined(NANA_WINDOWS)
			char pstr[MAX_PATH];
      if (SUCCEEDED(SHGetFolderPathA(0, CSIDL_PROFILE, 0, SHGFP_TYPE_CURRENT, pstr))) {
        strncpy(path, pstr, len);
				return path;
      }
#elif defined(NANA_LINUX) || defined(NANA_MACOS)
			const char * pstr = ::getenv("HOME");
      if (pstr) {
        strncpy(path, pstr, len);
				return path;
      }
#endif
			return path;
		}
    
    uint64 file_size(const char* p)
    {
#if defined(NANA_WINDOWS)
      //Some compilation environment may fail to link to GetFileSizeEx
      typedef BOOL(__stdcall *GetFileSizeEx_fptr_t)(HANDLE, PLARGE_INTEGER);
      GetFileSizeEx_fptr_t get_file_size_ex = (GetFileSizeEx_fptr_t)(::GetProcAddress(::GetModuleHandleA("Kernel32.DLL"), "GetFileSizeEx"));
      if (get_file_size_ex)
      {
        HANDLE handle = ::CreateFileA(p, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
        if (INVALID_HANDLE_VALUE != handle)
        {
          LARGE_INTEGER li;
          if (!get_file_size_ex(handle, &li))
            li.QuadPart = 0;
          
          ::CloseHandle(handle);
          return li.QuadPart;
        }
      }
      return 0;
#elif defined(NANA_POSIX)
      FILE * stream = ::fopen(p, "rb");
      long long size = 0;
      if (stream)
      {
#	if defined(NANA_LINUX)
        fseeko64(stream, 0, SEEK_END);
        size = ftello64(stream);
#	elif defined(NANA_MACOS)
        fseeko(stream, 0, SEEK_END);
        size = ftello(stream);
#	endif
        ::fclose(stream);
      }
      return size;
#endif
    }


		char* pretty_file_size(const char* path, char* str, int len)
		{
				uint64 bytes = file_size(path);
				const char * ustr[] = { "KB", "MB", "GB", "TB" };
        if (bytes < 1024) {
					_snprintf(str, len, "%d Bytes", bytes);
        } else {
					double cap = bytes / 1024.0;
					size_t uid = 0;
					while ((cap >= 1024.0) && (uid < sizeof(ustr) / sizeof(char *)))
					{
						cap /= 1024.0;
						++uid;
          }
					_snprintf(str, len, "%3.2f %s", cap, ustr[uid]);
				}
				return str;
    }
    
		bool modified_file_time(const char* p, struct tm* t)
		{
#if defined(NANA_WINDOWS)
			WIN32_FILE_ATTRIBUTE_DATA attr;
			if (::GetFileAttributesExA(p, GetFileExInfoStandard, &attr))
			{
				FILETIME local_file_time;
				if (::FileTimeToLocalFileTime(&attr.ftLastWriteTime, &local_file_time))
				{
					SYSTEMTIME st;
					::FileTimeToSystemTime(&local_file_time, &st);
					t->tm_year = st.wYear - 1900;
					t->tm_mon = st.wMonth - 1;
					t->tm_mday = st.wDay;
					t->tm_wday = st.wDayOfWeek - 1;
					t->tm_yday = day_in_year(st.wYear, st.wMonth, st.wDay);

					t->tm_hour = st.wHour;
					t->tm_min = st.wMinute;
					t->tm_sec = st.wSecond;
					return true;
				}
			}
#elif defined(NANA_POSIX)
			struct stat attr;
			if (0 == ::stat(p, &attr))
			{
				t = *(::localtime(&attr.st_ctime));
				return true;
			}
#endif
			return false;
		}

		//Because of No wide character version of POSIX
#if defined(NANA_LINUX) || defined(NANA_MACOS)
		const char* splstr = "/";
#else
		const char* splstr = "/\\";
#endif


		/// true if the path is empty, false otherwise. ??
		bool path_empty(const char* pathstr)
		{
#if defined(NANA_WINDOWS)
			return (::GetFileAttributesA(pathstr) == INVALID_FILE_ATTRIBUTES);
#elif defined(NANA_LINUX) || defined(NANA_MACOS)
			struct stat sta;
			return (::stat(pathstr, &sta) == -1);
#endif
		}

		char* path_extension(const char* pathstr)
		{
      int pathstr_len = cstr_len(pathstr);
			// todo: make more globlal
#if defined(NANA_WINDOWS)
      const char* SLorP="\\/.";
			char P='.';
#else
			const char* SLorP="\\/.";
			char P='.';
#endif
			int pos = cstr_rfindchrs(pathstr, -1, SLorP, -1, 0);

      if (    ( pos<0) || ( pathstr[pos] != P ) || ( pos + 1 == pathstr_len  ) ) {
        pos = pathstr_len;
      }

			return (char*)pathstr + pos;
		}
    
    int parent_path_len(const char* path, int len)
    {
      int index = len;
      
      if (index) {
        const char* str = path;
        
        for (--index; index > 0; --index)
        {
          char c = str[index];
          if (c != '\\' && c != '/')
            break;
        }
        
        for (--index; index > 0; --index)
        {
          char c = str[index];
          if (c == '\\' || c == '/')
            break;
        }
      }
      
      return index;
    }

		int path_what(const char* pathstr)
		{
#if defined(NANA_WINDOWS)
			unsigned long attr = ::GetFileAttributesA(pathstr);
      if (INVALID_FILE_ATTRIBUTES == attr) {
				return file_not_found; //??
      }

      if (FILE_ATTRIBUTE_DIRECTORY & attr) {
				return file_directory;
      }

			return file_regular;
#elif defined(NANA_LINUX) || defined(NANA_MACOS)
			struct stat sta;
      if (-1 == ::stat(pathstr, &sta)) {
				return file_not_found; //??
      }

      if ((S_IFDIR & sta.st_mode) == S_IFDIR) {
				return file_directory;
      }

      if ((S_IFREG & sta.st_mode) == S_IFREG) {
				return file_regular;
      }

			return file_none;
#endif
		}
    
#if defined(NANA_WINDOWS)
#define preferred_separator  '\\'
#else
#define preferred_separator  '/'
#endif
		char* filename(const char* pathstr)
		{
      int pathstr_len = strlen(pathstr);
			int pos = cstr_rfindchrs(pathstr, pathstr_len, splstr, -1, 0);
			if (pos>=0)
			{
				if (pos + 1 == pathstr_len)
				{
          static char tmp[2] = {0};
          tmp[0] = preferred_separator;

          if (pathstr_len != cstr_rfindchrs(pathstr, pathstr_len, splstr, pos, 1)) {
						tmp[0] = '.';
          }

					return tmp;
				}
				return (char*)pathstr + pos + 1;
			}

			return (char*)pathstr;
		}

		char* remove_filename(char* pathstr)
    {
      int pathstr_len = strlen(pathstr);
#ifdef NANA_WINDOWS
			char seps[] = "/\\";
#else
			char seps[] = "/\\";
#endif
			int pos = cstr_rfindchrs(pathstr, pathstr_len, seps, -1, 0);
			if (pos >= 0) {
				pos = cstr_rfindchrs(pathstr, pathstr_len, seps, pos, 1);
				if (pos >= 0)
				{
#ifdef NANA_WINDOWS
					if (pathstr[pos] == ':')
					{
            ++pos;
            pathstr[pos++] = '\\';
						pathstr[pos++] = 0;
						return (char*)pathstr;
					}
#endif
					++pos;
				}
        else {
					pos = 0;
        }
			}
      else {
				pos = 0;
      }

      pathstr[pos] = 0;
			return (char*)pathstr;
		}

		char* path_generic_string(char* pathstr)
		{
			char* str = pathstr;
      for (;*str;++str) {
        if (*str = '\\') {
          *str = '/';
        }
      }
			return pathstr;
		}
		char* path_add(char* pathstr, int maxlen, const char* p)
    {
      int pathstr_len;
			if (NULL==p || 0==*p)
				return pathstr;
      
      pathstr_len = strlen(pathstr);
			if (0==cstr_cmp(pathstr, pathstr_len, p, -1))
			{
				const char* other = p;
				if ((other[0] != '/') && (other[0] == preferred_separator))
				{
          if (!(*pathstr) && (pathstr[pathstr_len-1] != '/' && pathstr[pathstr_len-1] != preferred_separator)) {
						pathstr_len = cstr_cat_c(pathstr, pathstr_len, maxlen, preferred_separator);
          }
				}

				pathstr_len = cstr_cat(pathstr, pathstr_len, maxlen, other, -1);
      } else {
				const char* other = p;
				if ((other[0] != '/') && (other[0] == preferred_separator)) {
          if (!(*pathstr) && (pathstr[pathstr_len-1] != '/' && pathstr[pathstr_len-1] != preferred_separator)) {
						pathstr_len = cstr_cat_c(pathstr, pathstr_len, maxlen, preferred_separator);
          }
				}
        
				pathstr_len = cstr_cat(pathstr, pathstr_len, maxlen, p, -1);
			}
			return pathstr;
		}

		bool not_found_error(int errval)
		{
#if defined(NANA_WINDOWS)
			switch (errval)
			{
			case ERROR_FILE_NOT_FOUND:
			case ERROR_PATH_NOT_FOUND:
			case ERROR_INVALID_NAME:
			case ERROR_INVALID_DRIVE:
			case ERROR_NOT_READY:
			case ERROR_INVALID_PARAMETER:
			case ERROR_BAD_PATHNAME:
			case ERROR_BAD_NETPATH:
				return true;
			}
			return false;
#elif defined(NANA_POSIX)
			return (errval == ENOENT || errval == ENOTDIR);
#else
			static_assert(false, "Only Windows and Unix are supported now (Mac OS is experimental)");
#endif
		}
    
    int file_status(const char* p)
    {
#if defined(NANA_WINDOWS)
      DWORD attr = ::GetFileAttributesA(p);
      if (INVALID_FILE_ATTRIBUTES == attr)
      {
        if (not_found_error(::GetLastError()))
          return file_not_found;
        return file_unknown;
      }
      return (FILE_ATTRIBUTE_DIRECTORY & attr) ? file_directory : file_regular;
#elif defined(NANA_POSIX)
      struct stat path_stat;
      if (0 != ::stat(p, &path_stat))
      {
        if (errno == ENOENT || errno == ENOTDIR)
          return file_not_found;
        
        return file_unknown;
      }
      
      auto prms = static_cast<perms>(path_stat.st_mode & static_cast<unsigned>(perms::mask));
      
      if (S_ISREG(path_stat.st_mode))
        return file_regular;
      
      if (S_ISDIR(path_stat.st_mode))
        return file_directory;
      
      if (S_ISLNK(path_stat.st_mode))
        return file_symlink;
      
      if (S_ISBLK(path_stat.st_mode))
        return file_block;
      
      if (S_ISCHR(path_stat.st_mode))
        return file_character;
      
      if (S_ISFIFO(path_stat.st_mode))
        return file_fifo;
      
      if (S_ISSOCK(path_stat.st_mode))
        return file_socket;
      
      return file_unknown };
#endif		
    }
    
    bool file_is_directory(const char* p)
    {
      return (file_status(p) == file_directory);
    }

			bool rm_file(const char* p)
			{
				if (NULL==p || 0==*p)
					return false;
#if defined(NANA_WINDOWS)	
				return (FALSE != ::DeleteFileA(p));
#elif defined(NANA_POSIX)
				return (!remove(p));
#endif			
			}

			bool rm_dir(const char* p)
			{
#if defined(NANA_WINDOWS)
				return (FALSE != ::RemoveDirectoryA(p)) || not_found_error(::GetLastError());
#elif defined(NANA_POSIX)
				return (!::rmdir(p)) || not_found_error(errno);
#endif			
			}
      
      bool rm_empty_dir(const char* p) {
#if defined(NANA_WINDOWS)
        if (FALSE != ::RemoveDirectoryA(p))
          return true;
        return false;
#elif defined(NANA_POSIX)
        if (::rmdir(p)) {
          return false;
        }
        return true;
#endif
      }

			//rm_dir_recursive
			//@brief: remove a directory, if it is not empty, recursively remove it's subfiles and sub directories
			bool rm_dir(const char* p, BOOL is_del_sub)
      {
        if (NULL==p || 0==*p)
            return false;

        if (is_del_sub) {
          findinfo_t finfo[1] = {0};
          fileinfo_t info[1] = {0};
          for (; find_next_file(finfo, p, "*", info, FF_SUBDIR);) {
            if (S_ISDIR(info->attrib)) {
              rm_dir(info->name, is_del_sub);
            } else {
              rm_file(info->name);
            }
          }
        }

        return rm_empty_dir(p);
			}
      
      bool rm_file_or_dir(const char* p)
      {
        int stat = file_status(p);
        if (stat == file_directory) {
          return rm_dir(p, true);
        }
        return rm_file(p);
      }

#if defined(NANA_WINDOWS)
			void filetime_to_c_tm(FILETIME& ft, struct tm* t)
			{
				FILETIME local_file_time;
				if (::FileTimeToLocalFileTime(&ft, &local_file_time))
				{
					SYSTEMTIME st;
					::FileTimeToSystemTime(&local_file_time, &st);
					t->tm_year = st.wYear - 1900;
					t->tm_mon = st.wMonth - 1;
					t->tm_mday = st.wDay;
					t->tm_wday = st.wDayOfWeek - 1;
					t->tm_yday = day_in_year(st.wYear, st.wMonth, st.wDay);

					t->tm_hour = st.wHour;
					t->tm_min = st.wMinute;
					t->tm_sec = st.wSecond;
				}
			}
#endif

		time_t file_last_write_time(const char* p)
		{
			struct tm t;
			modified_file_time(p, &t);
			time_t dateTime = mktime(&t);
			return dateTime;
		}
    
    char* pretty_file_date(const char* path, char* str, int len) // todo: move to .cpp
    {
      struct tm t;
      modified_file_time(path, &t);
      strftime(str, len, "%Y-%m-%d, %H:%M:%S", &t);
      return str;
    }
    
			bool create_directory(const char* p)
			{
#if defined(NANA_WINDOWS)
				return (FALSE != ::CreateDirectoryA(p, 0));
#elif defined(NANA_POSIX)
				return 0 == ::mkdir(p);
#endif
			}

			char* current_path()
      {
        static char buf[1024] = "";
#if defined(NANA_WINDOWS)
				DWORD len = ::GetCurrentDirectoryA(1024, buf);
#elif defined(NANA_POSIX)
				char* pstr = ::getcwd(buf, 260);
        if (pstr) {
					return pstr;
        }
        buf[0] = 0;
#endif
				return buf;
			}

			void set_current_path(const char* p)
			{
#if defined(NANA_WINDOWS)
				::SetCurrentDirectoryA(p);
#elif defined(NANA_POSIX)
				::chdir(p);
#endif
			}


