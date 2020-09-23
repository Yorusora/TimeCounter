#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QTimer"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isStart = false;
    timer = new QTimer;
    src_ms = 0;
    src_s = 0;
    src_m = 0;
    src_h = 0;
    reverseTime = 0;
    result = "00:00:00:000";
    recordedData = "\0";
    ui->timeDisplay_lcdNumber->display("00:00:00:000");
    connect(timer,SIGNAL(timeout()),this,SLOT(updateTime()));
    ui->start_pushButton->setEnabled(true);
    ui->stop_pushButton->setEnabled(false);
    ui->reset_pushButton->setEnabled(false);
    ui->pauseOrContinue_pushButton->setEnabled(false);
    ui->isCountDown_checkBox->setCheckState(Qt::Unchecked);
    ui->hourInput_spinBox->setEnabled(false);
    ui->minuteInput_spinBox->setEnabled(false);
    ui->secondInput_spinBox->setEnabled(false);
    ui->record_pushButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTime()//更新时间显示
{
    src_ms++;
    QString hour,minute,second,msecond;
    if(ui->isCountDown_checkBox->isChecked() == false)//若倒计时复选框不被勾选则执行
    {
        if(src_ms >= 1000)
        {
            src_s++;
            src_ms = 0;
        }
        if(src_s >= 60)
        {
            src_m++;
            src_s = 0;
        }
        if(src_m >= 60)
        {
            src_h++;
            src_m = 0;
        }
        hour = QString::number(src_h);
        minute = QString::number(src_m);
        second = QString::number(src_s);
        msecond = QString::number(src_ms);
    }
    if(ui->isCountDown_checkBox->isChecked() == true)//若复选框被勾选则执行
    {
        while(reverseTime > 0)
        {
            rhour = ui->hourInput_spinBox->value();
            rminute = ui->minuteInput_spinBox->value();
            rsecond = ui->secondInput_spinBox->value() - 1;
            reverseTime = 0;
        }
        if(rmsecond < 0)
        {
            rsecond--;
            src_ms = 0;
        }
        if(rsecond < 0)
        {
            rminute--;
            rsecond = 59;
        }
        if(rminute < 0)
        {
            rhour --;
            rminute = 59;
        }
        rmsecond = 999 - src_ms;
        hour = QString::number(rhour);
        minute = QString::number(rminute);
        second = QString::number(rsecond);
        msecond = QString::number(rmsecond);
        if(rhour == 0 && rminute == 0 && rsecond == 0 && rmsecond == 0)
            timer->stop();
    }
    if(hour.length() == 1)
        hour = "0" + hour;
    if(minute.length() == 1)
        minute = "0" + minute;
    if(second.length() == 1)
        second = "0" + second;
    if(msecond.length() == 1)
        msecond = "00" + msecond;
    else if(msecond.length() == 2)
        msecond = "0" + msecond;
    result = hour + ":" + minute + ":" + second + ":" + msecond;
    ui->timeDisplay_lcdNumber->display(result);
    if(ui->isCountDown_checkBox->isChecked() == true)
        if(result == "00:00:00:000")
        {
            ui->pauseOrContinue_pushButton->setEnabled(false);
            ui->reset_pushButton->setEnabled(true);
            ui->stop_pushButton->setEnabled(false);
        }
}

void MainWindow::on_start_pushButton_clicked()//开始按钮点击事件
{
    isStart = true;
    timer->start(1);
    ui->start_pushButton->setEnabled(false);
    ui->stop_pushButton->setEnabled(true);
    ui->reset_pushButton->setEnabled(false);
    ui->pauseOrContinue_pushButton->setEnabled(true);
    ui->isCountDown_checkBox->setEnabled(false);
    ui->hourInput_spinBox->setEnabled(false);
    ui->minuteInput_spinBox->setEnabled(false);
    ui->secondInput_spinBox->setEnabled(false);
    ui->record_pushButton->setEnabled(true);
}

