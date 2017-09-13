// Takes text file path as keyboard input and writes the unigram vector to new text file. Writes relative
// frequency of both printable characters.

#include <iostream> 
#include <string>
#include <ctype.h>
#include <fstream>

using namespace std;

int main() {
    string filepath;
    char c;
    double unigram_count[95] = {0};
    double total_unigrams = 0;
    
    cout << "Please input the path to a text file: ";
    cin >> filepath;
    
    // Read characters from file and store absolute frequency in double array
    
    ifstream in_text(filepath);
    
    while (in_text >> noskipws >> c) {
        if (isprint(c)) {
            int i = c - 32;
            unigram_count[i]++;
            total_unigrams++;
        }
    }
    in_text.close();
    
    // Create and write to new text file containing relative frequencies
    
    string new_filename = filepath.erase(filepath.length()-4,4) + "_unigrams.txt";
    ofstream out_text(new_filename);
    
    for (int i = 0; i < 95; i++) {
        char a = i + 32;
        out_text << a << ": " << unigram_count[i] / total_unigrams << "\n";
    }
    out_text.close();
    
    return 0;
}



