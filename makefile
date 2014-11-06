out: GIFencoder.o GIFmain.o list.o stb_image.o
	gcc -o out GIFmain.o GIFenconder.o list.o
stb_image.o: stb_image.c
	gcc -o stb_image.o -c stb_image.c
GIFmain.o: GIFmain.c
	gcc -o GIFmain.o -c GIFmain.c
GIFenconder.o: GIFencoder.c GIFencoder.h
	gcc -o GIFencoder.o -c GIFencoder.c
list.o: list.c list.h
	gcc -o list.o -c list.c