void MainWindow::on_stop_pushButton_clicked()//停止按钮点击事件
{
    isStart = false;
    timer->stop();
    ui->start_pushButton->setEnabled(false);
    ui->stop_pushButton->setEnabled(false);
    ui->reset_pushButton->setEnabled(true);
    ui->pauseOrContinue_pushButton->setEnabled(false);
    ui->isCountDown_checkBox->setEnabled(false);
    ui->record_pushButton->setEnabled(false);
}

void MainWindow::on_reset_pushButton_clicked()//重置按钮点击事件
{
    isStart = false;
    src_ms = 0;src_s = 0;src_m = 0;src_h = 0;
    reverseTime = 1;
    recordedData = "\0";
    ui->timeDisplay_lcdNumber->display("00:00:00:000");
    if(ui->isCountDown_checkBox->isChecked() == true)
    {
        ui->hourInput_spinBox->setEnabled(true);
        ui->minuteInput_spinBox->setEnabled(true);
        ui->secondInput_spinBox->setEnabled(true);
        hour = QString::number(ui->hourInput_spinBox->value());
        minute = QString::number(ui->minuteInput_spinBox->value());
        second = QString::number(ui->secondInput_spinBox->value());
        if(hour.length() == 1)
            hour = "0" + hour;
        if(minute.length() == 1)
            minute = "0" + minute;
        if(second.length() == 1)
            second = "0" + second;
        result = hour + ":" + minute + ":" + second + ":000";
        ui->timeDisplay_lcdNumber->display(result);
    }
    if(ui->isCountDown_checkBox->isChecked() == false)
    {
        ui->hourInput_spinBox->setEnabled(false);
        ui->minuteInput_spinBox->setEnabled(false);
        ui->secondInput_spinBox->setEnabled(false);
    }
    ui->pauseOrContinue_pushButton->setText("暂停");
    ui->start_pushButton->setEnabled(true);
    ui->stop_pushButton->setEnabled(false);
    ui->reset_pushButton->setEnabled(false);
    ui->pauseOrContinue_pushButton->setEnabled(false);
    ui->isCountDown_checkBox->setEnabled(true);
    ui->record_pushButton->setEnabled(false);
    ui->dataRecorder_textBrowser->setMarkdown(recordedData);
}

void MainWindow::on_pauseOrContinue_pushButton_clicked()//暂停或继续按钮点击事件
{
    if(isStart == true)
    {
        timer->stop();
        ui->pauseOrContinue_pushButton->setText("继续");
        ui->start_pushButton->setEnabled(false);
        ui->stop_pushButton->setEnabled(false);
        ui->reset_pushButton->setEnabled(true);
        ui->pauseOrContinue_pushButton->setEnabled(true);
        ui->isCountDown_checkBox->setEnabled(false);
    }
    if(isStart == false)
    {
        timer->start(1);
        ui->pauseOrContinue_pushButton->setText("暂停");
        ui->start_pushButton->setEnabled(false);
        ui->stop_pushButton->setEnabled(true);
        ui->reset_pushButton->setEnabled(false);
        ui->pauseOrContinue_pushButton->setEnabled(true);
        ui->isCountDown_checkBox->setEnabled(false);
    }
    isStart = !isStart;
}

