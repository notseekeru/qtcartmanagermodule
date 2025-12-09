#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

 class OrderManager : public QObject
{,.
    Q_OBJECT

    QVariantList m_menuItems;
    QMap<int, int> m_orderQuantities;

public:
    OrderManager() {
        m_menuItems = {
            QVariantMap{{"id", 1}, {"name", "Bacardi Black"}, {"price", 25.99}, {"image", "qrc:/assets/Bacardi Black.png"}},
            QVariantMap{{"id", 2}, {"name", "Alla Carbonara"}, {"price", 15.50}, {"image", "qrc:/assets/Alla Carbonara.png"}},
        };
    }

    Q_INVOKABLE QVariantList getMenuItems() {z
        return m_menuItems;
    }

    Q_INVOKABLE void addToOrder(int id)
    {
        m_orderQuantities[id]++;
        emit orderChanged();
    }

    Q_INVOKABLE void clearOrder()
    {
        m_orderQuantities.clear();
        emit orderChanged();
    }

    Q_INVOKABLE QVariantList getOrderItems()
    {
        QVariantList items;
        for (auto orderItem = m_orderQuantities.begin(); orderItem != m_orderQuantities.end(); ++orderItem) {
            QVariantMap menuItem = findMenuItem(orderItem.key());
            items.append(QVariantMap{
                {"name", menuItem["name"]},
                {"quantity", orderItem.value()},
                {"lineTotal", menuItem["price"].toDouble() * orderItem.value()}
            });
        }
        return items;
    }

    Q_INVOKABLE double getOrderTotal()
    {
        double total = 0;
        for (auto orderItem = m_orderQuantities.begin(); orderItem != m_orderQuantities.end(); ++orderItem) {
            QVariantMap menuItem = findMenuItem(orderItem.key());
            total += menuItem["price"].toDouble() * orderItem.value();
        }
        return total;
    }

signals:
    void orderChanged();

private:
    QVariantMap findMenuItem(int id)
    {
        for (int i = 0; i < m_menuItems.size(); i++) {
            QVariantMap menuItem = m_menuItems[i].toMap();
            if (menuItem["id"].toInt() == id) {
                return menuItem;
            }
        }
        return QVariantMap();
    }
};

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    OrderManager orderManager;
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("orderManager", &orderManager);
    engine.loadFromModule("cartmanagerstruct", "Main");
    return app.exec();
}

#include "main.moc"
