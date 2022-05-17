#include <QObject>
#include <QDebug>
#include <thread>
#include <string>

class Logger : public QObject {
Q_OBJECT

private:
    Logger(QObject *parent = nullptr) : QObject(parent) {}

public:
    static Logger &Instance(){
        if (sInstance == nullptr) {
            sInstance = new Logger();
        }
        return *sInstance;
    }
public slots:
    void log (const std::string &msg) const {
        qDebug() << msg.c_str();
    }

protected:
    static Logger *sInstance;
};

class Bot : public QObject {
Q_OBJECT
public:
    Bot(std::string mName, QObject *parent = nullptr) : QObject(parent), mName(std::move(mName)){
        QObject::connect(
                this, &Bot::sayFirst,
                &Logger::Instance(), []() {
                    Logger::Instance().log("Hey, yo! What's up, bro?");
                }
        );

        QObject::connect(
                this, &Bot::saySecond,
                &Logger::Instance(), []() {
                    Logger::Instance().log("Ohhh! You are so ugly, maaaan!");
                }
        );

        QObject::connect(
                this, &Bot::sayThird,
                &Logger::Instance(), []() {
                    Logger::Instance().log("Get out of there, NIGGA!!!");
                }

        );
    }
public:
    void say(){

        using namespace std::literals;
        std::this_thread::sleep_for(3s);

        emit sayFirst();

        std::this_thread::sleep_for(3s);

        emit saySecond();

        std::this_thread::sleep_for(3s);

        emit sayThird();
    }

public slots:
signals:
    void sayFirst();
    void saySecond();
    void sayThird();

private:
    std::string mName;

};

Logger *Logger::sInstance = nullptr;

int main (int argc, char *argv []) {
    Logger::Instance().log("Bot started!\n");
    Bot bot("Scot");
    bot.say();

    return 0;
}

#include "main.moc"