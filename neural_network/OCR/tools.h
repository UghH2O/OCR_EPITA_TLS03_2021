#ifndef TOOLS_H
#define TOOLS_H

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <err.h>

// different lengthS for the neural network
#define INPUT_LEN  30*30
#define HIDDEN_LEN  30
#define OUTPUT_LEN  2*26+18


// struct for the neural network
// contains all the 3 layers: input, hidden and output
// and all weights and bias
typedef struct network
{
    //layers of the network, lenghes always matches the network's
    double input[INPUT_LEN];
    double hidden[HIDDEN_LEN];
    double output[OUTPUT_LEN];

    //weights and biases of the network, lenghes always matches the network's
    double weights01[INPUT_LEN*HIDDEN_LEN];
    double weights12[HIDDEN_LEN*OUTPUT_LEN];
    double bias01[HIDDEN_LEN];
    double bias12[OUTPUT_LEN];
    
} network;

void MutltMAtrix(double weights[], double fromNeurons[], double toNeurons[]
                 , size_t w_r, size_t w_c, size_t n_c);

void AddMAtrix(double Neurons[], double bias[], size_t size);

void SoftMax(double neurons [], size_t size);

void Sigmoid(double neurons[], size_t size);

void nextLayer(double weihgts[], double fromNeurons[], double bias[],
               double toNeurons[],
               size_t w_c, size_t outSize);



void buildNetwork(network *net,
            size_t inputSize, size_t hiddenSize, size_t outputSize);

void saveNetwork(network *net, char filename[], 
            size_t inputSize, size_t hiddenSize, size_t outputSize);

void loadNetwork(network *net, char filename[], 
            size_t inputSize, size_t hiddenSize, size_t outputSize);

void loadTraining(double array[], char filename[], size_t size, size_t index);


void print_matrix(char s[], double m[], size_t rows, size_t cols);

#endif