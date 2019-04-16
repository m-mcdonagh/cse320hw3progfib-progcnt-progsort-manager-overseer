progfib:
	gcc -I./inc -o hw4_progfib

progcnt:
	gcc -I./inc -o hw4_progcnt

progqsort:
	gcc -I./inc -o hw4_progqsort

manager:
	gcc src/artist_manager.c src/artist_ds.c src/artist.c src/error_checking.c -I./inc -o artist_manager

overseer:
	gcc -I./inc -o museum_overseer

