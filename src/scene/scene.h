#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <string>
#include <vector>

#include "material/material.h"
#include "shapes/shape.h"
#include "textures/texture.h"
#include "ray.h"
#include "rgb.h"

#include "utils/dllist.h"

class Shape_node {
    public:
        Shape_node()
        {
            s = nullptr;
            id = "";
        }
        ~Shape_node() {
            if(s != nullptr)
                delete s;
        }

        Shape *s;
        std::string id;
};

class Material_node {
    public:
        Material_node()
        {
            m = nullptr;
            id = "";
        }
        ~Material_node() {
            if(m != nullptr)
                delete m;
        }

        Material *m;
        std::string id;
};

class Texture_node {
    public:
        Texture_node()
        {
            t = nullptr;
            id = "";
        }
        ~Texture_node() {
            if(t != nullptr)
                delete t;
        }

        Texture *t;
        std::string id;
};

class Scene {
	public:
		Scene()
		{
			shapes 		= new DLList<Shape>(DLL_DELETE_DATA);
			lights 		= new DLList<Shape>(~DLL_DELETE_DATA);
			// No borramos los datos enlazados en esta lista, ya que como
			// también están enlazados en la lista Shapes, al borrar la
			// lista Shapes eliminamos los datos enlazados por la lista
			// Lights. Así solamente borraremos la estructura creada,
			// pues los datos ya han sido o van a ser borrados.

			materials 	= new DLList<Material>(DLL_UNIQUE_ID | DLL_DELETE_DATA);
			textures 	= new DLList<Texture>(DLL_UNIQUE_ID | DLL_DELETE_DATA);
		}
		~Scene()
		{
			delete shapes;
			delete lights;
			delete materials;
			delete textures;
		}

		/***
		 * Métodos para construir la escena. Devuelven false en caso de error.
		 ***/

		// Añade un nuevo objeto a la escena. El objeto utilizará como
		// material el indicado por la string, que deberá existir en la
		// lista de materiales al añadir el objeto.
		// Si la variable bLight es true, quiere decir que es una luz
		// y hemos de añadirla también a nuestra lista de luces.
		bool add_object(Shape *new_object, std::string material_id, bool is_light = false);

		// Añade una nueva textura a la lista de texturas. Todas las
		// texturas que se utilicen deben estar en esta lista. Tendrá
		// la ID que se indique.
		bool add_texture(Texture *new_texture, std::string texture_id);

		// Devuelve un puntero a la textura con el ID indicado.
		Texture *get_texture(std::string texture_id) { return textures->get_data(texture_id); };

		// Añade un nuevo material a nuestra lista de materiales. La textura
		// que se indique debe estar anteriormente añadida a la lista de
		// texturas. Tendrá la ID indicada.
		bool add_material(Material *new_material, std::string texture_id, std::string material_id);

		// Devuelve un puntero al material con el ID indicado.
		Material *get_material(std::string material_id) { return materials->get_data(material_id); };

		/***
		 * Métodos para la consulta.
		 * True si hay intersección (se rellenarán las estructuras convenientes,
		 * en caso de que sea necesario) y false si no la hay.
		 ***/
		// Lanza el rayo a la escena y calcula la intersección más cercana, si la hubiera.
		bool nearest_intersection(Ray r, float min_dist, float max_dist, HitRecord &hit);

		// Lanza un rayo y calcula si hay alguna intersección entre dist_min y dist_max.
		bool shadow_intersection(Ray r, float min_dist, float max_dist);

		// Devuelve el numero de objetos de la escena (objetos + luces).
		int get_num_objs() { /*return shapes->get_num_eltos();*/ return shapes_list.size(); };

		// Devuelve el elemento i-ésimo de la lista de objetos.
		Shape *get_object(int i) { /*return shapes->get_data(i);*/ return shapes_list[i].s; };

		// Devuelve el numero de luces de la escena.
		int get_num_lights() { /*return lights->get_num_eltos();*/ return lights_list.size(); };

		// Devuelve el elemento i-ésimo de la lista de luces.
		Shape *get_light(int i) { /*return lights->get_data(i);*/ return lights_list[i].s; };

		// Cambia el color de fondo de la escena.
		void set_bg_color(const RGB &color) { bg_color = color; }

		// Obtiene el color de fondo.
		RGB get_bg_color() const { return bg_color; }

		// Incluye entre los elementos a mostrar las AABB.
		bool show_AABB();

	private:
		DLList<Shape> 		*shapes;
		DLList<Shape> 		*lights;
		DLList<Material> 	*materials;
		DLList<Texture>		*textures;

		std::vector<Shape_node>     shapes_list;
		std::vector<Shape_node>     lights_list;
		std::vector<Material_node>  materials_list;
		std::vector<Texture_node>   textures_list;

		RGB					bg_color;
};

#endif // __SCENE_HPP__
