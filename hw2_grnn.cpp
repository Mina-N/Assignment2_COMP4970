

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>

using namespace std;

#define POPULATION 595


class Data_Point {
    public:
    int pnt_id; // Identification number
    double clsfr; // Classifier
    double feat_vecs[95]; // Array for each of the 95 features of each element
    
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
    
    double grnn_classify(Data_Point trng_set[], double sigma[]);
};

void init_trng_set(Data_Point trng_set[]);

int main() {
    
    // read in data
    
    //ifstream data_stream("our_dataset.txt");
    
    double sigma[POPULATION];
    for (int i = 0; i < POPULATION; i++) {
        sigma[i] = 0.0281;
    }
    
    // Instantiate and Initialize arrays for the training set and cluster points
    Data_Point trng_set[POPULATION];
    
    init_trng_set(trng_set);
    
    double clsf_rate = 0;
    
    for (int i = 0; i < POPULATION; i++) {
        double prediction = trng_set[i].grnn_classify(trng_set, sigma);
        if (prediction < 0 && trng_set[i].clsfr < 0 || prediction > 0 && trng_set[i].clsfr > 0) {
            clsf_rate++;
        }
    }
    cout << "Sigma: " << sigma[0] << endl;
    cout << "Prediction Rate: " << clsf_rate / POPULATION << endl;
    
    return 0;
}

void init_trng_set(Data_Point trng_set[]) {
    ifstream input("our_dataset.txt");
    string line;
    int j = 0;
    while (getline(input, line)) {
        trng_set[j++] = Data_Point(line);
    }
}

double Data_Point::grnn_classify(Data_Point trng_set[], double sigma[]) {
    double gaussian = 0;
    double weighted_gaussian = 0;
    for (int i = 0; i < POPULATION; i++) {
        double distance_squared = 0;
        for (int j = 0; j < 94; j++) {
            distance_squared += pow((trng_set[i].feat_vecs[j] - this->feat_vecs[j]),2);
        }
        if (distance_squared != 0) {
            gaussian += exp(-distance_squared/(2*pow(sigma[i],2)));
            weighted_gaussian += exp(-distance_squared/(2*pow(sigma[i],2))) * trng_set[i].clsfr;
        }
    }
    return weighted_gaussian / gaussian;
}



