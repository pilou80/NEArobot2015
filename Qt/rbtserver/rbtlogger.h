#ifndef RBTLOGGER_H
#define RBTLOGGER_H

#include <QObject>
#include <QDebug>

#define maxMessageLog 2000
#define log(module,message) rbtLogger::logMessage(module,message)

class rbtLogger;

class rbtLogger : public QObject
{
    Q_OBJECT
public:
    explicit rbtLogger(QObject *parent = 0, bool newLogger = false);
    static void logMessage(QString module, QString message);

signals:
    void sendMessageToClient(QList<int> clientIds, QByteArray data);

public slots:
    void registerClientId(int id, QtMsgType fromType, QStringList modules, bool fullLog);
    void unregisterClientId(int id);

protected:
    static rbtLogger *logger;

private:
    typedef struct {
        int id;
        QtMsgType type;
        QStringList modules;
        //int lastIndex;
    } structClient;

    typedef struct {
        int index;
        QString message;
        QtMsgType type;
        QString module;
    }structMessage;

    void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static void addMessage(structMessage message);
    static structMessage messageConstruct(QtMsgType type, QString message);
    static structMessage messageConstruct(QString module, QString message);

    void emitMessages();
    void emitMessages(structMessage message);


    static QList<structClient*> clients;
    static int index;
    int lastEmit;
    static QList<structMessage> messages;
    static int realMessageCount;

};

#endif // RBTLOGGER_H
