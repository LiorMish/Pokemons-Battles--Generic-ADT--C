#include "Pokemon.h"
#include "Defs.h"
#include <stdio.h>
#include <string.h>

/////////////////////////////////////////////////// Function declaration

static int isTypeExistInList(type **, char *, int);

/////////////////////////////////////////////////// create structs functions

type *create_pokemon_type(char *name)
{
    type *init = (type *)malloc(sizeof(type));
    if (init == NULL || name == NULL)
        return NULL;
    init->name = (char *)malloc(sizeof(char) * strlen(name) + 1);
    if (init->name == NULL)
        return NULL;
    strcpy(init->name, name);
    init->effective_against_me = NULL;
    init->effective_against_others = NULL;
    init->numOfPokemons = 0;
    init->effective_against_me_len = 0;
    init->effective_against_others_len = 0;
    return init;
}

bioInfo *create_biological_information(double height, double weight, int attack)
{
    bioInfo *init = (bioInfo *)malloc(sizeof(bioInfo));
    if (init == NULL)
    {
        return NULL;
    }
    init->weight = weight;
    init->height = height;
    init->attack = attack;
    return init;
}

pokemon *create_pokemon(char *name, char *species, bioInfo *bio, type *type)
{
    pokemon *p = (pokemon *)malloc(sizeof(pokemon));
    if (name == NULL || species == NULL || p == NULL || bio == NULL)
    {
        return NULL;
    }
    p->name = (char *)malloc(sizeof(char) * strlen(name) + 1);
    if (p->name == NULL)
        return NULL;
    p->species = (char *)malloc(sizeof(char) * strlen(species) + 1);
    if (p->species == NULL)
        return NULL;

    strcpy(p->name, name);
    strcpy(p->species, species);
    p->bio = bio;
    p->type = type;
    type->numOfPokemons++;
    return p;
}

/////////////////////////////////////////////////// add to lists functions

status add_effective_against_me_pokemon(type *a, type *b)
{
    if (a == NULL || b == NULL)
        return failure;

    int index = isTypeExistInList(a->effective_against_me, b->name, a->effective_against_me_len);
    if (index != -1)
        return exist;

    a->effective_against_me = (type **)realloc(a->effective_against_me, (a->effective_against_me_len + 1) * sizeof(type *));
    a->effective_against_me[a->effective_against_me_len] = b;
    a->effective_against_me_len++;
    return success;
}

status add_effective_against_others_pokemon(type *a, type *b)
{
    if (a == NULL || b == NULL)
        return failure;

    int index = isTypeExistInList(a->effective_against_others, b->name, a->effective_against_others_len);
    if (index != -1)
        return exist;

    a->effective_against_others = (type **)realloc(a->effective_against_others, (a->effective_against_others_len + 1) * sizeof(type *));
    a->effective_against_others[a->effective_against_others_len] = b;
    a->effective_against_others_len++;
    return success;
}

/////////////////////////////////////////////////// delete from lists functions

status delete_effective_against_me_pokemon(type *a, char *b)
{
    if (a == NULL || b == NULL)
        return failure;

    int index = isTypeExistInList(a->effective_against_me, b, a->effective_against_me_len);
    if (index == -1)
        return not_exist;

    int j;
    for (j = index + 1; j < a->effective_against_me_len; j++)
    {
        a->effective_against_me[j - 1] = a->effective_against_me[j];
    }
    a->effective_against_me = (type **)realloc(a->effective_against_me, (a->effective_against_me_len - 1) * sizeof(type *));
    a->effective_against_me_len--;

    return success;
}

status delete_effective_against_others_pokemon(type *a, char *b)
{
    if (a == NULL || b == NULL)
        return failure;

    int index = isTypeExistInList(a->effective_against_others, b, a->effective_against_others_len);
    if (index == -1)
        return not_exist;

    int j;
    for (j = index + 1; j < a->effective_against_others_len; j++)
    {
        a->effective_against_others[j - 1] = a->effective_against_others[j];
    }
    a->effective_against_others = (type **)realloc(a->effective_against_others, (a->effective_against_others_len - 1) * sizeof(type *));
    a->effective_against_others_len--;

    return success;
}

/////////////////////////////////////////////////// print functions

