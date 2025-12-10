#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

namespace Core
{
    struct MenuItem
    {
        int id;
        std::string name;
        double price;
        std::string image;
        std::string category;
    };

    struct OrderItem
    {
        std::string name;
        int quantity;
        double lineTotal;
    };

    class OrderManager
    {
    public:
        OrderManager()
        {
            initializeMenu();
        }

        const std::vector<MenuItem>& getAllMenuItems() const
        {
            return menuItems;
        }

        std::vector<MenuItem> getItemsByCategory(const std::string& category) const
        {
            std::vector<MenuItem> filtered;
            std::copy_if(menuItems.begin(), menuItems.end(), 
                        std::back_inserter(filtered),
                        [&category](const MenuItem& item) {
                            return item.category == category;
                        });
            return filtered;
        }

        std::vector<std::string> getAllCategories() const
        {
            std::vector<std::string> categories;
            for (const auto& item : menuItems) {
                if (std::find(categories.begin(), categories.end(), item.category) == categories.end()) {
                    categories.push_back(item.category);
                }
            }
            return categories;
        }

        void addItemToOrder(int itemId)
        {
            orderQuantities[itemId]++;
        }

        void clearAllOrders()
        {
            orderQuantities.clear();
        }

        std::vector<OrderItem> getCurrentOrderItems() const
        {
            std::vector<OrderItem> items;
            items.reserve(orderQuantities.size());
            
            for (const auto& [itemId, quantity] : orderQuantities) {
                const MenuItem* item = findMenuItemById(itemId);
                if (item) {
                    items.push_back({
                        item->name,
                        quantity,
                        item->price * quantity
                    });
                }
            }
            return items;
        }

        double calculateOrderTotal() const
        {
            double total = 0.0;
            for (const auto& [itemId, quantity] : orderQuantities) {
                const MenuItem* item = findMenuItemById(itemId);
                if (item) {
                    total += item->price * quantity;
                }
            }
            return total;
        }

    private:
        std::vector<MenuItem> menuItems;
        std::map<int, int> orderQuantities;

        void initializeMenu()
        {
            menuItems = {
                {1, "Bacardi Black", 25.99, "qrc:/assets/Bacardi Black.png", "bottles"},
                {2, "Alla Carbonara", 15.50, "qrc:/assets/Alla Carbonara.png", "foods"}
            };
        }

        const MenuItem* findMenuItemById(int id) const
        {
            auto it = std::find_if(menuItems.begin(), menuItems.end(),
                                  [id](const MenuItem& item) {
                                      return item.id == id;
                                  });
            return (it != menuItems.end()) ? &(*it) : nullptr;
        }
    };
}

class QtOrderManager : public QObject
{
    Q_OBJECT

public:
    QtOrderManager() = default;

    Q_INVOKABLE QVariantList getMenuItems()
    {
        return convertMenuItems(manager.getAllMenuItems());
    }

    Q_INVOKABLE QVariantList getMenuItemsByCategory(const QString& category)
    {
        return convertMenuItems(manager.getItemsByCategory(category.toStdString()));
    }

    Q_INVOKABLE QStringList getCategories()
    {
        QStringList result;
        for (const auto& category : manager.getAllCategories()) {
            result.append(QString::fromStdString(category));
        }
        return result;
    }

    Q_INVOKABLE void addToOrder(int id)
    {
        manager.addItemToOrder(id);
        emit orderChanged();
    }

    Q_INVOKABLE void clearOrder()
    {
        manager.clearAllOrders();
        emit orderChanged();
    }

    Q_INVOKABLE QVariantList getOrderItems()
    {
        QVariantList result;
        for (const auto& item : manager.getCurrentOrderItems()) {
            result.append(QVariantMap{
                {"name", QString::fromStdString(item.name)},
                {"quantity", item.quantity},
                {"lineTotal", item.lineTotal}
            });
        }
        return result;
    }

    Q_INVOKABLE double getOrderTotal()
    {
        return manager.calculateOrderTotal();
    }

signals:
    void orderChanged();

private:
    Core::OrderManager manager;

    QVariantList convertMenuItems(const std::vector<Core::MenuItem>& items) const
    {
        QVariantList result;
        for (const auto& item : items) {
            result.append(QVariantMap{
                {"id", item.id},
                {"name", QString::fromStdString(item.name)},
                {"price", item.price},
                {"image", QString::fromStdString(item.image)},
                {"category", QString::fromStdString(item.category)}
            });
        }
        return result;
    }
};

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    QtOrderManager orderManager;
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("backend", &orderManager);
    engine.loadFromModule("cartmanagerstruct", "Main");
    
    return app.exec();
}

#include "main.moc"
