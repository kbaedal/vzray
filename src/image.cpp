#include <cmath>
#include "image.h"

Image::Image()
{}

Image::Image(int a_nWidth, int a_nHeight)
{
	m_nWidth = a_nWidth;
	m_nHeight = a_nHeight;
	
	m_rgbImage = new RGB*[m_nWidth];
	
	for(int i = 0; i < m_nWidth; i++)
		m_rgbImage[i] = new RGB[m_nHeight];
}

Image::Image(int a_nWidth, int a_nHeight, const RGB &a_rgbColor)
{
	m_nWidth = a_nWidth;
	m_nHeight = a_nHeight;
	
	m_rgbImage = new RGB*[m_nWidth];
	
	for(int i = 0; i < m_nWidth; i++) {
		m_rgbImage[i] = new RGB[m_nHeight];
		
		for(int j = 0; j < m_nHeight; j++) {
			m_rgbImage[i][j].set(a_rgbColor);
		}
	}
}

bool Image::set(int a_nWidth, int a_nHeight, const RGB &a_rgbColor)
{
	if(a_nWidth < 0 || a_nWidth > m_nWidth) return false;
	if(a_nHeight < 0 || a_nHeight > m_nHeight) return false;
	
	m_rgbImage[a_nWidth][a_nHeight].set(a_rgbColor);
	
	return true;
}

bool Image::get(int a_nWidth, int a_nHeight, RGB &a_rgbColor)
{
	if(a_nWidth < 0 || a_nWidth > m_nWidth) return false;
	if(a_nHeight < 0 || a_nHeight > m_nHeight) return false;
	
	a_rgbColor.set(m_rgbImage[a_nWidth][a_nHeight]);
	
	return true;	
}

void Image::gammaCorrect(float dGamma)
{
	RGB rgbTemp;
	float fPower = 1.0 / dGamma;
	
	for(int x = 0; x < m_nWidth; x++) {
		for(int y = 0; y < m_nHeight; y++) {
			rgbTemp.set(m_rgbImage[x][y]);
			
			rgbTemp.clamp(0.0, 1.0);
						
			m_rgbImage[x][y].R = pow(rgbTemp.r(), fPower);
			m_rgbImage[x][y].G = pow(rgbTemp.g(), fPower);
			m_rgbImage[x][y].B = pow(rgbTemp.b(), fPower);
		}
	}
}

void Image::savePPM(std::ostream &os)
{
	// write header
	os << "P6\n" << m_nWidth << ' ' << m_nHeight << "\n255\n";
	
	unsigned int nR, nG, nB;
	unsigned char cR, cG, cB;
	
	// write clamped [0, 255] values
	for(int y = m_nHeight-1; y >= 0; y--) {
		for(int x = 0; x < m_nWidth; x++) {
			nR = (unsigned int) (256 * m_rgbImage[x][y].r());
			nG = (unsigned int) (256 * m_rgbImage[x][y].g());
			nB = (unsigned int) (256 * m_rgbImage[x][y].b());
			
			if(nR > 255) nR = 255;
			if(nG > 255) nG = 255;
			if(nB > 255) nB = 255;
			
			cR = (unsigned char) (nR);
			cG = (unsigned char) (nG);
			cB = (unsigned char) (nB);
			
			os.put(cR);
			os.put(cG);
			os.put(cB);
		}
	}
}
