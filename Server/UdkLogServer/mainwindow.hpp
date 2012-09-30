#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP


#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <QMainWindow>
#include <QMetaType>
#include "server.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    typedef boost::shared_ptr<ULogger::Server> ServerPtr;

private:
    static const size_t SERVER_PORT;
    static const std::string LOG_FILE;

private:
    ServerPtr m_server;

    Ui::MainWindow *ui;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void SigOnLogReceived(const std::string &log);

private slots:
    void SlotOnLogReceived(const std::string &log);

    void on_stayOnTopCheckBox_toggled(bool checked);

    void on_clearPushButton_pressed();

private:
    void OnLogReceived(const std::string &clientId, const std::string &log);
};

Q_DECLARE_METATYPE(std::string)

#endif // MAINWINDOW_HPP
