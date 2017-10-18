#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

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
float* distance(Data_Point trng_set[], int array_length, int random_index, float distances[]);
int closest_instance(float distances[], int array_length);
int* three_closest_instances(float distances[], int array_length, int index_array[]);
int* five_closest_instances(float distances[], int array_length, int index_array[]);
float new_classification(Data_Point trng_set[], int instance1_index, int instance2_index, int instance3_index, int instance4_index, int instance5_index);

int main() {
    //variables that keep track of whether the classification was correct or not
    double correct = 0;
    double incorrect = 0;

    srand(time(0));

    for (int i = 0; i < 595; i++) {
        // Instantiate and Initialize arrays for the training set and cluster points
        Data_Point trng_set[POPULATION];
        float distances[595];
        int k = 3;
        int index_array_3[3];
        int index_array_5[5];
        int index = 0;
        float new_class;

        int act_pop = init_trng_set(trng_set);

        //trng_set is now an array that contains 595 Data_Points, with each Data_Point storing a point id, classification, and a feature vector of size 95

        //randomly select a Data Point from the array
        int random_index = (rand() % act_pop);
        cout << random_index << endl;

        //calculate the distances between the target instance and the other instances by calculating Euclidean distances between the 95 features
        distance(trng_set, act_pop, random_index, distances);
        /*
        //select the closest (k = 1) / three closest (k = 3) / five closest (k = 5) instances and find the new classification for the target instance
        cout << "k = 1, 3, or 5?" << endl;
        cin >> k;

        while (k != 1 && k != 3 && k != 5) {
            cout << "Please enter k = 1, 3, or 5." << endl;
            cin >> k;
        }
        */

        if (k == 1) {
            index = closest_instance(distances, act_pop);
           // trng_set[random_index].clsfr = trng_set[index].clsfr;
            cout << "New classification for Data_Point " << random_index << " is " << trng_set[index].clsfr << endl;
            if ((trng_set[random_index].clsfr < 0 && trng_set[index].clsfr < 0) || (trng_set[random_index].clsfr > 0 && trng_set[index].clsfr > 0) || (trng_set[random_index].clsfr == 0 && trng_set[index].clsfr == 0)) {
                correct++;
            }
            else {
                incorrect++;
            }
        } else if (k == 3) {
            three_closest_instances(distances, act_pop, index_array_3);
            new_class = new_classification(trng_set, index_array_3[0], index_array_3[1], index_array_3[2], -1, -1);
            //trng_set[random_index].clsfr = new_class;
            cout << "New classification for Data_Point " << random_index << " is " << new_class << endl;
            if ((trng_set[random_index].clsfr < 0 && new_class < 0) || (trng_set[random_index].clsfr == 0 && new_class == 0) || (trng_set[random_index].clsfr > 0 && new_class > 0)) {
                correct++;
            }
            else {
                incorrect++;
            }

        } else if (k == 5) {
            five_closest_instances(distances, act_pop, index_array_5);
            new_class = new_classification(trng_set, index_array_5[0], index_array_5[1], index_array_5[2],
                                           index_array_5[3], index_array_5[4]);
           // trng_set[random_index].clsfr = new_class;
            cout << "New classification for Data_Point " << random_index << " is " << new_class << endl;
            if ((trng_set[random_index].clsfr < 0 && new_class < 0) || (trng_set[random_index].clsfr == 0 && new_class == 0) || (trng_set[random_index].clsfr > 0 && new_class > 0)) {
                correct++;
            }
            else {
                incorrect++;
            }
        }

    }

    //return classification rate
    cout << "The classification rate for k-nearest neighbor is: " << (correct / (incorrect + correct)) << endl;

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
    int j = 0;
    while (getline(input, line)) {
        trng_set[j++] = Data_Point(line);
        //cout << trng_set[j-1].pnt_id << endl;
        //cout << trng_set[j-1].clsfr << endl;
        for (int i = 0; i < 95; i++) {
           // cout << trng_set[j-1].feat_vecs[i] << " ";
        }
       // cout << " " << endl;
    }

    return j;
}


