#include "operatorlayer.h"
#include <cpp/chatmodel.h>
#include <cpp/client.h>

#include <QQmlContext>

OperatorLayer::OperatorLayer(QQmlApplicationEngine &engine, boost::asio::io_service &service)
        : QObject()
          , m_chat_model(nullptr)
          , m_client(nullptr)
{

    boost::asio::ip::tcp::resolver resolver(service);
    boost::asio::ip::tcp::endpoint epoint(boost::asio::ip::address::from_string("127.0.0.1"), 12345);
    auto epointI = resolver.resolve(epoint);

    m_client = new chat_client(service, epointI);
    m_chat_model = new ChatModel(this);

    engine.rootContext()->setContextProperty("cl", m_client);
    engine.rootContext()->setContextProperty("chmod", m_chat_model);

    m_client->setReadHandle([this](chat_message &chatMessage) {
        if (chatMessage.get_flag2message() == chat_message::Flags::MESSAGE) {
            m_chat_model->add(QString::fromUtf8(chatMessage.body(), chatMessage.body_length()));
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
    if (m_client) {
        delete m_client;
        delete m_chat_model;
        m_client = nullptr;
        m_chat_model = nullptr;
    }
}
