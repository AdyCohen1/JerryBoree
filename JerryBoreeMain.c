#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LinkedList.h"
#include "HashTable.h"
#include "MultiValueHashTable.h"
#include "Jerry.h"
#include "Defs.h"

Planet** planets = NULL; // pointer to an array of Planet pointers initialize to NULL
int planets_len = 0; // length of the planets array
Jerry** jerries = NULL; // pointer to an array of Jerry pointers
int jerries_len = 0; // length of the jerries array
int pcs_count = 0; //counter for PC's to help me decide how long the table list supposed to be


// Function to allocate memory for planets and jerries arrays
status allocateMemory(const int numberOfPlanets) {
    planets = (Planet **)malloc(numberOfPlanets * sizeof(Planet *)); // allocate memory for planet array of pointers
    if (!planets) { //in case memory allocation fails, return failure
        return failure;
    }
    return success; //if allocation success return success
}

void destroyGlobalsAlloc() {
    //that func have the only responsibility to delete all data in program and exit the program.
    for (int i = 0; i < planets_len; i ++) { // destroy all planets
        if (planets[i] != NULL) {
            DestroyPlanet(planets[i]);
            planets[i] = NULL;
        }
    }
    pcs_count = 0;
    planets_len = 0;
    free(planets); // Free the memory allocated for planets
    exit(0); // exit the program
}

//that func is responsible to delete all jerries in case of memory problem during read configuration file!
//after that part jerries gets free and the func in un necessary anymore
void destroyAllB4StartMain() {
    if (jerries != NULL) {
        for (int i = 0; i < jerries_len; i ++) { //destroy all jerries
            if (jerries[i] != NULL) {
                DestroyJerry(jerries[i]);
                jerries[i] = NULL; //set Jerry to NULL for safety
            }
        }
        jerries_len = 0;
        free(jerries); // Free the memory allocated for Jerries
        jerries = NULL;
    }
    destroyGlobalsAlloc(); // always call destroy Global allocates (to delete all planets!)
}

// after reading the FILE - that func get the responsibility to destroy the whole program structures and allocates.
void destroyAllProgram(multiValueHashTable pcMultiHash, hashTable jerriesHashTable, LinkedList jerriesList) {
    if (jerriesHashTable) {
        destroyHashTable(jerriesHashTable);
    }
    if (pcMultiHash) {
        destroyMultiValueHashTable(pcMultiHash);
    }
    if (jerriesList) {
        destroyList(jerriesList);
    }
    destroyAllB4StartMain(); // to be insured jerries frees and also to delete all planets
}

/**
 * All the following function created to create generic ADT's : Hash table, Multi Hash and Linked List
 */

char* copyStrDeep(char* element){ // deep copy func for name's strings
    if (!element) {
        return NULL;
    }
    char* name = (char *)malloc(strlen((char *)element) + 1);
    if (!name) {
        return NULL;
    }
    strcpy(name, element);
    return name;
}

Jerry* copyJerry(Jerry* jerry) { // copy jerry structure (not a deep copy)
    if (!jerry) {
        return NULL;
    }
    Jerry* copyJerry = (Jerry*)jerry;
    return copyJerry;
}

status freeStr(char* element){ // free strings (names)
    if (!element)
        return failure;
    free(element);
    return success;
}

status freeJerry(Element element) { //free Jerry structure (call destroy Jerry from Jerry.h)
    if (!element) {
        return failure;
    }
    Jerry* jerry = (Jerry*)element;
    if (DestroyJerry(jerry) == failure) {
        return failure;
    }
    return success;
}

status freeNothing(Element element) { // to avoid double frees I created free nothing func to make fake free to shared objects
    if (!element) {
        return failure;
    }
    return success;
}

status printJerry(Element element) { // print Jerry func that called Print Jerry from Jerry.h.
    if (!element) {
        return failure;
    }
    Jerry* jerry = (Jerry*)element;
    return PrintJerry(jerry);
}

status printPlanet(Planet* planet) { //same as print jerry for planets
    if (!planet) {
        return failure;
    }
    return PrintPlanet(planet);
}

