

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include <random>

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
    
    
    // Classifies Data_Point using vector of length
    double grnn_classify(Data_Point trng_set[], double* sigma, int pop_index);
    
    double grnn_classify(Data_Point trng_set[], double sigma);
};

class Std_Deviation {
    public:
    int pop_size;
    double* fitness;
    double* sigma;
    int* rank;
    
    // Dummy Constructor
    Std_Deviation(int pop_size_in) {
        pop_size = pop_size_in;
 
        fitness = new double[pop_size];
        sigma = new double[NUM_DATA_POINTS*pop_size];
        rank = new int[pop_size];
    }
    
    // Main Constructor - Initilize population of size pop_size_in to random values between 0 and 2
    Std_Deviation(Data_Point trng_set[], int pop_size_in) {
        
        // Dynamically allocate instance variables
        pop_size = pop_size_in;
        fitness = new double[pop_size];
        sigma = new double[NUM_DATA_POINTS*pop_size];
        rank = new int[pop_size];
        
        // Max and min values for sigma
        double min_sigma = 0.00001;
        double max_sigma = 0.25;
        
        // Declare temporary instance variables
        double* temp_fitness;
        temp_fitness = new double[pop_size];
        double* temp_sigma;
        temp_sigma = new double[NUM_DATA_POINTS*pop_size];
        int* temp_rank;
        temp_rank = new int[pop_size];
        
        // Seed rand()
        srand(time(0));
        
        // Initialize population
        for (int i = 0; i < pop_size; i++) {
            for (int j = 0; j < NUM_DATA_POINTS; j++) {
                temp_sigma[i*NUM_DATA_POINTS + j] = ((double) rand() / RAND_MAX) * (max_sigma - min_sigma) + min_sigma;
            }
        }

        for (int i = 0; i < pop_size; i++) {
            temp_fitness[i] = 0;
        }
        double prediction;
        
        cout << "\nInitial Fitness of Population" << endl;
        for (int i = 0; i < pop_size; i++) {
            prediction = 0;
            for (int j = 0; j < NUM_DATA_POINTS; j++) {
                prediction = trng_set[j].grnn_classify(trng_set, temp_sigma, i);
                if (prediction < 0 && trng_set[j].clsfr < 0 || prediction > 0 && trng_set[j].clsfr > 0) {
                    temp_fitness[i]++;
                }
            }
            temp_fitness[i] = temp_fitness[i] / NUM_DATA_POINTS;
            cout << "Individual " << i << ": " << temp_fitness[i] << endl;
            temp_rank[i] = 1;
            for (int j = 0; j < i; j++) {
                if (temp_fitness[i] > temp_fitness[j]) {
                    temp_rank[j]++;
                }
                else {
                    temp_rank[i]++;
                }
            }
        }
        
        
        
        for (int i = 0; i < pop_size; i++) {
            for (int j = 0; j < NUM_DATA_POINTS; j++) {
                sigma[NUM_DATA_POINTS*(temp_rank[i]-1) + j] = temp_sigma[NUM_DATA_POINTS*i + j];
            }
            fitness[temp_rank[i]-1] = temp_fitness[i];
            rank[temp_rank[i]-1] = temp_rank[i];
        }
        
        delete [] temp_fitness;
        delete [] temp_sigma;
        delete [] temp_rank;
        
    }
    
    void set_fitness(double fitness_in, int index) {
        fitness[index] = fitness_in;
    }
    
    void assess_fitness(Data_Point trng_set[]) {
        
        double* temp_fitness;
        temp_fitness = new double[pop_size];
        int* temp_rank;
        temp_rank = new int[pop_size];
        double prediction;
        
        cout << "\nFitness of Children" << endl;
        for (int i = 0; i < pop_size; i++) {
            prediction = 0;
            for (int j = 0; j < NUM_DATA_POINTS; j++) {
                prediction = trng_set[j].grnn_classify(trng_set, sigma, i);
                if (prediction < 0 && trng_set[j].clsfr < 0 || prediction > 0 && trng_set[j].clsfr > 0) {
                    temp_fitness[i]++;
                }
                if (temp_fitness[i] > NUM_DATA_POINTS) {
                    cout << "temp_fitness overflowing: " << temp_fitness[i] << endl;
                }
            }
            temp_fitness[i] /= NUM_DATA_POINTS;
            cout << "Child " << i << ": " << temp_fitness[i] << endl;
            temp_rank[i] = 1;
            for (int j = 0; j < i; j++) {
                if (temp_fitness[i] > temp_fitness[j]) {
                    temp_rank[j]++;
                }
                else {
                    temp_rank[i]++;
                }
            }
        }
        
        
        
        for (int i = 0; i < pop_size; i++) {
            fitness[temp_rank[i]-1] = temp_fitness[i];
            rank[temp_rank[i]-1] = temp_rank[i];
        }
        
        delete [] temp_fitness;
        delete [] temp_rank;
    }
    
