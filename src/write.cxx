#include "app.h"

int createPokemon(int id, string name){
    if(pokemon.find(id) != pokemon.end()){
        cout << "pokemon already registered";
        return;
    }

    pokemon[id] = name;
    cout << "pokemon registered !";
    return 0;
}