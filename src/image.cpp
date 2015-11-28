#include <cmath>
#include "image.h"

Image::Image(int a_width, int a_height)
{
	width   = a_width;
	height  = a_height;

    pixels = new Contrib*[width];

	for(int i = 0; i < width; i++)
		pixels[i] = new Contrib[height];
}

Image::Image(int a_width, int a_height, const RGB &a_color)
{
	width   = a_width;
	height  = a_height;

	pixels = new Contrib*[width];

	for(int i = 0; i < width; i++) {
		pixels[i] = new Contrib[height];

        for(int j = 0; j < height; j++) {
			pixels[i][j][0].set(a_color);
		}
	}
}

Image::~Image()
{
    for(int i = 0; i < width; ++i) {
        delete [] pixels[i];
    }

    delete [] pixels;
}

bool Image::set(int a_width, int a_height, const RGB &a_color)
{
	if(a_width < 0 || a_width > width) return false;
	if(a_height < 0 || a_height > height) return false;

	pixels[a_width][a_height][0].set(a_color);

	return true;
}

bool Image::set(int a_width, int a_height, const Contrib &a_contrib)
{
	if(a_width < 0 || a_width > width) return false;
	if(a_height < 0 || a_height > height) return false;

	pixels[a_width][a_height] = a_contrib;

	return true;
}

bool Image::get(int a_width, int a_height, RGB &a_color)
{
	if(a_width < 0 || a_width > width) return false;
	if(a_height < 0 || a_height > height) return false;

	a_color.set(pixels[a_width][a_height][0]);

	return true;
}

bool Image::get(int a_width, int a_height, Contrib &a_contrib)
{
	if(a_width < 0 || a_width > width) return false;
	if(a_height < 0 || a_height > height) return false;

	a_contrib = pixels[a_width][a_height];

	return true;
}


void Image::gamma_correct(double gamma)
{
	Contrib c;
	double  power = 1.0f / gamma;

	for(int x = 0; x < width; ++x) {
		for(int y = 0; y < height; ++y) {
			c = pixels[x][y];

			for(int z = 0; z < 4; ++z)
                c[z].clamp(0.0f, 1.0f);

            for(int z = 0; z < 4; ++z) {
                pixels[x][y][z].R = pow(c[z].r(), power);
                pixels[x][y][z].G = pow(c[z].g(), power);
                pixels[x][y][z].B = pow(c[z].b(), power);
			}
		}
	}
}

void Image::save_ppm(std::ostream &os, int type)
{
	// write header
	os << "P6\n" << width << ' ' << height << "\n255\n";

	// write clamped [0, 255] values
	for(int y = height-1; y >= 0; y--) {
		for(int x = 0; x < width; x++) {
			unsigned int img_r = static_cast<unsigned int> (256 * pixels[x][y][type].r());
			unsigned int img_g = static_cast<unsigned int> (256 * pixels[x][y][type].g());
			unsigned int img_b = static_cast<unsigned int> (256 * pixels[x][y][type].b());

			if(img_r > 255) img_r = 255;
			if(img_g > 255) img_g = 255;
			if(img_b > 255) img_b = 255;

			unsigned char file_r = static_cast<unsigned char> (img_r);
			unsigned char file_g = static_cast<unsigned char> (img_g);
			unsigned char file_b = static_cast<unsigned char> (img_b);

			os.put(file_r);
			os.put(file_g);
			os.put(file_b);
		}
	}
}

void Image::create_final_img()
{
    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            pixels[x][y][0].set(0.0f, 0.0f, 0.0f);

            // Acumulamos los resultados.
            for(int z = 1; z < 4; ++z)
                pixels[x][y][0] += pixels[x][y][z];
        }
    }
}
