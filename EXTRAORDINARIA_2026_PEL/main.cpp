#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>

// Inclusión directa de archivos de código fuente (.cpp) para evitar archivos de cabecera (.h)
#include "Urgencias.cpp"
#include "HistorialPacientes.cpp"
#include "Estadisticas.cpp"
#include "MenuSistema.cpp"

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Instanciar el sistema de urgencias
    SistemaUrgencias sistema;

    // Iniciar el bucle de comandos del menú
    sistema.ejecutar();

    return 0;
}
