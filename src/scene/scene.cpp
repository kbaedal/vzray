#include <vector>

#include "scene.h"

#include "statistics.h"

#include "ray.h"
#include "shapes/shape.h"
#include "aabb/aabb.h"
#include "shapes/box.h"
#include "shapes/sphere.h"

#include "textures/texture.h"
#include "textures/simpletexture.h"

#include "material/material.h"
#include "material/diffusematerial.h"

Scene::~Scene()
{
    // Borramos los nodos y los objetos enlazados por los nodos.

    // Objetos.
    for(size_t i = 0; i < shapes.size(); ++i) {
        if(shapes[i] != nullptr) {
            delete shapes[i]->s;
            delete shapes[i];
        }
    }

    // Luces.
    // Solo borramos los nodos, puesto que los objetos ya han sido
    // borrados en la lista de objetos.
    for(size_t i = 0; i < lights.size(); ++i) {
        if(lights[i] != nullptr) {
            delete lights[i];
        }
    }

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
}

bool Scene::add_object(Shape *new_object, std::string material_id, bool is_light)
{
    Shape_node *new_node = new Shape_node;

	for(size_t i = 0; i < materials.size(); ++i) {
        if(materials[i]->id == material_id) {
            new_object->set_material(materials[i]->m);

            new_node->s = new_object;

            if(is_light) {
                shapes.push_back(new_node);
                lights.push_back(new_node);
            }
            else {
                shapes.push_back(new_node);
            }

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


bool Scene::nearest_intersection(Ray r, float min_dist, float max_dist, HitRecord &hit_r)
{
	bool is_hit = false;

    ++Statistics::num_primary_rays;

    for(size_t i = 0; i < shapes.size(); ++i) {
        if(shapes[i]->s->hit(r, min_dist, max_dist, hit_r)) {
            max_dist    = hit_r.dist;
            is_hit      = true;
        }
    }

	return is_hit;
}

bool Scene::shadow_intersection(Ray r, float min_dist, float max_dist)
{
    return false;
}

bool Scene::show_AABB()
{
    /*
	std::vector<Point>	vertexArray;
	std::vector<Point>	sphereArray;
	Shape				*pShape;
	AABB				abBox;

	Texture 			*pTexture;
	Material			*pMaterial;

	// Almacenamos todas las abBox que tengamos en la escena, excepto
	// si están sin inicializar.
	for(int i = 0; i < this->getNumObjs(); i++) {
		pShape = this->getObject(i);

		if(!pShape->getMaterial()->isLight() && pShape->applyBounds()) {
			abBox = pShape->getAABB();
			vertexArray.push_back(abBox.m_p3Min);
			vertexArray.push_back(abBox.m_p3Max);
		}
	}

	// Añadimos la textura para nuestro material de las AABB
	pTexture = new SimpleTexture(RGB(1.f, 0.f, 0.f));
	this->addTexture(pTexture, "AABB_Texture");

	// Y ahora el material
	pMaterial = new DiffuseMaterial(this->getTexture("AABB_Texture"));
	this->addMaterial(pMaterial, "AABB_Texture", "AABB_Material");

	// Para cada par de vertices (una abBox), creamos ocho puntos
	// que serán las esquinas en coordenadas del mundo de la abBox,
	// en las que colocaremos esferas.
	for(unsigned int i = 0; i < (vertexArray.size()); i += 2) {
		sphereArray.push_back(vertexArray[i]);
		sphereArray.push_back(vertexArray[i+1]);

		sphereArray.push_back(Point(vertexArray[i+1].x(),	vertexArray[i].y(), 	vertexArray[i].z()));
		sphereArray.push_back(Point(vertexArray[i].x(), 	vertexArray[i+1].y(),	vertexArray[i].z()));
		sphereArray.push_back(Point(vertexArray[i+1].x(), vertexArray[i+1].y(), 	vertexArray[i].z()));
		sphereArray.push_back(Point(vertexArray[i].x(), 	vertexArray[i].y(), 	vertexArray[i+1].z()));
		sphereArray.push_back(Point(vertexArray[i+1].x(), vertexArray[i].y(), 	vertexArray[i+1].z()));
		sphereArray.push_back(Point(vertexArray[i].x(), 	vertexArray[i+1].y(), 	vertexArray[i+1].z()));

		std::clog << "Sphering..." << std::endl;
		std::clog << vertexArray[i] << std::endl;
		std::clog << vertexArray[i+1] << std::endl;
		std::clog << Point(vertexArray[i+1].x(),	vertexArray[i].y(), 	vertexArray[i].z()) << std::endl;
		std::clog << Point(vertexArray[i].x(), 	vertexArray[i+1].y(),	vertexArray[i].z()) << std::endl;
		std::clog << Point(vertexArray[i+1].x(), vertexArray[i+1].y(), 	vertexArray[i].z()) << std::endl;
		std::clog << Point(vertexArray[i].x(), 	vertexArray[i].y(), 	vertexArray[i+1].z()) << std::endl;
		std::clog << Point(vertexArray[i+1].x(), vertexArray[i].y(), 	vertexArray[i+1].z()) << std::endl;
		std::clog << Point(vertexArray[i].x(), 	vertexArray[i+1].y(), 	vertexArray[i+1].z()) << std::endl;

	}

	// Y las añadimos como objetos a la lista, con un radio de 0.1

	for(unsigned int i = 0; i < (sphereArray.size()); i++) {
		pShape = new Sphere(sphereArray[i], 0.1f, NULL);
		pShape->setShadow(false);
		this->addObject(pShape, "AABB_Material", false);
	}

	// Y las añadimos como objetos con un material transparente
	//~ for(int i = 0; i < (vertexArray.size()); i += 2) {
		//~ pShape = new Box(vertexArray[i], vertexArray[i+1], NULL);
		//~ this->addObject(pShape, "AABB_Material", false);
	//~ }

	return true;
	*/

	return false;
}
