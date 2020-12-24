#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //初始化
    mTcpSocket = NULL;
    bkTcpSocket = NULL;
    mTcpSocket = new QTcpSocket(this);   //在堆区分配空间，指定父对象（为了析构）
    bkTcpSocket = new QTcpSocket(this);
    mProStatus = false;
    bkProStatus = false;

    mBar = menuBar();
    pFile = mBar->addMenu("File");
    pSave = pFile->addAction("Save");
    pOpen = pFile->addAction("Open");
    pAbout = pFile->addAction("About");

    connect(pSave, &QAction::triggered,
            [=]()
            {
                QString path = QFileDialog::getSaveFileName(this, "save", "../", "TXT(*.txt)" );
                if(path.isEmpty() == false)
                {
                    QFile file; //创建文件对象
                    file.setFileName(path); //关联文件名字
                    if(file.open(QIODevice::WriteOnly) == true)
                    {
                        QString mIp = ui->lineEdit_mip->text();
                        file.write(mIp.toUtf8());
                        file.write("\n");
                        QString bkIp = ui->lineEdit_bkip->text();
                        file.write(bkIp.toUtf8());
                        file.write("\n");
                        QString timeLine = ui->lineEdit_tl->text();
                        file.write(timeLine.toUtf8());
                        file.write("\n");
                        QString f1Str = ui->lineEdit_f1->text();
                        file.write(f1Str.toUtf8());
                        file.write("\n");
                        QString f2Str = ui->lineEdit_f2->text();
                        file.write(f2Str.toUtf8());
                        file.write("\n");
                        QString f3Str = ui->lineEdit_f3->text();
                        file.write(f3Str.toUtf8());
                        file.write("\n");
                        QString f4Str = ui->lineEdit_f4->text();
                        file.write(f4Str.toUtf8());
                        file.write("\n");
                        QString f5Str = ui->lineEdit_f5->text();
                        file.write(f5Str.toUtf8());
                        file.write("\n");
                        QString f6Str = ui->lineEdit_f6->text();
                        file.write(f6Str.toUtf8());
                        file.write("\n");
                        QString f7Str = ui->lineEdit_f7->text();
                        file.write(f7Str.toUtf8());
                        file.write("\n");
                        QString f8Str = ui->lineEdit_f8->text();
                        file.write(f8Str.toUtf8());
                        file.write("\n");
                        QString f9Str = ui->lineEdit_f9->text();
                        file.write(f9Str.toUtf8());
                        file.write("\n");
                        QString f10Str = ui->lineEdit_f10->text();
                        file.write(f10Str.toUtf8());
                        file.write("\n");
                    }
                    file.close();
                }
            }
            );
    connect(pOpen, &QAction::triggered,
            [=]()
            {
                QString path = QFileDialog::getOpenFileName(this, "open", "../", "TXT(*.txt)" );
                if(path.isEmpty() == false)
                {
                    QFile file(path);
                    if(file.open(QIODevice::ReadOnly) == true)
                    {
                        QByteArray array;
                        if(file.atEnd() == false)
                        {
                            array = file.readLine();
                            ui->lineEdit_mip->setText(array.trimmed());
                            array = file.readLine();
                            ui->lineEdit_bkip->setText(array.trimmed());
                            array = file.readLine();
                            ui->lineEdit_tl->setText(array.trimmed());
                            array = file.readLine();
                            ui->lineEdit_f1->setText(array.trimmed());
                            array = file.readLine();
                            ui->lineEdit_f2->setText(array.trimmed());
                            array = file.readLine();
                            ui->lineEdit_f3->setText(array.trimmed());
                            array = file.readLine();
                            ui->lineEdit_f4->setText(array.trimmed());
                            array = file.readLine();
                            ui->lineEdit_f5->setText(array.trimmed());
                            array = file.readLine();
                            ui->lineEdit_f6->setText(array.trimmed());
                            array = file.readLine();
                            ui->lineEdit_f7->setText(array.trimmed());
                            array = file.readLine();
                            ui->lineEdit_f8->setText(array.trimmed());
                            array = file.readLine();
                            ui->lineEdit_f9->setText(array.trimmed());
                            array = file.readLine();
                            ui->lineEdit_f10->setText(array.trimmed());
                        }
                    }
                    file.close();
                }
            }
            );

    connect(pAbout, &QAction::triggered,
            [=]()
            {
                QMessageBox::information(this,"About","V1.1 \nAuthor: Chenxi Du (Darren) \nEmail: gansudcx@163.com or cxdu@ctasiapacific.com \n"
                                                      "Shortcut: Standby:ctrl+b; Online:ctrl+l; Space:Play;  P:Pause; S:status; \nF1-10:Cue F1-10");
            }
            );

