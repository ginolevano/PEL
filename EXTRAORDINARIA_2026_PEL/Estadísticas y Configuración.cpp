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