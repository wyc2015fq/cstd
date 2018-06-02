
all:
	gcc -o2 -o test_cap test_cap.c cap.c window.c ../fmtimg/fmtimg.c ../imgproc/color.c `pkg-config --cflags --libs gtk+-2.0`

clean:
	-rm *.o
