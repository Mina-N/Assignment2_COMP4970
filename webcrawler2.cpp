#include <iostream>
#include <string>
#include <ctype.h>
#include <fstream>
#include <iostream.h>
#include <cstring>
#include <stdlib.h>
#include <fstream.h>
#include <iomanip.h>


using namespace std;

public int find_children(string filename, int depth); //include as parameters pointers to the array of depths and queue containing urls
public void char_extractor(string fileame);


int main() {

    string filepath;
    int depth_limit = 0;
    int depth = 0;

    //queue that stores urls
    queue<string> url_queue;

    //array that stores depths of urls

    //prompting input
    cout << "Please input the path to a text file: ";
    cin >> filepath;


    //checking user input
    while (depth_limit < 1) {
        cout << "Please input a depth limit >= 1: ";
        cin >> depth_limit;
    }

    //push root filepath onto queue
    url_queue.push(filepath);


    while (!url_queue.empty()) {

        //pop url from queue
        filepath = url_queue.pop();


        //set depth to depth associated with url by checking array --> necessary for backtracking up the tree of urls


        //save html of url in a text file?


        //perform character extraction of url
        char_extractor(filepath);

        //Find children of url and store them into the queue. Also store their associated depths into an array.
        if (depth < depth_limit) {
            find_children(filepath, depth); //include queue and array parameters here
        }

    }

    return 0;
}



public void char_extractor(string filename) {

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



public int find_children(string filename, int depth) { //does this work with a string filename instead of a char filename? include array and queue as parameters here

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
            //push url into queue if url has not already been seen, but first remove "href=" from the front of url
            //store associated depth of url into array as (depth + 1)
            num_of_links++;
        }
    }

    webpage.close();

    return num_of_links;

}