// print strings (names)
status printStr(char* element) {
    if (!element) {
        return failure;
    }
    printf("%s : \n", element); // according to task request
    return success;
}

bool compareStr(Element element1, Element element2) { // compare func between two chars (names)
    if (!element1 || !element2) {
        return false;
    }
    bool flag =  strcmp((char *)element1, (char *)element2) == 0;
    if (flag == false) {
        return false;
    }
    return true;
}

bool equalJerriesId(Jerry* jerry1, Jerry* jerry2){ //compare between two Jerries structures according to ID
    if (!jerry1 || !jerry2) {
        return false;
    }
    if (strcmp(jerry1->ID, jerry2->ID) == 0) {
        return true;
    }
    return false;
}

int strToHashNum(Element element) { // transform from char into numeric value
    if (!element) {
        return 0;
    }
    char* nameStr = (char *)element;
    int hashNum = 0;
    while (*nameStr) {
        hashNum += *nameStr; // Sum ASCII values of characters (each char has a numeric value)
        nameStr++;
    }
    return hashNum;
}

bool isPrime(int x) { //helper function to help finding the next prime number
    if (x < 2) {
        return false;
    }
    for (int i = 2; i * i <= x; i++) { // math calculate to find prime number
        if (x % i == 0) {
            return false;
        }
    }
    return true;
}

//that func gets number and finds the smallest prime number that bigger than the input number
int findNextPrime(int size) {
    if (!size) {
        return -1;
    }
    int candidate = size + 1; // to be sure the number will be bigger than input
    while(!isPrime(candidate)) {
        candidate++;
    }
    if (candidate >= 11) {
        return candidate; // explanation in Read Me file! - the minimum size of hash table
    }
    return 11;
}

// that function have the responsibility to create all structures that storing all data during program
status createStructuresForProgram(hashTable* jerriesHash, multiValueHashTable* pcMultiHash, LinkedList* jerriesList) {
    int hashSize = findNextPrime(jerries_len * 2 ); // calculate hash size and multi hash size
    int multiSize = findNextPrime(pcs_count * 3);

    // creating Jerries Hash table such that : key will be jerry - ID and the value will be a pointer to Jerry object
    // hash table have no ability to destroy Jerry - contain fake free that free nothing
    *jerriesHash = createHashTable(
        (CopyFunction)copyStrDeep,
        (FreeFunction)freeStr,
        (PrintFunction)printStr,
        (CopyFunction)copyJerry,
        (FreeFunction)freeNothing,
        (PrintFunction)printJerry,
        (EqualFunction)compareStr,
        strToHashNum,
        hashSize);

    if (!(*jerriesHash)) {
        return failure;
    }

    // add all elements into hash table from jerries array
    for (int i = 0; i < jerries_len; i++) {
        if (addToHashTable(*jerriesHash, jerries[i]->ID, jerries[i]) == failure) { //add all jerries to jerries hash table. key = Jerry ID, value = pointer into Jerry_s
            destroyHashTable(*jerriesHash);
            return failure;
        }
    }
    // creating PC multi Hash table such that : key will be pc name and the value will be a pointer to Jerry objects that contains that pc
    // multi hash table have no ability to destroy Jerry - contain fake free that free nothing for the values
    *pcMultiHash = createMultiValueHashTable(
        (CopyFunction)copyStrDeep,
        (FreeFunction)freeStr,
        (PrintFunction)printStr,
        (CopyFunction)copyJerry,
        (FreeFunction)freeNothing,
        (PrintFunction)printJerry,
        (EqualFunction)compareStr,
        (EqualFunction)equalJerriesId,
        strToHashNum,
        multiSize);

    if (!(*pcMultiHash)) {
        return failure;
    }
    for (int i = 0; i < jerries_len; i++) {
        for (int j = 0; j < jerries[i]->physical_count; j++) {
            addToMultiValueHashTable(*pcMultiHash, jerries[i]->characteristics[j]->pc_name, jerries[i]); //that func handle case of exist key
        }
    }

    //creating Linked List such that - key will be Jerry's ID and the value will be Jerry object
    // that structure save Jerries according to order of creating.
    // the only structure that responsible to delete Jerry after finishing reading the File
    *jerriesList = createLinkedList(
        (CopyFunction)copyJerry,
        (FreeFunction)freeJerry,
        (PrintFunction)printJerry,
        (EqualFunction)equalJerriesId);

    if (!(*jerriesList)) {
        return failure;
    }

    for (int i = 0; i < jerries_len; i++) {
        if (appendNode(*jerriesList, jerries[i]) == failure) {
            return failure;
        }
    }
    free(jerries);
    return success;
}

