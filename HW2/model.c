/*
 * Leonardo DaGraca
 * HW2: Coding a Controller
 * model.c file: Model component
 */
#include "model.h"

//Constructor to init a static allocation of model
//For attribute init
void model_init(Model *self, int temp) {
    if (self) self->temp = temp;
}

//Dynamic allocation and init of model
Model *model_create(int temp) {
    Model *instance = (Model *) malloc(sizeof(Model));
    if(!instance) {
        fprintf(stderr, "Allocation Failed!\n");
        return NULL;
    }
    model_init(instance, temp);
    return instance;
}

void model_reset(Model *self) {
}

void free_model(Model *model) {
    if (model) {
        model_reset(model);
        free(model);
    }
}

int get_temp(Model *self) {
    if(!self) return -1;
    return self->temp;
}