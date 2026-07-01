//
// Created by GINO LEVANO on 01/07/2026.
//

#include <limits>

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
        if (comando == "help") {
            mostrarAyuda();
        }
        else if (comando == "add") {
            comandoAdd();
        }
        else if (comando == "attend") {
            comandoAttend();
        }
        else if (comando == "wait") {
            comandoWait();
        }
        else if (comando == "history") {
            comandoHistory();
        }
        else if (comando == "get") {
            comandoGet();
        }
        else if (comando == "stats") {
            comandoStats();
        }
        else if (comando == "demo") {
            comandoDemo();
        }
        else if (comando == "state") {
            mostrarEstadoGeneral();
        }
        else {
            std::cout << "Comando no reconocido. Escribe 'help'." << std::endl;
        }
    }

    void mostrarAyuda() const {
        std::cout << std::endl;
        std::cout << "Comandos disponibles:" << std::endl;
        std::cout << "help     -> mostrar ayuda" << std::endl;
        std::cout << "add      -> anadir paciente" << std::endl;
        std::cout << "attend   -> atender siguiente paciente" << std::endl;
        std::cout << "wait     -> mostrar lista de espera" << std::endl;
        std::cout << "history  -> mostrar historial" << std::endl;
        std::cout << "get      -> mostrar paciente del historial por indice" << std::endl;
        std::cout << "stats    -> mostrar estadisticas" << std::endl;
        std::cout << "demo     -> cargar pacientes de prueba" << std::endl;
        std::cout << "state    -> mostrar estado general" << std::endl;
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
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
};