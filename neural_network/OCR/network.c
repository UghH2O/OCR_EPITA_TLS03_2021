#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "execute.h"
#include "train.h"

//function called by the 
int net_train() {

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

    loadNetwork(&net, "neural_network/OCR/network.txt", INPUT_LEN, HIDDEN_LEN, OUTPUT_LEN);

    train_network(&net);    

    exit(0);
}