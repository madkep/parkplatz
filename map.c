#include "map.h"

HashMap* createMap(){

    HashMap* map = (HashMap*)malloc(sizeof(HashMap));

    map->size = 0;
    map->capac = 1;

    map->hashArray = (hashElem**)calloc(2,sizeof(hashElem*));

    return map;
}

void insert(HashMap* map, long key, void* data){

    int pos;

    if((map->size*100)/map->capac > 70) enlarge(map);

    for(pos = hash(key,map->capac); ; pos++){ //se recorre el mapa hasta encontrar una casilla disponible

        if (map->hashArray[pos] == NULL){
            map->hashArray[pos] = (hashElem*)malloc(sizeof(hashElem));
            map->hashArray[pos]->key = key;
            map->hashArray[pos]->data = createList();  //se crea una lista en caso de que se deba almacenar mas de un elemento
            pushBack(map->hashArray[pos]->data, data);
            map->size++;
            return;
        }
        if (map->hashArray[pos]->key == key){  //llaves iguales, sirve para a�os y generos
            pushBack(map->hashArray[pos]->data, data);
            return;
        }

        if(pos == map->capac) pos = 1;  //llegando al final del mapa, se comienza de nuevo
    }
}

void delete(HashMap* map, long key){

    int pos = hash(key,map->capac), start = pos;
    hashElem* elem = map->hashArray[pos];
    if(elem->key == key){
        free(elem);
        map->hashArray[pos] = NULL;
    }

    for(pos++; pos != start ; pos++){
        elem = map->hashArray[pos];
        if(elem->key == key){
            free(elem);
            map->hashArray[pos] = NULL;
        }

        if(pos == map->capac) pos = 1;
    }
}

void* search(HashMap* map, long key){

    int pos = hash(key,map->capac), start = pos;
    hashElem* elem = map->hashArray[pos];
    if(elem == NULL) return NULL;
    if(elem->key == key) return elem->data;

    for(pos++; pos != start ; pos++){  //se recorre el mapa a partir de la posicion que indica el hash en busca de la llave
        elem = map->hashArray[pos];
        if(elem == NULL) return NULL;
        if (elem->key == key) return elem->data;

        if(pos == map->capac) pos = 1;
    }

    return NULL;
}

int hash(long key, int capac){

    double keyCpy = key;

    keyCpy *= 0.6180339;  //parte decimal del radio dorado
    keyCpy = keyCpy - (int)keyCpy;

    int pos = capac * keyCpy;
    if(pos) return pos;
    return 1;
}

void enlarge(HashMap* map){

    int i;
    hashElem** oldMap = map->hashArray;

    map->size = 0;
    map->capac *= 2;
    map->hashArray = (hashElem**)calloc(map->capac + 1,sizeof(hashElem*));

    for(i = 1; i <= (map->capac/2); i++){
        if(oldMap[i] != NULL){

            //se recorre la lista, ingresando cada elemento de ella en el mapa
            for(first(oldMap[i]->data); current(oldMap[i]->data) != NULL; popFront(oldMap[i]->data) ){
                insert(map,oldMap[i]->key,current(oldMap[i]->data));
            }

            free(oldMap[i]->data);  //se libera la memoria de la lista
            free(oldMap[i]);  //se libera la memoria del hashElem
        }
    }

    free(oldMap);
}
