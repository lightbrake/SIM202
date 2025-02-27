// #ifndef ORDERBOOK_H
// #define ORDERBOOK_H

// #include <iostream>
// #include <map>
// #include <vector>
// #include <algorithm>
// #include <fstream>
// #include <iomanip>
// #include "Message.h"
// #include "Transaction.h"
// #include "Portefeuille.h"
// #include "Clock.h"

// class OrderBook {
// private:
//     std::multimap<double, Ordre, std::greater<double>> buyOrders;
//     std::multimap<double, Ordre, std::less<double>> sellOrders;
//     Transaction transactionManager;
//     Portefeuille portfolioManager;
//     Clock clock;

//     void savePendingOrdersToFile() const {
//         std::ofstream file("pending_orders.txt", std::ios::app); // Append pour conserver entre sessions
//         if (file.is_open()) {
//             std::string instant = clock.getTime();
//             for (const auto& pair : buyOrders) {
//                 const Ordre& o = pair.second;
//                 file << o.clientID << "," << o.type << "," << o.action << ","
//                      << o.quantite << "," << o.prix << "," << instant << ",0\n";
//             }
//             for (const auto& pair : sellOrders) {
//                 const Ordre& o = pair.second;
//                 file << o.clientID << "," << o.type << "," << o.action << ","
//                      << o.quantite << "," << o.prix << "," << instant << ",0\n";
//             }
//             file.close();
//         }
//     }

//     void loadPendingOrders() {
//         std::ifstream file("pending_orders.txt");
//         if (file.is_open()) {
//             std::string line;
//             while (std::getline(file, line)) {
//                 std::istringstream iss(line);
//                 std::string clientID, action, instant;
//                 char type, comma;
//                 int quantite;
//                 double prix;
//                 int status;
//                 iss >> clientID >> comma >> type >> comma >> action >> comma 
//                     >> quantite >> comma >> prix >> comma >> instant >> comma >> status;
//                 Ordre ordre{type, clientID, action, quantite, prix};
//                 if (type == 'A') buyOrders.insert({prix, ordre});
//                 else if (type == 'V') sellOrders.insert({prix, ordre});
//             }
//             file.close();
//             std::cout << "Ordres en attente chargés depuis la session précédente.\n";
//         }
//     }

//     void processOrders(const std::string& instant) {
//         if (!clock.isContinuous()) return; // Pas de transactions en pré-ouverture/pré-clôture
//         while (!buyOrders.empty() && !sellOrders.empty()) {
//             auto bestBuyIt = buyOrders.begin();
//             auto bestSellIt = sellOrders.begin();
//             if (bestBuyIt->first >= bestSellIt->first) {
//                 int tradeQuantity = std::min(bestBuyIt->second.quantite, bestSellIt->second.quantite);
//                 double tradePrice = bestSellIt->first;
//                 std::string action = bestBuyIt->second.action;

//                 std::cout << "[Heure actuelle avant transaction : " << instant << "]\n";
//                 transactionManager.addTransactionRecord(bestBuyIt->second.clientID, action, 'A', tradePrice, tradeQuantity, instant, false);
//                 transactionManager.addTransactionRecord(bestSellIt->second.clientID, action, 'V', tradePrice, tradeQuantity, instant, false);
//                 portfolioManager.updatePortfolio(bestBuyIt->second.clientID, action, tradeQuantity, true);
//                 portfolioManager.updatePortfolio(bestSellIt->second.clientID, action, tradeQuantity, false);
//                 std::cout << "[Trade] " << bestBuyIt->second.clientID << " (Achat) et " 
//                           << bestSellIt->second.clientID << " (Vente) "
//                           << "pour " << tradeQuantity << "x " << action 
//                           << " à " << tradePrice << " € à " << instant << std::endl;
                
//                 if (bestBuyIt->second.quantite > tradeQuantity) {
//                     Ordre updatedBuy = bestBuyIt->second;
//                     updatedBuy.quantite -= tradeQuantity;
//                     buyOrders.erase(bestBuyIt);
//                     buyOrders.insert({updatedBuy.prix, updatedBuy});
//                 } else {
//                     buyOrders.erase(bestBuyIt);
//                 }
                
