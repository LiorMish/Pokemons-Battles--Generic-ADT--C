#include "Pokemon.h"
#include "Defs.h"
#include "LinkedList.h"
#include "MaxHeap.h"
#include "BattleByCategory.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 300 // max size of user input.

/////////////////////////////////////////////////// Global veriables

type **pokemonTypes = NULL; // types list.
bioInfo *bioPtr = NULL;
pokemon *pokPtr = NULL;
int maxCapacityInType; // maximum number of pokemons in each type.
int numOfTypes;        // number of types sent in the file.
int numOfPokemons = 0; // // number of pokemons sent in the file.
Battle battle;

/////////////////////////////////////////////////// Functions declaration

static int findType(char *);

static void readFile(char *);

static void printScreen();

static void printPokemonTypes();

static void freePokedex(Battle, int);

/////////////////////////////////////////////////// Static functions

static int findType(char *typeName) {
    if (typeName == NULL)
        return -1;
    int i;
    for (i = 0; i < numOfTypes; i++) {
        if (strcmp(pokemonTypes[i]->name, typeName) == 0)
            return i;
    }
    return -1;
}

static void printPokemonTypes() {
    int i;
    for (i = 0; i < numOfTypes; i++)
        print_pokemon_type(pokemonTypes[i]);
}

static void freePokedex(Battle battle, int typesCount) {
    // free the battle
    destroyBattleByCategory(battle);

    // free all the types created.
    int i;
    for (i = 0; i < typesCount; i++)
        freeType(pokemonTypes[i]);

    if (typesCount > 0)
        free(pokemonTypes);
}

// /////////////////////////////////////////////////// read file function

static void readFile(char *file) {
    FILE *cFile = fopen(file, "r");
    char line[BUFSIZE];
    int lineCount = 0;
    char *tempWord1, *tempWord2, *tempWord3, *tempWord4, *tempWord5, *tempWord6;
    char *me = "effective-against-me";
    int index1, index2;

    //////////////////////////////////////////////////////////////////////////////// create types
    pokemonTypes = (type **) malloc(sizeof(type *) * numOfTypes);
    if (pokemonTypes == NULL) // Memory allocation glitch
        printf("Memory Problem");

    while (fscanf(cFile, "%s", line) == 1) {
        if (lineCount == 1) {
            battle = createBattleByCategory(maxCapacityInType, numOfTypes, line, (equalFunction) comparePokemons,
                                            (copyFunction) copyPokemon, (freeFunction) freePokemon,
                                            (getCategoryFunction) getTypeName, (getAttackFunction) getAttack,
                                            (printFunction) print_pokemon);
            if (battle == NULL) {
                printf("No memory available.\n");
                break;
            }
            tempWord1 = strtok(line, ",");
            int i;
            for (i = 0; i < numOfTypes; i++) {
                pokemonTypes[i] = create_pokemon_type(tempWord1);
                if (pokemonTypes[i] == NULL) // Memory allocation glitch
                {
                    freePokedex(battle, i); // free all the types that created until now.
                    printf("No memory available.\n");
                }
                tempWord1 = strtok(NULL, ",");
            }
            lineCount++;
            continue;
        }
        //////////////////////////////////////////////////////////////////////////////////// add types to lists
        if (lineCount == 2) {
            while (strcmp(line, "Pokemons") != 0) {
                index1 = findType(line);
                fscanf(cFile, "%s", line);
                tempWord1 = strtok(line, ":");

                if (strcmp(tempWord1, me) == 0) {
                    tempWord2 = strtok(NULL, ",");
                    while (tempWord2 != NULL) {

                        index2 = findType(tempWord2);
                        add_effective_against_me_pokemon(pokemonTypes[index1], pokemonTypes[index2]);
                        tempWord2 = strtok(NULL, ",");
                    }
                } else {
                    tempWord2 = strtok(NULL, ",");
                    while (tempWord2 != NULL) {

                        index2 = findType(tempWord2);
                        add_effective_against_others_pokemon(pokemonTypes[index1], pokemonTypes[index2]);
                        tempWord2 = strtok(NULL, ",");
                    }
                }

                fscanf(cFile, "%s", line);
            }
            lineCount++;
        }
        ///////////////////////////////////////////////////////////////////// create pokemons

        if (lineCount > 2) {
            while (fscanf(cFile, "%s", line) == 1) {
                tempWord1 = strtok(line, ",");
                tempWord2 = strtok(NULL, ",");
                tempWord3 = strtok(NULL, ",");
                tempWord4 = strtok(NULL, ",");
                tempWord5 = strtok(NULL, ",");
                tempWord6 = strtok(NULL, ",");

                bioPtr = create_biological_information(atof(tempWord3), atof(tempWord4), atoi(tempWord5));
                if (bioPtr == NULL) // Memory allocation glitch
                {
                    freePokedex(battle, numOfTypes); // free all the pokemons and types that created until now.
                    printf("No memory available.\n");
                }

                int index = findType(tempWord6);
                if (index != -1) {
                    pokPtr = create_pokemon(tempWord1, tempWord2, bioPtr, pokemonTypes[index]);
                    if (pokPtr == NULL) // Memory allocation glitch
                    {
                        freePokedex(battle,
                                    numOfTypes); // free all the pokemons and types that created until now.
                        printf("No memory available.\n");
                    }
                    insertObject(battle, (element) pokPtr);
                    numOfPokemons++;
                }
            }
            break;
        }
        lineCount++;
    }

    fclose(cFile); /* close the file */
}