void MainWindow::on_isCountDown_checkBox_stateChanged(int arg1)//是否倒计时复选框状态事件
{
    if(ui->isCountDown_checkBox->isChecked() == true)
    {
        reverseTime = 1;
        if(ui->hourInput_spinBox->value() == 0  && ui->minuteInput_spinBox->value() == 0 && ui->secondInput_spinBox->value() == 0)
            ui->start_pushButton->setEnabled(false);
        else
            ui->start_pushButton->setEnabled(true);
        ui->hourInput_spinBox->setEnabled(true);
        ui->minuteInput_spinBox->setEnabled(true);
        ui->secondInput_spinBox->setEnabled(true);
        ui->start_pushButton->setText("开始倒计时");
        ui->stop_pushButton->setText("停止倒计时");
        hour = QString::number(ui->hourInput_spinBox->value());
        minute = QString::number(ui->minuteInput_spinBox->value());
        second = QString::number(ui->secondInput_spinBox->value());
        if(hour.length() == 1)
            hour = "0" + hour;
        if(minute.length() == 1)
            minute = "0" + minute;
        if(second.length() == 1)
            second = "0" + second;
        result = hour + ":" + minute + ":" + second + ":000";
        ui->timeDisplay_lcdNumber->display(result);
    }
    if(ui->isCountDown_checkBox->isChecked() == false)
    {
        reverseTime = 0;
        result = "00:00:00:000";
        ui->timeDisplay_lcdNumber->display(result);
        ui->start_pushButton->setEnabled(true);
        ui->hourInput_spinBox->setEnabled(false);
        ui->minuteInput_spinBox->setEnabled(false);
        ui->secondInput_spinBox->setEnabled(false);
        ui->start_pushButton->setText("开始计时");
        ui->stop_pushButton->setText("停止计时");
    }
}

void MainWindow::on_hourInput_spinBox_valueChanged(int arg1)//时输入值改变事件
{
    if(ui->isCountDown_checkBox->isChecked() == true)
    {
        if(ui->hourInput_spinBox->value() == 0  && ui->minuteInput_spinBox->value() == 0 && ui->secondInput_spinBox->value() == 0)
            ui->start_pushButton->setEnabled(false);
        else
            ui->start_pushButton->setEnabled(true);
    }
    hour = QString::number(ui->hourInput_spinBox->value());
    minute = QString::number(ui->minuteInput_spinBox->value());
    second = QString::number(ui->secondInput_spinBox->value());
    if(hour.length() == 1)
        hour = "0" + hour;
    if(minute.length() == 1)
        minute = "0" + minute;
    if(second.length() == 1)
        second = "0" + second;
    result = hour + ":" + minute + ":" + second + ":000";
    ui->timeDisplay_lcdNumber->display(result);
}

void MainWindow::on_minuteInput_spinBox_valueChanged(int arg1)//分输入值改变事件
{
    if(ui->isCountDown_checkBox->isChecked() == true)
    {
        if(ui->hourInput_spinBox->value() == 0  && ui->minuteInput_spinBox->value() == 0 && ui->secondInput_spinBox->value() == 0)
            ui->start_pushButton->setEnabled(false);
        else
            ui->start_pushButton->setEnabled(true);
    }
    hour = QString::number(ui->hourInput_spinBox->value());
    minute = QString::number(ui->minuteInput_spinBox->value());
    second = QString::number(ui->secondInput_spinBox->value());
    if(hour.length() == 1)
        hour = "0" + hour;
    if(minute.length() == 1)
        minute = "0" + minute;
    if(second.length() == 1)
        second = "0" + second;
    result = hour + ":" + minute + ":" + second + ":000";
    ui->timeDisplay_lcdNumber->display(result);
}

void MainWindow::on_secondInput_spinBox_valueChanged(int arg1)//秒输入值改变事件
{
    if(ui->isCountDown_checkBox->isChecked() == true)
    {
        if(ui->hourInput_spinBox->value() == 0  && ui->minuteInput_spinBox->value() == 0 && ui->secondInput_spinBox->value() == 0)
            ui->start_pushButton->setEnabled(false);
        else
            ui->start_pushButton->setEnabled(true);
    }
    hour = QString::number(ui->hourInput_spinBox->value());
    minute = QString::number(ui->minuteInput_spinBox->value());
    second = QString::number(ui->secondInput_spinBox->value());
    if(hour.length() == 1)
        hour = "0" + hour;
    if(minute.length() == 1)
        minute = "0" + minute;
    if(second.length() == 1)
        second = "0" + second;
    result = hour + ":" + minute + ":" + second + ":000";
    ui->timeDisplay_lcdNumber->display(result);
}

void MainWindow::on_record_pushButton_clicked()
{
    QString  tempData = result + "\n";
    recordedData = tempData + recordedData;
    ui->dataRecorder_textBrowser->setMarkdown(recordedData);
}
