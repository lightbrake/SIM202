#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Message.h"

struct TransactionRecord {
    std::string clientID;
    char type;
    double prix;
    int quantite;
    std::string instant; // Remplacement de date
    bool enCours;
};

class Transaction {
private:
    std::map<std::string, std::vector<TransactionRecord>> historique;// action -> liste de transactions

public:
    void addTransaction(const Message& msg, const std::string& instant) {
        std::vector<Ordre> ordres = msg.parseContent();
        for (const Ordre& ordre : ordres) {
            TransactionRecord record {msg.clientID, ordre.type, ordre.prix, ordre.quantite, instant, true};
            historique[ordre.action].push_back(record);
        }
        saveTransactionToFile(msg, instant);
    }

    void printAllTransactions() const {
        // for (const auto& pair : historique) {
        //     std::cout << "Action: " << pair.first << std::endl;
        //     for (const auto& record : pair.second) {
        //         std::cout << "  Client: " << record.clientID
        //                   << ", Type: " << record.type 
        //                   << ", Prix: " << record.prix 
        //                   << ", Quantité: " << record.quantite 
        //                   << ", Instant: " << record.instant // Remplacement de Date
        //                   << ", En cours: " << (record.enCours ? "Oui" : "Non") << std::endl;
        //     }
        // }
    std::cout << std::setw(15) << "Action" << std::setw(15) << "Client"<< "  " << std::setw(8) << "Type"<< "   "
              << std::setw(12) << "Prix (€)" << std::setw(12) << "Quantité" << std::setw(15) << "Instant"
              << std::setw(12) << "En cours" << "\n";
    for (const auto& pair : historique) {
        for (const auto& record : pair.second) {
            std::cout << std::setw(15) << pair.first << std::setw(15) << record.clientID << std::setw(8) << record.type
                      << std::setw(12) << record.prix << std::setw(12) << record.quantite << std::setw(15) << record.instant
                      << std::setw(12) << (record.enCours ? "Oui" : "Non") << "\n";
        }
    }
    }

    void saveTransactionToFile(const Message& msg, const std::string& instant) {
        std::string filename = "SER_" + msg.clientID + "_" + std::to_string(msg.messageID) + ".mes";
        std::ofstream file("messages/" + filename);
        if (file.is_open()) {
            for (const Ordre& ordre : msg.parseContent()) {
                file << msg.clientID << "," << ordre.type << "," << ordre.action << ","
                     << ordre.quantite << "," << ordre.prix << "," << instant << "\n";
            }
            file.close();
            std::cout << "Transaction enregistrée dans : " << filename << std::endl;
        }
    }

    void addTransactionRecord(const std::string& clientID, const std::string& action, char type, double prix, int quantite, const std::string& instant, bool enCours) {
        TransactionRecord record {clientID, type, prix, quantite, instant, enCours};
        historique[action].push_back(record);
        saveTransactionToFile(clientID, action, type, prix, quantite, instant, enCours);
    }

    void saveTransactionToFile(const std::string& clientID, const std::string& action, char type, double prix, int quantite, const std::string& instant, bool enCours) {
        std::ofstream file("transactions.txt", std::ios::app);
        if (file.is_open()) {
            file << clientID << "," << type << "," << action << ","
                 << quantite << "," << prix << "," << instant << "," << enCours << "\n";
            file.close();
        }
    }
};

#endif