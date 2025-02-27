// #include <iostream>
// #include <string>
// #include <sstream>
// #include <thread>
// #include <windows.h>
// #include "OrderBook.h"

// std::string buildOrderContent(char type, const std::string& action, int quantite, double prix) {
//     std::ostringstream oss;
//     oss << "#" << type << "," << action << "," << quantite << "," << prix;
//     return oss.str();
// }

// int main() {
//     SetConsoleOutputCP(CP_UTF8);
//     std::cout << "=== Simulation du Marché en Continu ===\n" << std::endl;

//     OrderBook orderBook;

//     // Phase 1 : Initialisation du marché
//     std::cout << "[Ajout des ordres initiaux au carnet]\n";
//     Message existingOrders("MarketInit", 0,
//         "#A,STOCK,500,149;"
//         "#A,STOCK,2000,148;"
//         "#A,STOCK,1500,147;"
//         "#A,STOCK,1000,146;"
//         "#V,STOCK,1000,150;"
//         "#V,STOCK,1500,151;"
//         "#V,STOCK,2500,152;"
//         "#V,STOCK,1000,153"
//     );
//     orderBook.addOrder(existingOrders);

//     std::cout << "\n=== Carnet d'ordres avant nouvelle offre ===\n";
//     orderBook.printOrderBook();

//     // Attendre 10h00 (90 secondes = 90 minutes depuis 8h30)
//     std::cout << "Attente jusqu'à 10h00...\n";
//     std::this_thread::sleep_for(std::chrono::seconds(95));

//     // Phase 2 : Gros ordre d'achat
//     std::cout << "\n[Un client place un ordre d'achat de 4000 actions à 152 €]\n";
//     Message newOrder("clientX", 1, "#A,STOCK,4000,152");
//     orderBook.addOrder(newOrder);

//     std::cout << "\n=== Carnet d'ordres après exécution ===\n";
//     orderBook.printOrderBook();

//     std::cout << "\n=== Historique des Transactions ===\n";
//     orderBook.printTransactions();

//     std::cout << "\n=== Portefeuille après exécution initiale ===\n";
//     orderBook.getPortfolioManager().printPortfolio();

//     // // Phase 3 : Mode interactif
//     // std::cout << "\n=== Passage en mode interactif ===\n";
//     // while (true) {
//     //     std::cout << "\n=== Ajouter un nouvel ordre ===\n";
//     //     std::string clientID;
//     //     char type;
//     //     std::string action;
//     //     int quantite;
//     //     double prix;

//     //     std::cout << "Entrez votre ID client : ";
//     //     std::cin >> clientID;

//     //     std::cout << "Type d'ordre (A pour Achat, V pour Vente) : ";
//     //     std::cin >> type;
//     //     type = toupper(type);
//     //     if (type != 'A' && type != 'V') {
//     //         std::cout << "Type invalide. Utilisez 'A' ou 'V'.\n";
//     //         continue;
//     //     }

//     //     std::cout << "Action (ex: STOCK) : ";
//     //     std::cin >> action;

//     //     std::cout << "Quantité : ";
//     //     std::cin >> quantite;
//     //     if (quantite <= 0) {
//     //         std::cout << "La quantité doit être positive.\n";
//     //         continue;
//     //     }

//     //     std::cout << "Prix : ";
//     //     std::cin >> prix;
//     //     if (prix <= 0) {
//     //         std::cout << "Le prix doit être positif.\n";
//     //         continue;
//     //     }

//     //     if (type == 'V') {
//     //         if (!orderBook.getPortfolioManager().hasEnoughShares(clientID, action, quantite)) {
//     //             std::cout << "Erreur : Pas assez de " << action << " pour vendre " << quantite << " unités.\n";
//     //             orderBook.getPortfolioManager().printPortfolio();
//     //             continue;
//     //         }
//     //     }

//     //     std::string content = buildOrderContent(type, action, quantite, prix);
//     //     Message manualOrder(clientID, rand() % 1000 + 1, content);

//     //     std::cout << "\nAjout de l'ordre au carnet...\n";
//     //     orderBook.addOrder(manualOrder);

