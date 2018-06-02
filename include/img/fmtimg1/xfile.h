#ifndef _XFILE_H_
#define _XFILE_H_
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN
#include <tchar.h>
#endif
#ifndef TRUE
#define TRUE (1)
#endif // TRUE
#ifndef FALSE
#define FALSE (0)
#endif // FALSE
struct XFILE;
typedef struct XFILE {
  struct XFILEFUNCTAB* vfptr;
  FILE* m_fp;
  
  unsigned char* m_pBuffer;
  unsigned long m_Size;
  int m_bFreeOnClose;
  long m_Position; //current position
  long m_Edge;  //buffer size
}
XFILE;
typedef int ( *close_t ) ( XFILE* stream );
typedef size_t ( *read_t ) ( void *buffer, size_t size, size_t count, XFILE* stream );
typedef size_t ( *write_t ) ( const void *buffer, size_t size, size_t count, XFILE* stream );
typedef int ( *seek_t ) ( XFILE* stream, long offset, int origin );
typedef long ( *tell_t ) ( XFILE* stream );
typedef int ( *flush_t ) ( XFILE* stream );
typedef int ( *eof_t ) ( XFILE* stream );
typedef long ( *error_t0 ) ( XFILE* stream );
typedef int ( *putc_t ) ( int c, XFILE* stream );
typedef long ( *getc_t ) ( XFILE* stream );
typedef char* ( *gets_t ) ( char *string, int n, XFILE* stream );
typedef long ( *scanf_t ) ( XFILE* stream, const char *format, void* output );
typedef struct XFILEFUNCTAB {
  int type;
  int ( *m_close ) ( XFILE* stream );
  size_t ( *m_read ) ( void *buffer, size_t size, size_t count, XFILE* stream );
  size_t ( *m_write ) ( const void *buffer, size_t size, size_t count, XFILE* stream );
  int ( *m_seek ) ( XFILE* stream, long offset, int origin );
  long ( *m_tell ) ( XFILE* stream );
  long ( *m_size ) ( XFILE* stream );
  int ( *m_flush ) ( XFILE* stream );
  int ( *m_eof ) ( XFILE* stream );
  long ( *m_error ) ( XFILE* stream );
  int ( *m_putc ) ( int c, XFILE* stream );
  long ( *m_getc ) ( XFILE* stream );
  char* ( *m_gets ) ( char *string, int n, XFILE* stream );
  long ( *m_scanf ) ( XFILE* stream, const char *format, void* output );
}
XFILEFUNCTAB;
#define xfread(buffer, size, count, stream) ((stream)->vfptr->m_read(buffer, size, count, (XFILE*)(stream)))
#define xfwrite(buffer, size, count, stream) ((stream)->vfptr->m_write(buffer, size, count, (XFILE*)(stream)))
#define xfseek(stream, offset, origin)    ((stream)->vfptr->m_seek((XFILE*)(stream), offset, origin))
#define xfclose(stream)   ((stream)->vfptr->m_close((XFILE*)(stream)))
#define xftell(stream)    ((stream)->vfptr->m_tell((XFILE*)(stream)))
#define xfsize(stream)    ((stream)->vfptr->m_size((XFILE*)(stream)))
#define xfflush(stream)   ((stream)->vfptr->m_flush((XFILE*)(stream)))
#define xfeof(stream)     ((stream)->vfptr->m_eof((XFILE*)(stream)))
#define xferror(stream)   ((stream)->vfptr->m_error((XFILE*)(stream)))
#define xfgetc(stream)    ((stream)->vfptr->m_getc((XFILE*)(stream)))
#define xfgets(_string, _n, stream)    ((stream)->vfptr->m_gets(_string, _n, (XFILE*)(stream)))
#define xfscanf(stream, _format, _output)   ((stream)->vfptr->m_scanf((XFILE*)(stream), _format, _output))
#define xfputc(c, stream) ((stream)->vfptr->m_putc(c, (XFILE*)(stream))) 
#define xftype(stream)   ((stream)->vfptr->type) 
//#define xfputc(c, stream)  ((stream)->vfptr->m_write(&c, 1, 1, stream) == 1)

