#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "playerctl.h"

// Creates new pixbuf for media art and rescales image
GdkPixbuf* load_and_scale_image(const char *filepath) {
    GError *error = NULL;
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filepath, &error);

    if (!pixbuf) {
        g_printerr("Coundn't load an image: %s\n", error->message);
        g_clear_error(&error);
        return NULL;
    }

    int original_width = gdk_pixbuf_get_width(pixbuf);
    int original_height = gdk_pixbuf_get_height(pixbuf);

    int new_width, new_height;

    // Calculate new dimensions 

    if (original_width > original_height) {
        new_width = 240;
        new_height = (240.0 / original_width) * original_height;
    } 
    else {
        new_height = 240;
        new_width = (240.0 / original_height) * original_width;
    }

    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, new_width, new_height, GDK_INTERP_BILINEAR);
    g_object_unref(pixbuf);

    return scaled_pixbuf;
}

// Set song name from playerctl
static gboolean update_song_title(GtkWidget *label) {
    char *text = get_playerctl_info("playerctl metadata title");
    if(text){gtk_label_set_label(GTK_LABEL(label), text);}
    else{gtk_label_set_label(GTK_LABEL(label), "-");}
    free(text);
    return TRUE;
}

// Set artist name from playerctl
static gboolean update_song_artist(GtkWidget *label) {
    char *text = get_playerctl_info("playerctl metadata artist");
    if(text){gtk_label_set_label(GTK_LABEL(label), text);}
    else{gtk_label_set_label(GTK_LABEL(label), "-");}
    free(text);
    return TRUE;
}

// Set album cover from url
static gboolean set_album_art_url(GtkWidget *image) {

    char *metadata = get_playerctl_info("playerctl metadata");
    const char *art_key = "mpris:artUrl";
    char *art_url = NULL;

    if (metadata != NULL) {
        char *art_pos = strstr(metadata, art_key);
        if (art_pos != NULL) {
            art_pos += strlen(art_key);

            char *end_pos = strchr(art_pos, '\n');
            if (end_pos != NULL) {
                *end_pos = '\0';
            }

            art_url = strdup(art_pos);
            char *decoded_art_url = url_decode(art_url);
            char *trimmed_art_url = trim_whitespace(decoded_art_url);
            free(art_url); free(decoded_art_url);

            // Remove "file://" prefix
            const char *file_prefix = "file://";
            if (strncmp(trimmed_art_url, file_prefix, strlen(file_prefix)) == 0) {
                char *local_path = strdup(trimmed_art_url + strlen(file_prefix));
                free(trimmed_art_url);
                trimmed_art_url = local_path;
            }
            GdkPixbuf *scaled_pixbuf = load_and_scale_image(trimmed_art_url);
            gtk_image_set_from_pixbuf(GTK_IMAGE(image), scaled_pixbuf);
        }
        else{
            gtk_image_clear(GTK_IMAGE(image));
        }
        free(metadata);
    }

    return TRUE;
}

// Metadata container
void player_metadata(GtkWidget *metadata_box){
    GtkWidget *image = gtk_image_new();
    GtkWidget *title_label = gtk_label_new("-");
    GtkWidget *artist_label = gtk_label_new("-");

    // Label wrap
    gtk_widget_set_size_request(GTK_WIDGET(title_label), 240, -1);
    gtk_widget_set_size_request(GTK_WIDGET(artist_label), 240, -1);

    gtk_label_set_line_wrap(GTK_LABEL(title_label), TRUE);
    gtk_label_set_line_wrap(GTK_LABEL(artist_label), TRUE);

    // Update CSS
    gtk_widget_set_name(title_label, "title_label");
    gtk_widget_set_name(artist_label, "artist_label");

    // Update functions
    g_timeout_add(500, (GSourceFunc)update_song_title, title_label);
    g_timeout_add(500, (GSourceFunc)update_song_artist, artist_label);
    g_timeout_add(500, (GSourceFunc)set_album_art_url, image);

    gtk_box_pack_start(GTK_BOX(metadata_box), image, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(metadata_box), title_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(metadata_box), artist_label, FALSE, FALSE, 0);
}