static void printScreen() {
    printf("Please choose one of the following numbers:\n");
    printf("1 : Print all Pokemons by types\n");
    printf("2 : Print all Pokemons types\n");
    printf("3 : Insert Pokemon to battles training camp\n");
    printf("4 : Remove strongest Pokemon by type\n");
    printf("5 : Fight\n");
    printf("6 : Exit\n");
}

// /////////////////////////////////////////////////// main function

int main(int argc, char *argv[]) {

    if (argc != 4)
        printf("Not enough arguments");

    numOfTypes = atoi(argv[1]);
    maxCapacityInType = atoi(argv[2]);

    ////////////////////////////////////////////// read the file
    if (numOfTypes > 0)
        readFile(argv[3]);


    status s;
    char input;
    char typeName[BUFSIZE], pokemonName[BUFSIZE], pokemonSpecies[BUFSIZE], pokemonHeight[BUFSIZE], pokemonWeight[BUFSIZE], pokemonAttack[BUFSIZE];
    int index;
    pokemon *pokemonPtr;
    bioInfo *bioPtr;

    do {
        ////////////////////////////////////////////////////////////////////////////////// cases
        printScreen();

        scanf(" %c", &input); // to "eat up" the trailing '\n'

        if ((input > '6') || (input < '1')) // check valid input
            printf("Please choose a valid number.\n");

        switch (input) {
            /////////////////////////////////////////////////////////////////////////////// case 1

            case '1': /* Print all Pokemons by types */
                displayObjectsByCategories(battle);
                break;

                /////////////////////////////////////////////////////////////////////////////// case 2

            case '2': /* Print all Pokemons types */

                printPokemonTypes();
                break;

                /////////////////////////////////////////////////////////////////////////////// case 3

            case '3': /* Insert Pokemon to battles training camp */
                printf("Please enter Pokemon type name:\n");
                scanf("%s", typeName);

                index = findType(typeName);
                if (index == -1) {
                    printf("Type name doesn't exist.\n");
                    break;
                }
                if (getNumberOfObjectsInCategory(battle, typeName) == maxCapacityInType) {
                    printf("Type at full capacity.\n");
                    break;
                }

                printf("Please enter Pokemon name:\n");
                scanf("%s", pokemonName);

                printf("Please enter Pokemon species:\n");
                scanf("%s", pokemonSpecies);

                printf("Please enter Pokemon height:\n");
                scanf("%s", pokemonHeight);

                printf("Please enter Pokemon weight:\n");
                scanf("%s", pokemonWeight);

                printf("Please enter Pokemon attack:\n");
                scanf("%s", pokemonAttack);

                bioPtr = create_biological_information(atof(pokemonHeight), atof(pokemonWeight), atoi(pokemonAttack));
                pokemonPtr = create_pokemon(pokemonName, pokemonSpecies, bioPtr, pokemonTypes[index]);

                s = insertObject(battle, (element) pokemonPtr);
                if (s == success) {
                    printf("The Pokemon was successfully added.\n");
                    print_pokemon((element) pokemonPtr);
                    numOfPokemons++;
                }

                //freePokemon(pokemonPtr);
                break;

                /////////////////////////////////////////////////////////////////////////////// case 4

            case '4': /* Remove strongest Pokemon by type */
                printf("Please enter type name:\n");
                scanf("%s", typeName);

                index = findType(typeName);
                if (index == -1) {
                    printf("Type name doesn't exist.\n");
                    break;
                }

                if (pokemonTypes[index]->numOfPokemons == 0) {
                    printf("There is no Pokemon to remove.\n");
                    break;
                }

                element e = removeMaxByCategory(battle, (element) pokemonTypes[index]->name);
                pokemonPtr = (pokemon *) e;
                if (pokemonPtr != NULL) {
                    printf("The strongest Pokemon was removed:\n");
                    pokemonTypes[index]->numOfPokemons--;
                    print_pokemon((element) pokemonPtr);
                    freePokemon((element) pokemonPtr);
                    numOfPokemons--;
                    break;
                }

                ///////////////////////////////////////////////////////////////////////////////  case 5

            case '5': /* Fight */
                printf("Please enter Pokemon type name:\n");
                scanf("%s", typeName);

                index = findType(typeName);
                if (index == -1) {
                    printf("Type name doesn't exist.\n");
                    break;
                }

                printf("Please enter Pokemon name:\n");
                scanf("%s", pokemonName);

                printf("Please enter Pokemon species:\n");
                scanf("%s", pokemonSpecies);

                printf("Please enter Pokemon height:\n");
                scanf("%s", pokemonHeight);

                printf("Please enter Pokemon weight:\n");
                scanf("%s", pokemonWeight);

                printf("Please enter Pokemon attack:\n");
                scanf("%s", pokemonAttack);

                bioPtr = create_biological_information(atof(pokemonHeight), atof(pokemonWeight), atoi(pokemonAttack));
                pokemonPtr = create_pokemon(pokemonName, pokemonSpecies, bioPtr, pokemonTypes[index]);
                pokemonTypes[index]->numOfPokemons--;

                pokemon *winner = (pokemon *) fight(battle, (element) pokemonPtr);
                if (winner == NULL)
                    printf("There are no Pokemons to fight against you.\n");

                freePokemon(pokemonPtr);
                break;

                ///////////////////////////////////////////////////////////////////////////////  case 6

            case '6':                                           /* Exit */
                freePokedex(battle, numOfTypes); // free all the pokemons and types that created.
                printf("All the memory cleaned and the program is safely closed.\n");
                break;
        }
    } while (input != '6');
}