typedef struct XFILE XFILE_IO;
typedef struct XFILE XFILE_MEM;
#define MALLOCX(_T, _p) (_p = (_T*)pmalloc(sizeof(_T)))
static int sfclose( XFILE* xf ) {
  pfree( xf );
  return 0;
}
static int xioclose( XFILE_IO* stream ) {
  int ierr = 0;
  ierr = fclose( stream->m_fp );
  stream->m_fp = NULL;
  pfree(stream);
  return ierr;
}
static size_t xioread( void *buffer, size_t size, size_t count, XFILE_IO* stream ) {
  return fread( buffer, size, count, stream->m_fp );
}
static size_t xiowrite( const void *buffer, size_t size, size_t count, XFILE_IO* stream ) {
  return fwrite( buffer, size, count, stream->m_fp );
}
static int xioseek( XFILE_IO* stream, long offset, int origin ) {
  return ( int ) ( fseek( stream->m_fp, offset, origin ) == 0 );
}
static long xiotell( XFILE_IO* stream ) {
  return ftell( stream->m_fp );
}
static long xiosize( XFILE_IO* stream ) {
  return fsize(stream->m_fp);
}
static int xioflush( XFILE_IO* stream ) {
  return ( int ) ( fflush( stream->m_fp ) == 0 );
}
static int xioeof( XFILE_IO* stream ) {
  return ( int ) ( feof( stream->m_fp ) != 0 );
}
static long xioerror( XFILE_IO* stream ) {
  return ferror( stream->m_fp );
}
static int xioputc( unsigned char c, XFILE_IO* stream ) {
  return fputc( c, stream->m_fp );
}
static long xiogetc( XFILE_IO* stream ) {
  return getc( stream->m_fp );
}
static char* xiogets( char *string, int n, XFILE_IO* stream ) {
  return fgets( string, n, stream->m_fp );
}
static long xioscanf( XFILE_IO* stream, const char *format, void* output ) {
  return fscanf( stream->m_fp, format, output );
}
static int memclose( XFILE_MEM* stream ) {
  if ( ( stream->m_pBuffer ) && ( stream->m_bFreeOnClose ) ) {
    pfree( stream->m_pBuffer );
    stream->m_pBuffer = NULL;
    stream->m_Size = 0;
    pfree(stream);
  }
  return TRUE;
}
static size_t memread( void *buffer, size_t size, size_t count, XFILE_MEM* stream ) {
  long nCount, nRead;
  if ( buffer == NULL )
    return 0;
  if ( stream->m_pBuffer == NULL )
    return 0;
  if ( stream->m_Position >= ( long ) stream->m_Size )
    return 0;
  nCount = ( long ) ( count * size );
  if ( nCount == 0 )
    return 0;
  if ( stream->m_Position + nCount > ( long ) stream->m_Size )
    nRead = ( stream->m_Size - stream->m_Position );
  else
    nRead = nCount;
  memcpy( buffer, stream->m_pBuffer + stream->m_Position, nRead );
  stream->m_Position += nRead;
  return ( size_t ) ( nRead / size );
}
static int memalloc( unsigned long dwNewLen, XFILE_MEM* stream ) {
  if ( dwNewLen > ( unsigned long ) stream->m_Edge ) {
    // find new buffer size
    unsigned long dwNewBufferSize = ( unsigned long ) ( ( ( dwNewLen >> 16 ) + 1 ) << 16 );
    // allocate new buffer
    if ( stream->m_pBuffer == NULL )
      stream->m_pBuffer = ( unsigned char* ) pmalloc( dwNewBufferSize );
    else
      stream->m_pBuffer = ( unsigned char* ) prealloc( stream->m_pBuffer, dwNewBufferSize );
    // I own this buffer now (caller knows nothing about it)
    stream->m_bFreeOnClose = TRUE;
    stream->m_Edge = dwNewBufferSize;
  }
  return ( stream->m_pBuffer != 0 );
}
static size_t memwrite( const void *buffer, size_t size, size_t count, XFILE_MEM* stream ) {
  long nCount;
  if ( stream->m_pBuffer == NULL )
    return 0;
  if ( buffer == NULL )
    return 0;
  nCount = ( long ) ( count * size );
  if ( nCount == 0 )
    return 0;
  if ( stream->m_Position + nCount > stream->m_Edge ) {
    if ( !memalloc( stream->m_Position + nCount, stream ) ) {
      return FALSE;
    }
  }
  memcpy( stream->m_pBuffer + stream->m_Position, buffer, nCount );
  stream->m_Position += nCount;
  if ( stream->m_Position > ( long ) stream->m_Size )
    stream->m_Size = stream->m_Position;
  return count;
}
static int memseek( XFILE_MEM* stream, long offset, int origin ) {
  long lNewPos;
  if ( stream->m_pBuffer == NULL )
    return FALSE;
  lNewPos = stream->m_Position;
  if ( origin == SEEK_SET )
    lNewPos = offset;
  else if ( origin == SEEK_CUR )
    lNewPos += offset;
  else if ( origin == SEEK_END )
    lNewPos = stream->m_Size + offset;
  else
    return FALSE;
  if ( lNewPos < 0 )
    lNewPos = 0;
  stream->m_Position = lNewPos;
  return TRUE;
}
static long memtell( XFILE_MEM* stream ) {
  return stream->m_Position;
}
static long memsize( XFILE_MEM* stream ) {
  return stream->m_Size;
}
static int memflush( XFILE_MEM* stream ) {
  return TRUE;
}
static int memeof( XFILE_MEM* stream ) {
  return ( stream->m_Position >= ( long ) stream->m_Size );
}
static long memerror( XFILE_MEM* stream ) {
  if ( stream->m_pBuffer == NULL )
    return -1;
  return ( stream->m_Position > ( long ) stream->m_Size );
}
static int memputc( unsigned char c, XFILE_MEM* stream ) {
  if ( stream->m_pBuffer == NULL )
    return FALSE;
  if ( stream->m_Position >= stream->m_Edge ) {
    if ( !memalloc( stream->m_Position + 1, stream ) ) {
      return FALSE;
    }
  }
  stream->m_pBuffer[ stream->m_Position++ ] = c;
  if ( stream->m_Position > ( long ) stream->m_Size )
    stream->m_Size = stream->m_Position;
  return TRUE;
}
static long memgetc( XFILE_MEM* stream ) {
  if ( memeof( stream ) )
    return EOF;
  return *( unsigned char* ) ( ( unsigned char* ) stream->m_pBuffer + stream->m_Position++ );
}
static char* memgets( char *string, int n, XFILE_MEM* stream ) {
  long c, i = 0;
  n--;
  while ( i < n ) {
    c = memgetc( stream );
    if ( c == EOF )
      return 0;
    string[ i++ ] = ( char ) c;
    if ( c == '\n' )
      break;
  }
  string[ i ] = 0;
  return string;
}
static long memscanf( const char *format, void* output ) {
  return 0;
}
enum { xft_xio, xft_mem };
#define XFILEFUNCTABDEF(_N) static XFILEFUNCTAB _N = { xft_##_N, (close_t)_N##close,(read_t)_N##read,(write_t)_N##write,(seek_t)_N##seek,(tell_t)_N##tell,(tell_t)_N##size,(flush_t)_N##flush,(eof_t)_N##eof,(error_t0)_N##error,(putc_t)_N##putc,(getc_t)_N##getc,(gets_t)_N##gets,(scanf_t)_N##scanf};
static XFILE* xfinit_file( XFILE_IO* stream, FILE* fp ) {
  XFILEFUNCTABDEF( xio );
  stream->m_fp = fp;
  stream->vfptr = &xio;
  return ( XFILE* ) stream;
}
static XFILE* xfopen_file( FILE* fp ) {
  XFILE_IO * stream;
  MALLOCX( XFILE_IO, stream );
  return xfinit_file( stream, fp );
}
#ifdef _WIN
static XFILE* xfopen( const TCHAR* filename, const TCHAR* mode ) {
  return xfopen_file( _tfopen( filename, mode ) );
}
#else
static XFILE* xfopen( const char* filename, const char* mode ) {
  return xfopen_file( fopen( filename, mode ) );
}
#endif
static XFILE* xfinit_mem( XFILE_MEM* stream, unsigned char* pBuffer, unsigned long size ) {
  XFILEFUNCTABDEF( mem );
  stream->m_pBuffer = pBuffer;
  stream->m_Position = 0;
  stream->m_Size = stream->m_Edge = size;
  stream->m_bFreeOnClose = ( pBuffer == 0 );
  stream->vfptr = &mem;
  return ( XFILE* ) stream;
}
static XFILE* xfopen_mem( unsigned char* pBuffer, unsigned long size ) {
  XFILE_MEM * stream;
  MALLOCX( XFILE_MEM, stream );
  return xfinit_mem( stream, pBuffer, size );
}
static long fload(FILE* fp, char* buf, long bufsize) {
  int sz = bufsize ? bufsize : fsize(fp);
  if (buf) {
    sz = fread(buf, 1, sz, fp);
  }
  return sz;
}
#endif // _XFILE_H_
