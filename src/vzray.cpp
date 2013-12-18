#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstring>

#include "parser/parser.h"

#include "test.h"

#include "vzray.h"

static void MuestraAyuda(std::string name)
{
    std::cerr 	<< "Uso: " << name << " <opcion(es)> fichero-a-renderizar <fichero-imagen-salida>\n"
				<< "Opciones:\n"
				<< "\t-h,--help    \t\tMuestra este mensaje.\n"
				<< "\t-t,--test    \t\tRealiza un test (ignorado si se indica fichero-a-renderizar).\n"
				<< "\t-s,--showaabb\t\tMuestra las AABB en la escena.\n"
				<< std::endl;
}

// Prints info of render process.
void ImprimeInfo(int nLineaAct, int nLineasTot);
void PrintTime(string strHead, float dTicks);

// Intiating data structures
bool InitData(Globals *pGlobals);
// Main render loop
bool StartRender(Globals *pGlobals);
// Cleaning the room
bool CleanData(Globals *pGlobals);

// Tells renderer to show AABB
bool showAABB(Globals *pGlobals);

int main(int argc, char *argv[])
{
	Globals TheGlobals;
	clock_t tRenderTicks;
            //tInitDataTicks,
            //tCleanDataTicks;
	bool bEndStatus;

	if(argc > 1) {
		for(int i = 1; i < argc; i++) {
			if(argv[i][0] == '-') { // Procesamos las opciones.
				if(std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help") {
					MuestraAyuda(argv[0]);
					return 0;
				}
				else if(std::string(argv[i]) == "-t" || std::string(argv[i]) == "--test") {
					TheGlobals.nOptsFlags |= GLB_DO_TEST;
				}
				else if(std::string(argv[i]) == "-s" || std::string(argv[i]) == "--showaabb") {
					TheGlobals.nOptsFlags |= GLB_SHOW_AABB;
				}
			}
			else {
				// Se proporciona fichero, anulamos el test.
				TheGlobals.nOptsFlags &= ~GLB_DO_TEST;

				if(TheGlobals.strSceneFile.size() == 0)
					// Fichero de escena a renderizar
					TheGlobals.strSceneFile.assign(argv[i]);
				else
					// Fichero donde guardar la imagen
					TheGlobals.strOutputFile.assign(argv[i]);
			}
		}
	}

	if((TheGlobals.strSceneFile.size() == 0) || (argc < 2)) {
		MuestraAyuda(argv[0]);
		return 0;
	}

	std::streambuf *stLogBuf, *stBackup;
	std::fstream fsLog;

	// Redirigiremos clog a un fichero para escribir cosas interesantes en él.
	fsLog.open("log.txt", std::fstream::out);

	stBackup = std::clog.rdbuf();	// Backup del streambuf de clog

	stLogBuf = fsLog.rdbuf();       // Obtenemos el streambuf del fichero
	std::clog.rdbuf(stLogBuf);		// Redirigimos clog

	if(TheGlobals.nOptsFlags & GLB_DO_TEST) {
		Test vTest;
		bEndStatus = vTest.LaunchTest();
	}
	else {
		tRenderTicks = clock();

		// Start render loop
		bEndStatus = StartRender(&TheGlobals);

		tRenderTicks = clock() - tRenderTicks;

		PrintTime("\nRender Time: ", ((float)tRenderTicks)/CLOCKS_PER_SEC);
	}

	std::clog.rdbuf(stBackup);		// Restauramos el streambuf de clog
	fsLog.close();					// Cerramos el fichero de log.

	if(bEndStatus)
		return 0;
	else
		return -1;
}

