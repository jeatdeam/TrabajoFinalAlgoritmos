#pragma once
#pragma once
#include <iostream>
#include "Cliente.h"

using namespace std;

// ============================================================
// CLASS: NodoCircular<T> y ListaCircular<T>
// Lista circular generica - Modulo de busqueda/historial
template <typename T>
class NodoCircular {
public:
    T                data;
    NodoCircular<T>* next;

    NodoCircular(T data) {
        this->data = data;
        this->next = nullptr;
    }
};

template <typename T>
class ListaCircular {
private:
    NodoCircular<T>* ultimo;

    // RECURSIVIDAD privada
    template <typename Criterio>
    NodoCircular<T>* buscarRecursivo(NodoCircular<T>* actual,
        NodoCircular<T>* inicio,
        Criterio criterio) {
        if (actual == nullptr) return nullptr;
        if (criterio(actual->data)) return actual;
        if (actual->next == inicio) return nullptr;
        return buscarRecursivo(actual->next, inicio, criterio);
    }

public:
    ListaCircular() { ultimo = nullptr; }

    // Metodo 1: insertar al final
    void insertarFinal(T dato) {
        NodoCircular<T>* nuevo = new NodoCircular<T>(dato);
        if (ultimo == nullptr) {
            ultimo = nuevo;
            ultimo->next = ultimo;
        }
        else {
            nuevo->next = ultimo->next;
            ultimo->next = nuevo;
            ultimo = nuevo;
        }
    }

    // Metodo 2: buscar con lambda y recursividad
    template <typename Criterio>
    NodoCircular<T>* buscar(Criterio criterio) {
        if (ultimo == nullptr) return nullptr;
        NodoCircular<T>* inicio = ultimo->next;
        return buscarRecursivo(inicio, inicio, criterio);
    }

    // Metodo 3: mostrar con lambda
    template <typename Mostrar>
    void mostrar(Mostrar mostrarDato) {
        if (ultimo == nullptr) {
            cout << "\nNo hay clientes registrados.\n";
            return;
        }
        NodoCircular<T>* inicio = ultimo->next;
        NodoCircular<T>* actual = inicio;
        do {
            mostrarDato(actual->data);
            actual = actual->next;
        } while (actual != inicio);
    }

    bool estaVacia() { return ultimo == nullptr; }

    NodoCircular<T>* getUltimo() { return ultimo; }
};


// CLASS: Cola<T>
// Cola generica - usada para historial de ventas (FIFO)
template <typename T>
class NodoCola {
public:
    T            data;
    NodoCola<T>* next;

    NodoCola(T data) {
        this->data = data;
        this->next = nullptr;
    }
};

template <typename T>
class Cola {
private:
    NodoCola<T>* frente;
    NodoCola<T>* final_;

public:
    Cola() : frente(nullptr), final_(nullptr) {}

    // Metodo 1: encolar
    void encolar(T dato) {
        NodoCola<T>* nuevo = new NodoCola<T>(dato);
        if (frente == nullptr) {
            frente = nuevo;
            final_ = nuevo;
        }
        else {
            final_->next = nuevo;
            final_ = nuevo;
        }
    }

    // Metodo 2: desencolar
    bool desencolar(T& dato) {
        if (frente == nullptr) return false;
        NodoCola<T>* eliminar = frente;
        dato = frente->data;
        frente = frente->next;
        if (frente == nullptr) final_ = nullptr;
        delete eliminar;
        return true;
    }

    // Metodo 3: mostrar con lambda
    template <typename Mostrar>
    void mostrar(Mostrar mostrarDato) {
        NodoCola<T>* actual = frente;
        while (actual != nullptr) {
            mostrarDato(actual->data);
            actual = actual->next;
        }
    }

    bool estaVacia() { return frente == nullptr; }
};