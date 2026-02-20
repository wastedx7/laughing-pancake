#include "main.h"

class Connection{
    public:
        Connection(const string &filename, char mode){
            db = make_unique<Database>();

            if (mode = 'c'){
                file.open(filename, ios::out | ios::trunc | ios::binary);
                if(!file) throw runtime_error("failed to create database");
            } else {
                file.open(filename, ios::in | ios::out | ios::binary);
                if (!file) throw runtime_error("failed to load database");
                load();
            }
        }
};