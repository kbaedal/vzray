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
#include "shapes/sphere.h"
#include "shapes/cylinder.h"
#include "shapes/box.h"
#include "shapes/triangle.h"
#include "shapes/mesh.h"
#include "shapes/meshtriangle.h"

Parser::Parser(void) : MAX_ETIQUETA(15), MAX_CONTENT(25)
{
	m_nError 	= -1; 		// No ha ocurrido ningún error aún.
	m_nLine 	= 1;		// De ocurrir, ocurre como muy pronto en la primera linea
	m_bEOF		= false;	// Mientras sea nulo, no hemos llegado al fin del fichero.

    m_strTipoError[0].assign("Etiqueta desconocida.");
    m_strTipoError[1].assign("Fin de archivo inesperado.");
    m_strTipoError[2].assign("Contenido de etiqueta desconocido.");
    m_strTipoError[3].assign("Numero (float o int) no valido.");
    m_strTipoError[4].assign("Fichero no encontrado o algo raro (Badbit).");
    m_strTipoError[5].assign("Fichero no encontrado o algo raro (Failbit).");
    m_strTipoError[6].assign("Fichero no encontrado o algo raro (Eofbit).");
    m_strTipoError[7].assign("Fichero ya abierto, so mongolo.");
    m_strTipoError[8].assign("Caracter desconocido.");
    m_strTipoError[10].assign("Fichero correcto.");
}

Parser::~Parser()
{
    m_fileEscena.close();
}

void Parser::getError(std::string &strError, int &nLine, int &nErrorCode)
{
    if(m_nError != -1)
		strError.assign(m_strTipoError[m_nError]);
    else
        strError.assign("Sin error.");

    nLine = m_nLine;
    nErrorCode = m_nError;
}

bool Parser::leerFichero(const std::string &strName, Globals *pGlobals)
{
	m_pGlobals = pGlobals; // Inicializamos el puntero para rellenar los datos.

    m_fileEscena.open(strName.c_str()); // Abrimos el fichero.

    if(!m_fileEscena.is_open()) {
        m_nError = 7;
        return false;
    }

    if(!m_fileEscena.good()) {
        if(m_fileEscena.bad())
            m_nError = 4;
        else if(m_fileEscena.fail())
            m_nError = 5;
        else m_nError = 6;

        return false;
    }

    do {
		if(!ignorarChars()) {
			if(!m_bEOF) // No hemos llegado al fin de archivo. Error.
				return false;
		}
		else {
			std::string strEtiqueta;

			if(readToken(strEtiqueta))
			{
				// Procesar las etiquetas encontradas.
				if(strEtiqueta == "config") {
					if(!processConfig())
						return false;
				}
				else if(strEtiqueta == "scene") {
					if(!processScene())
						return false;
				}
				else {
					m_nError = 0; // Etiqueta desconocida.
					return false;
				}
			}
			else {
				m_nError = 0; // Etiqueta desconocida.
				return false;
			}
		}
	}
	while(!m_bEOF);
    m_nError = 10;	// Si llegamos hasta aquí, es que el fichero se ha leido
    return true;	// bien y al completo.
}

bool Parser::ignorarChars()
{
    bool bFindChar = false;
    char cRead;

    do {
        if(m_fileEscena.get(cRead)) { // No estamos al final del archivo si pasamos de aqui.
			switch(cRead) {
				// Ignoramos saltos de linea, espacios y tabuladores.
                case '\n': m_nLine++;
                case '\r':
                case ' ':
                case '\t':
                    break;
                case '<':
					// Encontrado inicio de etiqueta. Devolvermos el control
					// si no es un comentario.
					if(isComment()) {
						if(!processComment())
							return false;
					}
					else {
						bFindChar = true;
					}
                    break;
                default: // Caracter erroneo o no esperado.
					m_nError = 8;
                    return false;
                    break;
            }
        }
        else { // Fin de archivo.
            m_nError = 1;
            m_bEOF = true;
            return false;
        }
    }
    while(!bFindChar);
    return true;
}

bool Parser::readToken(std::string &strToken)
{
    int 		nCont = 0;
    char 		cRead;
    std::string strTemp(""); // Cadena inicializada sin caracteres.

    while(nCont < MAX_ETIQUETA) {
        if(!m_fileEscena.get(cRead)) {
            m_nError = 1; // Fin del fichero, devolvemos false.
            return false;
        }
        else {
            if(cRead != '>')
				// Añadimos el caracter leido a la cadena.
                strTemp += cRead;
            else {
                // Fin de la etiqueta, copiamos lo leido a nuestra
                // cadena y devolvemos true.
                strToken.assign(strTemp);
                return true;
            }
        }
        nCont++;
    }
    // Superado el tamaño máximo de etiqueta, devolvemos false.
    m_nError = 0; // Etiqueta desconocida.
    return false;
}

bool Parser::readContent(std::string &strContent, const char cStopChar)
{
    int 		nCont = 0;
    char 		cRead;
    std::string	strTemp("");

    while(nCont < MAX_CONTENT) {
        if(!m_fileEscena.get(cRead)) {
            m_nError = 1; // Fin del fichero, devolvemos false.
            return false;
        }
        else {
            if(cRead != cStopChar)
                strTemp += cRead;
            else {
                // Fin del contenido, copiamos los datos y volvemos.
                strContent.assign(strTemp);
                return true;
            }
        }
        nCont++;
    }
    m_nError = 2; // Superado el tamaño máximo de contenido, devolvemos false.
    return false;
}

bool Parser::readFloat(float &dDato, const char cStopChar)
{
	std::string strTemp("");

	// Leemos el contenido en una cadena.
	readContent(strTemp, cStopChar);

	// Comprobamos que la cadena contenga un numero válido para pasarselo
	// a atof() con total seguridad.
	if(validFloat(strTemp)) {
		dDato = atof(strTemp.c_str());
		return true;
	}
	else {
		m_nError = 3;
		return false;
	}
}

