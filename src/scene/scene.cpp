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
    /*
	// Si el material indicado no existe, salimos.
	if(materials->get_data(material_id) == nullptr) {
		return false;
	}
	else {
		new_object->set_material(materials->get_data(material_id));

		if(is_light) // Si es una luz insertamos en ambas listas.
			return (shapes->insertar("", new_object) && lights->insertar("", new_object));
		else // Si no lo es, solo en la de objetos.
			return shapes->insertar("", new_object);
	}
	*/

    /*
    Vamos a intentarlo de otra manera, no vaya a ser
    que el lio este en la doble lista o algo asi.
    */
    Shape_node new_node;

	for(size_t i = 0; i < materials_list.size(); ++i) {
        if(materials_list[i].id == material_id) {
            new_object->set_material(materials_list[i].m);

            new_node.s = new_object;

            if(is_light) {
                shapes_list.push_back(new_node);
                lights_list.push_back(new_node);
            }
            else
                shapes_list.push_back(new_node);

            // Evitamos el borrado del objeto en el destructor.
            new_node.s = nullptr;
            return true;
        }
    }

    return false;
}

bool Scene::add_texture(Texture *new_texture, std::string texture_id)
{
	// La lista se encargará de indicarnos si ya existe una textura con ese ID.
	/*return textures->insertar(texture_id, new_texture);*/

    std::cerr << "\nInsertando textura:\n\tid:\t" << texture_id << "\n" << std::flush;
	/*
	Pues lo dicho antes.
	*/
	for(size_t i = 0; i < textures_list.size(); ++i) {
        std::cerr << "\tTextura[" << i << "]: " << textures_list[i].id << "\n" << std::flush;
        if(textures_list[i].id == texture_id) {
            std::cerr << "\tEsta textura ya está en nuestra escena.\n" << std::flush;

            return false;
        }
	}

    std::cerr << "\tCreando nuevo nodo.\n" << std::flush;

	Texture_node new_node;
	new_node.t = new_texture;
	new_node.id = texture_id;

	std::cerr << "\tPush_back().\n" << std::flush;

	textures_list.push_back(new_node);

	std::cerr << "\tTextura insertada.\n" << std::flush;

    std::cerr << "\tWTF.\n" << std::flush;

    // Evitamos el borrado de la textura en el destructor.
    new_node.t = nullptr;
    return true;
}

bool Scene::add_material(Material *new_material, std::string texture_id, std::string material_id)
{
    /*
	// Comprobamos que existe la textura indicada.
	if(textures->get_data(texture_id) == nullptr) {
		return false;
	}
	else {
		// Nos aseguramos de que el material tiene asociada la textura.
		new_material->set_texture(textures->get_data(texture_id));

		// Insertamos. Igual que para las texturas, si ya existe un material
		// con el ID indicado, la lista devolverá false.
		return materials->insertar(material_id, new_material);
	}
	*/

	/*
	Mas experimentos.
	*/
	std::cerr << "\nInsertando material:\n\tid:\t" << material_id << "\n";

	Texture *t = nullptr;

	// ¿Existe textura?
	for(size_t i = 0; i < textures_list.size(); ++i) {
        if(textures_list[i].id == texture_id) {
            std::cerr << "\tLa textura asociada existe.\n" << std::flush;

            t = textures_list[i].t;
        }
	}

	if(t == nullptr) {
        std::cerr << "\tLa textura asociada NO existe.\n" << std::flush;
        return false;
	}

	// ¿Existe material?
	for(size_t i = 0; i < materials_list.size(); ++i) {
        if(materials_list[i].id == material_id) {
            std::cerr << "\tEl material a insertar ya existe.\n" << std::flush;
            return false;
        }
	}

	new_material->set_texture(t);

	Material_node new_node;
	new_node.m = new_material;
	new_node.id = material_id;

	materials_list.push_back(new_node);

	std::cerr << "\tMaterial insertado.\n" << std::flush;

    // Evitamos el borrado del material en el destructor.
    new_node.m = nullptr;
	return true;
}

bool Scene::nearest_intersection(Ray r, float min_dist, float max_dist, HitRecord &hit_r)
{
	bool is_hit = false;

    ++Statistics::num_primary_rays;

    /*
	for(int i = 0; i < shapes->get_num_eltos(); i++) {
		if(shapes->get_data(i)->hit(r, min_dist, max_dist, hit_r)) {
			max_dist    = hit_r.dist;
			is_hit      = true;
		}
	}
	*/

	/*
	Nuevo sistemazo.
	*/
    for(size_t i = 0; i < shapes_list.size(); ++i) {
        if(shapes_list[i].s->hit(r, min_dist, max_dist, hit_r)) {
            max_dist = hit_r.dist;
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
