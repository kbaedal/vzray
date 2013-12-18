/**********************************************************************
 * Plantilla para listas doblemente enlazadas.                        *
 * Al ser una plantilla han de ir tanto la declaración como la        *
 * implementación en el mismo fichero.                                *
 **********************************************************************/

#ifndef __DLLIST_HPP__
#define __DLLIST_HPP__

#include <string>

// Máscaras para los flags.
const unsigned int DLL_UNIQUE_ID 	= 0x01; // 0000 0001
const unsigned int DLL_DELETE_DATA 	= 0x02; // 0000 0010

template <class T> class DLListNode {
	public:
		DLListNode(std::string strID, T *a_pT, bool a_bDel = false)
		{
			m_pT = a_pT;
			m_pSig = m_pAnt = NULL;
			m_strID = strID;
			m_bDel = a_bDel;
		}

		~DLListNode()
		{			
			if(m_pT != NULL && m_bDel)
				delete m_pT;
		}			
	
		void setData(T *a_pT) { m_pT = a_pT; }
		T *getData() { return m_pT; }
		
		void setID(std::string a_strID) { m_strID = a_strID; };
		std::string getID() { return m_strID; }
		
		void setSig(DLListNode<T> *pSig) { m_pSig = pSig; }
		DLListNode *getSig() { return m_pSig; }
		void setAnt(DLListNode<T> *pAnt) { m_pAnt = pAnt; }
		DLListNode *getAnt() { return m_pAnt; }
	private:
		T 				*m_pT;
		std::string 	m_strID;		
		DLListNode<T> 	*m_pSig, *m_pAnt;
		bool			m_bDel;
};

template <class T> class DLList {
	public:
		DLList(unsigned int nFlags = 0x00)
		{
			m_pInicio = m_pFin = NULL;
			m_nNumEltos = 0;
			m_nUNIQUE_ID = nFlags & DLL_UNIQUE_ID;
			m_nDELETE_DATA = nFlags & DLL_DELETE_DATA;		
		}
		~DLList();
		
		// Insertamos un nuevo elemento en la lista, y le asignamos un
		// nombre que sera su identificador.
		// Devuelve false en caso de error. 
		bool insertar(std::string strID, T *pDatos);
		
		// Borramos el primer elemento de la lista que tenga el ID indicado. 
		// Devuelve false en caso de error.
		bool eliminar(std::string strID);
		
		// Borramos el elemento i-ésimo de la lista. 
		// Devuelve false en caso de error.
		bool eliminar(int i);
		
		// Devolvemos los datos del primer elemento de la lista que tenga
		// el ID indicado. Devuelve null si no existen los datos buscados.
		T *getData(std::string strID);
		
		// Nos devolverá el elemento en la i-ésima posición de la
		// lista, o NULL si esta fuera de rango.
		T *getData(int i);
		
		// Devuelve el numero de elementos en la lista.
		int getNumEltos() { return m_nNumEltos; }
		
		// Devuelve el ID del elemento i-ésimo.
		std::string getID(int i);
		
	private:
		int				m_nNumEltos;			// Numero de elementos de la lista
		DLListNode<T> 	*m_pInicio, *m_pFin;	// Punteros para manejar la lista
		/*
		 * Flags:
		 * 	- m_nUNIQUE_ID: Si está activo, no se podrá insertar un nodo nuevo
		 * 		con un ID que ya tenga otro nodo.
		 * 	- m_nDELETE_DATA: Si está activo, los datos a los que apuntan los
		 * 		nodos de la lista serán eliminados al destruir la lista.
		 */
		unsigned int	m_nUNIQUE_ID, m_nDELETE_DATA;		
};

template <class T>
bool DLList<T>::insertar(std::string strID, T *pDatos)
{
	if(m_pInicio == NULL) { // Primer nodo de la lista.
		m_pInicio = new DLListNode<T>(strID, pDatos);
		m_pFin = m_pInicio;
	}
	else { // No es el primero, si es necesario, comprobamos el ID 
		DLListNode<T> *pTemp = m_pInicio;

		if((m_nUNIQUE_ID & DLL_UNIQUE_ID) != 0)
		{
			while (pTemp != NULL) {
				if(strID.compare(pTemp->getID()) == 0)
				// Si encontramos un ID igual, devolvemos error.
					return false;
					
				pTemp = pTemp->getSig();
			}
		}
		
		// No hay un ID igual o no es necesario comprobarlo, insertamos.
		// Pasaremos el flag m_nDELETE_DATA como ultimo argumento al constructor
		// del nodo:
		// 	- m_nDELETE_DATA & DLL_DELETE_DATA = true si m_nDELETE_DATA = 0x02,
		//		falso en otra circunstancia.
		pTemp = new DLListNode<T>(strID, pDatos, m_nDELETE_DATA & DLL_DELETE_DATA);
		
		m_pFin->setSig(pTemp);
		pTemp->setAnt(m_pFin);
		m_pFin = pTemp;
	}
	// Incrementamos nuestro contador de elementos en la lista.
	m_nNumEltos++; 	
	return true;	
}

