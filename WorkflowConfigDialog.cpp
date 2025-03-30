#include "WorkflowConfigDialog.h"
#include <QSqlQueryModel>
#include <QVBoxLayout>

WorkflowConfigDialog::WorkflowConfigDialog(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    workflowTable = new QTableView(this);
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT * FROM workflow_steps");
    workflowTable->setModel(model);
    layout->addWidget(workflowTable);
}
