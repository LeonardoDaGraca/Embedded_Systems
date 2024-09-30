#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model.h"
#include "view.h"
#include "view_listener.h"
#include <stdlib.h>

typedef struct {
    Model *model;
    View *view;
} Controller;

void controller_init(Controller *self, Model *model, View *view);
Controller *controller_create(Model *model, View *view);
void controller_reset(Controller *self);
void free_controller(Controller *controller);
void controller_set_temp(Controller *self, int new_temp);
void controller_user_exit(Controller *self);
int controller_get_temp(Controller *self);

#endif
