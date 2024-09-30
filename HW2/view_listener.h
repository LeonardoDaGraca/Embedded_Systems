#ifndef VIEW_LISTENER_H
#define VIEW_LISTENER_H

typedef struct {
    void (* set_temp)(int temp);
    void (* user_exit)(void);
    int (*get_temp)(void);
} View_listener;

#endif



