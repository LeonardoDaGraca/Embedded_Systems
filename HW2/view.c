/*
 * Leonardo DaGraca
 * HW2: Coding a Controller
 * view.c file: View component
 *
 * references: https://stackoverflow.com/questions/448944/c-non-blocking-keyboard-input
 */
#include "view.h"


//Constructor to init a static allocation of view
//For attribute init
void view_init(View *self, View_listener listener) {
    if (self) self->listener = listener;
}

View *view_create(View_listener listener) {
    View *instance = (View * )(malloc(sizeof(View)));
    if (!instance) {
        fprintf(stderr, "Allocation Failed!\n");
        return NULL;
    }
    view_init(instance, listener);
    return instance;
}

void view_reset(View *self) {
}

void free_view(View *view) {
    if (view) {
        view_reset(view);
        free(view);
    }
}

//kbhit function to check for user input (non-blocking)
int kbhit(void) {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int handle_user_input(View *view) {
    char input[20];
    int new_temp;

    if (fgets(input, sizeof(input), stdin)) {
        if (sscanf(input, "t %d", &new_temp) == 1) {
            view->listener.set_temp(new_temp); // Set new temperature through the listener
            printf("Set Temperature: %dF\n", new_temp);
        } else if (strcmp(input, "x\n") == 0) {
            view->listener.user_exit(); // Exit the program through the listener
            return 1; // Signal to exit
        }
    }
    return 0; // Continue running
}

void run(View *view) {
    int current_temp = 72;
    int desired_temp;
    int exit_flag = 0;

    printf("Welcome to Den\n");
    //user to set the initial temperature
    printf("Set Temperature: ");
    if (scanf("%d", &desired_temp) == 1) {
        current_temp = desired_temp;
        printf("Set Temperature: %dF\n", desired_temp);
    }

    //start the simulation
    while (!exit_flag) {
        sleep(3);
        int fakenews_temp = fakenews();

        //adjust temperature towards desired_temp
        if (current_temp < desired_temp) {
            current_temp++;
        } else if (current_temp > desired_temp) {
            current_temp--;
        } else {
            current_temp = fakenews_temp;
        }

        printf("Current Temperature: %dF\n", current_temp); // Report the adjusted temperature

        //check for user input without blocking the temperature updates
        if (kbhit()) {
            char input[20];
            if (fgets(input, sizeof(input), stdin)) {
                int new_temp;
                if (sscanf(input, "t %d", &new_temp) == 1) {
                    desired_temp = new_temp; //update the desired temperature based on user input
                    printf("Set Temperature: %dF\n", desired_temp);
                } else if (strcmp(input, "x\n") == 0) {
                    exit_flag = 1; //signal to exit the program
                }
            }
        }
    }
    printf("User has exited the program\n");
}

void view_set_listener(View *self, View_listener listener) {
    self->listener = listener;
}
