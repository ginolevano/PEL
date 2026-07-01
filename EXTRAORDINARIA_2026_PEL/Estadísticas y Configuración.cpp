#include <iostream>
#include <string>
using namespace std;

class NodoEstadistica {
public:
    string nombre;
    int valor;
    NodoEstadistica* siguiente;

    NodoEstadistica(string nombre, int valor) {
        this->nombre = nombre;
        this->valor = valor;
        this->siguiente = nullptr;
    }
};

class EstadisticasConfiguracion {
private:
    NodoEstadistica* cabeza;

public:
    EstadisticasConfiguracion() {
        cabeza = nullptr;

        agregarEstadistica("Total pacientes atendidos", 0);
        agregarEstadistica("Pacientes prioridad 1", 0);
        agregarEstadistica("Pacientes prioridad 2", 0);
        agregarEstadistica("Pacientes prioridad 3", 0);
        agregarEstadistica("Pacientes prioridad 4", 0);
        agregarEstadistica("Pacientes prioridad 5", 0);
    }

    ~EstadisticasConfiguracion() {
        NodoEstadistica* actual = cabeza;

        while (actual != nullptr) {
            NodoEstadistica* borrar = actual;
            actual = actual->siguiente;
            delete borrar;
        }

        cabeza = nullptr;
    }

    void agregarEstadistica(string nombre, int valor) {
        NodoEstadistica* nuevo = new NodoEstadistica(nombre, valor);

        if (cabeza == nullptr) {
            cabeza = nuevo;
        } else {
            NodoEstadistica* actual = cabeza;

            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }

            actual->siguiente = nuevo;
        }
    }

    void incrementar(string nombre) {
        NodoEstadistica* actual = cabeza;

        while (actual != nullptr) {
            if (actual->nombre == nombre) {
                actual->valor++;
                return;
            }

            actual = actual->siguiente;
        }
    }

    void registrarPacienteAtendido(int prioridad) {
        incrementar("Total pacientes atendidos");

        if (prioridad >= 1 && prioridad <= 5) {
            string nombrePrioridad = "Pacientes prioridad " + to_string(prioridad);
            incrementar(nombrePrioridad);
        } else {
            cout << "Prioridad no valida. Debe estar entre 1 y 5." << endl;
        }
    }

    void mostrarEstadisticas() const {
        cout << "\n===== ESTADISTICAS Y CONFIGURACION =====" << endl;

        NodoEstadistica* actual = cabeza;

        while (actual != nullptr) {
            cout << actual->nombre << ": " << actual->valor << endl;
            actual = actual->siguiente;
        }

        cout << "========================================" << endl;
    }

    int obtenerValor(string nombre) const {
        NodoEstadistica* actual = cabeza;

        while (actual != nullptr) {
            if (actual->nombre == nombre) {
                return actual->valor;
            }

            actual = actual->siguiente;
        }

        return -1;
    }

    void reiniciarEstadisticas() {
        NodoEstadistica* actual = cabeza;

        while (actual != nullptr) {
            actual->valor = 0;
            actual = actual->siguiente;
        }

        cout << "Estadisticas reiniciadas correctamente." << endl;
    }
};