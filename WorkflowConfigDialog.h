#ifndef WORKFLOWCONFIGDIALOG_H
#define WORKFLOWCONFIGDIALOG_H
#include <QDialog>
#include <QTableView>

class WorkflowConfigDialog : public QDialog {
    Q_OBJECT
public:
    explicit WorkflowConfigDialog(QWidget *parent = nullptr);
private:
    QTableView *workflowTable;
};
#endif // WORKFLOWCONFIGDIALOG_H
