// ============================================================================
// REVISIÓN Y RECOMENDACIONES DE INTEGRACIÓN (POR FAVOR REVISAR):
// 1. BUG DE BUCLE INFINITO EN limpiarBuffer(): Si se introduce una entrada no 
//    numérica (como texto en Prioridad o Índice), std::cin entra en estado de error 
//    y std::cin.ignore() no hace nada. CORREGIDO: Se añadió std::cin.clear() antes.
// 2. INCLUSIONES DE LOS COMPAÑEROS: Para que este archivo reconozca las clases 
//    externas, se requiere incluir sus partes al inicio:
//    #include "Urgencias.cpp"
//    #include "Historial.cpp"             // (Persona B debe crearlo)
//    #include "Estadísticas y Configuración.cpp" // (Renombrar a Estadisticas.cpp recomendado)
// ============================================================================

//
// Created by GINO LEVANO on 01/07/2026.
//
#include <iostream>
#include <string>
#include <limits>
#include "Urgencias.cpp"
#include "Estadisticas.cpp"

// ==========================================
// SISTEMA DE URGENCIAS - MENU POR COMANDOS
// ==========================================
class SistemaUrgencias {
private:
    ListaEspera listaEspera;
    HistorialPacientes historial;
    Estadisticas estadisticas;

public:
    void ejecutar() {
        std::string comando;
        bool salir = false;

        std::cout << "==========================================" << std::endl;
        std::cout << "   SISTEMA DE URGENCIAS - ZONA MILITAR" << std::endl;
        std::cout << "==========================================" << std::endl;

        mostrarAyuda();

        while (!salir) {
            std::cout << std::endl;
            std::cout << "> ";
            std::getline(std::cin, comando);

            if (comando == "exit") {
                salir = true;
            } else {
                procesarComando(comando);
            }
        }

        std::cout << "Sistema finalizado." << std::endl;
    }

private:
    void procesarComando(const std::string& comando) {
        if (comando == "ayuda") {
            mostrarAyuda();
        }
        else if (comando == "anadir") {
            comandoAdd();
        }
        else if (comando == "atender") {
            comandoAttend();
        }
        else if (comando == "esperar") {
            comandoWait();
        }
        else if (comando == "historial") {
            comandoHistory();
        }
        else if (comando == "mostrar") {
            comandoGet();
        }
        else if (comando == "estadistica") {
            comandoStats();
        }
        else if (comando == "demo") {
            comandoDemo();
        }
        else if (comando == "estado") {
            mostrarEstadoGeneral();
        }
        else {
            std::cout << "Comando no reconocido. Escribe 'ayuda'." << std::endl;
        }
    }

    void mostrarAyuda() const {
        std::cout << std::endl;
        std::cout << "Comandos disponibles:" << std::endl;
        std::cout << "ayuda     -> mostrar ayuda" << std::endl;
        std::cout << "anadir      -> anadir paciente" << std::endl;
        std::cout << "atender   -> atender siguiente paciente" << std::endl;
        std::cout << "esperar     -> mostrar lista de espera" << std::endl;
        std::cout << "historial  -> mostrar historial" << std::endl;
        std::cout << "mostrar      -> mostrar paciente del historial por indice" << std::endl;
        std::cout << "estadistica    -> mostrar estadisticas" << std::endl;
        std::cout << "demo     -> cargar pacientes de prueba" << std::endl;
        std::cout << "estado    -> mostrar estado general" << std::endl;
        std::cout << "exit     -> salir" << std::endl;
    }

    void comandoAdd() {
        std::string nombre;
        std::string motivo;
        int prioridad;

        std::cout << "Nombre del paciente: ";
        std::getline(std::cin, nombre);

        std::cout << "Motivo de consulta: ";
        std::getline(std::cin, motivo);

        std::cout << "Prioridad (1-5): ";
        std::cin >> prioridad;
        limpiarBuffer();

        if (prioridad >= 1 && prioridad <= 5) {
            Paciente* nuevoPaciente = new Paciente(nombre, motivo, prioridad);
            listaEspera.insertarPaciente(nuevoPaciente);

            std::cout << std::endl;
            std::cout << "Paciente anadido correctamente." << std::endl;

            // Estado que toca despues de anadir
            listaEspera.mostrarListaEspera();
        } else {
            std::cout << "Prioridad incorrecta. Debe estar entre 1 y 5." << std::endl;
        }
    }

    void comandoAttend() {
        Paciente* atendido = listaEspera.atenderSiguiente();

        if (atendido != nullptr) {
            std::cout << std::endl;
            std::cout << "Paciente atendido:" << std::endl;
            atendido->mostrarInfo();

            historial.anadirPaciente(atendido);
            estadisticas.registrarPaciente(atendido);

            std::cout << std::endl;
            std::cout << "Paciente movido al historial y registrado en estadisticas." << std::endl;

            // Estado que toca despues de atender
            listaEspera.mostrarListaEspera();
            historial.mostrarHistorial();
            estadisticas.mostrarEstadisticas();
        } else {
            std::cout << "No hay pacientes en espera." << std::endl;
        }
    }

    void comandoWait() const {
        listaEspera.mostrarListaEspera();
    }

    void comandoHistory() const {
        historial.mostrarHistorial();
    }

    void comandoGet() const {
        int indice;

        std::cout << "Indice del paciente en historial: ";
        std::cin >> indice;
        limpiarBuffer();

        historial.mostrarPacientePorIndice(indice);
    }

    void comandoStats() const {
        estadisticas.mostrarEstadisticas();
    }

    void comandoDemo() {
        listaEspera.insertarPaciente(new Paciente("Ana", "Herida", 1));
        listaEspera.insertarPaciente(new Paciente("Luis", "Fiebre", 3));
        listaEspera.insertarPaciente(new Paciente("Carlos", "Fractura", 2));
        listaEspera.insertarPaciente(new Paciente("Marta", "Revision", 5));
        listaEspera.insertarPaciente(new Paciente("Gino", "Dolor", 1));

        std::cout << "Pacientes de prueba cargados correctamente." << std::endl;

        // Estado que toca despues de cargar demo
        listaEspera.mostrarListaEspera();
    }

    void mostrarEstadoGeneral() const {
        std::cout << std::endl;
        std::cout << "========== ESTADO GENERAL DEL SISTEMA ==========" << std::endl;

        listaEspera.mostrarListaEspera();
        historial.mostrarHistorial();
        estadisticas.mostrarEstadisticas();

        std::cout << "================================================" << std::endl;
    }

    void limpiarBuffer() const {
        if (std::cin.fail()) {
            std::cin.clear(); // Limpia el estado de error de la consola
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
};