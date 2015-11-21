#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstring>

#include "parser/parser.h"

#include "test.h"

#include "vzray.h"
#include "statistics.h"

static void muestra_ayuda(std::string name)
{
    std::cerr 	<< "Uso: " << name << " <opcion(es)> fichero-a-renderizar <fichero-imagen-salida>\n"
				<< "Opciones:\n"
				<< "\t-h,--help    \t\tMuestra este mensaje.\n"
				<< "\t-t,--test    \t\tRealiza un test (ignorado si se indica fichero-a-renderizar).\n"
				<< "\t-s,--showaabb\t\tMuestra las AABB en la escena.\n"
				<< std::endl;
}

// Prints info of render process.
void imprime_info(int linea_act, int lineas_tot);
void print_time(string head, double ticks);
void print_statistics();

// Intiating data structures
bool init_data(Globals *globales);
// Leer la escena del archivo.
bool parse_file(Globals *globals, std::string scene_desc_file);
// Main render loop
bool start_render(Globals *globales);
bool start_render_v2(Globals *globales);
// Guarda la imagen.
bool save_file(Globals *globales, std::string output_file);
// Cleaning the room
bool clean_data(Globals *globales);

// Tells renderer to show AABB
bool show_aabb(Globals *globales);

// Devuelve el nombre del fichero, sin path y sin extension.
void image_file_name(const std::string &input_file, std::string &output_file);

int main(int argc, char *argv[])
{
	Globals globales;
	clock_t render_ticks;
            //tInitDataTicks,
            //tCleanDataTicks;
    std::string scene_desc_file,
                output_file;
	bool end_status;

	if(argc > 1) {
		for(int i = 1; i < argc; ++i) {
			if(argv[i][0] == '-') { // Procesamos las opciones.
				if(std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help") {
					muestra_ayuda(argv[0]);
					return 0;
				}
				else if(std::string(argv[i]) == "-t" || std::string(argv[i]) == "--test") {
					globales.options |= GLB_DO_TEST;
				}
				else if(std::string(argv[i]) == "-s" || std::string(argv[i]) == "--showaabb") {
					globales.options |= GLB_SHOW_AABB;
				}
			}
			else {
				// Se proporciona fichero, anulamos el test.
				globales.options &= ~GLB_DO_TEST;

				if(scene_desc_file.empty())
					// Fichero de escena a renderizar
					scene_desc_file.assign(argv[i]);
				else
					// Fichero donde guardar la imagen
					output_file.assign(argv[i]);
			}
		}
	}

	if(scene_desc_file.empty() || (argc < 2)) {
		muestra_ayuda(argv[0]);
		return 0;
	}
	else {
        std::streambuf *log_buf, *backup;
        std::fstream log;

        // Redirigiremos clog a un fichero para escribir cosas interesantes en él.
        log.open("log.txt", std::fstream::out);

        backup = std::clog.rdbuf();	// Backup del streambuf de clog

        log_buf = log.rdbuf();      // Obtenemos el streambuf del fichero
        std::clog.rdbuf(log_buf);	// Redirigimos clog

        if(globales.options & GLB_DO_TEST) {
            Test test;
            end_status = test.launch_test();
        }
        else {
            end_status = parse_file(&globales, scene_desc_file);
            if(end_status) {
                render_ticks = clock();

                // Start render loop
                end_status = start_render(&globales);

                render_ticks = clock() - render_ticks;

                print_time("\nRender Time: ", static_cast<float>(render_ticks)/CLOCKS_PER_SEC);
                print_statistics();


                if(output_file.empty()) {
                    std::string	temp;

                    // Averiguamos el nombre del fichero.
                    image_file_name(scene_desc_file, temp);

                    // Añadimos la extension.
                    temp += ".ppm";

                    output_file = temp;
                }

                save_file(&globales, output_file);
            }
        }

        std::clog.rdbuf(backup);		// Restauramos el streambuf de clog
        log.close();					// Cerramos el fichero de log.
    }

	if(end_status)
		return 0;
	else
		return -1;
}

