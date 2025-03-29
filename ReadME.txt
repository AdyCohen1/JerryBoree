JerryBoree HW3_Advanced_Programin

Read Me File:

project description (introduction):
my project is designed to manage a collection of jerries and planets.
the program give the user an option to interact with planets and jerries and focused about time complexity.
each jerry and planet is unique by ID or Name.
the user can do different actions in the program:
1 : Take this Jerry away from me
2 : I think I remember something about my Jerry
3 : Oh wait. That can't be right
4 : I guess I will take back my Jerry now
5 : I can't find my Jerry. Just give me a similar one
6 : I lost a bet. Give me your saddest Jerry
7 : Show me what you got
8 : Let the Jerries play
9 : I had enough. Close this place

such that selection option 7 or 8 will give the user more actions in the program:
case 7 will open:
1 : All Jerries
2 : All Jerries by physical characteristics
3 : All known planets

case 8 will open:
1 : Interact with fake Beth
2 : Play golf
3 : Adjust the picture settings on the TV


Code Structure:

Defs.h:
declare about generic functions such that : 
copy function
free function
equal function
print function
Transform Into Number Function
status (success or failure)
bool (true or false)
Element - void *


Jerry.h:
that script is a public script created to help the programer understand the interface of Jerry Module.
in that script I declared about structures (Planet, Jerry, Origin and Physical Characteristic)
all structure are public.
Functions declared: in Jerry.h I gave functions signature to help the programmer knowing how to use the interface.
each function of create object return a pointer to the new object, functions like (remove pc, add pc and prints funcs) return status (success or failure) and one bool func to get know if jerry have a specific pc.
in additional I declared about Destroy functions to make sure no leak of memory will happen.

Jerry.c:
that script is a private script. in that script I implemented all the funcs from Jerry.h.
for each Create structure func I checked if allocation fails - in case of failure in memory the funcs will return NULL.
that script give the user the whole interface that the user can use for the program.

4 generic ADT's:
Key Value Pair- that ADT is type of pair that contain data for key and value
Linked List- ADT for Linked List : adding elements always to the end. the list start by index 1.
Hash Table - ADT for hash table contains Linked List (Node is KVP) to handle collusion by chaining- give an interface for structure that can find a key in time complexity of O(1).
Multi Hash Table- ADT for multi hash contains Hash Table and Linked List - interface to hash table such that a key contain Linked List of values (each node is KVP)

**size of table list:
	minimum size of table lists is 11 (small prime number).
	table list size calculate to be the next prime number - for hash table the calc will be twice Jerries count next prime (the idea is to create table big enough to save search time complexity O(1) in averege cases) is mult by 2 because during the program jerries are leaving daycare and in my vission a case that after creating all the program daycare will gets twice amount of jerries while non of them leaving is not gonne append.
	for pc multi hash the calc will be 3 times number of unique pc names next prime. I got the idea to make it in that size during listening to Lab lesson.

Jerry Boree Main:

main script - 
main a daycare that contains Jerries.

the main func starts by reading a configuration file.
during reading configuration file I creating Jerries and Planets and store them in Global arrays **Lior approved me in forum to use Globals arrays as helper array for step before storing the data in ADT's. in the end of reading the file the whole objects in the program already created.
a step after reading the file - I declared about 3 ADT's:
Linked List :
that ADT is the only structure responsible to delete and destroy Jerry object during the program.
key - JERRY. ID 
Value- Jerry* jerry (contains pointer to jerry structure)
Linked list save jerries by order of creating during the program.

Hash Table:
that ADT is an structure that saves Jerries such that-
key - Jerry.ID
value - Jerry* jerry
the difference between Hash Table and Linked List is by time complexity in searching for Jerry (in hash table O(1), in Linked List O(jerries_count))

Multi Hash Table:
that ADT is an structure that saves all jerries with same pc.
key - pc name
value - Linked List of pointers to Jerries ocjects.
that ADT gives the user an option to find all jerries that have the same unique pc by pc name in time complexity of O(1)

after that step - the main called helper func that builds the 3 ADT's and **frees jerries Global array** - from that point of program the only responsible to manage Jerries is those 3 ADT's (Linked LIst is the only one contain real free function to destroy jerry if needed - the two others contains fake frees to avoid double frees)

Main script contains helper functions:
functions to create the generic ADT's to make them works in way the user can use them for manaje Jerries according to the task. (contains - copy funcs (for names and for jerry), free funcs (fake free, strings free, planets free , jerries free), prints funcs, equal funcs (for Jerries ID, and strings (such that pc's names)))
func to find the next prime number
helper funcs to avoid copy code in menu to make my code readble and easy to debug.
Destroy all program structures func - in case the user choose "9" the program will delete to avoid memory leaks and end the program.

after that point- the menu will printed to stdout and the user can start manage Jerries in daycare!
