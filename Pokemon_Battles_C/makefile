prog:BattleByCategory.o LinkedList.o MainSecond.o MaxHeap.o Pokemon.o
	gcc BattleByCategory.o LinkedList.o MainSecond.o MaxHeap.o Pokemon.o -o PokemonsBattles

LinkedList.o: LinkedList.c LinkedList.h Defs.h
	gcc -c LinkedList.c	

MaxHeap.o: MaxHeap.c MaxHeap.h Defs.h
	gcc -c MaxHeap.c

BattleByCategory.o: BattleByCategory.c BattleByCategory.h Defs.h LinkedList.h MaxHeap.h
	gcc -c BattleByCategory.c

Pokemon.o: Pokemon.c Pokemon.h Defs.h
	gcc -c Pokemon.c

MainSecond.o: MainSecond.c MaxHeap.h Defs.h LinkedList.h Pokemon.h BattleByCategory.h
	gcc -c MainSecond.c

clean:
	rm -f *.o
