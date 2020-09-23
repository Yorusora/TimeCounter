#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QTimer *timer;
    int src_ms;
    int src_s;
    int src_m;
    int src_h;
    bool isStart;
    int reverseTime;
    QString hour,minute,second,msecond;
    int rhour,rminute,rsecond,rmsecond;
    QString result;
    QString recordedData;

private slots:
    void on_start_pushButton_clicked();

    void on_stop_pushButton_clicked();

    void updateTime();

    void on_reset_pushButton_clicked();

    void on_pauseOrContinue_pushButton_clicked();

    void on_isCountDown_checkBox_stateChanged(int arg1);

    void on_hourInput_spinBox_valueChanged(int arg1);

    void on_minuteInput_spinBox_valueChanged(int arg1);

    void on_secondInput_spinBox_valueChanged(int arg1);

    void on_record_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
