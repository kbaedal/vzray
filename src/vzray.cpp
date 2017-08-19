#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "parser/parser.h"

#include "test.h"

#include "vzray.h"
#include "statistics.h"
#include "log.h"

// Máscaras para los flags.
// Declaradas como extern. En la definición es reduntante, y no necesario.
namespace Global_opts {
    const unsigned int kglb_show_aabb    = 0x01; // 0000 0001
    const unsigned int kglb_do_test      = 0x02; // 0000 0010
    const unsigned int kglb_do_contrib   = 0x04; // 0000 0100
};

static void muestra_ayuda(std::string name)
{
    std::cerr 	<< "Uso: " << name << " <opcion(es)> fichero-a-renderizar <fichero-imagen-salida>\n"
				<< "Opciones:\n"
				<< "\t-h,--help    \t\tMuestra este mensaje.\n"
				<< "\t-t,--test    \t\tRealiza un test (ignorado si se indica fichero-a-renderizar).\n"
				<< "\t-s,--showaabb\t\tMuestra las AABB en la escena.\n"
				<< "\t-c,--contrib \t\tCrea diferentes imaganes para cada tipo de contribucion al color.\n"
				<< std::endl;
}

// Render process info and log
void imprime_info(int linea_act, int lineas_tot);
void print_time(string head, double ticks);
void print_statistics();
/*
std::streambuf *redirect_clog(std::fstream &log_file);
void restore_clog(std::streambuf *backup, std::fstream &log_file);
void log_message(const std::string &str);
*/

// Intiating data structures
bool init_data(Globals *globales);
// Leer la escena del archivo.
bool parse_file(Globals *globals, std::string scene_desc_file);
// Main render loop
bool start_render(Globals *globales);
bool start_render_v2(Globals *globales);
// Guarda la imagen.
bool save_file(Globals *globales, std::string output_file, int type = 0);
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
                output_file,
                output_file_dir,
                output_file_path;

    // Redirigimos std::clog a un archivo de texto.
    CLog_Redir  clogredir("log.txt");

    // Y configuramos el logger.
    LOGCFG.headers  = true;
    LOGCFG.level    = DEBUG;

    bool end_status;

    LOG() << "main - Comprobando argumentos.";

	if(argc > 1) {
		for(int i = 1; i < argc; ++i) {
			if(argv[i][0] == '-') { // Procesamos las opciones.
				if(std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help") {
					muestra_ayuda(argv[0]);
					return 0;
				}
				else if(std::string(argv[i]) == "-t" || std::string(argv[i]) == "--test") {
					globales.options |= Global_opts::kglb_do_test;
				}
				else if(std::string(argv[i]) == "-s" || std::string(argv[i]) == "--showaabb") {
					globales.options |= Global_opts::kglb_show_aabb;
				}
				else if(std::string(argv[i]) == "-c" || std::string(argv[i]) == "--contrib") {
					globales.options |= Global_opts::kglb_do_contrib;
				}
			}
			else {
				// Se proporciona fichero, anulamos el test.
				globales.options &= ~Global_opts::kglb_do_test;

				if(scene_desc_file.empty())
					// Fichero de escena a renderizar
					scene_desc_file.assign(argv[i]);
				else
					// Fichero donde guardar la imagen
					output_file.assign(argv[i]);
			}
		}
	}

	LOG() << "main -    ... hecho.";

	if((scene_desc_file.empty() || (argc < 2)) && !(globales.options & Global_opts::kglb_do_test)) {
		muestra_ayuda(argv[0]);
		return 0;
	}
	else {

        if(globales.options & Global_opts::kglb_do_test) {
            Test test;
            end_status = test.launch_test();
        }
        else {
            LOG() << "main - Leyendo archivo...";
            end_status = parse_file(&globales, scene_desc_file);
            LOG() << "main -     ... hecho.";
            if(end_status) {
                render_ticks = clock();

                // Start render loop
                if(globales.options & Global_opts::kglb_do_contrib)
                    end_status = start_render_v2(&globales);
                else
                    end_status = start_render(&globales);

                render_ticks = clock() - render_ticks;

                print_time("\n\nRender Time: ", static_cast<float>(render_ticks)/CLOCKS_PER_SEC);
                print_statistics();

                if(output_file.empty()) {
                    std::string	temp;

                    // Averiguamos el nombre del fichero.
                    image_file_name(scene_desc_file, temp);

                    // Añadimos la extension.
                    output_file         = temp + ".ppm";
                    output_file_dir     = temp + "_dir.ppm";
                    output_file_path    = temp + "_path.ppm";
                }

                if(globales.options & Global_opts::kglb_do_contrib)
                    globales.image->create_final_img();

                globales.image->gamma_correct(2.2f);

                save_file(&globales, output_file, 0);

                if(globales.options & Global_opts::kglb_do_contrib) {
                    save_file(&globales, output_file_dir, 1);
                    save_file(&globales, output_file_path, 2);
                }
            }
        }
    }

	if(end_status)
		return 0;
	else
		return -1;
}

