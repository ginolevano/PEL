#ifndef ESTADISTICAS_CPP
#define ESTADISTICAS_CPP

#include <iostream>
#include <string>
#include "Urgencias.cpp"

class NodoEstadistica {
public:
    std::string nombre;
    int valor;
    NodoEstadistica* siguiente;

    NodoEstadistica(const std::string& nombre, int valor)
        : nombre(nombre), valor(valor), siguiente(nullptr) {}
};

class Estadisticas {
private:
    NodoEstadistica* cabeza;

public:
    Estadisticas() : cabeza(nullptr) {
        agregarEstadistica("Total pacientes atendidos", 0);
        agregarEstadistica("Pacientes prioridad 1", 0);
        agregarEstadistica("Pacientes prioridad 2", 0);
        agregarEstadistica("Pacientes prioridad 3", 0);
        agregarEstadistica("Pacientes prioridad 4", 0);
        agregarEstadistica("Pacientes prioridad 5", 0);

        // --- CONFIGURACIÓN DE LA HERRAMIENTA ---
        agregarEstadistica("ID de la Unidad de Combate", 101);
        agregarEstadistica("Umbral de Alerta de Espera", 5);
        agregarEstadistica("Capacidad Inicial de Historial", 2);
    }

    Estadisticas(const Estadisticas&) = delete;
    Estadisticas& operator=(const Estadisticas&) = delete;

    ~Estadisticas() {
        NodoEstadistica* actual = cabeza;
        while (actual != nullptr) {
            NodoEstadistica* borrar = actual;
            actual = actual->siguiente;
            delete borrar;
        }
    }

    void registrarPaciente(Paciente* p) {
        if (p != nullptr) {
            registrarPacienteAtendido(p->getPrioridad());
        }
    }

    void registrarPacienteAtendido(int prioridad) {
        if (prioridad >= 1 && prioridad <= 5) {
            incrementar("Total pacientes atendidos");
            std::string nombrePrioridad = "Pacientes prioridad " + std::to_string(prioridad);
            incrementar(nombrePrioridad);
        } else {
            std::cout << "Prioridad no valida. Debe estar entre 1 y 5." << std::endl;
        }
    }

    void mostrarEstadisticas() const {
        std::cout << "\n===== ESTADISTICAS Y CONFIGURACION =====" << std::endl;

        NodoEstadistica* actual = cabeza;
        while (actual != nullptr) {
            std::cout << actual->nombre << ": " << actual->valor << std::endl;
            actual = actual->siguiente;
        }

        std::cout << "========================================" << std::endl;
    }

    int obtenerValor(const std::string& nombre) const {
        NodoEstadistica* actual = cabeza;
        while (actual != nullptr) {
            if (actual->nombre == nombre) {
                return actual->valor;
            }
            actual = actual->siguiente;
        }
        return -1;
    }

    void establecerValor(const std::string& nombre, int nuevoValor) {
        NodoEstadistica* actual = cabeza;
        while (actual != nullptr) {
            if (actual->nombre == nombre) {
                actual->valor = nuevoValor;
                return;
            }
            actual = actual->siguiente;
        }
    }

    void reiniciarEstadisticas() {
        NodoEstadistica* actual = cabeza;
        while (actual != nullptr) {
            // Solo reiniciar contadores de estadísticas, no las configuraciones de la herramienta
            if (actual->nombre.rfind("Total", 0) == 0 || actual->nombre.rfind("Pacientes", 0) == 0) {
                actual->valor = 0;
            }
            actual = actual->siguiente;
        }
        std::cout << "Estadisticas de pacientes reiniciadas correctamente." << std::endl;
    }

private:
    void agregarEstadistica(const std::string& nombre, int valor) {
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

    void incrementar(const std::string& nombre) {
        NodoEstadistica* actual = cabeza;
        while (actual != nullptr) {
            if (actual->nombre == nombre) {
                actual->valor++;
                return;
            }
            actual = actual->siguiente;
        }
    }
};

#endif // ESTADISTICAS_CPP