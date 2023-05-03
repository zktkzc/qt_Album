#ifndef PROSETPAGE_H
#define PROSETPAGE_H

#include <QWizardPage>
#include <QLineEdit>
#include <QDir>

namespace Ui {
class ProSetPage;
}

class ProSetPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ProSetPage(QWidget *parent = nullptr);
    ~ProSetPage();

protected:
    bool isComplete() const;

private:
    Ui::ProSetPage *ui;
};

#endif // PROSETPAGE_H