bool StartRender(Globals *pGlobals)
{
	CRandomMother 	MyRNG(time(NULL));
	Parser			MyParser;
	std::string 	strRendererType;

	std::cout << "\nReading file: " << pGlobals->strSceneFile << " ... ";

	if(!MyParser.leerFichero(pGlobals->strSceneFile, pGlobals)) {
		std::string strError;
		int nError, nLine;

		MyParser.getError(strError, nLine, nError);
		std::cerr << "ERROR: Linea " << nLine;
		std::cerr << " (" << nError << ") - " << strError << std::endl;

		return false;
	}

	std::cout << "Done." << std::endl;

	switch(pGlobals->pRenderer->rendererType()) {
		case 0:
			strRendererType = "Whitted raytracing";
			break;
		case 1:
			strRendererType = "Pathtracing";
			break;
		default:
			strRendererType = "Unknown";
			break;
	}

	std::cout 	<< "\nRendering:\n"
				<< " - Samples per pixel:     \t" << pGlobals->nSamplesPerPixel << " spp\n"
				<< " - Shadow rays per sample:\t" << pGlobals->nShadowRays << " sps\n"
				<< " - Image resolution:      \t" << pGlobals->nResX << "x" << pGlobals->nResY << " px\n"
				<< " - Illumination strategy: \t" << strRendererType
				<< std::endl << std::endl;

	std::clog 	<< "Rendering file " << pGlobals->strSceneFile << std::endl
				<< "Enter: Main render loop.\n";

	// Si se indica, mostramos las AABB
	if(pGlobals->nOptsFlags & GLB_SHOW_AABB)
		pGlobals->pScene->showAABB();

	for(int i = 0; i < pGlobals->nResX; i++) {
		ImprimeInfo(i+1, pGlobals->nResX);
		for(int j = 0; j < pGlobals->nResY; j++) {
			RGB rgbPixelColor(0.0, 0.0, 0.0);

			//i = pGlobals->nResX/2; j = pGlobals->nResY/2;
			//i=50; j=250;
			//std::clog << "StartRender::Shooting ray!" << endl;
			if(pGlobals->nSamplesPerPixel > 1) {
				for(int k = 0; k < pGlobals->nSamplesPerPixel; k++) {
					//std::clog << "Sampling with k = " << k << std::endl;
					Ray rRay = pGlobals->pCamera->getRay((float(i)+MyRNG.Random()-.5)/pGlobals->nResX, (float(j)+MyRNG.Random()-.5)/pGlobals->nResY, MyRNG.Random(), MyRNG.Random());

					rgbPixelColor = rgbPixelColor + pGlobals->pRenderer->getColor(rRay, pGlobals->pScene, .00001f, 1e5, 1) * 1.0/pGlobals->nSamplesPerPixel;
				}
			}
			else
			{
				Ray rRay = pGlobals->pCamera->getRay(float(i)/float(pGlobals->nResX), float(j)/float(pGlobals->nResY), MyRNG.Random(), MyRNG.Random());

				rgbPixelColor = rgbPixelColor + pGlobals->pRenderer->getColor(rRay, pGlobals->pScene, .00001f, 1e5, 1);
			}

			//std::clog << "RgbPixelColor: " << rgbPixelColor << endl;
			pGlobals->pImage->set(i, j, rgbPixelColor);

			//i=pGlobals->nResX; j=pGlobals->nResY;
		}
	}

	std::clog << "Exit: Main render loop.\n";

	pGlobals->pImage->gammaCorrect(2.2f);

	ofstream 	fsImage;
	std::string	strImageFile, strTemp;

	if(pGlobals->strOutputFile.size() == 0) {
		unsigned int    nPos = 0;
		bool 	        bFileComp = false;

		// Primera pasada: eliminar directorios, si los hay. Es decir,
		// si tenemos una cadena del estilo "../un_dir/escena.vzr",
		// nos quedamos con "escena.vzr".
		// Recorremos la cadena del final al principio para encontrar
		// el caracter '/', si lo hay. Copiamos el resto a partir
		// de esa posicion.
		for(int i = pGlobals->strSceneFile.size(); i > 0; i--) {
			switch(pGlobals->strSceneFile[i]) {
				case '/':
					nPos = i+1; // Nos colocamos en el caracter adecuado.
					bFileComp = true;
					break;
				default:
					break;
			}
			if(bFileComp)
				break;
		}
		// Copiamos la cadena.
		for(;nPos < pGlobals->strSceneFile.size(); nPos++)
			strTemp += pGlobals->strSceneFile[nPos];

		// Segunda pasada: buscamos el primer '.' desde el final,
		// que debe indicar el principio de la extensión. Copiamos
		// hasta esa posicion. Es decir, para la cadena "escena.vzr"
		// nos quedaremos con "escena".
		nPos = 0;
		bFileComp = false;
		for(int i = strTemp.size(); i > 0; i--) {
			switch(strTemp[i]) {
				case '.':
					nPos = i;
					bFileComp = true;
					break;
				default:
					break;
			}
			if(bFileComp)
				break;
		}
		// Copiamos la cadena.
		for(unsigned int i = 0; i < nPos; i++)
			strImageFile += strTemp[i];

		// Y añadimos la extension
		strImageFile += ".ppm";
	}
	else
		strImageFile.assign(pGlobals->strOutputFile);

	std::cout << "\n\nSaving file: " << strImageFile << " ... ";

	//std::string strImageFile = pGlobals->strTheFile + ".ppm";
	//fsImage.open(strImageFile.c_str(), ios::binary);

	fsImage.open(strImageFile.c_str(), ios::binary);

	pGlobals->pImage->savePPM(fsImage);

	fsImage.close();
	std::cout << "Done.\n";

	return true;
}

void ImprimeInfo(int nLineaAct, int nLineasTot)
{
	static char cBar;
	int nPos = (nLineaAct * 20) / (nLineasTot);
	char szCadena[128] ="\rProgress... [....................]  ";

	for(int i=0; i<20; i++)
	{
		if(i < nPos) szCadena[14+i] = '+';
	}

	switch(cBar) {
		case '|': cBar = '/'; break;
		case '/': cBar = '-'; break;
		case '-': cBar = '\\'; break;
		case '\\': cBar = '|'; break;
		default : cBar = '|'; break;
	}
	szCadena[36] = cBar;

	std::cout << szCadena << std::flush;
}

void PrintTime(string strHead, float dTicks)
{
	int nHours = 0, nMins = 0;

	if(dTicks >= 3600) { // At least 1 hour
		nHours = (int) dTicks / 3600;
		dTicks = dTicks - float(nHours * 3600);
	}

	if(dTicks >= 60) { // At least 1 min
		nMins = (int) dTicks / 60;
		dTicks = dTicks - float(nMins * 60);
	}

	std::cout << strHead << nHours << "h " << nMins << "m ";
	std::cout << std::fixed << std::setprecision(2) << dTicks << "s" << std::endl << std::endl;
}
