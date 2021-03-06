#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <iostream>
#include <QFileInfo>
//#include <QtAndroid>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSslError>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <QAction>
#include <QMenu>
#include <QPointer>
#include <QMessageBox>
#include <memory>
#include "qmllistobject.h"
#include "httpaccess.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

    QNetworkAccessManager *Network_Access;
    QNetworkRequest *Network_Request;
    QNetworkReply *Network_Reply;

    QJsonObject Json_Object;
    QJsonDocument Json_Document;

    void lockInterface(bool Value);
    void listDiseases(const QStringList &Diseases);
    void listDiffirentialDiagnosis(const QStringList &DifferentialDiagnosis);
    void listResult(const QStringList &JointDiseases);

    std::shared_ptr<QmlListObject> Possible_Symptom_List;
    std::shared_ptr<QmlListObject> Examine_Symptom_List;

    std::shared_ptr<QmlListObject> Matched_Diseases_List;
    std::shared_ptr<QmlListObject> Examine_Diseases_List;

    std::shared_ptr<QmlListObject> Differantial_Diagnosis_List;
    std::shared_ptr<QmlListObject> Examine_Diagnosis_List;

    std::shared_ptr<QmlListObject> Result_List;
    QStringList Query_Result;

    QMessageBox *Popup_Message;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void searchCurrentTextChanged(QString);
    void lookForDiseaseButtonClicked();
    void clearExamineListClicked();
    void examineDiseasesButtonClicked();
    void examineDiffirentialSymptomsButtonClicked();
    void examineJointDiseasesButtonClicked();

    void addToExaminedSymptomsListButtonClicked();
    void addAllToExaminedSymptomsList();

    void addToExamineDiseasesListButtonClicked();
    void addAllToExaminedDiseasesButtonClicked();

    void addToExamineDiagnosisListButtonClicked();
    void addAllToExeminedDiagnosisButtonClicked();


private slots:

    void clearResultButtonClicked();
    void clearExamineDiseaseListButtonClicked();
    void clearExamineDiagnosisListButtonClicked();

private:
    Ui::MainWindow *ui;

    HttpAccess Http_Access;

};
#endif // MAINWINDOW_H
