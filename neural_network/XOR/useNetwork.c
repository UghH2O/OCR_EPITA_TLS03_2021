#include "executeNetwork.h"
#include "trainNetwork.h"
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef struct Network
{
    //layers of the network, lenghes always matches the network's
    double input[2];
    double hidden[4];
    double output[2];

    //weights and biases of the network, lenghes always matches the network's
    double weights01[8];
    double weights12[8];
    double bias01[4];
    double bias12[2];
}Network;

void buildNetwork(double weights0[], double bias0[],
                  double weights1[], double bias1[],
                  size_t inputSize, size_t hiddenSize, size_t outputSize)
{
    srand(time(NULL));

    

    for (size_t i = 0; i < inputSize; i++)
    {
        for (size_t j = 0; j < hiddenSize; j++)
        {
            double x= rand()/(RAND_MAX+1.0);
            x = x*2-1;
            weights0[i * hiddenSize + j] = x;
        } 
    }
    
    for (size_t i = 0; i < hiddenSize; i++)
    {
        for (size_t j = 0; j < outputSize; j++)
        {
            double x= rand()/(RAND_MAX+1.0);
            x = x*2-1;
            weights1[i * outputSize + j] = x;
        } 
    }

    for (size_t i = 0; i < hiddenSize; i++)
    {
        double x = rand()/(RAND_MAX+1.0); 
        x = x*2-1;
        bias0[i] =x;
    }

    for (size_t k = 0; k < outputSize; k++)
    {
        double x= rand()/(RAND_MAX+1.0);
        x = x*2-1; 
        bias1[k] = x;
    }
}

void saveNetwork(double weights0[], double weights1[], double bias0[], double bias1[]
                , char filename[], size_t inputSize, size_t hiddenSize, size_t outputSize)
{
    FILE *fptr;
    fptr = fopen(filename, "w");
    if(fptr == NULL)
        printf("%s doesn't exist\n", filename);
    for(size_t i = 0; i < inputSize * hiddenSize; i++)
    {
        fprintf(fptr, "%f,", weights0[i]);
    }
    for(size_t i = 0; i < hiddenSize; i++)
    {
        fprintf(fptr, "%f,", bias0[i]);
    }
    for(size_t i = 0; i < hiddenSize * outputSize; i++)
    {
        fprintf(fptr, "%f,", weights1[i]);
    }
    for(size_t i = 0; i < outputSize; i++)
    {
        fprintf(fptr, "%f,", bias1[i]);
    }
    fclose(fptr);
}

void loadNetwork(double weights0[], double weights1[], double bias0[], double bias1[]
                , char filename[], size_t inputSize, size_t hiddenSize, size_t outputSize)
{
    FILE *fptr;
    fptr = fopen(filename, "r");
    if(fptr == NULL)
        printf("%s doesn't exist\n", filename);
    double content[inputSize * hiddenSize + hiddenSize * outputSize + hiddenSize + outputSize];
    for(size_t i = 0; i < inputSize * hiddenSize + hiddenSize * outputSize + hiddenSize + outputSize; i++)
    {
        int err = fscanf(fptr, "%lf,", &content[i]);
        if(err == EOF)
            printf("error while loading network's data");
            
    }
    for(size_t i = 0; i < inputSize * hiddenSize; i++)
    {
        weights0[i] = content[i];
    }
    for(size_t i = 0; i < hiddenSize * outputSize; i++)
    {
        weights1[i] = content[i + inputSize * hiddenSize];
    }
    for(size_t i = 0; i < hiddenSize; i++)
    {
        bias0[i] = content[i + inputSize * hiddenSize + hiddenSize * outputSize];
    }
    for(size_t i = 0; i < outputSize; i++)
    {
        bias1[i] = content[i + inputSize * hiddenSize + hiddenSize * outputSize + hiddenSize];
    }
    fclose(fptr);
}

void print_matrix(char s[], double m[], size_t rows, size_t cols)
{
    printf("%s =\n", s);
    for(size_t i = 0; i<rows; i++)
    {
        for(size_t j = 0; j<cols; j++)
        {
            printf("%4g ",m[i*cols + j]);
        }
        printf("\n");
    }
}

