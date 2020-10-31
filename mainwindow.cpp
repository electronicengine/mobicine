#include "mainwindow.h"
#include <QFileDialog>
#include <QDebug>
//#include <QAndroidJniEnvironment>
#include <QResource>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QPoint>
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


    connect(ui->examine_differential_diagnosis_button, SIGNAL(clicked(bool)), this, SLOT(examineDiffirentialDiagnosisButtonClicked()));
    connect(ui->examine_joint_result_button, SIGNAL(clicked(bool)), this, SLOT(examineJointDiseasesButtonClicked()));
    connect(ui->clear_result_button, SIGNAL(clicked(bool)), this, SLOT(clearResultButtonClicked()));
    connect(ui->clear_examine_diagnosis_list_button, SIGNAL(clicked(bool)), this, SLOT(clearExamineDiagnosisListButtonClicked()));
    connect(ui->clear_examine_disease_list_button, SIGNAL(clicked(bool)), this, SLOT(clearExamineDiseaseListButtonClicked()));


    Network_Access = new QNetworkAccessManager(this);
    Network_Request = new QNetworkRequest(QUrl("http://www.yusufbulbul.com/mobicine.php"));
    Network_Request->setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
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

    for(int i=0; i< (int)Diseases.size(); i++)
        if(Diseases.at(i) != "")
            Matched_Diseases_List->addItem(Diseases[i]);

    if(Matched_Diseases_List->getSize() <= 0)
        Matched_Diseases_List->addItem("No Result");

    ui->tabWidget->setCurrentIndex(1);

}



void MainWindow::listDiffirentialDiagnosis(const QStringList &DifferentialDiagnosis)
{
    Differantial_Diagnosis_List->clearList();

    for(int i=0; i< (int)DifferentialDiagnosis.size(); i++)
        if(DifferentialDiagnosis.at(i) != "")
            Differantial_Diagnosis_List->addItem(DifferentialDiagnosis[i]);

    if(Differantial_Diagnosis_List->getSize() <= 0)
        Differantial_Diagnosis_List->addItem("No Result");
}



void MainWindow::listResult(const QStringList &JointDiseases)
{
//    for(int i=0; i< (int)JointDiseases.size(); i++)
//        ui->result_list->addItem(JointDiseases[i]);
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
            if(JointDiseases.at(i) != "")
                Result_List->addItem(JointDiseases[i]);
    }

    if(Result_List->getSize() <= 0)
        Result_List->addItem("No Result");
}



QByteArray MainWindow::getRequestJson(const QString &Query)
{
    QJsonObject jobj;
    jobj.insert("query", Query);
    QJsonDocument doc(jobj);

    return doc.toJson();
}



QStringList MainWindow::getHttpResponse(const QString &Query)
{

    int time_out_counter = 0;

    Query_Result.clear();

    Network_Reply = Network_Access->post(*Network_Request, getRequestJson(Query));

    while(!Network_Reply->isFinished())
    {
        if(time_out_counter++ >= NETWORK_TIMEOUT)
        {
            QMessageBox::information(this, "Connection", "Connection Error!");
            return QStringList();
        }

        qApp->processEvents();
    }


    QByteArray response = Network_Reply->readAll();

    Query_Result = QString(response).split('|');
    Query_Result = Query_Result.toSet().toList();

    return Query_Result;
}



