#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

// Structure représentant un ordre individuel extrait du message
struct Ordre {
    char type;        // 'A' pour Achat, 'V' pour Vente
    std::string clientID;  // Identifiant du client
    std::string action;
    int quantite;
    double prix;
    std::string date;
};

class Message {
public:
    // Attributs
    std::string clientID;  // Identifiant unique du client
    int messageID;         // Numéro unique du message
    std::string contenu;   // Exemple : "#A,TSLA,10,750.5,2025-02-04;#V,AAPL,5,150.0,2025-02-04"

    // Constructeur
    Message(const std::string& id, int num, const std::string& cont)
        : clientID(id), messageID(num), contenu(cont) {}

    // Convertit l'objet Message en chaîne de caractères pour l'envoi
    std::string toString() const {
        std::ostringstream oss;
        oss << clientID << ";" << messageID << ";" << contenu;
        return oss.str();
    }

    // Vérifie que le message respecte le format attendu
    bool validate() const {
        if(clientID.empty()) return false;
        if(messageID <= 0) return false;
        if(contenu.empty()) return false;
        return true;
    }

    // Analyse le contenu du message pour extraire la liste des ordres
    std::vector<Ordre> parseContent() const {
        std::vector<Ordre> ordres;
        std::istringstream iss(contenu);
        std::string token;

        // Les ordres sont séparés par un point-virgule
        while (std::getline(iss, token, ';')) {
            if(token.empty()) continue;
            Ordre ordre;
            std::istringstream ordreStream(token);
            std::string field;
            std::vector<std::string> fields;
            while (std::getline(ordreStream, field, ',')) {
                fields.push_back(field);
            }
            // Vérification du format : un ordre doit avoir exactement 5 champs
            if(fields.size() != 5) {
                std::cerr << "[Erreur] Format d'ordre invalide: " << token << std::endl;
                continue;
            }
            // Extraction des données
            ordre.type = (fields[0][0] == '#') ? fields[0][1] : fields[0][0];
            ordre.clientID = this->clientID; // Ajout du client ID
            ordre.action = fields[1];
            ordre.quantite = std::stoi(fields[2]);
            ordre.prix = std::stod(fields[3]);
            ordre.date = fields[4];

            ordres.push_back(ordre);
        }
        return ordres;
    }
};

#endif // MESSAGE_H