//                 if (bestSellIt->second.quantite > tradeQuantity) {
//                     Ordre updatedSell = bestSellIt->second;
//                     updatedSell.quantite -= tradeQuantity;
//                     sellOrders.erase(bestSellIt);
//                     sellOrders.insert({updatedSell.prix, updatedSell});
//                 } else {
//                     sellOrders.erase(bestSellIt);
//                 }
//             } else {
//                 break;
//             }
//         }
//         savePendingOrdersToFile();
//     }

// public:
//     OrderBook() {
//         clock.start(); // Démarre l’horloge
//         loadPendingOrders(); // Charge les ordres de la veille
//     }

//     void addOrder(const Message& msg) {
//         std::string instant = clock.getTime();
//         std::cout << "[Heure actuelle avant ajout : " << instant << "]\n";
//         if (clock.isClosed()) {
//             std::cout << "Marché fermé à " << instant << ". Affichage final :\n";
//             printOrderBook();
//             printTransactions();
//             std::cout << "Fin de la journée boursière.\n";
//             exit(0);
//         }
//         if (clock.isPreClosing()) {
//             std::cout << "Marché en pré-clôture à " << instant << ". Ordres accumulés.\n";
//         }
//         std::vector<Ordre> orders = msg.parseContent();
//         for (const Ordre& order : orders) {
//             if (order.type == 'A') buyOrders.insert({order.prix, order});
//             else if (order.type == 'V') sellOrders.insert({order.prix, order});
//         }
//         processOrders(instant);
//     }
    
//     void printOrderBook() const {
//         // std::cout << "\n=== Carnet d'Ordres à " << clock.getTime() << " ===" << std::endl;
//         // std::cout << "\nOrdres d'Achat (prix décroissant) :" << std::endl;
//         // for (const auto& pair : buyOrders) {
//         //     const Ordre& o = pair.second;
//         //     std::cout << "Client: " << o.clientID 
//         //               << ", " << o.quantite << "x " << o.action 
//         //               << " à " << pair.first << " €" << std::endl;
//         // }
//         // std::cout << "\nOrdres de Vente (prix croissant) :" << std::endl;
//         // for (const auto& pair : sellOrders) {
//         //     const Ordre& o = pair.second;
//         //     std::cout << "Client: " << o.clientID 
//         //               << ", " << o.quantite << "x " << o.action 
//         //               << " à " << pair.first << " €" << std::endl;
//         // }
// std::cout << "\n=== Carnet d'Ordres à " << clock.getTime() << " ===\n";
    
//     std::cout << "\nOrdres d'Achat:\n";
//     std::cout << std::setw(15) << "Client" << "    " << std::setw(12) << "Quantité" << "        " << std::setw(10) << "Prix (€)\n";
//     for (const auto& pair : buyOrders) {
//         const Ordre& o = pair.second;
//         std::cout << std::setw(15) << o.clientID << "  " << std::setw(12) << o.quantite << "    " << std::setw(10) << pair.first << "\n";
//     }
    
//     std::cout << "\nOrdres de Vente:\n";
//     std::cout << std::setw(15) << "Client" << "    " << std::setw(12) << "Quantité" << "        " << std::setw(10) << "Prix (€)\n";
//     for (const auto& pair : sellOrders) {
//         const Ordre& o = pair.second;
//         std::cout << std::setw(15) << o.clientID << "  " << std::setw(12) << o.quantite << "    " << std::setw(10) << pair.first << "\n";
//     }
//     }
    
//     void printTransactions() const {
//         transactionManager.printAllTransactions();
//     }

//     Portefeuille& getPortfolioManager() {
//         return portfolioManager;
//     }

//     bool isClosed() const { // Ajout pour main.cpp
//         return clock.isClosed();
//     }
// };

// #endif // ORDERBOOK_H
#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include "Message.h"
#include "Transaction.h"
#include "Portefeuille.h"
#include "Clock.h"