// Function to get a planet by its name from the planets array
Planet* getPlanetByName(char* planet_name) {
    int i = 0;
    for (i = 0; i < planets_len; i++) {
        if(strcmp(planets[i]->planet_name, planet_name) == 0) { // compare planet names
            return planets[i]; //return the planet if found else return NULL
        }
    }
    return NULL;
}

// helper function to create a planet from a line in the configuration file
Planet* createPlanet_helper(char* line) {
    char planet_name[300]; //maximum size of string is 300 bits according to task information
    float x;
    float y;
    float z;
    sscanf(line, "%299[^,],%f,%f,%f", planet_name, &x, &y, &z); // scan planet information from the line
    for (int i = 0; i < planets_len; i++) {
        if (strcmp(planets[i]->planet_name, planet_name) == 0) {
            return NULL; // Stop creating the duplicate Jerry
        }
    }
    if (planet_name != NULL) { //prevent NULL name will send to the function
        Planet *planet = CreatePlanet(planet_name, x, y, z); //send the information to create planet func - the func return pointer to new planet structure
        if (planet == NULL) {
            printf("A memory problem has been detected in the program \n"); //in case of memory allocation fails - print to stdout "memory problem" and exit the program after delete all data and free all allocations
            destroyGlobalsAlloc();
        }return planet;
    }
    return NULL;;
}


// Function to create planets by reading from the file
status createPlanets(FILE *file, int numberOfPlanets) {
    char one_line[256];
    for (int i = 0; i < numberOfPlanets; i++) {
        fgets(one_line, sizeof(one_line), file);  //read a line from the file - "fgets" read a line and remember the line number
        Planet *planet = createPlanet_helper(one_line);  // create planet from the line
        if (planet != NULL) {
            planets[planets_len] = planet;  // store the planet in the planets array
            planets_len ++; //increase by 1 the number of planets
        }else {
            return failure;
        }
    }
    return success;
}

// helper function to create a Jerry from a line in the configuration file
Jerry* createJerry_helper(char* line, Jerry** jerries, int jerries_len) {
    char id[300];
    char origin_name[300];
    char planet_name[300];
    int happiness;
    if (id != NULL) {
        sscanf(line, "%299[^,],%299[^,],%299[^,],%d", id, origin_name, planet_name, &happiness);

        for (int i = 0; i < jerries_len; i++) { //handle case of double jerries (make sure ID is unique)

            if (strcmp(jerries[i]->ID, id) == 0) {
                return NULL; // Stop creating the duplicate Jerry
            }
        }

        Planet *planet = getPlanetByName(planet_name); //use helper func to add the planet to Jerry in case its exist
        if (planet == NULL) { // if planet not found, return NULL
            return NULL;
        }
        if (id != NULL) { //prevent case of sending nulls names
            Origin *origin = CreateOrigin(planet, origin_name); // create origin for Jerry
            if (!origin) { // if origin not created, return NULL
                printf("A memory problem has been detected in the program \n");
                destroyAllB4StartMain();
            }

            Jerry *jerry = CreateJerry(id, happiness, origin); //create the Jerry
            if (!jerry) {
                printf("A memory problem has been detected in the program \n"); //in case of memory allocation fails - print to stdout "memory problem" and exit the program after delete all data and free all allocations
                destroyAllB4StartMain();
            }
            return jerry; //return Jerry in case of success
        }
    }
    return NULL;
}



