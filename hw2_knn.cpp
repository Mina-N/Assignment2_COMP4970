#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <math.h>

using namespace std;

#define K 5 // Number of clusters
#define POPULATION 768 // Arbitrary until a population size is defined

class Data_Point {
    public:
    int pnt_id, clsfr, cluster; // An identification number, classifier, and the cluster index
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
    
    float distance(Data_Point b) {
        float dist = 0;
        int i;
        for(i = 0; i < 95; i++)
            dist += pow((this->feat_vecs[i] + b.feat_vecs[i]), 2);
        
        return sqrt(dist);
    }
};

int init_trng_set(Data_Point trng_set[]);
int classify(int n, Data_Point trng_set[], Data_Point test_item);

int main() {
    // Instantiate and Initialize arrays for the training set and cluster points
    Data_Point trng_set[POPULATION]; 
    
    int act_pop = init_trng_set(trng_set); // Actual population of the set
    
    time_t t;
    srand((unsigned) time_t(&t));
    // Pick a random element from the test set and remove it
    Data_Point test_item = trng_set[rand() % act_pop];
    
    // Classify the test item based off the training set.
    int test_class = classify(act_pop, trng_set, test_item);
    // printf("%i: %i, %i\n", test_item.pnt_id, test_item.clsfr, test_class);
    
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
    while(getline(input, line))
        trng_set[i++] = Data_Point(line);
    
    return i;
} 


/* Classify
MAC 10/18/17
Classify a test element as the majority classification of the k closest elements
in the training set.

inputs: number of training elements, array of training elements, test element
outputs: classifier
updates:
*/
int classify(int n, Data_Point trng_set[], Data_Point test_item) {
    int i, j, ind, clsfr = 0;
    int closest[K];
    float min = 1;
    float distances[POPULATION];
     
    for(i = 0; i < n; i++)
        distances[i] = test_item.distance(trng_set[i]);
    
    // // printf("%i\n\n", test_item.pnt_id);
    // for(j = 0; j < K; j++) {
    //     min = distances[0];
        
        
    //     for(i = 0; i < n; i++) {
    //         if(distances[i] < min) {
    //             printf("%i, %i: %f, %f\n", i, trng_set[i].clsfr, min, distances[i]);
    //             min = distances[i];
    //             ind = i;
    //         }
    //     }
        
        
    //     closest[j] = ind;
    //     distances[ind] = 1;
    //     min = distances[0];
    // }
    
    min = distances[0];
    for(i = 0; i < n; i++) {
        if(distances[i] < min) {
            min = distances[i];
            printf("%i\n", i);
        }
    }
    
    if(clsfr > 0)
        return 1;
    else return -1;
}