[Переслано от Dmitry Vasyliev]
#include <QDebug>
#include <QObject>
#include <string>
#include <thread>
#include <utility>

class Logger : public QObject {
Q_OBJECT
private:
    Logger(QObject *parent = nullptr) : QObject(parent) {}
public:
    static Logger &Instance() {
        if (sInstance == nullptr) {
            sInstance = new Logger();
        }
        return *sInstance;
    }
public slots:
    void log(const std::string &msg) const {
        qDebug() << msg.c_str();
    };
protected:
    static Logger *sInstance;
};

class Downloader : public QObject {
Q_OBJECT
public:
    Downloader(std::string mName, QObject *parent = nullptr) : QObject(parent), mName(std::move(mName)) {
        QObject::connect(
                this, &Downloader::downloadStarted,
                &Logger::Instance(), []() {
                    Logger::Instance().log("Download Started:");
                }
        );

        QObject::connect(
                this, &Downloader::downloadFinished,
                &Logger::Instance(), []() {
                    Logger::Instance().log("Download Finished:");
                }
        );
    }
public:
    void download() {
        emit downloadStarted();

        using namespace std::literals;
        std::this_thread::sleep_for(5s);

        emit downloadFinished();
    }
public slots:
signals:
    void downloadStarted();
    void downloadFinished();
private:
    std::string mName;
};

Logger *Logger::sInstance = nullptr;

int main(int argc, char *argv[]) {
    Logger::Instance().log("App started!");
    Downloader downloader("Book");
    downloader.download();

    return 0;
}

#include "main.moc"