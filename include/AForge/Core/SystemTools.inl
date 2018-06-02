
/// <summary>
/// Set of systems tools.
/// </summary>
/// 
/// <remarks><para>The class is a container of different system tools, which are used
/// across the framework. Some of these tools are platform specific, so their
/// implementation is different on different platform, like .NET and Mono.</para>
/// </remarks>
/// 
/// <summary>
/// Copy block of unmanaged memory.
/// </summary>
/// 
/// <param name="dst">Destination pointer.</param>
/// <param name="src">Source pointer.</param>
/// <param name="count">Memory block's length to copy.</param>
/// 
/// <returns>Return's value of <paramref name="dst"/> - pointer to destination.</returns>
/// 
/// <remarks><para>This function is required because of the fact that .NET does
/// not provide any way to copy unmanaged blocks, but provides only methods to
/// copy from unmanaged memory to managed memory and vise versa.</para></remarks>
///

/// <summary>
/// Copy block of unmanaged memory.
/// </summary>
/// 
/// <param name="dst">Destination pointer.</param>
/// <param name="src">Source pointer.</param>
/// <param name="count">Memory block's length to copy.</param>
/// 
/// <returns>Return's value of <paramref name="dst"/> - pointer to destination.</returns>
/// 
/// <remarks><para>This function is required because of the fact that .NET does
/// not provide any way to copy unmanaged blocks, but provides only methods to
/// copy from unmanaged memory to managed memory and vise versa.</para></remarks>
/// 
static void* CopyUnmanagedMemory( void* dst, const void* src, int count )
{
#if !MONO
    return memcpy( dst, src, count );
#else
    int countUint = count >> 2;
    int countByte = count & 3;

    uint* dstUint = (uint*) dst;
    uint* srcUint = (uint*) src;

    while ( countUint-- != 0 )
    {
        *dstUint++ = *srcUint++;
    }

    byte* dstByte = (byte*) dstUint;
    byte* srcByte = (byte*) srcUint;

    while ( countByte-- != 0 )
    {
        *dstByte++ = *srcByte++;
    }
    return dst;
#endif
}

/// <summary>
/// Fill memory region with specified value.
/// </summary>
/// 
/// <param name="dst">Destination pointer.</param>
/// <param name="filler">Filler byte's value.</param>
/// <param name="count">Memory block's length to fill.</param>
/// 
/// <returns>Return's value of <paramref name="dst"/> - pointer to destination.</returns>
/// 

/// <summary>
/// Fill memory region with specified value.
/// </summary>
/// 
/// <param name="dst">Destination pointer.</param>
/// <param name="filler">Filler byte's value.</param>
/// <param name="count">Memory block's length to fill.</param>
/// 
/// <returns>Return's value of <paramref name="dst"/> - pointer to destination.</returns>
/// 
static void* SetUnmanagedMemory( void* dst, int filler, int count )
{
#if !MONO
    return memset( dst, filler, count );
#else
    int countUint = count >> 2;
    int countByte = count & 3;

    byte fillerByte = (byte) filler;
    uint fiilerUint = (uint) filler | ( (uint) filler << 8 ) |
                                      ( (uint) filler << 16 );// |
                                      //( (uint) filler << 24 );

    uint* dstUint = (uint*) dst;

    while ( countUint-- != 0 )
    {
        *dstUint++ = fiilerUint;
    }

    byte* dstByte = (byte*) dstUint;

    while ( countByte-- != 0 )
    {
        *dstByte++ = fillerByte;
    }
    return dst;
#endif
}



