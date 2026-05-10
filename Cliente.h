#pragma once
#include <string>
#include <iostream>

using namespace std;

// ============================================================
// CLASS: Venta
// Representa una venta registrada para un cliente
// ============================================================
template <typename T>
class Nodo {
public:
    T        data;
    Nodo<T>* next;

    Nodo(T data) {
        this->data = data;
        this->next = nullptr;
    }
};

class Venta {
public:
    int monto;

    Venta(int m = 0) {
        monto = m;
    }
};

// ============================================================
// CLASS: Cliente
// Representa una cuenta empresarial en el CRM (Salesforce)
// ============================================================
class Cliente {
public:
    string       id;
    string       nombre_empresa;
    string       correo;
    string       telefono;
    int          codigo_usuario;
    int          activo;     // 1 = Activo, 0 = Inactivo
    int          prioridad;  // 1 = Normal, 2 = VIP, 3 = Prioritario
    Nodo<Venta>* ventas;
    Nodo<string>* interacciones;

    Cliente(string id = "", string nombre = "", string correo = "",
        string telefono = "", int codigo = 0,
        int activo = 1, int prioridad = 1) {
        this->id = id;
        this->nombre_empresa = nombre;
        this->correo = correo;
        this->telefono = telefono;
        this->codigo_usuario = codigo;
        this->activo = activo;
        this->prioridad = prioridad;
        this->ventas = nullptr;
        this->interacciones = nullptr;
    }

    void mostrar() const {
        string etiqueta;
        if (prioridad == 3) etiqueta = "[*** PRIORITARIO ***]";
        else if (prioridad == 2) etiqueta = "[** VIP **]";
        else                     etiqueta = "[Normal]";

        string estado = (activo == 1) ? "ACTIVO" : "INACTIVO";

        cout << "  +------------------------------------------+" << endl;
        cout << "  | ID       : " << id << endl;
        cout << "  | Nombre   : " << nombre_empresa << endl;
        cout << "  | Correo   : " << correo << endl;
        cout << "  | Telefono : " << telefono << endl;
        cout << "  | Codigo   : " << codigo_usuario << endl;
        cout << "  | Estado   : " << estado << endl;
        cout << "  | Prioridad: " << etiqueta << endl;
        cout << "  +------------------------------------------+" << endl;
    }

    // Serializa para guardar en archivo .txt
    string serialize() const {
        return id + "|" + nombre_empresa + "|" + correo + "|" +
            telefono + "|" + to_string(codigo_usuario) + "|" +
            to_string(activo) + "|" + to_string(prioridad);
    }
};