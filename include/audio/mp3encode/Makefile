#     Makefile for 8hz-mp3 on UNIXOID systems. 
#     8hz 1998 
####################################################

                               
CC = gcc
SOURCES=main.c      \
	wave.c      \
	tables.c    \
	layer3.c    \
	l3psy.c     \
	l3subband.c \
	l3bitstream.c \
	formatBitstream.c \
	l3mdct.c    \
        l3loop.c    \
        reservoir.c \
        

OTHERC =ieeefloat.c  \
	portableio.c \
	fft.c        \
	bitstream.c  \
	huffman.c   

OBJ = $(SOURCES:.c=.o) $(OTHERC:.c=.o)

NINT_SWITCH = 
CC_SWITCHES = -O9 -Wall -DBS_FORMAT=BINARY $(NINT_SWITCH) -DNDEBUG
LIBS        = -lm 
PGM         = 8hz-mp3

all: $(PGM) todo

static:	$(OBJ)
	$(CC) -static -o $(PGM) $(OBJ) $(LIBS) 

$(PGM):	$(OBJ)
	$(CC) -o $(PGM) $(OBJ) $(LIBS)

%.o: %.c 
	$(CC) $(CC_SWITCHES) -c $< -o $@

todo:
	@echo "To do:"
	@echo " mpeg encoding ;> "

clean:
	rm -f $(OBJ) core $(PGM) *.bak

depend:
	@echo "[Updating dependencies]"
	@makedepend $(INCLUDEP) $(SOURCES) 2> /dev/null

# DO NOT DELETE THIS LINE -- make depend depends on it.

