#include "../../headers/minirt.h"

void    free_all(t_rt *rt)
{
    // freeing mlx and img (idk how that is going)
    free(rt->sp);
    free(rt->pl);
    free(rt->cy);
}