bool start_render(Globals *globales)
{
	CRandomMother 	rng(time(NULL));
	std::string     renderer_type;

	switch(globales->renderer->renderer_type()) {
		case 0:
			renderer_type = "Whitted raytracing";
			break;
		case 1:
			renderer_type = "Pathtracing";
			break;
		default:
			renderer_type = "Unknown";
			break;
	}

	std::cout 	<< "\nRendering:\n"
				<< " - Samples per pixel:     \t" << globales->samples_per_pixel << " spp\n"
				<< " - Shadow rays per sample:\t" << globales->shadow_rays << " sps\n"
				<< " - Image resolution:      \t" << globales->res_x << "x" << globales->res_y << " px\n"
				<< " - Illumination strategy: \t" << renderer_type
				<< std::endl << std::endl;

	std::clog 	<< "Enter: Main render loop.\n";

	// Si se indica, mostramos las AABB
	if(globales->options & GLB_SHOW_AABB)
		globales->scene->show_AABB();

	for(int i = 0; i < globales->res_x; i++) {
		imprime_info(i+1, globales->res_x);
		for(int j = 0; j < globales->res_y; j++) {
			RGB pixel_color(0.0f, 0.0f, 0.0f);

			//i = globales->res_x/2; j = globales->res_y/2;
			//i=50; j=250;
			//std::clog << "StartRender::Shooting ray!" << endl;
			if(globales->samples_per_pixel > 1) {
				for(int k = 0; k < globales->samples_per_pixel; k++) {
					// std::clog << "Sampling (i, j, k) = (" << i << ", " << j << ", " << k << ")" << std::endl;
                    Ray r = globales->camera->get_ray((double(i)+rng.Random()-.5)/double(globales->res_x), (double(j)+rng.Random()-.5)/double(globales->res_y), rng.Random(), rng.Random());
                    //Ray r = globales->camera->get_ray(double(i)/double(globales->res_x), double(j)/double(globales->res_y), 0.0f, 0.0f);

					pixel_color += globales->renderer->get_color(r, globales->scene, .00001f, 1e5, 1) * 1.0/globales->samples_per_pixel;
				}
			}
			else
			{
				Ray r = globales->camera->get_ray(double(i)/double(globales->res_x), double(j)/double(globales->res_y), rng.Random(), rng.Random());

				//pixel_color = pixel_color + globales->renderer->get_color(r, globales->scene, .00001f, 1e5, 1);
				pixel_color += globales->renderer->get_color(r, globales->scene, .00001f, 1e5, 1);
			}

			//std::clog << "RgbPixelColor: " << pixel_color << endl;
			globales->image->set(i, j, pixel_color);

			//i=globales->res_x; j=globales->res_y;
		}
	}

	std::clog << "Exit: Main render loop.\n";

	return true;
}

bool start_render_v2(Globals *globales)
{
	CRandomMother 	rng(time(NULL));
	std::string     renderer_type;

	switch(globales->renderer->renderer_type()) {
		case 0:
			renderer_type = "Whitted raytracing";
			break;
		case 1:
			renderer_type = "Pathtracing";
			break;
		default:
			renderer_type = "Unknown";
			break;
	}

	std::cout 	<< "\nRendering:\n"
				<< " - Samples per pixel:     \t" << globales->samples_per_pixel << " spp\n"
				<< " - Shadow rays per sample:\t" << globales->shadow_rays << " sps\n"
				<< " - Image resolution:      \t" << globales->res_x << "x" << globales->res_y << " px\n"
				<< " - Illumination strategy: \t" << renderer_type
				<< std::endl << std::endl;

	std::clog 	<< "Enter: Main render loop.\n";

	// Si se indica, mostramos las AABB
	if(globales->options & GLB_SHOW_AABB)
		globales->scene->show_AABB();

	for(int i = 0; i < globales->res_x; i++) {
		imprime_info(i+1, globales->res_x);
		for(int j = 0; j < globales->res_y; j++) {
			RGB pixel_color(0.0f, 0.0f, 0.0f);

			//i = globales->res_x/2; j = globales->res_y/2;
			//i=50; j=250;
			//std::clog << "StartRender::Shooting ray!" << endl;
			if(globales->samples_per_pixel > 1) {
				for(int k = 0; k < globales->samples_per_pixel; k++) {
					// std::clog << "Sampling (i, j, k) = (" << i << ", " << j << ", " << k << ")" << std::endl;
                    Ray r = globales->camera->get_ray((double(i)+rng.Random()-.5)/double(globales->res_x), (double(j)+rng.Random()-.5)/double(globales->res_y), rng.Random(), rng.Random());
                    //Ray r = globales->camera->get_ray(double(i)/double(globales->res_x), double(j)/double(globales->res_y), 0.0f, 0.0f);

					pixel_color += globales->renderer->get_color(r, globales->scene, .00001f, 1e5, 1) * 1.0/globales->samples_per_pixel;
				}
			}
			else
			{
				Ray r = globales->camera->get_ray(double(i)/double(globales->res_x), double(j)/double(globales->res_y), rng.Random(), rng.Random());

				//pixel_color = pixel_color + globales->renderer->get_color(r, globales->scene, .00001f, 1e5, 1);
				pixel_color += globales->renderer->get_color(r, globales->scene, .00001f, 1e5, 1);
			}

			//std::clog << "RgbPixelColor: " << pixel_color << endl;
			globales->image->set(i, j, pixel_color);

			//i=globales->res_x; j=globales->res_y;
		}
	}

	std::clog << "Exit: Main render loop.\n";

	return true;
}

