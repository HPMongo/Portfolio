CFLAGS = -Wall -fpic -coverage -lm -std=c99

default: all

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)

testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)


unittest1: unittest1.c dominion.o 
	gcc -o unittest1 -g unittest1.c  dominion.o rngs.o  $(CFLAGS)

unittest2: unittest2.c dominion.o 
	gcc -o unittest2 -g unittest2.c  dominion.o rngs.o  $(CFLAGS)

unittest3: unittest3.c dominion.o 
	gcc -o unittest3 -g unittest3.c  dominion.o rngs.o  $(CFLAGS)

unittest4: unittest4.c dominion.o 
	gcc -o unittest4 -g unittest4.c  dominion.o rngs.o  $(CFLAGS)

cardtest1: cardtest1.c dominion.o 
	gcc -o cardtest1 -g cardtest1.c  dominion.o rngs.o  $(CFLAGS)

cardtest2: cardtest2.c dominion.o 
	gcc -o cardtest2 -g cardtest2.c  dominion.o rngs.o  $(CFLAGS)

cardtest3: cardtest3.c dominion.o 
	gcc -o cardtest3 -g cardtest3.c  dominion.o rngs.o  $(CFLAGS)

cardtest4: cardtest4.c dominion.o 
	gcc -o cardtest4 -g cardtest4.c  dominion.o rngs.o  $(CFLAGS)

randomtestadventurer: randomtestadventurer.c dominion.o 
	gcc -o randomtestadventurer -g randomtestadventurer.c  \
dominion.o rngs.o  $(CFLAGS)

randomtestcard: randomtestcard.c dominion.o 
	gcc -o randomtestcard -g randomtestcard.c  \
dominion.o rngs.o  $(CFLAGS)



all: playdom player testDrawCard testBuyCard badTestDrawCard


unittestresults.out: unittest1 unittest2 unittest3 unittest4 cardtest1 \
cardtest2 cardtest3 cardtest4 
	./unittest1
	./unittest2
	./unittest3
	./unittest4
	./cardtest1
	./cardtest2
	./cardtest3
	./cardtest4
	gcov dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out

randomtestadventurer.out: randomtestadventurer
	echo "Running randomtestadventurer..."
	./randomtestadventurer > randomtestadventurer.out
	gcov dominion.c >> randomtestadventurer.out
	cat dominion.c.gcov >> randomtestadventurer.out

randomtestcard.out: randomtestcard
	echo "Running randomtestcard ..." 
	./randomtestcard > randomtestcard.out
	gcov dominion.c >> randomtestcard.out
	cat dominion.c.gcov >> randomtestcard.out


clean:
	rm -f *.o playdom.exe playdom test.exe test player player.exe testInit testInit.exe *.gcov *.gcda *.gcno *.so
	rm -rf *.dSYM
	rm cardtest1
	rm cardtest2
	rm cardtest3
	rm cardtest4
	rm unittest1
	rm unittest2
	rm unittest3
	rm unittest4
	rm badTestDrawCard
	rm testDrawCard
