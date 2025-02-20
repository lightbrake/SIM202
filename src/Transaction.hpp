#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Message.hpp"  // Intégration de la classe Message

// Structure représentant une transaction individuelle
struct TransactionRecord {
    std::string clientID;  // Ajout de l'identifiant du client
    char type;             // 'A' pour Achat, 'V' pour Vente
    double prix;           // Prix d'exécution de la transaction
    int quantite;          // Quantité d'actions échangées
    std::string date;      // Date de la transaction
};

class Transaction {
private:
    std::map<std::string, std::vector<TransactionRecord>> historique; // Transactions par action

public:
    // Ajoute une transaction à partir d'un message reçu
    void addTransaction(const Message& msg) {
        std::vector<Ordre> ordres = msg.parseContent();

        for (const Ordre& ordre : ordres) {
            TransactionRecord record {msg.clientID, ordre.type, ordre.prix, ordre.quantite, ordre.date};
            historique[ordre.action].push_back(record);
        }
        
        // Générer un fichier de transaction
        saveTransactionToFile(msg);
    }

    // Retourne toutes les transactions pour une action donnée
    std::vector<TransactionRecord> getTransactionsForAction(const std::string& action) const {
        auto it = historique.find(action);
        return (it != historique.end()) ? it->second : std::vector<TransactionRecord>();
    }

    // Affiche tout l'historique des transactions
    void printAllTransactions() const {
        for (const auto& pair : historique) {
            std::cout << "Action: " << pair.first << std::endl;
            for (const auto& record : pair.second) {
                std::cout << "  Client: " << record.clientID
                          << ", Type: " << record.type 
                          << ", Prix: " << record.prix 
                          << ", Quantité: " << record.quantite 
                          << ", Date: " << record.date << std::endl;
            }
        }
    }

    // Enregistre la transaction dans un fichier
    void saveTransactionToFile(const Message& msg) {
        std::string filename = "SER_" + msg.clientID + "_" + std::to_string(msg.messageID) + ".mes";
        std::ofstream file("messages/" + filename);

        if (file.is_open()) {
            for (const Ordre& ordre : msg.parseContent()) {
                file << msg.clientID << "," 
                     << ordre.type << "," 
                     << ordre.action << "," 
                     << ordre.quantite << "," 
                     << ordre.prix << "," 
                     << ordre.date << "\n";
            }
            file.close();
            std::cout << "Transaction enregistrée dans : " << filename << std::endl;
        } else {
            std::cerr << "Erreur lors de l'ouverture du fichier de transaction." << std::endl;
        }
    }

    ///t
// Ajoutez cette méthode dans la classe Transaction
void addTransactionRecord(const std::string& clientID, const std::string& action, char type, double prix, int quantite, const std::string& date) {
    TransactionRecord record {clientID, type, prix, quantite, date};
    historique[action].push_back(record);
    // Vous pouvez également ajouter ici une logique pour sauvegarder en fichier si nécessaire.
}

};





#endif // TRANSACTION_H