class OrderBook {
private:
    std::multimap<double, Ordre, std::greater<double>> buyOrders;  // Ordres limités d'achat
    std::multimap<double, Ordre, std::less<double>> sellOrders;    // Ordres limités de vente
    std::vector<Ordre> buyMarketOrders;                            // Ordres "à tout prix" d'achat
    std::vector<Ordre> sellMarketOrders;                           // Ordres "à tout prix" de vente
    Transaction transactionManager;
    Portefeuille portfolioManager;
    Clock clock;
    double lastPrice;  // Dernier cours, initialisé dynamiquement

    void savePendingOrdersToFile() const {
        std::ofstream file("pending_orders.txt", std::ios::app);
        if (file.is_open()) {
            std::string instant = clock.getTime();
            for (const auto& pair : buyOrders) {
                const Ordre& o = pair.second;
                file << o.clientID << "," << o.type << "," << o.action << ","
                     << o.quantite << "," << o.prix << "," << instant << ",0\n";
            }
            for (const auto& o : buyMarketOrders) {
                file << o.clientID << "," << o.type << "," << o.action << ","
                     << o.quantite << ",-1," << instant << ",0\n";
            }
            for (const auto& pair : sellOrders) {
                const Ordre& o = pair.second;
                file << o.clientID << "," << o.type << "," << o.action << ","
                     << o.quantite << "," << o.prix << "," << instant << ",0\n";
            }
            for (const auto& o : sellMarketOrders) {
                file << o.clientID << "," << o.type << "," << o.action << ","
                     << o.quantite << ",-1," << instant << ",0\n";
            }
            file.close();
        }
    }

