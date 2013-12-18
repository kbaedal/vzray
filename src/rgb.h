#ifndef __RGB_H__
#define __RGB_H__ 1

#include <iostream>

/**
 * Clase para el manejo de colores.
 */
class RGB
{
	public:
		// Constructores
		RGB() { R = G = B = 0.f; }
		RGB(float a_R, float a_G, float a_B)
		{
			this->set(a_R, a_G, a_B);
		}
		
		// Operadores
		RGB operator+(RGB const &b) { return RGB(R + b.R, G + b.G, B + b.B); }
		RGB operator-(RGB const &b) { return RGB(R - b.R, G - b.G, B - b.B); }
		RGB operator*(RGB const &b) { return RGB(R * b.R, G * b.G, B * b.B); }
		RGB operator/(RGB const &b) { return RGB(R / b.R, G / b.G, B / b.B); }
		RGB operator*(float f) { return RGB(R * f, G * f, B * f); }
		RGB operator/(float f) { return RGB(R / f, G / f, B / f); }
		
		// Otras operaciones
		void set(float a_R, float a_G, float a_B)
		{
			R = a_R;
			G = a_G;
			B = a_B;	
		}
		void set(const RGB &rgb)
		{
			R = rgb.r();	
			G = rgb.g();
			B = rgb.b();
		}
		
		/**
		 * @brief Acota los valores entre un minimo y un maximo.
		 * @param dMin Valor mínimo.
		 * @param dMax Valor máximo.
		 */
		void clamp(float dMin, float dMax);			
				
		float r() const { return R; }
		float g() const { return G; }
		float b() const { return B; }
		
		// Manejo del operador << para ostream
		friend std::ostream& operator<<(std::ostream &os, const RGB &c);
				
		// Componentes
		float R;
		float G;
		float B;
};

#endif
