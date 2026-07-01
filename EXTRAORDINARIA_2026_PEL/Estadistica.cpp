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
#include "Urgencias.cpp"
#include "MenuSistema.cpp"

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

    void registrarPaciente(Paciente * p) {
        if (p != nullptr) {
            registrarPacienteAtendido(p->getPrioridad());
        }
    }

    void registrarPacienteAtendido(int prioridad) {
        if (prioridad >= 1 && prioridad <= 5) {
            incrementar("Total pacientes atendidos");

            std::string nombrePrioridad =
                "Pacientes prioridad " + std::to_string(prioridad);

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

    void reiniciarEstadisticas() {
        NodoEstadistica* actual = cabeza;
        while (actual != nullptr) {
            actual->valor = 0;
            actual = actual->siguiente;
        }

        std::cout << "Estadisticas reiniciadas correctamente." << std::endl;
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