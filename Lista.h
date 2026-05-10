#pragma once
#include <iostream>
#include "Cliente.h"

using namespace std;

// ============================================================
// CLASS: Node<T> y ListaSimple<T>
// Lista enlazada simple generica - Modulo de registro/listado
// ============================================================
template <typename T>
class Node {
public:
    T        data;
    Node<T>* next;
    Node<T>* prev;

    Node(T data) {
        this->data = data;
        this->next = nullptr;
        this->prev = nullptr;
    }
};

template <typename T>
class ListaSimple {
private:
    Node<T>* head;
    int      length;

    Node<T>* NodeAt(int pos) {
        if (pos >= length || pos < 0) return nullptr;
        int      index = 0;
        Node<T>* aux = head;
        while (index < pos) {
            aux = aux->next;
            index++;
        }
        return aux;
    }

    // RECURSIVIDAD - privado
    void PrintRecursive(Node<T>* nodo, int numero) {
        if (nodo == nullptr) return;              // CASO BASE
        cout << "\n  [Cliente #" << numero << "]" << endl;
        nodo->data.mostrar();
        PrintRecursive(nodo->next, numero + 1);   // LLAMADA RECURSIVA
    }

    int CountRecursive(Node<T>* nodo) {
        if (nodo == nullptr) return 0;            // CASO BASE
        return 1 + CountRecursive(nodo->next);    // LLAMADA RECURSIVA
    }

public:
    ListaSimple() : head(nullptr), length(0) {}

    ~ListaSimple() {
        Node<T>* aux = head;
        while (aux != nullptr) {
            Node<T>* nextNode = aux->next;
            delete aux;
            aux = nextNode;
        }
    }

    void AddLast(T data) {
        Node<T>* node = new Node<T>(data);
        if (length == 0) {
            head = node;
        }
        else {
            Node<T>* nodeLast = NodeAt(length - 1);
            nodeLast->next = node;
        }
        length++;
    }

    void AddFirst(T data) {
        Node<T>* node = new Node<T>(data);
        node->next = head;
        head = node;
        length++;
    }

    int Length() { return length; }

    T GetPos(int pos) {
        Node<T>* node = NodeAt(pos);
        return node->data;
    }

    void ModifyPos(T data, int pos) {
        Node<T>* currentNode = NodeAt(pos);
        if (currentNode != nullptr) {
            currentNode->data = data;
        }
    }

    void RemoveFirst() {
        if (length == 0) return;
        Node<T>* aux = head;
        head = head->next;
        delete aux;
        length--;
    }

    void RemovePos(int pos) {
        if (length == 0 || pos < 0 || pos >= length) return;
        if (pos == 0) {
            RemoveFirst();
        }
        else {
            Node<T>* nodeBefore = NodeAt(pos - 1);
            Node<T>* nodeErase = NodeAt(pos);
            if (nodeBefore != nullptr && nodeErase != nullptr) {
                nodeBefore->next = nodeErase->next;
                delete nodeErase;
                length--;
            }
        }
    }

    // Metodos recursivos publicos
    void PrintRecursive() { PrintRecursive(head, 1); }
    int  CountRecursive() { return CountRecursive(head); }

    bool IsEmpty() { return head == nullptr; }

    Node<T>* GetHead() { return head; }
};