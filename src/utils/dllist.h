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
		DLListNode(std::string a_id, T *a_data, bool a_del = false)
		{
			data    = a_data;
			id      = a_id;
			del     = a_del;

			sig = ant = nullptr;
		}

		~DLListNode()
		{
			if(data != nullptr && del)
				delete data;
		}

		void set_data(T *a_data) { data = a_data; }
		T *get_data() { return data; }

		void set_id(std::string a_id) { id = a_id; };
		std::string get_id() { return id; }

		void set_sig(DLListNode<T> *a_sig) { sig = a_sig; }
		DLListNode *get_sig() { return sig; }
		void set_ant(DLListNode<T> *a_ant) { ant = a_ant; }
		DLListNode *get_ant() { return ant; }
	private:
		T 				*data;
		std::string 	id;
		DLListNode<T> 	*sig, *ant;
		bool			del;
};

template <class T> class DLList {
	public:
		DLList(unsigned int flags = 0x00)
		{
			num_eltos   = 0;
			unique_id   = flags & DLL_UNIQUE_ID;
			delete_data = flags & DLL_DELETE_DATA;

			inicio = fin = nullptr;
		}
		~DLList();

		// Insertamos un nuevo elemento en la lista, y le asignamos un
		// nombre que sera su identificador.
		// Devuelve false en caso de error.
		bool insertar(std::string id, T *data);

		// Borramos el primer elemento de la lista que tenga el ID indicado.
		// Devuelve false en caso de error.
		bool eliminar(std::string id);

		// Borramos el elemento i-ésimo de la lista.
		// Devuelve false en caso de error.
		bool eliminar(int i);

		// Devolvemos los datos del primer elemento de la lista que tenga
		// el ID indicado. Devuelve null si no existen los datos buscados.
		T *get_data(std::string id);

		// Nos devolverá el elemento en la i-ésima posición de la
		// lista, o nullptr si esta fuera de rango.
		T *get_data(int i);

		// Devuelve el numero de elementos en la lista.
		int get_num_eltos() { return num_eltos; }

		// Devuelve el ID del elemento i-ésimo.
		std::string get_id(int i);

	private:
		int				num_eltos;		// Numero de elementos de la lista
		DLListNode<T> 	*inicio, *fin;  // Punteros para manejar la lista
		/*
		 * Flags:
		 * 	- unique_id: Si está activo, no se podrá insertar un nodo nuevo
		 * 		con un ID que ya tenga otro nodo.
		 * 	- delete_data: Si está activo, los datos a los que apuntan los
		 * 		nodos de la lista serán eliminados al destruir la lista.
		 */
		//unsigned int	unique_id, delete_data;
		bool    unique_id,
                delete_data;
};

template <class T>
bool DLList<T>::insertar(std::string id, T *data)
{
	if(inicio == nullptr) { // Primer nodo de la lista.
		inicio = new DLListNode<T>(id, data);
		fin = inicio;
	}
	else { // No es el primero, si es necesario, comprobamos el ID
		DLListNode<T> *temp = inicio;

		//if((unique_id & DLL_UNIQUE_ID) != 0)
		if(unique_id)
		{
			while (temp != nullptr) {
				if(id.compare(temp->get_id()) == 0)
				// Si encontramos un ID igual, devolvemos error.
					return false;

				temp = temp->get_sig();
			}
		}

		// No hay un ID igual o no es necesario comprobarlo, insertamos.
		// Pasaremos el flag m_nDELETE_DATA como ultimo argumento al constructor
		// del nodo:
		// 	- m_nDELETE_DATA & DLL_DELETE_DATA = true si m_nDELETE_DATA = 0x02,
		//		falso en otra circunstancia.
		//temp = new DLListNode<T>(strID, pDatos, m_nDELETE_DATA & DLL_DELETE_DATA);
		temp = new DLListNode<T>(id, data, delete_data);

		fin->set_sig(temp);
		temp->set_ant(fin);
		fin = temp;
	}
	// Incrementamos nuestro contador de elementos en la lista.
	++num_eltos;
	return true;
}

