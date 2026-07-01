#ifndef HISTORIAL_CPP
#define HISTORIAL_CPP

#include <iostream>
#include <string>
#include "Urgencias.cpp"

// ==========================================
// ESTRUCTURA DINÁMICA 1: HISTORIAL DE PACIENTES
// (Vector Dinámico de Punteros con Redimensionamiento Contiguo)
// ==========================================
class HistorialPacientes {
private:
    Paciente** pacientes; // Array dinámico de punteros a Paciente
    int capacidad;        // Capacidad actual del array
    int cantidad;         // Número actual de pacientes en el historial

    // Método privado para redimensionar el historial duplicando la capacidad
    void redimensionar() {
        int nuevaCapacidad = capacidad * 2;
        Paciente** nuevoArray = new Paciente*[nuevaCapacidad];

        // Copiar los punteros del antiguo array al nuevo
        for (int i = 0; i < cantidad; ++i) {
            nuevoArray[i] = pacientes[i];
        }

        // Liberar la memoria del array de punteros antiguo (NO de los pacientes en sí)
        delete[] pacientes;

        pacientes = nuevoArray;
        capacidad = nuevaCapacidad;
        std::cout << "[SISTEMA] Historial redimensionado de forma contigua. Nueva capacidad: " << capacidad << std::endl;
    }

public:
    // Constructor con capacidad inicial (por defecto 2 para forzar redimensionamiento rápido)
    HistorialPacientes(int capInicial = 2) {
        capacidad = capInicial;
        cantidad = 0;
        pacientes = new Paciente*[capacidad];
    }

    // El destructor se encarga de liberar la memoria de todos los pacientes
    // que se encuentran en el Heap y del array de punteros
    ~HistorialPacientes() {
        for (int i = 0; i < cantidad; ++i) {
            delete pacientes[i]; // Liberación de la memoria física del paciente
        }
        delete[] pacientes; // Liberación del array de punteros
    }

    // Deshabilitar constructor de copia y operador de asignación
    // para evitar copias accidentales del historial y dobles liberaciones en el destructor
    HistorialPacientes(const HistorialPacientes&) = delete;
    HistorialPacientes& operator=(const HistorialPacientes&) = delete;

    // Añadir paciente atendido
    void anadirPaciente(Paciente* p) {
        if (p == nullptr) return;
        if (cantidad == capacidad) {
            redimensionar();
        }
        pacientes[cantidad++] = p;
    }

    // Mostrar el listado cronológico de pacientes
    void mostrarHistorial() const {
        std::cout << "\n=============================================" << std::endl;
        std::cout << "||      HISTORIAL DE PACIENTES ATENDIDOS   ||" << std::endl;
        std::cout << "=============================================" << std::endl;
        if (cantidad == 0) {
            std::cout << "  (Historial vacio)" << std::endl;
        } else {
            for (int i = 0; i < cantidad; ++i) {
                std::cout << "  [" << i << "] ";
                pacientes[i]->mostrarInfo();
            }
        }
        std::cout << "=============================================" << std::endl;
    }

    // Consultar información detallada de un paciente por su índice
    void mostrarPacientePorIndice(int index) const {
        if (index >= 0 && index < cantidad) {
            std::cout << "\nConsulta detallada por indice [" << index << "]:" << std::endl;
            std::cout << "  - ";
            pacientes[index]->mostrarInfo();
        } else {
            std::cout << "Indice de consulta incorrecto. Rango valido: [0 - " << cantidad - 1 << "]" << std::endl;
        }
    }
};

#endif // HISTORIAL_CPP