status createJerries(FILE *file) {
    char one_line[256];
    while (fgets(one_line, sizeof(one_line), file)) {// read each line from the file
        if (one_line[0] != '\t' && &one_line[0] != NULL) {
            Jerry *jerry = createJerry_helper(one_line, jerries, jerries_len);
            if (jerry != NULL) {
                Jerry** temp = realloc(jerries, (jerries_len + 1) * sizeof(Jerry*));
                if (!temp) {
                    printf("A memory problem has been detected in the program \n");
                    destroyAllB4StartMain();
                    return failure;
                }
                jerries = temp;
                jerries[jerries_len] = jerry; // Add Jerry to the array
                jerries_len++; //increase the number of Jerries
            }
        }else {
            if (jerries_len > 0) { // ensure there's a Jerry to add to
                Jerry* new_jerry = jerries[jerries_len - 1]; //add the new jerry to the array of jerries in the end of the aaray
                if (new_jerry) { //now add pcs to jerry in case there are
                    char pc_name[300];
                    float pc_value;
                    sscanf(one_line + 1, "%299[^:]:%f", pc_name, &pc_value);
                    PhysicalCharacteristic *pc = CreatePhysicalCharacteristics(pc_name, pc_value);
                    if (pc == NULL) {
                        DestroyJerry(new_jerry);
                        printf("A memory problem has been detected in the program \n"); //in case of memory allocation fails - print to stdout "memory problem" and exit the program after delete all data and free all allocations
                        destroyAllB4StartMain();
                    }else {
                        for (int i = 0; i < jerries_len; i++) {
                            for (int j = 0; j < jerries[i]->physical_count; j++) {
                                if (strcmp(pc->pc_name, jerries[i]->characteristics[j]->pc_name) == 0) {
                                    pcs_count--;
                                    break;
                                }
                            }
                            break;
                        }
                        pcs_count++; // count unique pc's to know how big create the multi hash table
                        AddPhysicalCharacter(new_jerry, pc);
                    }
                }
            }
        }
    }
    return success;
}

Jerry* jerryExist(hashTable jerriesHash ,char* name) { // helper function to avoid copy code in cases
    return lookupInHashTable(jerriesHash, name);
}

float valByPC(Jerry* jerry, char* pcName) { // helper func that gets value of specific pc.
    for (int i = 0; i < jerry->physical_count; i++) {
        if (strcmp(jerry->characteristics[i]->pc_name, pcName) == 0) {
            float valRes = jerry->characteristics[i]->pc_val;
            return valRes;
        }
    }
    return INFINITY;
}

Jerry* closestPCVal(LinkedList valueList, float val, char* pcName) { //helper func to help finding the closest value in case 6
    int counter = getLengthList(valueList);
    float absRes = INFINITY;
    Jerry* similarJerry = NULL;
    for (int i = 1; i <= counter; i ++) {
        Jerry* optionalJerry = (Jerry *)getDataByIndex(valueList, i); //checking each Jerry that contain that pc
        float currVal = valByPC(optionalJerry, pcName);
        if (currVal != INFINITY) {
            float resNum = fabs(currVal - val);

            if (resNum < absRes) {
                absRes = resNum;
                similarJerry = optionalJerry; // update the similar jerry
            }
        }
    }
    return similarJerry;
}
// helper func to reduce code in main
Planet* planetExist(char* name) { //checks if planet exist by name
    Planet* planet = NULL;
    for (int i = 0; i < planets_len; i++) {
        if (strcmp(planets[i]->planet_name, name) == 0) {
            planet = planets[i];
            break;
        }
    }
    return planet;
}

status addJerryToStructures(hashTable jerriesHash, LinkedList jerriesList ,Jerry* jerry) { //new Jerry is no contain pc !
    if (!jerriesHash || !jerry) {
        return failure;
    }
    if (addToHashTable(jerriesHash, jerry->ID, jerry) == failure) {
        return failure;
    }
    if (appendNode(jerriesList, jerry) == failure) {
        return failure;
    }

    return success;
}


