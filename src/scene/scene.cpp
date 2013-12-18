#include <vector>

#include "scene.h"

#include "ray.h"
#include "shapes/shape.h"
#include "aabb/aabb.h"
#include "shapes/box.h"
#include "shapes/sphere.h"

#include "textures/texture.h"
#include "textures/simpletexture.h"

#include "material/material.h"
#include "material/diffusematerial.h"


bool Scene::addObject(Shape *pNewObject, std::string strMatID, bool bLight)
{
	// Si el material indicado no existe, salimos.
	if(Materials->getData(strMatID) == NULL) {
		return false;
	}
	else {
		pNewObject->setMaterial(Materials->getData(strMatID));

		if(bLight) // Si es una luz insertamos en ambas listas.
			return (Shapes->insertar("", pNewObject) && Lights->insertar("", pNewObject));
		else // Si no lo es, solo en la de objetos.
			return Shapes->insertar("", pNewObject);
	}
}

bool Scene::addTexture(Texture *pNewTexture, std::string strTextureID)
{
	// La lista se encargará de indicarnos si ya existe una textura con ese ID.
	return Textures->insertar(strTextureID, pNewTexture);
}

bool Scene::addMaterial(Material *pNewMaterial, std::string strTextureID, std::string strMatID)
{
	// Comprobamos que existe la textura indicada.
	if(Textures->getData(strTextureID) == NULL) {
		return false;
	}
	else {
		// Nos aseguramos de que el material tiene asociada la textura.
		pNewMaterial->setTexture(Textures->getData(strTextureID));

		// Insertamos. Igual que para las texturas, si ya existe un material
		// con el ID indicado, la lista devolverá false.
		return Materials->insertar(strMatID, pNewMaterial);
	}
}

bool Scene::nearestIntersection(Ray a_rRay, float a_dMin, float a_dMax, HitRecord &htHit)
{
	bool bHit = false;

	for(int i = 0; i < Shapes->getNumEltos(); i++) {
		if(Shapes->getData(i)->hit(a_rRay, a_dMin, a_dMax, htHit)) {
			a_dMax = htHit.dDist;
			bHit = true;
		}
	}

	return bHit;
}

bool Scene::shadowIntersection(Ray a_rRay, float fdist_min, float fdist_max)
{
    return false;
}

bool Scene::showAABB()
{
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
}
