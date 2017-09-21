#include <iostream>
#include <string>
#include <ctype.h>
#include <fstream>
#include <iostream.h>
#include <cstring>
#include <stdlib.h>
#include <fstream.h>
#include <iomanip.h>
#include <stack>

//***CHANGE INCONSISTENCIES BETWEEN CHARACTERS AND STRINGS***


using namespace std;

public int find_children(string filename, int depth, stack<string>& stack1, stack<int>& stack2); //include as parameters pointers to the array of depths and queue containing urls
public void char_extractor(string filename);


int main() {

    string filepath;
    int depth_limit = 0;
    int depth = 0;

    //stack that stores urls
    stack<string> url_stack;

    //stack that stores depths of urls
    stack<int> depth_stack;

    //prompting input
    cout << "Please input the path to a text file: ";
    cin >> filepath;


    //checking user input
    while (depth_limit < 1) {
        cout << "Please input a depth limit >= 1:";
        cin >> depth_limit;
    }

    //push root filepath onto stack
    url_stack.push(filepath);

    //push root depth onto stack
    depth_stack.push(0);


    while (!url_stack.empty()) {

        //pop url from queue
        filepath = url_stack.pop();


        //set depth to depth associated with url by checking array --> necessary for backtracking up the tree of urls?
        depth = depth_stack.pop();


        //perform character extraction of url
        char_extractor(filepath);

        //Find children of url and store them into the stack. Also store their associated depths into an stack. Saves html to output file.
        if (depth < depth_limit) {
            find_children(filepath, depth, url_stack, depth_stack);
        }

    }

    return 0;
}



public void char_extractor(char filename[]) {

    filename =

    char c;
    double unigram_count[95] = {0};
    double total_unigrams = 0;

    ifstream in_text(filename);

    while (in_text >> noskipws >> c) {
        if (isprint(c)) {
            int i = c - 32;
            unigram_count[i]++;
            total_unigrams++;
        }
    }

    in_text.close();

    // Create and write to new text file containing relative frequencies

    string new_filename = filename.erase(filename.length() - 4, 4) + "_unigrams.txt";
    ofstream out_text(new_filename);

    for (int i = 0; i < 95; i++) {
        char a = i + 32;
        out_text << a << ": " << unigram_count[i] / total_unigrams << "\n";
    }

    out_text.close();
}



public int find_children(char filename[], int depth, stack<string>& stack1, stack<int>& stack2) {

    char javaCall[100], url[100];
    int num_of_links = 0;

    strcat(javaCall, "java getWebPage ");
    strcat(javaCall, filename);
    strcat(javaCall, " > output.html");
    system(javaCall);
    ifstream webpage("output.html");

    while (!webpage.eof()) {
        webpage >> setw(99) >> url;
        if (strncmp("href", url, 4) == 0) {
            //push url onto stack if url has not already been seen? but first remove "href=" from the front of url
            stack1.push(url);

            //store associated depth of url into stack as (depth + 1)
            stack2.push(depth + 1);
            num_of_links++;
        }
    }

    webpage.close();

    return num_of_links;

}