// Function to display the menu and get the user's choice
int menu() {
    char choice[300];
    printf("Welcome Rick, what are your Jerry's needs today ? \n");
    printf("1 : Take this Jerry away from me \n");
    printf("2 : I think I remember something about my Jerry \n");
    printf("3 : Oh wait. That can't be right \n");
    printf("4 : I guess I will take back my Jerry now \n");
    printf("5 : I can't find my Jerry. Just give me a similar one \n");
    printf("6 : I lost a bet. Give me your saddest Jerry \n");
    printf("7 : Show me what you got \n");
    printf("8 : Let the Jerries play \n");
    printf("9 : I had enough. Close this place \n");
    scanf("%s", choice); //scan the user's choice
    while (getchar() != '\n'); //cleaning input buffer (in case the user added invalid input - clear the stream to prevent case of multiply fails
    if (strlen(choice) != 1) { // validate the input length in case the input is not a single character, return 0 (invalid choice).
        return 0;
    }
    // converting the user's choice from a character to an integer using hash helper func!
    int choice_to_num = strToHashNum(choice) - 48; // in ASCII (48 == 0 , 49 == 1 ) - so that calc converts back char to int (handle case of invalid input!)
    return choice_to_num; //return the user choice
}

//mini menu for case 7
int miniMenu() {
    char choice_case[300];
    printf("What information do you want to know ? \n");
    printf("1 : All Jerries \n");
    printf("2 : All Jerries by physical characteristics \n");
    printf("3 : All known planets \n");
    scanf("%s", choice_case);
    while (getchar() != '\n'); //cleaning input buffer (in case the user added invalid input - clear the stream to prevent case of multiply fails
    if (strlen(choice_case) != 1 ) { // validate the input length in case the input is not a single character, return 0 (invalid choice).
        return 0;
    }
    int choice_to_num = strToHashNum(choice_case) - 48;
    return choice_to_num; //return the user choice
}

//mini menu for case 8
int miniMenu2() {
    char choice_case[300];
    printf("What activity do you want the Jerries to partake in ? \n");
    printf("1 : Interact with fake Beth \n");
    printf("2 : Play golf \n");
    printf("3 : Adjust the picture settings on the TV \n");
    scanf("%s", choice_case);
    while (getchar() != '\n'); //cleaning input buffer (in case the user added invalid input - clear the stream to prevent case of multiply fails
    if (strlen(choice_case) != 1 ) {
        return 0;
    }
    int choice_to_num = strToHashNum(choice_case) - 48;
    return choice_to_num; //return the user choice
}