void imprime_info(int linea_act, int lineas_tot)
{
	static char barra;
	int pos = (linea_act * 20) / (lineas_tot);
	char cadena[128] ="\rProgress... [....................]  ";

	for(int i=0; i<20; i++)
	{
		if(i < pos) cadena[14+i] = '+';
	}

	switch(barra) {
		case '|': barra = '/'; break;
		case '/': barra = '-'; break;
		case '-': barra = '\\'; break;
		case '\\': barra = '|'; break;
		default : barra = '|'; break;
	}
	cadena[36] = barra;

	std::cout << cadena << std::flush;
}

void print_time(std::string head, double ticks)
{
	int hours = 0, mins = 0;

	if(ticks >= 3600) { // At least 1 hour
		hours = (int) ticks / 3600;
		ticks = ticks - float(hours * 3600);
	}

	if(ticks >= 60) { // At least 1 min
		mins = (int) ticks / 60;
		ticks = ticks - float(mins * 60);
	}

	std::cout << head << hours << "h " << mins << "m ";
	std::cout << std::fixed << std::setprecision(2) << ticks << "s" << std::endl << std::endl;
}

void print_statistics()
{
    std::cout 	<< "Rayos primarios lanzados:      " << Statistics::num_primary_rays << "\n"
                << " ---\n"
				<< "Numero triangulos:             " << Statistics::num_triangles << "\n"
				<< "Test rayo-triangulo:           " << Statistics::num_triangle_tests << "\n"
				<< "Intersecciones rayo-triangulo: " << Statistics::num_triangles_isec << "\n"
				<< " ---\n"
				<< "Otras primitivas:              " << Statistics::num_primitives << "\n"
				<< "Tests rayo-primitiva:          " << Statistics::num_prim_tests << "\n"
				<< "Intersecciones rayo-primtiva:  " << Statistics::num_prim_isecs << "\n\n";
}

void image_file_name(const std::string &input_file, std::string &output_file)
{
     // Eliminamos hasta la última barra del path.
    int found = input_file.find_last_of("/\\");
    if(found != -1)
        output_file = input_file.substr(found + 1);
    else
        output_file = input_file;

    // Eliminamos desde el último punto.
    found = output_file.find_last_of(".");
    if(found != -1)
        output_file = output_file.substr(0, found);
}

bool parse_file(Globals *globales, std::string scene_desc_file)
{
    Parser parser;

    std::cout << "\nReading file: " << scene_desc_file << " ... ";

	if(!parser.leer_fichero(scene_desc_file, globales)) {
		std::string err_type;
		int err_code, line;

		parser.get_error(err_type, line, err_code);
		std::cerr << "ERROR: Linea " << line;
		std::cerr << " (" << err_code << ") - " << err_type << std::endl;

		return false;
	}

	std::cout << "Done." << std::endl;

	return true;
}

bool save_file(Globals *globales, std::string output_file)
{
    ofstream 	os_image_file;

    globales->image->gamma_correct(2.2f);

	std::cout << "\n\nSaving file: " << output_file << " ... ";

	os_image_file.open(output_file.c_str(), ios::binary);

	globales->image->save_ppm(os_image_file);

	os_image_file.close();

	std::cout << "Done.\n";

	return true;
}
