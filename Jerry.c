#include "Jerry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *Create Planet func gets:
 * @param planet_name
 * @param x
 * @param y
 * @param z
 * @return pointer to planet
 * the func create new object (structure) of Planet - contains name and coordinates
 */
Planet* CreatePlanet(char *planet_name, float x, float y, float z){
    Planet *new_planet = NULL; //initialize pointer to planet structure to NULL.

    if (planet_name == NULL) {
        return NULL; // if the pointer to planet name is null - return null and stop the func (there is no planet to create)
    }

    new_planet = (Planet*)malloc(sizeof(Planet)); //ask for memory place for the new structure in size of Planet
    if (!new_planet) {
        return NULL; // return NULL if the allocation failed
    }

    new_planet->planet_name = (char *) malloc(strlen(planet_name) + 1); //memory allocation for name of the new planet (plus 1 for /0)
    if (!new_planet->planet_name) {// check if memory allocation was successful
        free(new_planet); // free the previously allocated memory (in case of micro fail I have need to free the macro)
        return NULL; //in that case of failure in asking for place return NULL
    }
    // in case the allocation success
    strcpy(new_planet->planet_name, planet_name); //copy the planet name into the allocated memory
    // set the coordinates
    new_planet->x = x;
    new_planet->y = y;
    new_planet->z = z;

    return new_planet; // return the new planet (the func will return a pointer to the new planet)
}

/**
 * Create Origin func:
 * @param planet
 * @param origin_name
 * @return pointer to the new origin structure
 */
Origin* CreateOrigin(Planet *planet, char *origin_name) {
    Origin *new_origin = NULL; // initialize a pointer to new origin structure to NULL for safety

    if (!planet || !origin_name) { // checking if inputs are valid
        return NULL;
    }

    new_origin = (Origin*) malloc(sizeof(Origin)); // allocate memory for the Origin structure
    if (!new_origin) {
        return NULL; //same as planet func - return null and print in case of failure
    }

    new_origin->origin_name= (char *) malloc(strlen(origin_name) +1); // allocate memory for the Origin name
    if (!new_origin->origin_name) { // check if memory allocation was successful
        free(new_origin);
        return NULL;
    }

    strcpy(new_origin->origin_name, origin_name); // copy the origin name into the allocated memory
    new_origin->planet = planet; // set the associated Planet
    return new_origin; // return the new origin structure
}

/**
 * Create Physical Characteristic func:
 * @param pc_name
 * @param pc_val
 * @return
 */
PhysicalCharacteristic* CreatePhysicalCharacteristics(char *pc_name, float pc_val) {
    PhysicalCharacteristic *new_pc = NULL; // initialize a pointer to NULL for safet
    if (!pc_name) { // checking if the input name is valid - if not return null
        return NULL;
    }

    new_pc = (PhysicalCharacteristic*)malloc(sizeof(PhysicalCharacteristic)); // allocate memory for the PhysicalCharacteristic structure
    if (!new_pc) {
        return NULL; // return NULL if allocation failed
    }

    new_pc->pc_name =(char *) malloc(strlen(pc_name)+ 1); // allocate memory for the characteristic name (string)

    if (!new_pc->pc_name) { // same as planet - in case of failure in allocate memory free from micro to macro
        free(new_pc);
        return NULL;
    }

    strcpy(new_pc->pc_name, pc_name); // copy the characteristic name into the allocated memory
    new_pc->pc_val = pc_val; //set the pc value
    return new_pc; //return the new pc
}

/**
 * Create Jerry func:
 * @param ID
 * @param happiness
 * @param origin
 * @return
 */
