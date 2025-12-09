import QtQuick
import QtQuick.Controls

ApplicationWindow {
    width: 800
    height: 600
    visible: true
    title: "Order App"

    property int orderVersion: 0

    Connections {
        target: orderManager
        function onOrderChanged() { orderVersion++ }
    }

    Row {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 20

        Column {
            width: parent.width * 0.6
            height: parent.height

            ListView {
                width: parent.width
                height: parent.height
                spacing: 5
                clip: true
                model: orderManager.getMenuItems()

                delegate: Row {
                    spacing: 10

                    Image {
                        width: 60
                        height: 60
                        fillMode: Image.PreserveAspectFit
                        source: modelData.image
                    }

                    Label {
                        text: modelData.name
                        color: "white"
                    }

                    Label {
                        text: "$" + modelData.price.toFixed(2)
                        color: "white"
                    }

                    Button {
                        text: "Add"
                        onClicked: orderManager.addToOrder(modelData.id)
                    }
                }
            }
        }

        Column {
            width: parent.width * 0.35
            height: parent.height

            Row {
                Label {
                    text: "Your Order"
                    color: "white"
                }

                Button {
                    text: "Clear"
                    palette.buttonText: "black"
                    onClicked: orderManager.clearOrder()
                }
            }

            ListView {
                width: parent.width
                height: parent.height - 60
                spacing: 5
                clip: true
                model: orderVersion >= 0 ? orderManager.getOrderItems() : []

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
                text: "Total: $" + orderManager.getOrderTotal().toFixed(2)
                color: "white"
            }
        }
    }
}
