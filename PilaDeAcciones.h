#include <iostream>
using namespace std;
#include <vector>


template <typename T>
class PilaAcciones {
        Nodo<T>* head;
        void mostrarRecursivo(Nodo<T>* aux, int count, string accion = "", bool eleccion = false) {

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


        void registrarInteraccion(T interaccion){
                Nodo<T>* nuevo = new Nodo<T>(interaccion);
                nuevo -> next = head;
                head = nuevo;
        }

        void eliminarInteraccion(T interaccion) {

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
                    return;
                }
            
                prev = temp;
                temp = temp->next;
            }
        }

        void mostrarTodasInteracciones(string accion, bool eleccion) {
            mostrarRecursivo(head, 1, accion, eleccion);
        }

};
