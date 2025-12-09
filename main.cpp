#include <QAbstractListModel>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QMap>
#include <QString>
#include <QVariantList>
#include <QVariantMap>

struct Product {
    int id;
    QString name;
    QString description;
    double price;
    QString image;
};

class ProductModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ProductRoles {
        IdRole = Qt::UserRole + 1,
        ProductIdRole,
        NameRole,
        DescriptionRole,
        PriceRole,
        ImageRole
    };

    explicit ProductModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setProducts(const QList<Product> &products);
    const Product *productById(int id) const;

private:
    QList<Product> m_products;
};

struct CartItem {
    Product product;
    int quantity = 0;

    double lineTotal() const { return product.price * quantity; }

    QVariantMap toVariantMap() const
    {
        QVariantMap map;
        map["id"] = product.id;
        map["name"] = product.name;
        map["price"] = product.price;
        map["quantity"] = quantity;
        map["lineTotal"] = lineTotal();
        return map;
    }
};

class CartManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList cartItems READ cartItems NOTIFY cartChanged)
    Q_PROPERTY(int itemCount READ itemCount NOTIFY cartChanged)
    Q_PROPERTY(double totalPrice READ totalPrice NOTIFY cartChanged)

public:
    explicit CartManager(ProductModel *model, QObject *parent = nullptr);

    Q_INVOKABLE void addProduct(int productId);
    Q_INVOKABLE void removeProduct(int productId);
    Q_INVOKABLE void clearCart();

    QVariantList cartItems() const;
    int itemCount() const;
    double totalPrice() const;

signals:
    void cartChanged();

private:
    ProductModel *m_productModel;
    QMap<int, int> m_quantities;
};

static QList<Product> buildProductCatalog()
{
    return {
        { 1, QStringLiteral("Nordic Chair"), QStringLiteral("Clean lines and natural wood."), 149.0, QStringLiteral("") },
        { 2, QStringLiteral("Maple Desk"), QStringLiteral("Solid maple workspace with room for dual monitors."), 299.0, QStringLiteral("") },
        { 3, QStringLiteral("Aurora Lamp"), QStringLiteral("Soft, diffused light for evening work sessions."), 89.0, QStringLiteral("") }
    };
}

ProductModel::ProductModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ProductModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_products.count();
}

QVariant ProductModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_products.count())
        return {};

    const Product &product = m_products.at(index.row());

    switch (role) {
    case IdRole:
    case ProductIdRole:
        return product.id;
    case NameRole:
        return product.name;
    case DescriptionRole:
        return product.description;
    case PriceRole:
        return product.price;
    case ImageRole:
        return product.image;
    default:
        return {};
    }
}

QHash<int, QByteArray> ProductModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[ProductIdRole] = "productId";
    roles[NameRole] = "name";
    roles[DescriptionRole] = "description";
    roles[PriceRole] = "price";
    roles[ImageRole] = "image";
    return roles;
}

void ProductModel::setProducts(const QList<Product> &products)
{
    beginResetModel();
    m_products = products;
    endResetModel();
}

const Product *ProductModel::productById(int id) const
{
    for (const Product &product : m_products) {
        if (product.id == id)
            return &product;
    }
    return nullptr;
}

CartManager::CartManager(ProductModel *model, QObject *parent)
    : QObject(parent)
    , m_productModel(model)
{
}

void CartManager::addProduct(int productId)
{
    if (!m_productModel->productById(productId))
        return;

    ++m_quantities[productId];
    emit cartChanged();
}

void CartManager::removeProduct(int productId)
{
    auto it = m_quantities.find(productId);
    if (it == m_quantities.end())
        return;

    --(*it);
    if (*it <= 0)
        m_quantities.remove(productId);

    emit cartChanged();
}

void CartManager::clearCart()
{
    if (m_quantities.isEmpty())
        return;

    m_quantities.clear();
    emit cartChanged();
}

QVariantList CartManager::cartItems() const
{
    QVariantList items;

    for (auto it = m_quantities.cbegin(); it != m_quantities.cend(); ++it) {
        const Product *product = m_productModel->productById(it.key());
        if (!product)
            continue;

        CartItem entry{*product, it.value()};
        items.append(entry.toVariantMap());
    }

    return items;
}

int CartManager::itemCount() const
{
    int total = 0;
    for (int quantity : m_quantities)
        total += quantity;
    return total;
}

double CartManager::totalPrice() const
{
    double total = 0.0;
    for (auto it = m_quantities.cbegin(); it != m_quantities.cend(); ++it) {
        const Product *product = m_productModel->productById(it.key());
        if (!product)
            continue;
        CartItem entry{*product, it.value()};
        total += entry.lineTotal();
    }
    return total;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    ProductModel productModel;
    productModel.setProducts(buildProductCatalog());

    CartManager cartManager(&productModel);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("productModel", &productModel);
    engine.rootContext()->setContextProperty("cartManager", &cartManager);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("cartmanager", "Main");

    return app.exec();
}#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "cartmanager.h"
#include "productmodel.h"

static QList<Product> buildProductCatalog()
{
    return {
        { 1, QStringLiteral("Nordic Chair"), QStringLiteral("Clean lines and natural wood."), 149.0, QStringLiteral("" ) },
        { 2, QStringLiteral("Maple Desk"), QStringLiteral("Solid maple workspace with room for dual monitors."), 299.0, QStringLiteral("") },
        { 3, QStringLiteral("Aurora Lamp"), QStringLiteral("Soft, diffused light for evening work sessions."), 89.0, QStringLiteral("") }
    };
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    ProductModel productModel;
    productModel.setProducts(buildProductCatalog());

    CartManager cartManager(&productModel);

    engine.rootContext()->setContextProperty("productModel", &productModel);
    engine.rootContext()->setContextProperty("cartManager", &cartManager);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("cartmanager", "Main");

    return app.exec();
}