status print_pokemon_type(type *t)
{
    if (t == NULL)
    {
        return failure;
    }
    printf("Type %s -- %d pokemons", t->name, t->numOfPokemons);
    if (t->effective_against_me_len > 0)
    {
        printf("\n");
        printf("\tThese types are super-effective against %s:", t->name);

        int i;
        for (i = 0; i < t->effective_against_me_len - 1; i++)
        {
            printf("%s ,", t->effective_against_me[i]->name);
        }
        printf("%s", t->effective_against_me[i]->name);
    }
    if (t->effective_against_others_len > 0)
    {
        printf("\n");
        printf("\t%s moves are super-effective against:", t->name);
        int i;
        for (i = 0; i < t->effective_against_others_len - 1; i++)
        {
            printf("%s ,", t->effective_against_others[i]->name);
        }
        printf("%s", t->effective_against_others[i]->name);
    }
    printf("\n\n");
    return success;
}

/////////////////////////////////////////////////// static functions

static int isTypeExistInList(type **list, char *name, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        if (strcmp(list[i]->name, name) == 0)
            return i;
    }
    return -1;
}

/////////////////////////////////////////////////// free functions

void freeBio(bioInfo *bio)
{
    free(bio);
}


void freeType(type *type)
{
    free(type->effective_against_me);
    free(type->effective_against_others);
    free(type->name);
    free(type);
}

/////////////////////////////////////////////////// part 2 functions
status freePokemon(pokemon *p)
{
    if (p == NULL)
        return failure;
    free(p->name);
    free(p->species);
    freeBio(p->bio);
    free(p);

    return success;
}

status print_pokemon(pokemon* p)
{
    if (p == NULL)
        return failure;

    printf("%s :\n", p->name);
    printf("%s, %s Type.\n", p->species, p->type->name);
    printf("Height: %.2f m    Weight: %.2f kg    Attack: %d", p->bio->height, p->bio->weight, p->bio->attack);
    printf("\n\n");

    return success;
}

pokemon* copyPokemon(pokemon* p)
{
    if (p == NULL)
        return NULL;

    pokemon *copiedPokemon = (pokemon *)malloc(sizeof(pokemon));
    if (copiedPokemon == NULL)
        return NULL;

    copiedPokemon->name = (char *)malloc(sizeof(char) * strlen(p->name) + 1);
    if (copiedPokemon->name == NULL)
        return NULL;
    copiedPokemon->species = (char *)malloc(sizeof(char) * strlen(p->species) + 1);
    if (copiedPokemon->species == NULL)
        return NULL;

    strcpy(copiedPokemon->name, p->name);
    strcpy(copiedPokemon->species, p->species);
    copiedPokemon->bio = create_biological_information(p->bio->height, p->bio->weight, p->bio->attack);
    if (copiedPokemon->bio == NULL)
        return NULL;

    copiedPokemon->type = p->type;

    return copiedPokemon;
}

char *getTypeName(pokemon* pok)
{
    if (pok == NULL)
        return NULL;

    return pok->type->name;
}

int comparePokemons(pokemon* pok1, pokemon* pok2)
{
    int attack1 = pok1->bio->attack;
    int attack2 = pok2->bio->attack;

    if (attack1 > attack2)
        return 1;
    if (attack2 > attack1)
        return -1;
    return 0;
}

int getAttack(element elem1, element elem2, int* attack1Ptr, int* attack2Ptr){
    pokemon *p1 = (pokemon *)elem1;
    pokemon *p2 = (pokemon *)elem2;

    int attack1 = p1->bio->attack;
    int attack2 = p2->bio->attack;

    // check if pokemon 2 effective against pokemon 1
    int effectiveAgainstMeIndex = isTypeExistInList(p1->type->effective_against_me, p2->type->name, p1->type->effective_against_me_len);
    int effectiveAgainstOthersIndex = isTypeExistInList(p2->type->effective_against_others, p1->type->name, p2->type->effective_against_others_len);

    if (effectiveAgainstMeIndex != -1 || effectiveAgainstOthersIndex != -1)
        attack1 = attack1 - 10;

    // check if pokemon 1 effective against pokemon 2
    effectiveAgainstMeIndex = isTypeExistInList(p2->type->effective_against_me, p1->type->name, p2->type->effective_against_me_len);
    effectiveAgainstOthersIndex = isTypeExistInList(p1->type->effective_against_others, p2->type->name, p1->type->effective_against_others_len);

    if (effectiveAgainstMeIndex != -1 || effectiveAgainstOthersIndex != -1)
        attack2 = attack2 - 10;

    *attack1Ptr = attack1;
    *attack2Ptr = attack2;

    return attack1 - attack2;

}
