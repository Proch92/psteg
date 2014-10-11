elf:
	gcc -c src/psteg.c src/alpha.c src/info.c src/lsbextract.c src/createPNG.c
	gcc -o psteg psteg.o createPNG.o alpha.o info.o lsbextract.o -lpng -lm