void train(Network *net)
{

    double input00[2] = {0, 0};
    double input01[2] = {0, 1};
    double input10[2] = {1, 0};
    double input11[2] = {0, 0};

    const double wantedsame[2] = {1, 0};
    const double wanteddiff[2] = {0, 1};

    double Costs[4];
    double avgCost = 100;
    double tmpCost = 100;
    do
    {
        double gradient00[22];
        double gradient01[22];
        double gradient10[22];
        double gradient11[22];

        ExecuteNetwork(input00, net->weights01, net->bias01, net->hidden, net->weights12, net->bias12, net->output, 0);
        buildGradientVector(gradient00, net->weights01, net->weights12, net->bias01,  net->bias12, input00, net->hidden, net->output, wantedsame);
       // printf("input : 00, output : (0 = %f) (1 = %f)\n", outputLayer[0], outputLayer[1]);
        Costs[0] = CostFunc(net->output, wantedsame, 2);

        ExecuteNetwork(input01, net->weights01, net->bias01, net->hidden, net->weights12, net->bias12, net->output, 0);
        buildGradientVector(gradient01, net->weights01, net->weights12, net->bias01,  net->bias12, input01, net->hidden, net->output, wanteddiff);
        //printf("input : 01, output : (0 = %f) (1 = %f)\n", outputLayer[0], outputLayer[1]);
        Costs[1] = CostFunc(net->output, wanteddiff, 2);

        ExecuteNetwork(input10, net->weights01, net->bias01, net->hidden, net->weights12, net->bias12, net->output, 0);
        buildGradientVector(gradient10, net->weights01, net->weights12, net->bias01,  net->bias12, input10, net->hidden, net->output, wanteddiff);
       // printf("input : 10, output : (0 = %f) (1 = %f)\n", outputLayer[0], outputLayer[1]);
        Costs[2] = CostFunc(net->output, wanteddiff, 2);

        ExecuteNetwork(input11, net->weights01, net->bias01, net->hidden, net->weights12, net->bias12, net->output, 0);
        buildGradientVector(gradient11, net->weights01, net->weights12, net->bias01,  net->bias12, input00, net->hidden, net->output, wantedsame);
       // printf("input : 11, output : (0 = %f) (1 = %f)\n", outputLayer[0], outputLayer[1]);
        Costs[3] = CostFunc(net->output, wantedsame, 2);

        for (size_t j = 0; j < 8; j++)
        {
            net->weights12[j] -= (gradient00[j] + gradient01[j] + gradient10[j] + gradient11[j]) / 20;
        }

        for (size_t j = 0; j < 2; j++)
        {
            net->bias12[j] -= (gradient00[j + 8] + gradient01[j + 8] + gradient10[j + 8] + gradient11[j + 8]) / 20;
        }

        for (size_t j = 0; j < 8; j++)
        {
            net->weights01[j] -= (gradient00[j + 10] + gradient01[j + 10] + gradient10[j + 10] + gradient11[j + 10]) / 20;
        }

        for (size_t j = 0; j < 4; j++)
        {
            net->bias01[j] -= (gradient00[j + 18] + gradient01[j + 18] + gradient10[j + 18] + gradient11[j + 18]) / 20;
        }

        tmpCost = avgCost;
        avgCost = (Costs[0] + Costs[1] + Costs[2] + Costs[3]) / 4;
        printf("%f\n", avgCost);
        saveNetwork(net->weights01, net->weights12, net->bias01, net->bias12, "test.txt", 2, 4, 2);

    } while (avgCost > 0.01 && tmpCost > avgCost);

    ExecuteNetwork(input00, net->weights01, net->bias01, net->hidden, net->weights12, net->bias12, net->output, 1);
    printf("COST : %f\n", CostFunc(net->output, wantedsame, 2));
    ExecuteNetwork(input01, net->weights01, net->bias01, net->hidden, net->weights12, net->bias12, net->output, 1);
    printf("COST : %f\n", CostFunc(net->output, wanteddiff, 2));
    ExecuteNetwork(input10, net->weights01, net->bias01, net->hidden, net->weights12, net->bias12, net->output, 1);
    printf("COST : %f\n", CostFunc(net->output, wanteddiff, 2));
    double input[2] = {1,1};
    ExecuteNetwork(input, net->weights01, net->bias01, net->hidden, net->weights12, net->bias12, net->output, 1);
    printf("COST : %f\n", CostFunc(net->output, wantedsame, 2));
}

void use()
{   
    Network net = {

        .input = {0,0},
        .hidden = {0,0,0},
        .output = {0,0},
        .weights01 = {0,0,0,0,0,0,0,0},
        .weights12 = {0,0,0,0,0,0,0,0},
        .bias01 = {0,0,00,},
        .bias12 = {0,0}
    };

    buildNetwork(net.weights01, net.bias01, net.weights12, net.bias12, 2, 4, 2);
    train(&net);
    
}
