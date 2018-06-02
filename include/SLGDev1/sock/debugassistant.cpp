
#include "slgdev/slgdev_private.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
enum DEBUG_TYPE
{
    DEBUG_CONSOLE = 1,
    DEBUG_STDOUT = 2,
    DEBUG_FILE = 3
};
int OutputInfo(const char* format , ...)
{

slgdev_config_t* g_cfg = get_config();
    if (!g_cfg->m_debug)
    {
        return 0;
    }

    class DebugAssistant
    {
        DEBUG_TYPE m_DebugMode;
        FILE* m_pFile;
        void* m_OutputHandle;
        CriticalSection m_SynLog;
    public:
        DebugAssistant(DEBUG_TYPE mode = DEBUG_STDOUT , const char* fileName = NULL) :
            m_DebugMode(mode),
            m_pFile(NULL),
            m_OutputHandle(NULL)
        {
            if (DEBUG_CONSOLE == m_DebugMode)
            {
#if defined WIN32 && !defined WINCE
                AllocConsole();
                m_OutputHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
#else
                m_OutputHandle = stdout;
#endif
            }

            if (NULL != fileName)
            {
                m_pFile = fopen(fileName , ("wb"));
            }

        }

        ~DebugAssistant()
        {
            if (DEBUG_CONSOLE == m_DebugMode)
            {
#if defined WIN32 && !defined WINCE

                if (m_OutputHandle)
                {
                    ::CloseHandle(m_OutputHandle);
                    m_OutputHandle = NULL;
                }

                FreeConsole();
#else
                m_OutputHandle = NULL;
#endif
            }

            if (m_pFile)
            {
                fclose(m_pFile);
            }

        }

    public:
        void OutputMsg(const char* msg)
        {
            int length = strlen(msg);
            AoutLock lock(&m_SynLog);

            switch (m_DebugMode)
            {
            case DEBUG_CONSOLE:
            {
#if defined WIN32 && !defined WINCE
                ULONG beWritten = 0;
                WriteConsole(m_OutputHandle , msg , length , &beWritten , NULL);
#else
                fprintf((FILE*)m_OutputHandle, "%s", msg);
#endif
            }

            break;

            case DEBUG_STDOUT:
                fprintf((FILE*)m_OutputHandle, "%s", msg);
                break;

            case DEBUG_FILE:
                if (m_pFile)
                {
                    fwrite(msg , sizeof(char) , length , m_pFile);
                    fwrite(("\n") , sizeof(char) , 1 , m_pFile);
                }

                break;

            default:
                break;
            }

        }

    };

    static DebugAssistant _DebugInfo(DEBUG_CONSOLE/*DEBUG_FILE , _T( "chsjcard.log" )*/);

    char msg[1024] = {0};

    va_list arg_ptr;

    va_start(arg_ptr, format);

    vsnprintf(msg , 1024, format, arg_ptr);

    strcat(msg , ("\n"));

    va_end(arg_ptr);

    _DebugInfo.OutputMsg(msg);

    return 0;
}

