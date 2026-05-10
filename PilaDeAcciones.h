#include <iostream>
using namespace std;
#include <vector>


template <typename T>
class PilaAcciones {
        Nodo<T>* head;
        void mostrarRecursivo(Nodo<T>* aux, int count, string accion = "", bool eleccion = false) {
            //accion -> venta, llamada, etc
            if(aux == nullptr) return;

            if(eleccion == false) {
                cout << count << ". " << aux->data << endl;
                mostrarRecursivo(aux->next, count + 1, accion, eleccion);
            
            } else {
                if(aux->data == accion) {
                    cout << count << ". " << aux->data << endl;
                    count++;
                }
                mostrarRecursivo(aux->next, count, accion, eleccion);
            }
        }   
    public:
        PilaAcciones() : head(nullptr) {}

    // ListaClientes<Cliente> listaDoble;     //Lista doble enlazada - ordenar clientes
    // PilaAcciones<string>   pilaAcciones;   // Pila          - historial de acciones


        void registrarInteraccion(T interaccion, Nodo<Cliente>* cliente){
        
            // 🔹 nodo para la pila
            Nodo<T>* nuevoPila = new Nodo<T>(interaccion);
            nuevoPila->next = head;
            head = nuevoPila;
        
            // 🔹 nodo para el cliente
            Nodo<T>* nuevoCliente = new Nodo<T>(interaccion);
            nuevoCliente->next = cliente->data.interacciones;
            cliente->data.interacciones = nuevoCliente;
        
            cout << "Interaccion registrada exitosamente -> " << interaccion << endl;
        }

        void eliminarInteraccion(T interaccion, Nodo<Cliente>* cliente) {
            // Eliminar de la pila
            Nodo<T>* temp = head; 
            Nodo<T>* prev = nullptr;

            while(temp != nullptr) {
            
                if(temp->data == interaccion) {
                    if(prev == nullptr) { //SI es el ultimo elemento de la pila 
                        head = head->next;
                    } else {
                        prev->next = temp->next;
                    }
                    delete temp;
                    break;
                }
            
                prev = temp;
                temp = temp->next;
            }

            // Eliminar del cliente

           Nodo<T>* temp2 = head; 
           Nodo<T>* prev2 = nullptr;

            while(temp2 != nullptr) {
            
                if(temp2->data == interaccion) {
                    if(prev2 == nullptr) { //SI es el ultimo elemento de la pila 
                        head = head->next;
                    } else {
                        prev2->next = temp2->next;
                    }
                    delete temp;
                    return;
                }
            
                prev2 = temp2;
                temp2 = temp2->next;
            }
        }

        void mostrarTodasInteracciones(string accion, bool eleccion) {
            mostrarRecursivo(head, 1, accion, eleccion);
        }

};
