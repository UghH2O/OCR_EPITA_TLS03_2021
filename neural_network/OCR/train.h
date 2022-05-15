#ifndef TRAIN_H
#define TRAIN_H

#include "execute.h"

#define GRADIENT_LEN 30*30*30 + 30*(2*26+18) + 30 + 2 * 26 + 18

// training element, contain the matrix of the image of the caracter to train
// the caracter and the wanted output for the neural network
typedef struct trainingElement
{   
    double image[INPUT_LEN];
    char answer;
    double wanted[OUTPUT_LEN];

}trainingElement;


//the types of the elements for the partial derivatives
typedef enum Type
{
    WEIGHT,
    BIAS,
    ACTIVATION
} Type;

void train_network(network *net);

#endif