//    cdDock = new QDockWidget(this);
//    addDockWidget(Qt::TopDockWidgetArea, cdDock);
//    cdTimer = new QTimer(this);

    // 当Socket建立,显示connected，
    connect(mTcpSocket, &QTcpSocket::connected,
            [=]()
            {
                ui->m_checkBox->setText("Connected");
                ui->m_checkBox->setCheckState(Qt::Checked);
                mProStatus = true;
            }
            );
    connect(bkTcpSocket, &QTcpSocket::connected,
            [=]()
            {
                ui->bk_checkBox->setText("Connected");
                ui->bk_checkBox->setCheckState(Qt::Checked);
                bkProStatus = true;
            }
            );

    //when watchout return message,  display it in qmessage box in status bar.
    connect(mTcpSocket, &QTcpSocket::readyRead,
            [=]()
            {
                QByteArray array = mTcpSocket->readAll();
                ui->stusMlabel->setText(array);

            }
            );
    connect(bkTcpSocket, &QTcpSocket::readyRead,
            [=]()
            {
                QByteArray array = bkTcpSocket->readAll();
                ui->stusBklabel->setText(array);
            }
            );
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_m_checkBox_stateChanged(int arg1)
{

    //获取服务器ip和端口
    QString ip = ui->lineEdit_mip->text();

    //判断IP地址是否是有效的IP。
    QHostAddress addr;
    if(addr.setAddress(ui->lineEdit_mip->text()) == true)   //IP地址有效
    {
        if(ui->m_checkBox->isChecked() == true)         //Chckbox被选中
        {
            //主动和服务器建立连接
            mTcpSocket->connectToHost(QHostAddress(ip), port);
        }
        else        //checkbox 没有选中
        {
            mTcpSocket->disconnectFromHost();
            ui->m_checkBox->setCheckState(Qt::Unchecked);
            ui->m_checkBox->setText("Not Connect");
            mProStatus = false;
        }
    }
    else    //如果IP地址无效
    {
        ui->m_checkBox->setCheckState(Qt::Unchecked);
        QMessageBox::warning(this, "Invalid ip address","Please input an valid ip address", QMessageBox::Ok);
    }
}

void MainWindow::on_bk_checkBox_stateChanged(int arg1)
{
    //获取服务器ip和端口
    QString ip = ui->lineEdit_bkip->text();

    //判断IP地址是否是有效的IP。
    QHostAddress addr;
    if(addr.setAddress(ui->lineEdit_bkip->text()) == true)   //IP地址有效
    {
        if(ui->bk_checkBox->isChecked() == true)         //Chckbox被选中
        {
            //主动和服务器建立连接
            bkTcpSocket->connectToHost(QHostAddress(ip), port);
        }
        else        //checkbox 没有选中
        {
            bkTcpSocket->disconnectFromHost();
            ui->bk_checkBox->setCheckState(Qt::Unchecked);
            ui->bk_checkBox->setText("Not Connect");
            bkProStatus = false;
        }
    }
    else    //如果IP地址无效
    {
        ui->bk_checkBox->setCheckState(Qt::Unchecked);
        QMessageBox::warning(this, "Invalid ip address","Please input an valid ip address", QMessageBox::Ok);
    }
}

void MainWindow::on_stusButton_clicked()
{
        QString getStatus = "getStatus \n";
        if(mProStatus == true)
        {
           mTcpSocket->write(getStatus.toUtf8().data());
        }
        if(bkProStatus == true)
        {
            bkTcpSocket->write(getStatus.toUtf8().data());
        }
        qDebug("getStatus");
}

