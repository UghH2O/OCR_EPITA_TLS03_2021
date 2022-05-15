#include "tools.h"

// weights * layer = Nextlayer
void MutltMAtrix(double weights[], double fromNeurons[], double toNeurons[]
                 , size_t w_r, size_t w_c, size_t n_c)
{
    for (size_t i = 0; i < w_r; i++){
        for (size_t j = 0; j < n_c; j++){
            toNeurons[i * n_c + j] = 0;
            for (size_t k = 0; k < w_c; k++){
                toNeurons[i * n_c + j] +=
                    weights[i * w_c + k] * fromNeurons[k * n_c + j];
            }
        }
    }
}
// nextLayer += bias
void AddMAtrix(double Neurons[], double bias[], size_t size)
{
    for(size_t i = 0; i < size; i++){
        Neurons[i] = Neurons[i] + bias[i];
    }
}

// nextLayer = ReLu(nextLayer)
void ReLu(double neurons[], size_t size)
{
    for(size_t i = 0; i < size; i++){
        if(neurons[i] < 0){
            neurons[i] = 0;
        }
    }
}

// nextLayer = sigmoid(nextLayer)
void Sigmoid(double neurons[], size_t size)
{
    for(size_t i = 0; i < size; i++){
        neurons[i] = 1 / (1 + exp( -neurons[i]));
    }
}

// nextLayer = sortmax(nextLayer)
void SoftMax(double neurons [], size_t size)
{
    double somme = 0;
    for(size_t i = 0; i < size; i++){
        somme += exp(neurons[i]);
    }
    for(size_t i = 0; i < size; i++){
        neurons[i] = exp(neurons[i]) / somme;
    }
}

/*
    calculate the next layer : func(weights*layer+bias) = nextlayer
*/
void nextLayer(double weihgts[], double fromNeurons[], double bias[],
               double toNeurons[],
               size_t w_c, size_t outSize)
{
    // weights * layer = Nextlayer
    MutltMAtrix(weihgts,fromNeurons,toNeurons,outSize,w_c,1);
    // nextLayer += bias
    AddMAtrix(toNeurons,bias,outSize);
    // nextLayer = f(nextLayer)
    SoftMax(toNeurons,outSize);
}


/*
    NETWORK HANDLING
*/

// build the neural network
// All weights and bias will be random float between -1 and 1
void buildNetwork(network *net, 
            size_t inputSize, size_t hiddenSize, size_t outputSize)
{
    srand(time(NULL));

    for (size_t i = 0; i < inputSize; i++)
    {
        for (size_t j = 0; j < hiddenSize; j++)
        {
            double x= rand()/(RAND_MAX+1.0);
            x = x*2-1;
            net->weights01[i * hiddenSize + j] = x;
        } 
    }

    for (size_t i = 0; i < hiddenSize; i++)
    {
        for (size_t j = 0; j < outputSize; j++)
        {
            double x= rand()/(RAND_MAX+1.0);
            x = x*2-1;
            net->weights12[i * outputSize + j] = x;
        } 
    }

    for (size_t i = 0; i < hiddenSize; i++)
    {
        double x = rand()/(RAND_MAX+1.0); 
        x = x*2-1;
        net->bias01[i] =x;
    }

    for (size_t k = 0; k < outputSize; k++)
    {
        double x= rand()/(RAND_MAX+1.0);
        x = x*2-1; 
        net->bias12[k] = x;
    }
}


// save the Network in a file named filename
void saveNetwork(network *net, char filename[], 
            size_t inputSize, size_t hiddenSize, size_t outputSize)
{
    FILE *fptr;
    fptr = fopen(filename, "w");
    if(fptr == NULL)
        printf("%s doesn't exist\n", filename);
    for(size_t i = 0; i < inputSize * hiddenSize; i++)
    {
        fprintf(fptr, "%lf,", net->weights01[i]);
    }
    for(size_t i = 0; i < hiddenSize; i++)
    {
        fprintf(fptr, "%lf,", net->bias01[i]);
    }
    for(size_t i = 0; i < hiddenSize * outputSize; i++)
    {
        fprintf(fptr, "%lf,", net->weights12[i]);
    }
    for(size_t i = 0; i < outputSize; i++)
    {
        fprintf(fptr, "%lf,", net->bias12[i]);
    }
    fclose(fptr);
}


// load the network saved in a file named filename
void loadNetwork(network *net, char filename[], 
            size_t inputSize, size_t hiddenSize, size_t outputSize)
{
    FILE *fptr;
    fptr = fopen(filename, "r");
    if(fptr == NULL)
        printf("%s doesn't exist\n", filename);

    double content[inputSize * hiddenSize + hiddenSize 
                * outputSize + hiddenSize + outputSize];

    for(size_t i = 0; i < inputSize * hiddenSize + 
                hiddenSize * outputSize + hiddenSize + outputSize; i++)
    {
        int err = fscanf(fptr, "%lf,", &content[i]);
        if(err == EOF)
            printf("error while loading network's data\n");
            
    }
    for(size_t i = 0; i < inputSize * hiddenSize; i++)
    {
        net->weights01[i] = content[i];
    }
    for(size_t i = 0; i < hiddenSize; i++)
    {
        net->bias01[i] = content[i + inputSize * hiddenSize];
    }
    for(size_t i = 0; i < hiddenSize * outputSize; i++)
    {
        net->weights12[i] = content[i + inputSize * 
                    hiddenSize + hiddenSize * outputSize];
    }
    for(size_t i = 0; i < outputSize; i++)
    {
        net->bias12[i] = content[i + inputSize * hiddenSize + hiddenSize * 
                                    outputSize + hiddenSize];
    }
    fclose(fptr);
}


// load an matrix for the training saved in a file which is name end by end
void loadTraining(double array[], char end[], size_t size, size_t index)
{  
    FILE *fptr;
    char filename[50] = "set_training/";
    if(index%70 < 26){
        char send[7] = {end[0], 
                        (char) (end[0] + 32), '.', 't', 'x', 't', '\0'};
        if (index >= 70){
            strcat(filename, "arial/");
        } else {
            strcat(filename, "times/");
        }
        strcat(filename, send);
    } else {
        if (index >= 70){
            strcat(filename, "arial/");
        } else {
            strcat(filename, "times/");
        }
        strcat(filename, end);
    }
    fptr = fopen(filename, "r");
    if(fptr == NULL)
        errx(1,"%s doesn't exist\n", filename);
    for(size_t j = 0; j < size; j++){
        int err = fscanf(fptr, "%lf,", &array[j]);
        if(err == EOF)
            errx(1, "error while loading traings data %lu\n", index);
    }
    fclose(fptr);
}