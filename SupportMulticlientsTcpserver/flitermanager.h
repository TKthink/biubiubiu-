#ifndef FLITERMANAGER_H
#define FLITERMANAGER_H

#include <QDialog>

namespace Ui {
class fliterManager;
}

class fliterManager : public QDialog
{
    Q_OBJECT

public:
    explicit fliterManager(QWidget *parent = nullptr);
    ~fliterManager();

private slots:
    void on_Add_clicked();

    void on_Properties_clicked();

    void on_Remove_clicked();

private:
    Ui::fliterManager *ui;
};

#endif // FLITERMANAGER_H
