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
                nuevo -> prev = temp; 
                temp -> next = nuevo;
            }
            cout << "Usuario registrado exitosamente...\n";
        }

            //     ListaClientes<Cliente> listaDoble;     //Lista doble enlazada - ordenar clientes
    // PilaAcciones<string>   pilaAcciones;
        void registrarVenta(int codigo_usuario, int cantidadVendida) {

            Nodo<T>* temp = head; //esto no apunta a un objeto cliente, sino a un nodo que contiene un cliente.

            while (temp != nullptr) {

                if (temp->data.codigo_usuario == codigo_usuario) {

                    Venta v(cantidadVendida); //creamos la instancia de venta e inicializamos con la cantidad vendida
                    Nodo<Venta>* nueva_venta = new Nodo<Venta>(v); //aqui guardamos la instancia de la venta en un nodo

                    if (temp->data.ventas == nullptr) { //temp es un nodo que apunta a un nodo que contiene un cliente como data. Es que se puede acceder de esta forma
                        temp->data.ventas = nueva_venta; //... aqui se puede hacer esto por que si es la primera venta, entonces
                        //ventas = nullptr lo defines en el constructor. Ahora le estas asginandio un puntero nueva_venta es decir un nodo que tiene como tipo de dato una Venta

                    } else {
                        //aqui data.ventas es un puntero a nodo de venta, y es el primer nodo de venta del cliente encontrado
                        Nodo<Venta>* aux = temp->data.ventas; //aux es un nodo que contiene a T data y como es template ese T data es un Venta* ventas
                        //ventas es un puntero a nodo de venta, entonces aux apunta al primer nodo de venta del cliente encontrado
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
            //opcion : ventas, codigo_usuario y nombre
            //bubble sort -> comprar elementos adyacentes y cambiarlos de lugar. 
            // class Cliente {
            // public:
            //     string nombre_empresa;
            //     int codigo_usuario;
            //     Nodo<Venta>* ventas;

            //     Cliente(string nombre, int codigo) : nombre_empresa(nombre), codigo_usuario(codigo), ventas(nullptr) {}
            // };
            if(head == nullptr) return;

            bool swapped;
            //garantiza que al menos entre a evaluar una vez
            do{
                swapped = false;
                Nodo<T>* temp = head;

                while(temp -> next != nullptr) {
                    if(temp -> data.opcion > temp -> next -> data.opcion) {
                        swap(temp->data, temp->next->data);
                        swapped = true;
                    }
                    temp = temp->next;
                }
            }while(swapped)
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