#ifndef __PARSER_H__
#define __PARSER_H__

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

#include "vzray.h"
#include "vec3.h"
#include "point.h"
#include "shapes/shape.h"
#include "shapes/meshtriangle.h"
#include "matrix/transform.h"

/**
 * Clase para procesar el fichero en el que se describe la escena.
 */
class Parser {
    private:
        // Puntero a los datos globales que rellenaremos con la informacion de archivo.
        Globals *m_pGlobals;
        
		// Manejador del archivo.
        std::ifstream m_fileEscena;
        
		// Mensajes de error del parser.
		std::string m_strTipoError[15];
        
		// Control de los mensajes de error.
        signed int m_nError;        
		
		// Controlamos la línea en la que estamos, para en el caso de tener
		// que informar de un error, poder decir en que linea ha ocurrido.
		int m_nLine;
        
        // Nos indicará si hemos llegado al fin del fichero.
        bool m_bEOF;
        
		// Constantes auxiliares.
        const int MAX_ETIQUETA;
        const int MAX_CONTENT;
		
        // Import file basic functions
        bool readToken(std::string &strToken);
        bool readContent(std::string &strContent, const char cStopChar);
        bool readFloat(float &dDato, const char cStopChar);
        bool readInt(int &nDato, const char cStopChar);
        bool readBloqueTxt(const std::string &strEtiqueta, std::string &strResultado);
        bool readBloqueFloats(const std::string &strEtiqueta, const int nNumF, float *fltArray);
        bool readBloqueInts(const std::string &strEtiqueta, const int nNumF, int *intArray);
		bool readBloqueVec3(const std::string &strEtiqueta, Vec3 &v3Resultado);
		bool readBloquePoint(const std::string &strEtiqueta, Point &p3Resultado);

        bool ignorarChars();
		
		bool validFloat(const std::string &strFloat);
		bool validInt(const std::string &strInt);
		
		bool isComment();
			
		// Process imported data
		bool processComment();
		
		bool processConfig();
		bool processRenderer();
		bool processCamera();
		bool processImage();
		
		bool processScene();
		
		bool processTransform(Transform *pTrans);
		
		bool processTexture();
		bool processSimpleTex();
		
		bool processMaterial();
		bool processDiffuseMat();
		bool processSpecularMat();
		bool processDielectricMat();
		bool processLightMat();
		
		bool processObject();
		bool processPlane();
		bool processSphere();
		bool processCylinder();
		bool processBox();
		bool processParallelogram();
		bool processTriangle();
		bool processMesh();
		bool processMeshVertex(Shape *pShape, std::vector<MeshTriangle *> *MTList);
		bool processMeshFile(Shape *pShape, std::vector<MeshTriangle *> *MTList);
        
    public:
        Parser();
        ~Parser();
        /**
         * Lee el fichero indicado, y almacena el resultado en pGlobals.
         * 
         * @param strName Ruta al fichero.
         * @param pGloblas Puntero a un objeto de clase Globals.
         * @return Verdadero si todo ha ido bien.
         */
        bool leerFichero(const std::string &strName, Globals *pGlobals);
        
        /**
         * Lee el fichero indicado, y almacena el resultado en pGlobals.
         * 
         * @param strName Ruta al fichero.
         * @param pGloblas Puntero a un objeto de clase Globals.
         * @return Verdadero si todo ha ido bien.
         */
        void getError(std::string &strError, int &nLine, int &nErrorCode);
};

#endif // __PARSER_H__
