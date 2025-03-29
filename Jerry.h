#ifndef JERRY_H
#define JERRY_H
#include "Defs.h"
/**
 *@struct Planet:
 *represent planet in the universe -
 *planet has: name (planet_name) and coordinates (x,y,z).
 */
//struct for representing planet (contain name and coordinates)
typedef struct Planet_t {
    char *planet_name; /// name of the planet (dynamic allocated string)
    float x;
    float y;
    float z;
} Planet;

/**
 * @struct Origin:
 * represent the origin of jerry -
 * origin has: pointer to Planet structure (planet) and name (origin_name)
 */
typedef struct Origin_t {
    Planet *planet; /// pointer to Planet
    char *origin_name; /// an Origin name (dynamic allocated string).
} Origin ;

/**
 * @struct PhysicalCharacteristic:
 * represent a single physical characteristic of jerry-
 * PhysicalCharacteristic has: name (pc_name) and value of the PhysicalCharacteristic (pc_val).
 */
typedef struct PhysicalCharacteristic_t {
    char *pc_name; ///name of the PhysicalCharacteristic
    float pc_val; /// value of the PhysicalCharacteristic.
} PhysicalCharacteristic;

/**
 * @struct Jerry:
 * represent a Jerry character -
 * Jerry has: unique id (ID), happiness level (happiness), an origin (origin),
 * an array of pointers to physical characteristics (characteristics) and count of physical characteristic.
 */
typedef struct Jerry_t {
    char *ID; /// an unique ID
    int happiness; /// happiness level
    Origin *origin; /// pointer to origin structure
    PhysicalCharacteristic **characteristics; /// array of pointers to physical characteristics.
    int physical_count; /// count of physical characteristics
} Jerry ;


/*************
 *Functions declarations (implement them in Jerry.c)
 ************/
/**
 *CreatePlanet func:
 *gets - pointer to name (unknown length of name), x,y,z coordinates.
 *return - a pointer to Planet structure.
 */
Planet* CreatePlanet(char *planet_name, float x, float y, float z);

/**
 *CreateOrigin func:
 *gets - pointer to planet structure and pointer to origin name (unknown length).
 *return - a pointer to Origin structure.
 */
Origin* CreateOrigin(Planet *planet, char *origin_name);

/**
 *CreatePhysicalCharacteristics func:
 *gets - pointer to name (unknown length of name) and value of the physical characteristic.
 *return - a pointer to PhysicalCharacteristic structure.
 */
PhysicalCharacteristic* CreatePhysicalCharacteristics(char *pc_name, float pc_val);

/**
 *CreateJerry func:
 *gets - pointer to unique id (unknown length of id), integer represent happiness level and a pointer to Origin structure.
 *return - a pointer to Jerry structure.
 */
Jerry* CreateJerry(char *ID, int happiness, Origin *origin);

/**
 *IsPhysicalCharacteristicExists func:
 *gets - pointer to Jerry structure and pointer to physical characteristic name
 *return - a flag (true or false) representing if the specific jerry have a specific physical characteristic.
 */
bool IsPhysicalCharacteristicExists(Jerry *jerry, char *pc_name);

/**
 *AddPhysicalCharacter func:
 *gets - a pointer to Jerry structure and pointer to PhysicalCharacteristic structure.
 *return - a status flag (failure or success) that representing if new PhysicalCharacteristic added to jerry.
 */
status AddPhysicalCharacter(Jerry *jerry, PhysicalCharacteristic *pc);

/**
 *RemovePhysicalCharacter func:
 *gets - a pointer to Jerry structure and pointer to PhysicalCharacteristic name.
 *return - a status flag (failure or success) that representing if PhysicalCharacteristic removed from jerry.
 */
status RemovePhysicalCharacter(Jerry *jerry, char *pc_name);

/**
 *PrintJerry func:
 *gets - a pointer to Jerry structure.
 *return - a status flag (failure or success) that representing if jerry have been printed as well.
 */
status PrintJerry(Jerry *jerry);

/**
 *PrintPlanet func:
 *gets - a pointer to Planet structure.
 *return - a status flag (failure or success) that representing if planet have been printed as well.
 */
status PrintPlanet(Planet *planet);

/**
 *Destroy funcs:
 *gets - a pointer to the structure.
 *return - void (nothing)
 *the purpose of those funcs is to free each one of the structure that the program built during the running to make sure there is no memory leaks
 *the funcs gets pointer to the structure and freeing all associated memory.
 */
status DestroyJerry(Jerry *jerry);
void DestroyPlanet(Planet *planet);
void DestroyOrigin(Origin *origin);
void DestroyPhysicalCharacteristic(PhysicalCharacteristic *pc);


#endif