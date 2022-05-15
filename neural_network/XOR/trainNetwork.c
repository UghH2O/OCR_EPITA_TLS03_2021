#include "executeNetwork.h"
#include <math.h>
#include <stdio.h>
#include "useNetwork.h"

double CostFunc(double outputLayer[], const double wanted[], size_t size)
{
    double cost = 0;

    double nbnode = 0;
    for (size_t i = 0; i < size; i++)
    {
        cost += (outputLayer[i] - wanted[i]) * (outputLayer[i] - wanted[i]);
        nbnode++;
    }
    //printf("COST : %f\n", cost/nbnode);
    return cost / nbnode;
}

double buildGradientElem(int indexi, int indexj, double output[], double previous[], double weight[], const double wanted[], int weightcols, char type)
{
    double dCa = (output[indexj] - wanted[indexj]);
    /*
    double U = exp(output[indexi]);
    double V = 0;
    for (int i = 0; i < size; i++)
    {
        V += exp(output[i]);
    }

    double daZ = (U * U - U * V) / (V * V);
    */
    double daZ = 1;
    // if (output[indexi] < 0) {daZ = 0;}

    double dZelem = 1;
    if (type == 'b')
    {
        dZelem = 1;
    }
    else if (type == 'w')
    {
        dZelem = previous[indexi];
    }
    else if (type == 'a')
    {
        dZelem = weight[indexi * weightcols + indexj];
    }
    return dCa * daZ * dZelem;
}


double buildGradienthidden(int indexi, int indexj, double output[],double input[], double weight[], const double wanted[], int weightcols, char type)
{
    double dCa0 = (output[0] - wanted[0]);
    double dCa1 = (output[1] - wanted[1]);

    double dZelem = 1;
    if (type == 'b')
    {
        dZelem = 1;
    }
    else if (type == 'w')
    {
        dZelem = input[indexi];
    }

    double dZZ0 = weight[indexj * weightcols];
    double dZZ1 = weight[indexj * weightcols + 1];

    return (dCa0*dZZ0 + dCa1*dZZ1)*dZelem;
}

void buildGradientVector(double gradient[],
                         double weights0[], double weights1[], double bias0[], double bias1[],
                         double input[], double hidden[], double output[], const double wanted[])
{
    for (size_t j = 0; j < 8; j++)
    {
        gradient[j] = buildGradientElem( j % 4, (j - j % 4) / 4, output, hidden, weights1, wanted, 4, 'w');
    }

    for (size_t j = 0; j < 2; j++)
    {
        gradient[j + 8] = buildGradientElem( j, j, output, hidden, bias1, wanted, 1, 'b');
    }

    for (size_t j = 0; j < 8; j++)
    {
        gradient[j + 10] = buildGradienthidden(j%2,(j - j % 2) / 2,output,input, weights0, wanted, 2, 'w');

    }

    for (size_t j = 0; j < 4; j++)
    {
        gradient[j + 18] = buildGradienthidden(j,j,output, input, bias0, wanted, 1, 'b');
    }
}