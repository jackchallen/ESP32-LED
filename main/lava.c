// Simple lava color

#include "app.h"

typedef struct c_s
{
   uint8_t cycle;
   uint8_t step;
   uint8_t pos;
} c_t;

typedef struct lava_s
{
   c_t r,
     g,
     b,
     w;
} lava_t;

const char *
applava (app_t * a)
{
   if (!a->cycle)
   {                            // Sanity check / defaults
      free (a->data);
      a->data = calloc (1, sizeof (lava_t));
      if (!a->colourset || (!a->r && !a->g && !a->b))
      {
         a->r = a->g = a->b = 255;
         a->colourset = 1;
      }
   }
   lava_t *lava = a->data;
   uint8_t l = a->fader;

   void next (c_t * c)
   {
      if (!c->step || c->cycle < c->step)
      {
         c->step = 256 / (a->speed + esp_random () % a->speed);
         c->pos = esp_random ();
         c->cycle = 255 - a->step / 2;
      } else
         c->cycle -= c->step;
   }
   next (&lava->r);
   next (&lava->g);
   next (&lava->b);
   next (&lava->w);

   for (unsigned int i = 0; i < a->len; i++)
      setRGBWl (a->start + i, (long) a->r * wheel[(256 * i / a->len + lava->r.pos) & 255] * (255 - cos8[lava->r.cycle]) / 255 / 255,    //
                (long) a->g * wheel[(256 * i / a->len + lava->g.pos) & 255] * (255 - cos8[lava->g.cycle]) / 255 / 255,  //
                (long) a->b * wheel[(256 * i / a->len + lava->b.pos) & 255] * (255 - cos8[lava->b.cycle]) / 255 / 255,  //
                (long) a->w * wheel[(256 * i / a->len + lava->w.pos) & 255] * (255 - cos8[lava->w.cycle]) / 255 / 255,  //
                l);
   return NULL;
}