main.o: /usr/include/stdlib.h /usr/include/features.h
main.o: /usr/include/sys/cdefs.h /usr/include/errno.h
main.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
main.o: /usr/include/alloca.h /usr/include/stdio.h /usr/include/libio.h
main.o: /usr/include/_G_config.h /usr/include/string.h /usr/include/time.h
main.o: /usr/include/sys/time.h /usr/include/linux/types.h
main.o: /usr/include/linux/posix_types.h /usr/include/asm/posix_types.h
main.o: /usr/include/asm/types.h /usr/include/linux/time.h
main.o: /usr/include/sys/types.h /usr/include/sys/bitypes.h types.h
main.o: portableio.h ieeefloat.h /usr/include/math.h /usr/include/huge_val.h
main.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/nan.h
main.o: /usr/include/values.h /usr/include/ieee754.h /usr/include/ieee854.h
main.o: error.h wave.h layer3.h
wave.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
wave.o: portableio.h ieeefloat.h /usr/include/math.h /usr/include/features.h
wave.o: /usr/include/sys/cdefs.h /usr/include/huge_val.h
wave.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/nan.h
wave.o: /usr/include/values.h /usr/include/ieee754.h /usr/include/ieee854.h
wave.o: types.h /usr/include/time.h /usr/include/sys/time.h
wave.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
wave.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
wave.o: /usr/include/linux/time.h /usr/include/sys/types.h
wave.o: /usr/include/sys/bitypes.h error.h wave.h
tables.o: types.h /usr/include/stdio.h /usr/include/libio.h
tables.o: /usr/include/_G_config.h /usr/include/time.h
tables.o: /usr/include/features.h /usr/include/sys/cdefs.h
tables.o: /usr/include/sys/time.h /usr/include/linux/types.h
tables.o: /usr/include/linux/posix_types.h /usr/include/asm/posix_types.h
tables.o: /usr/include/asm/types.h /usr/include/linux/time.h
tables.o: /usr/include/sys/types.h /usr/include/sys/bitypes.h portableio.h
tables.o: ieeefloat.h /usr/include/math.h /usr/include/huge_val.h
tables.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/nan.h
tables.o: /usr/include/values.h /usr/include/ieee754.h /usr/include/ieee854.h
tables.o: tables.h
layer3.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
layer3.o: /usr/include/string.h /usr/include/features.h
layer3.o: /usr/include/sys/cdefs.h types.h /usr/include/time.h
layer3.o: /usr/include/sys/time.h /usr/include/linux/types.h
layer3.o: /usr/include/linux/posix_types.h /usr/include/asm/posix_types.h
layer3.o: /usr/include/asm/types.h /usr/include/linux/time.h
layer3.o: /usr/include/sys/types.h /usr/include/sys/bitypes.h portableio.h
layer3.o: ieeefloat.h /usr/include/math.h /usr/include/huge_val.h
layer3.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/nan.h
layer3.o: /usr/include/values.h /usr/include/ieee754.h /usr/include/ieee854.h
layer3.o: wave.h error.h layer3.h l3psy.h l3subband.h l3mdct.h l3loop.h
layer3.o: l3bitstream.h bitstream.h
l3psy.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
l3psy.o: types.h /usr/include/time.h /usr/include/features.h
l3psy.o: /usr/include/sys/cdefs.h /usr/include/sys/time.h
l3psy.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
l3psy.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
l3psy.o: /usr/include/linux/time.h /usr/include/sys/types.h
l3psy.o: /usr/include/sys/bitypes.h portableio.h ieeefloat.h
l3psy.o: /usr/include/math.h /usr/include/huge_val.h /usr/include/endian.h
l3psy.o: /usr/include/bytesex.h /usr/include/nan.h /usr/include/values.h
l3psy.o: /usr/include/ieee754.h /usr/include/ieee854.h error.h layer3.h
l3psy.o: l3psy.h fft.h tables.h psy_data.h
l3subband.o: /usr/include/math.h /usr/include/features.h
l3subband.o: /usr/include/sys/cdefs.h /usr/include/huge_val.h
l3subband.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/nan.h
l3subband.o: /usr/include/values.h /usr/include/ieee754.h
l3subband.o: /usr/include/ieee854.h types.h /usr/include/stdio.h
l3subband.o: /usr/include/libio.h /usr/include/_G_config.h
l3subband.o: /usr/include/time.h /usr/include/sys/time.h
l3subband.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
l3subband.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
l3subband.o: /usr/include/linux/time.h /usr/include/sys/types.h
l3subband.o: /usr/include/sys/bitypes.h portableio.h ieeefloat.h tables.h
l3subband.o: l3subband.h
l3bitstream.o: /usr/include/stdlib.h /usr/include/features.h
l3bitstream.o: /usr/include/sys/cdefs.h /usr/include/errno.h
l3bitstream.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
l3bitstream.o: /usr/include/alloca.h l3bitstream.h types.h
l3bitstream.o: /usr/include/stdio.h /usr/include/libio.h
l3bitstream.o: /usr/include/_G_config.h /usr/include/time.h
l3bitstream.o: /usr/include/sys/time.h /usr/include/linux/types.h
l3bitstream.o: /usr/include/linux/posix_types.h
l3bitstream.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
l3bitstream.o: /usr/include/linux/time.h /usr/include/sys/types.h
l3bitstream.o: /usr/include/sys/bitypes.h portableio.h ieeefloat.h
l3bitstream.o: /usr/include/math.h /usr/include/huge_val.h
l3bitstream.o: /usr/include/endian.h /usr/include/bytesex.h
l3bitstream.o: /usr/include/nan.h /usr/include/values.h
l3bitstream.o: /usr/include/ieee754.h /usr/include/ieee854.h bitstream.h
l3bitstream.o: layer3.h l3psy.h l3mdct.h l3loop.h formatBitstream.h huffman.h
l3bitstream.o: tables.h error.h
formatBitstream.o: formatBitstream.h /usr/include/stdlib.h
formatBitstream.o: /usr/include/features.h /usr/include/sys/cdefs.h
formatBitstream.o: /usr/include/errno.h /usr/include/linux/errno.h
formatBitstream.o: /usr/include/asm/errno.h /usr/include/alloca.h
formatBitstream.o: /usr/include/stdio.h /usr/include/libio.h
formatBitstream.o: /usr/include/_G_config.h /usr/include/assert.h
l3mdct.o: /usr/include/math.h /usr/include/features.h
l3mdct.o: /usr/include/sys/cdefs.h /usr/include/huge_val.h
l3mdct.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/nan.h
l3mdct.o: /usr/include/values.h /usr/include/ieee754.h /usr/include/ieee854.h
l3mdct.o: types.h /usr/include/stdio.h /usr/include/libio.h
l3mdct.o: /usr/include/_G_config.h /usr/include/time.h
l3mdct.o: /usr/include/sys/time.h /usr/include/linux/types.h
l3mdct.o: /usr/include/linux/posix_types.h /usr/include/asm/posix_types.h
l3mdct.o: /usr/include/asm/types.h /usr/include/linux/time.h
l3mdct.o: /usr/include/sys/types.h /usr/include/sys/bitypes.h portableio.h
l3mdct.o: ieeefloat.h layer3.h l3mdct.h
l3loop.o: /usr/include/math.h /usr/include/features.h
l3loop.o: /usr/include/sys/cdefs.h /usr/include/huge_val.h
l3loop.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/nan.h
l3loop.o: /usr/include/values.h /usr/include/ieee754.h /usr/include/ieee854.h
l3loop.o: types.h /usr/include/stdio.h /usr/include/libio.h
l3loop.o: /usr/include/_G_config.h /usr/include/time.h
l3loop.o: /usr/include/sys/time.h /usr/include/linux/types.h
l3loop.o: /usr/include/linux/posix_types.h /usr/include/asm/posix_types.h
l3loop.o: /usr/include/asm/types.h /usr/include/linux/time.h
l3loop.o: /usr/include/sys/types.h /usr/include/sys/bitypes.h portableio.h
l3loop.o: ieeefloat.h error.h tables.h layer3.h l3loop.h huffman.h
l3loop.o: l3bitstream.h bitstream.h reservoir.h
reservoir.o: /usr/include/stdio.h /usr/include/libio.h
reservoir.o: /usr/include/_G_config.h /usr/include/stdlib.h
reservoir.o: /usr/include/features.h /usr/include/sys/cdefs.h
reservoir.o: /usr/include/errno.h /usr/include/linux/errno.h
reservoir.o: /usr/include/asm/errno.h /usr/include/alloca.h
reservoir.o: /usr/include/math.h /usr/include/huge_val.h
reservoir.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/nan.h
reservoir.o: /usr/include/values.h /usr/include/ieee754.h
reservoir.o: /usr/include/ieee854.h types.h /usr/include/time.h
reservoir.o: /usr/include/sys/time.h /usr/include/linux/types.h
reservoir.o: /usr/include/linux/posix_types.h /usr/include/asm/posix_types.h
reservoir.o: /usr/include/asm/types.h /usr/include/linux/time.h
reservoir.o: /usr/include/sys/types.h /usr/include/sys/bitypes.h portableio.h
reservoir.o: ieeefloat.h error.h layer3.h l3loop.h huffman.h l3bitstream.h
reservoir.o: bitstream.h reservoir.h
