progfib:
	gcc src/progfib.c src/defs.c -I./inc -o hw4_progfib -lpthread

progcnt:
	gcc src/progcnt.c src/defs.c src/error_checking.c -I./inc -o hw4_progcnt -lpthread

progqsort:
	gcc src/progqsort.c src/defs.c src/error_checking.c -I./inc -o hw4_progqsort -g -lpthread

manager:
	gcc src/artist_manager.c src/artist_ds.c src/artist.c src/defs.c src/error_checking.c -I./inc -o artist_manager -lpthread

overseer:
	gcc -I./inc -o museum_overseer

clean:
	rm hw4_progfib hw4_progcnt hw4_progqsort artist_manager museum_overseer
