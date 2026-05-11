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


         void registrarVenta(int codigo_usuario, int cantidadVendida, Nodo<Cliente>* cliente) {

            Nodo<T>* temp = head; //esto no apunta a un objeto cliente, sino a un nodo que contiene un cliente.

            while (temp != nullptr) {

                if (temp->data.codigo_usuario == codigo_usuario) {

                    Venta v(cantidadVendida); 
                    Nodo<Venta>* nueva_venta = new Nodo<Venta>(v); 
                    if (temp->data.ventas == nullptr) { 
                        temp->data.ventas = nueva_venta; 

                    } else {
                        
                        Nodo<Venta>* aux = temp->data.ventas;
                        while (aux->next != nullptr) {
                            aux = aux->next;
                        }

                        aux->next = nueva_venta;
                        nueva_venta->prev = aux;
                    }

                    cout << "Se ha registrado una nueva venta...\n";
                    return;
                }

                temp = temp->next;
    }
        cout << "Cliente no encontrado...\n";
    }


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
