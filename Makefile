mine_sweeper.out: mine_sweeper.o imdone.o
	gcc -g -Wall -o mine_sweeper.out mine_sweeper.o imdone.o

mine_sweeper.o : mine_sweeper.h mine_sweeper.c
	gcc -g -Wall -c mine_sweeper.c

imdone.o: mine_sweeper.h imdone.c
	gcc -g -Wall -c imdone.c

clean:
	rm *.o mine_sweeper.out
