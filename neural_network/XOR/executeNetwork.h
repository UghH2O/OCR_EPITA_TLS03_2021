#ifndef EXECUTE_NETWORK_H
#define EXECUTE_NETWORK_H
#include<stddef.h>


void ExecuteNetwork(double input[], double weights0[], double bias0[],
                    double hiddenLayer[], double weights1[], double bias1[]
                    , double outputLayer[], int showresult);

void printNetwork(double inputLayer[], double weights0[], double bias0[],
                        double hiddenLayer[], double weights1[], double bias1[],
                        double outputLayer[],void (*f)(double[]));

void print_matrix(char s[], double m[], size_t rows, size_t cols);

#endif