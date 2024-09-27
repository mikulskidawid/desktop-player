#include <gtk/gtk.h>
#include <gtk-layer-shell/gtk-layer-shell.h>

#include "metadata.h"
#include "controls.h"

void create_widget(int h_pos, int v_pos, int h_margin, int v_margin){

    // Creating new GTK window for widget
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_layer_init_for_window(GTK_WINDOW(window));

    // Position 
    gtk_layer_set_layer(GTK_WINDOW(window), GTK_LAYER_SHELL_LAYER_BOTTOM);

    if(!v_pos){gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_TOP, TRUE);}
    else{gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_BOTTOM, TRUE);}

    if(!h_pos){gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_LEFT, TRUE);}
    else{gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_RIGHT, TRUE);}

    // Margin
    gtk_layer_set_margin(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_LEFT, h_margin);
    gtk_layer_set_margin(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_RIGHT, h_margin);
    gtk_layer_set_margin(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_TOP, v_margin);
    gtk_layer_set_margin(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_BOTTOM, v_margin);

    gtk_window_set_title(GTK_WINDOW(window), "DesktopPlayer");
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);

    // Containers
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *metadata_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *controls_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);

    gtk_container_add(GTK_CONTAINER(window), main_box);
    gtk_box_pack_start(GTK_BOX(main_box), metadata_box, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), controls_box, FALSE, FALSE, 0);

    // Metadata
    player_metadata(metadata_box);

    // Media Controls
    media_controls(controls_box);

    gtk_widget_show_all(window);
}

int main(int argc, char *argv[]){

    // Parameters
    int h_pos = 0;
    int v_pos = 0;
    int h_margin = 0;
    int v_margin = 0;

    if (argc != 9) {
        printf("Usage: %s -h_pos <h_pos> -v_pos <v_pos> -h_margin <h_margin> -v_margin <v_margin>\n", argv[0]);
        printf("    -h_pos - Horizontal position (0 - left | 1 - right)\n");
        printf("    -v_pos - Vertical position (0 - top | 1 - bottom)\n");
        printf("    -h_margin - Horizontal margin (px)\n");
        printf("    -v_margin - Vertical margin (px)\n");
        return 1;
    }

    // String to int conversion
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h_pos") == 0) {
            h_pos = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-v_pos") == 0) {
            v_pos = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-h_margin") == 0) {
            h_margin = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-v_margin") == 0) {
            v_margin = atoi(argv[++i]);
        }
    }

    gtk_init(&argc, &argv);

    create_widget(h_pos, v_pos, h_margin, v_margin);

    // Custom CSS
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider,
        "#controls_box { padding: 8px; }"
        "#title_label { font-size: 18px;}"
        "#artist_label { font-size: 14px;}", -1, NULL);

    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(), 
        GTK_STYLE_PROVIDER(css_provider), 
        GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_main();

    return 0;
}
