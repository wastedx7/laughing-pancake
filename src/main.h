#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <memory>
#include <cstdlib>

using namespace std;

constexpr int MAX_ROWS = 100;

struct Address {
    int id = 0;
    bool set = false;
    string name;
    string email;
};

struct Database {
    array<Address, MAX_ROWS> rows;
};

