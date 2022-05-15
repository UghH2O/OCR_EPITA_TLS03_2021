#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "execute.h"
#include "train.h"

// main function to execute the neural network separately
int main() {

    network net = 
    {
        .input = {},
        .hidden = {},
        .output = {},

        .weights01 = {},
        .weights12 = {},
        .bias01 = {},
        .bias12 = {}
    };

    loadNetwork(&net, "network.txt", INPUT_LEN, HIDDEN_LEN, OUTPUT_LEN);

    //train the network
    train_network(&net);    

    exit(0);
}