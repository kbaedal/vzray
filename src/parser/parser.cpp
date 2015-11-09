#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <cstdio>

#include "parser.h"

#include "scene/scene.h"

#include "renderer/renderer.h"
#include "renderer/directrenderer.h"
#include "renderer/pathrenderer.h"

#include "camera/camera.h"
#include "camera/pinhole.h"
#include "camera/thinlens.h"

#include "image.h"

#include "textures/texture.h"
#include "textures/simpletexture.h"

#include "material/material.h"
#include "material/dielectricmaterial.h"
#include "material/diffusematerial.h"
#include "material/specularmaterial.h"
#include "material/lightmaterial.h"

#include "matrix/transform.h"

#include "shapes/shape.h"
#include "shapes/parallelogram.h"
#include "shapes/plane.h"
#include "shapes/disc.h"
#include "shapes/sphere.h"
#include "shapes/cylinder.h"
#include "shapes/box.h"
#include "shapes/triangle.h"
#include "shapes/mesh.h"
#include "shapes/meshtriangle.h"

const int Parser::kmax_etiqueta     = 15;
const int Parser::kmax_contenido    = 25;

Parser::Parser(void)
{
	num_error   = -1; 		// No ha ocurrido ningún error aún.
	num_linea   = 1;		// De ocurrir, ocurre como muy pronto en la primera linea
	eof		    = false;	// Mientras sea nulo, no hemos llegado al fin del fichero.

    tipo_error[0].assign("Etiqueta desconocida.");
    tipo_error[1].assign("Fin de archivo inesperado.");
    tipo_error[2].assign("Contenido de etiqueta desconocido.");
    tipo_error[3].assign("Numero (double o int) no valido.");
    tipo_error[4].assign("Fichero no encontrado o algo raro (Badbit).");
    tipo_error[5].assign("Fichero no encontrado o algo raro (Failbit).");
    tipo_error[6].assign("Fichero no encontrado o algo raro (Eofbit).");
    tipo_error[7].assign("Fichero ya abierto, so mongolo.");
    tipo_error[8].assign("Caracter desconocido.");
    tipo_error[10].assign("Fichero correcto.");
}

Parser::~Parser()
{
    if(fichero_escena.is_open())
        fichero_escena.close();
}

void Parser::get_error(std::string &a_tipo_error, int &a_num_linea, int &a_num_error)
{
    if(num_error != -1)
		a_tipo_error.assign(tipo_error[num_error]);
    else
        a_tipo_error.assign("Sin error.");

    a_num_linea = num_linea;
    a_num_error = num_error;
}

bool Parser::leer_fichero(const std::string &fichero, Globals *datos_globales)
{
	globales = datos_globales; // Inicializamos el puntero para rellenar los datos.

    fichero_escena.open(fichero.c_str()); // Abrimos el fichero.

    if(!fichero_escena.is_open()) {
        num_error = 7;
        return false;
    }

    if(!fichero_escena.good()) {
        if(fichero_escena.bad())
            num_error = 4;
        else if(fichero_escena.fail())
            num_error = 5;
        else num_error = 6;

        return false;
    }

    do {
		if(!ignorar_chars()) {
			if(!eof) // No hemos llegado al fin de archivo. Error.
				return false;
		}
		else {
			std::string etiqueta;

			if(read_token(etiqueta))
			{
				// Procesar las etiquetas encontradas.
				if(etiqueta == "config") {
					if(!process_config())
						return false;
				}
				else if(etiqueta == "scene") {
					if(!process_scene())
						return false;
				}
				else {
					num_error = 0; // Etiqueta desconocida.
					return false;
				}
			}
			else {
				num_error = 0; // Etiqueta desconocida.
				return false;
			}
		}
	}
	while(!eof);
    num_error = 10;	// Si llegamos hasta aquí, es que el fichero se ha leido
    return true;	// bien y al completo.
}

bool Parser::ignorar_chars()
{
    bool encontrado = false;
    char c;

    do {
        if(fichero_escena.get(c)) { // No estamos al final del archivo si pasamos de aqui.
			switch(c) {
				// Ignoramos saltos de linea, espacios y tabuladores.
                case '\n': num_linea++;
                case '\r':
                case ' ':
                case '\t':
                    break;
                case '<':
					// Encontrado inicio de etiqueta. Devolvermos el control
					// si no es un comentario.
					if(is_comment()) {
						if(!process_comment())
							return false;
					}
					else {
						encontrado = true;
					}
                    break;
                default: // Caracter erroneo o no esperado.
					num_error = 8;
                    return false;
                    break;
            }
        }
        else { // Fin de archivo.
            num_error = 1;
            eof = true;
            return false;
        }
    }
    while(!encontrado);
    return true;
}

bool Parser::read_token(std::string &token)
{
    int 		cont = 0;
    char 		c;
    std::string temp(""); // Cadena inicializada sin caracteres.

    while(cont < kmax_etiqueta) {
        if(!fichero_escena.get(c)) {
            num_error = 1; // Fin del fichero, devolvemos false.
            return false;
        }
        else {
            if(c != '>')
				// Añadimos el caracter leido a la cadena.
                temp += c;
            else {
                // Fin de la etiqueta, copiamos lo leido a nuestra
                // cadena y devolvemos true.
                token = temp;
                return true;
            }
        }
        cont++;
    }
    // Superado el tamaño máximo de etiqueta, devolvemos false.
    num_error = 0; // Etiqueta desconocida.
    return false;
}

