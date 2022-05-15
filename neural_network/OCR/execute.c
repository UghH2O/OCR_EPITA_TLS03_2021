#include "execute.h"

//returns the output of the neural network for a given image of a letter
char getChar(double input[],  network *net)
{
    //execution of the network
    const char result[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz&,?!-()'0123456789";
    

    nextLayer(net->weights01, input,net->bias01,net->hidden,INPUT_LEN,HIDDEN_LEN);
    nextLayer(net->weights12,net->hidden,net->bias12,net->output,HIDDEN_LEN,OUTPUT_LEN);    
    
    //search of the index int the output layer with the biggest activation
    size_t max = 0;
    for (size_t i = 0; i < OUTPUT_LEN; i++)
    {
        if(net->output[i] > net->output[max])
            max = i;
    }

    if(result[max] == '&')
        return '.';
    return result[max]; 
}