#include "Leaderboard.h"

// Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
// variable will point to the highest all-times score, and all other scores will be reachable from it
// via the "next_leaderboard_entry" member variable pointer.
void Leaderboard::read_from_file(const string &filename) {
    std::ifstream file(filename);

    if (!file) {
        return;
    }

    std::string line;
    LeaderboardEntry* last_entry = nullptr; // Keep track of the last entry for easy linking

    int counter = 1;
    while (std::getline(file, line) && counter <= 10) {
        unsigned long score;
        time_t time_stamp;
        std::string player_name;

        // Use stringstream to parse each line
        std::stringstream line_ss(line);
        if (!(line_ss >> score >> time_stamp >> player_name)) {
            std::cerr << "Error reading line: " << line << std::endl;
            continue; // Skip malformed lines
        }

        LeaderboardEntry* entry = new LeaderboardEntry(score, time_stamp, player_name);

        if (head_leaderboard_entry == nullptr) {
            head_leaderboard_entry = entry; // Set the head if it's the first entry
        } else {
            last_entry->next = entry; // Link the previous entry to the new one
        }

        last_entry = entry; // Update the last entry to the current one
        counter++;
    }
}


// Write the latest leaderboard status to the given file in the format specified in the PA instructions
void Leaderboard::write_to_file(const string &filename) {
    std::ofstream file(filename);

    if (!file) {
        std::cerr << "Unable to open file for writing: " << filename << std::endl;
        return;
    }

    LeaderboardEntry* ptr = head_leaderboard_entry;
    while (ptr != nullptr){
        file << ptr->score << " " << ptr->last_played << " " << ptr->player_name << std::endl;
        ptr = ptr->next;
    }
}

// Print the current leaderboard status to the standard output in the format specified in the PA instructions
void Leaderboard::print_leaderboard() {
    std::cout << "Leaderboard\n-----------\n";

    LeaderboardEntry* ptr = head_leaderboard_entry;
    int counter = 1;
    while (ptr != nullptr){
        std::string player_name = ptr->player_name;
        unsigned long score = ptr->score;
        time_t time_stamp = ptr->last_played;
        std::tm* tm_ptr = std::localtime(&time_stamp);
        std::cout << counter++ << ". " << player_name << " " << score << " " << std::put_time(tm_ptr, "%H:%M:%S/%d.%m.%Y") << std::endl; 
        ptr = ptr->next;
    }
}

//  Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
//  is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
//  top 10 all-time high-scores should be kept in descending order by the score).
void Leaderboard::insert(LeaderboardEntry *new_entry) {
    if (head_leaderboard_entry == nullptr || head_leaderboard_entry->score <= new_entry->score){
        new_entry->next = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
        return;
    }

    // traverse the linked list to find a number lower than new entry
    LeaderboardEntry* ptr = head_leaderboard_entry;
    while (ptr->next != nullptr && ptr->next->score >= new_entry->score){
        ptr = ptr->next;
    }
    new_entry->next = ptr->next;
    ptr->next = new_entry;

    // Ensure the leaderboard size does not exceed 10
    ptr = head_leaderboard_entry;
    int counter = 1;
    while (ptr != nullptr && counter < 10) {
        ptr = ptr->next;
        counter++;
    }

    // If there’s an 11th entry, delete it and all entries after it
    if (ptr != nullptr && ptr->next != nullptr) {
        LeaderboardEntry* temp = ptr->next;
        ptr->next = nullptr;  // Disconnect the list at the 10th entry

        // Delete the remaining entries starting from temp
        while (temp != nullptr) {
            LeaderboardEntry* to_delete = temp;
            temp = temp->next;
            delete to_delete;
        }
    }
}

// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard() {
    while (head_leaderboard_entry != nullptr){
        LeaderboardEntry* temp = head_leaderboard_entry;
        head_leaderboard_entry = head_leaderboard_entry->next;
        delete temp;
    }
    head_leaderboard_entry = nullptr; // Safeguard against dangling pointer
}
