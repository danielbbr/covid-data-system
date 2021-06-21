#include <iostream>
#include <string>   
using namespace std;

class Region {
    private:
        string name;
    public:
        string get_region_name() { return name; }
        Region(string name): name(name) {}
};

class State {
    private:
        Region region;
        int uf;
        string name;

    public:
        int get_state_uf() { return uf; }
        string get_state_name() { return name; }
        Region get_state_region() { return region; }
        State(int uf, string name, Region region): uf(uf), name(name), region(region) {}
};

class City {
    private:
        State state;
        string name;
    public:
        State get_city_state() { return state; }
        string get_city_name() { return name; }
        City(State state, string name): state(state), name(name) {}

};

int main() {
    Region r("Sudeste");
    State s (33, "Rio de Janeiro", r);
    City c (s, "Rio de Janeiro");
    cout << c.get_city_state().get_state_uf() << endl;
}