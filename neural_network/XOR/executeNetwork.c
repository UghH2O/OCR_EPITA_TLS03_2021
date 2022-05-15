#include "MatrixFunc.h"
#include <stdio.h>
#include <stddef.h>
/*

    This file handles neural network with one hidden layer ONLY.

*/

// the types and the showing process will change in the future, this one is for Xor neural network
void ExecuteNetwork(double input[], double weights0[], double bias0[],
                    double hiddenLayer[], double weights1[], double bias1[]
                    , double outputLayer[], int showresult)
{
    nextLayer(weights0, input, bias0, hiddenLayer, 2, 4);
    nextLayer(weights1, hiddenLayer, bias1, outputLayer, 4, 2);

    if (showresult == 1)
    {
        printf("Inputs :\n %f Xor %f\nOutput :\n", input[0], input[1]);

        for (size_t i = 0; i <2; i++)
        {
            printf("%ld (%f)\n\n", i, outputLayer[i]);
        }
    }
}

/*void printNetwork(double inputLayer[], double weights0[], double bias0[],
                  double hiddenLayer[], double weights1[], double bias1[],
                  double outputLayer[], void (*f)(double[]))

{
}*/