void MainWindow::on_stbButton_clicked(bool checked)
{
    QString standByOn = "standBy true 1000 \n";
    QString standByOff = "standBy false 1000 \n";

    if(checked == true)
    {
        if(mProStatus == true)
        {
           mTcpSocket->write(standByOn.toUtf8().data());
        }
        if(bkProStatus == true)
        {
            bkTcpSocket->write(standByOn.toUtf8().data());
        }
        qDebug("standBy true 1000");
    }
    else
    {
        if(mProStatus == true)
        {
           mTcpSocket->write(standByOff.toUtf8().data());
        }
        if(bkProStatus == true)
        {
            bkTcpSocket->write(standByOff.toUtf8().data());
        }
        qDebug("standBy false 1000");
    }
}

void MainWindow::on_onlButton_clicked(bool checked)
{
    QString onLine = "online true \n";
    QString offLine = "online false \n";

    if(checked == true)
    {
        if(mProStatus == true)
        {
           mTcpSocket->write(onLine.toUtf8().data());
        }
        if(bkProStatus == true)
        {
            bkTcpSocket->write(onLine.toUtf8().data());
        }
        qDebug("online true ");
    }
    else
    {
        if(mProStatus == true)
        {
           mTcpSocket->write(offLine.toUtf8().data());
        }
        if(bkProStatus == true)
        {
            bkTcpSocket->write(offLine.toUtf8().data());
        }
        qDebug("online false ");
    }
}


void MainWindow::on_playButton_clicked()
{
    QString cmdPlay = "run " + ui->lineEdit_tl->text() + "\n";
    if(mProStatus == true)
    {
       mTcpSocket->write(cmdPlay.toUtf8().data());
    }
    if(bkProStatus == true)
    {
        bkTcpSocket->write(cmdPlay.toUtf8().data());
    }
}

void MainWindow::on_pauseButton_clicked()
{
    QString cmdHalt = "halt " + ui->lineEdit_tl->text() + "\n";
    if(mProStatus == true)
    {
       mTcpSocket->write(cmdHalt.toUtf8().data());
    }
    if(bkProStatus == true)
    {
        bkTcpSocket->write(cmdHalt.toUtf8().data());
    }
}

void MainWindow::on_f1Button_clicked()
{
    QString cmdF1 = "gotoControlCue \"F1\" false " + ui->lineEdit_tl->text() + "\n";
    QString cmdHalt = "halt " + ui->lineEdit_tl->text() + "\n";
    if(mProStatus == true)
    {
       mTcpSocket->write(cmdF1.toUtf8().data());
       mTcpSocket->write(cmdHalt.toUtf8().data());
    }
    if(bkProStatus == true)
    {
        bkTcpSocket->write(cmdF1.toUtf8().data());
        bkTcpSocket->write(cmdHalt.toUtf8().data());

    }
}

void MainWindow::on_f2Button_clicked()
{
    QString cmdF2 = "gotoControlCue \"F2\" false " + ui->lineEdit_tl->text() + "\n";
    QString cmdHalt = "halt " + ui->lineEdit_tl->text() + "\n";
    if(mProStatus == true)
    {
        mTcpSocket->write(cmdF2.toUtf8().data());
        mTcpSocket->write(cmdHalt.toUtf8().data());

    }
    if(bkProStatus == true)
    {
        bkTcpSocket->write(cmdF2.toUtf8().data());
        bkTcpSocket->write(cmdHalt.toUtf8().data());
    }
}

void MainWindow::on_f3Button_clicked()
{
    QString cmdHalt = "halt " + ui->lineEdit_tl->text() + "\n";
    QString cmdF3 = "gotoControlCue \"F3\" false " + ui->lineEdit_tl->text() + "\n";
    if(mProStatus == true)
    {
        mTcpSocket->write(cmdF3.toUtf8().data());
        mTcpSocket->write(cmdHalt.toUtf8().data());

    }
    if(bkProStatus == true)
    {
        bkTcpSocket->write(cmdF3.toUtf8().data());
        bkTcpSocket->write(cmdHalt.toUtf8().data());

    }
}

