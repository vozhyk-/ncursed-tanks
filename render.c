#include "client.h"

void draw_tank(int pos_x, int pos_y, int x, int y, int angle)
{
    int xx = x-pos_x;
    int yy = y-pos_y;
    if (yy > 0 && yy < (LINES-1))
        if (xx > 0 && xx < (COLS-1))
            put_col_str(COL_W, yy, xx-1, "<*>");
        else
            if (xx == 0)
                put_col_str(COL_W, yy, xx, "*>");
            else if (xx == (COLS-1))
                put_col_str(COL_W, yy, xx-1, "<*");
}

void draw_map(map_t map, int pos_x, int pos_y, int width, int height)
{
    int end = pos_x+width;
    for (int i = pos_x; i < end && i < map_data->length; i++)
    {
        int yy = map[i]-pos_y;
        int xx = i-pos_x;
        put_col_str(COL_G, yy, xx, "V");
        put_col_str(COL_C, yy+1, xx, "H");
        put_col_str(COL_M, yy+2, xx, "W");
    }
}

void render_map()
{
    draw_map(g_map, dx, dy, COLS, LINES);
}
