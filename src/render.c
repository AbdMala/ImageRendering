
#include "render.h"

#include <math.h>
#include <string.h>

#include "util.h"

// useful for converting degrees to radians
#define PI 3.14159265358979323846

// useful for mapping f into the range [0, k)
float float_mod(float f, int k) {
    // map into (-k, k)
    float res = fmodf(f, k);

    // fold into [0, k]
    if (res < 0) {
        res += k;
    }

    // accommodate for floating point rounding errors
    if (res >= k) {
        res = 0.0;
    }
    return res;
}

/** Move the player according to its velocities.
 */
void update_player(player_t* p, ctx_t const* ctx) {

    int w =  p->angle + p->v_angular ;
    int ww = (w % 360) +360;
    p->angle = ww % 360;
    /////////////////////////////////////////////////////
    /////////////////////////////////////////////////////

    float win = p->angle * (PI/180) ;

    float a = cosf(win) * p->v ;

    float b = sinf(win) * p->v ;


    float tmpx = p->x + a ;
    float tmpy = p->y - b ;
    if (tmpx < 0){
        tmpx = tmpx + ctx->map_size ;
    }
    if (tmpy < 0){
        tmpy = tmpy + ctx->map_size ;
    }
    float xc = float_mod(tmpx , ctx->map_size)  ;
    float yc = float_mod(tmpy , ctx->map_size) ;

    p->x = xc  ;
    p->y = yc ;

    ////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////


    int ms = ctx->map_size ;
    int ind = (int)p->x + (((int)p->y)*ms) ;
    int tmp = (ctx->height_map)[ind] ;

    int h = p->height + p->v_height ;

    if ( (h - tmp ) >= 20) {
        p->height = h;
    } else {
        p->height = tmp + 20 ;
    }



}

/** Draw a vertical line into the context's out buffer in the screen column u
 *  from screen row v_from (exclusively) to screen row v_to (inclusively).
 *
 *  For every call to this function, v_from <= v_to has to hold.
 *  If v_from == v_to holds, nothing is drawn.
 *
 *  Coordinates have their origin in the bottom left corner of the screen and
 *  are 1-based.
 */
void draw_line(ctx_t* c, int u, int v_from, int v_to, uint32_t color) {


    int wid = c->scr_width ;
    int hit = c->scr_height ;

    int spa = u - 1 ;
    int zei = hit - 1 ;

    if (v_from <= v_to) {
        for(int i = v_from  ; i < v_to ; i++){


            c->out[(zei-i)*wid+spa] = color ;
        }
    }



}

/** Render the scene from player's perspective into the context's out buffer.
 */
void render(const player_t* p, ctx_t* c) {

    for(int i = 0 ; i < c->scr_width * c->scr_height -1 ; i++){
        c->out[i] = c->sky_color ;
    }
    float wink = p->angle * (PI/180) ;


    for (int d  = c->distance   ; d > 0 ; d --){
        float a_end = cosf(wink) * d ;
        float b_end = sinf(wink) * d ;

        float Lx = p->x + a_end - b_end ;
        float Ly = p->y - b_end - a_end ;

        float Rx = p->x + a_end + b_end ;
        float Ry = p->y - b_end + a_end ;

        float t = 0.0 ;


        for(int u= 0 ; u < c->scr_width ; u++){
            float x = Lx + t*(Rx - Lx) ;
            float y = Ly + t*(Ry - Ly) ;

            if (x < 0){
                x = x + c->map_size ;
            }
            if (y < 0){
                y = y + c->map_size ;
            }


            float qx = float_mod(x , c->map_size) ;
            float qy = float_mod(y , c->map_size) ;
            if (t >= 0.0 && t <= 1.0 ){

                t = t + (1.0/((float)c->scr_width - 1.0)) ;
            }

            int var = (int)qx + ((int)qy * c->map_size) ;
            int Qz = (c->height_map)[var] ;

            float Ws = (float)c->scr_width / 2 ;
            float Hih = ((float)Qz - (float)p->height) / (float) d ;
            float Hs = (float) c->scr_height / 2 ;

            int Vs = Ws * Hih + Hs ;
            if (Vs < 0) {
                Vs = 0 ;
            }
            else if (Vs  > c->scr_height) {
                Vs = c->scr_height ;
            }

            draw_line(c, u ,0 , Vs , c->color_map[var] ) ;



        }

    }



}

int bonus_implemented(void) {
    // TODO change to 1 if the bonus exercise is implemented.
    return 0;
}