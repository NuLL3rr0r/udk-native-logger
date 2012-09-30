#include <windows.h>
#include <sstream>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <QMessageBox>
#include <QScrollBar>
#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "server.hpp"
#include "log.hpp"

const size_t MainWindow::SERVER_PORT = 1317;
const std::string MainWindow::LOG_FILE = "ULog";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_server(new ULogger::Server(SERVER_PORT))
{
    ui->setupUi(this);

    qRegisterMetaType<std::string>();

    QObject::connect(this, SIGNAL(SigOnLogReceived(const std::string &)),
                     SLOT(SlotOnLogReceived(const std::string &)),
                     Qt::QueuedConnection);

    try {
        Log::Initialize(LOG_FILE);

        m_server->OnMessageReceived = boost::bind(&MainWindow::OnLogReceived, this, _1, _2);
        m_server->Start();
    }
    catch(std::exception &ex) {
        QMessageBox::critical(this, "Error", ex.what());
    }
    catch(...) {
        QMessageBox::critical(this, "Error", "Server does not start !!");
    }
}

MainWindow::~MainWindow()
{
    if (m_server->IsRunning()) {
        m_server->Stop();
    }
    m_server.reset();

    delete ui;
}

void MainWindow::SlotOnLogReceived(const std::string &log)
{
    ui->logTextEdit->append(QString::fromUtf8(log.c_str()));
    if (ui->scrollOnOutputCheckBox->isChecked()) {
        QScrollBar *sb = ui->logTextEdit->verticalScrollBar();
        sb->setValue(sb->maximum());
    }
}

void MainWindow::OnLogReceived(const std::string &clientId, const std::string &log)
{
    std::string logText;

    try {
        std::stringstream json;
        json << log;

        boost::property_tree::ptree logTree;
        boost::property_tree::read_json(json, logTree);

        std::stringstream formatted;
        formatted << "["
                  << clientId
                  << ", "
                  << logTree.get<std::string>("log.type")
                  << ", "
                  << logTree.get<std::string>("log.timestamp")
                  << ", "
                  << logTree.get<std::size_t>("log.line")
                  << ", "
                  << logTree.get<std::string>("log.function")
                  << ", "
                  << logTree.get<std::string>("log.file")
                  << "] ";

        bool isFirstOne = true;
        BOOST_FOREACH(const boost::property_tree::ptree::value_type &child,
                      logTree.get_child("log.messages")) {
            if (!isFirstOne) {
                formatted << ", ";
            } else {
                isFirstOne = false;
            }
            formatted << child.second.get<std::string>("");
        }

        logText = formatted.str();
    }

    catch (std::exception const &ex) {
        logText = ((boost::format("Failed to parse received data: %1%")
                    % ex.what()).str());
    }

    catch (...) {
        logText = "Failed to parse received data !!";
    }

    emit SigOnLogReceived(logText);
    LOG(logText.c_str());
}


void MainWindow::on_stayOnTopCheckBox_toggled(bool checked)
{
    if (checked) {
        SetWindowPos(this->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    } else {
        SetWindowPos(this->winId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
}


void MainWindow::on_clearPushButton_pressed()
{
    ui->logTextEdit->clear();
}
