bin/task: obj/main.o obj/MusicShopDB.o
	gcc -l sqlite3 -o bin/task obj/main.o obj/MusicShopDB.o

obj/main.o: src/main.c
	gcc -l sqlite3 -c src/main.c -o obj/main.o

obj/MusicShopDB.o: src/MusicShopDB.c
	gcc -l sqlite3 -c src/MusicShopDB.c -o obj/MusicShopDB.o

clean:
	rm -f *.o bin/task

check:
	shell: /usr/bin/bash -e {0}
distcheck:
	shell: /usr/bin/bash -e {0}
