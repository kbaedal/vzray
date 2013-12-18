#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <string>

#include "material/material.h"
#include "shapes/shape.h"
#include "textures/texture.h"
#include "ray.h"
#include "rgb.h"

#include "utils/dllist.h"

class Scene {
	public:
		Scene()
		{
			Shapes 		= new DLList<Shape>(DLL_DELETE_DATA);
			Lights 		= new DLList<Shape>(~DLL_DELETE_DATA); 
			// No borramos los datos enlazados en esta lista, ya que como
			// también están enlazados en la lista Shapes, al borrar la
			// lista Shapes eliminamos los datos enlazados por la lista
			// Lights. Así solamente borraremos la estructura creada,
			// pues los datos ya han sido o van a ser borrados.
			
			Materials 	= new DLList<Material>(DLL_UNIQUE_ID | DLL_DELETE_DATA);
			Textures 	= new DLList<Texture>(DLL_UNIQUE_ID | DLL_DELETE_DATA);
		}
		~Scene()
		{
			delete Shapes;
			delete Lights;
			delete Materials;
			delete Textures;
		}
		
		/***
		 * Métodos para construir la escena. Devuelven false en caso de error.
		 ***/
		
		// Añade un nuevo objeto a la escena. El objeto utilizará como 
		// material el indicado por la string, que deberá existir en la 
		// lista de materiales al añadir el objeto.
		// Si la variable bLight es true, quiere decir que es una luz
		// y hemos de añadirla también a nuestra lista de luces.
		bool addObject(Shape *pNewObject, std::string strMatID, bool bLight = false);
		
		// Añade una nueva textura a la lista de texturas. Todas las
		// texturas que se utilicen deben estar en esta lista. Tendrá
		// la ID que se indique.
		bool addTexture(Texture *pNewTexture, std::string strTextureID);
		
		// Devuelve un puntero a la textura con el ID indicado.
		Texture *getTexture(std::string strTextureID) { return Textures->getData(strTextureID); };
		
		// Añade un nuevo material a nuestra lista de materiales. La textura
		// que se indique debe estar anteriormente añadida a la lista de
		// texturas. Tendrá la ID indicada.
		bool addMaterial(Material *pNewMaterial, std::string strTextureID, std::string strMatID);
		
		// Devuelve un puntero al material con el ID indicado.
		Material *getMaterial(std::string strMaterialID) { return Materials->getData(strMaterialID); };
		
		/***
		 * Métodos para la consulta.
		 * True si hay intersección (se rellenarán las estructuras convenientes,
		 * en caso de que sea necesario) y false si no la hay.
		 ***/
		// Lanza el rayo a la escena y calcula la intersección más cercana, si la hubiera.
		bool nearestIntersection(Ray a_rRay, float a_dMin, float a_dMax, HitRecord &htHit);
		
		// Lanza un rayo y calcula si hay alguna intersección entre dist_min y dist_max.
		bool shadowIntersection(Ray a_rRay, float fdist_min, float fdist_max);
		
		// Devuelve el numero de objetos de la escena (objetos + luces).
		int getNumObjs() { return Shapes->getNumEltos(); };
		
		// Devuelve el elemento i-ésimo de la lista de objetos.
		Shape *getObject(int i) { return Shapes->getData(i); };
				
		// Devuelve el numero de luces de la escena.
		int getNumLights() { return Lights->getNumEltos(); };
		
		// Devuelve el elemento i-ésimo de la lista de luces.
		Shape *getLight(int i) { return Lights->getData(i); };
		
		// Cambia el color de fondo de la escena.
		void setBGColor(const RGB &a_rgbBGColor) { m_rgbBGColor = a_rgbBGColor; }
		
		// Obtiene el color de fondo.
		RGB getBGColor() const { return m_rgbBGColor; }
		
		// Incluye entre los elementos a mostrar las AABB.
		bool showAABB();
			
	private:
		DLList<Shape> 		*Shapes;
		DLList<Shape> 		*Lights;
		DLList<Material> 	*Materials;
		DLList<Texture>		*Textures;
		
		RGB					m_rgbBGColor;
};

#endif // __SCENE_HPP__
