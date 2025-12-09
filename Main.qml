import QtQuick
import QtQuick.Controls

ApplicationWindow {
    width: 800
    height: 600
    visible: true
    title: "Cart Manager"

    palette.windowText: "white"
    palette.text: "white"

    Row {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 20

        Column {
            width: parent.width * 0.6
            height: parent.height
            spacing: 5

            Label { 
                text: "Products"
                color: "white"
            }
            

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
                        color: "white"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Label { 
                        text: "$" + price.toFixed(2)
                        color: "white"
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
                Label { 
                    text: "Cart"
                    color: "white"
                }
                Button {
                    text: "Clear"
                    palette.buttonText: "black"
                    onClicked: cartManager.clearCart()
                }
            }

            ListView {
                width: parent.width
                height: parent.height - 60
                model: cartManager.cartItems
                spacing: 5

                delegate: Row {
                    Label { 
                        text: modelData.name
                        color: "white"
                    }
                    Label { 
                        text: "Qty: " + modelData.quantity
                        color: "white"
                    }
                    Label { 
                        text: "$" + modelData.lineTotal.toFixed(2)
                        color: "white"
                    }
                }
            }

            Label { 
                text: "Total: $" + cartManager.totalPrice.toFixed(2)
                color: "white"
            }
        }
    }
}
