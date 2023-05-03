#ifndef PROSETPAGE_H
#define PROSETPAGE_H

#include <QWizardPage>
#include <QLineEdit>
#include <QDir>
#include <QFileDialog>
#include <QStringList>

namespace Ui {
class ProSetPage;
}

class ProSetPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ProSetPage(QWidget *parent = nullptr);
    ~ProSetPage();
    void getProSettings(QString& name, QString& path);

protected:
    bool isComplete() const;

private slots:
    void on_pushButton_clicked();

private:
    Ui::ProSetPage *ui;
};

#endif // PROSETPAGE_H
