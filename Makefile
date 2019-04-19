progfib:
	gcc src/progfib.c src/defs.c -I./inc -o hw4_progfib -lpthread

progcnt:
	gcc -I./inc -o hw4_progcnt

progqsort:
	gcc -I./inc -o hw4_progqsort

manager:
	gcc src/artist_manager.c src/artist_ds.c src/artist.c src/error_checking.c -I./inc -o artist_manager

overseer:
	gcc -I./inc -o museum_overseer

clean:
	rm hw4_progfib hw4_progcnt artist_manager museum_overseer
