#ifndef DOCUMENTDETAILSDIALOG_H
#define DOCUMENTDETAILSDIALOG_H

#include <QDialog>
#include <QJsonObject>

class QLabel;
class QTextEdit;

class DocumentDetailsDialog : public QDialog {
    Q_OBJECT
public:
    explicit DocumentDetailsDialog(int docId, QWidget *parent = nullptr);

private:
    void loadDocumentDetails(int docId);

    QLabel *titleLabel;
    QTextEdit *contentEdit;
    QLabel *statusLabel;
    int m_docId;
};

#endif // DOCUMENTDETAILSDIALOG_H
