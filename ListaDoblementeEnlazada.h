#include <iostream>
using namespace std;
#include <vector>


template <typename T>
class ListaClientes {
        Nodo<T>* head;
        vector<T> vectorList;
    public:
        ListaClientes() : head(nullptr) {}
        void registrarCliente(T cliente) {

            Nodo<T>* nuevo = new Nodo<T>(cliente);
            
            if(head == nullptr) {
                head = nuevo;
            } else {
                Nodo<T>* temp = head;
                while(temp -> next != nullptr){
                    temp = temp -> next;
                }
                // nuevo -> prev = temp; 
                temp -> next = nuevo;
            }
            cout << "Usuario registrado exitosamente...\n";
        }

        Nodo<T>* buscarCliente(string opcion, string valor) {

            if(head == nullptr) {
                cout<<"\n-------------------------------\n";
                cout << "No hay clientes registrados...\n";
                cout<<"-------------------------------\n\n";
                return nullptr;
            }

            Nodo<T>* temp = head;

            while(temp != nullptr) {
            
                if(opcion == "codigo") {
                    if(temp->data.codigo_usuario == stoi(valor)) {
                        return temp;
                    }
                }
            
                else if(opcion == "empresa") {
                    if(temp->data.nombre_empresa == valor) {
                        return temp;
                    }
                }
            
                temp = temp->next;
            }
            //  cout<<"Cliente no encontrado...\n";
                return nullptr; // no encontrado

            }


            //     ListaClientes<Cliente> listaDoble;     //Lista doble enlazada - ordenar clientes
    // PilaAcciones<string>   pilaAcciones;
       

        void madeVector() {

            Nodo<T>* temp = head;

            while(temp != nullptr) {
                vectorList.push_back(temp->data);
                temp = temp -> next;
            }

        }


        void desordenarClientes() {
            //shuffle
            madeVector();

            for(int i = vectorList.size() - 1; i > 0; i-- ) { // en este algoritmos se reduce una iteracion y se va en reversa
                int j = rand() % (i+1); // rand() te da un numero aleatorio grande de 0 a RAND_MAX
                swap(vectorList[i], vectorList[j]);
            }
            //Emparejar los valores con la lista enlazada
            Nodo<T>* aux = head;
            int i = 0;

            while(aux != nullptr) {
                aux -> data = vectorList[i++]; // aux -> data = vectorList[i]; i = i + 1;  esa linea es equivalente a lo mostrado. 
                aux = aux -> next;
            }

        }

        void ordenarCliente_opcion(string opcion) {

            if(head == nullptr) return;
                
            bool swapped;
                
            do {
                swapped = false;
                Nodo<T>* temp = head;
            
                while(temp->next != nullptr) {
                
                    bool condicion = false;
                
                    if(opcion == "nombre") {
                        condicion = temp->data.nombre_empresa > temp->next->data.nombre_empresa;
                    }
                
                    else if(opcion == "codigo") {
                        condicion = temp->data.codigo_usuario > temp->next->data.codigo_usuario;
                    }
                
                    // else if(opcion == "ventas") {
                    //     int total1 = sumarVentas(temp->data.ventas);
                    //     int total2 = sumarVentas(temp->next->data.ventas);
                    
                    //     condicion = total1 > total2;
                    // }

                if(condicion) {
                    swap(temp->data, temp->next->data);
                    swapped = true;
                }
                temp = temp->next;
            }
        } while(swapped);
}

        void mostrarClientes(string opcion) {

            if(head == nullptr) {
                cout << "-------------------------------\n";
                cout << "No hay clientes registrados...\n";
                cout << "-------------------------------\n\n";
                return;
            }

            Nodo<T>* temp = head;
            while(temp != nullptr) {
                cout << "-----------------------------\n";
                if(opcion == "codigo") {
                    cout << "codigo usuario: " << temp->data.codigo_usuario << endl;
                } else {
                    //nombre empresa
                    cout << "Nombre empresa: " << temp->data.nombre_empresa << endl;
                }

                temp = temp -> next;
            }
        }

        
        void eliminarCliente(string opcion, string valor) {

            if(head == nullptr) return;

            Nodo<T>* temp = head;
            Nodo<T>* prev = nullptr;

            while(temp != nullptr) {
            
                bool coincide = false;
            
                if(opcion == "nombre" && temp->data.nombre_empresa == valor)
                    coincide = true;
            
                if(opcion == "codigo" && to_string(temp->data.codigo_usuario) == valor)
                    coincide = true;
            
                if(coincide) {
                
                    if(prev == nullptr) {
                        // eliminar head
                        head = temp->next;
                    } else {
                        prev->next = temp->next;
                    }
                
                    delete temp;
                    cout << "Cliente eliminado\n";
                    return;
                }
            
                prev = temp;
                temp = temp->next;
            }
        
            cout << "Cliente no encontrado\n";
        }

};