void MainWindow::on_f4Button_clicked()
{
    QString cmdHalt = "halt " + ui->lineEdit_tl->text() + "\n";
    QString cmdF4 = "gotoControlCue \"F4\" false " + ui->lineEdit_tl->text() + "\n";
    if(mProStatus == true)
    {
        mTcpSocket->write(cmdF4.toUtf8().data());
        mTcpSocket->write(cmdHalt.toUtf8().data());

    }
    if(bkProStatus == true)
    {
        bkTcpSocket->write(cmdF4.toUtf8().data());
        bkTcpSocket->write(cmdHalt.toUtf8().data());

    }
}

void MainWindow::on_f5Button_clicked()
{
    QString cmdHalt = "halt " + ui->lineEdit_tl->text() + "\n";
    QString cmdF5 = "gotoControlCue \"F5\" false " + ui->lineEdit_tl->text() + "\n";
    if(mProStatus == true)
    {
        mTcpSocket->write(cmdF5.toUtf8().data());
        mTcpSocket->write(cmdHalt.toUtf8().data());

    }
    if(bkProStatus == true)
    {
        bkTcpSocket->write(cmdF5.toUtf8().data());
        bkTcpSocket->write(cmdHalt.toUtf8().data());

    }
}

void MainWindow::on_f6Button_clicked()
{
    QString cmdHalt = "halt " + ui->lineEdit_tl->text() + "\n";
    QString cmdF6 = "gotoControlCue \"F6\" false " + ui->lineEdit_tl->text() + "\n";
    if(mProStatus == true)
    {
        mTcpSocket->write(cmdF6.toUtf8().data());
        mTcpSocket->write(cmdHalt.toUtf8().data());

    }
    if(bkProStatus == true)
    {
        bkTcpSocket->write(cmdF6.toUtf8().data());
        bkTcpSocket->write(cmdHalt.toUtf8().data());

    }
}

void MainWindow::on_f7Button_clicked()
{
    QString cmdHalt = "halt " + ui->lineEdit_tl->text() + "\n";
    QString cmdF7 = "gotoControlCue \"F7\" false " + ui->lineEdit_tl->text() + "\n";
    if(mProStatus == true)
    {
        mTcpSocket->write(cmdF7.toUtf8().data());
        mTcpSocket->write(cmdHalt.toUtf8().data());   
    }
    if(bkProStatus == true)
    {
        bkTcpSocket->write(cmdF7.toUtf8().data());
        bkTcpSocket->write(cmdHalt.toUtf8().data()); 
    }
}

void MainWindow::on_f8Button_clicked()
{
    QString cmdHalt = "halt " + ui->lineEdit_tl->text() + "\n";
    QString cmdF8 = "gotoControlCue \"F8\" false " + ui->lineEdit_tl->text() + "\n";
    if(mProStatus == true)
    {
        mTcpSocket->write(cmdF8.toUtf8().data());
        mTcpSocket->write(cmdHalt.toUtf8().data());    
    }
    if(bkProStatus == true)
    {
        bkTcpSocket->write(cmdF8.toUtf8().data());
        bkTcpSocket->write(cmdHalt.toUtf8().data());
    }
}

void MainWindow::on_f9Button_clicked()
{
    QString cmdHalt = "halt " + ui->lineEdit_tl->text() + "\n";
    QString cmdF9 = "gotoControlCue \"F9\" false " + ui->lineEdit_tl->text() + "\n";
    if(mProStatus == true)
    {
        mTcpSocket->write(cmdF9.toUtf8().data());
        mTcpSocket->write(cmdHalt.toUtf8().data());  
    }
    if(bkProStatus == true)
    {
        bkTcpSocket->write(cmdF9.toUtf8().data());
        bkTcpSocket->write(cmdHalt.toUtf8().data());
    }
}

void MainWindow::on_f10Button_clicked()
{
    QString cmdHalt = "halt " + ui->lineEdit_tl->text() + "\n";
    QString cmdF10 = "gotoControlCue \"F10\" false " + ui->lineEdit_tl->text() + "\n";
    if(mProStatus == true)
    {
        mTcpSocket->write(cmdF10.toUtf8().data());
        mTcpSocket->write(cmdHalt.toUtf8().data()); 
    }
    if(bkProStatus == true)
    {
        bkTcpSocket->write(cmdF10.toUtf8().data());
        bkTcpSocket->write(cmdHalt.toUtf8().data());
    }
}







