#include <vector>

#include "bvh/bvh.h"

#include "log.h"

BVH::BVH(std::vector<Shape *> &shapes_list, int i0, int i1, int axis)
{
    LOG() << "BVH:Constructor general: Entrando. (i0, i1) : " << i0 << ", " << i1;

    if( i1 - i0 == 0 ) { // Un elemento en la lista.
        LOG() << "BVH:Constructor general: Un solo objeto.";

        izq = der = shapes_list[i0];

        aabb = shapes_list[i0]->get_AABB();
    }
    else if( i1 - i0 == 1 ) { // Dos elementos en la lista.
        LOG() << "BVH:Constructor general: Dos objetos.";

        izq = shapes_list[i0];
        der = shapes_list[i1];

        LOG() << "BVH:Constructor general: Dos objetos: aabb: i0: " << i0 << ", " << shapes_list[i0]->get_AABB();

        LOG() << "BVH:Constructor general: Dos objetos: aabb: i1: " << i1 << ", " << shapes_list[i1]->get_AABB();
        aabb = surround(shapes_list[i0]->get_AABB(), shapes_list[i1]->get_AABB());

        LOG() << "BVH:Constructor general: Dos objetos. Saliendo.";
    }
    else { // Tres o más, subdividimos.
        LOG() << "BVH:Constructor general: Tres o más objetos (" << i0 << ", " << i1 << ", " << axis << ") - Entrando.";

        for( int i = i0; i <= i1; ++i) {
            LOG() << "BVH:Constructor general:3oM: aabb original: " << aabb;
            LOG() << "BVH:Constructor general:3oM: aabb a surrondear: " << shapes_list[i]->get_AABB();
            aabb = surround(aabb, shapes_list[i]->get_AABB());
            LOG() << "BVH:Constructor general:3oM: aabb surrondeada: " << aabb;
        }

        LOG() << "BVH:Constructor general: AABB general:" << aabb;

        // Partimos por el eje indicado
        //double pivot = (aabb.maximo.e[axis % 3] - aabb.minimo.e[axis % 3]) / 2.0f;
        double pivot = (aabb.minimo.e[axis % 3] + aabb.maximo.e[axis % 3]) / 2.0f;
        LOG() << "BVH:Constructor general: Calculando pivote: (pivote, eje): " << pivot << ", " << axis;

        // Reordenamos la lista de objetos de acuerdo al pivote elegido.
        int div = divide_space(shapes_list, i0, i1, pivot, (axis % 3));

        LOG() << "Espacio dividido: (i0, i1, div): (" << i0 << ", " << i1 << ", " << div << ")";
        
        for(int x = i0; x <= i1; ++x)
            LOG() << "   AABB(" << x << "): " << shapes_list[x]->get_AABB();

        // Generamos nuevas ramas para nuestro árbol con la nueva lista.
        //izq = create_subtree(shapes_list, i0, div, 0);
        //der = create_subtree(shapes_list, div+1, i1, 0);
        izq = new BVH(shapes_list, i0, div - 1, (axis + 1) % 3);
        der = new BVH(shapes_list, div, i1, (axis + 1) % 3);

        LOG() << "BVH:Constructor general: Tres o más objetos - Saliendo. ";
    }
}

bool BVH::hit(const Ray &r, double min_dist, double max_dist, HitRecord &hit) const
{
    bool    h_izq = false,
            h_der = false;

    // Si el rayo pasa por la aabb, comprobamos las ramas.
    if( aabb.hit(r, min_dist, max_dist) ) {
        h_izq = izq->hit(r, min_dist, max_dist, hit);

        if( h_izq )
            max_dist = hit.dist;

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
    LOG() << "BVH::divide_space: Entrando.";

    Shape   *temp;
    double  centroid;
    int     dev_index = 0;

    // Para todos los objetos de la lista, comprobamos si el centroide de su AABB es menor
    // o mayor en el eje indicado que el pivote. No es necesario ordenar la lista, simplemente
    // que todos los objetos queden a un lado o al otro.
    for( int i = i0; i <= i1; ++i )
    {
        LOG() << "BVH::divide_space: Calculando centroide.";

        centroid = shapes_list[i]->get_AABB().centroid().e[axis];
        
        LOG() << "BVH::divide_space: aabb del objeto: " << shapes_list[i]->get_AABB();
        LOG() << "BVH::divide_space: Centroide en eje (" << axis << "): " << centroid << " - Pivote: " << pivot;

        if( centroid < pivot ) {
            LOG() << "BVH::divide_space: Centroide a la izquierda.";
            LOG() << "BVH::divide_space:    Cambiando (" << i << ") por (" << i + dev_index << ")";
            // Si está a la izquierda
            temp            = shapes_list[i];
            shapes_list[i]  = shapes_list[i0 + dev_index];
            shapes_list[i0 + dev_index] = temp;
            ++dev_index;

        }
        else {
            LOG() << "BVH::divide_space: Centroide a la derecha.";
        }
    }

    // Si todos los objetos quedan a un lado, partimos por la mitad.
    if( (dev_index == 0) || ((i0 + dev_index) == i1) ) dev_index = (i1 - i0) / 2;

    return (i0 + dev_index);
}
