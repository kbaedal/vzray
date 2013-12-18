#ifndef __VZRAY_H__
#define __VZRAY_H__

#include "camera/camera.h"
#include "scene/scene.h"
#include "image.h"
#include "renderer/renderer.h"

// Máscaras para los flags.
const unsigned int GLB_SHOW_AABB	= 0x01; // 0000 0001
const unsigned int GLB_DO_TEST		= 0x02; // 0000 0010

/**
 * Clase para las variables globlales.
 * 
 * Serán accesibles para las partes más importantes del
 * programa, como el parser o la camara, por ejemplo.
 */
class Globals {
	public:
		Globals()
		{
			nResX 			 	= 512;
			nResY 			 	= 384;
			nSamplesPerPixel 	= 8;
			nShadowRays 	 	= 1;
			nMaxDepth		 	= 5;
			
			nOptsFlags			= 0x00;
			
			// A inicializar por el parser.
			pCamera 	= NULL;
			pScene 		= NULL;
			pImage 		= NULL;
			pRenderer 	= NULL;
		}
		~Globals()
		{
			// Check before delete.
			if(pCamera != NULL) delete pCamera;
			if(pScene != NULL) delete pScene;
			if(pImage != NULL) delete pImage;
			if(pRenderer != NULL) delete pRenderer;
		}

		int	nResX,					///< Resolución de la imagen en x.
			nResY,					///< Resolución de la imagen en y.
			nSamplesPerPixel,		///< Muestras a tomar por cada pixel.
			nShadowRays,			///< Rays de sombra por cada muestra.
			nMaxDepth;				///< Máximos rebotes de un rayo.
		
		unsigned int nOptsFlags;	///< Diferentes opciones.
			
		std::string strSceneFile; 	///< Contiene el fichero a renderizar.
		std::string strOutputFile;	///< Fichero donde guardar la imagen.
		
		Camera 		*pCamera;		///< Información de la camara.
		Scene		*pScene;		///< Escena leída del fichero.
		Image		*pImage;		///< Imagen generada por el renderizador.
		Renderer 	*pRenderer;		///< El renderizador, datos sacados del fichero.
};

#endif // __VZRAY_H
