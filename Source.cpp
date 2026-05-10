#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <functional>
#include "Cliente.h"
#include "Lista.h"
#include "Stack.h"
#include "ListaCircularyCola.h"
#include "ListaDoblementeEnlazada.h"
#include "PilaDeAcciones.h"


using namespace std;

// ============================================================
// CLASS: CRMSystem
// Sistema CRM unificado con los 4 modulos del grupo.
//
// MODULO TU NOMBRE  : [1] Registrar  [2] Listar clientes
// MODULO COMPANERO  : [3] Buscar     [4] Historial de ventas
// ============================================================
class CRMSystem {
private:

    // --- ESTRUCTURAS DE DATOS ---
    ListaSimple<Cliente>  listaClientes;   // Lista simple  - registro/listado
    Stack<Cliente>        pilaRecientes;   // Pila          - ultimos registrados
    ListaCircular<Cliente> listaCircular;  // Lista circular - busqueda/historial
    
    ListaClientes<Cliente> listaDoble;     //Lista doble enlazada - ordenar clientes
    PilaAcciones<string>   pilaAcciones;   // Pila          - historial de acciones

    string filename;
    int    nextId;

    // ---- LAMBDA 1: validarCorreo ----
    // Verifica '@' y dominio valido en el correo
    // Complejidad: O(k)
    function<bool(const string&)> validarCorreo;

    // ---- LAMBDA 2: esActivo ----
    // Filtra clientes activos en el CRM
    // Complejidad: O(1)
    function<bool(const Cliente&)> esActivo;

    // ---- LAMBDA 3: esVIP ----
    // Filtra clientes VIP o Prioritarios
    // Complejidad: O(1)
    function<bool(const Cliente&)> esVIP;

    // ---- Helpers de consola ----
    void printHeader() {
        cout << endl;
        cout << "  +==================================================+" << endl;
        cout << "  |                                                  |" << endl;
        cout << "  |       SALESFORCE CRM  *  2026-1                  |" << endl;
        cout << "  |     Gestion Global de Clientes                   |" << endl;
        cout << "  |                                                  |" << endl;
        cout << "  +==================================================+" << endl;
    }

    void printLine() {
        cout << "  +--------------------------------------------------+" << endl;
    }

