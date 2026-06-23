#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>

// ==========================================
// 1. ENUM Y CLASE ITEM
// ==========================================
enum class TipoItem {
    BOOST_EXPERENCIA,
    BOOST_PODER
};

class Item {
private:
    std::string nombre;
    TipoItem tipo;
    int valor;

public:
    Item(const std::string& nombre = "", TipoItem tipo = TipoItem::BOOST_EXPERENCIA, int valor = 0)
        : nombre(nombre), tipo(tipo), valor(valor) {}

    std::string getNombre() const { return nombre; }
    TipoItem getTipo() const { return tipo; }
    int getValor() const { return valor; }

    void setNombre(const std::string& nombre) { this->nombre = nombre; }
    void setTipo(TipoItem tipo) { this->tipo = tipo; }
    void setValor(int valor) { this->valor = valor; }

    std::string getTipoString() const {
        return (tipo == TipoItem::BOOST_EXPERENCIA) ? "BOOST_EXPERENCIA" : "BOOST_PODER";
    }

    void mostrarInfo() const {
        std::cout << "Item: " << nombre << " | Tipo: " << getTipoString() << " | Valor: " << valor << std::endl;
    }
};

// ==========================================
// 2. CLASE SOLDADO
// ==========================================
class Soldado {
private:
    std::string nombre;
    bool esAliado;
    int nivel;
    int poderCombate;
    int experienciaActual;
    int experienciaParaNivel;

public:
    Soldado(const std::string& nombre = "", bool esAliado = true, int nivel = 1, int poderCombate = 10)
        : nombre(nombre), esAliado(esAliado), nivel(nivel), poderCombate(poderCombate),
          experienciaActual(0), experienciaParaNivel(100 * nivel) {}

    std::string getNombre() const { return nombre; }
    bool getEsAliado() const { return esAliado; }
    int getNivel() const { return nivel; }
    int getPoderCombate() const { return poderCombate; }
    int getExperienciaActual() const { return experienciaActual; }
    int getExperienciaParaNivel() const { return experienciaParaNivel; }

    void setNombre(const std::string& nombre) { this->nombre = nombre; }
    void setEsAliado(bool esAliado) { this->esAliado = esAliado; }
    void setNivel(int nivel) { this->nivel = nivel; }
    void setPoderCombate(int poderCombate) { this->poderCombate = poderCombate; }

    void ganarExperiencia(int exp) {
        if (!estaVivo()) return;
        experienciaActual += exp;
        std::cout << "El soldado " << nombre << " recibe " << exp << " puntos de experiencia" << std::endl;
        if (experienciaActual >= experienciaParaNivel) {
            subirNivel();
        }
    }

    void subirNivel() {
        nivel++;
        experienciaActual = 0;
        experienciaParaNivel = 100 * nivel;
        poderCombate += 5;
        std::cout << "[NIVEL UP] " << nombre << " ha subido al nivel " << nivel << "! Poder aumentado a " << poderCombate << std::endl;
    }

    void aplicarItem(const Item& item) {
        if (!estaVivo()) return;
        if (item.getTipo() == TipoItem::BOOST_EXPERENCIA) {
            ganarExperiencia(item.getValor());
        } else if (item.getTipo() == TipoItem::BOOST_PODER) {
            poderCombate += item.getValor();
            std::cout << "El soldado " << nombre << " aumenta su poder en " << item.getValor() << " puntos (Poder actual: " << poderCombate << ")" << std::endl;
        }
    }

    void recibirDanio(int danio) {
        poderCombate -= danio;
        if (poderCombate < 0) {
            poderCombate = 0;
        }
        std::cout << "El soldado " << nombre << " recibe " << danio << " puntos de danio. Poder restante: " << poderCombate << std::endl;
        if (poderCombate == 0) {
            std::cout << "[MUERTE] " << nombre << " ha caido en combate." << std::endl;
        }
    }

    bool estaVivo() const { return poderCombate > 0; }

    void mostrarInfo() const {
        std::cout << "  - Soldado: " << nombre 
                  << " | Bando: " << (esAliado ? "Aliado" : "Enemigo") 
                  << " | Nivel: " << nivel 
                  << " | Poder: " << poderCombate 
                  << " | Exp: " << experienciaActual << "/" << experienciaParaNivel << std::endl;
    }
};

// ==========================================
// 3. ESTRUCTURAS LINEALES PLANTILLAS RECURSIVAS
// ==========================================
template <typename T>
struct Node {
    T data;
    Node<T>* next;
    Node(T val) : data(val), next(nullptr) {}
};

template <typename T>
class LinkedList {
private:
    Node<T>* head;

    void addRecursive(Node<T>*& current, T val) {
        if (current == nullptr) {
            current = new Node<T>(val);
        } else {
            addRecursive(current->next, val);
        }
    }

    int sizeRecursive(Node<T>* current) const {
        if (current == nullptr) return 0;
        return 1 + sizeRecursive(current->next);
    }

    T getRecursive(Node<T>* current, int index, int currentIdx) const {
        if (current == nullptr) {
            throw std::out_of_range("Indice fuera de rango en lista enlazada");
        }
        if (currentIdx == index) {
            return current->data;
        }
        return getRecursive(current->next, index, currentIdx + 1);
    }

    void removeRecursive(Node<T>*& current, int index, int currentIdx) {
        if (current == nullptr) {
            throw std::out_of_range("Indice fuera de rango en lista enlazada");
        }
        if (currentIdx == index) {
            Node<T>* temp = current;
            current = current->next;
            delete temp;
        } else {
            removeRecursive(current->next, index, currentIdx + 1);
        }
    }

    void clearRecursive(Node<T>* current) {
        if (current == nullptr) return;
        clearRecursive(current->next);
        delete current;
    }

public:
    LinkedList() : head(nullptr) {}
    ~LinkedList() { clearRecursive(head); }

    void add(T val) { addRecursive(head, val); }
    int size() const { return sizeRecursive(head); }
    T get(int index) const { return getRecursive(head, index, 0); }
    void removeAt(int index) { removeRecursive(head, index, 0); }
};

template <typename T>
using DinamicMemoryList = LinkedList<T>;

// ==========================================
// 4. CLASE TROPA (ARRAY FIJO DE TAMAÑO 5 DE PUNTEROS)
// ==========================================
class Tropa {
private:
    std::string nombreTropa;
    Soldado* soldados[5];
    int numSoldados;

