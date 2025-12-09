import QtQuick
import QtQuick.Controls

ApplicationWindow {
    width: 800
    height: 600
    visible: true
    title: "Cart Manager"

    Row {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 20

        Column {
            width: parent.width * 0.6
            height: parent.height
            spacing: 5

            Label { text: "Products" }

            ListView {
                width: parent.width
                height: parent.height - 30
                model: productModel
                spacing: 5

                delegate: Row {
                    spacing: 10
                    Image {
                        width: 60
                        height: 60
                        source: image
                        fillMode: Image.PreserveAspectFit
                        visible: image !== ""
                    }
                    Label { 
                        text: name
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Label { 
                        text: "$" + price.toFixed(2)
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Button {
                        text: "Add"
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: cartManager.addProduct(productId)
                    }
                }
            }
        }

        Column {
            width: parent.width * 0.35
            height: parent.height
            spacing: 5

            Row {
                Label { text: "Cart" }
                Button {
                    text: "Clear"
                    onClicked: cartManager.clearCart()
                }
            }

            ListView {
                width: parent.width
                height: parent.height - 60
                model: cartManager.cartItems
                spacing: 5

                delegate: Row {
                    Label { text: modelData.name }
                    Label { text: "Qty: " + modelData.quantity }
                    Label { text: "$" + modelData.lineTotal.toFixed(2) }
                }
            }

            Label { text: "Total: $" + cartManager.totalPrice.toFixed(2) }
        }
    }
}
