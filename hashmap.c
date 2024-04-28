#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value){
  long posicion = hash(key, map->capacity);
  long posicionOriginal = posicion;

  while(map->buckets[posicion]!= NULL && map->buckets[posicion]->key != NULL)
  {
    if(strcmp(map->buckets[posicion]->key, key) == 0)
    {
      return;
    }
    posicion = (posicion + 1) % map->capacity;
    if(posicion == posicionOriginal)
    {
      return;
    } 
  }
  map->buckets[posicion] = createPair(key,value);
  map->current = posicion;

  map->size++;
}


void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  Pair** old = map->buckets;

  map->capacity *= 2;
  map ->buckets = (Pair**)malloc(map->capacity * sizeof(Pair*));
  map->size = 0;

  for(long i = 0; i < map->capacity / 2; i++)
  {
    if(old[i] != NULL)
    {
      insertMap(map, old[i]->key, old[i]->value);
      free(old[i]);
    }
  }
  free(old);
}

HashMap * createMap(long capacity){
  HashMap *mapa = (HashMap*)malloc(sizeof(HashMap));
  if(mapa == NULL)
  {
    free(mapa);
    return NULL;
  }
  mapa->buckets = (Pair**)calloc(capacity, sizeof(Pair*));
  if(mapa->buckets == NULL)
  {
    free(mapa);
    return NULL;
  }
  mapa->size = 0;
  mapa->current = -1;
  mapa->capacity = capacity;

  return mapa;
}

void eraseMap(HashMap * map, char * key) 
{
    if (map == NULL || key == NULL) // Verificar si el puntero al mapa o la clave es nulo
        return;

    long posicion = hash(key, map->capacity);
    long posicionOriginal = posicion;

    while (map->buckets[posicion] != NULL) // Corregir la condición del bucle
    {
        if (strcmp(map->buckets[posicion]->key, key) == 0)
        {
            free(map->buckets[posicion]); // Liberar memoria del par clave-valor eliminado
            map->buckets[posicion] = NULL;
            map->size--;
            return;
        }

        posicion = (posicion + 1) % map->capacity;

        if (posicion == posicionOriginal)
        {
            // Si hemos vuelto al inicio, significa que hemos recorrido todo el mapa sin encontrar la clave
            return;
        }
    }
}

Pair * searchMap(HashMap * map,  char * key) 
{
  long posicion = hash(key, map->capacity);
  long posicionOriginal = posicion;
  while(map->buckets[posicion]!= NULL && map->buckets)
    {
      if(strcmp(map->buckets[posicion]->key, key) == 0)
      {
        map->current = posicion;
        return map->buckets[posicion];
        
      }
      posicion = (posicion + 1) % map->capacity;
      if(posicion == posicionOriginal)
      {
        return NULL;
      }
    }
  return NULL;
}

Pair * firstMap(HashMap * map) 
{
    if (map == NULL) return NULL;

    for (int i = 0; i < map->capacity; i++)
    {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) // Verificar si el par no es nulo y tiene una clave válida
        {
            map->current = i; // Asignación solo cuando se encuentra el primer bucket no nulo
            return map->buckets[i];
        }
    }

    // Si no se encuentra ningún par no nulo, se devuelve NULL
    return NULL;
}

Pair * nextMap(HashMap * map) 
{  
  if(map==NULL) return NULL;
  for(int i=map->current+1;i<map->capacity;i++)
    {
      if(map->buckets[i]!=NULL)
      {
        map->current=i;
        return map->buckets[i];
        
      }
    }
  return NULL;
}
