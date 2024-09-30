#ifndef VIEW_H
#define VIEW_H

#include "view_listener.h"
#include "fakenews.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/select.h>
#include <string.h>

typedef struct {
    View_listener listener;
} View;

void view_init(View *self, View_listener listener);
View *view_create(View_listener listener);
void view_reset(View *self);
void free_view(View *view);
int kbhit(void);
int handle_user_input(View *view);
void run(View *view);
void view_set_listener(View *self, View_listener listener);
#endif