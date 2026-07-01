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

#ifndef MENUSISTEMA_CPP
#define MENUSISTEMA_CPP

//
// Created by GINO LEVANO on 01/07/2026.
//
#include <iostream>
#include <string>
#include <limits>
#include "Urgencias.cpp"
#include "Historial.cpp"
#include "Estadisticas.cpp"

// ==========================================
// SISTEMA DE URGENCIAS - MENU POR COMANDOS
// ==========================================
class SistemaUrgencias {
private:
    // Declaración ordenada: estadisticas primero para que se inicialice antes que historial
    Estadisticas estadisticas;
    ListaEspera listaEspera;
    HistorialPacientes historial;

public:
    // Constructor que inicializa el historial con la capacidad obtenida de la configuración
    SistemaUrgencias() 
        : estadisticas(), 
          listaEspera(), 
          historial(estadisticas.obtenerValor("Capacidad Inicial de Historial")) {}

    void precargarPaciente(Paciente* p) {
        if (p != nullptr) {
            listaEspera.insertarPaciente(p);
        }
    }

    void ejecutar() {
        std::string comando;
        bool salir = false;

        std::cout << "==========================================" << std::endl;
        std::cout << "   SISTEMA DE URGENCIAS - BASE MILITAR #" 
                  << estadisticas.obtenerValor("ID de la Unidad de Combate") << std::endl;
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
        else if (comando == "config") {
            comandoConfig();
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
        std::cout << "config    -> cambiar configuracion de la herramienta" << std::endl;
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
            listaEspera.mostrarListaEspera(estadisticas.obtenerValor("Umbral de Alerta de Espera"));
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
            listaEspera.mostrarListaEspera(estadisticas.obtenerValor("Umbral de Alerta de Espera"));
            historial.mostrarHistorial();
            estadisticas.mostrarEstadisticas();
        } else {
            std::cout << "No hay pacientes en espera." << std::endl;
        }
    }

    void comandoWait() const {
        listaEspera.mostrarListaEspera(estadisticas.obtenerValor("Umbral de Alerta de Espera"));
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
        // Nivel 1 [Prioridad 1] - Código Rojo (Reanimación inmediata / Riesgo vital)
        listaEspera.insertarPaciente(new Paciente("Floki Vilgerdarson", "Traumatismo toracico cerrado por explosion", 1));
        listaEspera.insertarPaciente(new Paciente("Ana Gomez", "Herida penetrante por bala en abdomen", 1));
        listaEspera.insertarPaciente(new Paciente("Gino Levano", "Shock hipovolemico por hemorragia masiva", 1));

        // Nivel 2 [Prioridad 2] - Código Naranja (Emergente / Muy urgente)
        listaEspera.insertarPaciente(new Paciente("Carlos Ruiz", "Fractura abierta expuesta de femur derecho", 2));
        listaEspera.insertarPaciente(new Paciente("Marta Diaz", "Quemaduras de segundo y tercer grado por metralla", 2));
        listaEspera.insertarPaciente(new Paciente("Ragnar Lothbrok", "Corte arterial profundo en brazo izquierdo", 2));
        listaEspera.insertarPaciente(new Paciente("Lagertha Shield", "Traumatismo craneoencefalico moderado", 2));

        // Nivel 3 [Prioridad 3] - Código Amarillo (Urgente / Estable con dolor)
        listaEspera.insertarPaciente(new Paciente("Luis Perez", "Fiebre de 40 grados con deshidratacion severa", 3));
        listaEspera.insertarPaciente(new Paciente("Bjorn Ironside", "Dolor abdominal agudo (sospecha apendicitis)", 3));
        listaEspera.insertarPaciente(new Paciente("Ivar Ragnarsson", "Esguince de tobillo grado III por caida", 3));

        // Nivel 4 [Prioridad 4] - Código Verde (Estándar / Urgencia menor)
        listaEspera.insertarPaciente(new Paciente("Rollo Sigurdsson", "Corte superficial en mano por manipulacion de arma", 4));
        listaEspera.insertarPaciente(new Paciente("Erik el Rojo", "Infeccion cutanea localizada en pierna", 4));
        listaEspera.insertarPaciente(new Paciente("Ubba Lothbrokson", "Otalgia severa y sospecha de infeccion de oido", 4));

        // Nivel 5 [Prioridad 5] - Código Azul (No urgente / Consultas y revisiones)
        listaEspera.insertarPaciente(new Paciente("Marta Sanchez", "Chequeo de rutina de constantes vitales en combate", 5));
        listaEspera.insertarPaciente(new Paciente("Harald Fairhair", "Cefalea tensional leve y cansancio extremo", 5));
        listaEspera.insertarPaciente(new Paciente("Alfred de Wessex", "Tos persistente e irritacion de garganta leve", 5));

        std::cout << "Pacientes de prueba (demo de 16 pacientes de urgencia) cargados correctamente." << std::endl;

        // Estado que toca despues de cargar demo
        listaEspera.mostrarListaEspera(estadisticas.obtenerValor("Umbral de Alerta de Espera"));
    }

    void mostrarEstadoGeneral() const {
        std::cout << std::endl;
        std::cout << "========== ESTADO GENERAL DEL SISTEMA ==========" << std::endl;

        listaEspera.mostrarListaEspera(estadisticas.obtenerValor("Umbral de Alerta de Espera"));
        historial.mostrarHistorial();
        estadisticas.mostrarEstadisticas();

        std::cout << "================================================" << std::endl;
    }

    void comandoConfig() {
        std::cout << "\n===== CONFIGURACION DE LA HERRAMIENTA =====" << std::endl;
        std::cout << "1. ID de la Unidad de Combate (Actual: " << estadisticas.obtenerValor("ID de la Unidad de Combate") << ")" << std::endl;
        std::cout << "2. Umbral de Alerta de Espera (Actual: " << estadisticas.obtenerValor("Umbral de Alerta de Espera") << ")" << std::endl;
        std::cout << "3. Capacidad Inicial de Historial (Actual: " << estadisticas.obtenerValor("Capacidad Inicial de Historial") << ")" << std::endl;
        std::cout << "Seleccione el parametro a modificar (1-3) o 0 para salir: ";

        int opcion;
        std::cin >> opcion;
        limpiarBuffer();

        if (opcion < 1 || opcion > 3) {
            std::cout << "Operacion cancelada." << std::endl;
            return;
        }

        std::cout << "Ingrese el nuevo valor entero: ";
        int nuevoValor;
        std::cin >> nuevoValor;
        limpiarBuffer();

        if (nuevoValor < 1) {
            std::cout << "El valor debe ser al menos 1." << std::endl;
            return;
        }

        std::string parametro;
        if (opcion == 1) {
            parametro = "ID de la Unidad de Combate";
        } else if (opcion == 2) {
            parametro = "Umbral de Alerta de Espera";
        } else if (opcion == 3) {
            parametro = "Capacidad Inicial de Historial";
            std::cout << "[NOTA] El cambio de capacidad del historial surtira efecto en el proximo reinicio del sistema." << std::endl;
        }

        estadisticas.establecerValor(parametro, nuevoValor);
        std::cout << "Parametro '" << parametro << "' actualizado con exito a: " << nuevoValor << std::endl;
    }

    void limpiarBuffer() const {
        if (std::cin.fail()) {
            std::cin.clear(); // Limpia el estado de error de la consola
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
};

#endif // MENUSISTEMA_CPP