    int getPoderRecursivo(int idx) const {
        if (idx >= numSoldados) return 0;
        int power = (soldados[idx] != nullptr && soldados[idx]->estaVivo()) ? soldados[idx]->getPoderCombate() : 0;
        return power + getPoderRecursivo(idx + 1);
    }

    int getVivosRecursivo(int idx) const {
        if (idx >= numSoldados) return 0;
        int alive = (soldados[idx] != nullptr && soldados[idx]->estaVivo()) ? 1 : 0;
        return alive + getVivosRecursivo(idx + 1);
    }

    void aplicarItemRecursivo(const Item& item, int idx) {
        if (idx >= numSoldados) return;
        if (soldados[idx] != nullptr && soldados[idx]->estaVivo()) {
            soldados[idx]->aplicarItem(item);
        }
        aplicarItemRecursivo(item, idx + 1);
    }

    void mostrarInfoRecursivo(int idx) const {
        if (idx >= numSoldados) return;
        if (soldados[idx] != nullptr) {
            std::cout << "  [" << idx << "] ";
            soldados[idx]->mostrarInfo();
        }
        mostrarInfoRecursivo(idx + 1);
    }

    void liberarSoldadosRecursivo(int idx) {
        if (idx >= numSoldados) return;
        delete soldados[idx];
        soldados[idx] = nullptr;
        liberarSoldadosRecursivo(idx + 1);
    }

public:
    Tropa(const std::string& nombreTropa = "Tropa sin nombre")
        : nombreTropa(nombreTropa), numSoldados(0) {
        for (int i = 0; i < 5; ++i) soldados[i] = nullptr;
    }

    ~Tropa() { liberarSoldadosRecursivo(0); }

    std::string getNombreTropa() const { return nombreTropa; }
    int getNumSoldados() const { return numSoldados; }
    Soldado* getSoldado(int index) const {
        if (index < 0 || index >= numSoldados) return nullptr;
        return soldados[index];
    }
    void setNombreTropa(const std::string& nombre) { nombreTropa = nombre; }

    bool agregarSoldado(Soldado* soldado) {
        if (estaLlena() || soldado == nullptr) return false;
        soldados[numSoldados++] = soldado;
        return true;
    }

    bool eliminarSoldado(int index) {
        if (index < 0 || index >= numSoldados) return false;
        delete soldados[index];
        for (int i = index; i < numSoldados - 1; ++i) {
            soldados[i] = soldados[i + 1];
        }
        soldados[--numSoldados] = nullptr;
        return true;
    }

    bool estaLlena() const { return numSoldados >= 5; }
    bool estaVacia() const { return numSoldados == 0; }

    int getPoderCombateTotal() const { return getPoderRecursivo(0); }
    int getNumSoldadosVivos() const { return getVivosRecursivo(0); }
    void aplicarItemATodos(const Item& item) { aplicarItemRecursivo(item, 0); }

    void mostrarInfo() const {
        std::cout << "=== Tropa: " << nombreTropa << " ===" << std::endl;
        std::cout << "Soldados en tropa: " << numSoldados << "/5" << std::endl;
        std::cout << "Poder de combate total: " << getPoderCombateTotal() << std::endl;
        mostrarInfoRecursivo(0);
    }
};

// ==========================================
// 5. CLASE EJERCITO
// ==========================================
class Ejercito {
private:
    DinamicMemoryList<Tropa*>* tropas;
    std::string nombreEjercito;
    bool esAliado;

    int getNumSoldadosTotalRecursivo(int idx) const {
        if (idx >= tropas->size()) return 0;
        return tropas->get(idx)->getNumSoldados() + getNumSoldadosTotalRecursivo(idx + 1);
    }

    int getPoderCombateTotalRecursivo(int idx) const {
        if (idx >= tropas->size()) return 0;
        return tropas->get(idx)->getPoderCombateTotal() + getPoderCombateTotalRecursivo(idx + 1);
    }

    int getNumSoldadosVivosRecursivo(int idx) const {
        if (idx >= tropas->size()) return 0;
        return tropas->get(idx)->getNumSoldadosVivos() + getNumSoldadosVivosRecursivo(idx + 1);
    }

    void mostrarInfoRecursivo(int idx) const {
        if (idx >= tropas->size()) return;
        std::cout << "[" << idx + 1 << "] ";
        tropas->get(idx)->mostrarInfo();
        mostrarInfoRecursivo(idx + 1);
    }

    void liberarTropasRecursivo(int idx) {
        if (idx >= tropas->size()) return;
        delete tropas->get(idx);
        liberarTropasRecursivo(idx + 1);
    }

public:
    Ejercito() : nombreEjercito("Ejercito sin nombre"), esAliado(true) {
        tropas = new DinamicMemoryList<Tropa*>();
    }

    Ejercito(std::string nombreEjercito, bool esAliado)
        : nombreEjercito(nombreEjercito), esAliado(esAliado) {
        tropas = new DinamicMemoryList<Tropa*>();
    }

    ~Ejercito() {
        liberarTropasRecursivo(0);
        delete tropas;
    }

    std::string getNombreEjercito() const { return nombreEjercito; }
    bool getEsAliado() const { return esAliado; }
    int getNumTropas() const { return tropas->size(); }
    Tropa* getTropa(int index) const {
        if (index < 0 || index >= tropas->size()) return nullptr;
        return tropas->get(index);
    }
    void setNombreEjercito(const std::string& nombre) { nombreEjercito = nombre; }

    void agregarTropa(Tropa* tropa) {
        if (tropa != nullptr) tropas->add(tropa);
    }

    void eliminarTropa(int index) {
        if (index >= 0 && index < tropas->size()) {
            delete tropas->get(index);
            tropas->removeAt(index);
        }
    }

    int getNumSoldadosTotal() const { return getNumSoldadosTotalRecursivo(0); }
    int getPoderCombateTotal() const { return getPoderCombateTotalRecursivo(0); }
    int getNumSoldadosVivos() const { return getNumSoldadosVivosRecursivo(0); }

    void mostrarInfo() const {
        std::cout << "==================================================" << std::endl;
        std::cout << "EJERCITO: " << nombreEjercito 
                  << " | Bando: " << (esAliado ? "Aliado" : "Enemigo") 
                  << " | Tropas: " << tropas->size() << std::endl;
        std::cout << "Poder Total: " << getPoderCombateTotal() 
                  << " | Soldados Vivos: " << getNumSoldadosVivos() << std::endl;
        std::cout << "==================================================" << std::endl;
        mostrarInfoRecursivo(0);
    }
};

