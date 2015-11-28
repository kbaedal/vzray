#ifndef __VZRAY_H__
#define __VZRAY_H__

#include "camera/camera.h"
#include "scene/scene.h"
#include "image.h"
#include "renderer/renderer.h"

// Máscaras para los flags.
namespace Global_opts {
    extern const unsigned int kglb_show_aabb;   // Show bounding boxes?
    extern const unsigned int kglb_do_test;     // Make a test?
    extern const unsigned int kglb_do_contrib;  // Create separate contirbutions images?
};

/**
 * Estructura para las variables globlales.
 *
 * Serán accesibles para las partes más importantes del
 * programa, como el parser o la camara, por ejemplo.
 */
class Globals {
    public:
        Globals()
        {
            res_x               = 512;
            res_y 			 	= 384;
            samples_per_pixel 	= 8;
            shadow_rays 	 	= 1;
            max_depth		 	= 5;

            options 			= 0x00;

            // A inicializar por el parser.
            camera      = nullptr;
            scene 	    = nullptr;
            image       = nullptr;
            renderer    = nullptr;
        }
        ~Globals()
        {
            // Check before delete.
            if(camera   != nullptr) delete camera;
            if(scene    != nullptr) delete scene;
            if(image    != nullptr) delete image;
            if(renderer != nullptr) delete renderer;
        }

        int	res_x,					///< Resolución de la imagen en x.
            res_y,					///< Resolución de la imagen en y.
            samples_per_pixel,		///< Muestras a tomar por cada pixel.
            shadow_rays,			///< Rays de sombra por cada muestra.
            max_depth;				///< Máximos rebotes de un rayo.

        unsigned int options;	    ///< Diferentes opciones.

        Camera 		*camera;		///< Información de la camara.
        Scene		*scene;		    ///< Escena leída del fichero.
        Image		*image;         ///< Contribucion directa de la luz a la imagen.
        Renderer 	*renderer;		///< El renderizador, datos sacados del fichero.
};

#endif // __VZRAY_H