bool Parser::read_content(std::string &content, const char stop_char)
{
    int 		cont = 0;
    char 		c;
    std::string	temp("");

    while(cont < kmax_contenido) {
        if(!fichero_escena.get(c)) {
            num_error = 1; // Fin del fichero, devolvemos false.
            return false;
        }
        else {
            if(c != stop_char)
                temp += c;
            else {
                // Fin del contenido, copiamos los datos y volvemos.
                content= temp;
                return true;
            }
        }
        cont++;
    }
    num_error = 2; // Superado el tamaño máximo de contenido, devolvemos false.
    return false;
}

bool Parser::read_float(double &dato, const char stop_char)
{
	std::string temp("");

	// Leemos el contenido en una cadena.
	read_content(temp, stop_char);

	// Comprobamos que la cadena contenga un numero válido para pasarselo
	// a atof() con total seguridad.
	if(valid_float(temp)) {
		dato = atof(temp.c_str());
		return true;
	}
	else {
		num_error = 3;
		return false;
	}
}

bool Parser::read_int(int &dato, const char stop_char)
{
	std::string temp("");

	// Leemos el contenido en una cadena.
	read_content(temp, stop_char);

	// Comprobamos que la cadena contenga un numero válido para pasarselo
	// a atoi() con total seguridad.
	if(valid_int(temp)) {
		dato = atoi(temp.c_str());
		return true;
	}
	else {
		num_error = 3;
		return false;
	}
}

bool Parser::read_bloque_txt(const std::string &etiqueta, std::string &resultado)
{
    std::string buffer_etiqueta,
				buffer_contenido,
				cierre("/");

	// Ignoramos hasta llegar a la apertura de una etiqueta.
    if(!ignorar_chars())
        return false;

    // Leemos que etiqueta es.
    if(!read_token(buffer_etiqueta))
        return false;

    // Si no es la etiqueta que buscamos, devolvemos error.
    if(etiqueta != buffer_etiqueta)
        return false;

    // Leemos el contenido.
    if(!read_content(buffer_contenido, '<'))
        return false;

    // Antes de leer el cierre de la etiqueta, componemos la cadena para
    // comparar con dicho cierre. El cierre de la etiqueta debe ser igual
    // que la apertura pero con una barra delante:
    //	- Si apertura es <etiqueta>, el cierre debe ser </etiqueta>
    cierre += buffer_etiqueta;
    buffer_etiqueta.clear();

    // Leemos el cierre de la etiqueta.
    if(!read_token(buffer_etiqueta))
        return false;

    // Y comprobamos que sea el cierre correcto.
    if(cierre != buffer_etiqueta)
        return false;

	// Copiamos el resultado para devolverlo.
    resultado = buffer_contenido;
    return true;
}

// Leeremos, dentro de la etiqueta etiqueta, nNumF floats, que
// almacenaremos en el array de floats fltArray. Al llamar a este metodo
// debemos garantizar que fltArray puede almacenar nNumF floats.
bool Parser::read_bloque_floats(const std::string &etiqueta, const int cantidad, double *numeros)
{
	std::string	buffer_etiqueta,
				cierre("/");
	char		separador;

	if(!ignorar_chars())
        return false;
    if(!read_token(buffer_etiqueta))
        return false;
	if(etiqueta != buffer_etiqueta)
        return false;

	for(int i = 0; i < cantidad; i++) {
		if(i == cantidad - 1) // Ultimo double a leer
			separador = '<';
		else
			separador = ',';

		if(!read_float(numeros[i], separador))
			return false;
	}

	cierre += buffer_etiqueta;
    buffer_etiqueta.clear();

    if(!read_token(buffer_etiqueta))
        return false;

    if(cierre != buffer_etiqueta)
        return false;

    return true;
}

bool Parser::read_bloque_ints(const std::string &etiqueta, const int cantidad, int *numeros)
{
	std::string	buffer_etiqueta,
				cierre("/");
	char		separador;

	if(!ignorar_chars())
        return false;
    if(!read_token(buffer_etiqueta))
        return false;
	if(etiqueta != buffer_etiqueta)
        return false;

	for(int i = 0; i < cantidad; i++) {
		if(i == cantidad - 1) // Ultimo double a leer
			separador = '<';
		else
			separador = ',';

		if(!read_int(numeros[i], separador))
			return false;

	}

	cierre += buffer_etiqueta;
    buffer_etiqueta.clear();

    if(!read_token(buffer_etiqueta))
        return false;

    if(cierre != buffer_etiqueta)
        return false;

    return true;
}

bool Parser::read_bloque_vec3(const std::string &etiqueta, Vec3 &v)
{
	double componentes[3];

	if(!read_bloque_floats(etiqueta, 3, componentes))
		return false;

	v.set(componentes);

	return true;
}

bool Parser::read_bloque_point(const std::string &etiqueta, Point &p)
{
	double componentes[3];

	if(!read_bloque_floats(etiqueta, 3, componentes))
		return false;

	p.set(componentes);

	return true;
}