// ==========================================
// 6. CLASE JUGADOR
// ==========================================
class Jugador {
public:
    std::string nombre;
    std::string passwd;
    LinkedList<Soldado*>* soldados;
    LinkedList<Item*>* inventario;
    Ejercito* ejercito_guardado;

    Jugador(const std::string& nombre = "", const std::string& passwd = "")
        : nombre(nombre), passwd(passwd) {
        soldados = new LinkedList<Soldado*>();
        inventario = new LinkedList<Item*>();
        ejercito_guardado = new Ejercito(nombre + "_ejercito", true);
    }

    ~Jugador() {
        // Liberar reserva de soldados
        for (int i = 0; i < soldados->size(); ++i) {
            delete soldados->get(i);
        }
        delete soldados;

        // Liberar inventario de items
        for (int i = 0; i < inventario->size(); ++i) {
            delete inventario->get(i);
        }
        delete inventario;

        delete ejercito_guardado;
    }
};

// ==========================================
// 7. PERSISTENCIA JSON (PARSEO MANUAL)
// ==========================================
static void guardarSoldadosReservaRecursivo(std::ofstream& out, LinkedList<Soldado*>* lista, int idx) {
    if (lista == nullptr || idx >= lista->size()) return;
    Soldado* s = lista->get(idx);
    if (s != nullptr) {
        out << "        {\n";
        out << "          \"nombre\": \"" << s->getNombre() << "\",\n";
        out << "          \"esAliado\": " << (s->getEsAliado() ? "true" : "false") << ",\n";
        out << "          \"nivel\": " << s->getNivel() << ",\n";
        out << "          \"poderCombate\": " << s->getPoderCombate() << "\n";
        out << "        }";
        if (idx < lista->size() - 1) out << ",";
        out << "\n";
    }
    guardarSoldadosReservaRecursivo(out, lista, idx + 1);
}

static void guardarItemsInventarioRecursivo(std::ofstream& out, LinkedList<Item*>* lista, int idx) {
    if (lista == nullptr || idx >= lista->size()) return;
    Item* it = lista->get(idx);
    if (it != nullptr) {
        out << "        {\n";
        out << "          \"nombre\": \"" << it->getNombre() << "\",\n";
        out << "          \"tipo\": " << static_cast<int>(it->getTipo()) << ",\n";
        out << "          \"valor\": " << it->getValor() << "\n";
        out << "        }";
        if (idx < lista->size() - 1) out << ",";
        out << "\n";
    }
    guardarItemsInventarioRecursivo(out, lista, idx + 1);
}

static void guardarSoldadosTropaRecursivo(std::ofstream& out, Tropa* tropa, int idx) {
    if (tropa == nullptr || idx >= tropa->getNumSoldados()) return;
    Soldado* s = tropa->getSoldado(idx);
    if (s != nullptr) {
        out << "            {\n";
        out << "              \"nombre\": \"" << s->getNombre() << "\",\n";
        out << "              \"esAliado\": " << (s->getEsAliado() ? "true" : "false") << ",\n";
        out << "              \"nivel\": " << s->getNivel() << ",\n";
        out << "              \"poderCombate\": " << s->getPoderCombate() << "\n";
        out << "            }";
        if (idx < tropa->getNumSoldados() - 1) out << ",";
        out << "\n";
    }
    guardarSoldadosTropaRecursivo(out, tropa, idx + 1);
}

static void guardarTropasEjercitoRecursivo(std::ofstream& out, Ejercito* ejercito, int idx) {
    if (ejercito == nullptr || idx >= ejercito->getNumTropas()) return;
    Tropa* t = ejercito->getTropa(idx);
    if (t != nullptr) {
        out << "        {\n";
        out << "          \"nombreTropa\": \"" << t->getNombreTropa() << "\",\n";
        out << "          \"soldados\": [\n";
        guardarSoldadosTropaRecursivo(out, t, 0);
        out << "          ]\n";
        out << "        }";
        if (idx < ejercito->getNumTropas() - 1) out << ",";
        out << "\n";
    }
    guardarTropasEjercitoRecursivo(out, ejercito, idx + 1);
}

void guardarPartida(Jugador* jugador, const std::string& filename) {
    if (jugador == nullptr) return;
    
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cout << "Error al abrir el archivo de guardado: " << filename << std::endl;
        return;
    }

    out << "{\n";
    out << "  \"nombre\": \"" << jugador->nombre << "\",\n";
    out << "  \"passwd\": \"" << jugador->passwd << "\",\n";
    
    out << "  \"reserva\": [\n";
    guardarSoldadosReservaRecursivo(out, jugador->soldados, 0);
    out << "  ],\n";

    out << "  \"inventario\": [\n";
    guardarItemsInventarioRecursivo(out, jugador->inventario, 0);
    out << "  ],\n";

    out << "  \"ejercito\": {\n";
    out << "    \"nombreEjercito\": \"" << jugador->ejercito_guardado->getNombreEjercito() << "\",\n";
    out << "    \"esAliado\": " << (jugador->ejercito_guardado->getEsAliado() ? "true" : "false") << ",\n";
    out << "    \"tropas\": [\n";
    guardarTropasEjercitoRecursivo(out, jugador->ejercito_guardado, 0);
    out << "    ]\n";
    out << "  }\n";
    out << "}\n";

    out.close();
    std::cout << "[SISTEMA] Partida guardada exitosamente en " << filename << std::endl;
}

static std::string extractValue(const std::string& line, const std::string& key) {
    size_t keyPos = line.find("\"" + key + "\"");
    if (keyPos == std::string::npos) return "";
    
    size_t colonPos = line.find(":", keyPos);
    if (colonPos == std::string::npos) return "";

    size_t firstQuote = line.find("\"", colonPos);
    if (firstQuote != std::string::npos && firstQuote < line.find_first_of("0123456789tf", colonPos)) {
        size_t secondQuote = line.find("\"", firstQuote + 1);
        if (secondQuote != std::string::npos) {
            return line.substr(firstQuote + 1, secondQuote - firstQuote - 1);
        }
    } else {
        size_t valStart = line.find_first_not_of(" :,\t\r\n", colonPos + 1);
        size_t valEnd = line.find_first_of(",}\t\r\n", valStart);
        if (valStart != std::string::npos) {
            if (valEnd != std::string::npos) return line.substr(valStart, valEnd - valStart);
            return line.substr(valStart);
        }
    }
    return "";
}

