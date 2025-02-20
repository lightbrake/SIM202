#include <iostream>
#include "message.hpp"
#include "Transaction.hpp"

int main() {
    std::cout << "=== Test des Classes Message et Transaction ===\n" << std::endl;

    // 1. Création d'un message client contenant plusieurs ordres
    Message msg1("client123", 1, "#A,TSLA,10,750.5,2025-02-04;#V,AAPL,5,150.0,2025-02-04");

    // 2. Vérification du message
    if (!msg1.validate()) {
        std::cerr << "[Erreur] Message invalide !" << std::endl;
        return 1;
    }

    // 3. Affichage du message
    std::cout << "[Client] Message envoyé : " << msg1.toString() << std::endl;

    // 4. Extraction des ordres depuis le message
    std::vector<Ordre> ordres = msg1.parseContent();

    std::cout << "\n[Serveur] Ordres extraits du message :\n";
    for (const auto& ordre : ordres) {
        std::cout << "  Type: " << ordre.type
                  << ", Action: " << ordre.action
                  << ", Quantité: " << ordre.quantite
                  << ", Prix: " << ordre.prix
                  << ", Date: " << ordre.date << std::endl;
    }

    // 5. Création du gestionnaire de transactions
    Transaction transactionManager;

    // 6. Ajout des transactions à partir du message
    transactionManager.addTransaction(msg1);

    // 7. Affichage des transactions enregistrées
    std::cout << "\n=== Historique des Transactions ===" << std::endl;
    transactionManager.printAllTransactions();

}