float* distance(Data_Point trng_set[], int array_length, int random_index, float distances[]) {

    int i, j = 0;

    while (i < array_length) {

        //calculate distance between trng_set[i] and trng_set[random_index]
        float sum = 0;
        for (int k = 0; k < 95; k++) {
            sum += pow((trng_set[random_index].feat_vecs[k] - trng_set[i].feat_vecs[k]), 2);
        }

        distances[j] = sqrt(sum);
        i++;
        j++;
    }

  //  for (int i = 0; i < array_length; i++) {
   //     cout << (i + 1) << ": " << distances[i] << endl;
  //  }

    return distances;

}

int closest_instance(float distances[], int array_length) {

    int closest_index = 0;
    int min = distances[0];

    for (int i = 0; i < array_length; i++) {
        if (distances[i] < min) {
            closest_index = i;
        }
    }
    return closest_index;
}


int* three_closest_instances(float distances[], int array_length, int index_array[]) {
    int index1, index2, index3 = 0;
    float min = distances[0];

    for (int i = 0; i < array_length; i++) {
        if (distances[i] < min) {
            index1 = i;
        }
    }
    //cout << index1 << endl;

    min = distances[0];

    for (int i = 0; i < array_length; i++) {
        if ((i != index1) && (distances[i] < min)) {
            index2 = i;
        }
    }

    //cout << index2 << endl;

    min = distances[0];

    for (int i = 0; i < array_length; i++) {
        if ((i != index1) && (i != index2) && (distances[i] < min)) {
            index3 = i;
        }
    }

   // cout << index3 << endl;


    index_array[0] = index1;
    //cout << index_array[0] << endl;
    index_array[1] = index2;
   // cout << index_array[1] << endl;
    index_array[2] = index3;
    //cout << index_array[2] << endl;

    return index_array;
}

int* five_closest_instances(float distances[], int array_length, int index_array[]) {

    int index1, index2, index3, index4, index5 = 0;
    float min = distances[0];

    for (int i = 0; i < array_length; i++) {
        if (distances[i] < min) {
            index1 = i;
        }
    }

    min = distances[0];

    for (int i = 0; i < array_length; i++) {
        if ((i != index1) && (distances[i] < min)) {
            index2 = i;
        }
    }

    min = distances[0];

    for (int i = 0; i < array_length; i++) {
        if ((i != index1) && (i != index2) && (distances[i] < min)) {
            index3 = i;
        }
    }

    min = distances[0];

    for (int i = 0; i < array_length; i++) {
        if ((i != index1) && (i != index2) && (i != index3) && (distances[i] < min)) {
            index4 = i;
        }
    }

    min = distances[0];

    for (int i = 0; i < array_length; i++) {
        if ((i != index1) && (i != index2) && (i != index3) && (i != index4) && (distances[i] < min)) {
            index5 = i;
        }
    }

    index_array[0] = index1;
    index_array[1] = index2;
    index_array[2] = index3;
    index_array[3] = index4;
    index_array[4] = index5;

    return index_array;
}

float new_classification(Data_Point trng_set[], int instance1_index, int instance2_index, int instance3_index, int instance4_index, int instance5_index) {
    float new_classification = 0;
    // k = 3
    if (instance4_index == -1 && instance5_index == -1) {
        new_classification = ((trng_set[instance1_index].clsfr + trng_set[instance2_index].clsfr + trng_set[instance3_index].clsfr) / 3.0);
    }
    else { // k = 5
        new_classification = ((trng_set[instance1_index].clsfr + trng_set[instance2_index].clsfr + trng_set[instance3_index].clsfr + trng_set[instance4_index].clsfr + trng_set[instance5_index].clsfr) / 5.0);
    }

    return new_classification;

}



