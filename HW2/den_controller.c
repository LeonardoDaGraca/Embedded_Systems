/*
 * Leonardo DaGraca
 * HW2: Coding a Controller
 * den_controller.c file: Controller Component
 */
#include "den_controller.h"

void controller_init(Controller *self, Model *model, View *view) {
    if (self) {
        self->model = model;
        self->view = view;
    }
}

Controller *controller_create(Model *model, View *view) {
    Controller *instance = (Controller *) malloc(sizeof(Controller));
    if (!instance) {
        fprintf(stderr, "Allocation Failed!\n");
        return NULL;
    }
    controller_init(instance, model, view);
    View_listener listener = {
            .set_temp = (void (*)(int)) controller_set_temp,
            .user_exit = (void (*)(void)) controller_user_exit,
            .get_temp = (int (*)(void)) controller_get_temp
    };
    view_set_listener(view, listener);
    return instance;
}

void controller_reset(Controller *self) {
}

void free_controller(Controller *controller) {
    if (controller) {
        controller_reset(controller);
        free(controller);
    }
}

//update temp in the model and inform the view
//void controller_set_temp(Controller *self, int new_temp) {
//    if (self && self->model) {
//        self->model->temp = new_temp; // Update the model temperature
//        printf("Set Temperature: %dF\n", new_temp);
//    }
//}

//handle user exit
void controller_user_exit(Controller *self) {
    printf("Exiting the program...\n");
}

//get current temp from model
int controller_get_temp(Controller *self) {
    if(self && self->model) {
        return self->model->temp;
    }
    return -1;
}

void controller_set_temp(Controller *self, int new_temp) {
    if (self && self->model && self->view) {
        self->model->temp = new_temp; //update model
        printf("Current Temperature: %dF\n", get_temp(self->model)); //report new current temperature
    }
}
