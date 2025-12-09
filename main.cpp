#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QAbstractListModel>

// Product data structure
struct Product {
    int id;
    QString name;
    double price;
    QString image;
};

// Product list model for QML
class ProductModel : public QAbstractListModel {
    Q_OBJECT
    QList<Product> products;

public:
    enum { ProductIdRole = Qt::UserRole + 1, NameRole, PriceRole, ImageRole };

    ProductModel(QObject *parent = nullptr) : QAbstractListModel(parent) {
        products = {
            {1, "Bacardi Black", 25.99, "qrc:/assets/Bacardi Black.png"},
            {2, "Alla Carbonara", 15.50, "qrc:/assets/Alla Carbonara.png"},
            {3, "Burger", 8.99, ""},
            {4, "Pizza", 12.50, ""},
            {5, "Fries", 3.99, ""},
            {6, "Soda", 1.99, ""},
        };
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        return parent.isValid() ? 0 : products.count();
    }

    QVariant data(const QModelIndex &index, int role) const override {
        if (!index.isValid() || index.row() >= products.count())
            return {};
        
        const Product &p = products[index.row()];
        if (role == ProductIdRole) return p.id;
        if (role == NameRole) return p.name;
        if (role == PriceRole) return p.price;
        if (role == ImageRole) return p.image;
        return {};
    }

    QHash<int, QByteArray> roleNames() const override {
        return {{ProductIdRole, "productId"}, {NameRole, "name"}, {PriceRole, "price"}, {ImageRole, "image"}};
    }

    const Product* findProduct(int id) const {
        for (const Product &p : products)
            if (p.id == id) return &p;
        return nullptr;
    }
};

// Cart manager
class CartManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList cartItems READ cartItems NOTIFY cartChanged)
    Q_PROPERTY(int itemCount READ itemCount NOTIFY cartChanged)
    Q_PROPERTY(double totalPrice READ totalPrice NOTIFY cartChanged)

    ProductModel *productModel;
    QMap<int, int> quantities;

public:
    CartManager(ProductModel *model, QObject *parent = nullptr) 
        : QObject(parent), productModel(model) {}

    Q_INVOKABLE void addProduct(int id) {
        if (productModel->findProduct(id)) {
            quantities[id]++;
            emit cartChanged();
        }
    }

    Q_INVOKABLE void clearCart() {
        if (!quantities.isEmpty()) {
            quantities.clear();
            emit cartChanged();
        }
    }

    QVariantList cartItems() const {
        QVariantList items;
        for (auto it = quantities.cbegin(); it != quantities.cend(); ++it) {
            const Product *p = productModel->findProduct(it.key());
            if (p) {
                QVariantMap item;
                item["name"] = p->name;
                item["quantity"] = it.value();
                item["lineTotal"] = p->price * it.value();
                items.append(item);
            }
        }
        return items;
    }

    int itemCount() const {
        int total = 0;
        for (int qty : quantities) total += qty;
        return total;
    }

    double totalPrice() const {
        double total = 0;
        for (auto it = quantities.cbegin(); it != quantities.cend(); ++it) {
            const Product *p = productModel->findProduct(it.key());
            if (p) total += p->price * it.value();
        }
        return total;
    }

signals:
    void cartChanged();
};

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    ProductModel productModel;
    CartManager cartManager(&productModel);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("productModel", &productModel);
    engine.rootContext()->setContextProperty("cartManager", &cartManager);

    engine.loadFromModule("cartmanagerstruct", "Main");
    return app.exec();
}

#include "main.moc"
