#ifndef URGENCIAS_CPP
#define URGENCIAS_CPP

#include <iostream>
#include <string>
// ==========================================
// 1. CLASE PACIENTE (Modelo de Datos)
// ==========================================
class Paciente {
private:
    std::string id;
    std::string nombre;
    std::string motivo;
    int prioridad; // De 1 (más urgente) a 5 (menos urgente)
    static int nextId; // Contador estático para IDs únicos

public:
    Paciente(const std::string& nombre = "", const std::string& motivo = "", int prioridad = 5)
        : nombre(nombre), motivo(motivo), prioridad(prioridad) {
        // Formatear ID único incremental (ej: PAC_1, PAC_2...)
        this->id = "PAC_" + std::to_string(nextId++);
    }

    // Deshabilitar constructor de copia y operador de asignación
    // para evitar la duplicidad accidental de pacientes en el sistema
    Paciente(const Paciente&) = delete;
    Paciente& operator=(const Paciente&) = delete;

    // Getters
    std::string getId() const { return id; }
    std::string getNombre() const { return nombre; }
    std::string getMotivo() const { return motivo; }
    int getPrioridad() const { return prioridad; }

    // Setters
    void setNombre(const std::string& nombre) { this->nombre = nombre; }
    void setMotivo(const std::string& motivo) { this->motivo = motivo; }
    void setPrioridad(int prioridad) {
        if (prioridad >= 1 && prioridad <= 5) {
            this->prioridad = prioridad;
        } else {
            this->prioridad = 5; // Valor por defecto ante entradas erróneas
        }
    }

    // Mostrar información
    void mostrarInfo() const {
        std::cout << "[" << id << "] " << nombre
                  << " | Prioridad: " << prioridad
                  << " | Motivo: " << motivo << std::endl;
    }
};

// Inicialización del contador estático
int Paciente::nextId = 1;

// ==========================================
// 2. NODO DE ENLACE (Estructura Lineal)
// ==========================================
struct NodoPaciente {
    Paciente* paciente; // Puntero al objeto real en memoria
    NodoPaciente* next;

    NodoPaciente(Paciente* p) : paciente(p), next(nullptr) {}
};

// ==========================================
// 3. COLA ENLAZADA DINÁMICA (FIFO)
// ==========================================
class ColaEnlazada {
private:
    NodoPaciente* front;
    NodoPaciente* rear;
    int currentSize;

public:
    ColaEnlazada() : front(nullptr), rear(nullptr), currentSize(0) {}

    // Deshabilitar constructor de copia y operador de asignación
    // para evitar copias superficiales (shallow copies) de los nodos de la cola
    ColaEnlazada(const ColaEnlazada&) = delete;
    ColaEnlazada& operator=(const ColaEnlazada&) = delete;

    // El destructor libera los nodos de la estructura,
    // pero NO borra al Paciente real ya que este se traslada al Historial.
    ~ColaEnlazada() {
        clear();
    }

    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    bool isEmpty() const {
        return front == nullptr;
    }

    int size() const {
        return currentSize;
    }

    // Insertar paciente al final (FIFO)
    void enqueue(Paciente* p) {
        if (p == nullptr) return;
        NodoPaciente* nuevo = new NodoPaciente(p);
        if (isEmpty()) {
            front = rear = nuevo;
        } else {
            rear->next = nuevo;
            rear = nuevo;
        }
        currentSize++;
    }

    // Extraer paciente del frente (FIFO)
    Paciente* dequeue() {
        if (isEmpty()) return nullptr;
        NodoPaciente* temp = front;
        Paciente* p = temp->paciente;

        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        }

        delete temp; // Libera el nodo enlace
        currentSize--;
        return p; // Devuelve el puntero al paciente
    }

    // Mostrar todos los pacientes en esta cola
    void mostrarCola() const {
        NodoPaciente* temp = front;
        while (temp != nullptr) {
            std::cout << "  - ";
            temp->paciente->mostrarInfo();
            temp = temp->next;
        }
    }
};

// ==========================================
// 4. LISTA DE ESPERA (Array Estático de Colas)
// ==========================================
class ListaEspera {
private:
    ColaEnlazada colas[5]; // colas[0] = Prioridad 1, ..., colas[4] = Prioridad 5

public:
    ListaEspera() {}

    // Deshabilitar copia para prevenir la clonación accidental del sistema de triaje
    ListaEspera(const ListaEspera&) = delete;
    ListaEspera& operator=(const ListaEspera&) = delete;

    // Liberación segura de la memoria de todos los pacientes en espera al salir del programa
    ~ListaEspera() {
        for (int i = 0; i < 5; ++i) {
            while (!colas[i].isEmpty()) {
                Paciente* p = colas[i].dequeue();
                delete p; // Libera el paciente que nunca fue atendido
            }
        }
    }

    // Insertar paciente en su nivel de prioridad correspondiente (Triaje)
    void insertarPaciente(Paciente* p) {
        if (p == nullptr) return;
        int idx = p->getPrioridad() - 1;
        if (idx >= 0 && idx < 5) {
            colas[idx].enqueue(p);
        } else {
            colas[4].enqueue(p); // Por defecto prioridad 5 si el índice falla
        }
    }

    // Atender al paciente de mayor prioridad (Triage de Manchester)
    Paciente* atenderSiguiente() {
        // Busca desde la prioridad 1 (colas[0]) hasta la 5 (colas[4])
        for (int i = 0; i < 5; ++i) {
            if (!colas[i].isEmpty()) {
                return colas[i].dequeue();
            }
        }
        return nullptr; // Ningún paciente en espera
    }

    // Mostrar el estado de todas las colas de urgencias
    // Mostrar el estado de todas las colas de urgencias con umbral de alerta opcional
    void mostrarListaEspera(int umbral = 5) const {
        std::cout << "\n=============================================" << std::endl;
        std::cout << "||     LISTA DE ESPERA - ZONA DE URGENCIAS ||" << std::endl;
        std::cout << "=============================================" << std::endl;
        for (int i = 0; i < 5; ++i) {
            int totalCola = colas[i].size();
            std::cout << "Nivel " << (i + 1) << " [Prioridad " << (i + 1)
                      << "] (Total: " << totalCola << ")";
            if (totalCola >= umbral) {
                std::cout << " [⚠️ SATURACION]";
            }
            std::cout << ":" << std::endl;
            if (colas[i].isEmpty()) {
                std::cout << "  (Cola vacia)" << std::endl;
            } else {
                colas[i].mostrarCola();
            }
        }
        std::cout << "=============================================" << std::endl;
    }
};

#endif // URGENCIAS_CPP