bool Parser::valid_float(const std::string &num)
{
	// Para que una cadena contenga un double válido deben darse las
	// siguientes condiciones:
	//	- Puede haber espacios en blanco antes del primer caracter.
	//	- También puede haber espacios en blanco tras el último.
	//	- El primer caracter debe ser el signo, si lo hay, (+ o -).
	//	- Tras el signo solo puede haber números, nunca otro signo.
	//	- Puede haber un punto para indicar los decimales.
	//	- Tras el punto no puede haber otro punto.
	//	- Puede haber una letra 'e' o 'E', indicando el exponente.
	//	- Tras la 'e' o la 'E' puede haber un signo.
	//	- Tras la 'e' o la 'E' no puede haber un punto, sólo cifras.

	unsigned int    cont        = 0;
    bool	        en_numero 	= false,
                    fin_numero 	= false,
                    punto 		= false,
                    signo_man	= false,
                    signo_exp	= false,
                    en_exp		= false;

	while(cont < num.length()) {
		switch(num[cont]) {
			case ' ':
				if(en_numero)
					// Permitimos los espacios al principio y al final
					// de la cadena.
					fin_numero = true;
				break;
			case '.':
				if(!punto && !fin_numero) {
					// Hemos encontrado el punto decimal.
					punto = true;
					// Y puede que no haya cifra por delante, pero aún
					// así es un numero válido: .56, por ejemplo.
					en_numero = true;
				}
				else
					// Hemos encontrado otro punto. Error.
					return false;
				break;
			case '+':
			case '-':
				if(!en_numero && !signo_man && !fin_numero) {
					// Encontramos el signo de la mantisa, antes
					// de cualquier cifra.
					signo_man = true;
					// Y ya tenemos que procesar el número.
					en_numero = true;
				}
				else if(en_numero && en_exp && !signo_exp) {
					// Encontramos el signo del exponente, que debe venir
					// detrás de la 'e' que lo indica.
					signo_exp = true;
				}
				else
					// Hemos encontrado un signo en un lugar en el que
					// no debería estar.
					return false;
				break;
			case 'e':
			case 'E':
				if(en_numero && !en_exp && !fin_numero)
					// Hemos encontrado la 'e' que indica el inicio
					// del exponente.
					en_exp = true;
				else
					// Hemos encontrado la 'e' en un lugar en el que
					// no debería estar, o ya había una e en el numero.
					return false;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if(!en_numero && !fin_numero)
					// Una cifra. Encontrada por primera vez nos marcará
					// el inicio del número, a partir de aquí entran en
					// juego las demás reglas.
					en_numero = true;
				else if(fin_numero)
					// Hemos encontrado una cifra después de haber leído
					// un espacio que indicaba el final del número.
					return false;
				break;
			default:
				// En este caso, encontarmos un caracter no válido.
				return false;
				break;
		}
		// Avanzamos en la cadena.
		cont++;
	}
	// Si hemos llegado hasta aquí, quiere decir que la cadena leída
	// tiene el formato adecuado para ser convertida a float.
	return true;
}

bool Parser::valid_int(const std::string &num)
{
	// Para que una cadena contenga un int válido deben darse las
	// siguientes condiciones:
	//	- Puede haber espacios en blanco antes del primer caracter.
	//	- También puede haber espacios en blanco tras el último.
	//	- El primer caracter debe ser el signo, si lo hay, (+ o -).
	//	- Tras el signo solo puede haber números, nunca otro signo.

	unsigned int 	cont = 0;
    bool	        en_numero 	= false,
                    fin_numero 	= false;

	while(cont < num.length())
    {
		switch(num[cont]) {
			case ' ':
				if(en_numero)
					// Permitimos los espacios al principio y al final
					// de la cadena.
					fin_numero = true;
				break;
			case '+':
			case '-':
				if(!en_numero)
					// Encontramos el signo antes de cualquier cifra.
					// Ya tenemos que procesar el número.
					en_numero = true;
				else
					// Hemos encontrado un signo en un lugar en el que
					// no debería estar.
					return false;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if(!en_numero && !fin_numero)
					// Una cifra. Encontrada por primera vez nos marcará
					// el inicio del número, a partir de aquí entran en
					// juego las demás reglas.
					en_numero = true;
				else if(fin_numero)
					// Hemos encontrado una cifra después de haber leído
					// un espacio que indicaba el final del número.
					return false;
				break;
			default:
				// En este caso, encontarmos un caracter no válido.
				return false;
				break;
		}
		// Avanzamos en la cadena.
		cont++;
	}
	// Si hemos llegado hasta aquí, quiere decir que la cadena leída
	// tiene el formato adecuado para ser convertida a int.
	return true;
}

bool Parser::is_comment()
{
	char c;

	if(fichero_escena.get(c)) {
		// Retrocedemos la posicion del fichero, para que las proximas
		// funciones puedan procesar correctamente la etiqueta encontrada,
		// sea esta un comentario o no.
		fichero_escena.unget();

		if(c == '!') // Tenemos un comentario.
			return true;
		else // No es un comentario.
			return false;
	}
	else {
		num_error = 1; // Fin de fichero inesperado.
		return false;
	}
}

bool Parser::process_comment()
{
	bool 		comment = false;
	char 		c;
	std::string	temp("<"); // Este caracter ya está leído.

	// Recogemos los tres caracteres siguientes del archivo para comprobar
	// que tenemos la apertura del comentario, que está compuesta por
	// la cadena "<!--".
	for(int i = 0; i < 3; i++)
		if(fichero_escena.get(c))
			temp += c;
		else {
			num_error = 1; // Fin de fichero inesperado.
			return false;
		}

	if(temp != "<!--") // Si no coincide, devolvemos false.
		return false;

	// Ahora buscaremos el primer signo '>', para comprobar que es parte
	// de la etiqueta de cierre: "-->"
	do {
		if(fichero_escena.get(c)) {
			switch(c) {
				case '>': // Encontrado un '>'
					// Encontrado un '>', retrocedemos tres posiciones nuestra
					// posicion en el archivo y leemos tres caracteres.
					for(int i = 0; i < 3; i++, fichero_escena.unget());

					temp.clear();

					for(int i = 0; i < 3; i++) {
						if(fichero_escena.get(c))
							temp += c;
						else {
							num_error = 1; // Fin de fichero inesperado.
							return false;
						}
					}

					if(temp == "-->") // Fin de comentario.
						comment = true;
					break;
				case '\n': num_linea++;
					break;
				default:
					break;
			}
		}
		else {
			num_error = 1; // Fin de fichero inesperado.
			return false;
		}
	}
	while(!comment);

	// Si llegamos aquí es que el comentario está correctamente construido.
	return true;
}

