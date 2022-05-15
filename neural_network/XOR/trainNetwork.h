#ifndef TRAIN_NETWORK_H
#define TRAIN_NETWORK_H

void train();
double CostFunc(double outputLayer[], const double wanted[], size_t size);
void buildGradientVector(double gradient[],
                         double weights0[], double weights1[], double bias0[], double bias1[],
                         double input[], double hidden[], double output[], const double wanted[]);
#endif