Jerry* CreateJerry(char *ID, int happiness, Origin *origin) {
    Jerry *new_jerry = NULL; // initialize a pointer to NULL for safety
    if (!ID || !origin || happiness < 0 || happiness > 100) { // validate the inputs
        return NULL;
    }

    new_jerry = (Jerry*) malloc(sizeof(Jerry)); // allocate memory for the Jerry structure
    if(!new_jerry) {
        return NULL;
    }

    new_jerry->ID= (char *) malloc(strlen(ID)+1); // allocate memory for the Jerry ID (string) + 1 for "\0"
    if (!new_jerry->ID) {
        free(new_jerry);
        return NULL; //in case of failure - return NULL
    }

    strcpy(new_jerry->ID, ID); //copy the id to the new structure in the place that allocated in the program for the id
    new_jerry->happiness = happiness; //set happiness
    new_jerry->origin = origin; //set origin
    new_jerry->characteristics = NULL; //initialize an empty array for pc
    new_jerry->physical_count = 0; // initialize counter of pcs to 0 (no pc at the beginning)
    return new_jerry;
}

bool IsPhysicalCharacteristicExists(Jerry *jerry, char *pc_name) {
    int i = 0;
    if (!jerry || !pc_name) {
        return false;
    }
    for (i=0; i<(jerry->physical_count); i++) {
        if (strcmp(jerry->characteristics[i]->pc_name, pc_name) == 0) {
            return true;
        }
    }
    return false; //in case the chosen Jerry does not have that physical characteristics.
}

/**
 * Add Physical Characteristic into Jerry func:
 * @param jerry
 * @param pc
 * @return status (success or failure)
 */
status AddPhysicalCharacter(Jerry *jerry, PhysicalCharacteristic *pc) {
    PhysicalCharacteristic **temp = NULL; // initialize array of pointers for reallocated memory.

    if (!jerry || !pc) { // validation checks
        return failure;
    }
    //asking the program to reallocate the memory the structure need in the program. - for safety do it on temporary variable in case of failure to not loos all the information
    temp = realloc(jerry->characteristics, (jerry->physical_count + 1) * sizeof(PhysicalCharacteristic*));
    if (temp != NULL) { // in case of success reallocating memory
        jerry->characteristics = temp; //set the old pcs into the same place as same as the structure before adding new pc
        jerry->characteristics[jerry->physical_count] = pc; // add the new pc in the end of the array of pcs
        jerry->physical_count++; //add 1 to the counter of pcs
        return success;
    }
    return failure; // in case realloc fails return failure and do nothing (save the pcs array as before)
}

/**
 * Remove Physical Characteristic from Jerry func:
 * @param jerry
 * @param pc_name
 * @return status
 */
status RemovePhysicalCharacter(Jerry *jerry, char *pc_name) {
    PhysicalCharacteristic *to_remove = NULL; //initialize a pointer to PC structure - to "catch" the pc I want to delete
    PhysicalCharacteristic **temp = NULL; // initialize array of pointers for reallocated memory

    if (!jerry || !pc_name || jerry->physical_count == 0) { //validation checks
        return failure;
    }

    for (int i = 0; i < jerry->physical_count; i++) { //start searching for match with the input PC name
        if (strcmp(jerry->characteristics[i]->pc_name, pc_name) == 0) { //in case jerry has the pc
            to_remove = jerry->characteristics[i]; // save the pointer to the pc (to make it possible to delete after organize the array)
            for(int j = i; j < jerry->physical_count - 1; j++) {
                jerry->characteristics[j] = jerry->characteristics[j + 1]; // shifting left all the pcs- in that way Jerry's pcs array not contain the pc to remove
            }
            break;
        }
    }
    if (to_remove) { //in case of the last for loop find the pc:
        jerry->physical_count --; // decrease the counter of pcs in 1
        if (jerry->physical_count == 0) { //in case the pc that found is the only one to delete - free the array and the name of the pc
            free(jerry->characteristics);
            jerry->characteristics = NULL;
        }
        else {
            temp = realloc(jerry->characteristics,jerry->physical_count * sizeof(PhysicalCharacteristic*)); //in case Jerry has more pcs - reallocate the size of the pcs array
            if (!temp) { // if reallocated fails it return NULL then:
                free(to_remove->pc_name); // free the pc the user want to delete (name and structure)
                free(to_remove);
                to_remove = NULL; //initialize with NULL to make sure not exception will create
                jerry->physical_count ++; //increase the counter in 1 (the original size before this func)
                jerry->characteristics[jerry->physical_count - 1] = to_remove; //add to the last element in the array of pointers to pcs NULL
                return failure;
            }
            jerry->characteristics = temp; //if reallocation success - point to the new size of array and remove the pc to delete
        }
        free(to_remove->pc_name); //free the char and name of the pc to delete - to be safe from memory leaks
        free(to_remove);
        return success;
    }
    return failure;
}