bool Parser::readInt(int &nDato, const char cStopChar)
{
	std::string strTemp("");

	// Leemos el contenido en una cadena.
	readContent(strTemp, cStopChar);

	// Comprobamos que la cadena contenga un numero válido para pasarselo
	// a atoi() con total seguridad.
	if(validInt(strTemp)) {
		nDato = atoi(strTemp.c_str());
		return true;
	}
	else {
		m_nError = 3;
		return false;
	}
}

bool Parser::readBloqueTxt(const std::string &strEtiqueta, std::string &strResultado)
{
    std::string strBufferEtq,
				strBufferCnt,
				strCierre("/");

	// Ignoramos hasta llegar a la apertura de una etiqueta.
    if(!ignorarChars())
        return false;

    // Leemos que etiqueta es.
    if(!readToken(strBufferEtq))
        return false;

    // Si no es la etiqueta que buscamos, devolvemos error.
    if(strEtiqueta != strBufferEtq)
        return false;

    // Leemos el contenido.
    if(!readContent(strBufferCnt, '<'))
        return false;

    // Antes de leer el cierre de la etiqueta, componemos la cadena para
    // comparar con dicho cierre. El cierre de la etiqueta debe ser igual
    // que la apertura pero con una barra delante:
    //	- Si apertura es <etiqueta>, el cierre debe ser </etiqueta>
    strCierre += strBufferEtq;
    strBufferEtq.clear();

    // Leemos el cierre de la etiqueta.
    if(!readToken(strBufferEtq))
        return false;

    // Y comprobamos que sea el cierre correcto.
    if(strCierre != strBufferEtq)
        return false;

	// Copiamos el resultado para devolverlo.
    strResultado = strBufferCnt;
    return true;
}

// Leeremos, dentro de la etiqueta strEtiqueta, nNumF floats, que
// almacenaremos en el array de floats fltArray. Al llamar a este metodo
// debemos garantizar que fltArray puede almacenar nNumF floats.
bool Parser::readBloqueFloats(const std::string &strEtiqueta, const int nNumF, float *fltArray)
{
	std::string	strBufferEtq,
				strCierre("/");
	char		cSep;

	if(!ignorarChars())
        return false;
    if(!readToken(strBufferEtq))
        return false;
	if(strEtiqueta != strBufferEtq)
        return false;

	for(int i = 0; i < nNumF; i++) {
		if(i == nNumF - 1) // Ultimo float a leer
			cSep = '<';
		else
			cSep = ',';

		if(!readFloat(fltArray[i], cSep))
			return false;
	}

	strCierre += strBufferEtq;
    strBufferEtq.clear();

    if(!readToken(strBufferEtq))
        return false;

    if(strCierre != strBufferEtq)
        return false;

    return true;
}

bool Parser::readBloqueInts(const std::string &strEtiqueta, const int nNumF, int *intArray)
{
	std::string	strBufferEtq,
				strCierre("/");
	char		cSep;

	if(!ignorarChars())
        return false;
    if(!readToken(strBufferEtq))
        return false;
	if(strEtiqueta != strBufferEtq)
        return false;

	for(int i = 0; i < nNumF; i++) {
		if(i == nNumF - 1) // Ultimo float a leer
			cSep = '<';
		else
			cSep = ',';

		if(!readInt(intArray[i], cSep))
			return false;

	}

	strCierre += strBufferEtq;
    strBufferEtq.clear();

    if(!readToken(strBufferEtq))
        return false;

    if(strCierre != strBufferEtq)
        return false;

    return true;
}

bool Parser::readBloqueVec3(const std::string &strEtiqueta, Vec3 &v3Resultado)
{
	float vArray[3];

	if(!readBloqueFloats(strEtiqueta, 3, vArray))
		return false;

	v3Resultado.set(vArray[0], vArray[1], vArray[2]);

	return true;
}

bool Parser::readBloquePoint(const std::string &strEtiqueta, Point &p3Resultado)
{
	float vArray[3];

	if(!readBloqueFloats(strEtiqueta, 3, vArray))
		return false;

	p3Resultado.set(vArray[0], vArray[1], vArray[2]);

	return true;
}

