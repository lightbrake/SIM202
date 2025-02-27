// #ifndef MESSAGE_H
// #define MESSAGE_H

// #include <string>
// #include <vector>
// #include <sstream>
// #include <iostream>

// struct Ordre {
//     char type;
//     std::string clientID;
//     std::string action;
//     int quantite;
//     double prix;
//     // date supprimé, instant sera géré ailleurs
// };

// class Message {
// public:
//     std::string clientID;
//     int messageID;
//     std::string contenu;

//     Message(const std::string& id, int num, const std::string& cont)
//         : clientID(id), messageID(num), contenu(cont) {}

//     std::string toString() const {
//         std::ostringstream oss;
//         oss << clientID << ";" << messageID << ";" << contenu;
//         return oss.str();
//     }

//     bool validate() const {
//         if (clientID.empty() || messageID <= 0 || contenu.empty()) return false;
//         return true;
//     }

//     std::vector<Ordre> parseContent() const {
//         std::vector<Ordre> ordres;
//         std::istringstream iss(contenu);
//         std::string token;

//         while (std::getline(iss, token, ';')) {
//             if (token.empty()) continue;
//             Ordre ordre;
//             std::istringstream ordreStream(token);
//             std::string field;
//             std::vector<std::string> fields;
//             while (std::getline(ordreStream, field, ',')) {
//                 fields.push_back(field);
//             }
//             if (fields.size() != 4) { // Plus de date, 4 champs : type, action, quantite, prix
//                 std::cerr << "[Erreur] Format d'ordre invalide: " << token << std::endl;
//                 continue;
//             }
//             ordre.type = (fields[0][0] == '#') ? fields[0][1] : fields[0][0];
//             ordre.clientID = this->clientID;
//             ordre.action = fields[1];
//             ordre.quantite = std::stoi(fields[2]);
//             ordre.prix = std::stod(fields[3]);
//             ordres.push_back(ordre);
//         }
//         return ordres;
//     }
// };

// #endif
#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

struct Ordre {
    char type;
    std::string clientID;
    std::string action;
    int quantite;
    double prix;// -1 signifie "à tout prix"
    // date supprimé, instant sera géré ailleurs
};

class Message {
public:
    std::string clientID;
    int messageID;
    std::string contenu;

    Message(const std::string& id, int num, const std::string& cont)
        : clientID(id), messageID(num), contenu(cont) {}

    std::string toString() const {
        std::ostringstream oss;
        oss << clientID << ";" << messageID << ";" << contenu;
        return oss.str();
    }

    bool validate() const {
        if (clientID.empty() || messageID <= 0 || contenu.empty()) return false;
        return true;
    }

    std::vector<Ordre> parseContent() const {
        std::vector<Ordre> ordres;
        std::istringstream iss(contenu);
        std::string token;

        while (std::getline(iss, token, ';')) {
            if (token.empty()) continue;
            Ordre ordre;
            std::istringstream ordreStream(token);
            std::string field;
            std::vector<std::string> fields;
            while (std::getline(ordreStream, field, ',')) {
                fields.push_back(field);
            }
            if (fields.size() != 4) { // 4 champs : type, action, quantite, prix
                std::cerr << "[Erreur] Format d'ordre invalide: " << token << std::endl;
                continue;
            }
            ordre.type = (fields[0][0] == '#') ? fields[0][1] : fields[0][0];
            ordre.clientID = this->clientID;
            ordre.action = fields[1];
            ordre.quantite = std::stoi(fields[2]);
            // Gestion du prix : "MKT" ou vide = "à tout prix"
            if (fields[3] == "MKT" || fields[3].empty()) {
                ordre.prix = -1; // "À tout prix"
            } else {
                ordre.prix = std::stod(fields[3]);
            }
            ordres.push_back(ordre);
        }
        return ordres;
    }
};
#endif