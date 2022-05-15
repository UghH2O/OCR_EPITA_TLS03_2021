#include "train.h"

//calculates the cost of an execution
double cost(double wanted[],network *net)
{
    double cost = 0;
    double nbnodes = OUTPUT_LEN;

    for (size_t i = 0; i < nbnodes; i++)
        cost += (net->output[i] - wanted[i]) * (net->output[i] - wanted[i]);

    return cost / nbnodes;
}

// compute a part of the gradient of a weights or a bias 
// between the input and the hidden layers
// compute precizely the gradient of the indexact activation of the output
double gradientIH(size_t indexelmt, size_t indexact, 
                    double wanted[], network *net) {

    double dZap = net->hidden[indexelmt];

    double dAsz = 1;
    if(indexact == indexelmt)
        dAsz = net->output[indexact] * ( 1 - net->output[indexact]);
    else
        dAsz = (-1) * net->output[indexact] * net->output[indexelmt];

    double dCas = 2 * (net->output[indexact] - wanted[indexact]);

    return dZap * dAsz * dCas;
}

// compute a part of the gradient of a weights or a bias 
// between the input and the hidden layers
// call the gradientIH function for all the output layer
double gradientHidden(size_t indexelmt, size_t indexact, 
                        double wanted[], Type type, network *net) {

    double dZelm = 1;
    if (type == WEIGHT){
        dZelm = net->input[indexelmt];
    }

    double dAz = 1;
    if(indexact == indexelmt)
        dAz = net->hidden[indexact] * ( 1 - net->hidden[indexact]);
    else
        dAz = (-1) * net->hidden[indexact] * net->hidden[indexact];

    double partial = dAz * dZelm;

    double sum = 0;

    for(size_t i = 0; i < OUTPUT_LEN; i++){

        sum += (gradientIH(indexact, i, wanted, net) * partial);
    }

    return sum;
}


// comput the gradient for a bias or a weight 
// between the hidden and the output layer
double gradientElmt(size_t indexelmt, size_t indexact, 
                double wanted[], Type type, network *net) {

    double dZelm = 1;
    if (type == WEIGHT){
        dZelm = net->hidden[indexelmt];
    }

     double dAz = 1;
    if(indexact == indexelmt)
        dAz = net->output[indexact] * ( 1 - net->output[indexact]);
    else
        dAz = (-1) * net->output[indexact] * net->output[indexelmt];

    double dCa = 2 * (net->output[indexact] - wanted[indexact]);

    return dZelm * dAz * dCa;
}

//computes the gradient of an execution
void gradientVector(double gradient[], double wanted[],network *net) {

    size_t index = 0;

    //Weights from input to hidden
    for (size_t i = 0; i < INPUT_LEN; i++)
    {
        for (size_t j = 0; j < HIDDEN_LEN; j++)
        {
            gradient[index] = gradientHidden(i, j, wanted, WEIGHT, net);
            index++;
        }
    }

    //Bias from input to hidden
    for (size_t i = 0; i < HIDDEN_LEN; i++)
    {
        gradient[index] = gradientHidden(i, i, wanted, BIAS, net);
        index++;
    }

    //Weights from hidden to output
    for (size_t i = 0; i < HIDDEN_LEN; i++)
    {
        for (size_t j = 0; j < OUTPUT_LEN; j++)
        {
            gradient[index] = gradientElmt(i, j, wanted, WEIGHT, net);
            index++;
        }
    }

    //Bias from hidden to output
    for (size_t i = 0; i < OUTPUT_LEN; i++)
    {
        gradient[index] = gradientElmt(i, i, wanted, BIAS, net);
        index++;
    }
}

//returns the output layer of the network for a given input
void execute(network *net)
{
    nextLayer(net->weights01, net->input, net->bias01, 
                    net->hidden, INPUT_LEN, HIDDEN_LEN);
    nextLayer(net->weights12, net->hidden, net->bias12, 
                    net->output, HIDDEN_LEN, HIDDEN_LEN);
}

//trains the network using the gradient descent algorithm
void train_network(network *net)
{
    //create inputs (load matrix of letters)
    trainingElement trainElmts[5][28];

    const char result[] = 
     "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz&,?!-()'0123456789";

    // load the elements for the training and put them in a matrix
    for (size_t i = 0; i < 5; i++){
        for(size_t j = 0; j < 28; j++){
            trainingElement t = 
            {
                .image = {},
                .answer = result[(i * 28 + j) % 70],
                .wanted = {}
            };
            for(size_t j = 0; j < 70; j++){
                if(j == i % 70){
                    t.wanted[j] = 1;
                } else {
                    t.wanted[j] = 0;
                }
            }
            char filename[6] = {result[(i * 28 + j) % 70], 
                                '.', 't', 'x', 't', '\0'};
            loadTraining(t.image, filename, INPUT_LEN, i * 28 + j);
            trainElmts[i][j] = t;
        }
    }

    unsigned long minibatch = 0;
    int compteur = 0;
    
    // training loop
    while (compteur < 300)
    {   
        double gradient[GRADIENT_LEN] = {};
        for(size_t j = 0; j < GRADIENT_LEN; j++){
            gradient[j] = 0;
        }
        
        double Cost = 0;
        
        if (minibatch >= 1)
            minibatch = 0;

        // get the cost and the gradient from all the training elements
        for (size_t i = 0; i < 28; i++)
        {  
            for (size_t t = 0; t < INPUT_LEN; t++)
            {
                net->input[t] = trainElmts[(minibatch + i)%5][i].image[t];
            }
            //execute
            execute(net);

            //make cost
            Cost += cost(trainElmts[minibatch][i].wanted,net);

            //build gradient
            double currGrad[GRADIENT_LEN] = {};
        
            gradientVector(currGrad,trainElmts[minibatch][i].wanted,net);

            for (size_t j = 0; j < GRADIENT_LEN; j++)
                gradient[j] += currGrad[j];
        }

        for (size_t t = 0; t < GRADIENT_LEN; t++){
            gradient[t] = gradient[t] / 28;
        }

        if(compteur%5 == 0)
            printf("train : %d\tcost = %lf\n",compteur, Cost);

        //propagate gradient
        int offset = 0;
        for (size_t j = 0; j < INPUT_LEN*HIDDEN_LEN; j++)
        {
            net->weights01[j] -= gradient[j] * 0.3;
        }

        offset += INPUT_LEN*HIDDEN_LEN;
        
        for (size_t j = 0; j < HIDDEN_LEN; j++)
        {
            net->bias01[j] -= gradient[j + offset] * 0.3;
        }

        offset+=HIDDEN_LEN;
        for (size_t j = 0; j < HIDDEN_LEN*OUTPUT_LEN; j++)
        {
            net->weights12[j] -= gradient[j + offset] * 0.3;
        }

        offset+= HIDDEN_LEN*OUTPUT_LEN;
        for (size_t j = 0; j < OUTPUT_LEN; j++)
        {
            net->bias12[j] -= gradient[j + offset] * 0.3;
        }

        //save network (in each loop so the program can be interrupted easily)
        saveNetwork(net, "network.txt",INPUT_LEN,HIDDEN_LEN,OUTPUT_LEN);

        minibatch++;
        compteur++;
    }
    saveNetwork(net, "network.txt",
                    INPUT_LEN,HIDDEN_LEN,OUTPUT_LEN);
}