bool Parser::process_config()
{
	std::string etiqueta;
    bool 		fin_bloque = false;

    while(!fin_bloque) {
		// Buscamos etiqueta.
		if(!ignorar_chars())
			return false;

        if(!read_token(etiqueta))
            return false;

        if(etiqueta == "/config")
			fin_bloque = true;
        else
        {
            if(etiqueta == "renderer") {
				if(!process_renderer())
					return false;
            }
            else if(etiqueta == "camera") {
				if(!process_camera())
					return false;
            }
            else if(etiqueta == "image") {
				if(!process_image())
					return false;
            }
            else {
				// Etiqueta desconocida, no perteneciente a la seccion config.
                return false;
            }
        }
    }

    return true;
}

bool Parser::process_renderer()
{
	std::string tipo, // Tipo de renderer
				temp; // Auxiliar para comprobaciones.
	int			num_samples, max_depth;

	if(!read_bloque_txt("type", tipo))
		return false;

	if(!read_bloque_ints("samples", 1, &num_samples))
		return false;

	if(!read_bloque_ints("max_depth", 1, &max_depth))
		return false;

	if(num_samples > 0) // Otherwise, use default.
		globales->samples_per_pixel = num_samples;

	if(max_depth > 0) // Otherwise, use default.
		globales->max_depth = max_depth;

	if(!ignorar_chars())
		return false;

	if(!read_token(temp))
		return false;

	if(temp != "/renderer")
		return false;

	if(tipo == "whitted") {
		if(globales->renderer == nullptr)
			globales->renderer = new DirectRenderer(globales->max_depth);
		else // Renderer ya creado, error.
			return false;
	}
	else if(tipo == "path") {
		if(globales->renderer == nullptr)
			globales->renderer = new PathRenderer(globales->max_depth, time(nullptr));
		else
			return false;
	}
	else // Renderer desconocido, error.
		return false;

	return true;
}

bool Parser::process_camera()
{
	std::string tipo, // Tipo de camera
				temp; // Auxiliar para comprobaciones.
	Point		pos;
	Vec3		gaze, up;
	double		dist, low_x, low_y, high_x, high_y, push;

	double		aux[2];

	if(!read_bloque_txt("type", tipo))
		return false;

	if(!read_bloque_point("pos", pos))
		return false;

	if(!read_bloque_vec3("gaze", gaze))
		return false;

	if(!read_bloque_vec3("up", up))
		return false;

	if(!read_bloque_floats("dist", 1, &dist))
		return false;

	if(!read_bloque_floats("low_corner", 2, aux))
		return false;

	low_x = aux[0];
	low_y = aux[1];

	if(!read_bloque_floats("high_corner", 2, aux))
		return false;

	high_x = aux[0];
	high_y = aux[1];

	if(!read_bloque_floats("push", 1, &push))
		return false;

	if(!ignorar_chars())
		return false;

	if(!read_token(temp))
		return false;

	if(temp != "/camera")
		return false;

	if(tipo == "pinhole") {
		if(globales->camera == nullptr)
			globales->camera = new Pinhole(pos, gaze, up, dist,
				low_x, low_y, high_x, high_y, push);
		else // Camera ya creada, error.
			return false;
	}
	else
		// Tipo de camara desconocida.
		return false;

	return true;
}

bool Parser::process_image()
{
	std::string temp;
	int			res_x, res_y;
	double		background_color[3];

	if(!read_bloque_ints("res_x", 1, &res_x))
		return false;

	if(!read_bloque_ints("res_y", 1, &res_y))
		return false;

	if(!read_bloque_floats("bgcolor", 3, background_color))
		return false;

	if(res_x > 0) // Otherwise, use default.
		globales->res_x = res_x;

	if(res_y > 0) // Otherwise, use default.
		globales->res_y = res_y;

	if(!ignorar_chars())
		return false;

	if(!read_token(temp))
		return false;

	if(temp != "/image")
		return false;

	if(globales->image == nullptr)
		globales->image = new Image(res_x, res_y,
			RGB(background_color[0], background_color[1], background_color[2]));
	else // Image ya creada, error.
		return false;

	return true;
}

bool Parser::process_scene()
{
	std::string etiqueta;
    bool 		fin_bloque = false;

	if(globales->scene == nullptr) {
		globales->scene = new Scene;

		while(!fin_bloque) {
			// Buscamos etiqueta.
			if(!ignorar_chars())
				return false;

			if(!read_token(etiqueta))
				return false;

			if(etiqueta == "/scene")
				fin_bloque = true;
			else
			{
				if(etiqueta == "texture") {
					if(!process_texture())
						return false;
				}
				else if(etiqueta == "material") {
					if(!process_material())
						return false;
				}
				else if(etiqueta == "object") {
					if(!process_object())
						return false;
				}
				else // Etiqueta desconocida, no perteneciente a la seccion config.
					return false;
			}
		}
	}
	else // Scene ya creada, error.
		return false;

	return true;
}

bool Parser::process_texture()
{
	std::string tipo;

	if(!read_bloque_txt("type", tipo))
		return false;

	if(tipo == "simple") {
		if(!process_simple_tex())
			return false;
	}
	else // Textura desconocida, error.
		return false;

	return true;
}

bool Parser::process_simple_tex()
{
	std::string	id, temp;
	Vec3		color;
	Texture		*texture;

	if(!read_bloque_txt("id", id))
		return false;

	if(!read_bloque_vec3("color", color))
		return false;

	if(!ignorar_chars())
		return false;

	if(!read_token(temp))
		return false;

	if(temp != "/texture")
		return false;

	texture = new SimpleTexture(RGB(color.x(), color.y(), color.z()));

    return globales->scene->add_texture(texture, id);
}