    void select_parents(void) {
        for (int i = 0; i < pop_size; i++) {
            for (int j = 0; j < NUM_DATA_POINTS; j++) {
            }
        }
        Std_Deviation temp_parents(pop_size);
        srand(time(0));
        int P;
        cout << "\nFitness of Parents" << endl;
        for (int i = 0; i < pop_size; i++) {
            cout << "Parent " << i << ": " << fitness[i] << endl;
        }
        for (int i = 0; i < pop_size; i++) {
            P = (rand() % (pop_size * (pop_size + 1)/2)) + 1;
            for (int j = pop_size; j > 0; j--) {
                P -= j;
                if (P <=0) {
                    temp_parents.fitness[i] = fitness[pop_size-j];
                    for (int k = 0; k < NUM_DATA_POINTS; k++) {
                        temp_parents.sigma[NUM_DATA_POINTS*i + k] = sigma[NUM_DATA_POINTS*(pop_size-j) + k];
                    }
                    temp_parents.rank[i] = rank[pop_size-j];
                    break;
                }
            }
        }
        
        for (int i = 0; i < pop_size; i++) {
            fitness[i] = temp_parents.fitness[i];
            for (int j = 0; j < NUM_DATA_POINTS; j++) {
                sigma[NUM_DATA_POINTS*i + j] = temp_parents.sigma[NUM_DATA_POINTS*i + j];
            }
            rank[i] = temp_parents.rank[i];
        }
        cout << "\nFitness of Selected Parents" << endl;
        for (int i = 0; i < pop_size; i++) {
            cout << "Selected Parent " << i << ": " << fitness[i] << endl;
        }
        for (int i = 0; i < pop_size; i++) {
            for (int j = 0; j < NUM_DATA_POINTS; j++) {
            }
        }
    }
    
