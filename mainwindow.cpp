#include "mainwindow.h"
#include <QFileDialog>
#include <QDebug>
//#include <QAndroidJniEnvironment>
#include <QResource>
#include <QStandardPaths>
#include <QPoint>
#include <QJsonArray>
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Possible_Symptom_List = std::make_shared<QmlListObject>(ui->possible_symptomps_list_widget);
    Examine_Symptom_List = std::make_shared<QmlListObject>(ui->examined_symptom_widget);
    Matched_Diseases_List = std::make_shared<QmlListObject>(ui->matched_diseases_widget);
    Examine_Diseases_List = std::make_shared<QmlListObject>(ui->examine_diseases_widget);
    Differantial_Diagnosis_List = std::make_shared<QmlListObject>(ui->differential_diagnosis_widget);
    Examine_Diagnosis_List = std::make_shared<QmlListObject>(ui->examine_diagnosis_widget);
    Result_List = std::make_shared<QmlListObject>(ui->result_widget);

    connect(ui->search_line, SIGNAL(textEdited(QString)), this, SLOT(searchCurrentTextChanged(QString)));
    connect(ui->clear_examine_list_button, SIGNAL(clicked()), this, SLOT(clearExamineListClicked()));
    connect(ui->examine_diseases_button, SIGNAL(clicked()), this, SLOT(examineDiseasesButtonClicked()));

    connect(ui->add_to_examined_diseases_list_button, SIGNAL(clicked(bool)), this, SLOT(addToExamineDiseasesListButtonClicked()));
    connect(ui->add_examine_diagnosis_list_button, SIGNAL(clicked(bool)), this, SLOT(addToExamineDiagnosisListButtonClicked()));
    connect(ui->add_to_examine_symptoms_button, SIGNAL(clicked()), this, SLOT(addToExaminedSymptomsListButtonClicked()));

    connect(ui->add_all_to_examined_diseases_button, SIGNAL(clicked(bool)), this, SLOT(addAllToExaminedDiseasesButtonClicked()));
    connect(ui->add_all_to_examined_diagnosis_button, SIGNAL(clicked(bool)), this, SLOT(addAllToExeminedDiagnosisButtonClicked()));
    connect(ui->add_all_to_examined_symptoms_button, SIGNAL(clicked()), this, SLOT(addAllToExaminedSymptomsList()));


    connect(ui->examine_differential_diagnosis_button, SIGNAL(clicked(bool)), this, SLOT(examineDiffirentialSymptomsButtonClicked()));
    connect(ui->examine_joint_result_button, SIGNAL(clicked(bool)), this, SLOT(examineJointDiseasesButtonClicked()));
    connect(ui->clear_result_button, SIGNAL(clicked(bool)), this, SLOT(clearResultButtonClicked()));
    connect(ui->clear_examine_diagnosis_list_button, SIGNAL(clicked(bool)), this, SLOT(clearExamineDiagnosisListButtonClicked()));
    connect(ui->clear_examine_disease_list_button, SIGNAL(clicked(bool)), this, SLOT(clearExamineDiseaseListButtonClicked()));

}



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::lockInterface(bool Value)
{
    ui->examine_diseases_button->setEnabled(!Value);
    ui->examine_differential_diagnosis_button->setEnabled(!Value);
    ui->examine_joint_result_button->setEnabled(!Value);

    if(Value)
    {
        Popup_Message = new QMessageBox;

        Popup_Message->setText("Please Wait...");
        Popup_Message->show();
    }
    else
    {
        Popup_Message->hide();
        Popup_Message->close();
        delete Popup_Message;
    }
}


void MainWindow::listDiseases(const QStringList &Diseases)
{

    Matched_Diseases_List->clearList();

    foreach (QString disease, Diseases)
        Matched_Diseases_List->addItem(disease);


    ui->tabWidget->setCurrentIndex(1);

}



void MainWindow::listDiffirentialDiagnosis(const QStringList &DifferentialDiagnosis)
{
    Differantial_Diagnosis_List->clearList();

    for(int i=0; i< (int)DifferentialDiagnosis.size(); i++)
        Differantial_Diagnosis_List->addItem(DifferentialDiagnosis[i]);

}



void MainWindow::listResult(const QStringList &JointDiseases)
{

    Result_List->clearList();

    for(int i=0; i< (int)JointDiseases.size(); i++)
    {
        bool exist = false;
        for(int k=0; k< Matched_Diseases_List->getSize(); k++)
        {
            if(Matched_Diseases_List->getItem(k) == JointDiseases.at(i))
               exist = true;
        }

        if(exist == true)
            Result_List->addItem(JointDiseases[i]);
    }

    if(Result_List->getSize() <= 0)
        Result_List->addItem("No Result");
}



