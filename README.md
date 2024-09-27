# desktop-player
Simple desktop widget for media control in Linux.

![alt text](https://github.com/mikulskidawid/desktop-player/blob/main/widget_screenshot.png "creenshot")

Widget was made in C with GTK and works in Wayland only. It uses playerctl for media controling.

Usage: -h_pos <h_pos> -v_pos <v_pos> -h_margin <h_margin> -v_margin <v_margin>

    -h_pos - Horizontal position (0 - left | 1 - right)
    -v_pos - Vertical position (0 - top | 1 - bottom)
    -h_margin - Horizontal margin (px)
    -v_margin - Vertical margin (px)