    void pause() {
        cout << "\n  Presione ENTER para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    // ---- Bubble Sort para ventas (compañero) ----
    void ordenarVentasBubbleSort(Nodo<Venta>* inicio) {
        if (inicio == nullptr || inicio->next == nullptr) return;
        bool intercambio;
        do {
            intercambio = false;
            Nodo<Venta>* actual = inicio;
            while (actual->next != nullptr) {
                if (actual->data.monto < actual->next->data.monto) {
                    Venta temp = actual->data;
                    actual->data = actual->next->data;
                    actual->next->data = temp;
                    intercambio = true;
                }
                actual = actual->next;
            }
        } while (intercambio);
    }

public:

    CRMSystem(string filename = "clientes.txt") {
        this->filename = filename;
        this->nextId = 1;

        // LAMBDA 1
        validarCorreo = [](const string& correo) -> bool {
            int posAt = -1;
            for (int i = 0; i < (int)correo.size(); i++) {
                if (correo[i] == '@') { posAt = i; break; }
            }
            if (posAt <= 0) return false;
            int posDot = -1;
            for (int i = posAt + 1; i < (int)correo.size(); i++) {
                if (correo[i] == '.') { posDot = i; break; }
            }
            if (posDot < 0 || posDot == (int)correo.size() - 1) return false;
            return true;
            };

        // LAMBDA 2
        esActivo = [](const Cliente& c) -> bool {
            return c.activo == 1;
            };

        // LAMBDA 3
        esVIP = [](const Cliente& c) -> bool {
            return c.prioridad >= 2;
            };

        loadFromFile();
    }

    ~CRMSystem() {
        saveToFile();
    }

    // =========================================================
    // MODULO 1: REGISTRAR CLIENTE (tu modulo)
    // Complejidad: O(n) AddLast + O(k) validacion correo
    // =========================================================
    void registrarCliente() {
        clearScreen();
        printHeader();
        cout << "\n  [ NUEVO REGISTRO DE CLIENTE ]" << endl;
        printLine();

        string nombre, correo, telefono;
        int    prioridad, activo, codigo;

        cout << "\n  Nombre empresa : ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, nombre);

        // Validacion con LAMBDA 1
        do {
            cout << "  Correo         : ";
            getline(cin, correo);
            if (!validarCorreo(correo))
                cout << "  [!] Correo invalido. Ej: nombre@empresa.com" << endl;
        } while (!validarCorreo(correo));

        cout << "  Telefono       : ";
        getline(cin, telefono);

        cout << "  Codigo usuario : ";
        cin >> codigo;

        cout << "  Estado (1=Activo, 0=Inactivo)           : ";
        cin >> activo;
        if (activo != 0) activo = 1;

        cout << "  Prioridad (1=Normal, 2=VIP, 3=Prior.)   : ";
        cin >> prioridad;
        if (prioridad < 1 || prioridad > 3) prioridad = 1;

        string id = "CL" + to_string(nextId++);

        Cliente nuevo(id, nombre, correo, telefono, codigo, activo, prioridad);


        // Agrega a ambas estructuras
        listaClientes.AddLast(nuevo);    // Lista simple
        pilaRecientes.Push(nuevo);       // Pila de recientes
        listaCircular.insertarFinal(nuevo); // Lista circular (para busqueda)

        //Agregar a lista doble enlazada
        listaDoble.registrarCliente(nuevo);


        saveToFile();

        cout << "\n  [OK] Cliente registrado. ID: " << id << endl;
        pause();
    }

    // =========================================================
    // MODULO 2: LISTAR CLIENTES (tu modulo)
    // =========================================================

    // Iterativo - Complejidad: O(n)
    void listarClientes() {
        clearScreen();
        printHeader();
        cout << "\n  [ LISTA DE CLIENTES REGISTRADOS ]" << endl;
        printLine();

        if (listaClientes.IsEmpty()) {
            cout << "\n  (No hay clientes registrados)" << endl;
            pause();
            return;
        }

        for (int i = 0; i < listaClientes.Length(); i++) {
            cout << "\n  [Cliente #" << i + 1 << "]" << endl;
            listaClientes.GetPos(i).mostrar();
        }
        cout << "\n  Total: " << listaClientes.Length() << " clientes" << endl;
        pause();
    }

    // Recursivo - Complejidad: O(n)
    void listarClientesRecursivo() {
        clearScreen();
        printHeader();
        cout << "\n  [ LISTA RECURSIVA DE CLIENTES ]" << endl;
        printLine();
        cout << "  (Recorrido recursivo sobre lista enlazada)" << endl;

        if (listaClientes.IsEmpty()) {
            cout << "\n  (No hay clientes registrados)" << endl;
            pause();
            return;
        }

        listaClientes.PrintRecursive();
        cout << "\n  Total (recursivo): "
            << listaClientes.CountRecursive() << " clientes" << endl;
        pause();
    }

    // Clientes activos con LAMBDA 2 - Complejidad: O(n)
    void listarClientesActivos() {
        clearScreen();
        printHeader();
        cout << "\n  [ CLIENTES ACTIVOS ]" << endl;
        printLine();

        int count = 0;
        for (int i = 0; i < listaClientes.Length(); i++) {
            Cliente c = listaClientes.GetPos(i);
            if (esActivo(c)) {   // LAMBDA 2
                c.mostrar();
                count++;
            }
        }
        if (count == 0) cout << "\n  (No hay clientes activos)" << endl;
        else cout << "\n  Total activos: " << count << endl;
        pause();
    }

    // Clientes VIP con LAMBDA 3 - Complejidad: O(n)
    void listarClientesVIP() {
        clearScreen();
        printHeader();
        cout << "\n  [ CLIENTES VIP Y PRIORITARIOS ]" << endl;
        printLine();

        int count = 0;
        for (int i = 0; i < listaClientes.Length(); i++) {
            Cliente c = listaClientes.GetPos(i);
            if (esVIP(c)) {   // LAMBDA 3
                c.mostrar();
                count++;
            }
        }
        if (count == 0) cout << "\n  (No hay clientes VIP)" << endl;
        else cout << "\n  Total VIP/Prioritarios: " << count << endl;
        pause();
    }

    // Ultimos registrados con la PILA - Complejidad: O(n)
    void verRecientes() {
        clearScreen();
        printHeader();
        pilaRecientes.ShowRecent(5);
        pause();
    }

    // =========================================================
    // MODULO 3: BUSCAR CLIENTE (compañero)
    // Lista circular + lambdas + recursividad
    // =========================================================
    void buscarCliente() {
        clearScreen();
        printHeader();
        cout << "\n  [ BUSCAR CLIENTE ]" << endl;
        printLine();

        if (listaCircular.estaVacia()) {
            cout << "\n  (No hay clientes registrados)" << endl;
            pause();
            return;
        }

        cout << "\n  1. Buscar por codigo de usuario" << endl;
        cout << "  2. Buscar por nombre de empresa" << endl;
        cout << "  3. Buscar cliente con ventas registradas" << endl;
        cout << "\n  Opcion: ";

        int opcion;
        cin >> opcion;

        NodoCircular<Cliente>* encontrado = nullptr;

        if (opcion == 1) {
            int codigo;
            cout << "\n  Ingrese codigo: ";
            cin >> codigo;

            // LAMBDA buscar por codigo
            auto porCodigo = [codigo](Cliente c) {
                return c.codigo_usuario == codigo;
                };
            encontrado = listaCircular.buscar(porCodigo);

        }
        else if (opcion == 2) {
            string nombre;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n  Ingrese nombre de empresa: ";
            getline(cin, nombre);

            // LAMBDA buscar por nombre
            auto porNombre = [nombre](Cliente c) {
                return c.nombre_empresa == nombre;
                };
            encontrado = listaCircular.buscar(porNombre);

        }
        else if (opcion == 3) {
            // LAMBDA buscar con ventas
            auto conVentas = [](Cliente c) {
                return c.ventas != nullptr;
                };
            encontrado = listaCircular.buscar(conVentas);

        }
        else {
            cout << "\n  [!] Opcion invalida." << endl;
            pause();
            return;
        }

        if (encontrado != nullptr) {
            cout << "\n  [OK] Cliente encontrado:" << endl;
            encontrado->data.mostrar();
            if (encontrado->data.ventas == nullptr)
                cout << "  Ventas: Sin ventas registradas" << endl;
            else
                cout << "  Ventas: Tiene ventas registradas" << endl;
        }
        else {
            cout << "\n  [!] No se encontro ningun cliente." << endl;
        }
        pause();
    }

    // =========================================================
    // MODULO 4: HISTORIAL DE VENTAS (compañero)
    // Lista circular + Cola + Bubble Sort
    // =========================================================
    void historialCliente() {
        clearScreen();
        printHeader();
        cout << "\n  [ HISTORIAL DE VENTAS ]" << endl;
        printLine();

        if (listaCircular.estaVacia()) {
            cout << "\n  (No hay clientes registrados)" << endl;
            pause();
            return;
        }

        int codigo;
        cout << "\n  Ingrese codigo del cliente: ";
        cin >> codigo;

        auto porCodigo = [codigo](Cliente c) {
            return c.codigo_usuario == codigo;
            };

        NodoCircular<Cliente>* encontrado = listaCircular.buscar(porCodigo);

        if (encontrado == nullptr) {
            cout << "\n  [!] Cliente no encontrado." << endl;
            pause();
            return;
        }

        cout << "\n  Cliente: " << encontrado->data.nombre_empresa << endl;
        cout << "  Codigo : " << encontrado->data.codigo_usuario << endl;

        Nodo<Venta>* aux = encontrado->data.ventas;

        if (aux == nullptr) {
            cout << "\n  Este cliente no tiene historial de ventas." << endl;
            pause();
            return;
        }

        cout << "\n  1. Mostrar historial sin ordenar" << endl;
        cout << "  2. Ordenar por monto (Bubble Sort)" << endl;
        cout << "\n  Opcion: ";

        int opcion;
        cin >> opcion;

        if (opcion == 2) {
            ordenarVentasBubbleSort(aux);
            cout << "\n  Historial ordenado de mayor a menor (Bubble Sort)." << endl;
        }

        // Encolar ventas en la Cola para mostrar en FIFO
        Cola<Venta> colaVentas;
        Nodo<Venta>* tmp = encontrado->data.ventas;
        while (tmp != nullptr) {
            colaVentas.encolar(tmp->data);
            tmp = tmp->next;
        }

        cout << "\n  =====================================" << endl;
        cout << "        HISTORIAL DE VENTAS           " << endl;
        cout << "  =====================================" << endl;

        int contador = 1;
        int totalVentas = 0;

        colaVentas.mostrar([&contador, &totalVentas](Venta v) {
            cout << "\n  Venta #" << contador << endl;
            cout << "  Monto: S/ " << v.monto << endl;
            totalVentas += v.monto;
            contador++;
            });

        cout << "\n  -------------------------------------" << endl;
        cout << "  Cantidad de ventas : " << contador - 1 << endl;
        cout << "  Total vendido      : S/ " << totalVentas << endl;
        pause();
    }

    // =========================================================
    // Registrar interaccion
    // =========================================================


                // cout << "----Opciones de interaccion----\n";
                // cout << "1. Registrar venta. \n";
                // cout << "2. Registrar llamada.\n";
                // cout << "3. Registrar email.\n";
                // cout << "4. Eliminar venta.\n";
                // cout << "5. Eliminar llamada.\n";


                // void validarOpcionInteraccion(string &message) {

                    
                // int opcion = 0;

                // cout << message;

                // while(!(cin>>opcion) || opcion < 1 || opcion > 5) {
                //     cout << "\n  [!] Opcion invalida. Ingrese un numero entre 1 y 5: ";
                //     cin.clear();
                //     cin.ignore(1000, '\n');
                //     cout << message;
                // }

                // cout << "Opcion valida: " << opcion << endl;

                // }

    void validarOpcionInteraccion(const string &message, int &opcion) {

        cout << "----Opciones de interaccion----\n";
        cout << "1. Registrar venta. \n";
        cout << "2. Registrar llamada.\n";
        cout << "3. Eliminar venta.\n";
        cout << "4. Eliminar llamada.\n";

        cout << message;

        while(!(cin>>opcion) || opcion < 1 || opcion > 4) {
            cout << "\n  [!] Opcion invalida. Ingrese un numero entre 1 y 5: ";
            cin.clear();
            cin.ignore(1000, '\n');
            cout << message;
        }

        cout << "Opcion valida... " << opcion << endl;

    }

    void registrarInteraccion() {

        int opcion = 0;
        validarOpcionInteraccion("Ingrese la opcion elegida: ", opcion);

        switch (opcion) {
            case 1:
                //venta
                pilaAcciones.registrarInteraccion("venta");
                break;
            case 2:
                //llamada
                pilaAcciones.registrarInteraccion("llamada");
                break;
            case 3:
                pilaAcciones.eliminarInteraccion("venta");
                //eliminar venta
                break;
            case 4:
                pilaAcciones.eliminarInteraccion("llamada");
                //eliminar llamada
                break;
        }

    }


    void validarOpcionOrdenamiento(const string &message, int &opcion) {

        cout << "----Opciones de ordenamiento----\n";
        cout << "1. nombre de empresa. \n";
        cout << "2. codigo de usuario.\n";

        cout << message;

        while(!(cin>>opcion) || opcion < 1 || opcion > 2 {
            cout << "\n  [!] Opcion invalida. Ingrese un numero entre 1 - 2: ";
            cin.clear();
            cin.ignore(1000, '\n');
            cout << message;
        }

        cout << "Opcion valida... " << opcion << endl;
    }


    void ordenarClientes() {

        int opcion = 0;
        validarOpcionOrdenamiento("Ingrese la opcion de ordenamiento: ", opcion);

            switch (opcion) {
                case 1:
                    //ordenar por nombre de empresa
                    listaDoble.ordenarCliente_opcion("nombre");
                    break;
                case 2:
                    //ordenar por codigo de usuario
                    listaDoble.ordenarCliente_opcion("codigo");
                    break;
            }

    }

    // =========================================================
    // Ordenar clientes
    // =========================================================



    // =========================================================
    // HELPERS: guardar y cargar archivo .txt
    // =========================================================
    void saveToFile() {
        ofstream file(filename);
        if (!file.is_open()) return;
        file << "# CRM Salesforce - clientes.txt\n";
        file << "# id|nombre|correo|telefono|codigo|activo|prioridad\n";
        for (int i = 0; i < listaClientes.Length(); i++) {
            file << listaClientes.GetPos(i).serialize() << "\n";
        }
        file.close();
    }

    void loadFromFile() {
        ifstream file(filename);
        if (!file.is_open()) return;

        string linea;
        while (getline(file, linea)) {
            if (linea.empty() || linea[0] == '#') continue;

            string campos[7];
            int    campo = 0;
            string token = "";
            for (char ch : linea) {
                if (ch == '|') { campos[campo++] = token; token = ""; }
                else             token += ch;
            }
            campos[campo] = token;
            if (campo < 6) continue;

            string id = campos[0];
            string nombre = campos[1];
            string correo = campos[2];
            string telefono = campos[3];
            int    codigo = stoi(campos[4]);
            int    activo = stoi(campos[5]);
            int    prior = stoi(campos[6]);

            Cliente c(id, nombre, correo, telefono, codigo, activo, prior);
            listaClientes.AddLast(c);
            listaCircular.insertarFinal(c);

            if (id.size() > 2) {
                int num = stoi(id.substr(2));
                if (num >= nextId) nextId = num + 1;
            }
        }
        file.close();
    }

    // =========================================================
    // ESTADISTICAS - usa lambdas 2 y 3
    // =========================================================
    void mostrarEstadisticas() {
        clearScreen();
        printHeader();
        cout << "\n  [ ESTADISTICAS DEL SISTEMA ]" << endl;
        printLine();

        int total = listaClientes.CountRecursive();
        int activos = 0, vip = 0;

        for (int i = 0; i < listaClientes.Length(); i++) {
            Cliente c = listaClientes.GetPos(i);
            if (esActivo(c)) activos++;
            if (esVIP(c))    vip++;
        }

        cout << "\n  Total registrados    : " << total << endl;
        cout << "  Clientes activos     : " << activos << endl;
        cout << "  Clientes VIP/Prior.  : " << vip << endl;
        cout << "  Recientes en pila    : " << pilaRecientes.Size() << endl;
        cout << "  Archivo              : " << filename << endl;
        printLine();
        cout << "  Estructuras usadas:" << endl;
        cout << "    - ListaSimple<Cliente>   (registro y listado)" << endl;
        cout << "    - Stack<Cliente>         (clientes recientes)" << endl;
        cout << "    - ListaCircular<Cliente> (busqueda/historial)" << endl;
        cout << "    - Cola<Venta>            (historial FIFO)" << endl;
        pause();
    }

    // =========================================================
    // MENU PRINCIPAL
    // =========================================================
    void runMenu() {
        int opcion = 0;
        do {
            clearScreen();
            printHeader();
            cout << "  FLUJO: [Registrar] --> [Buscar] --> [Interaccion] --> [Historial]" << endl;
            cout << "  +--------------------------------------------------+" << endl;
            cout << "  |  REGISTRO Y LISTADO                              |" << endl;
            cout << "  +--------------------------------------------------+" << endl;
            cout << "  |   [1] Registrar cliente                          |" << endl;
            cout << "  |   [2] Listar clientes                            |" << endl;
            cout << "  +--------------------------------------------------+" << endl;
            cout << "  |  BUSQUEDA E HISTORIAL                            |" << endl;
            cout << "  +--------------------------------------------------+" << endl;
            cout << "  |   [3] Buscar cliente                             |" << endl;
            cout << "  |   [4] Mostrar historial de cliente               |" << endl;
            cout << "  +--------------------------------------------------+" << endl;
            cout << "  |  OPERACIONES                                     |" << endl;
            cout << "  +--------------------------------------------------+" << endl;
            cout << "  |   [5] Registrar interaccion  (proximamente...)   |" << endl;
            cout << "  |   [6] Ordenar clientes       (proximamente...)   |" << endl;
            cout << "  +--------------------------------------------------+" << endl;
            cout << "  |   [0] Salir                                      |" << endl;
            cout << "  +--------------------------------------------------+" << endl;
            cout << "\n  Opcion: ";
            cin >> opcion;

            switch (opcion) {
            case 1: registrarCliente();   break;
            case 2: listarClientes();     break;
            case 3: buscarCliente();      break;
            case 4: historialCliente();   break;
            case 5: registrarInteraccion(); break;
            case 6: ordenarClientes(); break;
            case 0: cout << "\n  Cerrando CRM...\n"; break;
            default:
                cout << "\n  [!] Opcion invalida." << endl;
                pause();
            }
        } while (opcion != 0);
    }
};

// ============================================================
// MAIN
// ============================================================
int main() {
    CRMSystem crm("clientes.txt");
    crm.runMenu();
    return 0;
}