template <class T>
bool DLList<T>::eliminar(std::string id)
{
	bool 			found   = false;
	DLListNode<T> 	*temp   = inicio;

	while((temp != nullptr) && !found) {
		if(id.compare(temp->get_id()) == 0) {
			found = true;
		}
		else { 	// Avanzamos por la lista si no encontramos conincidencia.
				// En caso de encontrarla no avanzamos, para mantener
				// el puntero temporal apuntado al nodo adecuado.
			temp = temp->get_sig();
		}
	}

	if(!found) { // No hemos encontrado nada que eliminar.
		return false;
	}
	else { // Encontrado. temp apunta a nuestro nodo.
		if(inicio == fin) {
			// Tenemos un unico nodo en la lista. Eliminamos y ponemos
			// lus punteros a nullptr.
			delete temp;
			inicio = fin = nullptr;
		}
		else if(temp == inicio) {
			// Borramos el nodo inicial, que no es el único.
			inicio = temp->get_sig();
			inicio->set_ant(nullptr);

			delete temp;
		}
		else if(temp == fin) {
			// Lo mismo, pero para un nodo final.
			fin = temp->get_ant();
			fin->set_sig(nullptr);

			delete temp;
		}
		else {
			// Nodo intermedio, que no es inicial ni final.
			temp->get_ant()->set_sig(temp->get_sig());
			temp->get_sig()->set_ant(temp->get_ant());

			delete temp;
		}
	}
	// Decrementamos nuestro contador de elementos.
	--num_eltos;
	return true;
}


template <class T>
bool DLList<T>::eliminar(int i)
{
	if((i < 0) || (i >= num_eltos)) 	// Si se nos pide un elemento fuera
		return false;					// de rango, devolvemos false.
	else {
		int 			n = 0;
		DLListNode<T> 	*temp = inicio;

		while(i != n) {
			temp = temp->get_sig();
			++n;
		}

		// Encontrado. temp apunta a nuestro nodo.
		if(inicio == fin) {
			// Tenemos un unico nodo en la lista. Eliminamos y ponemos
			// lus punteros a nullptr.
			delete temp;
			inicio = fin = nullptr;
		}
		else if(temp == inicio) {
			// Borramos el nodo inicial, que no es el único.
			inicio = temp->get_sig();
			inicio->set_ant(nullptr);

			delete temp;
		}
		else if(temp == fin) {
			// Lo mismo, pero para un nodo final.
			fin = temp->get_ant();
			fin->set_sig(nullptr);

			delete temp;
		}
		else {
			// Nodo intermedio, que no es inicial ni final.
			temp->get_ant()->set_sig(temp->get_sig());
			temp->get_sig()->set_ant(temp->get_ant());

			delete temp;
		}
	}

	// Decrementamos nuestro contador de elementos.
	--num_eltos;
	return true;
}

template <class T>
T *DLList<T>::get_data(std::string id)
{
	DLListNode<T> 	*temp = inicio;

	while(temp != nullptr) {
		if(id.compare(temp->get_id()) == 0)
			return temp->get_data();
		temp = temp->get_sig();
	}

	return nullptr;
}

template <class T>
T *DLList<T>::get_data(int i)
{
	if((i < 0) || (i >= num_eltos)) 	// Si se nos pide un elemento fuera
		return nullptr;					// de rango, devolvemos nullptr.
	else {
		int 			n = 0;
		DLListNode<T> 	*temp = inicio;

		while(i != n) {
			temp = temp->get_sig();
			++n;
		}

		return temp->get_data();
	}
}

template <class T>
std::string DLList<T>::get_id(int i)
{
	if((i < 0) || (i >= num_eltos)) 	// Si se nos pide un elemento fuera
		return nullptr;					// de rango, devolvemos nullptr.
	else {
		int 			n = 0;
		DLListNode<T> 	*temp = inicio;

		while(i != n) {
			temp = temp->get_sig();
			++n;
		}

		return temp->get_id();
	}
}

template <class T>
DLList<T>::~DLList()
{
	if(inicio != nullptr) { // Si la lista no está vacía.
		while(inicio != fin) { // Mientras quede más de un nodo
			DLListNode<T> *temp = fin;

			fin = temp->get_ant();
			fin->set_sig(nullptr);

			delete temp;
		}

		// Nos queda el último nodo.
		delete inicio;
	}
}

#endif // __DLLIST_HPP__