bool Parser::validFloat(const std::string &strFloat)
{
	// Para que una cadena contenga un float válido deben darse las
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

	unsigned int    nCont = 0;
    bool	        bEnNumero 	= false,
                    bFinNumero 	= false,
                    bPunto 		= false,
                    bSignoMan	= false,
                    bSignoExp	= false,
                    bEnExp		= false;

	while(nCont < strFloat.length()) {
		switch(strFloat[nCont]) {
			case ' ':
				if(bEnNumero)
					// Permitimos los espacios al principio y al final
					// de la cadena.
					bFinNumero = true;
				break;
			case '.':
				if(!bPunto && !bFinNumero) {
					// Hemos encontrado el punto decimal.
					bPunto = true;
					// Y puede que no haya cifra por delante, pero aún
					// así es un numero válido: .56, por ejemplo.
					bEnNumero = true;
				}
				else
					// Hemos encontrado otro punto. Error.
					return false;
				break;
			case '+':
			case '-':
				if(!bEnNumero && !bSignoMan && !bFinNumero) {
					// Encontramos el signo de la mantisa, antes
					// de cualquier cifra.
					bSignoMan = true;
					// Y ya tenemos que procesar el número.
					bEnNumero = true;
				}
				else if(bEnNumero && bEnExp && !bSignoExp) {
					// Encontramos el signo del exponente, que debe venir
					// detrás de la 'e' que lo indica.
					bSignoExp = true;
				}
				else
					// Hemos encontrado un signo en un lugar en el que
					// no debería estar.
					return false;
				break;
			case 'e':
			case 'E':
				if(bEnNumero && !bEnExp && !bFinNumero)
					// Hemos encontrado la 'e' que indica el inicio
					// del exponente.
					bEnExp = true;
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
				if(!bEnNumero && !bFinNumero)
					// Una cifra. Encontrada por primera vez nos marcará
					// el inicio del número, a partir de aquí entran en
					// juego las demás reglas.
					bEnNumero = true;
				else if(bFinNumero)
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
		nCont++;
	}
	// Si hemos llegado hasta aquí, quiere decir que la cadena leída
	// tiene el formato adecuado para ser convertida a float.
	return true;
}

bool Parser::validInt(const std::string &strInt)
{
	// Para que una cadena contenga un int válido deben darse las
	// siguientes condiciones:
	//	- Puede haber espacios en blanco antes del primer caracter.
	//	- También puede haber espacios en blanco tras el último.
	//	- El primer caracter debe ser el signo, si lo hay, (+ o -).
	//	- Tras el signo solo puede haber números, nunca otro signo.

	unsigned int 	nCont = 0;
    bool	        bEnNumero 	= false,
                    bFinNumero 	= false;

	while(nCont < strInt.length())
    {
		switch(strInt[nCont]) {
			case ' ':
				if(bEnNumero)
					// Permitimos los espacios al principio y al final
					// de la cadena.
					bFinNumero = true;
				break;
			case '+':
			case '-':
				if(!bEnNumero)
					// Encontramos el signo antes de cualquier cifra.
					// Ya tenemos que procesar el número.
					bEnNumero = true;
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
				if(!bEnNumero && !bFinNumero)
					// Una cifra. Encontrada por primera vez nos marcará
					// el inicio del número, a partir de aquí entran en
					// juego las demás reglas.
					bEnNumero = true;
				else if(bFinNumero)
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
		nCont++;
	}
	// Si hemos llegado hasta aquí, quiere decir que la cadena leída
	// tiene el formato adecuado para ser convertida a int.
	return true;
}

bool Parser::isComment()
{
	char cComment;

	if(m_fileEscena.get(cComment)) {
		// Retrocedemos la posicion del fichero, para que las proximas
		// funciones puedan procesar correctamentela etiqueta encontrada,
		// sea esta un comentario o no.
		m_fileEscena.unget();

		if(cComment == '!') // Tenemos un comentario.
			return true;
		else // No es un comentario.
			return false;
	}
	else {
		m_nError = 1; // Fin de fichero inesperado.
		return false;
	}
}

bool Parser::processComment()
{
	bool 		bComment = false;
	char 		cBuffer;
	std::string	strTemp("<"); // Este caracter ya está leído.

	// Recogemos los tres caracteres siguientes del archivo para comprobar
	// que tenemos la apertura del comentario, que está compuesta por
	// la cadena "<!--".
	for(int i = 0; i < 3; i++)
		if(m_fileEscena.get(cBuffer))
			strTemp += cBuffer;
		else {
			m_nError = 1; // Fin de fichero inesperado.
			return false;
		}

	if(strTemp != "<!--") // Si no coincide, devolvemos false.
		return false;

	// Ahora buscaremos el primer signo '>', para comprobar que es parte
	// de la etiqueta de cierre: "-->"
	do {
		if(m_fileEscena.get(cBuffer)) {
			switch(cBuffer) {
				case '>': // Encontrado un '>'
					// Encontrado un '>', retrocedemos tres posiciones nuestra
					// posicion en el archivo y leemos tres caracteres.
					for(int i = 0; i < 3; i++, m_fileEscena.unget());

					strTemp.clear();

					for(int i = 0; i < 3; i++) {
						if(m_fileEscena.get(cBuffer))
							strTemp += cBuffer;
						else {
							m_nError = 1; // Fin de fichero inesperado.
							return false;
						}
					}

					if(strTemp == "-->") // Fin de comentario.
						bComment = true;
					break;
				case '\n': m_nLine++;
					break;
				default:
					break;
			}
		}
		else {
			m_nError = 1; // Fin de fichero inesperado.
			return false;
		}
	}
	while(!bComment);

	// Si llegamos aquí es que el comentario está correctamente construido.
	return true;
}

bool Parser::processConfig()
{
	std::string strEtiqueta;
    bool 		bFinBloque = false;

    while(!bFinBloque) {
		// Buscamos etiqueta.
		if(!ignorarChars())
			return false;

        if(!readToken(strEtiqueta))
            return false;

        if(strEtiqueta == "/config")
			bFinBloque = true;
        else
        {
            if(strEtiqueta == "renderer") {
				if(!processRenderer())
					return false;
            }
            else if(strEtiqueta == "camera") {
				if(!processCamera())
					return false;
            }
            else if(strEtiqueta == "image") {
				if(!processImage())
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

bool Parser::processRenderer()
{
	std::string strType, // Tipo de renderer
				strTemp; // Auxiliar para comprobaciones.
	int			nNumSamples, nMaxDepth;

	if(!readBloqueTxt("type", strType))
		return false;

	if(!readBloqueInts("samples", 1, &nNumSamples))
		return false;

	if(!readBloqueInts("max_depth", 1, &nMaxDepth))
		return false;

	if(nNumSamples > 0) // Otherwise, use default.
		m_pGlobals->nSamplesPerPixel = nNumSamples;

	if(nMaxDepth > 0) // Otherwise, use default.
		m_pGlobals->nMaxDepth = nMaxDepth;

	if(!ignorarChars())
		return false;

	if(!readToken(strTemp))
		return false;

	if(strTemp != "/renderer")
		return false;

	if(strType == "whitted") {
		if(m_pGlobals->pRenderer == NULL)
			m_pGlobals->pRenderer = new DirectRenderer(m_pGlobals->nMaxDepth);
		else // Renderer ya creado, error.
			return false;
	}
	else if(strType == "path") {
		if(m_pGlobals->pRenderer == NULL)
			m_pGlobals->pRenderer = new PathRenderer(m_pGlobals->nMaxDepth, time(NULL));
		else
			return false;
	}
	else // Renderer desconocido, error.
		return false;

	return true;
}

bool Parser::processCamera()
{
	std::string strType, // Tipo de camera
				strTemp; // Auxiliar para comprobaciones.
	Point		p3Pos;
	Vec3		v3Gaze, v3Up;
	float		dDist, dLowX, dLowY, dHighX, dHighY, dPush;

	float		fltArray[2];

	if(!readBloqueTxt("type", strType))
		return false;

	if(!readBloquePoint("pos", p3Pos))
		return false;

	if(!readBloqueVec3("gaze", v3Gaze))
		return false;

	if(!readBloqueVec3("up", v3Up))
		return false;

	if(!readBloqueFloats("dist", 1, &dDist))
		return false;

	if(!readBloqueFloats("low_corner", 2, fltArray))
		return false;

	dLowX = fltArray[0];
	dLowY = fltArray[1];

	if(!readBloqueFloats("high_corner", 2, fltArray))
		return false;

	dHighX = fltArray[0];
	dHighY = fltArray[1];

	if(!readBloqueFloats("push", 1, &dPush))
		return false;

	if(!ignorarChars())
		return false;

	if(!readToken(strTemp))
		return false;

	if(strTemp != "/camera")
		return false;

	if(strType == "pinhole") {
		if(m_pGlobals->pCamera == NULL)
			m_pGlobals->pCamera = new Pinhole(p3Pos, v3Gaze, v3Up, dDist,
				dLowX, dLowY, dHighX, dHighY, dPush);
		else // Camera ya creada, error.
			return false;
	}
	else
		// Tipo de camara desconocida.
		return false;

	return true;
}

bool Parser::processImage()
{
	std::string strTemp;
	int			nResX, nResY;
	float		dBGColor[3];

	if(!readBloqueInts("res_x", 1, &nResX))
		return false;

	if(!readBloqueInts("res_y", 1, &nResY))
		return false;

	if(!readBloqueFloats("bgcolor", 3, dBGColor))
		return false;

	if(nResX > 0) // Otherwise, use default.
		m_pGlobals->nResX = nResX;

	if(nResY > 0) // Otherwise, use default.
		m_pGlobals->nResY = nResY;

	if(!ignorarChars())
		return false;

	if(!readToken(strTemp))
		return false;

	if(strTemp != "/image")
		return false;

	if(m_pGlobals->pImage == NULL)
		m_pGlobals->pImage = new Image(nResX, nResY,
			RGB(dBGColor[0], dBGColor[1], dBGColor[2]));
	else // Image ya creada, error.
		return false;

	return true;
}

bool Parser::processScene()
{
	std::string strEtiqueta;
    bool 		bFinBloque = false;

	if(m_pGlobals->pScene == NULL) {
		m_pGlobals->pScene = new Scene;

		while(!bFinBloque) {
			// Buscamos etiqueta.
			if(!ignorarChars())
				return false;

			if(!readToken(strEtiqueta))
				return false;

			if(strEtiqueta == "/scene")
				bFinBloque = true;
			else
			{
				if(strEtiqueta == "texture") {
					if(!processTexture())
						return false;
				}
				else if(strEtiqueta == "material") {
					if(!processMaterial())
						return false;
				}
				else if(strEtiqueta == "object") {
					if(!processObject())
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

bool Parser::processTexture()
{
	std::string strType;

	if(!readBloqueTxt("type", strType))
		return false;

	if(strType == "simple") {
		if(!processSimpleTex())
			return false;
	}
	else // Textura desconocida, error.
		return false;

	return true;
}

bool Parser::processSimpleTex()
{
	std::string	strID, strTemp;
	Vec3		v3Color;
	Texture		*pTexture;

	if(!readBloqueTxt("id", strID))
		return false;

	if(!readBloqueVec3("color", v3Color))
		return false;

	if(!ignorarChars())
		return false;

	if(!readToken(strTemp))
		return false;

	if(strTemp != "/texture")
		return false;

	pTexture = new SimpleTexture(RGB(v3Color.x(), v3Color.y(), v3Color.z()));

	if(m_pGlobals->pScene->addTexture(pTexture, strID))
		return true;
	else // Ya existe una textura que ese ID, error.
		return false;
}

bool Parser::processMaterial()
{
	std::string strType;

	if(!readBloqueTxt("type", strType))
		return false;

	if(strType == "diffuse") {
		if(!processDiffuseMat())
			return false;
	}
	else if(strType == "specular") {
		if(!processSpecularMat())
			return false;
	}
	else if(strType == "dielectric") {
		if(!processDielectricMat())
			return false;
	}
	else if(strType == "light") {
		if(!processLightMat())
			return false;
	}
	else // Material desconocido, error.
		return false;

	return true;
}

bool Parser::processDiffuseMat()
{
	std::string strID, strTextureID, strTemp;
	Material 	*pMaterial;
	Texture		*pTexture;

	if(!readBloqueTxt("id", strID))
		return false;

	if(!readBloqueTxt("texture_id", strTextureID))
		return false;

	if(!ignorarChars())
		return false;

	if(!readToken(strTemp))
		return false;

	if(strTemp != "/material")
		return false;

	if((pTexture = m_pGlobals->pScene->getTexture(strTextureID)) != NULL) {
		pMaterial = new DiffuseMaterial(m_pGlobals->pScene->getTexture(strTextureID));
		if(!m_pGlobals->pScene->addMaterial(pMaterial, strTextureID, strID))
			return false; // Existe un material con el mismo ID.
	}
	else // La textura asociada al material no existe.
		return false;

	return true;
}

bool Parser::processSpecularMat()
{
	std::string strID, strTextureID, strTemp;
	Material 	*pMaterial;
	Texture		*pTexture;

	if(!readBloqueTxt("id", strID))
		return false;

	if(!readBloqueTxt("texture_id", strTextureID))
		return false;

	if(!ignorarChars())
		return false;

	if(!readToken(strTemp))
		return false;

	if(strTemp != "/material")
		return false;

	if((pTexture = m_pGlobals->pScene->getTexture(strTextureID)) != NULL) {
		pMaterial = new SpecularMaterial(m_pGlobals->pScene->getTexture(strTextureID));
		if(!m_pGlobals->pScene->addMaterial(pMaterial, strTextureID, strID))
			return false; // Existe un material con el mismo ID.
	}
	else // La textura asociada al material no existe.
		return false;

	return true;
}

bool Parser::processDielectricMat()
{
	std::string strID, strTextureID, strTemp;
	float		dIOR;
	Material 	*pMaterial;
	Texture		*pTexture;

	if(!readBloqueTxt("id", strID))
		return false;

	if(!readBloqueTxt("texture_id", strTextureID))
		return false;

	if(!readBloqueFloats("ior", 1, &dIOR))
		return false;

	if(!ignorarChars())
		return false;

	if(!readToken(strTemp))
		return false;

	if(strTemp != "/material")
		return false;

	if((pTexture = m_pGlobals->pScene->getTexture(strTextureID)) != NULL) {
		pMaterial = new DielectricMaterial(m_pGlobals->pScene->getTexture(strTextureID), dIOR);
		if(!m_pGlobals->pScene->addMaterial(pMaterial, strTextureID, strID))
			return false; // Existe un material con el mismo ID.
	}
	else // La textura asociada al material no existe.
		return false;

	return true;
}

bool Parser::processLightMat()
{
	std::string strID, strTextureID, strTemp;
	Material 	*pMaterial;
	Texture		*pTexture;

	if(!readBloqueTxt("id", strID))
		return false;

	if(!readBloqueTxt("texture_id", strTextureID))
		return false;

	if(!ignorarChars())
		return false;

	if(!readToken(strTemp))
		return false;

	if(strTemp != "/material")
		return false;

	if((pTexture = m_pGlobals->pScene->getTexture(strTextureID)) != NULL) {
		pMaterial = new LightMaterial(m_pGlobals->pScene->getTexture(strTextureID));
		if(!m_pGlobals->pScene->addMaterial(pMaterial, strTextureID, strID))
			return false; // Existe un material con el mismo ID.
	}
	else // La textura asociada al material no existe.
		return false;

	return true;
}

bool Parser::processObject()
{
	std::string strType;

	if(!readBloqueTxt("type", strType))
		return false;

	if(strType == "plane") {
		if(!processPlane())
			return false;
	}
	else if(strType == "sphere") {
		if(!processSphere())
			return false;
	}
	else if(strType == "cylinder") {
		if(!processCylinder())
			return false;
	}
	else if(strType == "box") {
		if(!processBox())
			return false;
	}
	else if(strType == "parallelogram") {
		if(!processParallelogram())
			return false;
	}
	else if(strType == "triangle") {
		if(!processTriangle())
			return false;
	}
	else if(strType == "mesh") {
		if(!processMesh())
			return false;
	}
	else
		// Tipo de objeto desconocido.
		return false;

	return true;
}

bool Parser::processPlane()
{
	std::string strMaterial, strTemp;
	Vec3		v3Normal;
	float		dDist;
	Material	*pMaterial;
	Shape		*pShape;
	Transform	tTrans;

	if(!readBloqueVec3("normal", v3Normal))
		return false;

	if(!readBloqueFloats("distance", 1, &dDist))
		return false;

	if(!readBloqueTxt("material", strMaterial))
		return false;

	if(!ignorarChars())
		return false;

	if(!readToken(strTemp))
		return false;

	if(strTemp == "transform") {
		if(!processTransform(&tTrans))
			return false;

		if(!ignorarChars())
			return false;

		if(!readToken(strTemp))
			return false;
	}

	if(strTemp != "/object")
		return false;

	if((pMaterial = m_pGlobals->pScene->getMaterial(strMaterial)) != NULL) {
		pShape = new Plane(v3Normal, dDist, pMaterial);
		pShape->setTrans(&tTrans);
		m_pGlobals->pScene->addObject(pShape, strMaterial, pMaterial->isLight());
	}
	else // Material asociado al objeto desconocido.
		return false;

	return true;
}

bool Parser::processSphere()
{
	std::string strMaterial, strTemp;
	Point		p3Center;
	float		dRadius;
	Material	*pMaterial;
	Shape		*pShape;
	Transform	tTrans;

	if(!readBloquePoint("center", p3Center))
		return false;

	if(!readBloqueFloats("radius", 1, &dRadius))
		return false;

	if(!readBloqueTxt("material", strMaterial))
		return false;

	if(!ignorarChars())
		return false;

	if(!readToken(strTemp))
		return false;

	if(strTemp == "transform") {
		if(!processTransform(&tTrans))
			return false;

		if(!ignorarChars())
			return false;

		if(!readToken(strTemp))
			return false;
	}

	if(strTemp != "/object")
		return false;

	if((pMaterial = m_pGlobals->pScene->getMaterial(strMaterial)) != NULL) {
		pShape = new Sphere(p3Center, dRadius, pMaterial);
		pShape->setTrans(&tTrans);
		m_pGlobals->pScene->addObject(pShape, strMaterial, pMaterial->isLight());
	}
	else // Material asociado al objeto desconocido.
		return false;

	return true;
}

bool Parser::processCylinder()
{
	std::string strMaterial, strTemp;
	Point		p3P0, p3P1;
	float		dRadius;
	Material	*pMaterial;
	Shape		*pShape;
	Transform	tTrans;

	if(!readBloquePoint("center_0", p3P0))
		return false;

	if(!readBloquePoint("center_1", p3P1))
		return false;

	if(!readBloqueFloats("radius", 1, &dRadius))
		return false;

	if(!readBloqueTxt("material", strMaterial))
		return false;

	if(!ignorarChars())
		return false;

	if(!readToken(strTemp))
		return false;

	if(strTemp == "transform") {
		if(!processTransform(&tTrans))
			return false;

		if(!ignorarChars())
			return false;

		if(!readToken(strTemp))
			return false;
	}

	if(strTemp != "/object")
		return false;

	if((pMaterial = m_pGlobals->pScene->getMaterial(strMaterial)) != NULL) {
		pShape = new Cylinder(p3P0, p3P1, dRadius, pMaterial);
		pShape->setTrans(&tTrans);
		m_pGlobals->pScene->addObject(pShape, strMaterial, pMaterial->isLight());
	}
	else // Material asociado al objeto desconocido.
		return false;

	return true;
}

bool Parser::processBox()
{
	std::string strMaterial, strTemp;
	Point		p3Min, p3Max;
	Material	*pMaterial;
	Shape		*pShape;
	Transform	tTrans;

	if(!readBloquePoint("min", p3Min))
		return false;

	if(!readBloquePoint("max", p3Max))
		return false;

	if(!readBloqueTxt("material", strMaterial))
		return false;

	if(!ignorarChars())
		return false;

	if(!readToken(strTemp))
		return false;

	if(strTemp == "transform") {
		if(!processTransform(&tTrans))
			return false;

		if(!ignorarChars())
			return false;

		if(!readToken(strTemp))
			return false;
	}

	if(strTemp != "/object")
		return false;

	if((pMaterial = m_pGlobals->pScene->getMaterial(strMaterial)) != NULL) {
		pShape = new Box(p3Min, p3Max, pMaterial);
		pShape->setTrans(&tTrans);
		m_pGlobals->pScene->addObject(pShape, strMaterial, pMaterial->isLight());
	}
	else // Material asociado al objeto desconocido.
		return false;

	return true;
}

bool Parser::processParallelogram()
{
	std::string strMaterial, strTemp;
	Point		p3Base;
	Vec3		v3VecU,
				v3VecV;
	Material	*pMaterial;
	Shape		*pShape;
	Transform	tTrans;

	if(!readBloquePoint("base", p3Base))
		return false;

	if(!readBloqueVec3("vec_u", v3VecU))
		return false;

	if(!readBloqueVec3("vec_v", v3VecV))
		return false;

	if(!readBloqueTxt("material", strMaterial))
		return false;

	if(!ignorarChars())
		return false;

	if(!readToken(strTemp))
		return false;

	if(strTemp == "transform") {
		if(!processTransform(&tTrans))
			return false;

		if(!ignorarChars())
			return false;

		if(!readToken(strTemp))
			return false;
	}

	if(strTemp != "/object")
		return false;

	if((pMaterial = m_pGlobals->pScene->getMaterial(strMaterial)) != NULL) {
		pShape = new Parallelogram(p3Base, v3VecU, v3VecV, pMaterial);
		pShape->setTrans(&tTrans);
		m_pGlobals->pScene->addObject(pShape, strMaterial, pMaterial->isLight());
	}
	else // Material asociado al objeto desconocido.
		return false;

	return true;
}

bool Parser::processTriangle()
{
	std::string strMaterial, strTemp;
	Point		p3P0,
				p3P1,
				p3P2;
	Material	*pMaterial;
	Shape		*pShape;
	Transform	tTrans;

	if(!readBloquePoint("vertex", p3P0))
		return false;

	if(!readBloquePoint("vertex", p3P1))
		return false;

	if(!readBloquePoint("vertex", p3P2))
		return false;

	if(!readBloqueTxt("material", strMaterial))
		return false;

	if(!ignorarChars())
		return false;

	if(!readToken(strTemp))
		return false;

	if(strTemp == "transform") {
		if(!processTransform(&tTrans))
			return false;

		if(!ignorarChars())
			return false;

		if(!readToken(strTemp))
			return false;
	}

	if(strTemp != "/object")
		return false;

	if((pMaterial = m_pGlobals->pScene->getMaterial(strMaterial)) != NULL) {
		pShape = new Triangle(p3P0, p3P1, p3P2, pMaterial);
		pShape->setTrans(&tTrans);
		m_pGlobals->pScene->addObject(pShape, strMaterial, pMaterial->isLight());
	}
	else // Material asociado al objeto desconocido.
		return false;

	return true;
}

bool Parser::processMesh()
{
	std::string strMaterial, strTemp;
	Material	*pMaterial;
	Shape		*pShape;
	Transform	tTrans;

	// Lista de punteros a MeshTriangle a añadir a la escena, que será
	// construida durante la lectura de la lista de vértices o del
	// fichero .obj que se nos proporcione. Una vez construida, los
	// añadiremos a la escena.
	std::vector<MeshTriangle *> MTList;

	if(!ignorarChars())
			return false;

	if(!readToken(strTemp))
		return false;

	if(strTemp == "vertex_list") {
        pShape = new Mesh();
		if(!processMeshVertex(pShape, &MTList)) {
            delete pShape;
            return false;
		}

	}
	else if(strTemp == "file") {
		pShape = new Mesh();
		if(!processMeshFile(pShape, &MTList)) {
            delete pShape;
            return false;
		}
	}
	else // Etiqueta desconocida, descripción errónea de la malla
		return false;

	if(!readBloqueTxt("material", strMaterial))
		return false;

	if(!ignorarChars())
		return false;

	if(!readToken(strTemp))
		return false;

	if(strTemp == "transform") {
		if(!processTransform(&tTrans))
			return false;

		if(!ignorarChars())
			return false;

		if(!readToken(strTemp))
			return false;
	}

	if(strTemp != "/object")
		return false;

	if((pMaterial = m_pGlobals->pScene->getMaterial(strMaterial)) != NULL) {
		pShape->setMaterial(pMaterial);
		pShape->setTrans(&tTrans);
		m_pGlobals->pScene->addObject(pShape, strMaterial, pMaterial->isLight());

		// Añadimos ahora a la escena todos los triángulos que componen la malla
		for(int i = 0; i < (int) MTList.size(); i++)
			m_pGlobals->pScene->addObject((Shape *) MTList[i], strMaterial, pMaterial->isLight());
	}
	else // Material asociado al objeto desconocido.
		return false;

	return true;
}

bool Parser::processMeshVertex(Shape *pShape, std::vector<MeshTriangle *> *MTList)
{
	std::string 	strEtiqueta;
	Point			p0, p1, p2;
	Mesh			*pMesh = dynamic_cast<Mesh *> (pShape);

	while(1) { // Always true
		if(!ignorarChars())
				return false;

		if(!readToken(strEtiqueta))
			return false;

		if(strEtiqueta == "/vertex_list") {
			// Fin del bloque de vertices
			return true;
		}
		else if(strEtiqueta == "v") {
			// Retrocedemos 3 posiciones en el archivo para poder leer
			// correctamente el vector
			for(int i=0; i < 3; i++, m_fileEscena.unget());

			if(!readBloquePoint("v", p0))
				return false;
			if(!readBloquePoint("v", p1))
				return false;
			if(!readBloquePoint("v", p2))
				return false;

			pMesh->addVertex(p0);
			pMesh->addVertex(p1);
			pMesh->addVertex(p2);

			MeshFace 	NewMF(-1);
			int			nCount = pMesh->getTriangleCount();

			// Triangle vertex
			NewMF.nVIndex[0] = (nCount * 3);
			NewMF.nVIndex[1] = (nCount * 3) + 1;
			NewMF.nVIndex[2] = (nCount * 3) + 2;

			// No normal given, calculate from points.

			// No texture given.

			// Creamos el triangulo
			MeshTriangle *pNewT = new MeshTriangle(NewMF, pMesh);

			// Añadimos el triangulo a la lista
			MTList->push_back(pNewT);

			// Incrementamos el numero de triangulos que tiene la malla
			pMesh->increaseTriangleCount();
		}
	}
}

bool Parser::processMeshFile(Shape *pShape, std::vector<MeshTriangle *> *MTList)
{
	std::string 	strEtiqueta, strFile, strLine, strType;
	std::ifstream 	fsFile;

	Point			v;	// Vertice
	Vec3			n;	// Normal
	Vec2			t;	// Textura

	Mesh			*pMesh = dynamic_cast<Mesh *> (pShape);

	int				nVertex, nNormals, nTextures, nFaces;

	// Para el log
	nVertex = nNormals = nTextures = nFaces = 0;

	if(!readContent(strFile, '<'))
        return false;

	if(!readToken(strEtiqueta))
		return false;

	if(strEtiqueta != "/file")
		return false;

	// Abrir fichero
	fsFile.open(strFile.c_str());

	// Leemos el fichero linea a linea
	while(std::getline(fsFile, strLine)){
		if((strLine[0] == 'v') && (strLine[1] == ' ')) { // Vertice
			// Procesar vertice

			unsigned int i = 1; // Posicion en la linea

			// Evitamos los espacios antes del primer numero, si los hay
			while(strLine[i] == ' ') i++;

			for(int j = 0; j < 3; j++) { // Tres veces para tres float
				std::string strTemp("");

				while(1) { // Hasta que no encontremos un espacio
					if((strLine[i] != ' ') && (i < (strLine.length() -1))) {
						strTemp = strTemp + strLine[i];
						i++; // Siguiente caracter
					}
					else
					{
						i++; 	// Saltamos el espacio en blanco
						break;
					}
				}

				if(strTemp.length() > 0) {
					if(validFloat(strTemp))
						v.e[j] = atof(strTemp.c_str());
					else {
						// Lanzar error
					}
				}
			}

			pMesh->addVertex(v);
            nVertex++;
		}
		else if((strLine[0] == 'v') && (strLine[1] == 'n')) { // Normal
			// Procesar normal

			unsigned int i = 3; // Posicion en la linea
			for(int j = 0; j < 3; j++) { // Tres veces para tres float
				std::string strTemp("");

				while(1) { // Hasta que no encontremos un espacio
					if((strLine[i] != ' ') && (i < (strLine.length() -1))) {
						strTemp = strTemp + strLine[i];
						i++; // Siguiente caracter
					}
					else
					{
						i++; 	// Saltamos el espacio en blanco
						break;
					}
				}

				if(strTemp.length() > 0) {
					if(validFloat(strTemp))
						n.e[j] = atof(strTemp.c_str());
					else {
						// Lanzar error
					}
				}
			}

			pMesh->addNormal(n);
            nNormals++;
		}
		else if((strLine[0] == 'v') && (strLine[1] == 't')) { // Texture
			// Procesar textura

			unsigned int i = 3; // Posicion en la linea
			for(int j = 0; j < 2; j++) { // Dos veces para Dos float
				std::string strTemp("");

				while(1) { // Hasta que no encontremos un espacio
					if((strLine[i] != ' ') && (i < (strLine.length() -1))) {
						strTemp = strTemp + strLine[i];
						i++; // Siguiente caracter
					}
					else
					{
						i++; 	// Saltamos el espacio en blanco
						break;
					}
				}

				if(strTemp.length() > 0) {
					if(validFloat(strTemp))
						t.e[j] = atof(strTemp.c_str());
					else {
						// Lanzar error
					}
				}
			}

            pMesh->addTexture(t);
            nTextures++;
		}
		else if((strLine[0] == 'f') && (strLine[1] == ' ')) { // Face
			// Procesar face

			MeshFace mfFace(-1);

			unsigned int i = 2, nBlock = 0;
			while(nBlock < 3) {
				int nNumber = 0; 			// Primer numero del bloque
				std::string strTemp(""); 	// Temporal para almacenar numeros

				while(nNumber < 3) { // Repetir hasta el tercer numero
					if(strLine[i] == '/') { // Fin de numero, excepto el ultimo del bloque
						// Si la cadena temporal tiene un entero valido
						if((strTemp.length() > 0) && validInt(strTemp)) {
							switch(nNumber) { // Veamos que posicion tenemos
								case 0: // Vertice
									mfFace.nVIndex[nBlock] = atoi(strTemp.c_str()) - 1;
									break;
								case 1: // Textura
									mfFace.nTIndex[nBlock] = atoi(strTemp.c_str()) - 1;
									break;
							}
						}
						// Si la cadena está vacía o el numero no es valido
						// no almacenamos nada. El valor está por defecto a -1,
						// con lo que el render sabrá qué hacer en caso de
						// encontrarlo.

						strTemp.clear(); 	// Limpiamos la cadena
						nNumber++;			// Siguiente numero
						i++;				// Avanzamos en la linea
					}
					else if(strLine[i] == ' ') { // Fin de bloque
						// Almacenamos el numero, si lo hay
						if((strTemp.length() > 0) && validInt(strTemp)) {
							switch(nNumber) {
								case 0: // Vertice
									mfFace.nVIndex[nBlock] = atoi(strTemp.c_str()) - 1;
									break;
								case 1: // Textura
									mfFace.nTIndex[nBlock] = atoi(strTemp.c_str()) - 1;
									break;
								case 2: // Normal
									mfFace.nNIndex[nBlock] = atoi(strTemp.c_str()) - 1;
									break;
							}
						}

						nNumber = 3;	// Salimos del while
						nBlock++;		// Nuevo bloque
						i++;			// Avanzamos por la cadena
					}
					else if(i == (strLine.length() - 1)) { // Final de linea
						// Almacenamos el numero, si lo hay
						if((strTemp.length() > 0) && validInt(strTemp)) {
							switch(nNumber) {
								case 0: // Vertice
									mfFace.nVIndex[nBlock] = atoi(strTemp.c_str()) - 1;
									break;
								case 1: // Textura
									mfFace.nTIndex[nBlock] = atoi(strTemp.c_str()) - 1;
									break;
								case 2: // Normal
									mfFace.nNIndex[nBlock] = atoi(strTemp.c_str()) - 1;
									break;
							}
						}

						nNumber = 3;
						nBlock = 3; 	// Salimos de ambos while
					}
					else { // Metemos el caracter en la cadena temporal, y seguimos
						strTemp = strTemp + strLine[i];
						i++;
					}
				}
			}

			// Creamos el nuevo triangulo.
			MeshTriangle *pNewT = new MeshTriangle(mfFace, pMesh);
			// Lo añadimos a la lista
			MTList->push_back(pNewT);

			// Incrementamos el numero de triangulos en la malla
			pMesh->increaseTriangleCount();

			nFaces++;
		}
	}
	fsFile.close();

	return true;
}

bool Parser::processTransform(Transform *pTrans)
{
	std::string strEtiqueta;
	Point		p3Temp;

	while(1) { // Always true
		if(!ignorarChars())
				return false;

		if(!readToken(strEtiqueta))
			return false;

		if(strEtiqueta == "/transform") {
			// Fin del bloque de transformaciones
			return true;
		}
		else {
			// Retrocedemos hasta el inicio de la etiqueta, para poder
			// utilizar las funciones de lectura de datos.
			for(unsigned int i = 0; i < strEtiqueta.length() + 2; i++, m_fileEscena.unget());

			if(strEtiqueta == "translate") {
				if(!readBloquePoint(strEtiqueta, p3Temp))
					return false;

				pTrans->translate(p3Temp);
			}
			else if(strEtiqueta == "scale") {
				if(!readBloquePoint(strEtiqueta, p3Temp))
					return false;

				pTrans->scale(p3Temp);
			}
			else if(strEtiqueta == "rotate_axis") {
				float fltArray[4];

				if(!readBloqueFloats(strEtiqueta, 4, fltArray))
					return false;

				pTrans->rotate(fltArray[0], Vec3(fltArray[1], fltArray[2], fltArray[3]));
			}
			else if(strEtiqueta == "rotate_x") {
				float dAng;

				if(!readBloqueFloats(strEtiqueta, 1, &dAng))
					return false;

				pTrans->rotateX(dAng);
			}
			else if(strEtiqueta == "rotate_y") {
				float dAng;

				if(!readBloqueFloats(strEtiqueta, 1, &dAng))
					return false;

				pTrans->rotateY(dAng);
			}
			else if(strEtiqueta == "rotate_z") {
				float dAng;

				if(!readBloqueFloats(strEtiqueta, 1, &dAng))
					return false;

				pTrans->rotateZ(dAng);
			}
			else {
				// Etiqueta desconocida.
				m_nError = 0;
				return false;
			}
		} // else
	} // while
}
