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


bool Scene::add_object(Shape *new_object, std::string material_id, bool is_light)
{
	// Si el material indicado no existe, salimos.
	if(materials->get_data(material_id) == NULL) {
		return false;
	}
	else {
		new_object->set_material(materials->get_data(material_id));

		if(is_light) // Si es una luz insertamos en ambas listas.
			return (shapes->insertar("", new_object) && lights->insertar("", new_object));
		else // Si no lo es, solo en la de objetos.
			return shapes->insertar("", new_object);
	}
}

bool Scene::add_texture(Texture *new_texture, std::string texture_id)
{
	// La lista se encargará de indicarnos si ya existe una textura con ese ID.
	return textures->insertar(texture_id, new_texture);
}

bool Scene::add_material(Material *new_material, std::string texture_id, std::string material_id)
{
	// Comprobamos que existe la textura indicada.
	if(textures->get_data(texture_id) == NULL) {
		return false;
	}
	else {
		// Nos aseguramos de que el material tiene asociada la textura.
		new_material->set_texture(textures->get_data(texture_id));

		// Insertamos. Igual que para las texturas, si ya existe un material
		// con el ID indicado, la lista devolverá false.
		return materials->insertar(material_id, new_material);
	}
}

bool Scene::nearest_intersection(Ray r, float min_dist, float max_dist, HitRecord &hit)
{
	bool is_hit = false;

    ++Statistics::num_primary_rays;

	for(int i = 0; i < shapes->get_num_eltos(); i++) {
		if(shapes->get_data(i)->hit(r, min_dist, max_dist, hit)) {
			max_dist = hit.dist;
			is_hit = true;
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