/**
 * Print Planet func:
 * @param planet
 * @return
 */
status PrintPlanet(Planet *planet) {
    if (!planet) { //check if the pointer is not NULL
        return failure;
    }
    //round the float number
    float roundedX = planet->x * 100.0f / 100.0f;
    float roundedY = planet->y * 100.0f / 100.0f;
    float roundedZ = planet->z * 100.0f / 100.0f;
    printf("Planet : %s (%.2f,%.2f,%.2f) \n", planet->planet_name, roundedX, roundedY, roundedZ); // print the string of the name and the coordinates with 2 nums after the point
    return success;
}

/**
 * Print Jerry func:
 * @param jerry
 * @return
 */
status PrintJerry(Jerry *jerry) {
    if (!jerry || !jerry->origin->origin_name || !jerry->origin->planet) { //validation checks
        return failure;
    }
    printf("Jerry , ID - %s : \n", jerry->ID); // print Jerry according to the task structure
    printf("Happiness level : %d \n", jerry->happiness);
    printf("Origin : %s \n", jerry->origin->origin_name);
    PrintPlanet(jerry->origin->planet); // correctly pass the planet reference to print the planet with the matching func

    if (jerry->physical_count > 0 && jerry->characteristics != NULL) { //checking for pcs to see if there is a need to print them (in case Jerry has pcs)
        printf("Jerry's physical Characteristics available : \n");

        for (int i = 0; i < jerry->physical_count; i++) { // with for loop on Jerry's pcs print one by one according to the task, round the value with 2 nums after the point
            float pc_val_rounded = jerry->characteristics[i]->pc_val * 100.0f / 100.0f;

            if (i == 0) {
                printf("\t");
            }
            printf("%s : %.2f", jerry->characteristics[i]->pc_name, pc_val_rounded);

            if (i != jerry->physical_count - 1) {
                printf(" , ");
            }
            else {
                printf(" \n");
            }
        }
    }
    return success;
}

/**
 *Destroy Funcs - for each structure I created a destroy func to protect the program from leaks memory
 *each func free the memory that allocated for the structure from micro to macro
/**
 *
 * @param planet
 */
void DestroyPlanet(Planet *planet) {
    if (planet) {
        free(planet->planet_name);
        planet->planet_name = NULL;
        free(planet);
    }
}

/**
 * * @param origin
 */
void DestroyOrigin(Origin *origin) {
    if (origin) {
        if (origin->origin_name) {
            free(origin->origin_name);
            origin->origin_name = NULL;
        }
        free(origin);
    }
}

/**
 *
 * @param pc
 */
void DestroyPhysicalCharacteristic(PhysicalCharacteristic *pc) {
    if (pc != NULL) {
        free(pc->pc_name);
        pc->pc_name = NULL;
        free(pc);
    }
}

/**
 *
 * @param jerry
 */
status DestroyJerry(Jerry *jerry) {
    if (jerry) {
        for (int i = 0; i < jerry->physical_count; i ++) {
            DestroyPhysicalCharacteristic(jerry->characteristics[i]);
            jerry->characteristics[i] = NULL;
        }
        free(jerry->characteristics);
        jerry->characteristics = NULL;

        if (jerry->origin) {
            DestroyOrigin(jerry->origin);
            jerry->origin = NULL;
        }


        free(jerry->ID);
        jerry->ID = NULL;

        free(jerry);
        return success;
    }
    return failure;
}