template <class T>
bool DLList<T>::eliminar(std::string strID)
{
	bool 			bFound = false;
	DLListNode<T> 	*pTemp = m_pInicio;
	
	while((pTemp != NULL) && !bFound) {
		if(strID.compare(pTemp->getID()) == 0) {
			bFound = true;
		}
		else { 	// Avanzamos por la lista si no encontramos conincidencia.
				// En caso de encontrarla no avanzamos, para mantener
				// el puntero temporal apuntado al nodo adecuado.
			pTemp = pTemp->getSig();
		}
	}
	
	if(!bFound) { // No hemos encontrado nada que eliminar.
		return false;
	} 
	else { // Encontrado. pTemp apunta a nuestro nodo.
		if(m_pInicio == m_pFin) {
			// Tenemos un unico nodo en la lista. Eliminamos y ponemos
			// lus punteros a NULL.
			delete pTemp;
			m_pInicio = m_pFin = NULL;
		}
		else if(pTemp == m_pInicio) {
			// Borramos el nodo inicial, que no es el único.
			m_pInicio = pTemp->getSig();
			m_pInicio->setAnt(NULL);
			
			delete pTemp;
		}
		else if(pTemp == m_pFin) {
			// Lo mismo, pero para un nodo final.
			m_pFin = pTemp->getAnt();
			m_pFin->setSig(NULL);
			
			delete pTemp;
		}
		else {
			// Nodo intermedio, que no es inicial ni final.
			pTemp->getAnt()->setSig(pTemp->getSig());
			pTemp->getSig()->setAnt(pTemp->getAnt());
		
			delete pTemp;
		}
	}
	// Decrementamos nuestro contador de elementos.
	m_nNumEltos--;
	return true;
}


template <class T>
bool DLList<T>::eliminar(int i)
{
	if((i < 0) || (i >= m_nNumEltos)) 	// Si se nos pide un elemento fuera
		return false;					// de rango, devolvemos false.
	else {
		int 			n = 0;
		DLListNode<T> 	*pTemp = m_pInicio;
		
		while(i != n) {
			pTemp = pTemp->getSig();
			n++;
		}
		
		// Encontrado. pTemp apunta a nuestro nodo.
		if(m_pInicio == m_pFin) {
			// Tenemos un unico nodo en la lista. Eliminamos y ponemos
			// lus punteros a NULL.
			delete pTemp;
			m_pInicio = m_pFin = NULL;
		}
		else if(pTemp == m_pInicio) {
			// Borramos el nodo inicial, que no es el único.
			m_pInicio = pTemp->getSig();
			m_pInicio->setAnt(NULL);
			
			delete pTemp;
		}
		else if(pTemp == m_pFin) {
			// Lo mismo, pero para un nodo final.
			m_pFin = pTemp->getAnt();
			m_pFin->setSig(NULL);
			
			delete pTemp;
		}
		else {
			// Nodo intermedio, que no es inicial ni final.
			pTemp->getAnt()->setSig(pTemp->getSig());
			pTemp->getSig()->setAnt(pTemp->getAnt());
		
			delete pTemp;
		}
	}
	
	// Decrementamos nuestro contador de elementos.
	m_nNumEltos--;
	return true;
}

template <class T>
T *DLList<T>::getData(std::string strID)
{
	DLListNode<T> 	*pTemp = m_pInicio;
	
	while(pTemp != NULL) {		
		if(strID.compare(pTemp->getID()) == 0)
			return pTemp->getData();
		pTemp = pTemp->getSig();
	}
	return NULL;		
}

template <class T>
T *DLList<T>::getData(int i)
{
	if((i < 0) || (i >= m_nNumEltos)) 	// Si se nos pide un elemento fuera
		return NULL;					// de rango, devolvemos NULL.
	else {
		int 			n = 0;
		DLListNode<T> 	*pTemp = m_pInicio;
		
		while(i != n) {
			pTemp = pTemp->getSig();
			n++;
		}
		
		return pTemp->getData();
	}
}

template <class T>
std::string DLList<T>::getID(int i)
{
	if((i < 0) || (i >= m_nNumEltos)) 	// Si se nos pide un elemento fuera
		return NULL;					// de rango, devolvemos NULL.
	else {
		int 			n = 0;
		DLListNode<T> 	*pTemp = m_pInicio;
		
		while(i != n) {
			pTemp = pTemp->getSig();
			n++;
		}
		
		return pTemp->getID();
	}
}

template <class T>
DLList<T>::~DLList()
{
	if(m_pInicio != NULL) { // Si la lista no está vacía.
		while(m_pInicio != m_pFin) { // Mientras quede más de un nodo
			DLListNode<T> *pTemp = m_pFin;
			
			m_pFin = pTemp->getAnt();
			m_pFin->setSig(NULL);
			
			delete pTemp;
		}
		
		// Nos queda el último nodo.
		delete m_pInicio;
	}
}

#endif // __DLLIST_HPP__

