/*
 * Leonardo DaGraca
 * HW2: Coding a Controller
 * main.c file: calls mvc components and runs simulation
 */
#include "fakenews.h"
#include "view.h"
#include "model.h"
#include "den_controller.h"


int main() {
    //init model, view, and controller
    Model *model = model_create(72);
    View *view = view_create((View_listener) {0});
    Controller *controller = controller_create(model, view);

    run(view);

    //free all components
    free_controller(controller);
    free_view(view);
    free_model(model);

    return 0;
}