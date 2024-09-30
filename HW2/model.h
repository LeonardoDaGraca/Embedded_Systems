#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int temp;
} Model;

void model_init(Model *self, int temp);
Model *model_create(int temp);
void model_reset(Model *self);
void free_model(Model *model);
int get_temp(Model *self);

#endif // MODEL_H