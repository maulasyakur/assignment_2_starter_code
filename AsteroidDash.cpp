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
    // std::ifstream file(input_file);

    // if (!file) {
    //     std::cerr << "Unable to open file: " << input_file << std::endl;
    //     return;
    // }

    // std::string line;
    // while (std::getline(file, line)){
    //     if (line.empty()){
    //         continue;
    //     }

    //     bool power_up = false;
    //     if (line[0] == '{'){
    //         power_up = true;
    //     }

    //     // set up the shape/sprite
    //     std::vector<std::vector<bool>> shape;
    //     do {
    //         std::vector<bool> row;
    //         for (char c : line){
    //             if (c == '1') row.push_back(true);
    //             else if (c == '0') row.push_back(false);
    //         }
    //         shape.push_back(row);

    //         if (line.find("]") != std::string::npos || line.find("}") != std::string::npos){
    //             break;
    //         }
    //     } while (std::getline(file,line));
    //     std::getline(file,line);

    //     // set up the starting row
    //     int starting_row = 0;
    //     if (line[0] == 's' && line[1] == ':'){
    //         starting_row = std::stoi(line.substr(2));
    //     }
    //     std::getline(file,line);

    //     // set up the tick
    //     int tick = 0;
    //     if (line[0] == 't' && line[1] == ':'){
    //         tick = std::stoi(line.substr(2));
    //     }
    //     std::getline(file,line);

    //     // set up the power_up type
    //     ObjectType type = ASTEROID;
    //     if (power_up){
    //         if (line[0] == 'e' && line[1] == ':'){
    //             std::string type_string = line.substr(2);
    //             if (type_string == "life"){
    //                 type = LIFE_UP;
    //             }
    //             else if (type_string == "ammo"){
    //                 type = AMMO;
    //             }
    //         }
    //     }
    //     std::getline(file,line);

    //     // create object
    //     CelestialObject* object = new CelestialObject(shape, type, starting_row, tick);

    //     // create rotation doubly circular linked list
    //     CelestialObject* ptr_prev = object;
    //     std::vector<std::vector<bool>>* ptr_prev_shape = &ptr_prev->shape;
    //     for (int delta = 90; delta < 360; delta += 90){
    //         // rotate the shape
    //         int row_size_prev = ptr_prev_shape->size();
    //         int col_size_prev = (*ptr_prev_shape)[0].size();
    //         std::vector<std::vector<bool>>* ptr_rotated_shape;
    //         if (delta == 90){
    //             ptr_rotated_shape = new std::vector<std::vector<bool>>(col_size_prev, std::vector<bool>(row_size_prev));
    //             for (int i = 0; i < row_size_prev; i++){
    //                 for (int j = 0; j < col_size_prev; j++){
    //                     (*ptr_rotated_shape)[j][row_size_prev - 1 - i] = (*ptr_prev_shape)[i][j];
    //                 }
    //             }
    //         }
    //         else if (delta == 180){
    //             ptr_rotated_shape = new std::vector<std::vector<bool>>(row_size_prev, std::vector<bool>(col_size_prev));
    //             for (int i = 0; i < row_size_prev; i++){
    //                 for (int j = 0; j < col_size_prev; j++){
    //                     (*ptr_rotated_shape)[row_size_prev - 1 - i][col_size_prev - 1 - j] = (*ptr_prev_shape)[i][j];
    //                 }
    //             }
    //         }
    //         else if (delta == 270){
    //             ptr_rotated_shape = new std::vector<std::vector<bool>>(col_size_prev, std::vector<bool>(row_size_prev));
    //             for (int i = 0; i < row_size_prev; i++){
    //                 for (int j = 0; j < col_size_prev; j++){
    //                     (*ptr_rotated_shape)[col_size_prev - 1 - j][i] = (*ptr_prev_shape)[i][j];
    //                 }
    //             }
    //         }

    //         // compare the rotated shape to the other shapes in the linked list
    //         int row_size_rotated = (*ptr_rotated_shape).size();
    //         int col_size_rotated = (*ptr_rotated_shape)[0].size();
    //         bool is_identical = false;
    //         CelestialObject* ptr_check = object;
    //         do{
    //             row_size_prev = ptr_check->shape.size();
    //             col_size_prev = ptr_check->shape[0].size();
    //             if (row_size_prev == row_size_rotated && col_size_prev == col_size_rotated){
    //                 is_identical = true;
    //                 for (int i = 0; i < row_size_prev; i++){
    //                     for (int j = 0; j < col_size_prev; j++){
    //                         if ((ptr_check->shape)[i][j] != (*ptr_rotated_shape)[i][j]){
    //                             is_identical = false;
    //                             break;
    //                         }
    //                     }
    //                     if (!is_identical) break;
    //                 }
    //             }

    //             ptr_check = ptr_check->right_rotation;

    //         } while(ptr_check != nullptr && !is_identical);

    //         // if there is an identical shaped rotation, then skip this one rotation
    //         if (is_identical) continue;
            
    //         CelestialObject* rotated_object = new CelestialObject(*ptr_rotated_shape, type, starting_row, tick);
    //         ptr_prev->right_rotation = rotated_object;
    //         rotated_object->left_rotation = ptr_prev;
    //         ptr_prev = rotated_object;
    //     }

    //     // Complete the circular linking
    //     object->left_rotation = ptr_prev;
    //     ptr_prev->right_rotation = object;

    //     // link to the linked list
    //     if (celestial_objects_list_head == nullptr){
    //         celestial_objects_list_head = object;
    //     }
    //     else {
    //         CelestialObject* ptr = celestial_objects_list_head;
    //         while (ptr->next_celestial_object != nullptr){
    //             ptr = ptr->next_celestial_object;
    //         }

    //         do{
    //             ptr->next_celestial_object = object;
    //             ptr = ptr->right_rotation;
    //         } while (ptr->next_celestial_object != object);
    //     }
    // }
    // file.close();
}


// Print the entire space grid
void AsteroidDash::print_space_grid() const {
    // TODO: Your code here
}


// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {
    // update player
    for (int i = 0; i < player->spacecraft_shape.size(); i++){
        for (int j = 0; j < player->spacecraft_shape[0].size(); j++){
            int grid_row = player->position_row + i;
            int grid_col = player->position_col + j;
            
            // Check boundaries of space_grid
            if (grid_row >= 0 && grid_row < space_grid.size() &&
                grid_col >= 0 && grid_col < space_grid[0].size()) {
                space_grid[grid_row][grid_col] = player->spacecraft_shape[i][j];
            }
        }
    }

    // update celestial object
    // for (int i = 0; i < celestial_objects_list_head->shape.size(); i++){
    //     for (int j = 0; j < celestial_objects_list_head->shape[0].size(); j++){
    //         int grid_row = celestial_objects_list_head->starting_row;

    //     }
    // }
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

    while (celestial_objects_list_head != nullptr) {
        CelestialObject::delete_rotations(celestial_objects_list_head);
        CelestialObject* new_head = celestial_objects_list_head->next_celestial_object;
        delete celestial_objects_list_head;
        celestial_objects_list_head = new_head;
    }
}