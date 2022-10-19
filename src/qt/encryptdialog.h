#ifndef ENCRYPTDIALOG_H
#define ENCRYPTDIALOG_H

#include "walletmodel.h"
#include <QDialog>

namespace Ui {
class EncryptDialog;
}

class EncryptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EncryptDialog(QWidget *parent = 0);
    ~EncryptDialog();

    void setModel(WalletModel* model);
    bool matchNewPasswords();

private Q_SLOTS:
    void on_btnCancel();
    void on_acceptPassphrase();
    void validateNewPass();
    void validateNewPassRepeat();
    void on_showPassphraseCheckBox_clicked();

private:
    Ui::EncryptDialog *ui;
    WalletModel* model;
    void closeEvent(QCloseEvent *event);
};

#endif // ENCRYPTDIALOG_H
