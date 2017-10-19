#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <float.h>

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
float new_classification_average(Data_Point trng_set[], int instance1_index, int instance2_index, int instance3_index, int instance4_index, int instance5_index);
float new_classification_common(Data_Point trng_set[], int instance1_index, int instance2_index, int instance3_index, int instance4_index, int instance5_index);
float getPopularElement(float a[], int array_length);

int main() {
    //variables that keep track of whether the classification was correct or not
    double correct = 0;
    double incorrect = 0;
    int k;
    Data_Point trng_set[POPULATION];

    int act_pop = init_trng_set(trng_set);

    //trng_set is now an array that contains 595 Data_Points, with each Data_Point storing a point id, classification, and a feature vector of size 95

   // for (int i = 0; i < 595; i++) {
   //     for (int j = 0; j < 95; j++) {
   //         cout << "instance " << i + 1 << "feature number " << j + 1 << " " << trng_set[i].feat_vecs[j] << endl;
   //     }
  //  }

    // srand(time(0));

    //select the closest (k = 1) / three closest (k = 3) / five closest (k = 5) instances and find the new classification for the target instance
    cout << "k = 1, 3, or 5?" << endl;
    cin >> k;

    while (k != 1 && k != 3 && k != 5) {
        cout << "Please enter k = 1, 3, or 5." << endl;
        cin >> k;
    }

    for (int i = 0; i < 595; i++) {
        // Instantiate and Initialize arrays for the training set and cluster points
        int index_array_3[3] = {0, 0, 0};
        int index_array_5[5] = {0, 0, 0, 0, 0};
        int index = 0;
        float new_class = 0;
        float distances[595];

        //calculate the distances between the target instance (i) and the other instances by calculating Euclidean distances between the 95 features
        distance(trng_set, act_pop, i, distances);

        /*
        //  int random_index = (rand() % act_pop);
        // cout << random_index << endl;
         */

        if (k == 1) {
            index = closest_instance(distances, act_pop);
            cout << "closest instance is " << index << endl;
            // trng_set[random_index].clsfr = trng_set[index].clsfr;
            cout << "New classification for Data_Point " << (i + 1) << " is " << trng_set[index].clsfr << endl;
            if ((trng_set[i].clsfr < 0 && trng_set[index].clsfr < 0) ||
                (trng_set[i].clsfr > 0 && trng_set[index].clsfr > 0) ||
                (trng_set[i].clsfr == 0 && trng_set[index].clsfr == 0)) {
                correct++;
            } else {
                incorrect++;
            }
        } else if (k == 3) {
            three_closest_instances(distances, act_pop, index_array_3);
            new_class = new_classification_average(trng_set, index_array_3[0], index_array_3[1], index_array_3[2], -1,
                                                   -1);
            // new_class = new_classification_common(trng_set, index_array_3[0], index_array_3[1], index_array_3[2], -1, -1);
            //trng_set[random_index].clsfr = new_class;
            cout << "New classification for Data_Point " << (i + 1) << " is " << new_class << endl;
            if ((trng_set[i].clsfr < 0 && new_class < 0) || (trng_set[i].clsfr == 0 && new_class == 0) ||
                (trng_set[i].clsfr > 0 && new_class > 0)) {
                correct++;
            } else {
                incorrect++;
            }

        } else if (k == 5) {
            five_closest_instances(distances, act_pop, index_array_5);
            new_class = new_classification_average(trng_set, index_array_5[0], index_array_5[1], index_array_5[2],
                                                   index_array_5[3], index_array_5[4]);
            // new_class = new_classification_common(trng_set, index_array_5[0], index_array_5[1], index_array_5[2], index_array_5[3], index_array_5[4]);
            // trng_set[random_index].clsfr = new_class;
            cout << "New classification for Data Point " << (i + 1) << " is " << new_class << endl;
            if ((trng_set[i].clsfr < 0 && new_class < 0) ||
                (trng_set[i].clsfr == 0 && new_class == 0) ||
                (trng_set[i].clsfr > 0 && new_class > 0)) {
                correct++;
            } else {
                incorrect++;
            }
        }

    }

    //return classification rate
    cout << "The classification rate for k-nearest neighbor is: " << (correct / (incorrect + correct)) << endl;

    return 0;

}

/* Initialize Training Set
Reads in a file containing the whole dataset of the training data. For each of
the training elements stores the id, classifier, and all 95 feature values.
input: Empty array of data points
output: Returns the amount of elements in the training set.
updates:
*/

/*

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
 */


    int init_trng_set(Data_Point trng_set[]) {
        ifstream input("our_dataset.txt");
        string line;
        int j = 0;
        double magnitude;
        while (getline(input, line)) {
            trng_set[j++] = Data_Point(line);
            magnitude = 0;
            for (int k = 0; k < 95; k++) {
                magnitude += pow(trng_set[j - 1].feat_vecs[k], 2);
            }
            if (magnitude != 0) {
                for (int k = 0; k < 95; k++) {
                    trng_set[j - 1].feat_vecs[k] /= sqrt(magnitude);
                }
            }
        }
        return j;
    }


// Calculate distance between target training instance and the other 595 training instances.

