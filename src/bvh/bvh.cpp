#include <vector>

#include "bvh/bvh.h"

BVH::BVH(std::vector<Shape *> &shapes_list, int i0, int i1, int axis)
{
    if( i1 - i0 == 0 ) {
        izq = der = shapes_list[i0];

        aabb = shapes_list[i0]->get_AABB();
    }
    else if( i1 - i0 == 1 ) {
        izq = shapes_list[i0];
        der = shapes_list[i1];

        aabb = surround(shapes_list[i0]->get_AABB(), shapes_list[i1]->get_AABB());
    }
    else {
        for( int i = i0; i <= i1; ++i)
            aabb = surround(aabb, shapes_list[i]->get_AABB());

        // Partimos por el eje x la primera vez.
        //double pivot = (aabb.maximo.x() - aabb.minimo.x()) / 2.0f;
        double pivot = (aabb.maximo.e[axis % 3] - aabb.minimo.e[axis % 3]) / 2.0f;

        // Reordenamos la lista de objetos de acuerdo al pivote elegido.
        int div = divide_space(shapes_list, i0, i1, pivot, (axis % 3));

        // Generamos nuevas ramas para nuestro árbol con la nueva lista.
        //izq = create_subtree(shapes_list, i0, div, 0);
        //der = create_subtree(shapes_list, div+1, i1, 0);
        izq = new BVH(shapes_list, i0, div, (axis + 1) % 3);
        der = new BVH(shapes_list, (div + 1), i1, (axis + 1) % 3);
    }
}

bool BVH::hit(const Ray &r, double min_dist, double max_dist, HitRecord &hit) const
{
    bool    h_izq = false,
            h_der = false;

    // Si el rayo pasa por la aabb, comprobamos las ramas.
    if( aabb.hit(r, min_dist, max_dist) ) {
        h_izq = izq->hit(r, min_dist, max_dist, hit);
        h_der = der->hit(r, min_dist, max_dist, hit);
    }

    return ( h_izq || h_der );
};

bool BVH::shadow_hit(const Ray &r, double min_dist, double max_dist) const
{
    bool    h_izq = false,
            h_der = false;

    // Si el rayo pasa por la aabb, comprobamos las ramas.
    if( aabb.hit(r, min_dist, max_dist) ) {
        h_izq = izq->shadow_hit(r, min_dist, max_dist);
        h_der = der->shadow_hit(r, min_dist, max_dist);
    }

    return ( h_izq || h_der );
};

Shape *BVH::create_subtree(std::vector<Shape *> &shapes_list, int i0, int i1, int axis)
{
    return nullptr;
}

int BVH::divide_space(std::vector<Shape *> &shapes_list, int i0, int i1, double pivot, int axis)
{
    Shape   *temp;
    double  centroid;
    int     dev_index = 0;

    // Para todos los objetos de la lista, comprobamos si el centroide de su AABB es menor
    // o mayor en el eje indicado que el pivote. No es necesario ordenar la lista, simplemente
    // que todos los objetos queden a un lado o al otro.
    for( int i = i0; i <= i1; ++i )
    {
        centroid = shapes_list[i]->get_AABB().centroid().e[axis];

        if( centroid < pivot ) {
            // Si está a la izquierda
            temp            = shapes_list[i];
            shapes_list[i]  = shapes_list[i + dev_index];
            shapes_list[i + dev_index] = temp;
            ++dev_index;
        }
    }

    return dev_index;
}
