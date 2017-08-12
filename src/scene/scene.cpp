#include <vector>

#include "scene.h"

#include "statistics.h"

#include "log.h"

#include "ray.h"
#include "shapes/shape.h"
#include "aabb/aabb.h"
#include "shapes/box.h"
#include "shapes/sphere.h"

#include "textures/texture.h"
#include "textures/simpletexture.h"

#include "material/material.h"
#include "material/diffusematerial.h"
#include "material/dielectricmaterial.h"

Scene::~Scene()
{
    // Borramos los nodos y los objetos enlazados por los nodos.

    // Objetos.
    for(size_t i = 0; i < shapes.size(); ++i) {
        if(shapes[i] != nullptr) {
            //delete shapes[i]->s;
            delete shapes[i];
            shapes[i] = nullptr;
        }
    }

    // Luces.
    // Solo borramos los nodos, puesto que los objetos ya han sido
    // borrados en la lista de objetos.
    /*
    for(size_t i = 0; i < lights.size(); ++i) {
        if(lights[i] != nullptr) {
            delete lights[i];
        }
    }
    */

    // Texturas.
    for(size_t i = 0; i < textures.size(); ++i) {
        if(textures[i] != nullptr) {
            delete textures[i]->t;
            delete textures[i];
        }
    }

    // Materiales.
    for(size_t i = 0; i < materials.size(); ++i) {
        if(materials[i] != nullptr) {
            delete materials[i]->m;
            delete materials[i];
        }
    }

    if(bvh != nullptr)
        delete bvh;
}

bool Scene::add_object(Shape *new_object, std::string material_id, bool is_light)
{
    //Shape_node *new_node = new Shape_node;

	for(size_t i = 0; i < materials.size(); ++i) {
        if(materials[i]->id == material_id) {
            new_object->set_material(materials[i]->m);

            //new_node->s = new_object;

            //shapes.push_back(new_node);
            shapes.push_back(new_object);

            if(is_light)
                //lights.push_back(new_node);
                lights.push_back(new_object);

            return true;
        }
    }

    return false;
}

bool Scene::add_texture(Texture *new_texture, std::string texture_id)
{
	for(size_t i = 0; i < textures.size(); ++i)
        if(textures[i]->id == texture_id)
            return false;

    Texture_node *new_node = new Texture_node;

	new_node->t     = new_texture;
	new_node->id    = texture_id;

	textures.push_back(new_node);

    return true;
}

Texture *Scene::get_texture(std::string texture_id)
{
    Texture *t = nullptr;

    for(size_t i = 0; i < textures.size(); ++i)
        if(textures[i]->id == texture_id)
            t = textures[i]->t;

    return t;
}

bool Scene::add_material(Material *new_material, std::string texture_id, std::string material_id)
{
    Texture *t = nullptr;

	// ¿Existe textura?
	for(size_t i = 0; i < textures.size(); ++i)
        if(textures[i]->id == texture_id)
            t = textures[i]->t;

    // No.
	if(t == nullptr)
        return false;

	// ¿Existe material?
	for(size_t i = 0; i < materials.size(); ++i)
        if(materials[i]->id == material_id)
            return false;

	new_material->set_texture(t);

	Material_node *new_node = new Material_node;
	new_node->m = new_material;
	new_node->id = material_id;

	materials.push_back(new_node);

    return true;
}

Material *Scene::get_material(std::string material_id)
{
    Material *m = nullptr;

    for(size_t i = 0; i < materials.size(); ++i)
        if(materials[i]->id == material_id)
            m = materials[i]->m;

    return m;
}


bool Scene::nearest_intersection(Ray r, double min_dist, double max_dist, HitRecord &hit_r)
{
	//bool is_hit = false;

    ++Statistics::num_primary_rays;

    /*
    for(size_t i = 0; i < shapes.size(); ++i) {
        if(shapes[i]->hit(r, min_dist, max_dist, hit_r)) {
            max_dist    = hit_r.dist;
            is_hit      = true;
        }
    }

	return is_hit;
	*/
	return this->bvh->hit(r, min_dist, max_dist, hit_r);
}

bool Scene::shadow_intersection(Ray r, double min_dist, double max_dist)
{
    return false;
}

bool Scene::show_AABB()
{
	Shape   *pBox;
	AABB    abBox;

	Texture *pTexture = new SimpleTexture(RGB(0.0f, 1.0f, 0.0f));
	this->add_texture(pTexture, "AABB_Texture");

	Material *pMaterial = new DiffuseMaterial(this->get_texture("AABB_Texture"));
	this->add_material(pMaterial, "AABB_Texture", "AABB_Material");

	// Almacenamos todas las abBox que tengamos en la escena, excepto
	// si están sin inicializar.

	size_t j = shapes.size();   // Tamaño de nuestra lista de objetos. Vamos a añadir más,
                                // así que comprobaremos solo hasta este punto. Lo que este
                                // más allá serán los nuevos objetos añadidos.
	for( size_t i = 0; i < j; ++i ) {
		if( !shapes[i]->get_material()->is_light() && shapes[i]->apply_bounds()) {
            abBox = shapes[i]->get_AABB();

			pBox = new Box(abBox.minimo, abBox.maximo, nullptr);

			this->add_object(pBox, "AABB_Material", false);
		}
	}

	return false;
}