bool Parser::process_material()
{
	std::string tipo;

	if(!read_bloque_txt("type", tipo))
		return false;

	if(tipo == "diffuse") {
		if(!process_diffuse_mat())
			return false;
	}
	else if(tipo == "specular") {
		if(!process_specular_mat())
			return false;
	}
	else if(tipo == "dielectric") {
		if(!process_dielectric_mat())
			return false;
	}
	else if(tipo == "light") {
		if(!process_light_mat())
			return false;
	}
	else // Material desconocido, error.
		return false;

	return true;
}

bool Parser::process_diffuse_mat()
{
	std::string id,
                texture_id,
                temp;
	Material 	*material;
	Texture		*texture;

	if(!read_bloque_txt("id", id))
		return false;

	if(!read_bloque_txt("texture_id", texture_id))
		return false;

	if(!ignorar_chars())
		return false;

	if(!read_token(temp))
		return false;

	if(temp != "/material")
		return false;

	if((texture = globales->scene->get_texture(texture_id)) != nullptr) {
		material = new DiffuseMaterial(texture);
		if(!globales->scene->add_material(material, texture_id, id))
			return false; // Existe un material con el mismo ID.
	}
	else // La textura asociada al material no existe.
		return false;

	return true;
}

bool Parser::process_specular_mat()
{
	std::string id, texture_id, temp;
	Material 	*material;
	Texture		*texture;

	if(!read_bloque_txt("id", id))
		return false;

	if(!read_bloque_txt("texture_id", texture_id))
		return false;

	if(!ignorar_chars())
		return false;

	if(!read_token(temp))
		return false;

	if(temp != "/material")
		return false;

	if((texture = globales->scene->get_texture(texture_id)) != nullptr) {
		material = new SpecularMaterial(texture);
		if(!globales->scene->add_material(material, texture_id, id))
			return false; // Existe un material con el mismo ID.
	}
	else // La textura asociada al material no existe.
		return false;

	return true;
}

bool Parser::process_dielectric_mat()
{
	std::string id, texture_id, temp;
	double		ior;
	Material 	*material;
	Texture		*texture;

	if(!read_bloque_txt("id", id))
		return false;

	if(!read_bloque_txt("texture_id", texture_id))
		return false;

	if(!read_bloque_floats("ior", 1, &ior))
		return false;

	if(!ignorar_chars())
		return false;

	if(!read_token(temp))
		return false;

	if(temp != "/material")
		return false;

	if((texture = globales->scene->get_texture(texture_id)) != nullptr) {
		material = new DielectricMaterial(texture, ior);
		if(!globales->scene->add_material(material, texture_id, id))
			return false; // Existe un material con el mismo ID.
	}
	else // La textura asociada al material no existe.
		return false;

	return true;
}

bool Parser::process_light_mat()
{
	std::string id, texture_id, temp;
	Material 	*material;
	Texture		*texture;

	if(!read_bloque_txt("id", id))
		return false;

	if(!read_bloque_txt("texture_id", texture_id))
		return false;

	if(!ignorar_chars())
		return false;

	if(!read_token(temp))
		return false;

	if(temp != "/material")
		return false;

	if((texture = globales->scene->get_texture(texture_id)) != NULL) {
		material = new LightMaterial(texture);
		if(!globales->scene->add_material(material, texture_id, id))
			return false; // Existe un material con el mismo ID.
	}
	else // La textura asociada al material no existe.
		return false;

	return true;
}

bool Parser::process_object()
{
	std::string tipo;

	if(!read_bloque_txt("type", tipo))
		return false;

	if(tipo == "plane") {
		if(!process_plane())
			return false;
	}
	else if(tipo == "disc") {
		if(!process_disc())
			return false;
	}
	else if(tipo == "sphere") {
		if(!process_sphere())
			return false;
	}
	else if(tipo == "cylinder") {
		if(!process_cylinder())
			return false;
	}
	else if(tipo == "box") {
		if(!process_box())
			return false;
	}
	else if(tipo == "parallelogram") {
		if(!process_parallelogram())
			return false;
	}
	else if(tipo == "triangle") {
		if(!process_triangle())
			return false;
	}
	else if(tipo == "mesh") {
		if(!process_mesh())
			return false;
	}
	else
		// Tipo de objeto desconocido.
		return false;

	return true;
}

bool Parser::process_plane()
{
	std::string material_id, temp;
	Point       base;
	Vec3		normal;
	Material	*material;
	Shape		*shape;
	Transform	trans;

	if(!read_bloque_point("base", base))
		return false;

	if(!read_bloque_vec3("normal", normal))
		return false;

	if(!read_bloque_txt("material", material_id))
		return false;

	if(!ignorar_chars())
		return false;

	if(!read_token(temp))
		return false;

	if(temp == "transform") {
		if(!process_transform(&trans))
			return false;

		if(!ignorar_chars())
			return false;

		if(!read_token(temp))
			return false;
	}

	if(temp != "/object")
		return false;

	if((material = globales->scene->get_material(material_id)) != nullptr) {
		shape = new Plane(base, normal, material);
		shape->set_trans(&trans);
		globales->scene->add_object(shape, material_id, material->is_light());

		return true;
	}
	else { // Material asociado al objeto desconocido.
		return false;
	}
}

