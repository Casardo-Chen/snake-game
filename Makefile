# Makefile for final project

snake: project.c
	gcc project.c gfx2.o -lX11 -lm -o project

clean:
	rm project

