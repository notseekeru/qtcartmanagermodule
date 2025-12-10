import QtQuick
import QtQuick.Controls

ApplicationWindow {
    width: 800
    height: 600
    visible: true
    title: "Order App"

    property int orderVersion: 0

    Connections {
        target: backend
        function onOrderChanged() { orderVersion++ }
    }

    Row {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 20

        Column {
            width: parent.width * 0.6
            height: parent.height
            spacing: 10

            Repeater {
                model: backend.getCategories()

                Column {
                    width: parent.width
                    spacing: 5

                    Label {
                        text: modelData.charAt(0).toUpperCase() + modelData.slice(1)
                        font.bold: true
                        font.pixelSize: 18
                        color: "white"
                    }

                    ListView {
                        width: parent.width
                        height: contentHeight
                        spacing: 5
                        clip: true
                        interactive: false
                        model: backend.getMenuItemsByCategory(modelData)

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
                                onClicked: backend.addToOrder(modelData.id)
                            }
                        }
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
                    onClicked: backend.clearOrder()
                }
            }

            ListView {
                width: parent.width
                height: parent.height - 60
                spacing: 5
                clip: true
                model: orderVersion >= 0 ? backend.getOrderItems() : []

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
                text: orderVersion >= 0 ? "Total: $" + backend.getOrderTotal().toFixed(2) : ""
                color: "white"
            }
        }
    }
}