bool Parser::process_disc()
{
	std::string material_id, temp;
	Point       center;
	Vec3		normal;
	double		radius;
	Material	*material;
	Shape		*shape;
	Transform	trans;

	if(!read_bloque_point("center", center))
		return false;

    if(!read_bloque_vec3("normal", normal))
		return false;

	if(!read_bloque_floats("radius", 1, &radius))
		return false;

	if(!read_bloque_txt("material", material_id))
		return false;

	if(!ignorar_chars())
		return false;

	if(!read_token(temp))
		return false;

	if(temp == "transform") {
		if(!process_transform(&trans))
			return false;

		if(!ignorar_chars())
			return false;

		if(!read_token(temp))
			return false;
	}

	if(temp != "/object")
		return false;

	if((material = globales->scene->get_material(material_id)) != nullptr) {
		shape = new Disc(center, normal, radius, material);
		shape->set_trans(&trans);
		globales->scene->add_object(shape, material_id, material->is_light());
	}
	else // Material asociado al objeto desconocido.
		return false;

	return true;
}

bool Parser::process_sphere()
{
	std::string material_id, temp;
	Point		center;
	double		radius;
	Material	*material;
	Shape		*shape;
	Transform	trans;

	if(!read_bloque_point("center", center))
		return false;

	if(!read_bloque_floats("radius", 1, &radius))
		return false;

	if(!read_bloque_txt("material", material_id))
		return false;

	if(!ignorar_chars())
		return false;

	if(!read_token(temp))
		return false;

	if(temp == "transform") {
		if(!process_transform(&trans))
			return false;

		if(!ignorar_chars())
			return false;

		if(!read_token(temp))
			return false;
	}

	if(temp != "/object")
		return false;

	if((material = globales->scene->get_material(material_id)) != nullptr) {
		shape = new Sphere(center, radius, material);
		shape->set_trans(&trans);
		globales->scene->add_object(shape, material_id, material->is_light());
	}
	else // Material asociado al objeto desconocido.
		return false;

	return true;
}

bool Parser::process_cylinder()
{
	std::string material_id, temp;
	Point		bottom, top;
	double	    radius;
	Material	*material;
	Shape		*shape;
	Transform	trans;

	if(!read_bloque_point("center_0", bottom))
		return false;

	if(!read_bloque_point("center_1", top))
		return false;

	if(!read_bloque_floats("radius", 1, &radius))
		return false;

	if(!read_bloque_txt("material", material_id))
		return false;

	if(!ignorar_chars())
		return false;

	if(!read_token(temp))
		return false;

	if(temp == "transform") {
		if(!process_transform(&trans))
			return false;

		if(!ignorar_chars())
			return false;

		if(!read_token(temp))
			return false;
	}

	if(temp != "/object")
		return false;

	if((material = globales->scene->get_material(material_id)) != nullptr) {
		shape = new Cylinder(bottom, top, radius, material);
		shape->set_trans(&trans);
		globales->scene->add_object(shape, material_id, material->is_light());
	}
	else // Material asociado al objeto desconocido.
		return false;

	return true;
}

bool Parser::process_box()
{
	std::string material_id, temp;
	Point		min_corner, max_corner;
	Material	*material;
	Shape		*shape;
	Transform	trans;

	if(!read_bloque_point("min", min_corner))
		return false;

	if(!read_bloque_point("max", max_corner))
		return false;

	if(!read_bloque_txt("material", material_id))
		return false;

	if(!ignorar_chars())
		return false;

	if(!read_token(temp))
		return false;

	if(temp == "transform") {
		if(!process_transform(&trans))
			return false;

		if(!ignorar_chars())
			return false;

		if(!read_token(temp))
			return false;
	}

	if(temp != "/object")
		return false;

	if((material = globales->scene->get_material(material_id)) != nullptr) {
		shape = new Box(min_corner, max_corner, material);
		shape->set_trans(&trans);
		globales->scene->add_object(shape, material_id, material->is_light());
	}
	else // Material asociado al objeto desconocido.
		return false;

	return true;
}

bool Parser::process_parallelogram()
{
	std::string material_id, temp;
	Point		base;
	Vec3		u,
				v;
	Material	*material;
	Shape		*shape;
	Transform	trans;

	if(!read_bloque_point("base", base))
		return false;

	if(!read_bloque_vec3("vec_u", u))
		return false;

	if(!read_bloque_vec3("vec_v", v))
		return false;

	if(!read_bloque_txt("material", material_id))
		return false;

	if(!ignorar_chars())
		return false;

	if(!read_token(temp))
		return false;

	if(temp == "transform") {
		if(!process_transform(&trans))
			return false;

		if(!ignorar_chars())
			return false;

		if(!read_token(temp))
			return false;
	}

	if(temp != "/object")
		return false;

	if((material = globales->scene->get_material(material_id)) != nullptr) {
		shape = new Parallelogram(base, u, v, material);
		shape->set_trans(&trans);
		globales->scene->add_object(shape, material_id, material->is_light());
	}
	else // Material asociado al objeto desconocido.
		return false;

	return true;
}

bool Parser::process_triangle()
{
	std::string material_id, temp;
	Point		p0,
				p1,
				p2;
	Material	*material;
	Shape		*shape;
	Transform	trans;

	if(!read_bloque_point("vertex", p0))
		return false;

	if(!read_bloque_point("vertex", p1))
		return false;

	if(!read_bloque_point("vertex", p2))
		return false;

	if(!read_bloque_txt("material", material_id))
		return false;

	if(!ignorar_chars())
		return false;

	if(!read_token(temp))
		return false;

	if(temp == "transform") {
		if(!process_transform(&trans))
			return false;

		if(!ignorar_chars())
			return false;

		if(!read_token(temp))
			return false;
	}

	if(temp != "/object")
		return false;

	if((material = globales->scene->get_material(material_id)) != nullptr) {
		shape = new Triangle(p0, p1, p2, material);
		shape->set_trans(&trans);
		globales->scene->add_object(shape, material_id, material->is_light());
	}
	else // Material asociado al objeto desconocido.
		return false;

	return true;
}