// Main function of the program - gets argc and argv = number of inputs and array of the inputs
int main(int argc, char* argv[]) {
    if (argc != 3) { // check if the correct number of arguments is provided
        return failure;
    }
    int numberOfPlanets = atoi(argv[1]); // convert the first argument to an integer (number of planets)
    char* configurationFile = argv[2];  // gets the configuration file

    if (allocateMemory(numberOfPlanets) == failure) { // allocate memory for planets and jerries arrays
        printf("A memory problem has been detected in the program \n");
        exit(0);
    }

    FILE *file = fopen(configurationFile, "r"); // open the configuration file
    if (!file) { // if the file cannot be opened, free memory and return failure
        free(planets);
        return failure;
    }

    char line[256];
    fgets(line, sizeof(line), file);  // Skip "Planets" line

    status flag1 = createPlanets(file, numberOfPlanets); // create planets from the file
    if (flag1 == failure) {
        destroyGlobalsAlloc();
    }

    fgets(line, sizeof(line), file);  // Skip "Jerries" line
    status flag2 = createJerries(file); // create jerries from the file
    if (flag2 == failure) {
        printf("A memory problem has been detected in the program \n");
        destroyAllB4StartMain();
    }
    fclose(file); //close the file

    //declare about structures
    hashTable jerriesHash = NULL;
    multiValueHashTable pcMultiHash = NULL;
    LinkedList jerriesList = NULL;

    if (createStructuresForProgram(&jerriesHash, &pcMultiHash, &jerriesList) == failure) { // save all data in structures and delete jerries global array!
        printf("A memory problem has been detected in the program \n");
        destroyAllProgram(pcMultiHash, jerriesHash, jerriesList);
    }
    jerries = NULL;



    int user_choice;
    do {
        user_choice = menu(); // display the menu and get user's choice
        switch (user_choice) {
            case 1: { // case 1 give an option to add new jerry to daycare
                char relevant_jerry_id[300];
                char relevant_planet_name[300];
                char relevant_origin[300];
                int happiness;
                printf("What is your Jerry's ID ? \n");
                scanf("%s", relevant_jerry_id);

                Jerry *relevant_jerry = jerryExist(jerriesHash ,relevant_jerry_id); //initialize a pointer to the relevant Jerry and check if that Jerry is already in daycare

                if (relevant_jerry != NULL) { // if jerry is already in daycare
                    printf("Rick did you forgot ? you already left him here ! \n");
                    break;
                }

                printf("What planet is your Jerry from ? \n");
                scanf("%s", relevant_planet_name);

                Planet* relevantPlanet = planetExist(relevant_planet_name);
                if (!relevantPlanet) { //checks for validation planet
                    printf("%s is not a known planet ! \n", relevant_planet_name);
                    break;
                }

                printf("What is your Jerry's dimension ? \n");
                scanf("%s", relevant_origin);

                printf("How happy is your Jerry now ? \n");
                scanf("%d", &happiness);

                if (happiness < 0 || happiness > 100) {
                    break;
                }
                Origin* newOrigin = CreateOrigin(relevantPlanet, relevant_origin);
                Jerry* newJerry = CreateJerry(relevant_jerry_id, happiness, newOrigin);

                //add new jerry to the structures
                if (addJerryToStructures(jerriesHash, jerriesList , newJerry) == failure) {
                    printf("A memory problem has been detected in the program \n");
                    destroyAllProgram(pcMultiHash, jerriesHash, jerriesList);
                }

                PrintJerry(newJerry);
                break;
            }

            case 2: { // that case give an option to add new pc for jerry
                char relevant_jerry_id[300];
                char relevant_pc_name[300];
                float pcVal;
                printf("What is your Jerry's ID ? \n");
                scanf("%s", relevant_jerry_id);

                Jerry *relevant_jerry = jerryExist(jerriesHash ,relevant_jerry_id); //checks for validation Jerry
                if (!relevant_jerry) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                printf("What physical characteristic can you add to Jerry - %s ? \n", relevant_jerry->ID);
                scanf("%s", relevant_pc_name); // ask for pc name

                if (IsPhysicalCharacteristicExists(relevant_jerry, relevant_pc_name) == true) {
                    printf("The information about his %s already available to the daycare ! \n", relevant_pc_name);
                    break; // in case of that pc is already in Jerry
                }

                printf("What is the value of his %s ? \n", relevant_pc_name);
                scanf("%f", &pcVal);

                PhysicalCharacteristic* new_pc = CreatePhysicalCharacteristics(relevant_pc_name, pcVal);
                if (AddPhysicalCharacter(relevant_jerry, new_pc) == failure) {
                    printf("A memory problem has been detected in the program \n");
                    destroyAllProgram(pcMultiHash, jerriesHash, jerriesList);
                }
                if (addToMultiValueHashTable(pcMultiHash, relevant_pc_name, relevant_jerry) == failure) { // add new pc to multi hash as a key and the jerry as value
                    printf("A memory problem has been detected in the program \n");
                    destroyAllProgram(pcMultiHash, jerriesHash, jerriesList);
                }
                displayMultiValueHashElementsByKey(pcMultiHash, relevant_pc_name); // display the new pc's values
                break;

            }

            case 3: { // that case is give an option to delete a pc from specific jerry
                char relevant_jerry_id[300];
                char relevant_pc_name[300];
                printf("What is your Jerry's ID ? \n");
                scanf("%s", relevant_jerry_id);

                Jerry *relevant_jerry = jerryExist(jerriesHash ,relevant_jerry_id);
                if (!relevant_jerry) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }

                printf("What physical characteristic do you want to remove from Jerry - %s ? \n", relevant_jerry_id);
                scanf("%s", relevant_pc_name);

                LinkedList existPCList = lookUpInMultiValueHashTable(pcMultiHash, relevant_pc_name);
                if (existPCList) { // checks jerry has that pc before delete that pc
                    if (searchByKeyInList(existPCList, relevant_jerry) != NULL) {
                        removeFromMultiValueHashTable(pcMultiHash, relevant_pc_name, relevant_jerry);
                        if (RemovePhysicalCharacter(relevant_jerry, relevant_pc_name) == failure) {
                            printf("A memory problem has been detected in the program \n");
                            destroyAllProgram(pcMultiHash, jerriesHash, jerriesList);
                        }
                        PrintJerry(relevant_jerry);
                        break;
                    }
                    printf("The information about his %s not available to the daycare ! \n", relevant_pc_name);
                    break;
                }
            }

            case 4: { //  in that case I can delete jerry from structures (find hin in O(1))
                char relevant_jerry_id[300];
                printf("What is your Jerry's ID ? \n");
                scanf("%s", relevant_jerry_id);

                Jerry *relevant_jerry = jerryExist(jerriesHash ,relevant_jerry_id); // search if jerry exist in O(1)
                if (!relevant_jerry) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
                removeFromHashTable(jerriesHash, relevant_jerry->ID); // remove that jerry
                for (int i = 0; i< relevant_jerry->physical_count; i++) {
                    if (lookUpInMultiValueHashTable(pcMultiHash, relevant_jerry->characteristics[i]->pc_name) != NULL) {
                        removeFromMultiValueHashTable(pcMultiHash, relevant_jerry->characteristics[i]->pc_name, relevant_jerry);
                    }
                }
                if (deleteNode(jerriesList, relevant_jerry) == failure) {
                    printf("A memory problem has been detected in the program \n");
                    destroyAllProgram(pcMultiHash, jerriesHash, jerriesList);
                }
                break;
            }

            case 5: { // in that case you can find similar jerry by knowing pc and closest value
                char relevant_pc_name[300];
                float pcVal;
                printf("What do you remember about your Jerry ? \n");
                scanf("%s", relevant_pc_name);

                LinkedList valueList = lookUpInMultiValueHashTable(pcMultiHash, relevant_pc_name);
                if (valueList == NULL) {
                    printf("Rick we can not help you - we do not know any Jerry's %s ! \n",relevant_pc_name);
                    break;
                }

                printf("What do you remember about the value of his %s ? \n", relevant_pc_name);
                scanf("%f", &pcVal);

                Jerry* similarJerry = closestPCVal(valueList, pcVal, relevant_pc_name); //use helper function to find the similar jerry
                printf("Rick this is the most suitable Jerry we found : \n");
                PrintJerry(similarJerry);
                removeFromHashTable(jerriesHash, similarJerry->ID);
                for (int i = 0; i< similarJerry->physical_count; i++) {
                    if (lookUpInMultiValueHashTable(pcMultiHash, similarJerry->characteristics[i]->pc_name) != NULL) {
                        removeFromMultiValueHashTable(pcMultiHash, similarJerry->characteristics[i]->pc_name, similarJerry);
                    }
                }
                if (deleteNode(jerriesList, similarJerry) == failure) { // delete that jerry after print the jerry
                    printf("A memory problem has been detected in the program \n");
                    destroyAllProgram(pcMultiHash, jerriesHash, jerriesList);
                }
                printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
                break;

            }
            case 6: { // that case give an option to get the saddest jerry and delete him from structures
                int counter = getLengthList(jerriesList);
                if (counter == 0 || !jerriesList) {
                    printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                    break;
                }
                printf("Rick this is the most suitable Jerry we found : \n");

                Jerry* sadJerry = NULL;
                for (int i = 1; i <= counter; i++) {
                    Jerry* currJerry = getDataByIndex(jerriesList, i);
                    if (i == 1) {
                        int lowHappiness = currJerry->happiness;
                        sadJerry = currJerry;
                    }else {
                        if (sadJerry != NULL) {
                            if (currJerry->happiness < sadJerry->happiness) {
                                sadJerry = currJerry;
                            }
                        }
                    }

                }
                if (sadJerry) {
                    PrintJerry(sadJerry);
                    removeFromHashTable(jerriesHash, sadJerry->ID);
                    for (int i = 0; i< sadJerry->physical_count; i++) {
                        if (lookUpInMultiValueHashTable(pcMultiHash, sadJerry->characteristics[i]->pc_name) != NULL) {
                            removeFromMultiValueHashTable(pcMultiHash, sadJerry->characteristics[i]->pc_name, sadJerry);
                        }
                    }
                    if (deleteNode(jerriesList, sadJerry) == failure) {
                        printf("A memory problem has been detected in the program \n");
                        destroyAllProgram(pcMultiHash, jerriesHash, jerriesList);
                    }
                    printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
                }
                break;
            }
            case 7: {
                int choiceCase;
                do {
                    choiceCase = miniMenu();
                    switch (choiceCase) {
                        case 1: {
                            if (getLengthList(jerriesList) > 0) {
                                displayList(jerriesList);
                                break;
                            }
                            printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                            break;
                        }

                        case 2: {
                            char relevant_pc_name[300];
                            printf("What physical characteristics ? \n");
                            scanf("%s", relevant_pc_name);

                            if (lookUpInMultiValueHashTable(pcMultiHash, relevant_pc_name) != NULL) {
                                displayMultiValueHashElementsByKey(pcMultiHash, relevant_pc_name);
                                break;
                            }
                            printf("Rick we can not help you - we do not know any Jerry's %s ! \n", relevant_pc_name);
                            break;
                        }

                        case 3: {
                            if (planets) {
                                for (int i = 0; i < planets_len; i ++) {
                                    PrintPlanet(planets[i]); // print each planet
                                }
                            }
                            break;
                        }

                        default: {
                            printf("Rick this option is not known to the daycare \n");
                            break;
                        }
                    }

                }while(0);

                break;
            }

            case 8: {
                int counter = getLengthList(jerriesList);
                if (counter == 0 || !jerriesList) {
                    printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                    break;
                }
                int choiceCase;
                do {
                    choiceCase = miniMenu2();
                    switch (choiceCase) {
                        case 1: {
                            for (int i = 1; i <= counter; i++) {
                                Jerry* currJerry = getDataByIndex(jerriesList, i);
                                if (currJerry->happiness >= 20) {
                                    currJerry->happiness += 15;
                                    if (currJerry->happiness > 100) {
                                        currJerry->happiness = 100;
                                    }
                                }else {
                                    currJerry->happiness -= 5;
                                    if (currJerry->happiness < 0){
                                        currJerry->happiness = 0;
                                    }
                                }
                            }
                            printf("The activity is now over ! \n");
                            displayList(jerriesList);
                            break;
                        }

                        case 2: {
                            for (int i = 1; i <= counter; i++) {
                                Jerry* currJerry = getDataByIndex(jerriesList, i);
                                if (currJerry->happiness >= 50) {
                                    currJerry->happiness += 10;
                                    if (currJerry->happiness > 100) {
                                        currJerry->happiness = 100;
                                    }
                                }else {
                                    currJerry->happiness -= 10;
                                    if (currJerry->happiness < 0){
                                        currJerry->happiness = 0;
                                    }
                                }
                            }
                            printf("The activity is now over ! \n");
                            displayList(jerriesList);
                            break;
                        }

                        case 3: {
                            for (int i = 1; i <= counter; i++) {
                                Jerry* currJerry = getDataByIndex(jerriesList, i);
                                    currJerry->happiness += 20;
                                    if (currJerry->happiness > 100) {
                                        currJerry->happiness = 100;
                                    }
                                }
                            printf("The activity is now over ! \n");
                            displayList(jerriesList);
                            break;
                        }
                        default: {
                            printf("Rick this option is not known to the daycare ! \n");
                            break;
                        }
                    }
                }while(0);
                break;
            }

            case 9: {
                printf("The daycare is now clean and close ! \n");
                destroyAllProgram(pcMultiHash, jerriesHash, jerriesList);
                break;
            }

            default: //default for case that any case choose (any other input different from 1-7)
                printf("Rick this option is not known to the daycare ! \n");
            break;
        }

    }while (user_choice != 9);

    return success; //return success!
}

