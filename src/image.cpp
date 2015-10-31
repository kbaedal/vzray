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

bool Image::get(int a_width, int a_height, RGB &a_color)
{
	if(a_width < 0 || a_width > width) return false;
	if(a_height < 0 || a_height > height) return false;

	a_color.set(pixels[a_width][a_height]);

	return true;
}

void Image::gamma_correct(float gamma)
{
	RGB temp;
	float power = 1.0 / gamma;

	for(int x = 0; x < width; x++) {
		for(int y = 0; y < height; y++) {
			temp.set(pixels[x][y]);

			temp.clamp(0.0, 1.0);

			pixels[x][y].R = pow(temp.r(), power);
			pixels[x][y].G = pow(temp.g(), power);
			pixels[x][y].B = pow(temp.b(), power);
		}
	}
}

void Image::save_ppm(std::ostream &os)
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
}
