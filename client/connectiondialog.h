#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include <QHostAddress>
#include <QMessageBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QEvent>
#include <QString>


class ConnectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConnectionDialog(QWidget* parent = nullptr);
    ~ConnectionDialog();

    void setAddressString(const QString& address, quint16 port);
    void setAddressString(const QHostAddress& address, quint16 port);
    void setLogin(const QString& login);
    void setPassword(const QString& password);
    QString getAddress() const;
    QString getLogin() const;
    QString getPassword() const;
    quint16 getPort();

public slots:
    void accept();

protected:
    bool eventFilter(QObject* object, QEvent* event);

private:
    QLineEdit* addressTextBox;
    QLineEdit* loginTextBox;
    QLineEdit* passwordTextBox;

    QPushButton* cancelButton;
    QPushButton* applyButton;

    QString address;
    QString login;
    QString password;
    quint16 port;
};

#endif // CONNECTIONDIALOG_H
