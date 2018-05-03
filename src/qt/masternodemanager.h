#ifndef MASTERNODEMANAGER_H
#define MASTERNODEMANAGER_H

#include "masternode.h"

#include <QWidget>
#include <QTimer>

namespace Ui {
    class MasternodeManager;
}
class ClientModel;
class WalletModel;

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

/** Masternode Manager page widget */
class MasternodeManager : public QWidget
{
    Q_OBJECT

public:
    explicit MasternodeManager(QWidget *parent = 0);
    ~MasternodeManager();

    void setClientModel(ClientModel *clientModel);
    void setWalletModel(WalletModel *walletModel);


public slots:
    void updateNodeList();

signals:

private:
    QTimer *timer;
    Ui::MasternodeManager *ui;
    ClientModel *clientModel;
    WalletModel *walletModel;

private slots:
    void updateNodeListRow(CMasterNode *mn, std::vector<pair<unsigned int, CTxIn>>& vecMasternodeScores, int mnRow);

};

#endif // MASTERNODEMANAGER_H