    void loadPendingOrders() {
        std::ifstream file("pending_orders.txt");
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string clientID, action, instant;
                char type, comma;
                int quantite;
                double prix;
                int status;
                iss >> clientID >> comma >> type >> comma >> action >> comma 
                    >> quantite >> comma >> prix >> comma >> instant >> comma >> status;
                Ordre ordre{type, clientID, action, quantite, prix};
                if (type == 'A') {
                    if (prix == -1) buyMarketOrders.push_back(ordre);
                    else buyOrders.insert({prix, ordre});
                } else if (type == 'V') {
                    if (prix == -1) sellMarketOrders.push_back(ordre);
                    else sellOrders.insert({prix, ordre});
                }
            }
            file.close();
            std::cout << "Ordres en attente chargés depuis la session précédente.\n";
        }
    }

    void processMarketOrders(const std::string& instant) {
        if (!clock.isContinuous()) return;

        // 1. Apparier les ordres "à tout prix" entre eux
        while (!buyMarketOrders.empty() && !sellMarketOrders.empty()) {
            Ordre& buyOrder = buyMarketOrders.back();
            Ordre& sellOrder = sellMarketOrders.back();
            int tradeQuantity = std::min(buyOrder.quantite, sellOrder.quantite);
            double tradePrice = lastPrice; // Utiliser le dernier cours connu
            std::string action = buyOrder.action;

            transactionManager.addTransactionRecord(buyOrder.clientID, action, 'A', tradePrice, tradeQuantity, instant, false);
            transactionManager.addTransactionRecord(sellOrder.clientID, action, 'V', tradePrice, tradeQuantity, instant, false);
            portfolioManager.updatePortfolio(buyOrder.clientID, action, tradeQuantity, true);
            portfolioManager.updatePortfolio(sellOrder.clientID, action, tradeQuantity, false);
            std::cout << "[Trade] " << buyOrder.clientID << " (Achat à tout prix) et " 
                      << sellOrder.clientID << " (Vente à tout prix) pour " << tradeQuantity << "x " << action 
                      << " à " << tradePrice << " € à " << instant << std::endl;

            buyOrder.quantite -= tradeQuantity;
            sellOrder.quantite -= tradeQuantity;
            if (buyOrder.quantite == 0) buyMarketOrders.pop_back();
            if (sellOrder.quantite == 0) sellMarketOrders.pop_back();
        }

        // 2. Traiter les achats "à tout prix" contre les ordres limités de vente
        while (!buyMarketOrders.empty() && !sellOrders.empty()) {
            Ordre& buyOrder = buyMarketOrders.back();
            auto bestSellIt = sellOrders.begin();
            int tradeQuantity = std::min(buyOrder.quantite, bestSellIt->second.quantite);
            double tradePrice = bestSellIt->first;
            std::string action = buyOrder.action;

            transactionManager.addTransactionRecord(buyOrder.clientID, action, 'A', tradePrice, tradeQuantity, instant, false);
            transactionManager.addTransactionRecord(bestSellIt->second.clientID, action, 'V', tradePrice, tradeQuantity, instant, false);
            portfolioManager.updatePortfolio(buyOrder.clientID, action, tradeQuantity, true);
            portfolioManager.updatePortfolio(bestSellIt->second.clientID, action, tradeQuantity, false);
            lastPrice = tradePrice;

            buyOrder.quantite -= tradeQuantity;
            if (buyOrder.quantite == 0) buyMarketOrders.pop_back();
            if (bestSellIt->second.quantite > tradeQuantity) {
                Ordre updatedSell = bestSellIt->second;
                updatedSell.quantite -= tradeQuantity;
                sellOrders.erase(bestSellIt);
                sellOrders.insert({updatedSell.prix, updatedSell});
            } else {
                sellOrders.erase(bestSellIt);
            }
        }

        // 3. Traiter les ventes "à tout prix" contre les ordres limités d’achat
        while (!sellMarketOrders.empty() && !buyOrders.empty()) {
            Ordre& sellOrder = sellMarketOrders.back();
            auto bestBuyIt = buyOrders.begin();
            int tradeQuantity = std::min(sellOrder.quantite, bestBuyIt->second.quantite);
            double tradePrice = bestBuyIt->first;
            std::string action = sellOrder.action;

            transactionManager.addTransactionRecord(bestBuyIt->second.clientID, action, 'A', tradePrice, tradeQuantity, instant, false);
            transactionManager.addTransactionRecord(sellOrder.clientID, action, 'V', tradePrice, tradeQuantity, instant, false);
            portfolioManager.updatePortfolio(bestBuyIt->second.clientID, action, tradeQuantity, true);
            portfolioManager.updatePortfolio(sellOrder.clientID, action, tradeQuantity, false);
            lastPrice = tradePrice;

            sellOrder.quantite -= tradeQuantity;
            if (sellOrder.quantite == 0) sellMarketOrders.pop_back();
            if (bestBuyIt->second.quantite > tradeQuantity) {
                Ordre updatedBuy = bestBuyIt->second;
                updatedBuy.quantite -= tradeQuantity;
                buyOrders.erase(bestBuyIt);
                buyOrders.insert({updatedBuy.prix, updatedBuy});
            } else {
                buyOrders.erase(bestBuyIt);
            }
        }
    }

    void processOrders(const std::string& instant) {
        if (!clock.isContinuous()) return;
        processMarketOrders(instant); // Traiter les ordres "à tout prix" d’abord
        while (!buyOrders.empty() && !sellOrders.empty()) {
            auto bestBuyIt = buyOrders.begin();
            auto bestSellIt = sellOrders.begin();
            if (bestBuyIt->first >= bestSellIt->first) {
                int tradeQuantity = std::min(bestBuyIt->second.quantite, bestSellIt->second.quantite);
                double tradePrice = bestSellIt->first;
                std::string action = bestBuyIt->second.action;

                std::cout << "[Heure actuelle avant transaction : " << instant << "]\n";
                transactionManager.addTransactionRecord(bestBuyIt->second.clientID, action, 'A', tradePrice, tradeQuantity, instant, false);
                transactionManager.addTransactionRecord(bestSellIt->second.clientID, action, 'V', tradePrice, tradeQuantity, instant, false);
                portfolioManager.updatePortfolio(bestBuyIt->second.clientID, action, tradeQuantity, true);
                portfolioManager.updatePortfolio(bestSellIt->second.clientID, action, tradeQuantity, false);
                lastPrice = tradePrice;
                std::cout << "[Trade] " << bestBuyIt->second.clientID << " (Achat) et " 
                          << bestSellIt->second.clientID << " (Vente) "
                          << "pour " << tradeQuantity << "x " << action 
                          << " à " << tradePrice << " € à " << instant << std::endl;

                if (bestBuyIt->second.quantite > tradeQuantity) {
                    Ordre updatedBuy = bestBuyIt->second;
                    updatedBuy.quantite -= tradeQuantity;
                    buyOrders.erase(bestBuyIt);
                    buyOrders.insert({updatedBuy.prix, updatedBuy});
                } else {
                    buyOrders.erase(bestBuyIt);
                }
                if (bestSellIt->second.quantite > tradeQuantity) {
                    Ordre updatedSell = bestSellIt->second;
                    updatedSell.quantite -= tradeQuantity;
                    sellOrders.erase(bestSellIt);
                    sellOrders.insert({updatedSell.prix, updatedSell});
                } else {
                    sellOrders.erase(bestSellIt);
                }
            } else {
                break;
            }
        }
        savePendingOrdersToFile();
    }

