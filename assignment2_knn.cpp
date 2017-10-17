#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#define POPULATION 595

class Data_Point {
public:
    int pnt_id; // Identification number
    float clsfr; // Classifier
    float feat_vecs[95]; // Array for each of the 95 features of each element

    /* Constructor takes in a line from the dataset, saves the id, classifier,
       and saves each of the features into the vector. */
    Data_Point() {  // Instantiation
        pnt_id = -1;
    }

    Data_Point(string line) { // Initialization
        istringstream iss;
        iss.str(line);

        iss >> pnt_id;
        iss >> clsfr;

        int i;
        for(i = 0; i < 95; i++)
            iss >> feat_vecs[i];
    }
};

int init_trng_set(Data_Point trng_set[]);

int main() {
    // Instantiate and Initialize arrays for the training set and cluster points
    Data_Point trng_set[POPULATION];

    int act_pop = init_trng_set(trng_set);

    return 0;
}

/* Initialize Training Set
MAC 10/15/17
Reads in a file containing the whole dataset of the training data. For each of
the training elements stores the id, classifier, and all 95 feature values.
input: Empty array of data points
output: Returns the amount of elements in the training set.
updates:
*/
int init_trng_set(Data_Point trng_set[]) {
    ifstream input("our_dataset.txt");
    string line;
    int i = 0;
    while (getline(input, line)) {
        trng_set[i++] = Data_Point(line);
        cout << trng_set->pnt_id << endl;
        cout << trng_set->clsfr << endl;
        for (int i = 0; i < 95; i++) {
            cout << trng_set->feat_vecs[i] << " ";
        }
        cout << " " << endl;
    }

    return i;
}