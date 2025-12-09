import QtQuick 6.8
import QtQuick.Controls 6.8
import QtQuick.Layouts 1.5

ApplicationWindow {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Simple Cart Demo")

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 16

        Label {
            text: qsTr("Furniture Market")
            font.pointSize: 26
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 32
            spacing: 16

            Label {
                text: qsTr("Catalog")
                Layout.fillWidth: true
                font.pointSize: 18
            }
            Label {
                text: qsTr("Items in cart: %1").arg(cartManager.itemCount)
                font.pointSize: 14
            }
            Label {
                text: qsTr("Total: $%1").arg(cartManager.totalPrice.toFixed(2))
                font.pointSize: 14
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 16

            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: productModel
                clip: true
                spacing: 10

                delegate: Frame {
                    width: parent.width
                    height: 140
                    Layout.fillWidth: true
                    background: Rectangle {
                        anchors.fill: parent
                        color: "#ffffff"
                        radius: 8
                        border.color: "#d0d0d0"
                        border.width: 1
                    }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 16

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 6

                            Label {
                                text: name
                                font.pointSize: 16
                                font.bold: true
                            }
                            Text {
                                text: description
                                color: "#4c4c4c"
                                wrapMode: Text.Wrap
                            }
                            Label {
                                text: qsTr("$%1").arg(price, 0, 'f', 2)
                                font.pointSize: 14
                                color: "#0078d4"
                            }
                        }

                        Button {
                            text: qsTr("Add to cart")
                            onClicked: cartManager.addProduct(productId)
                        }
                    }
                }
            }

            Frame {
                width: 320
                Layout.fillHeight: true
                background: Rectangle {
                    anchors.fill: parent
                    color: "#ffffff"
                    radius: 8
                    border.color: "#d0d0d0"
                    border.width: 1
                }

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 10

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 8
                        Label {
                            text: qsTr("Cart")
                            font.pointSize: 18
                            font.bold: true
                        }
                        Button {
                            text: qsTr("Clear")
                            enabled: cartManager.itemCount > 0
                            onClicked: cartManager.clearCart()
                        }
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        model: cartManager.cartItems
                        clip: true
                        spacing: 6

                        delegate: RowLayout {
                            Layout.fillWidth: true
                            spacing: 8

                            ColumnLayout {
                                Layout.fillWidth: true
                                Label { text: modelData.name; font.bold: true }
                                Label { text: qsTr("Qty: %1").arg(modelData.quantity) }
                            }

                            Label {
                                text: qsTr("$%1").arg(modelData.lineTotal, 0, 'f', 2)
                                Layout.alignment: Qt.AlignRight
                            }
                        }
                    }

                    Label {
                        text: qsTr("Total $%1").arg(cartManager.totalPrice.toFixed(2))
                        font.pointSize: 16
                        font.bold: true
                        color: "#0078d4"
                        Layout.alignment: Qt.AlignRight
                    }
                }
            }
        }
    }
}