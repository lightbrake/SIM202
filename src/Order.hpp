#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include "Message.hpp"
#include "Transaction.hpp"

// Classe OrderBook : gère le carnet d'ordres et l'exécution des transactions.
class OrderBook {
private:
    // File d'attente pour les ordres d'achat : tri par prix décroissant.
    std::multimap<double, Ordre, std::greater<double>> buyOrders;
    // File d'attente pour les ordres de vente : tri par prix croissant.
    std::multimap<double, Ordre, std::less<double>> sellOrders;
    
    Transaction transactionManager; // Gestionnaire des transactions.

public:
    // Ajoute les ordres d'un message dans le carnet.
    void addOrder(const Message& msg) {
        std::vector<Ordre> orders = msg.parseContent();
        for (const Ordre& order : orders) {
            if (order.type == 'A') {
                buyOrders.insert({order.prix, order});
            } else if (order.type == 'V') {
                sellOrders.insert({order.prix, order});
            }
        }
        // Après ajout, tente d'exécuter immédiatement les ordres appariés.
        processOrders();
    }
    
    // Traitement des ordres pour réaliser des transactions.
    void processOrders() {
        // Tant qu'il existe au moins un ordre d'achat et un ordre de vente...
        while (!buyOrders.empty() && !sellOrders.empty()) {
            auto bestBuyIt = buyOrders.begin();   // Meilleur ordre d'achat (prix le plus élevé)
            auto bestSellIt = sellOrders.begin();   // Meilleur ordre de vente (prix le plus bas)
            
            // Vérifier si le meilleur ordre d'achat peut couvrir le meilleur ordre de vente.
            if (bestBuyIt->first >= bestSellIt->first) {
                int tradeQuantity = std::min(bestBuyIt->second.quantite, bestSellIt->second.quantite);
                double tradePrice = bestSellIt->first; // Le prix de la transaction est celui du vendeur
                std::string action = bestBuyIt->second.action; // On suppose que l'action est la même

                // Enregistrement de la transaction pour l'acheteur (type 'A')
                transactionManager.addTransactionRecord(bestBuyIt->second.clientID, action, 'A', tradePrice, tradeQuantity, bestBuyIt->second.date);
                // Enregistrement de la transaction pour le vendeur (type 'V')
                transactionManager.addTransactionRecord(bestSellIt->second.clientID, action, 'V', tradePrice, tradeQuantity, bestSellIt->second.date);
                
                std::cout << "[Trade] " << bestBuyIt->second.clientID << " (Achat) et " 
                          << bestSellIt->second.clientID << " (Vente) "
                          << "pour " << tradeQuantity << "x " << action 
                          << " à " << tradePrice << " €" << std::endl;
                
                // Mise à jour des quantités dans les ordres.
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
                // Aucun appariement possible pour l'instant.
                break;
            }
        }
    }
    
    // Affiche les ordres restants dans le carnet.
    void printOrderBook() const {
        std::cout << "\n=== Carnet d'Ordres ===" << std::endl;
        std::cout << "\nOrdres d'Achat (prix décroissant) :" << std::endl;
        for (const auto& pair : buyOrders) {
            const Ordre& o = pair.second;
            std::cout << "Client: " << o.clientID 
                      << ", " << o.quantite << "x " << o.action 
                      << " à " << pair.first << " €" << std::endl;
        }
        
        std::cout << "\nOrdres de Vente (prix croissant) :" << std::endl;
        for (const auto& pair : sellOrders) {
            const Ordre& o = pair.second;
            std::cout << "Client: " << o.clientID 
                      << ", " << o.quantite << "x " << o.action 
                      << " à " << pair.first << " €" << std::endl;
        }
    }
    
    // Affiche l'historique des transactions enregistrées.
    void printTransactions() const {
        transactionManager.printAllTransactions();
    }
};

#endif // ORDERBOOK_H
