#include "AsteroidDash.h"


// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

        : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {

    read_player(player_file_name, player_name);  // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);  // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name);  // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    std::ifstream file(input_file);
    
    if (!file) {
        std::cerr << "Unable to open file: " << input_file << std::endl;
        return;
    }
    
    int value;
    std::vector<int> row;
    
    while (file >> value){
        row.push_back(value);
        
        if (file.peek() == '\n' || file.eof()) {
            space_grid.push_back(row);
            row.clear();
        }
    }
    
    file.close();
}

// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name) {
    std::ifstream file(player_file_name);

    if (!file) {
        std::cerr << "Unable to open file: " << player_file_name << std::endl;
        return;
    }

    int pos_row, pos_col;
    file >> pos_row;
    file >> pos_col;

    std::string line;
    std::getline(file, line);

    std::vector<std::vector<bool>> player_grid;
    std::vector<bool> row;
    while (std::getline(file, line)){
        for (char c : line){
            if (c == '1') row.push_back(true);
            else if (c == '0') row.push_back(false);
        }

        if (!row.empty()) {
            player_grid.push_back(row);
        }
    }

    player = new Player(player_grid, pos_row, pos_col, player_name);

    file.close();
}

// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const string &input_file) {
    std::ifstream file(input_file);

    if (!file) {
        std::cerr << "Unable to open file: " << input_file << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)){
        if (line.empty()){
            continue;
        }

        bool power_up = false;
        if (line[0] == '{'){
            power_up = true;
        }

        // set up the shape/sprite
        std::vector<std::vector<bool>> shape;
        do {
            std::vector<bool> row;
            for (char c : line){
                if (c == '1') row.push_back(true);
                else if (c == '0') row.push_back(false);
            }
            shape.push_back(row);

            if (line.find("]") != std::string::npos || line.find("}") != std::string::npos){
                break;
            }
        } while (std::getline(file,line));
        std::getline(file,line);

        // set up the starting row
        int starting_row = 0;
        if (line[0] == 's' && line[1] == ':'){
            starting_row = std::stoi(line.substr(2));
        }
        std::getline(file,line);

        // set up the tick
        int tick = 0;
        if (line[0] == 't' && line[1] == ':'){
            tick = std::stoi(line.substr(2));
        }
        std::getline(file,line);

        // set up the power_up type
        ObjectType type = ASTEROID;
        if (power_up){
            if (line[0] == 'e' && line[1] == ':'){
                std::string type_string = line.substr(2);
                if (type_string == "life"){
                    type = LIFE_UP;
                }
                else if (type_string == "ammo"){
                    type = AMMO;
                }
            }
        }
        std::getline(file,line);

        // create object
        CelestialObject* object = new CelestialObject(shape, type, starting_row, tick);

        // link to the linked list
        if (celestial_objects_list_head == nullptr){
            celestial_objects_list_head = object;
        }
        else {
            CelestialObject* ptr = celestial_objects_list_head;
            while (ptr->next_celestial_object != nullptr){
                ptr = ptr->next_celestial_object;
            }
            ptr->next_celestial_object = object;
        }
    }
    file.close();
}


// Print the entire space grid
void AsteroidDash::print_space_grid() const {
    // TODO: Your code here
}


// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {
    // TODO: Your code here
}

// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot() {
    // TODO: Your code here
}

// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {
    delete player;
}