public:
    OrderBook() {
        clock.start();
        loadPendingOrders();
        // Initialiser lastPrice au plus petit prix de vente disponible
        if (!sellOrders.empty()) {
            lastPrice = sellOrders.begin()->first;
            std::cout << "[Initialisation] lastPrice défini à " << lastPrice << " (plus petit prix de vente)\n";
        } else {
            lastPrice = 150.0;
            std::cout << "[Initialisation] Aucun ordre de vente limité, lastPrice par défaut = " << lastPrice << "\n";
        }
    }

    void addOrder(const Message& msg) {
        std::string instant = clock.getTime();
        std::cout << "[Heure actuelle avant ajout : " << instant << "]\n";
        if (clock.isClosed()) {
            std::cout << "Marché fermé à " << instant << ". Affichage final :\n";
            printOrderBook();
            printTransactions();
            std::cout << "Fin de la journée boursière.\n";
            exit(0);
        }
        if (clock.isPreClosing()) {
            std::cout << "Marché en pré-clôture à " << instant << ". Ordres accumulés.\n";
        }
        std::vector<Ordre> orders = msg.parseContent();
        for (const Ordre& order : orders) {
            if (order.type == 'A') {
                if (order.prix == -1) buyMarketOrders.push_back(order);
                else buyOrders.insert({order.prix, order});
            } else if (order.type == 'V') {
                if (order.prix == -1) sellMarketOrders.push_back(order);
                else sellOrders.insert({order.prix, order});
            }
        }
        processOrders(instant);
    }
    
    void printOrderBook() const {
        std::cout << "\n=== Carnet d'Ordres à " << clock.getTime() << " ===\n";
        std::cout << "\nOrdres d'Achat:\n";
        std::cout << std::setw(15) << "Client" << "   " << std::setw(12) << "Quantité" << "    " << std::setw(10) << "Prix (€)\n";
        for (const auto& o : buyMarketOrders) {
            std::cout << std::setw(15) << o.clientID << " " << std::setw(12) << o.quantite << " " << std::setw(10) << "MKT\n";
        }
        for (const auto& pair : buyOrders) {
            const Ordre& o = pair.second;
            std::cout << std::setw(15) << o.clientID << " " << std::setw(12) << o.quantite << " " << std::setw(10) << pair.first << "\n";
        }
        std::cout << "\nOrdres de Vente:\n";
        std::cout << std::setw(15) << "Client" << "   " << std::setw(12) << "Quantité" << "    " << std::setw(10) << "Prix (€)\n";
        for (const auto& o : sellMarketOrders) {
            std::cout << std::setw(15) << o.clientID << " " << std::setw(12) << o.quantite << " " << std::setw(10) << "MKT\n";
        }
        for (const auto& pair : sellOrders) {
            const Ordre& o = pair.second;
            std::cout << std::setw(15) << o.clientID << " " << std::setw(12) << o.quantite << " " << std::setw(10) << pair.first << "\n";
        }
    }
    
    void printTransactions() const {
        transactionManager.printAllTransactions();
    }

    Portefeuille& getPortfolioManager() {
        return portfolioManager;
    }

    bool isClosed() const { 
        return clock.isClosed();
    }

    Clock& getClock() {return clock;}
};

#endif