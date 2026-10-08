#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <stdexcept>
using namespace std;

bool has_extension(const string&);
bool ifContainsDigit(const string&);
bool isNumberCorrect(const string&);
void show_menu();
bool has_space(const string&);
int getOption();

class Person {
public:
    string first_name;
    string surname;
    int score;

    Person() : score(0) {}

    // Parse text data into a Person object
    static Person getData(const string& text) {
        Person person;
        vector<string> components;
        string word;

        // Traverse the string and split it by spaces
        for (char ch : text) {
            if (ch == ' ') {
                if (!word.empty()) {
                    components.push_back(word);
                    word = "";
                }
            } else {
                word += ch;
            }
        }
        // Add the last word (after the final space)
        if (!word.empty()) {
            components.push_back(word);
        }

        // Check if there are exactly 3 components: first name, surname, and score
        if (components.size() != 3) {
            throw runtime_error("Enter name, surname and score in one line separated by space ");
        }

        string first_name = components[static_cast<vector<string>::size_type>(0)];
        string surname = components[static_cast<vector<string>::size_type>(1)];
        string score_str = components[static_cast<vector<string>::size_type>(2)];

        // Ensure first_name and surname don't contain digits
        if (ifContainsDigit(first_name) || ifContainsDigit(surname)) {
            throw runtime_error("Invalid name or surname format.");
        }

        // Ensure score is a valid positive number
        if (!isNumberCorrect(score_str)) {
            throw runtime_error("Invalid score format. Enter a positive number.");
        }

        // Assign values to the Person object
        person.first_name = first_name;
        person.surname = surname;
        try {
            person.score = stoi(score_str);
        } catch (const out_of_range&) {
            throw runtime_error("Score is too large.");
        }

        return person;
    }


    // Save all people to a file
    static void savePeopleToFile(const vector<Person>& people, const string& executable_directory) {

        // Obtain and prepare file name to open
        string file_name;
        cout << "Enter file name to save the data: ";
        getline(cin, file_name);
        if (!has_extension(file_name)) {
            file_name += ".txt";
        }

        // Save relative file names next to the executable.
        string output_path = file_name;
        bool is_absolute_path = !file_name.empty() &&
            (file_name[0] == '/' || file_name.find(':') != string::npos);
        if (!is_absolute_path && !executable_directory.empty()) {
            output_path = executable_directory + "/" + file_name;
        }

        // Open file and save data
        ofstream outfile(output_path.c_str());
        if (outfile.is_open()) {
            for (const auto& person : people) {
                outfile << person.first_name << " " << person.surname << " " << person.score << endl;
            }
            outfile.close();
            cout << "Data saved to " << output_path << endl;
        } else {
            throw runtime_error("Error while saving the file!\n");
        }
    }


    // Method to change a person's details
    static void changePerson(vector<Person>& people) {

        // Ensure that there is data to modify
        if (people.empty())
            throw runtime_error("No objects to modify.");

        // Obtain index
        string input;
        cout << "Enter the index of the person you want to change (0 to " << people.size() - 1 << "): ";
        getline(cin, input);

        // Check if input is valid
        if (!isNumberCorrect(input))
            throw runtime_error("Entered index is invalid.");
        size_t parsed_length = 0;
        unsigned long long index_value;
        try {
            index_value = stoull(input, &parsed_length);
        } catch (const out_of_range&) {
            throw runtime_error("Entered index is invalid.");
        }
        if (parsed_length != input.length() || index_value >= people.size()) {
            throw runtime_error("Entered index is invalid.");
        }
        size_t index = static_cast<size_t>(index_value);

        // Obtain new person's data
        string new_value;
        cout << "Enter new data separated by spaces: FirstName Surname Score\n";
        getline(cin, new_value);
        if(!new_value.empty() && new_value != "\n") {
            people[index] = getData(new_value);
            cout << "Person updated successfully!" << endl;
        }
        else
            throw runtime_error("no data was modified\n");
    }

    // Method to add a new person
    static void addPerson(vector<Person>& people) {

        // Obtain new person's data
        string new_value;
        cout << "Enter data separated by spaces: FirstName Surname Score\n  ";
        getline(cin, new_value);

        // If valid save as last element
        if(!new_value.empty() && new_value != "\n") {
            Person new_person = getData(new_value);
            people.push_back(new_person);
            cout << "Person updated successfully!" << endl;
        }
        else
            throw runtime_error("Could not add person: incorrect data format.\n");
    }

    // Method to delete a person
    static void deletePerson(vector<Person>& people) {

        // Ensure that there is data to delete
        if (people.empty())
            throw runtime_error("No objects to delete since the list is empty.");

        // Obtain index
        string input;
        cout << "Enter the index of the person you want to delete (0 to " << people.size() - 1 << "): ";
        getline(cin, input);

        // Check if input is valid
        if (!isNumberCorrect(input))
            throw runtime_error("Entered index is invalid.");
        size_t parsed_length = 0;
        unsigned long long index_value;
        try {
            index_value = stoull(input, &parsed_length);
        } catch (const out_of_range&) {
            throw runtime_error("Entered index is invalid.");
        }
        if (parsed_length != input.length() || index_value >= people.size()) {
            throw runtime_error("Entered index is invalid.");
        }
        size_t index = static_cast<size_t>(index_value);

        // Delete person
        people.erase(people.begin() + index);
        cout << "Person deleted successfully!" << endl;
    }

};

