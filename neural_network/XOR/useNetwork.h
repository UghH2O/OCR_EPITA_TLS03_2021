#ifndef USE_NETWORK_H
#define USE_NETWORK_H

#include <stddef.h>


void buildNetwork(double inputLayer[], double weights0[], double bias0[],
                  double hiddenLayer[], double weights1[], double bias1[],
                 double outputLayer[], size_t inputSize, size_t hiddenSize, size_t outputSize);

void saveNetwork(double weights0[], double weights1[], double bias0[], double bias1[]
                , char filename[], size_t inputSize, size_t hiddenSize, size_t outputSize);


void loadNetwork(double weights0[], double weights1[], double bias0[], double bias1[]
                , char filename[], size_t inputSize, size_t hiddenSize, size_t outputSize);

void use();

#endif
