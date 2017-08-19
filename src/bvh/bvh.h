#ifndef BVH_H_INCLUDED
#define BVH_H_INCLUDED

#include "shapes/shape.h"
#include "aabb/aabb.h"

#include "log.h"

/**
 * \class Clase para la construccion del árbol jerárquico de volumenes.
 */
class BVH : public Shape {
    public:
        ~BVH()
        {
            if( izq != nullptr )
                delete izq;
            if( der != nullptr )
                delete der;

            izq = der = nullptr;
        };

        BVH();
        BVH(std::vector<Shape *> &shapes_list, int i0, int i1, int axis);
        BVH(Shape *s1, Shape *s2) {
            LOG() << "BVH::CONSTRUCTOR DOS OBJETOS.";

            izq = s1;
            der = s2;

            aabb = surround(s1->get_AABB(), s2->get_AABB());
        };

        bool hit(const Ray &r, double min_dist, double max_dist, HitRecord &hit) const;

        bool shadow_hit(const Ray &r, double min_dist, double max_dist) const;

        bool get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const
        {
            return false;
        };

    private:
        int divide_space(std::vector<Shape *> &shapes_list, int i0, int i1, double pivot, int axis);

        Shape *create_subtree(std::vector<Shape *> &shapes_list, int i0, int i1, int axis);

        Shape   *izq,
                *der;
};

#endif // BVH_H_INCLUDED