int menuDriver(vector<Person>&, const string&);
int openFile(vector<Person>&, const string&);
void printArray(const vector<Person>&);

int main(int argc, char* argv[]) {

    // create vector of class Person
    vector<Person> people;
    string executable_directory;
    if (argc > 0) {
        string executable_path = argv[0];
        string::size_type separator = executable_path.find_last_of("/\\");
        if (separator != string::npos) {
            executable_directory = executable_path.substr(0, separator);
        }
    }

    if(openFile(people, executable_directory)) {
        return 0;
    }

    menuDriver(people, executable_directory);

    return 0;
}

bool has_extension(const string& name) {
    const string extension = ".txt";
    if (name.length() < extension.length()) {
        return false;
    }
    const size_t start = name.length() - extension.length();
    for (size_t i = start; i < name.length(); ++i) {
        if (tolower(static_cast<unsigned char>(name[i])) != extension[i - start]) {
            return false;
        }
    }
    return true;
}

bool has_space(const string& line) {
    return (line.find(' ') != string::npos);
}

bool ifContainsDigit(const string& str) {
    for (string::size_type i = 0; i < str.length(); i++) {
        if (isdigit(static_cast<unsigned char>(str[i])) || str[i] == ' ')
            return true;
    }
    return false;
}

bool isNumberCorrect(const string& str) {
    if(str.empty())
        return false;
    for (char character : str) {
        if (!isdigit(static_cast<unsigned char>(character)))
            return false;
    }
    if (str[0] == '0' && str.length()>1)
        return false;
    return true;
}

void show_menu() {
    cout<<'\n';
    cout << "Enter a digit to choose an operation:\n";
    cout << "0. exit\n";
    cout << "1. change values of an object\n";
    cout << "2. add a new object\n";
    cout << "3. delete an object\n";
    cout << "4. print data\n";
}


int getOption() {
    string input;
    int option = -1;

    // Obtain option
    while (true) {
        getline(cin, input);

        // Check if safe to convert to int
        if (!isNumberCorrect(input) || input.length()!=1) {
            throw runtime_error("Please enter a digit between 0 and 4.");
        }

        option = stoi(input);

        // Ensure the input is within the valid range (0-3)
        if (option >= 0 && option <= 4) {
            return option;
        } else {
            throw runtime_error("Please enter a digit between 0 and 4.");
        }
    }
}

// Function to operate the menu
int menuDriver(vector<Person>& people, const string& executable_directory) {
    printArray(people);
    while (true) {
        show_menu();  // Show available options to the user
        try {
            bool modified = false;
            switch (getOption()) {
                case 0:
                    Person::savePeopleToFile(people, executable_directory);
                    return 0;  // Exit the program
                case 1:
                    Person::changePerson(people);
                    modified = true;
                break;
                case 2:
                    Person::addPerson(people);
                    modified = true;
                break;
                case 3:
                    Person::deletePerson(people);
                    modified = true;
                break;
                case 4:
                    printArray(people);
                break;
                default:
                    cout << "Invalid option. Please choose a valid option." << endl;
                break;
            }
            if (modified) {
                printArray(people);
                cout << "Do you want to save modified data to file? Enter (y/n)\n";
                string answer;
                while (true) {
                    getline(cin, answer);
                    if (answer == "y") {
                        Person::savePeopleToFile(people, executable_directory);
                        return 0;
                    }
                    if (answer == "n") {
                        break;
                    }
                    cout << "Incorrect format. Enter y for YES, n for NO\n";
                }
            }

        }
        catch (const runtime_error& e) {
            cout << "Exception: " << e.what() << endl;
        }
    }
}

int openFile(vector<Person>& people, const string& executable_directory) {
    bool flag = true;
    while(flag) {
        // obtain file name
        string file_name;
        cout << "Enter file name you want to open" << endl;
        getline(cin, file_name);

        // prepare for opening
        if (!has_extension(file_name))
            file_name += ".txt";

        // Open relative to the current directory first, then relative to the executable.
        ifstream file;
        file.open(file_name);
        if (!file.is_open() && !executable_directory.empty()) {
            string executable_file = executable_directory + "/" + file_name;
            file.open(executable_file.c_str());
            if (file.is_open()) {
                file_name = executable_file;
            }
        }


        // load data
        if (file.is_open()) {
            string line;
            int line_number = 0;
            while (getline(file, line)) {
                line_number++;
                try {
                    people.push_back(Person::getData(line));

                }
                catch (const runtime_error& e) {
                    cout << "Error in line " << line_number << ": " << e.what() << endl;
                }
            }
            if (line_number == 0) {
                cout << "file named " << file_name << " is empty\n" << endl;
            }
            file.close();
            flag = false;
        } else {
            cout << "Unable to open file" << endl;
            cout << "Do you want to try again? (y/n)\ny - to input another filename, n - exit program\n";
            string answer;
            while(true) {
                getline(cin, answer);
                if (answer == "y") {
                    break;
                }
                if (answer == "n") {
                    return 1;
                }
                cout << "Incorrect format. Enter y for YES, n for NO\n";
            }

        }

    }

    return 0;
}

void printArray(const vector<Person>& people) {
    cout << endl;
    for (size_t i = 0; i < people.size(); i++) {
        cout << i << ": "<< people[i].first_name << " " << people[i].surname << " " << people[i].score << endl;
    }
    cout << endl;
}