float* distance(Data_Point trng_set[], int array_length, int random_index, float distances[]) {

    int i = 0;

    while (i < array_length) {

        if (i != random_index) {

            //calculate distance between trng_set[i] and trng_set[random_index]
            float sum = 0;
            for (int k = 0; k < 95; k++) {

                sum += pow((trng_set[random_index].feat_vecs[k] - trng_set[i].feat_vecs[k]), 2);

            }

            distances[i] = sqrt(sum);
        }
        i++;
    }

    distances[random_index] = 1000000;

 // for (int i = 0; i < array_length; i++) {
 //       cout << (i + 1) << ": " << distances[i] << endl;
 // }

    return distances;

}

// When k = 1, calculate the nearest instance to the target instance and return the index of the nearest instance.

int closest_instance(float distances[], int array_length) {

    int closest_index = 0;
    float min = FLT_MAX;

    for (int i = 0; i < array_length; i++) {
        if (distances[i] < min) {
            closest_index = i;
            min = distances[i];
        }
    }
    return closest_index;
}

// When k = 3, calculate the three closest instances to the target instance and return their indices.

int* three_closest_instances(float distances[], int array_length, int index_array[]) {
    int index1, index2, index3 = 0;
    float min = FLT_MAX;

    for (int i = 0; i < array_length; i++) {
        if (distances[i] < min) {
            index1 = i;
            min = distances[i];
        }
    }
    //cout << index1 << endl;

    min = FLT_MAX;

    for (int i = 0; i < array_length; i++) {
        if ((i != index1) && (distances[i] < min)) {
            index2 = i;
            min = distances[i];
        }
    }

   // cout << index2 << endl;

    min = FLT_MAX;

    for (int i = 0; i < array_length; i++) {
        if ((i != index1) && (i != index2) && (distances[i] < min)) {
            index3 = i;
            min = distances[i];
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

// When k = 5, calculate the five closest instances to the target instance and return their indices.

int* five_closest_instances(float distances[], int array_length, int index_array[]) {

   //cout << array_length << endl;

    int index1, index2, index3, index4, index5 = 0;
    float min = FLT_MAX;

    for (int i = 0; i < array_length; i++) {
        if (distances[i] < min) {
            index1 = i;
            min = distances[i];
        }
    }

  //  cout << index1 << endl;

    min = FLT_MAX;

    for (int i = 0; i < array_length; i++) {
        if ((i != index1) && (distances[i] < min)) {
            index2 = i;
            min = distances[i];
        }
    }

  //cout << index2 << endl;

    min = FLT_MAX;

    for (int i = 0; i < array_length; i++) {
        if ((i != index1) && (i != index2) && (distances[i] < min)) {
            index3 = i;
            min = distances[i];
        }
    }

 // cout << index3 << endl;

    min = FLT_MAX;

    for (int i = 0; i < array_length; i++) {
        if ((i != index1) && (i != index2) && (i != index3) && (distances[i] < min)) {
            index4 = i;
            min = distances[i];
        }
    }

  // cout << index4 << endl;

    min = FLT_MAX;

    for (int i = 0; i < array_length; i++) {
        if ((i != index1) && (i != index2) && (i != index3) && (i != index4) && (distances[i] < min)) {
            index5 = i;
            min = distances[i];
        }
    }

  // cout << index5 << endl;

    index_array[0] = index1;
    index_array[1] = index2;
    index_array[2] = index3;
    index_array[3] = index4;
    index_array[4] = index5;

    return index_array;
}


// Calculates the new classification of the target instance by taking the average of the classifications of the k nearest instances. Returns the new classification.

float new_classification_average(Data_Point trng_set[], int instance1_index, int instance2_index, int instance3_index, int instance4_index, int instance5_index) {
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

// Finds the most common classification among the instances indicated and sets new_classification to this classification

float new_classification_common(Data_Point trng_set[], int instance1_index, int instance2_index, int instance3_index, int instance4_index, int instance5_index) {
    float new_classification = 0;
    float temp_three_array[3];
    float temp_five_array[5];

    // k = 3
    if (instance4_index == -1 && instance5_index == -1) {
        //store the three instances into a temporary array
        temp_three_array[0] = trng_set[instance1_index].clsfr;
        temp_three_array[1] = trng_set[instance2_index].clsfr;
        temp_three_array[2] = trng_set[instance3_index].clsfr;

        new_classification = getPopularElement(temp_three_array, 3);
    }
    else { // k = 5
        //store the five instances in a temporary array
        temp_five_array[0] = trng_set[instance1_index].clsfr;
        temp_five_array[1] = trng_set[instance2_index].clsfr;
        temp_five_array[2] = trng_set[instance3_index].clsfr;
        temp_five_array[3] = trng_set[instance4_index].clsfr;
        temp_five_array[4] = trng_set[instance5_index].clsfr;

        new_classification = getPopularElement(temp_five_array, 5);

    }

    return new_classification;
}


//Finds the most popular element in array a
float getPopularElement(float a[], int array_length)
{
    int count = 1, tempCount; //questionable
    float popular = a[0];
    float temp = 0;
    for (int i = 0; i < (array_length - 1); i++)
    {
        temp = a[i];
        tempCount = 0;
        for (int j = 1; j < array_length; j++)
        {
            if (temp == a[j])
                tempCount++;
        }
        if (tempCount > count)
        {
            popular = temp;
            count = tempCount;
        }
    }
    return popular;
}



