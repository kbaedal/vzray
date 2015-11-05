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
		RGB(double a_r, double a_g, double a_b)
		{
			this->set(a_r, a_g, a_b);
		}

		// Operadores
		RGB operator+(RGB const &b) { return RGB(R + b.R, G + b.G, B + b.B); }
		RGB operator-(RGB const &b) { return RGB(R - b.R, G - b.G, B - b.B); }
		RGB operator*(RGB const &b) { return RGB(R * b.R, G * b.G, B * b.B); }
		RGB operator/(RGB const &b) { return RGB(R / b.R, G / b.G, B / b.B); }
		RGB operator*(double f) { return RGB(R * f, G * f, B * f); }
		RGB operator/(double f) { return RGB(R / f, G / f, B / f); }

		// Otras operaciones
		void set(double a_r, double a_g, double a_b)
		{
			R = a_r;
			G = a_g;
			B = a_b;
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
		void clamp(double min_val, double max_val);

		double r() const { return R; }
		double g() const { return G; }
		double b() const { return B; }

		// Manejo del operador << para ostream
		friend std::ostream& operator<<(std::ostream &os, const RGB &c);

		// Componentes
		double R;
		double G;
		double B;
};

#endif
