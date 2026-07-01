#ifndef HISTORIAL_PACIENTES_CPP
#define HISTORIAL_PACIENTES_CPP

#include <iostream>
#include <string>
#include "Urgencias.cpp"

// ==========================================
// ESTRUCTURA DINÁMICA 1: HISTORIAL DE PACIENTES
// (Vector Dinámico de Punteros con Redimensionamiento Contiguo)
// ==========================================
class HistorialPacientes {
private:
    Paciente** historial; // Array dinámico de punteros a Paciente
    int capacidad;        // Capacidad actual del array
    int cantidad;         // Número actual de pacientes en el historial

    // Método privado para redimensionar el historial duplicando la capacidad
    void ampliarCapacidad() {
        int nuevaCapacidad = capacidad * 2;
        Paciente** nuevo = new Paciente*[nuevaCapacidad];

        // Copiar los punteros del antiguo array al nuevo
        for (int i = 0; i < cantidad; i++) {
            nuevo[i] = historial[i];
        }

        // Liberar la memoria del array de punteros antiguo (NO de los pacientes en sí)
        delete[] historial;

        historial = nuevo;
        capacidad = nuevaCapacidad;
        std::cout << "[SISTEMA] Historial redimensionado de forma contigua. Nueva capacidad: " << capacidad << std::endl;
    }

public:
    // Constructor con capacidad inicial parametrizable para lectura de configuración
    HistorialPacientes(int capInicial = 5) {
        capacidad = capInicial;
        cantidad = 0;
        historial = new Paciente*[capacidad];
    }

    // Deshabilitar constructor de copia y operador de asignación
    HistorialPacientes(const HistorialPacientes&) = delete;
    HistorialPacientes& operator=(const HistorialPacientes&) = delete;

    // El destructor se encarga de liberar la memoria de todos los pacientes en el Heap
    ~HistorialPacientes() {
        for (int i = 0; i < cantidad; i++) {
            delete historial[i];
        }
        delete[] historial;
    }

    // Añadir paciente atendido
    void anadirPaciente(Paciente* paciente) {
        if (paciente == nullptr)
            return;

        if (cantidad == capacidad) {
            ampliarCapacidad();
        }

        historial[cantidad] = paciente;
        cantidad++;
    }

    // Mostrar el listado de pacientes atendidos con formato claro
    void mostrarHistorial() const {
        std::cout << "\n=============================================" << std::endl;
        std::cout << "||      HISTORIAL DE PACIENTES ATENDIDOS   ||" << std::endl;
        std::cout << "=============================================" << std::endl;

        if (cantidad == 0) {
            std::cout << "  (Historial vacio)" << std::endl;
            std::cout << "=============================================" << std::endl;
            return;
        }

        for (int i = 0; i < cantidad; i++) {
            std::cout << "  [" << i << "] ";
            historial[i]->mostrarInfo();
        }

        std::cout << "=============================================" << std::endl;
    }

    // Consultar información detallada de un paciente por su índice
    void mostrarPacientePorIndice(int indice) const {
        if (indice < 0 || indice >= cantidad) {
            std::cout << "Indice de consulta incorrecto. Rango valido: [0 - " << cantidad - 1 << "]" << std::endl;
            return;
        }
        std::cout << "\nConsulta detallada por indice [" << indice << "]:" << std::endl;
        std::cout << "  - ";
        historial[indice]->mostrarInfo();
    }

    int getCantidad() const {
        return cantidad;
    }
};

#endif // HISTORIAL_PACIENTES_CPP