void MainWindow::searchCurrentTextChanged(QString Text)
{

    Possible_Symptom_List->clearList();
    if(Text.isEmpty())
        return;

    lockInterface(true);

    QString function_name = "getSimilarSymptoms";
    QStringList params(Text);

    QStringList result = Http_Access.makeHttpJsonRequest(function_name, params);

    for(int i=0; i<(int)result.size(); i++)
    {
        Possible_Symptom_List->addItem(result.at(i));
    }

    lockInterface(false);


}



void MainWindow::lookForDiseaseButtonClicked()
{


}



void MainWindow::addToExaminedSymptomsListButtonClicked()
{

    if(Possible_Symptom_List->getSize() <= 0)
        return;

    Examine_Symptom_List->addItem(Possible_Symptom_List->getCurrentItemText());

}



void MainWindow::addAllToExaminedSymptomsList()
{
    for(int i=0; i<Possible_Symptom_List->getSize(); i++)
        Examine_Symptom_List->addItem(Possible_Symptom_List->getItem(i));
}



void MainWindow::clearExamineListClicked()
{

    Examine_Symptom_List->clearList();

}



void MainWindow::examineDiseasesButtonClicked()
{

    QStringList examined_symptoms;
    QStringList result_disease_list;
    QString function_name = "getPossibleDiseases";

    lockInterface(true);

    int list_size = Examine_Symptom_List->getSize();

    for(int i = 0; i < list_size; i++)
        examined_symptoms.append(Examine_Symptom_List->getItem(i));

    result_disease_list = Http_Access.makeHttpJsonRequest(function_name, examined_symptoms);

    listDiseases(result_disease_list);

    lockInterface(false);

}



void MainWindow::examineDiffirentialSymptomsButtonClicked()
{
    QStringList selected_diseases;
    QStringList result_symptoms;
    QString function_name = "getDifferentialSymptoms";

    int list_size = Examine_Diseases_List->getSize();

    lockInterface(true);

    ui->tabWidget->setCurrentIndex(2);

    for(int i=0; i<list_size; i++)
        selected_diseases.append(Examine_Diseases_List->getItem(i));


    result_symptoms = Http_Access.makeHttpJsonRequest(function_name, selected_diseases);

    listDiffirentialDiagnosis(result_symptoms);

    lockInterface(false);

}



void MainWindow::examineJointDiseasesButtonClicked()
{
    QStringList result_diseases;
    QStringList selected_symptoms;
    QString function_name = "getPossibleDiseases";

    int list_size = Examine_Diagnosis_List->getSize();

    if(list_size <= 0)
        return;

    lockInterface(true);
    ui->tabWidget->setCurrentIndex(3);

    for(int i=0; i<list_size; i++)
        selected_symptoms.append(Examine_Diagnosis_List->getItem(i));

    result_diseases = Http_Access.makeHttpJsonRequest(function_name, selected_symptoms);

    listResult(result_diseases);
    lockInterface(false);
}



void MainWindow::addToExamineDiseasesListButtonClicked()
{
    if(Matched_Diseases_List->getSize() <= 0)
        return;

    Examine_Diseases_List->addItem(Matched_Diseases_List->getCurrentItemText());
}



void MainWindow::addAllToExaminedDiseasesButtonClicked()
{
    for(int i=0; i<Matched_Diseases_List->getSize(); i++)
        Examine_Diseases_List->addItem(Matched_Diseases_List->getItem(i));
}



void MainWindow::addAllToExeminedDiagnosisButtonClicked()
{
    for(int i=0; i<Differantial_Diagnosis_List->getSize(); i++)
        Examine_Diagnosis_List->addItem(Differantial_Diagnosis_List->getItem(i));
}



void MainWindow::addToExamineDiagnosisListButtonClicked()
{
    if(Differantial_Diagnosis_List->getSize() <= 0)
        return;

    Examine_Diagnosis_List->addItem(Differantial_Diagnosis_List->getCurrentItemText());
}



void MainWindow::clearResultButtonClicked()
{
    Result_List->clearList();
    Differantial_Diagnosis_List->clearList();
    Matched_Diseases_List->clearList();
    Examine_Diseases_List->clearList();
    Examine_Diagnosis_List->clearList();
    Examine_Symptom_List->clearList();
    Possible_Symptom_List->clearList();
}



void MainWindow::clearExamineDiseaseListButtonClicked()
{
    Matched_Diseases_List->clearList();
    Examine_Diseases_List->clearList();
}



void MainWindow::clearExamineDiagnosisListButtonClicked()
{
    Differantial_Diagnosis_List->clearList();
    Examine_Diagnosis_List->clearList();
}
