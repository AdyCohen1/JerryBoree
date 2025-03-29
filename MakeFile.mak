#Jerry Boree Make File:

JerryBoree: Jerry.o KeyValuePair.o LinkedList.o HashTable.o MultiValueHashTable.o JerryBoreeMain.o
	gcc Jerry.o KeyValuePair.o LinkedList.o HashTable.o MultiValueHashTable.o JerryBoreeMain.o -o JerryBoree


Jerry.o: Jerry.c Jerry.h Defs.h
	gcc -c Jerry.c


LinkedList.o: LinkedList.c LinkedList.h Defs.h
	gcc -c KeyValuePair.c


KeyValuePair.o: KeyValuePair.c KeyValuePair.h Defs.h
	gcc -c KeyValuePair.c


HashTable.o: HashTable.c HashTable.h LinkedList.h KeyValuePair.h Defs.h
	gcc -c HashTable.c


MultiValueHashTable.o: MultiValueHashTable.c MultiValueHashTable.h HashTable.h LinkedList.h KeyValuePair.h Defs.h
	gcc -c MultiValueHashTable.c


JerryBoreeMain.o: JerryBoreeMain.c MultiValueHashTable.h HashTable.h LinkedList.h KeyValuePair.h Defs.h
	gcc -c JerryBoreeMain.c


clean:
	rm -f *.o JerryBoree