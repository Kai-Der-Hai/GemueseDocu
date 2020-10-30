#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

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
    void on_btn_neuesGemuese_clicked();

    void on_listWidget_alleGemuese_itemClicked(QListWidgetItem *item);

    void on_btn_leseGemueseListe_clicked();

    void on_btn_speichereGemueseJson_clicked();


private:
    Ui::MainWindow *ui;
    QString m_gemuesePath;
    QString m_gemueseName;
};
#endif // MAINWINDOW_H
