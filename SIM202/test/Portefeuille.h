#ifndef PORTEFEUILLE_H
#define PORTEFEUILLE_H

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Portefeuille {
private:
    std::map<std::string, std::map<std::string, int>> portefeuille; // clientID -> (action, quantité)

public:
    void updatePortfolio(const std::string& clientID, const std::string& action, int quantite, bool achat) {
        if (achat) {
            portefeuille[clientID][action] += quantite;
        } else {
            portefeuille[clientID][action] -= quantite;
            if (portefeuille[clientID][action] <= 0) {
                portefeuille[clientID].erase(action);
            }
        }
        savePortfolioToFile();
    }

    void savePortfolioToFile() {
        std::ofstream file("portefeuilles.txt");
        if (file.is_open()) {
            for (const auto& client : portefeuille) {
                for (const auto& asset : client.second) {
                    file << client.first << "," << asset.first << "," << asset.second << "\n";
                }
            }
            file.close();
        } else {
            std::cerr << "[Erreur] Impossible d'ouvrir portefeuilles.txt\n";
        }
    }

    void printPortfolio() const {
        std::cout << "\n=== État des Portefeuilles ===\n";
        for (const auto& client : portefeuille) {
            std::cout << "Client: " << client.first << "\n";
            std::cout << std::setw(20) << "Action"<<"    " << std::setw(15) << "Quantité\n";
            for (const auto& asset : client.second) {
                std::cout << std::setw(20) << asset.first << std::setw(15) << asset.second << "\n";
            }
            std::cout << "\n";
        }
    }

    // Vérifie si le client a assez d'actions pour vendre
    bool hasEnoughShares(const std::string& clientID, const std::string& action, int quantite) const {
        auto clientIt = portefeuille.find(clientID);
        if (clientIt == portefeuille.end()) {
            return false; // Client n'a pas de portefeuille
        }
        auto assetIt = clientIt->second.find(action);
        if (assetIt == clientIt->second.end()) {
            return false; // Client n'a pas cette action
        }
        return assetIt->second >= quantite; // Vérifie si la quantité est suffisante
    }
};

#endif // PORTEFEUILLE_H