void MainWindow::searchCurrentTextChanged(QString Text)
{

    Possible_Symptom_List->clearList();
    if(Text.isEmpty())
        return;

    lockInterface(true);

    QString query = "SELECT symptoms FROM mobicine WHERE symptoms LIKE '%" +
            Text +"%'";

    QStringList list = getHttpResponse(query);

    for(int i=0; i<(int)list.size(); i++)
    {
        if(list.at(i) != "")
            Possible_Symptom_List->addItem(list.at(i));


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
    QString query;
    std::vector<QStringList> diseases;
    std::vector<QString> symptoms;
    QStringList result_disease_list;


    int list_size = Examine_Symptom_List->getSize();

    if(list_size <= 0)
        return;

    lockInterface(true);

    for(int i=0; i< list_size; i++)
        symptoms.push_back(Examine_Symptom_List->getItem(i));



    for(int i=0; i<(int)list_size; i++)
    {
        query = "SELECT diseases FROM mobicine WHERE symptoms LIKE '%" +
                symptoms[i] + "%'";


        getHttpResponse(query);

        diseases.push_back(Query_Result);
    }

    if(diseases.empty())
        return;




    for(int i=0; i < (int) diseases.size(); i++)
    {
        for(int k=0; k<diseases[i].size(); k++)
        {

            QString sample_disease = diseases[i].at(k);

            int count = 0;

            for(int l=0; l < (int) diseases.size(); l++)
            {

                for(int m=0; m < diseases[l].size(); m++)
                {
                    if(sample_disease == diseases[l].at(m))
                    {
                        count++;
                    }
                }
            }


            if(count == list_size)
            {
                bool exist = false;
                for(int i=0; i<result_disease_list.size(); i++)
                    if(result_disease_list.at(i) == sample_disease)
                        exist = true;

                if(exist == false)
                    result_disease_list.push_back(sample_disease);
            }
        }

    }



    listDiseases(result_disease_list);
    lockInterface(false);

}



void MainWindow::examineDiffirentialDiagnosisButtonClicked()
{
    QStringList symptoms;
    QString selected_diseases;
    QStringList differential_diagnosis;
    QString query;
    int list_size = Examine_Diseases_List->getSize();

    if(list_size <= 0)
        return;

    lockInterface(true);
    ui->tabWidget->setCurrentIndex(2);

    for(int i=0; i<list_size; i++)
    {
        selected_diseases = Examine_Diseases_List->getItem(i);
        query = "SELECT symptoms FROM mobicine WHERE diseases LIKE '%" +
                selected_diseases + "%'";

        symptoms = getHttpResponse(query);

        for(int k = 0; k < symptoms.size(); k++)
        {
            bool exist = false;

            for(int l = 0; l < differential_diagnosis.size(); l++)
            {
                if(differential_diagnosis.at(l) == symptoms.at(k))
                {
                    differential_diagnosis.erase(differential_diagnosis.begin() + l,
                                                 differential_diagnosis.begin() + l + 1);
                    exist = true;
                }
            }

            if(exist == false)
            {
                bool exist_same = false;
                for(int q=0; q<differential_diagnosis.size(); q++)
                    if(differential_diagnosis.at(q) == symptoms.at(k))
                        exist_same = true;

                if(exist_same == false)
                    differential_diagnosis.push_back(symptoms.at(k));
            }

        }

    }

    listDiffirentialDiagnosis(differential_diagnosis);
    lockInterface(false);

}



void MainWindow::examineJointDiseasesButtonClicked()
{
    QStringList result_diseases;
    QString selected_diagnosis;
    std::vector<QStringList> diseases;
    QString query;
    int list_size = Examine_Diagnosis_List->getSize();

    if(list_size <= 0)
        return;

    lockInterface(true);
    ui->tabWidget->setCurrentIndex(3);


    for(int i=0; i<list_size; i++)
    {
        selected_diagnosis = Examine_Diagnosis_List->getItem(i);

        query = "SELECT diseases FROM mobicine WHERE symptoms LIKE '%" +
                selected_diagnosis + "%'";

        diseases.push_back(getHttpResponse(query));
    }

    if(diseases[0].empty())
        return;



    for(int i=0; i < (int) diseases.size(); i++)
    {
        for(int k=0; k<diseases[i].size(); k++)
        {
            QString sample_disease = diseases[i].at(k);

            int count = 0;

            for(int l=0; l < (int) diseases.size(); l++)
            {

                for(int m=0; m < diseases[l].size(); m++)
                {
                    if(sample_disease == diseases[l].at(m))
                    {
                        count++;
                    }
                }
            }


            if(count == list_size)
            {

                bool exist_same = false;
                for(int q=0; q<result_diseases.size(); q++)
                    if(result_diseases.at(q) == sample_disease)
                        exist_same = true;

                if(exist_same == false)
                    result_diseases.push_back(sample_disease);

            }
        }

    }

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
