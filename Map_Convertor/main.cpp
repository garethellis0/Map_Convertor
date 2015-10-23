/*This Program Converts a given text file based map to a vector of 1's and 0's,
with 1's representing obstacles and 0's representing open space*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <istream>
#include <sstream>
#include <sys/stat.h>

//Prints all elements in a vector<char>
void printCharVector(std::vector<char> char_vector) {
	for (int i = 0; i < char_vector.size(); i++) {
		std::cout << char_vector[i] << std::endl;
	}
}


//Get the width from the user
int getWidth() {
	int width;

	std::cout << "Please enter the width: ";
	while (!(std::cin >> width)) {
		std::cout << "Invalid input, please enter the width as an integer: ";
		std::cin >> width;
	}

	return width;
}


//Checks if a a given file exists
bool fileExists(const std::string& filename)
{
	struct stat buf;
	if (stat(filename.c_str(), &buf) != -1)
	{
		return true;
	}
	return false;
}


// vector<string> -> vector<char>
// Converts a vector of strings to a vector of all characters in all strings of given vector
// (ie. ["hello", "world"] -> ['h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd'])
std::vector<char> stringVectorToCharVector(std::vector<std::string> string_vector) {
	std::vector<char> char_vector;

	for (int i = 0; i < string_vector.size(); i++) {
		for (int j = 0; j < string_vector[i].length(); j++) {
			char_vector.push_back(string_vector[i][j]);
		}
	}

	return char_vector;
}


//Gets which characters represent open space from the user
std::vector<char> getOpenChars() {
	std::vector<char> open_space_chars; // A vector of all chars that represent open space
	std::string raw_open_space_chars; // Raw input from the user (should be a comma seperated list of characters)
	std::string present_char;  // The character presently being considered

	//Get the raw list of open characters from the user
	std::cout << "Please Enter all characters that represent open space in a comma seperated list (NO SPACES): ";
	std::cin >> raw_open_space_chars;

	std::stringstream ss(raw_open_space_chars);

	char i;

	while (ss >> i) {
		open_space_chars.push_back(i);

		if (ss.peek() == ',')
			ss.ignore();
	}

	return open_space_chars;
}


// String -> String
//Gets a file name from the user (asks for one with a description given to it)
//!!!
std::string getFileNameOf(std::string file_description) {
	std::string file_name;

	std::cout << "Please enter the full name of " << file_description << ": ";
	std::cin >> file_name;

	return file_name;
}


// String -> Vector<Char>
//Returns all characters from a file in a vector<char>
std::vector<char> getCharactersFromFile(std::string file_name) {
	std::string present_line; //The present line being considered
	std::vector<std::string> lines; // All lines in the given file

	//Gets all the lines from the file and adds them to a vector<string>
	std::ifstream file;
	
	//Initially try to open the file
	file.open(file_name);

	//If file could not be opened, keep prompting the user to enter a new file,
	// until a given file can be found
	while (!(file.is_open())) {
		std::cout << "File not found, please enter another file name: ";
		std::cin >> file_name;
		file.open(file_name);
	} 

	std::cout << "Converting... \n";

	while (std::getline(file, present_line)) {
		lines.push_back(present_line);
	}

	file.close();

	std::vector<char> all_characters = stringVectorToCharVector(lines);
	
	return all_characters;
}


// vector<char> vector<char> -> Vector<int>
//Converts the vector of characters to a vector<int> where 1 is an obstacle and 0 is open space
// (open space is selected based off thepresence of characters in open_chars)
std::vector<char> convertMap(std::vector<char> original_map_file, std::vector<char> open_chars) {
	std::vector<char> converted_map_file;

	for (int i = 0; i < original_map_file.size(); i++) {
		for (int j = 0; j < open_chars.size(); j++) {
			if (original_map_file[i] == open_chars[j]) {
				converted_map_file.push_back('0');
				break;
			}
			else if (j == (open_chars.size() - 1)) {
				converted_map_file.push_back('1');
			}
		}

	}

	return converted_map_file;
}


//Gets the map type from the user (1 is a 1 dimensional (single line) map, 2 is a 2 Dimensional map)
int getMapType() {
	int map_type;

	std::cout << "Please Enter a Map Type (1 is a 1 dimensional (single line) map, 2 is a 2 Dimensional map): ";
	std::cin >> map_type;

	while ((map_type != 1) && (map_type != 2)) {
		std::cout << "Invalid Entry \n"
			<< "Please Enter a Map Type (1 is a 1 dimensional (single line) map, 2 is a 2 Dimensional map): ";
		std::cin >> map_type;
	}

	return map_type;
}


// vector<char> string -> void
//Writes a given vector<char> to a given file(name) 
void writeMapFile(std::vector<char> map, std::string file_name, int width, int map_type) {
	char y_or_n;

	//Checks that file does not already exist, asks if user wants to continue if it does
	while (fileExists(file_name)) {
		std::cout << "File already exists, are you sure you want to overwrite?(y/n): ";
		std::cin >> y_or_n;
		if (y_or_n == 'y') {
			break;
		} 
		else if (y_or_n == 'n') {
			file_name = getFileNameOf("converted map file");
		}
	}

	//Writes all characters to a file, on one line if map_type is 1, else on multiple lines
	if (map_type == 1) {
		std::ofstream file;
		file.open(file_name);
		for (int i = 0; i < map.size(); i++) {
			file << map[i];
		}
	}
	else {
		std::ofstream file;
		file.open(file_name);
		for (int i = 0; i < map.size(); i++) {
			if ((i % width) == 0) {
				file << '\n';
			}
			file << map[i];
		}
	}
}


int main() {
	std::cout << "*** Map Convertor ***\n"
		<< "The map you wish to convert should have only those characters representing the actual map \n"
		<< "Any other information, including width, height, and type, should be removed beforehand \n"
		<< "(and perhaps placed in the map name for reference) \n"
		<< "ALL FILES MUST BE LOCATED IN THE SAME FOLDER AS THE PROGRAM \n";
	std::vector<char> open_chars = getOpenChars(); //All characters that represent open space
	std::string original_map_file_name = getFileNameOf("the original map file"); //Name of the map file to convert
	std::vector<char> original_map_file = getCharactersFromFile(original_map_file_name); //Get the original map file
	std::vector<char> converted_map_file = convertMap(original_map_file, open_chars); //Convert the map
	std::cout << "Done! \n";
	int map_type = getMapType();
	int width = 0;
	if (map_type == 2) {
		width = getWidth();
	}
	std::string new_map_file_name = getFileNameOf("converted map file"); //Desired name for map file
	writeMapFile(converted_map_file, new_map_file_name, width, map_type);

	return 0;
}
