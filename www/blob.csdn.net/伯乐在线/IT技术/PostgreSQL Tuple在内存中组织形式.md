# PostgreSQL Tuple在内存中组织形式 - 文章 - 伯乐在线
/*
* Bit layouts for varlena headers on big-endian machines:
*
* 00xxxxxx 4-byte length word, aligned, uncompressed data (up to 1G)
* 01xxxxxx 4-byte length word, aligned, *compressed* data (up to 1G)
* 10000000 1-byte length word, unaligned, TOAST pointer
* 1xxxxxxx 1-byte length word, unaligned, uncompressed data (up to 126b)
*
* Bit layouts for varlena headers on little-endian machines:
*
* xxxxxx00 4-byte length word, aligned, uncompressed data (up to 1G)
* xxxxxx10 4-byte length word, aligned, *compressed* data (up to 1G)
* 00000001 1-byte length word, unaligned, TOAST pointer
* xxxxxxx1 1-byte length word, unaligned, uncompressed data (up to 126b)
*
* The “xxx” bits are the length field (which includes itself in all cases).
* In the big-endian case we mask to extract the length, in the little-endian
* case we shift. Note that in both cases the flag bits are in the physically
* first byte. Also, it is not possible for a 1-byte length word to be zero;
* this lets us disambiguate alignment padding bytes from the start of an
* unaligned datum. (We now *require* pad bytes to be filled with zero!)
*
* In TOAST pointers the va_tag field (see varattrib_1b_e) is used to discern
* the specific type and length of the pointer datum.
*/
