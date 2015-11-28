#ifndef __PARSER_H__
#define __PARSER_H__

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

#include "vzray.h"
#include "vec3.h"
#include "point.h"
#include "shapes/shape.h"
#include "shapes/meshtriangle.h"
#include "matrix/transform.h"

/**
 * \class Clase para procesar el fichero en el que se describe la escena.
 */
class Parser {
    private:
        // Puntero a los datos globales que rellenaremos con la informacion de archivo.
        Globals *globales;

		// Manejador del archivo.
        std::ifstream fichero_escena;

		// Mensajes de error del parser.
		std::string tipo_error[15];

		// Control de los mensajes de error.
        signed int num_error;

		// Controlamos la línea en la que estamos, para en el caso de tener
		// que informar de un error, poder decir en que linea ha ocurrido.
		int num_linea;

        // Nos indicará si hemos llegado al fin del fichero.
        bool eof;

		// Constantes auxiliares.
        static const int kmax_etiqueta;
        static const int kmax_contenido;

        // Import file basic functions
        bool read_token(std::string &token);
        bool read_content(std::string &content, const char stop_char);
        bool read_float(double &dato, const char stop_char);
        bool read_int(int &dato, const char stop_char);
        bool read_bloque_txt(const std::string &etiqueta, std::string &resultado);
        bool read_bloque_floats(const std::string &etiqueta, const int cantidad, double *numeros);
        bool read_bloque_ints(const std::string &etiqueta, const int cantidad, int *numeros);
		bool read_bloque_vec3(const std::string &etiqueta, Vec3 &v);
		bool read_bloque_point(const std::string &etiqueta, Point &p);

        bool ignorar_chars();

		bool valid_float(const std::string &num);
		bool valid_int(const std::string &num);

		bool is_comment();

		// Process imported data
		bool process_comment();

		bool process_config();
		bool process_renderer();
		bool process_camera();
		bool process_image();

		bool process_scene();
		bool process_settings();

		bool process_transform(Transform *t);

		bool process_texture();
		bool process_simple_tex();

		bool process_material();
		bool process_diffuse_mat();
		bool process_specular_mat();
		bool process_dielectric_mat();
		bool process_light_mat();

		bool process_object();
		bool process_plane();
		bool process_disc();
		bool process_sphere();
		bool process_cylinder();
		bool process_box();
		bool process_parallelogram();
		bool process_triangle();
		bool process_mesh();
		bool process_mesh_vertex(Shape *shape, std::vector<MeshTriangle *> *tlist);
		bool process_mesh_file(Shape *shape, std::vector<MeshTriangle *> *tlist);

    public:
        Parser();
        ~Parser();
        /**
         * \brief Lee el fichero indicado, y almacena el resultado en pGlobals.
         *
         * \param fichero Ruta al fichero.
         * \param globales Puntero a un objeto de clase Globals.
         * \return Verdadero si todo ha ido bien.
         */
        bool leer_fichero(const std::string &fichero, Globals *datos_globales);

        /**
         * \brief En caso de que LeerFichero falle, nos indicará porqué.
         *
         * \param tipo_error Cadena en la que se almacenará el error producido.
         * \param linea Numero de linea en el que está el error.
         * \param error Numero de código del error.
         * \return Verdadero si todo ha ido bien.
         */
        void get_error(std::string &tipo_error, int &linea, int &error);
};

#endif // __PARSER_H__