    void procreate(Data_Point trng_set[]) {
        Std_Deviation temp_children(pop_size);
        
        // Mutation rate
        double mutation_dev = 0.001;
        default_random_engine generator(time(0));
        normal_distribution<double> mutation(0.0,1.0);
        
        if (pop_size % 2 == 0) {
            double parent1_sigma;
            double parent2_sigma;
            for (int i = 0; i < pop_size; i += 2) {
                for (int j = 0; j < NUM_DATA_POINTS; j++) {
                    parent1_sigma = sigma[NUM_DATA_POINTS*i + j];
                    parent2_sigma = sigma[NUM_DATA_POINTS*(i+1) + j];
                    if (parent1_sigma > parent2_sigma) {
                        temp_children.sigma[NUM_DATA_POINTS*i + j] = ((double) rand() / RAND_MAX) * (parent1_sigma - parent2_sigma) + parent2_sigma + mutation_dev * mutation(generator);
                        temp_children.sigma[NUM_DATA_POINTS*(i+1) + j] = ((double) rand() / RAND_MAX) * (parent1_sigma - parent2_sigma) + parent2_sigma + mutation_dev * mutation(generator);
                    }
                    else {
                        temp_children.sigma[NUM_DATA_POINTS*i + j] = ((double) rand() / RAND_MAX) * (parent2_sigma - parent1_sigma) + parent1_sigma + mutation_dev * mutation(generator);
                        temp_children.sigma[NUM_DATA_POINTS*(i+1) + j] = ((double) rand() / RAND_MAX) * (parent2_sigma - parent1_sigma) + parent1_sigma + mutation_dev * mutation(generator);
                    }
                }
            }
        }
        
        else {
            double parent1_sigma;
            double parent2_sigma;
            for (int j = 0; j < NUM_DATA_POINTS; j++) {
                temp_children.sigma[j] = sigma[j] + mutation_dev * mutation(generator);
            }
            for (int i = 1; i < pop_size; i +=2) {
                for (int j = 0; j < NUM_DATA_POINTS; j++) {
                    parent1_sigma = sigma[NUM_DATA_POINTS*i + j];
                    parent2_sigma = sigma[NUM_DATA_POINTS*(i+1) + j];
                    if (parent1_sigma > parent2_sigma) {
                        temp_children.sigma[NUM_DATA_POINTS*i + j] = ((double) rand() / RAND_MAX) * (parent1_sigma - parent2_sigma) + parent2_sigma + mutation_dev * mutation(generator);
                        temp_children.sigma[NUM_DATA_POINTS*(i+1) + j] = ((double) rand() / RAND_MAX) * (parent1_sigma - parent2_sigma) + parent2_sigma + mutation_dev * mutation(generator);
                    }
                    else {
                        temp_children.sigma[NUM_DATA_POINTS*i + j] = ((double) rand() / RAND_MAX) * (parent2_sigma - parent1_sigma) + parent1_sigma + mutation_dev * mutation(generator);
                        temp_children.sigma[NUM_DATA_POINTS*(i+1) + j] = ((double) rand() / RAND_MAX) * (parent2_sigma - parent1_sigma) + parent1_sigma + mutation_dev * mutation(generator);
                    }
                }
            }
        }
        temp_children.assess_fitness(trng_set);
        
        for (int i = 0; i < pop_size; i++) {
            for (int j = 0; j < NUM_DATA_POINTS; j++) {
                sigma[NUM_DATA_POINTS*i + j] = temp_children.sigma[NUM_DATA_POINTS*i + j];
            }
            fitness[i] = temp_children.fitness[i];
            rank[i] = temp_children.rank[i];
        }
    }
    
    ~Std_Deviation() {
        delete [] fitness;
        delete [] sigma;
        delete [] rank;
    }

};

void init_trng_set(Data_Point trng_set[]);

int main() {
    
    // Empirically determined optimal uniform standard deviation value for Gaussian kernel
    double sigma = 0.1237;
    
    Data_Point trng_set[NUM_DATA_POINTS];
    
    init_trng_set(trng_set);
    
    cout << "\n***Generation 0***" << endl;
    Std_Deviation sigmas(trng_set, 10);

    for (int i = 0; i < 100; i++) {
        sigmas.select_parents();
        sigmas.procreate(trng_set);
        cout << "\n***Generation " << i+1 << "***\n" << endl;
    }
    
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

double Data_Point::grnn_classify(Data_Point trng_set[], double* sigma, int pop_index) {
    double gaussian = 0;
    double weighted_gaussian = 0;
    for (int i = 0; i < NUM_DATA_POINTS; i++) {
        double distance_squared = 0;
        for (int j = 0; j < NUM_FEATURES; j++) {
            distance_squared += pow((trng_set[i].feat_vecs[j] - this->feat_vecs[j]),2);
        }
        if (distance_squared != 0) {
            gaussian += exp(-distance_squared/(2*pow(sigma[NUM_DATA_POINTS*pop_index + i],2)));
            weighted_gaussian += exp(-distance_squared/(2*pow(sigma[NUM_DATA_POINTS*pop_index + i],2))) * trng_set[i].clsfr;
        }
    }
    return weighted_gaussian / gaussian;
}

double Data_Point::grnn_classify(Data_Point trng_set[], double sigma) {
    double gaussian = 0;
    double weighted_gaussian = 0;
    for (int i = 0; i < NUM_DATA_POINTS; i++) {
        double distance_squared = 0;
        for (int j = 0; j < NUM_FEATURES; j++) {
            distance_squared += pow((trng_set[i].feat_vecs[j] - this->feat_vecs[j]),2);
        }
        if (distance_squared != 0) {
            gaussian += exp(-distance_squared/(2*pow(sigma,2)));
            weighted_gaussian += exp(-distance_squared/(2*pow(sigma,2))) * trng_set[i].clsfr;
        }
    }
    return weighted_gaussian / gaussian;
}

