#include <iostream>
#include "Message.hpp"
#include "Transaction.hpp"
#include "Order.hpp"


////testtetetee
int main() {
    std::cout << "=== Simulation du Marché en Continu ===\n" << std::endl;

    OrderBook orderBook;

    // ** Phase 1 : Initialisation du marché avec les ordres existants **
    std::cout << "[Ajout des ordres initiaux au carnet]\n";

    Message existingOrders("MarketInit", 0,
        "#A,STOCK,500,149,2025-02-04;"
        "#A,STOCK,2000,148,2025-02-04;"
        "#A,STOCK,1500,147,2025-02-04;"
        "#A,STOCK,1000,146,2025-02-04;"
        "#V,STOCK,1000,150,2025-02-04;"
        "#V,STOCK,1500,151,2025-02-04;"
        "#V,STOCK,2500,152,2025-02-04;"
        "#V,STOCK,1000,153,2025-02-04"
    );

    orderBook.addOrder(existingOrders);

    // ** Affichage du carnet avant l'arrivée du nouvel ordre **
    std::cout << "\n=== Carnet d'ordres avant nouvelle offre ===\n";
    orderBook.printOrderBook();

    // ** Phase 2 : Un client place un gros ordre d'achat **
    std::cout << "\n[Un client place un ordre d'achat de 4000 actions à 152 €]\n";
    Message newOrder("clientX", 1, "#A,STOCK,4000,152,2025-02-04");
    orderBook.addOrder(newOrder);

    // ** Affichage du carnet après exécution de l'ordre **
    std::cout << "\n=== Carnet d'ordres après exécution ===\n";
    orderBook.printOrderBook();

    // ** Affichage des transactions effectuées **
    std::cout << "\n=== Historique des Transactions ===\n";
    orderBook.printTransactions();

    return 0;
}
