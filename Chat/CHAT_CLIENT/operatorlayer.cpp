#include "operatorlayer.h"
#include "client.h"
#include "chatmodel.h"
#include <QQmlContext>

OperatorLayer::OperatorLayer(QQmlApplicationEngine &engine, boost::asio::io_service &service)
        : QObject()
          , chatModel_(nullptr)
          , client_(nullptr)
{


    tcp::resolver resolver(service);
    tcp::resolver::query query("127.0.0.1", "12345");
    client_ = new chat_client(service, resolver.resolve(query));
    chatModel_ = new ChatModel(this);
    engine.rootContext()->setContextProperty("cl", client_);
    engine.rootContext()->setContextProperty("chmod", chatModel_);

    client_->setReadHandle([this](chat_message &chatMessage) {
        if (chatMessage.getFlagToMessage() == chat_message::Flags::MESSAGE) {
            chatModel_->add(QString::fromUtf8(chatMessage.body(), chatMessage.body_length()));
        } else {
            QByteArray str = QByteArray(chatMessage.body(), chatMessage.body_length());
            auto filename = str.mid(0, str.indexOf('|'));
            std::string str1 = str.mid(str.indexOf('|') + 1).toStdString();

            std::ofstream out(filename.toStdString(), std::ios::binary | std::ofstream::app);
            out.write(str1.c_str(), str1.length());
            out.close();
        }
    });
}

OperatorLayer::~OperatorLayer()
{
    if (client_) {
        delete client_;
        delete chatModel_;
        client_ = nullptr;
        chatModel_ = nullptr;
    }
}
