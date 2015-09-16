#include "rbtlogger.h"

int rbtLogger::index = 0;
int rbtLogger::realMessageCount = 0;
rbtLogger *rbtLogger::logger = NULL;
QList<rbtLogger::structClient*> rbtLogger::clients = QList<rbtLogger::structClient*>();
QList<rbtLogger::structMessage> rbtLogger::messages;

rbtLogger::rbtLogger(QObject *parent, bool newLogger) : QObject(parent)
{
    //typedef void (*QtMessageHandler)(QtMsgType, const QMessageLogContext &, const QString &);
    qInstallMessageHandler((QtMessageHandler)&messageHandler);
    if(newLogger)
    {
        Q_ASSERT(rbtLogger::logger == NULL);
        logger = this;
        lastEmit = 0;
    }
}


void rbtLogger::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        addMessage(messageConstruct(type, QString("[qDebug][DEBUG]%1 %2 %3 %4").arg(msg).arg(context.file).arg(context.line).arg(context.function)));
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    /*case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;*/
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}

void rbtLogger::registerClientId(int id, QtMsgType fromType, QStringList modules, bool fullLog)
{
    structClient* client = new structClient;

    client->id = id;
    client->type = fromType;
    client->modules = modules;

    QList<int> clientIds;
    clientIds.append(id);

    if(fullLog)
    {
        foreach(structMessage message, messages)
        {
            if(message.module.count())
            {
                if(client->modules.contains(message.module))
                    emit(sendMessageToClient(clientIds, message.message.toLatin1()));
            }
            else
            {
                if(client->type >= message.type)
                    emit(sendMessageToClient(clientIds, message.message.toLatin1()));
            }
        }
    }
    clients.append(client);
}

void rbtLogger::unregisterClientId(int id)
{
    for(int i=0; i < clients.count(); i++)
    {
        if(clients.at(i)->id == id)
        {
            delete clients.at(i);
            clients.removeAt(i);
            break;
        }
    }
}

void rbtLogger::emitMessages()
{
    for(int i=lastEmit; i<realMessageCount; i++)
    {
        int realIndex = i;
        if(realMessageCount > maxMessageLog)
        {
            realIndex = i - realMessageCount;
        }
        emitMessages(messages.at(realIndex));
    }
    lastEmit = index;
}

void rbtLogger::emitMessages(structMessage message)
{
    QList<int> clientIds;
    foreach(structClient* client, clients)
    {
        if(message.module.count())
        {
            if(client->modules.contains(message.module)) clientIds.append(client->id);
        }
        else
        {
            if(client->type >= message.type) clientIds.append(client->id);
        }
    }
    if(clientIds.count() != 0)
    {
        emit(sendMessageToClient(clientIds, message.message.toLatin1()));
    }
}

void rbtLogger::addMessage(structMessage message)
{
    messages.append(message);
    realMessageCount++;
    if(messages.count() > maxMessageLog) messages.removeFirst();
    if(logger != NULL) logger->emitMessages();
}

void rbtLogger::logMessage(QString module, QString message)
{
    addMessage(messageConstruct(module, QString("[%1]%1").arg(module).arg(message)));
}

rbtLogger::structMessage rbtLogger::messageConstruct(QtMsgType type, QString message)
{
    structMessage newMessage;
    newMessage.index = index++;
    newMessage.type = type;
    newMessage.message = message;

    return newMessage;

}

rbtLogger::structMessage rbtLogger:: messageConstruct(QString module, QString message)
{
    structMessage newMessage;
    newMessage.index = index++;
    newMessage.module = module;
    newMessage.message = message;

    return newMessage;
}