bool Parser::process_mesh()
{
	std::string material_id, temp;
	Material	*material;
	Shape		*shape;
	Transform	trans;

	// Lista de punteros a MeshTriangle a añadir a la escena, que será
	// construida durante la lectura de la lista de vértices o del
	// fichero .obj que se nos proporcione. Una vez construida, los
	// añadiremos a la escena.
	std::vector<MeshTriangle *> triangulos;

	if(!ignorar_chars())
			return false;

	if(!read_token(temp))
		return false;

	if(temp == "vertex_list") {
        shape = new Mesh();
		if(!process_mesh_vertex(shape, &triangulos)) {
            delete shape;
            return false;
		}

	}
	else if(temp == "file") {
		shape = new Mesh();
		if(!process_mesh_file(shape, &triangulos)) {
            delete shape;
            return false;
		}
	}
	else // Etiqueta desconocida, descripción errónea de la malla
		return false;

	if(!read_bloque_txt("material", material_id))
		return false;

	if(!ignorar_chars())
		return false;

	if(!read_token(temp))
		return false;

	if(temp == "transform") {
		if(!process_transform(&trans))
			return false;

		if(!ignorar_chars())
			return false;

		if(!read_token(temp))
			return false;
	}

	if(temp != "/object")
		return false;

	if((material = globales->scene->get_material(material_id)) != nullptr) {
		shape->set_material(material);
		shape->set_trans(&trans);
		globales->scene->add_object(shape, material_id, material->is_light());

		// Añadimos ahora a la escena todos los triángulos que componen la malla
		for(int i = 0; i < (int) triangulos.size(); i++)
			globales->scene->add_object((Shape *) triangulos[i], material_id, material->is_light());
	}
	else // Material asociado al objeto desconocido.
		return false;

	return true;
}

bool Parser::process_mesh_vertex(Shape *shape, std::vector<MeshTriangle *> *triangulos)
{
	std::string 	etiqueta;
	Point			p0, p1, p2;
	Mesh			*mesh = dynamic_cast<Mesh *> (shape);

	while(1) { // Always true
		if(!ignorar_chars())
				return false;

		if(!read_token(etiqueta))
			return false;

		if(etiqueta == "/vertex_list") {
			// Fin del bloque de vertices
			return true;
		}
		else if(etiqueta == "v") {
			// Retrocedemos 3 posiciones en el archivo para poder leer
			// correctamente el vector
			for(int i=0; i < 3; i++, fichero_escena.unget());

			if(!read_bloque_point("v", p0))
				return false;
			if(!read_bloque_point("v", p1))
				return false;
			if(!read_bloque_point("v", p2))
				return false;

			mesh->add_vertex(p0);
			mesh->add_vertex(p1);
			mesh->add_vertex(p2);

			MeshFace 	face(-1);
			int			num_triangles = mesh->get_triangle_count();

			// Triangle vertex
			face.vertex_index[0] = (num_triangles * 3);
			face.vertex_index[1] = (num_triangles * 3) + 1;
			face.vertex_index[2] = (num_triangles * 3) + 2;

			// No normal given, calculate from points.

			// No texture given.

			// Creamos el triangulo
			MeshTriangle *triangle = new MeshTriangle(face, mesh);

			// Añadimos el triangulo a la lista
			triangulos->push_back(triangle);

			// Incrementamos el numero de triangulos que tiene la malla
			mesh->increase_triangle_count();
		}
	}
}

