

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>

using namespace std;

#define NUM_DATA_POINTS 595
#define NUM_FEATURES 95


class Data_Point {
    public:
    int pnt_id; // Identification number
    double clsfr; // Classifier
    double feat_vecs[NUM_FEATURES]; // Array for each of the 95 features of each element
    
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
        for(i = 0; i < NUM_FEATURES; i++)
        iss >> feat_vecs[i];
    }
    
    double grnn_classify(Data_Point trng_set[], double sigma[]);
};

class Std_Deviation {
    public:
    int pop_size;
    double* fitness;
    double** sigma;
    int* rank;
    
    Std_Deviation(int pop_size_in) {
        pop_size = pop_size_in;
    }
    
    Std_Deviation(Data_Point trng_set[], int pop_size_in) {
        pop_size = pop_size_in;
        fitness = new double[pop_size];
        sigma = new double*[NUM_DATA_POINTS];
        for (int i = 0; i < NUM_DATA_POINTS; i++)
            sigma[i] = new double[pop_size];
        rank = new int[pop_size];
        
        double min_sigma = 0;
        double max_sigma = 2;
        
        double* temp_fitness;
        temp_fitness = new double[pop_size];
        double** temp_sigma;
        temp_sigma = new double*[NUM_DATA_POINTS];
        for (int i = 0; i < NUM_DATA_POINTS; i++)
            temp_sigma[i] = new double[pop_size];
        int* temp_rank;
        temp_rank = new int[pop_size];
        
        srand(time(0));
        
        for (int i = 0; i < pop_size; i++) {
            for (int j = 0; j < NUM_DATA_POINTS; j++)
                temp_sigma[j][i] = ((double) rand() / RAND_MAX) * (max_sigma - min_sigma) + min_sigma;
        }
        for (int i = 0; i < pop_size; i++)
            temp_fitness = 0;
        double prediction;

        
        
        
        for (int i = 0; i < pop_size; i++) {
            prediction = 0;
            cout << "First i = " << i << endl;
            for (int j = 0; j < NUM_DATA_POINTS; j++) {
                cout << "j = " << j << endl;
                prediction = trng_set[j].grnn_classify(trng_set, sigma[i]);
                if (prediction < 0 && trng_set[j].clsfr < 0 || prediction > 0 && trng_set[j].clsfr > 0) {
                    temp_fitness[i]++;
                }
            }
            cout << "Second i = " << i << endl;
            temp_fitness[i] /= NUM_DATA_POINTS;
            temp_rank[i] = 1;
            for (int j = 0; j < i; j++) {
                if (temp_fitness[i] > temp_fitness[j]) {
                    temp_rank[j]++;
                }
                else {
                    temp_rank[i]++;
                }
            }
            cout << "Second i = " << i << endl;
        }
        
        
        
        for (int i = 0; i < pop_size; i++) {
            for (int j = 0; j < NUM_DATA_POINTS; j++)
                sigma[j][temp_rank[i]-1] = temp_sigma[j][i];
            fitness[temp_rank[i]-1] = temp_fitness[i];
            rank[temp_rank[i]-1] = temp_rank[i];
        }
        delete [] temp_fitness;
        temp_fitness = NULL;
        delete [] temp_sigma;
        temp_sigma = NULL;
        delete [] temp_rank;
        temp_rank = NULL;
    }
    
    void select_parents() {
        Std_Deviation temp_parents(pop_size);
        srand(time(0));
        int P;
        for (int i = 0; i < pop_size; i++) {
            P = rand() % (pop_size * (pop_size + 1)) + 1;
            for (int j = pop_size; j > 0; j--) {
                P -= j;
                if (P <=0) {
                    temp_parents.fitness[i] = fitness[j-pop_size+1];
                    temp_parents.sigma[i] = sigma[j-pop_size+1];
                    temp_parents.rank[i] = rank[j-pop_size+1];
                    break;
                }
            }
        }
        fitness = temp_parents.fitness;
        sigma = temp_parents.sigma;
        rank = temp_parents.rank;
    }
    
    ~Std_Deviation() {
        delete [] fitness;
        for (int i = 0; i < NUM_DATA_POINTS; i++)
            delete [] sigma[i];
        delete [] sigma;
        delete [] rank;
    }
};

void init_trng_set(Data_Point trng_set[]);

int main() {
    
    // read in data
    
    //ifstream data_stream("our_dataset.txt");
    
    
    // Instantiate and Initialize arrays for the training set and cluster points
    
    double sigma[NUM_DATA_POINTS];
    for (int i = 0; i < NUM_DATA_POINTS; i++)
        sigma[i] = 0.1237;
    
    Data_Point trng_set[NUM_DATA_POINTS];
    
    init_trng_set(trng_set);
    
    Std_Deviation sigmas(trng_set, 10);
    
    int TP, TN, FP, FN, U;
    double clsf_rate;
    clsf_rate = 0;
    TP = 0;
    TN = 0;
    FP = 0;
    FN = 0;
    U = 0;
    for (int i = 0; i < NUM_DATA_POINTS; i++) {
        double prediction = trng_set[i].grnn_classify(trng_set, sigma);
        if (prediction < 0 && trng_set[i].clsfr < 0) {
            TN++;
            clsf_rate++;
        }
        else if (prediction > 0 && trng_set[i].clsfr > 0) {
            TP++;
            clsf_rate++;
        }
        else if (prediction < 0 && trng_set[i].clsfr > 0) {
            FN++;
        }
        else if (prediction > 0 && trng_set[i].clsfr < 0) {
            FP++;
        }
        else {
            U++;
        }
    }
    clsf_rate /= NUM_DATA_POINTS;
    cout << "Prediction Rate: " << clsf_rate << endl;
    cout << "True Positives: " << TP << endl;
    cout << "True Negatives: " << TN << endl;
    cout << "False Positives: " << FP << endl;
    cout << "False Negatives: " << FN << endl;
    if (U > 0) {
        cout << "Unclassifiables: " << U << endl;
    }
    return 0;
}

void init_trng_set(Data_Point trng_set[]) {
    ifstream input("our_dataset.txt");
    string line;
    int j = 0;
    double magnitude;
    while (getline(input, line)) {
        trng_set[j++] = Data_Point(line);
        magnitude = 0;
        for (int k = 0; k < NUM_FEATURES; k++)
            magnitude += pow(trng_set[j-1].feat_vecs[k], 2);
        if (magnitude != 0) {
            for (int k = 0; k < NUM_FEATURES; k++)
                trng_set[j-1].feat_vecs[k] /= sqrt(magnitude);
        }
    }
}

double Data_Point::grnn_classify(Data_Point trng_set[], double sigma[]) {
    double gaussian = 0;
    double weighted_gaussian = 0;
    cout << "1" << endl;
    for (int i = 0; i < NUM_DATA_POINTS; i++) {
        double distance_squared = 0;
        for (int j = 0; j < NUM_FEATURES; j++)
            distance_squared += pow((trng_set[i].feat_vecs[j] - this->feat_vecs[j]),2);
        if (distance_squared != 0) {
            gaussian += exp(-distance_squared/(2*pow(sigma[i],2)));
            weighted_gaussian += exp(-distance_squared/(2*pow(sigma[i],2))) * trng_set[i].clsfr;
        }
    }
    return weighted_gaussian / gaussian;
}


