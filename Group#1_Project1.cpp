#include <iostream>
#include <iomanip>
#include <cctype>
#include <limits>
#include <algorithm>
#include <string>
using namespace std;

class App { 
    public:
    string appName; // will be "facebook" "instagram"
    int appSize; // size of each app
};

class Smartphone {
    private:
    int capacity; // total storage space
    string name; // the user phone
    App* apps_array; // idk how many apps user will download.
    int numAppInst; // total apps currently installed

}
// constructor 
Smartphone(int cap, string nam){
    capacity = cap;
    name = nam;
    apps_array = nullptr;
    numAppInst = 0;
}
//Methods to prevent madness

bool has_app(string appName) // does the app already exists? true or false
void add_app(string appname, int appSize) // check size/space, check for dup
void remove_app(string appname) // deletes old app array
int get_available_space() // finds total space, calc remaining space.