bool Parser::process_mesh_file(Shape *shape, std::vector<MeshTriangle *> *triangulos)
{
	std::string 	etiqueta,
                    fichero,
                    linea,
                    tipo;
	std::ifstream 	file_stream;

	Point			v;	// Vertice
	Vec3			n;	// Normal
	Vec2			t;	// Textura

	Mesh			*mesh = dynamic_cast<Mesh *> (shape);

	int				vertex      = 0,
                    normals     = 0,
                    textures    = 0,
                    faces       = 0;


	if(!read_content(fichero, '<'))
        return false;

	if(!read_token(etiqueta))
		return false;

	if(etiqueta != "/file")
		return false;

	// Abrir fichero
	file_stream.open(fichero.c_str());

	// Leemos el fichero linea a linea
	while(std::getline(file_stream, linea)){
		if((linea[0] == 'v') && (linea[1] == ' ')) { // Vertice
			// Procesar vertice

			unsigned int i = 1; // Posicion en la linea

			// Evitamos los espacios antes del primer numero, si los hay
			while(linea[i] == ' ') i++;

			for(int j = 0; j < 3; j++) { // Tres veces para tres float
				std::string temp("");

				while(1) { // Hasta que no encontremos un espacio
					if((linea[i] != ' ') && (i < (linea.length() -1))) {
						temp = temp + linea[i];
						i++; // Siguiente caracter
					}
					else
					{
						i++; 	// Saltamos el espacio en blanco
						break;
					}
				}

				if(temp.length() > 0) {
					if(valid_float(temp))
						v.e[j] = atof(temp.c_str());
					else {
						// Lanzar error
					}
				}
			}

			mesh->add_vertex(v);
            vertex++;
		}
		else if((linea[0] == 'v') && (linea[1] == 'n')) { // Normal
			// Procesar normal

			unsigned int i = 3; // Posicion en la linea
			for(int j = 0; j < 3; j++) { // Tres veces para tres float
				std::string temp("");

				while(1) { // Hasta que no encontremos un espacio
					if((linea[i] != ' ') && (i < (linea.length() -1))) {
						temp = temp + linea[i];
						i++; // Siguiente caracter
					}
					else
					{
						i++; 	// Saltamos el espacio en blanco
						break;
					}
				}

				if(temp.length() > 0) {
					if(valid_float(temp))
						n.e[j] = atof(temp.c_str());
					else {
						// Lanzar error
					}
				}
			}

			mesh->add_normal(n);
            normals++;
		}
		else if((linea[0] == 'v') && (linea[1] == 't')) { // Texture
			// Procesar textura

			unsigned int i = 3; // Posicion en la linea
			for(int j = 0; j < 2; j++) { // Dos veces para Dos float
				std::string temp("");

				while(1) { // Hasta que no encontremos un espacio
					if((linea[i] != ' ') && (i < (linea.length() -1))) {
						temp = temp + linea[i];
						i++; // Siguiente caracter
					}
					else
					{
						i++; 	// Saltamos el espacio en blanco
						break;
					}
				}

				if(temp.length() > 0) {
					if(valid_float(temp))
						t.e[j] = atof(temp.c_str());
					else {
						// Lanzar error
					}
				}
			}

            mesh->add_texture(t);
            textures++;
		}
		else if((linea[0] == 'f') && (linea[1] == ' ')) { // Face
			// Procesar face
			MeshFace        face(-1);

			unsigned int    i       = 2,
                            block   = 0;

			while(block < 3) {
				int number = 0; 			// Primer numero del bloque
				std::string temp(""); 	// Temporal para almacenar numeros

				while(number < 3) { // Repetir hasta el tercer numero
					if(linea[i] == '/') { // Fin de numero, excepto el ultimo del bloque
						// Si la cadena temporal tiene un entero valido
						if((temp.length() > 0) && valid_int(temp)) {
							switch(number) { // Veamos que posicion tenemos
								case 0: // Vertice
									face.vertex_index[block] = atoi(temp.c_str()) - 1;
									break;
								case 1: // Textura
									face.texture_index[block] = atoi(temp.c_str()) - 1;
									break;
							}
						}
						// Si la cadena está vacía o el numero no es valido
						// no almacenamos nada. El valor está por defecto a -1,
						// con lo que el render sabrá qué hacer en caso de
						// encontrarlo.

						temp.clear(); 	// Limpiamos la cadena
						number++;			// Siguiente numero
						i++;				// Avanzamos en la linea
					}
					else if(linea[i] == ' ') { // Fin de bloque
						// Almacenamos el numero, si lo hay
						if((temp.length() > 0) && valid_int(temp)) {
							switch(number) {
								case 0: // Vertice
									face.vertex_index[block] = atoi(temp.c_str()) - 1;
									break;
								case 1: // Textura
									face.texture_index[block] = atoi(temp.c_str()) - 1;
									break;
								case 2: // Normal
									face.normal_index[block] = atoi(temp.c_str()) - 1;
									break;
							}
						}

						number = 3;	// Salimos del while
						block++;		// Nuevo bloque
						i++;			// Avanzamos por la cadena
					}
					else if(i == (linea.length() - 1)) { // Final de linea
						// Almacenamos el numero, si lo hay
						if((temp.length() > 0) && valid_int(temp)) {
							switch(number) {
								case 0: // Vertice
									face.vertex_index[block] = atoi(temp.c_str()) - 1;
									break;
								case 1: // Textura
									face.texture_index[block] = atoi(temp.c_str()) - 1;
									break;
								case 2: // Normal
									face.normal_index[block] = atoi(temp.c_str()) - 1;
									break;
							}
						}

						number = 3;
						block = 3; 	// Salimos de ambos while
					}
					else { // Metemos el caracter en la cadena temporal, y seguimos
						temp = temp + linea[i];
						i++;
					}
				}
			}

			// Creamos el nuevo triangulo.
			MeshTriangle *new_t = new MeshTriangle(face, mesh);
			// Lo añadimos a la lista
			triangulos->push_back(new_t);

			// Incrementamos el numero de triangulos en la malla
			mesh->increase_triangle_count();

			faces++;
		}
	}
	file_stream.close();

	return true;
}

bool Parser::process_transform(Transform *trans)
{
	std::string etiqueta;
	Point		p;

	while(1) { // Always true
		if(!ignorar_chars())
				return false;

		if(!read_token(etiqueta))
			return false;

		if(etiqueta == "/transform") {
			// Fin del bloque de transformaciones
			return true;
		}
		else {
			// Retrocedemos hasta el inicio de la etiqueta, para poder
			// utilizar las funciones de lectura de datos.
			for(unsigned int i = 0; i < etiqueta.length() + 2; i++, fichero_escena.unget());

			if(etiqueta == "translate") {
				if(!read_bloque_point(etiqueta, p))
					return false;

				trans->translate(p);
			}
			else if(etiqueta == "scale") {
				if(!read_bloque_point(etiqueta, p))
					return false;

				trans->scale(p);
			}
			else if(etiqueta == "rotate_axis") {
				double lista[4];

				if(!read_bloque_floats(etiqueta, 4, lista))
					return false;

				trans->rotate(lista[0], Vec3(lista[1], lista[2], lista[3]));
			}
			else if(etiqueta == "rotate_x") {
				double ang;

				if(!read_bloque_floats(etiqueta, 1, &ang))
					return false;

				trans->rotate_x(ang);
			}
			else if(etiqueta == "rotate_y") {
				double ang;

				if(!read_bloque_floats(etiqueta, 1, &ang))
					return false;

				trans->rotate_y(ang);
			}
			else if(etiqueta == "rotate_z") {
				double ang;

				if(!read_bloque_floats(etiqueta, 1, &ang))
					return false;

				trans->rotate_z(ang);
			}
			else {
				// Etiqueta desconocida.
				num_error = 0;
				return false;
			}
		} // else
	} // while
}
