#include <iostream>
#include <string>
#include "Urgencias.cpp"


class HistorialPacientes {
private:

    Paciente** historial;

    int capacidad;

    int cantidad;

    void ampliarCapacidad() {

        capacidad *= 2;

        Paciente** nuevo = new Paciente*[capacidad];

        for (int i = 0; i < cantidad; i++) {
            nuevo[i] = historial[i];
        }

        delete[] historial;

        historial = nuevo;
    }

public:

    HistorialPacientes() {

        capacidad = 5;

        cantidad = 0;

        historial = new Paciente*[capacidad];
    }

    HistorialPacientes(const HistorialPacientes&) = delete;

    HistorialPacientes& operator=(const HistorialPacientes&) = delete;

    ~HistorialPacientes() {

        for (int i = 0; i < cantidad; i++) {
            delete historial[i];
        }

        delete[] historial;
    }

    void anadirPaciente(Paciente* paciente) {

        if (paciente == nullptr)
            return;

        if (cantidad == capacidad) {
            ampliarCapacidad();
        }

        historial[cantidad] = paciente;

        cantidad++;
    }

    void mostrarHistorial() const {

        std::cout << std::endl;
        std::cout << "========== HISTORIAL ==========" << std::endl;

        if (cantidad == 0) {

            std::cout << "No hay pacientes atendidos." << std::endl;

            return;
        }

        for (int i = 0; i < cantidad; i++) {

            std::cout << "[" << i << "] ";

            historial[i]->mostrarInfo();
        }

        std::cout << "===============================" << std::endl;
    }

    void mostrarPacientePorIndice(int indice) const {

        if (indice < 0 || indice >= cantidad) {

            std::cout << "Indice incorrecto." << std::endl;

            return;
        }

        historial[indice]->mostrarInfo();
    }

    int getCantidad() const {

        return cantidad;
    }

};