//     //     std::cout << "\n=== Carnet d'ordres actuel ===\n";
//     //     orderBook.printOrderBook();

//     //     std::cout << "\n=== Historique des transactions ===\n";
//     //     orderBook.printTransactions();

//     //     std::cout << "\n=== Portefeuille mis à jour ===\n";
//     //     orderBook.getPortfolioManager().printPortfolio();

//     //     std::cout << "\nAjouter un autre ordre ? (o/n) : ";
//     //     char continuer;
//     //     std::cin >> continuer;
//     //     if (tolower(continuer) != 'o') break;
//     // }

//     // // Attendre la fermeture automatique
//     // std::cout << "Simulation en cours jusqu'à 17h05... (1s = 1min simulée)\n";
//     // while (!orderBook.isClosed()) {
//     //     std::this_thread::sleep_for(std::chrono::seconds(1));
//     // }

//     return 0;}
    
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>
#include <windows.h>
#include "OrderBook.h"

std::string buildOrderContent(char type, const std::string& action, int quantite, double prix) {
    std::ostringstream oss;
    if (prix == -1) {
        oss << "#" << type << "," << action << "," << quantite << ",MKT";
    } else {
        oss << "#" << type << "," << action << "," << quantite << "," << prix;
    }
    return oss.str();
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "=== Simulation du Marché en Continu ===\n" << std::endl;

    OrderBook orderBook;

    // Phase 1 : Initialisation du marché
    std::cout << "[Ajout des ordres initiaux au carnet]\n";
    Message existingOrders("MarketInit", 0,
        "#A,STOCK,500,149;"
        "#A,STOCK,2000,148;"
        "#A,STOCK,1500,147;"
        "#A,STOCK,1000,146;"
        "#V,STOCK,1000,150;"
        "#V,STOCK,1500,151;"
        "#V,STOCK,2500,152;"
        "#V,STOCK,1000,153"
    );
    orderBook.addOrder(existingOrders);

    std::cout << "\n=== Carnet d'ordres avant nouvelle offre ===\n";
    orderBook.printOrderBook();

    // Attendre 10h00 pour phase continue
    std::cout << "Attente jusqu'à 10h00...\n";
    // std::this_thread::sleep_for(std::chrono::seconds(95));

    // Phase 2 : Gros ordre d'achat "à tout prix"
    std::cout << "\n[Un client place un ordre d'achat de 4000 actions à tout prix]\n";
    Message newOrder("clientX", 1, "#A,STOCK,4000,MKT");
    orderBook.addOrder(newOrder);

    std::cout << "\n=== Carnet d'ordres après exécution ===\n";
    orderBook.printOrderBook();

    std::cout << "\n=== Historique des Transactions ===\n";
    orderBook.printTransactions();

    std::cout << "\n=== Portefeuille après exécution initiale ===\n";
    orderBook.getPortfolioManager().printPortfolio();

    // Phase 3 : Mode interactif
    char mode;
    std::cout << "tapez M si vous souhaitez utiliser le mode manuel et A si vous souhaitez accèder le mode automatique"<<std::endl;
    std::cin >> mode;
    std::cout << "\n=== Passage en mode manuel interactif ===\n";
    while (mode=='M') {
        std::cout << "\n=== Ajouter un nouvel ordre ===\n";
        std::string clientID;
        char type;
        std::string action;
        int quantite;
        double prix;

        std::cout << "Entrez votre ID client : ";
        std::cin >> clientID;

        std::cout << "Type d'ordre (A pour Achat, V pour Vente) : ";
        std::cin >> type;
        type = toupper(type);
        if (type != 'A' && type != 'V') {
            std::cout << "Type invalide. Utilisez 'A' ou 'V'.\n";
            continue;
        }

        std::cout << "Action (ex: STOCK) : ";
        std::cin >> action;

        std::cout << "Quantité : ";
        std::cin >> quantite;
        if (quantite <= 0) {
            std::cout << "La quantité doit être positive.\n";
            continue;
        }

        std::cout << "Prix (ou 'MKT' pour à tout prix) : ";
        std::string prixInput;
        std::cin >> prixInput;
        if (prixInput == "MKT") {
            prix = -1;
        } else {
            prix = std::stod(prixInput);
            if (prix <= 0) {
                std::cout << "Le prix doit être positif.\n";
                continue;
            }
        }

        if (type == 'V' && prix != -1) { // Pas de vérification pour "MKT"
            if (!orderBook.getPortfolioManager().hasEnoughShares(clientID, action, quantite)) {
                std::cout << "Erreur : Pas assez de " << action << " pour vendre " << quantite << " unités.\n";
                orderBook.getPortfolioManager().printPortfolio();
                continue;
            }
        }

        std::string content = buildOrderContent(type, action, quantite, prix);
        Message manualOrder(clientID, rand() % 1000 + 1, content);

        std::cout << "\nAjout de l'ordre au carnet...\n";
        orderBook.addOrder(manualOrder);

        std::cout << "\n=== Carnet d'ordres actuel ===\n";
        orderBook.printOrderBook();

        std::cout << "\n=== Historique des transactions ===\n";
        orderBook.printTransactions();

        std::cout << "\n=== Portefeuille mis à jour ===\n";
        orderBook.getPortfolioManager().printPortfolio();
        char aut;
        std::cout << "\n Passer au mode automatique ? (o/n) : ";
        std::cin >> aut;
        if (tolower(aut)=='o') {mode='A'; break;}
        std::cout << "\nAjouter un autre ordre ? (o/n) : ";
        char continuer;
        std::cin >> continuer;
        if (tolower(continuer) != 'o') break;
    }

    if (mode == 'A'){
        //orderBook.getClock().stop();
        std::string clientID;
        char type;
        std::string action;
        int quantite;
        double prix;
        std::cout << "Entrez votre ID client : ";
        std::cin >> clientID;

        std::cout << "Type d'ordre (A pour Achat, V pour Vente) : ";
        std::cin >> type;
        type = toupper(type);
        while (type != 'A' && type != 'V') {
            std::cout << "Type invalide. Utilisez 'A' ou 'V'.\n";
            std::cin >> type;
            type=toupper(type);
        }

        std::cout << "Action (ex: STOCK) : ";
        std::cin >> action;

        std::cout << "Quantité par transaction: ";
        std::cin >> quantite;
        while (quantite <= 0) {
            std::cout << "La quantité doit être positive.\n";
            std::cin >> quantite;
        }

        std::cout << "Prix (ou 'MKT' pour à tout prix) : ";
        std::string prixInput;
        std::cin >> prixInput;
        if (prixInput == "MKT") {
            prix = -1;
        } else {
            prix = std::stod(prixInput);
            while (prix <= 0) {
                std::cout << "Le prix doit être positif.\n";
                std::cin >> prixInput;
                if (prixInput == "MKT"){prix=-1;break;}
                prix=std::stod(prixInput);
            }
        }
        unsigned int T;
        std::cout << "Donner la période entre deux transactions successives\n"; 
        std::cin >> T;
        while (!orderBook.isClosed()){
            if (type == 'V' && prix != -1) { // Pas de vérification pour "MKT"
                if (!orderBook.getPortfolioManager().hasEnoughShares(clientID, action, quantite)) {
                    std::cout << "Erreur : Pas assez de " << action << " pour vendre " << quantite << " unités.\n";
                    orderBook.getPortfolioManager().printPortfolio();
                    continue;
                }
            }
            std::string content = buildOrderContent(type, action, quantite, prix);
            Message autoOrder(clientID, rand() % 1000 + 1, content);

            std::cout << "\nAjout de l'ordre au carnet...\n";
            orderBook.addOrder(autoOrder);

            std::cout << "\n=== Carnet d'ordres actuel ===\n";
            orderBook.printOrderBook();

            std::cout << "\n=== Historique des transactions ===\n";
            orderBook.printTransactions();

            std::cout << "\n=== Portefeuille mis à jour ===\n";
            orderBook.getPortfolioManager().printPortfolio();

            Clock clock = orderBook.getClock();
            clock.Timeskip(T);
            
            
        }


    }


    std::cout << "Simulation en cours jusqu'à 17h05... (1s = 1min simulée)\n";
    while (!orderBook.isClosed()){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}