bool start_render(Globals *globales)
{
	CRandomMother 	rng(time(NULL));

	std::cout 	<< "\nRendering:\n"
				<< " - Samples per pixel:     \t" << globales->samples_per_pixel << " spp\n"
				<< " - Shadow rays per sample:\t" << globales->shadow_rays << " sps\n"
				<< " - Maximum depth:         \t" << globales->max_depth << "\n"
				<< " - Image resolution:      \t" << globales->res_x << "x" << globales->res_y << " px\n"
				<< " - Illumination strategy: \t" << globales->renderer->renderer_type()
				<< std::endl << std::endl;

	// Si se indica, mostramos las AABB
	if(globales->options & Global_opts::kglb_show_aabb)
		globales->scene->show_AABB();

    LOG() << "start_render:: Generando BVH...";

    globales->scene->create_bvh();

    LOG() << "start_render::      ... hecho.";

    // Una divisón y n multiplicaciones se hacen mas
    // rapido que n divisiones.
    double  samp_div = 1.0f / globales->samples_per_pixel;

    LOG() << "start_render:: Generando muestras...";

    // Generamos las posiciones de muestreo.
    std::vector<Vec2>   pix_samp, cam_samp;

    // Tanto para los pixels como para los rayos de la camara.
    for(int k = 0; k < globales->samples_per_pixel; ++k) {
        pix_samp.push_back(Vec2(rng.Random() - 0.5f, rng.Random() - 0.5f));
        cam_samp.push_back(Vec2(rng.Random(), rng.Random()));
    }

    LOG() << "start_render::   ... hecho.";

    LOG() << "start_render:: Entrando en el bucle principal...";

	for(int i = 0; i < globales->res_x; i++) {
		imprime_info(i+1, globales->res_x);
		for(int j = 0; j < globales->res_y; j++) {
			RGB pixel_color(0.0f);

			//i = globales->res_x/2; j = globales->res_y/2;
			//i=50; j=250;
			//std::clog << "StartRender::Shooting ray!" << endl;

            for(int k = 0; k < globales->samples_per_pixel; ++k) {
                //std::clog << "Sampling (i, j, k) = (" << i << ", " << j << ", " << k << ")" << std::endl;

                double  cam_x = (static_cast<double>(i) + pix_samp[k].x()) / static_cast<double>(globales->res_x),
                        cam_y = (static_cast<double>(j) + pix_samp[k].y()) / static_cast<double>(globales->res_y);

                Ray r = globales->camera->get_ray(cam_x, cam_y, cam_samp[k].x(), cam_samp[k].y());

                pixel_color += globales->renderer->get_color(r, globales->scene, .00001f, 1e5, 1) * samp_div;
            }

			//std::clog << "RgbPixelColor: " << pixel_color << endl;
			globales->image->set(i, j, pixel_color);

			//i=globales->res_x; j=globales->res_y;
		}
	}

	LOG() << "start_render:  ... Fin del bucle principal.";

	return true;
}

