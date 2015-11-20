#include <cmath>
#include "image.h"

Image::Image()
{}

Image::Image(int a_width, int a_height)
{
	width = a_width;
	height = a_height;

	pixels = new RGB*[width];

	for(int i = 0; i < width; i++)
		pixels[i] = new RGB[height];

    cpixels = new Contrib*[width];

	for(int i = 0; i < width; i++)
		cpixels[i] = new Contrib[height];
}

Image::Image(int a_width, int a_height, const RGB &a_color)
{
	width = a_width;
	height = a_height;

	pixels = new RGB*[width];

	for(int i = 0; i < width; i++) {
		pixels[i] = new RGB[height];

		for(int j = 0; j < height; j++) {
			pixels[i][j].set(a_color);
		}
	}
}

bool Image::set(int a_width, int a_height, const RGB &a_color)
{
	if(a_width < 0 || a_width > width) return false;
	if(a_height < 0 || a_height > height) return false;

	pixels[a_width][a_height].set(a_color);

	return true;
}

bool Image::set(int a_width, int a_height, const Contrib &a_contrib)
{
	if(a_width < 0 || a_width > width) return false;
	if(a_height < 0 || a_height > height) return false;

	cpixels[a_width][a_height].comp[0].set(a_contrib.comp[0]);
	cpixels[a_width][a_height].comp[1].set(a_contrib.comp[1]);
	cpixels[a_width][a_height].comp[2].set(a_contrib.comp[2]);
	cpixels[a_width][a_height].comp[3].set(a_contrib.comp[3]);

	return true;
}

bool Image::get(int a_width, int a_height, RGB &a_color)
{
	if(a_width < 0 || a_width > width) return false;
	if(a_height < 0 || a_height > height) return false;

	a_color.set(pixels[a_width][a_height]);

	return true;
}

bool Image::get(int a_width, int a_height, Contrib &a_contrib)
{
	if(a_width < 0 || a_width > width) return false;
	if(a_height < 0 || a_height > height) return false;

	//a_color.set(pixels[a_width][a_height]);
	a_contrib = cpixels[a_width][a_height];

	return true;
}


void Image::gamma_correct(double gamma)
{
	RGB temp;
	Contrib c;
	double power = 1.0 / gamma;

	for(int x = 0; x < width; x++) {
		for(int y = 0; y < height; y++) {
			temp.set(pixels[x][y]);
			c = cpixels[x][y];

			temp.clamp(0.0f, 1.0f);
			for(int j = 0; j < 4; ++j)
                c.comp[j].clamp(0.0f, 1.0f);

			pixels[x][y].R = pow(temp.r(), power);
			pixels[x][y].G = pow(temp.g(), power);
			pixels[x][y].B = pow(temp.b(), power);

			for(int j = 0; j < 4; ++j) {
                cpixels[x][y].comp[j].R = pow(c.comp[j].r(), power);
                cpixels[x][y].comp[j].G = pow(c.comp[j].g(), power);
                cpixels[x][y].comp[j].B = pow(c.comp[j].b(), power);
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
			unsigned int img_r = (unsigned int) (256 * pixels[x][y].r());
			unsigned int img_g = (unsigned int) (256 * pixels[x][y].g());
			unsigned int img_b = (unsigned int) (256 * pixels[x][y].b());

			if(img_r > 255) img_r = 255;
			if(img_g > 255) img_g = 255;
			if(img_b > 255) img_b = 255;

			unsigned char file_r = (unsigned char) (img_r);
			unsigned char file_g = (unsigned char) (img_g);
			unsigned char file_b = (unsigned char) (img_b);

			os.put(file_r);
			os.put(file_g);
			os.put(file_b);
		}
	}

    /*
	for(int y = height-1; y >= 0; y--) {
		for(int x = 0; x < width; x++) {
			unsigned int img_r = static_cast<unsigned int> (256 * cpixels[x][y].comp[type].r());
			unsigned int img_g = static_cast<unsigned int> (256 * cpixels[x][y].comp[type].g());
			unsigned int img_b = static_cast<unsigned int> (256 * cpixels[x][y].comp[type].b());

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
	*/
}
