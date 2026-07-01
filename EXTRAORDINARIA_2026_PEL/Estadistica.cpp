// ============================================================================
// REVISIÓN Y RECOMENDACIONES DE INTEGRACIÓN (POR FAVOR REVISAR):
// 1. BUG LÓGICO EN registrarPacienteAtendido: Si la prioridad es inválida, se
//    imprime un mensaje de error pero se incrementa igualmente el contador 
//    "Total pacientes atendidos". Debe ir dentro del condicional "if".
// 2. GESTIÓN DE MEMORIA: Deshabilitar el constructor de copia y operador de 
//    asignación con "= delete" para evitar Double Frees / caídas del programa.
// 3. INCOMPATIBILIDAD CON pruebaMenu.cpp:
//    - El menú busca la clase con el nombre "Estadisticas", no "EstadisticasConfiguracion".
//    - El menú llama a "registrarPaciente(Paciente* p)" en lugar de "registrarPacienteAtendido(int)".
//    Se sugiere renombrar la clase a "Estadisticas" e implementar el método puente:
//    void registrarPaciente(Paciente* p) { if (p != nullptr) registrarPacienteAtendido(p->getPrioridad()); }
// 4. ESPACIO DE NOMBRES: Evitar "using namespace std;" a nivel global del archivo, 
//    ya que al incluirse directamente mediante #include contamina el main.cpp.
// ============================================================================

#ifndef ESTADISTICAS_CPP
#define ESTADISTICAS_CPP

#include <iostream>
#include <string>
#include "Urgencias.cpp"


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

class Estadisticas {
private:
    NodoEstadistica* cabeza;

public:
    Estadisticas() {
        cabeza = nullptr;

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

    // Deshabilitar copia para prevenir la duplicidad de nodos y doble liberación
    Estadisticas(const Estadisticas&) = delete;
    Estadisticas& operator=(const Estadisticas&) = delete;

    ~Estadisticas() {
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
        if (prioridad >= 1 && prioridad <= 5) {
            incrementar("Total pacientes atendidos");
            string nombrePrioridad = "Pacientes prioridad " + to_string(prioridad);
            incrementar(nombrePrioridad);
        } else {
            cout << "Prioridad no valida. Debe estar entre 1 y 5." << endl;
        }
    }

    void registrarPaciente(Paciente* p) {
        if (p != nullptr) {
            registrarPacienteAtendido(p->getPrioridad());
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

    void establecerValor(string nombre, int nuevoValor) {
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
};

#endif // ESTADISTICAS_CPP