bool start_render_v2(Globals *globales)
{
	CRandomMother 	rng(time(NULL));

	std::cout 	<< "\nRendering:\n"
				<< " - Samples per pixel:     \t" << globales->samples_per_pixel << " spp\n"
				<< " - Shadow rays per sample:\t" << globales->shadow_rays << " sps\n"
				<< " - Image resolution:      \t" << globales->res_x << "x" << globales->res_y << " px\n"
				<< " - Illumination strategy: \t" << globales->renderer->renderer_type()
				<< std::endl << std::endl;

	std::clog 	<< "Enter: Main render loop.\n";

	// Si se indica, mostramos las AABB
	if(globales->options & Global_opts::kglb_show_aabb)
		globales->scene->show_AABB();

	for(int i = 0; i < globales->res_x; i++) {
		imprime_info(i+1, globales->res_x);
		for(int j = 0; j < globales->res_y; j++) {
			Contrib pixel_color;

			//i = globales->res_x/2; j = globales->res_y/2;
			//i=50; j=250;
			//std::clog << "StartRender::Shooting ray!" << endl;
			if(globales->samples_per_pixel > 1) {
                // Una divisón y n multiplicacones se hacen mas
                // rapido que n divisiones.
                double samp_div = 1.0f / globales->samples_per_pixel;

				for(int k = 0; k < globales->samples_per_pixel; ++k) {
					// std::clog << "Sampling (i, j, k) = (" << i << ", " << j << ", " << k << ")" << std::endl;
					double  cam_x = (static_cast<double>(i) + rng.Random() - 0.5f)/(static_cast<double>(globales->res_x)),
                            cam_y = (static_cast<double>(j) + rng.Random() - 0.5f)/(static_cast<double>(globales->res_y)),
                            cam_sx = rng.Random(),
                            cam_sy = rng.Random();

                    Ray r = globales->camera->get_ray(cam_x, cam_y, cam_sx, cam_sy);

                    pixel_color += globales->renderer->get_color_v2(r, globales->scene, .00001f, 1e5, 1) * samp_div;
				}
			}
			else
			{
			    double  cam_x = static_cast<double>(i)/static_cast<double>(globales->res_x),
                        cam_y = static_cast<double>(j)/static_cast<double>(globales->res_y),
                        cam_sx = rng.Random(),
                        cam_sy = rng.Random();

				Ray r = globales->camera->get_ray(cam_x, cam_y, cam_sx, cam_sy);

				pixel_color += globales->renderer->get_color_v2(r, globales->scene, .00001f, 1e5, 1);
			}

			//std::clog << "RgbPixelColor: " << pixel_color << endl;
			globales->image->set(i, j, pixel_color);

			//i=globales->res_x; j=globales->res_y;
		}
	}

	LOG() << "Exit: Main render loop.";

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

bool save_file(Globals *globales, std::string output_file, int type)
{
    ofstream 	os_image_file;

	std::cout << "\n\nSaving file: " << output_file << " ... ";

	os_image_file.open(output_file.c_str(), ios::binary);

	globales->image->save_ppm(os_image_file, type);

	os_image_file.close();

	std::cout << "Done.\n";

	return true;
}

/*
std::streambuf *redirect_clog(std::fstream &log_file)
{
    // Redirigiremos clog a un fichero para escribir cosas interesantes en él.
    log_file.open("log.txt", std::fstream::out);

    std::streambuf *backup = std::clog.rdbuf();	// Backup del streambuf de clog

    std::streambuf *log_buf = log_file.rdbuf(); // Obtenemos el streambuf del fichero
    std::clog.rdbuf(log_buf);	// Redirigimos clog

    return backup;
}

void restore_clog(std::streambuf *backup, std::fstream &log_file)
{
    std::clog.rdbuf(backup);    // Restauramos el streambuf de clog
    log_file.close();			// Cerramos el fichero de log.
}

void log_message(const std::string &str)
{
    auto t  = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "[%d-%m-%Y %H:%M:%S] ");
    auto time_str = oss.str();
    time_str += str;

    std::clog << time_str << std::endl;
}
*/