Jugador* cargarPartida(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return nullptr;

    std::string line;
    std::string nombre = "";
    std::string passwd = "";

    while (std::getline(in, line)) {
        if (line.find("\"nombre\"") != std::string::npos) nombre = extractValue(line, "nombre");
        if (line.find("\"passwd\"") != std::string::npos) {
            passwd = extractValue(line, "passwd");
            break;
        }
    }

    if (nombre.empty()) {
        in.close();
        return nullptr;
    }

    Jugador* jugador = new Jugador(nombre, passwd);
    int state = 0; 
    Tropa* tropaActual = nullptr;

    while (std::getline(in, line)) {
        if (state == 0) {
            if (line.find("\"reserva\"") != std::string::npos) state = 1;
            else if (line.find("\"inventario\"") != std::string::npos) state = 2;
            else if (line.find("\"tropas\"") != std::string::npos) state = 3;
        }
        else if (state == 1) {
            if (line.find("]") != std::string::npos && line.find_first_not_of(" \t],") == std::string::npos) {
                state = 0;
                continue;
            }
            if (line.find("{") != std::string::npos) {
                std::string sNombre = "";
                bool sAliado = true;
                int sNivel = 1;
                int sPoder = 10;
                while (std::getline(in, line) && line.find("}") == std::string::npos) {
                    if (line.find("\"nombre\"") != std::string::npos) sNombre = extractValue(line, "nombre");
                    if (line.find("\"esAliado\"") != std::string::npos) sAliado = (extractValue(line, "esAliado") == "true");
                    if (line.find("\"nivel\"") != std::string::npos) sNivel = std::stoi(extractValue(line, "nivel"));
                    if (line.find("\"poderCombate\"") != std::string::npos) sPoder = std::stoi(extractValue(line, "poderCombate"));
                }
                jugador->soldados->add(new Soldado(sNombre, sAliado, sNivel, sPoder));
            }
        }
        else if (state == 2) {
            if (line.find("]") != std::string::npos && line.find_first_not_of(" \t],") == std::string::npos) {
                state = 0;
                continue;
            }
            if (line.find("{") != std::string::npos) {
                std::string iNombre = "";
                int iTipo = 0;
                int iValor = 0;
                while (std::getline(in, line) && line.find("}") == std::string::npos) {
                    if (line.find("\"nombre\"") != std::string::npos) iNombre = extractValue(line, "nombre");
                    if (line.find("\"tipo\"") != std::string::npos) iTipo = std::stoi(extractValue(line, "tipo"));
                    if (line.find("\"valor\"") != std::string::npos) iValor = std::stoi(extractValue(line, "valor"));
                }
                jugador->inventario->add(new Item(iNombre, static_cast<TipoItem>(iTipo), iValor));
            }
        }
        else if (state == 3) {
            if (line.find("]") != std::string::npos && line.find_first_not_of(" \t],") == std::string::npos) {
                state = 0;
                continue;
            }
            if (line.find("{") != std::string::npos) {
                std::string tNombre = "";
                while (std::getline(in, line) && line.find("\"soldados\"") == std::string::npos) {
                    if (line.find("\"nombreTropa\"") != std::string::npos) tNombre = extractValue(line, "nombreTropa");
                }
                tropaActual = new Tropa(tNombre);
                while (std::getline(in, line) && line.find("]") == std::string::npos) {
                    if (line.find("{") != std::string::npos) {
                        std::string sNombre = "";
                        bool sAliado = true;
                        int sNivel = 1;
                        int sPoder = 10;
                        while (std::getline(in, line) && line.find("}") == std::string::npos) {
                            if (line.find("\"nombre\"") != std::string::npos) sNombre = extractValue(line, "nombre");
                            if (line.find("\"esAliado\"") != std::string::npos) sAliado = (extractValue(line, "esAliado") == "true");
                            if (line.find("\"nivel\"") != std::string::npos) sNivel = std::stoi(extractValue(line, "nivel"));
                            if (line.find("\"poderCombate\"") != std::string::npos) sPoder = std::stoi(extractValue(line, "poderCombate"));
                        }
                        tropaActual->agregarSoldado(new Soldado(sNombre, sAliado, sNivel, sPoder));
                    }
                }
                jugador->ejercito_guardado->agregarTropa(tropaActual);
            }
        }
    }
    in.close();
    return jugador;
}

// ==========================================
// 8. LÓGICA DE INVENTARIO
// ==========================================
static void mostrarInventarioRecursivo(LinkedList<Item*>* inv, int idx) {
    if (inv == nullptr || idx >= inv->size()) return;
    std::cout << "  [" << idx + 1 << "] ";
    inv->get(idx)->mostrarInfo();
    mostrarInventarioRecursivo(inv, idx + 1);
}

