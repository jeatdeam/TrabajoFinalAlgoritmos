#pragma once
#include <iostream>

using namespace std;

// ============================================================
// CLASS: Stack<T>
// Pila generica - almacena ultimos clientes registrados (LIFO)
// Similar a "Recent Items" de Salesforce
// ============================================================
template <typename T>
class Stack {
private:
    struct StackNode {
        T          data;
        StackNode* next;
        StackNode(T data) : data(data), next(nullptr) {}
    };

    StackNode* top;
    int        size;

public:
    Stack() : top(nullptr), size(0) {}

    ~Stack() { while (!IsEmpty()) Pop(); }

    void Push(T data) {
        StackNode* node = new StackNode(data);
        node->next = top;
        top = node;
        size++;
    }

    void Pop() {
        if (IsEmpty()) return;
        StackNode* temp = top;
        top = top->next;
        delete temp;
        size--;
    }

    T Peek() { return top->data; }

    bool IsEmpty() { return top == nullptr; }

    int Size() { return size; }

    void ShowRecent(int n) {
        StackNode* current = top;
        int        count = 0;
        cout << "\n  [ ULTIMOS " << n << " CLIENTES REGISTRADOS (PILA) ]" << endl;
        while (current != nullptr && count < n) {
            cout << "\n  [Reciente #" << count + 1 << "]" << endl;
            current->data.mostrar();
            current = current->next;
            count++;
        }
        if (count == 0)
            cout << "  (No hay registros recientes)" << endl;
    }
};