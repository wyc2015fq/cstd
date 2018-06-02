
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;


{
    static struct PlatformType
    {
        const ushort PROCESSOR_ARCHITECTURE_INTEL = 0;
        const ushort PROCESSOR_ARCHITECTURE_IA64 = 6;
        const ushort PROCESSOR_ARCHITECTURE_AMD64 = 9;
        const ushort PROCESSOR_ARCHITECTURE_UNKNOWN = 0xFFFF;

        [StructLayout(LayoutKind.Sequential)]
        struct SYSTEM_INFO
        {
            ushort wProcessorArchitecture;
            ushort wReserved;
            uint dwPageSize;
            IntPtr lpMinimumApplicationAddress;
            IntPtr lpMaximumApplicationAddress;
            UIntPtr dwActiveProcessorMask;
            uint dwNumberOfProcessors;
            uint dwProcessorType;
            uint dwAllocationGranularity;
            ushort wProcessorLevel;
            ushort wProcessorRevision;
        };

        [DllImport("kernel32.dll")]
        static extern void GetNativeSystemInfo(ref SYSTEM_INFO lpSystemInfo);

        [DllImport("kernel32.dll")]
        static extern void GetSystemInfo(ref SYSTEM_INFO lpSystemInfo);

        static Platform GetOperationSystemPlatform()
        {
            var sysInfo = new SYSTEM_INFO();

            // WinXP and older - use GetNativeSystemInfo
            if (Environment.OSVersion.Version.Major > 5 ||
                (Environment.OSVersion.Version.Major == 5 && Environment.OSVersion.Version.Minor >= 1))
            {
                GetNativeSystemInfo(ref sysInfo);
            }
            // else use GetSystemInfo
            else
            {
                GetSystemInfo(ref sysInfo);
            }

            switch (sysInfo.wProcessorArchitecture)
            {
                case PROCESSOR_ARCHITECTURE_IA64:
                case PROCESSOR_ARCHITECTURE_AMD64:
                    return Platform.X64;

                case PROCESSOR_ARCHITECTURE_INTEL:
                    return Platform.X86;

                default:
                    return Platform.Unknown;
            }
        }
    }

    enum Platform
    {
        X86,
        X64,
        Unknown
    }

}
