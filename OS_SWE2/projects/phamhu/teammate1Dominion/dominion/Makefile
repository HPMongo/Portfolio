CFLAGS = -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS) -fprofile-arcs -ftest-coverage

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
#necessary test files for assignment #3
unittest1: unittest1.c dominion.o rngs.o
	gcc -o unittest1 -g unittest1.c dominion.o rngs.o $(CFLAGS)
unittest2: unittest2.c dominion.o rngs.o
	gcc -o unittest2 -g unittest2.c dominion.o rngs.o $(CFLAGS)
unittest3: unittest3.c dominion.o rngs.o
	gcc -o unittest3 -g unittest3.c dominion.o rngs.o $(CFLAGS)
unittest4: unittest4.c dominion.o rngs.o
	gcc -o unittest4 -g unittest4.c dominion.o rngs.o $(CFLAGS)
cardtest1: cardtest1.c dominion.o rngs.o
	gcc -o cardtest1 -g cardtest1.c dominion.o rngs.o $(CFLAGS)
cardtest2: cardtest2.c dominion.o rngs.o
	gcc -o cardtest2 -g cardtest2.c dominion.o rngs.o $(CFLAGS)
cardtest3: cardtest3.c dominion.o rngs.o
	gcc -o cardtest3 -g cardtest3.c dominion.o rngs.o $(CFLAGS)
cardtest4: cardtest4.c dominion.o rngs.o
	gcc -o cardtest4 -g cardtest4.c dominion.o rngs.o $(CFLAGS)
interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

unittestresults.out: unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4
	./unittest1 >> unittestresults.out
	./unittest2 >> unittestresults.out
	./unittest3 >> unittestresults.out
	./unittest4 >> unittestresults.out
	./cardtest1 >> unittestresults.out
	./cardtest2 >> unittestresults.out
	./cardtest3 >> unittestresults.out
	./cardtest4 >> unittestresults.out
	gcov dominion.c -b>> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out

#test files for assignment #4
random1: randomtestadventurer.c dominion.o rngs.o
	gcc -o random1 -g randomtestadventurer.c dominion.c rngs.o $(CFLAGS)

randomtestadventurer.out: random1
	./random1 >> randomtestadventurer.out
	gcov dominion.c -b >> randomtestadventurer.out
	cat dominion.c.gcov >> randomtestadventurer.out

random2: randomtestcard.c dominion.o rngs.o
	gcc -o random2 -g randomtestcard.c dominion.c rngs.o $(CFLAGS)

randomtestcard.out: random2
	./random2 >> randomtestcard.out
	gcov dominion.c -b >> randomtestcard.out
	cat dominion.c.gcov >> randomtestcard.out

#runtests: testDrawCard
#	./testDrawCard &> unittestresult.out
#	gcov dominion.c >> unittestresult.out
#	cat dominion.c.gcov >> unittestresult.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

#all: playdom player testDrawCard testBuyCard badTestDrawCard unittest1
all: playdom player unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4

clean:
	rm -f *.o playdom.exe playdom test.exe test player player.exe testInit testInit.exe *.gcov *.gcda *.gcno *.so unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4 unittestresults.out random1 random2 randomtestadventurer.out randomtestcard.out
