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
		RGB() { R = G = B = 0.0f; }
		RGB(double c) { set(c, c, c); }
		RGB(double a_r, double a_g, double a_b)
		{
			set(a_r, a_g, a_b);
		}
		RGB(double *c)
		{
		    set(c[0], c[1], c[2]);
		}

		// Operadores unarios
		const RGB &operator+() const { return *this; }
        RGB operator-() const { return RGB(-B, -G, -B);}
		RGB &operator+=(RGB const &b)
		{
            R += b.R;
            G += b.G;
            B += b.B;

            return *this;
        }

        // Operadores binarios
		friend RGB operator+(RGB a, RGB const &b)
		{
		    a += b;

		    return a;
		}

		friend RGB operator-(RGB a, RGB const &b)
		{
		    a += -b;

		    return a;
        }

		friend RGB operator*(RGB a, const RGB &b) { return RGB(a.R * b.R, a.G * b.G, a.B * b.B); }
		friend RGB operator/(RGB a, const RGB &b) { return RGB(a.R / b.R, a.G / b.G, a.B / b.B); }
		friend RGB operator*(RGB a, double f) { return RGB(a.R * f, a.G * f, a.B * f); }
		friend RGB operator*(double f, RGB a) { return RGB(a.R * f, a.G * f, a.B * f); }
		friend RGB operator/(RGB a, double f) { return RGB(a.R / f, a.G / f, a.B / f); }

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
		 * @param min_val Valor mínimo.
		 * @param max_val Valor máximo.
		 */
		void clamp(double min_val, double max_val);

		double r() const { return R; }
		double g() const { return G; }
		double b() const { return B; }

		// Manejo del operador << para ostream
		friend std::ostream& operator<<(std::ostream &os, const RGB &c);

		// Componentes
		double  R,
                G,
                B;
};

class Contrib {
    public:
        const Contrib &operator+() const { return *this; }
        Contrib operator-() const
        {
            Contrib c;

            for(int i = 0; i < 4; ++i)
                c[i] = -comp[i];

            return c;
        }

        Contrib &operator+=(const Contrib &c)
        {
            for(int i = 0; i < 4; ++i)
                comp[i] += c[i];

            return *this;
        }

        friend Contrib operator+(Contrib c1, const Contrib &c2)
        {
            c1 += c2;
            return c1;
        }

        friend Contrib operator*(Contrib c, double f)
        {
            for(int i = 0; i < 4; ++i)
                c[i] += c[i] * f;

            return c;
        }

        friend Contrib operator*(double f, Contrib c)
        {
            return c * f;
        }

        friend Contrib operator-(Contrib c1, const Contrib &c2)
        {
            c1 += -c2;
            return c1;
        }

        friend Contrib operator/(Contrib c, double f)
        {
            for(int i = 0; i < 4; ++i)
                c[i] += c[i] * (1.0f / f);

            return c;
        }

        friend Contrib operator*(Contrib c1, const Contrib &c2)
        {
            for(int i = 0; i < 4; ++i)
                c1[i] = c1[i] * c2[i];

            return c1;
        }

        void set(const Contrib &c)
        {
            for(int i = 0; i < 4; ++i)
                comp[i] = c[i];
        }

        RGB &operator[](int i) { return comp[i]; }
        const RGB &operator[](int i) const { return comp[i]; }

        RGB comp[4];  // 0 final image (1+2+3)
                      // 1 direct contribution
                      // 2 pathtracing contribution
                      // 3 caustics contribution
};

#endif
