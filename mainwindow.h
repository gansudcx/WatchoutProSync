#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QStatusBar>
#include <QLabel>
#include <QRegExpValidator>
#include <QMessageBox>
#include <QDebug>
#include <QDockWidget>
#include <QTimer>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_m_checkBox_stateChanged(int arg1);

    void on_bk_checkBox_stateChanged(int arg1);

    void on_stusButton_clicked();

    void on_onlButton_clicked(bool checked);

    void on_stbButton_clicked(bool checked);

    void on_playButton_clicked();

    void on_pauseButton_clicked();

    void on_f1Button_clicked();

    void on_f2Button_clicked();

    void on_f3Button_clicked();

    void on_f4Button_clicked();

    void on_f5Button_clicked();

    void on_f6Button_clicked();

    void on_f7Button_clicked();

    void on_f8Button_clicked();

    void on_f9Button_clicked();

    void on_f10Button_clicked();


private:
    Ui::MainWindow *ui;
    //watchout proction TCP.UDP port
    qint16 port = 3040;
    //给mailwindow创建成员属性，创建通信套接字
    QTcpSocket *mTcpSocket;
    QTcpSocket *bkTcpSocket;
    //保存链接状态。
    bool mProStatus;
    bool bkProStatus;

    QMenuBar *mBar;
    QMenu *pFile;
    QAction *pOpen;
    QAction *pSave;
    QAction *pAbout;
    QString path;

    QTimer *cdTimer;





};
#endif // MAINWINDOW_H
