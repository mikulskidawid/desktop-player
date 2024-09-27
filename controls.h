#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "playerctl.h"

// Update status button icon

gboolean update_status_btn(GtkButton *btn){
    char *status = get_playerctl_info("playerctl status");
    GtkWidget *image;

    if(status){
        if (strcmp(status, "Playing") == 0) {
        image = gtk_image_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_SMALL_TOOLBAR);
        gtk_button_set_image(btn, image);
        } 
        else if (strcmp(status, "Paused") == 0) {
            image = gtk_image_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_SMALL_TOOLBAR);
            gtk_button_set_image(btn, image);
        } 
        else {
            image = gtk_image_new_from_icon_name("media-playback-stop", GTK_ICON_SIZE_SMALL_TOOLBAR);
            gtk_button_set_image(btn, image);
        }
    }
    else {
        image = gtk_image_new_from_icon_name("media-playback-stop", GTK_ICON_SIZE_SMALL_TOOLBAR);
        gtk_button_set_image(btn, image);
    }
    return TRUE;
}

void play_pause(GtkButton *btn){
    update_status_btn(GTK_BUTTON(btn));
    system("playerctl play-pause");
}

void next_track(){system("playerctl next");}

void previous_track(){system("playerctl previous");}

// Media Controls
void media_controls(GtkWidget *controls_box){
    GtkWidget *status_btn = gtk_button_new_from_icon_name("media-playback-stop", GTK_ICON_SIZE_SMALL_TOOLBAR);
    GtkWidget *previous_btn = gtk_button_new_from_icon_name("media-skip-backward", GTK_ICON_SIZE_SMALL_TOOLBAR);
    GtkWidget *next_btn = gtk_button_new_from_icon_name("media-skip-forward", GTK_ICON_SIZE_SMALL_TOOLBAR);

    g_timeout_add(500, (GSourceFunc)update_status_btn, GTK_BUTTON(status_btn));

    g_signal_connect(status_btn, "clicked", G_CALLBACK(play_pause), GTK_BUTTON(status_btn));
    g_signal_connect(previous_btn, "clicked", G_CALLBACK(previous_track), NULL);
    g_signal_connect(next_btn, "clicked", G_CALLBACK(next_track), NULL);

    // Custom CSS
    gtk_widget_set_name(controls_box, "controls_box");

    gtk_box_pack_start(GTK_BOX(controls_box), previous_btn, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(controls_box), status_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(controls_box), next_btn, TRUE, FALSE, 0);

}