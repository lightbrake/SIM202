#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include "message.hpp"
#include "Transaction.hpp"

#define MESSAGE_FOLDER "messages/"

// Simule l'écriture d'un message dans un fichier par un client
void clientSendMessage(const Message& msg) {
    std::string filename = MESSAGE_FOLDER + std::string("CLI_") + msg.clientID + "_" + std::to_string(msg.messageID) + ".mes";
    std::ofstream file(filename);

    if (file.is_open()) {
        file << msg.toString(); // Écrit le message dans un fichier
        file.close();
        std::cout << "[Client] Message envoyé : " << filename << std::endl;
    } else {
        std::cerr << "[Client] Erreur lors de l'envoi du message." << std::endl;
    }
}

// Simule la lecture d'un message par le serveur
Message serverReceiveMessage(const std::string& clientID, int messageID) {
    std::string filename = MESSAGE_FOLDER + std::string("CLI_") + clientID + "_" + std::to_string(messageID) + ".mes";
    std::ifstream file(filename);
    std::string content;
    
    if (file.is_open()) {
        std::getline(file, content);
        file.close();
        std::cout << "[Serveur] Message reçu : " << content << std::endl;
        return Message(clientID, messageID, content);
    } else {
        std::cerr << "[Serveur] Erreur : fichier message introuvable." << std::endl;
        return Message("", 0, "");
    }
}

// Simule le traitement d'un message en transaction par le serveur
void serverProcessTransaction(Transaction& transactionManager, const Message& msg) {
    if (!msg.validate()) {
        std::cerr << "[Serveur] Message invalide, rejeté." << std::endl;
        return;
    }

    transactionManager.addTransaction(msg); // Convertit le message en transaction
    std::cout << "[Serveur] Transaction enregistrée." << std::endl;
}

// Simule l'affichage de l'historique des transactions
void serverShowTransactions(const Transaction& transactionManager) {
    std::cout << "\n=== Historique des Transactions ===" << std::endl;
    transactionManager.printAllTransactions();
}

// Programme principal (test du serveur)
int main() {
    std::cout << "=== Simulation Serveur Alpha ===\n" << std::endl;

    // Crée un dossier "messages/" si inexistant (Unix/Linux)
    system("mkdir -p mess");

    // Simule un message client
    Message msg1("client123", 1, "#A,TSLA,10,750.5,2025-02-04;#V,AAPL,5,150.0,2025-02-04");
    
    // Envoi du message (écriture dans un fichier)
    clientSendMessage(msg1);
    
    // Attente pour simuler le délai de transmission
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Serveur reçoit le message
    Message receivedMsg = serverReceiveMessage("client123", 1);
    
    // Création du gestionnaire de transactions
    Transaction transactionManager;

    // Traitement et stockage de la transaction
    serverProcessTransaction(transactionManager, receivedMsg);

    // Affichage de l'historique des transactions
    serverShowTransactions(transactionManager);

    return 0;
}