void inventarioComandante(Jugador* jugador) {
    if (jugador == nullptr) return;

    if (jugador->inventario->size() == 0) {
        std::cout << "\n--- INVENTARIO DE COMANDANTE ---\n" << std::endl;
        std::cout << "ERROR: SISTEMA DE INVENTARIO VACIO O SIN ITEMS" << std::endl;
        std::cout << "\nPresione Enter para volver..." << std::endl;
        std::cin.get();
        return;
    }

    std::cout << "\n--- INVENTARIO DE COMANDANTE ---\n" << std::endl;
    mostrarInventarioRecursivo(jugador->inventario, 0);
    std::cout << "  [0] Cancelar" << std::endl;
    std::cout << "\nSeleccione una opcion: ";
    
    int opcion;
    if (!(std::cin >> opcion)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Opcion invalida." << std::endl;
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (opcion == 0) return;
    if (opcion < 1 || opcion > jugador->inventario->size()) {
        std::cout << "Indice fuera de rango." << std::endl;
        return;
    }

    Item* itemElegido = jugador->inventario->get(opcion - 1);
    std::cout << "\nTropas en tu ejercito activo:\n" << std::endl;
    jugador->ejercito_guardado->mostrarInfo();
    
    std::cout << "Seleccione la tropa para aplicar el item (1 a " << jugador->ejercito_guardado->getNumTropas() << "): ";
    int numTropa;
    if (!(std::cin >> numTropa)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Opcion invalida." << std::endl;
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (numTropa < 1 || numTropa > jugador->ejercito_guardado->getNumTropas()) {
        std::cout << "Numero de tropa invalido." << std::endl;
        return;
    }

    Tropa* tropaTarget = jugador->ejercito_guardado->getTropa(numTropa - 1);
    if (tropaTarget == nullptr) return;

    std::cout << "\nAplicar a:\n  [1] Un soldado individual\n  [2] Toda la tropa\nSeleccione opcion: ";
    int destino;
    if (!(std::cin >> destino)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Opcion invalida." << std::endl;
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (destino == 1) {
        std::cout << "Seleccione el indice del soldado (0 a " << tropaTarget->getNumSoldados() - 1 << "): ";
        int idxSoldado;
        if (!(std::cin >> idxSoldado)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Opcion invalida." << std::endl;
            return;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        Soldado* soldadoTarget = tropaTarget->getSoldado(idxSoldado);
        if (soldadoTarget == nullptr) {
            std::cout << "Soldado no encontrado." << std::endl;
            return;
        }

        soldadoTarget->aplicarItem(*itemElegido);
        std::cout << "\n[OK] Item aplicado exitosamente a " << soldadoTarget->getNombre() << std::endl;
        jugador->inventario->removeAt(opcion - 1);
        delete itemElegido;
    }
    else if (destino == 2) {
        tropaTarget->aplicarItemATodos(*itemElegido);
        std::cout << "\n[OK] Item aplicado exitosamente a toda la tropa " << tropaTarget->getNombreTropa() << std::endl;
        jugador->inventario->removeAt(opcion - 1);
        delete itemElegido;
    }
}

// ==========================================
// 9. LÓGICA DE COMBATE Y REFUERZOS
// ==========================================
static void aplicarFestinSoldadosRecursivo(Tropa* tropa, int idx) {
    if (tropa == nullptr || idx >= tropa->getNumSoldados()) return;
    Soldado* s = tropa->getSoldado(idx);
    if (s != nullptr && s->estaVivo()) {
        s->ganarExperiencia(200);
        std::cout << "." << std::endl;
    }
    aplicarFestinSoldadosRecursivo(tropa, idx + 1);
}

static void aplicarFestinEjercitoRecursivo(Ejercito* ejercito, int idx) {
    if (ejercito == nullptr || idx >= ejercito->getNumTropas()) return;
    Tropa* t = ejercito->getTropa(idx);
    if (t != nullptr) aplicarFestinSoldadosRecursivo(t, 0);
    aplicarFestinEjercitoRecursivo(ejercito, idx + 1);
}

static void darExperienciaSoldadosRecursivo(Tropa* tropa, int idx, int exp) {
    if (tropa == nullptr || idx >= tropa->getNumSoldados()) return;
    Soldado* s = tropa->getSoldado(idx);
    if (s != nullptr && s->estaVivo()) {
        s->ganarExperiencia(exp);
        std::cout << "." << std::endl;
    }
    darExperienciaSoldadosRecursivo(tropa, idx + 1, exp);
}

static void darExperienciaEjercitoRecursivo(Ejercito* ejercito, int idx, int exp) {
    if (ejercito == nullptr || idx >= ejercito->getNumTropas()) return;
    Tropa* t = ejercito->getTropa(idx);
    if (t != nullptr) darExperienciaSoldadosRecursivo(t, 0, exp);
    darExperienciaEjercitoRecursivo(ejercito, idx + 1, exp);
}

static int buscarPrimerSoldadoVivoRecursivo(LinkedList<Soldado*>* lista, int idx) {
    if (lista == nullptr || idx >= lista->size()) return -1;
    if (lista->get(idx) != nullptr && lista->get(idx)->estaVivo()) return idx;
    return buscarPrimerSoldadoVivoRecursivo(lista, idx + 1);
}

static void rellenarTropaConRefuerzos(Tropa* tropa, LinkedList<Soldado*>* reserva) {
    if (tropa == nullptr || tropa->estaLlena() || reserva == nullptr || reserva->size() == 0) return;
    
    int idxRefuerzo = buscarPrimerSoldadoVivoRecursivo(reserva, 0);
    if (idxRefuerzo != -1) {
        Soldado* s = reserva->get(idxRefuerzo);
        reserva->removeAt(idxRefuerzo);
        tropa->agregarSoldado(s);
        std::cout << "[REFUERZO] El soldado " << s->getNombre() 
                  << " ha sido enviado como refuerzo a la tropa " << tropa->getNombreTropa() << "!" << std::endl;
        rellenarTropaConRefuerzos(tropa, reserva);
    }
}

static void rellenarEjercitoConRefuerzosRecursivo(Ejercito* ejercito, LinkedList<Soldado*>* reserva, int idx) {
    if (ejercito == nullptr || idx >= ejercito->getNumTropas()) return;
    Tropa* t = ejercito->getTropa(idx);
    if (t != nullptr && t->getNumSoldadosVivos() == 0) {
        std::cout << "\n[INFO] La tropa " << t->getNombreTropa() << " ha sido diezmada. Llamando refuerzos..." << std::endl;
        rellenarTropaConRefuerzos(t, reserva);
    }
    rellenarEjercitoConRefuerzosRecursivo(ejercito, reserva, idx + 1);
}

static bool existeNombreEnReservaRecursivo(LinkedList<Soldado*>* lista, const std::string& nombre, int idx) {
    if (lista == nullptr || idx >= lista->size()) return false;
    if (lista->get(idx) != nullptr && lista->get(idx)->getNombre() == nombre) return true;
    return existeNombreEnReservaRecursivo(lista, nombre, idx + 1);
}

static void ejecutarCombate(Jugador* jugador) {
    if (jugador->ejercito_guardado->getNumTropas() == 0) {
        std::cout << "\nERROR: No tienes tropas activas en tu ejercito para combatir." << std::endl;
        std::cout << "Presiona Enter para volver..." << std::endl;
        std::cin.get();
        return;
    }

    std::cout << "\nEn la distancia, una marea de estandartes desconocidos surge en el horizonte..." << std::endl;
    std::cout << "¡Un ejercito enemigo se aproxima sediento de guerra!\n" << std::endl;
    std::cout << "Pulsa INTRO para continuar..." << std::endl;
    std::cin.get();

    int fervorAliado = jugador->ejercito_guardado->getPoderCombateTotal();
    int fervorEnemigo = 0;
    bool victoriaAliada = true;

    for (int tIdx = 0; tIdx < jugador->ejercito_guardado->getNumTropas(); ++tIdx) {
        Tropa* tAliada = jugador->ejercito_guardado->getTropa(tIdx);
        if (tAliada == nullptr || tAliada->getNumSoldadosVivos() == 0) continue;

        int numCombate = tIdx + 1;
        std::cout << "========== COMBATE DE TROPAS " << numCombate << " ==========" << std::endl;
        std::cout << "Tus tropas (" << tAliada->getNombreTropa() << ") avanzan bajo estandartes al viento..." << std::endl;
        
        std::string nameEnemigo = "Tropa enemiga " + std::to_string(numCombate);
        std::cout << "Frente a ellas, " << nameEnemigo << " ruge desafiando a los cielos.\n" << std::endl;

        int poderEnemigo = 50 + (std::rand() % 100);
        fervorEnemigo += poderEnemigo;

        int poderAliado = tAliada->getPoderCombateTotal();
        std::cout << "Poder de combate de " << tAliada->getNombreTropa() << ": " << poderAliado << std::endl;
        std::cout << "Poder de combate de " << nameEnemigo << ": " << poderEnemigo << "\n" << std::endl;

        if (poderAliado >= poderEnemigo) {
            std::cout << "¡Tus guerreros arrasan la linea enemiga! La tropa " << tAliada->getNombreTropa() 
                      << " se alza victoriosa y su fervor se suma a la siguiente batalla.\n" << std::endl;
            
            int danio = poderEnemigo / tAliada->getNumSoldadosVivos();
            for (int sIdx = 0; sIdx < tAliada->getNumSoldados(); ++sIdx) {
                if (tAliada->getSoldado(sIdx)->estaVivo()) {
                    tAliada->getSoldado(sIdx)->recibirDanio(danio / 3);
                }
            }
            
            fervorAliado += 81;
            fervorEnemigo = 0;
        } else {
            std::cout << "¡Tus tropas han sido sobrepasadas por el enemigo!\n" << std::endl;
            for (int sIdx = 0; sIdx < tAliada->getNumSoldados(); ++sIdx) {
                tAliada->getSoldado(sIdx)->recibirDanio(tAliada->getSoldado(sIdx)->getPoderCombate());
            }
            victoriaAliada = false;
            fervorAliado = 0;
            break;
        }

        std::cout << "Estado actual de la batalla:" << std::endl;
        std::cout << "  Fervor de tu ejercito: " << fervorAliado << std::endl;
        std::cout << "  Fervor del enemigo: " << fervorEnemigo << "\n" << std::endl;

        std::cout << "Pulsa INTRO para continuar..." << std::endl;
        std::cin.get();
    }

    if (victoriaAliada) {
        std::cout << "\nCuando el polvo se asienta, tus estandartes siguen en pie." << std::endl;
        std::cout << "Los bardos cantaran esta jornada como una de las grandes victorias de Gisberia." << std::endl;
        std::cout << "Otorgando experiencia a tus tropas por la batalla...\n" << std::endl;

        darExperienciaEjercitoRecursivo(jugador->ejercito_guardado, 0, 200);

        std::cout << "\nPulsa INTRO para continuar..." << std::endl;
        std::cin.get();

        std::cout << "El ejercito enemigo, inspirado por el Valor de tus soldados ha solicitado unirse a tu ejercito. ¿Quieres que se unan a tus filas?" << std::endl;
        std::cout << "[S]i [N]o" << std::endl;
        std::cout << "\nSeleccione opcion: ";
        
        std::string reclutar;
        std::getline(std::cin, reclutar);

        if (reclutar == "S" || reclutar == "s") {
            std::string nombresDisponibles[] = {"Rollo", "Erik", "Ubba", "Floki", "Harald"};
            for (int i = 0; i < 3; ++i) {
                std::string nombreRecluta = nombresDisponibles[std::rand() % 5];
                if (existeNombreEnReservaRecursivo(jugador->soldados, nombreRecluta, 0)) {
                    nombreRecluta += " II";
                }
                Soldado* nuevo = new Soldado(nombreRecluta, true, 3 + (std::rand() % 3), 20 + (std::rand() % 20));
                jugador->soldados->add(nuevo);
            }
            std::cout << "\nLos soldados del ejercito enemigo se han unido a tus tropas gracias a tu Carisma y Conviccion en Batalla" << std::endl;
        } else {
            std::cout << "\nHas decidido no reclutar a los soldados enemigos. Regresan a sus tierras con respeto mutuo." << std::endl;
        }
    } else {
        std::cout << "\n[INFO] La batalla ha terminado en derrota. Reorganizando tropas supervivientes..." << std::endl;
        rellenarEjercitoConRefuerzosRecursivo(jugador->ejercito_guardado, jugador->soldados, 0);
    }

    std::cout << "\nPulsa INTRO para continuar..." << std::endl;
    std::cin.get();
}

void hogueraBatalla(Jugador* jugador) {
    if (jugador == nullptr) return;

    while (true) {
        std::cout << "\n===============================" << std::endl;
        std::cout << "||    HOGUERA DE BATALLA     ||" << std::endl;
        std::cout << "===============================" << std::endl;
        std::cout << "  1. Festin de la noche previa" << std::endl;
        std::cout << "  2. Iniciar combate" << std::endl;
        std::cout << "  3. Volver al menu principal" << std::endl;
        std::cout << "\nSeleccione una opcion: ";

        int opcion;
        if (!(std::cin >> opcion)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Opcion invalida." << std::endl;
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (opcion == 3) {
            break;
        } else if (opcion == 1) {
            std::cout << "\nPreparando el gran festin para otorgar fuerza y experiencia..." << std::endl;
            aplicarFestinEjercitoRecursivo(jugador->ejercito_guardado, 0);
            std::cout << "\nPulsa INTRO para continuar..." << std::endl;
            std::cin.get();
        } else if (opcion == 2) {
            ejecutarCombate(jugador);
        } else {
            std::cout << "Opcion incorrecta." << std::endl;
        }
    }
}

static void mostrarReservaRecursivo(LinkedList<Soldado*>* lista, int idx) {
    if (lista == nullptr || idx >= lista->size()) return;
    Soldado* s = lista->get(idx);
    if (s != nullptr) {
        std::cout << "  [" << idx << "] ";
        s->mostrarInfo();
    }
    mostrarReservaRecursivo(lista, idx + 1);
}

static void mostrarTropasEjercitoRecursivo(Ejercito* ejercito, int idx) {
    if (ejercito == nullptr || idx >= ejercito->getNumTropas()) return;
    Tropa* t = ejercito->getTropa(idx);
    if (t != nullptr) {
        std::cout << "  [" << idx + 1 << "] Tropa: " << t->getNombreTropa() 
                  << " | Soldados: " << t->getNumSoldados() << "/5" 
                  << " | Poder Total: " << t->getPoderCombateTotal() << std::endl;
    }
    mostrarTropasEjercitoRecursivo(ejercito, idx + 1);
}

// ==========================================
// 10. MENÚ DE GESTIÓN Y BUCLE PRINCIPAL
// ==========================================
static void crearPartidaPorDefectoSiNoExiste() {
    std::ifstream f("Geo_partida.json");
    if (f.good()) {
        f.close();
        return;
    }
    f.close();

    Jugador* geo = new Jugador("Geo", "Sudosu");
    Tropa* crustaceo = new Tropa("Escuadron Crustaceo");
    crustaceo->agregarSoldado(new Soldado("Patrik", true, 6, 30));
    crustaceo->agregarSoldado(new Soldado("Eugine", true, 6, 30));
    crustaceo->agregarSoldado(new Soldado("Stuart", true, 6, 30));
    crustaceo->agregarSoldado(new Soldado("Edward", true, 6, 30));
    crustaceo->agregarSoldado(new Soldado("Bob", true, 6, 30));
    geo->ejercito_guardado->agregarTropa(crustaceo);

    geo->soldados->add(new Soldado("Rollo", true, 5, 33));
    geo->soldados->add(new Soldado("Erik", true, 3, 30));
    geo->soldados->add(new Soldado("Floki", true, 7, 20));
    geo->soldados->add(new Soldado("Ubba", true, 4, 25));

    geo->inventario->add(new Item("Pocion de Fuerza", TipoItem::BOOST_PODER, 15));
    geo->inventario->add(new Item("Pocion de Sabiduria", TipoItem::BOOST_EXPERENCIA, 250));

    guardarPartida(geo, "Geo_partida.json");
    delete geo;
}

void menuGestionarSoldados(Jugador* jugador) {
    while (true) {
        std::cout << "\n=========================================" << std::endl;
        std::cout << "||     GESTION DE SOLDADOS Y TROPAS    ||" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "  1. Crear nueva tropa" << std::endl;
        std::cout << "  2. Modificar tropas del ejercito" << std::endl;
        std::cout << "  3. Entrenar soldados de una tropa" << std::endl;
        std::cout << "  4. Ver soldados disponibles" << std::endl;
        std::cout << "  5. Ver tropas existentes" << std::endl;
        std::cout << "  6. Volver al menu principal" << std::endl;
        std::cout << "\nSeleccione una opcion: ";

        int opcion;
        if (!(std::cin >> opcion)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Opcion incorrecta." << std::endl;
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (opcion == 6) {
            break;
        } else if (opcion == 1) {
            std::cout << "\n=== CREAR NUEVA TROPA ===" << std::endl;
            std::cout << "Ingrese el nombre de la nueva tropa: ";
            std::string nombreTropa;
            std::getline(std::cin, nombreTropa);

            if (jugador->soldados->size() == 0) {
                std::cout << "[INFO] No tienes soldados en la reserva para asignar." << std::endl;
                continue;
            }

            std::cout << "\nSoldados disponibles en la reserva:" << std::endl;
            mostrarReservaRecursivo(jugador->soldados, 0);

            std::cout << "\nSeleccione soldados para agregar a la tropa (maximo 5)." << std::endl;
            std::cout << "Ingrese los numeros de indice separados por espacios: ";
            
            std::string inputs;
            std::getline(std::cin, inputs);
            std::stringstream ss(inputs);
            int idx;

            Soldado* seleccionados[5];
            int nSeleccionados = 0;

            while (ss >> idx && nSeleccionados < 5) {
                if (idx >= 0 && idx < jugador->soldados->size()) {
                    seleccionados[nSeleccionados++] = jugador->soldados->get(idx);
                }
            }

            if (nSeleccionados == 0) {
                std::cout << "No se seleccionaron soldados validos." << std::endl;
                continue;
            }

            Tropa* nuevaTropa = new Tropa(nombreTropa);
            for (int i = 0; i < nSeleccionados; ++i) {
                for (int rIdx = 0; rIdx < jugador->soldados->size(); ++rIdx) {
                    if (jugador->soldados->get(rIdx) == seleccionados[i]) {
                        jugador->soldados->removeAt(rIdx);
                        break;
                    }
                }
                nuevaTropa->agregarSoldado(seleccionados[i]);
                std::cout << "[OK] " << seleccionados[i]->getNombre() << " agregado a la tropa. (" << i + 1 << "/5)" << std::endl;
            }

            std::cout << "\n¿Desea agregar esta tropa al ejercito activo? (S/N): ";
            std::string confirm;
            std::getline(std::cin, confirm);

            if (confirm == "S" || confirm == "s") {
                jugador->ejercito_guardado->agregarTropa(nuevaTropa);
                std::cout << "[OK] Tropa agregada al ejercito exitosamente." << std::endl;
            } else {
                delete nuevaTropa;
                std::cout << "Creacion de tropa cancelada." << std::endl;
            }

        } else if (opcion == 2) {
            std::cout << "\n=== MODIFICAR TROPAS DEL EJERCITO ===" << std::endl;
            if (jugador->ejercito_guardado->getNumTropas() == 0) {
                std::cout << "[INFO] No hay tropas en el ejercito." << std::endl;
                continue;
            }

            std::cout << "\nTropas en el ejercito:" << std::endl;
            mostrarTropasEjercitoRecursivo(jugador->ejercito_guardado, 0);

            std::cout << "\n[E] Eliminar una tropa del ejercito" << std::endl;
            std::cout << "[V] Volver al menu anterior" << std::endl;
            std::cout << "Seleccione una opcion: ";
            
            std::string accion;
            std::getline(std::cin, accion);

            if (accion == "E" || accion == "e") {
                std::cout << "Ingrese el numero de la tropa a eliminar (0 para cancelar): ";
                int elimIdx;
                if (std::cin >> elimIdx) {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    if (elimIdx > 0 && elimIdx <= jugador->ejercito_guardado->getNumTropas()) {
                        jugador->ejercito_guardado->eliminarTropa(elimIdx - 1);
                        std::cout << "[OK] Tropa eliminada y memoria liberada." << std::endl;
                    } else if (elimIdx != 0) {
                        std::cout << "Indice de tropa fuera de rango." << std::endl;
                    }
                } else {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
        } else if (opcion == 3) {
            std::cout << "\n=== ENTRENAR SOLDADOS DE UNA TROPA ===" << std::endl;
            if (jugador->ejercito_guardado->getNumTropas() == 0) {
                std::cout << "[INFO] No hay tropas para entrenar." << std::endl;
                continue;
            }
            jugador->ejercito_guardado->mostrarInfo();
            std::cout << "Seleccione el numero de la tropa a entrenar: ";
            int tIdx;
            if (std::cin >> tIdx) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (tIdx > 0 && tIdx <= jugador->ejercito_guardado->getNumTropas()) {
                    Tropa* target = jugador->ejercito_guardado->getTropa(tIdx - 1);
                    for (int sIdx = 0; sIdx < target->getNumSoldados(); ++sIdx) {
                        Soldado* s = target->getSoldado(sIdx);
                        if (s != nullptr && s->estaVivo()) s->ganarExperiencia(150);
                    }
                }
            } else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        } else if (opcion == 4) {
            std::cout << "\n=== SOLDADOS DISPONIBLES (SIN TROPA) ===" << std::endl;
            if (jugador->soldados->size() == 0) {
                std::cout << "[INFO] Todos los soldados estan asignados a tropas o la reserva esta vacia." << std::endl;
            } else {
                mostrarReservaRecursivo(jugador->soldados, 0);
            }
        } else if (opcion == 5) {
            std::cout << "\n=== TROPAS EXISTENTES ===" << std::endl;
            jugador->ejercito_guardado->mostrarInfo();
        }
    }
}

int main() {
    std::srand(std::time(nullptr));
    crearPartidaPorDefectoSiNoExiste();

    Jugador* jugadorActual = nullptr;

    while (true) {
        std::cout << "\n==============================================" << std::endl;
        std::cout << "||   BIENVENIDO AL SISTEMA DE BATALLA GISBERIA  ||" << std::endl;
        std::cout << "==============================================" << std::endl;
        std::cout << "=== PARTIDAS GUARDADAS ===" << std::endl;
        std::cout << "[1] Geo_partida.json" << std::endl;
        std::cout << "[2] Crear nueva partida" << std::endl;
        std::cout << "[0] Salir" << std::endl;
        std::cout << "\nSeleccione una opcion: ";

        int opcionMenu;
        if (!(std::cin >> opcionMenu)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada invalida." << std::endl;
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (opcionMenu == 0) {
            break;
        } else if (opcionMenu == 1) {
            std::cout << "\nREY ENOLAS V DE GISBERIA: Bienvenido de vuelta. Por seguridad, necesito verificar tu identidad." << std::endl;
            std::cout << "Ingresa tu contraseña: ";
            std::string pass;
            std::getline(std::cin, pass);

            if (pass == "Sudosu") {
                jugadorActual = cargarPartida("Geo_partida.json");
                if (jugadorActual != nullptr) {
                    std::cout << "\n[SISTEMA] Partida cargada exitosamente." << std::endl;
                    std::cout << "REY ENOLAS V DE GISBERIA: ¡Excelente, Gran Comandante " << jugadorActual->nombre << "! Tus tropas estan listas para la batalla." << std::endl;
                } else {
                    std::cout << "Error al cargar la partida." << std::endl;
                    continue;
                }
            } else {
                std::cout << "Contraseña incorrecta." << std::endl;
                continue;
            }
        } else if (opcionMenu == 2) {
            std::cout << "\n[INFO] Iniciando nueva partida..." << std::endl;
            std::cout << "\nREY ENOLAS V DE GISBERIA: Bienvenido soldado, estas en tu promocion para comandante del ejercito. Por favor, dime, ¿como te llamas?" << std::endl;
            std::cout << "Escribe tu nombre: ";
            std::string nombre;
            std::getline(std::cin, nombre);

            std::cout << "Escribe tu contraseña: ";
            std::string passwd;
            std::getline(std::cin, passwd);

            jugadorActual = new Jugador(nombre, passwd);
            
            Tropa* inicial = new Tropa("Escuadron Inicial");
            inicial->agregarSoldado(new Soldado("Recluta A", true, 1, 10));
            inicial->agregarSoldado(new Soldado("Recluta B", true, 1, 10));
            jugadorActual->ejercito_guardado->agregarTropa(inicial);
            
            jugadorActual->inventario->add(new Item("Pocion Basica", TipoItem::BOOST_PODER, 5));
        } else {
            std::cout << "Opcion incorrecta." << std::endl;
            continue;
        }

        while (true) {
            std::cout << "\n======================================" << std::endl;
            std::cout << "||     MENU PRINCIPAL - EJERCITO    ||" << std::endl;
            std::cout << "======================================" << std::endl;
            std::cout << "  1. Ver Informacion del ejercito activo" << std::endl;
            std::cout << "  2. Gestionar soldados" << std::endl;
            std::cout << "  3. Inventario de comandante" << std::endl;
            std::cout << "  4. Hoguera de Batalla" << std::endl;
            std::cout << "  5. Salir" << std::endl;
            std::cout << "\nSeleccione una opcion: ";

            int opcionJuego;
            if (!(std::cin >> opcionJuego)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Opcion incorrecta." << std::endl;
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (opcionJuego == 5) {
                std::cout << "[INFO] Saliendo del programa... Hasta pronto!" << std::endl;
                std::cout << "¿Desea guardar la partida antes de salir? (S/N): ";
                std::string guardar;
                std::getline(std::cin, guardar);
                if (guardar == "S" || guardar == "s") {
                    guardarPartida(jugadorActual, jugadorActual->nombre + "_partida.json");
                }
                delete jugadorActual;
                jugadorActual = nullptr;
                break;
            } else if (opcionJuego == 1) {
                jugadorActual->ejercito_guardado->mostrarInfo();
            } else if (opcionJuego == 2) {
                menuGestionarSoldados(jugadorActual);
            } else if (opcionJuego == 3) {
                inventarioComandante(jugadorActual);
            } else if (opcionJuego == 4) {
                hogueraBatalla(jugadorActual);
            } else {
                std::cout << "Opcion incorrecta." << std::endl;
            }
        }
    }

    std::cout << "Saliendo..." << std::endl;
    return 0;
}