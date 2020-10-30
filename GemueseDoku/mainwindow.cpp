#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gemuese.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Anlegen eines neuen Gemüses, in der Liste und bei späterem Speichern auch als Json
void MainWindow::on_btn_neuesGemuese_clicked()
{
    m_gemueseName = ui->txt_neuesGemuese->toPlainText();

    ui->listWidget_alleGemuese->addItem(m_gemueseName);

    if(m_gemuesePath == 0)
    {
        m_gemuesePath = QFileDialog::getExistingDirectory(this, "Bitte waehle den Pfad");
    }

    QDir directory(m_gemuesePath);
    QStringList listOfGemuese = directory.entryList(QStringList() << "*.json", QDir::Files);

    if(!listOfGemuese.contains(m_gemueseName + ".json"))
    {
        QString filename = m_gemuesePath + "/" + m_gemueseName + ".json";
        QFile jsonFile(filename);
        jsonFile.open(QFile::WriteOnly);
        QJsonObject jsonObject;
        jsonObject["Name"] = "";
        jsonObject["Saeen"] = "";
        jsonObject["Vorzucht"] = "";
        jsonObject["Nachbarn"] = "";
        jsonObject["Pflanzabstand in Reihe"] = "";
        jsonObject["Pflanzabstand Reihen"] = "";
        jsonObject["Kommentare"] = "";
        jsonObject["Beachten bei neuer Pflanzung"] = "";
        QJsonDocument jsonDocument(jsonObject);
        jsonFile.write(jsonDocument.toJson());
        jsonFile.close();
    }

    ui->listWidget_alleGemuese->setCurrentItem(ui->listWidget_alleGemuese->item(-1));

    ui->txt_neuesGemuese->clear();
}

void MainWindow::on_listWidget_alleGemuese_itemClicked(QListWidgetItem *item)
{

    m_gemueseName = item->text();

    if(m_gemuesePath == 0)
    {
        m_gemuesePath = QFileDialog::getExistingDirectory(this, "Bitte waehle den Pfad");
    }

    QFile jsonFile(m_gemuesePath + "/" + m_gemueseName + ".json");
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        QMessageBox box;
        box.setText("Cannot open " + m_gemueseName + ".json File doesn't exist");
        box.setIcon(QMessageBox::Critical);
        box.addButton("OK", QMessageBox::AcceptRole);
        box.exec();
    }

    QJsonParseError JsonParseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonFile.readAll(), &JsonParseError);
    jsonFile.close();
    QJsonObject jsonObject = jsonDocument.object();

    ui->txt_saeen->clear();
    ui->txt_saeen->insertPlainText(jsonObject["Saeen"].toString());
    ui->txt_vorzucht->clear();
    ui->txt_vorzucht->insertPlainText(jsonObject["Vorzucht"].toString());
    ui->txt_nachbarn->clear();
    ui->txt_nachbarn->insertPlainText(jsonObject["Nachbarn"].toString());
    ui->txt_pflanzabstaende_inReihe->clear();
    ui->txt_pflanzabstaende_inReihe->insertPlainText(jsonObject["Pflanzabstand in Reihe"].toString());
    ui->txt_pflanzabstaende_Reihen->clear();
    ui->txt_pflanzabstaende_Reihen->insertPlainText(jsonObject["Pflanzabstand Reihen"].toString());
    ui->txt_kommentare->clear();
    ui->txt_kommentare->insertPlainText(jsonObject["Kommentare"].toString());
    ui->txt_beachtenBeiNeuerPflanzung->clear();
    ui->txt_beachtenBeiNeuerPflanzung->insertPlainText(jsonObject["Beachten bei neuer Pflanzung"].toString());
}

//Einlesen aller json Dateien, die in dem gewünschten Ordner liegen
void MainWindow::on_btn_leseGemueseListe_clicked()
{
    m_gemuesePath = QFileDialog::getExistingDirectory(this, "Bitte waehle den Pfad");

    QDir directory(m_gemuesePath);

    QStringList listOfGemuese = directory.entryList(QStringList() << "*.json", QDir::Files);
    foreach(QString gemueseWithExtension, listOfGemuese)
    {
        QString gemuese = gemueseWithExtension.split(QLatin1Char('.')).at(0);
        if (ui->listWidget_alleGemuese->findItems(gemuese, Qt::MatchExactly).empty())
        {
            ui->listWidget_alleGemuese->addItem(gemuese);
        }
    }
}

void MainWindow::on_btn_speichereGemueseJson_clicked()
{
    if(m_gemuesePath == 0)
    {
        m_gemuesePath = QFileDialog::getExistingDirectory(this, "Bitte waehle den Pfad");
    }

    QJsonObject jsonObject;
    jsonObject["Name"] = m_gemueseName;
    jsonObject["Saeen"] = ui->txt_saeen->toPlainText();
    jsonObject["Vorzucht"] = ui->txt_vorzucht->toPlainText();
    jsonObject["Nachbarn"] = ui->txt_nachbarn->toPlainText();
    jsonObject["Pflanzabstand in Reihe"] = ui->txt_pflanzabstaende_inReihe->toPlainText();
    jsonObject["Pflanzabstand Reihen"] = ui->txt_pflanzabstaende_Reihen->toPlainText();
    jsonObject["Kommentare"] = ui->txt_kommentare->toPlainText();
    jsonObject["Beachten bei neuer Pflanzung"] = ui->txt_beachtenBeiNeuerPflanzung->toPlainText();

    QString filename = m_gemuesePath + "/" + m_gemueseName + ".json";
    QFile jsonFile(filename);

    if(!jsonFile.open(QFile::WriteOnly))
    {
        QMessageBox box;
        box.setText("Die Datei konnte nicht angelegt werden");
        box.setIcon(QMessageBox::Critical);
        box.addButton("OK", QMessageBox::AcceptRole);
        box.exec();
        return;
    }

    QJsonDocument document(jsonObject);
    jsonFile.write(document.toJson());
    jsonFile.close();

    //Confirm succesfully export:
    QMessageBox box;
    box.setText("Die Datei wurde erfolgreich angelegt");
    box.setIcon(QMessageBox::Information);
    box.addButton("OK", QMessageBox::AcceptRole);
    box.exec();
}
