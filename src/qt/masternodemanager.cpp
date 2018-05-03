#include "masternodemanager.h"
#include "ui_masternodemanager.h"

#include "clientmodel.h"
#include "walletmodel.h"
#include "activemasternode.h"
#include "masternodeconfig.h"
#include "masternode.h"

#include <QAbstractItemDelegate>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QScrollArea>
#include <QScroller>
#include <QDateTime>

MasternodeManager::MasternodeManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MasternodeManager),
    clientModel(0),
    walletModel(0)
{
    ui->setupUi(this);

//    QScroller::grabGesture(ui->scrollArea, QScroller::LeftMouseButtonGesture);
//    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateNodeList()));
    timer->start(30000);

    updateNodeList();
}

MasternodeManager::~MasternodeManager()
{
    delete ui;
}

void MasternodeManager::updateNodeList()
{
    // Load masternodes into a map for faster lookups.
    QMap<QString, CMasterNode*> mapMasternodes;
    BOOST_FOREACH(CMasterNode& mn, vecMasternodes)
    {
        std::string addr = mn.addr.ToString();
        mapMasternodes[QString::fromStdString(addr).normalized(QString::NormalizationForm_D)] = &mn;
    }

    std::vector<pair<unsigned int, CTxIn>> vecMasternodeScores = GetMasternodeScores(pindexBest->nHeight);

    // Update existing masternode rows.
    for(int i=0; i < ui->tableWidget->rowCount(); i++)
    {
        QString addr = ui->tableWidget->item(i, 0)->text();

        if (mapMasternodes.contains(addr))
        {
            CMasterNode *mn = mapMasternodes.value(addr);

            // populate list
            updateNodeListRow(mn, vecMasternodeScores, i);

            mapMasternodes.remove(addr);
        }
        else
        {
            // Remove the row if the masternode can't be found.
            ui->tableWidget->removeRow(i);
            i--;
        }
    }

    // Add new masternode rows.
    QMapIterator<QString, CMasterNode*> mnIterator(mapMasternodes);
    while (mnIterator.hasNext())
    {
        mnIterator.next();
        CMasterNode *mn = mnIterator.value();

        int lastRow = ui->tableWidget->rowCount();

        // populate list
        ui->tableWidget->insertRow(lastRow);
        updateNodeListRow(mn, vecMasternodeScores, lastRow);
    }
}

void MasternodeManager::updateNodeListRow(CMasterNode *mn, std::vector<pair<unsigned int, CTxIn>>& vecMasternodeScores, int mnRow)
{
    // Address, Rank, Active, Active Seconds, Last Seen, Pub Key
    QTableWidgetItem *activeItem = new QTableWidgetItem(QString::number(mn->IsEnabled()));
    QTableWidgetItem *addressItem = new QTableWidgetItem(QString::fromStdString(mn->addr.ToString()).normalized(QString::NormalizationForm_D));
    QTableWidgetItem *rankItem = new QTableWidgetItem(QString::number(GetMasternodeRank(mn->vin, vecMasternodeScores)));
    QTableWidgetItem *activeSecondsItem = new QTableWidgetItem(QString::number((qint64)(mn->lastTimeSeen - mn->now)));
    int64_t unixTime = mn->lastTimeSeen;
    QDateTime timestamp;
    timestamp.setTime_t(unixTime);

    QTableWidgetItem *lastSeenItem = new QTableWidgetItem(timestamp.toString(Qt::SystemLocaleShortDate));

    CScript pubkey;
    pubkey = GetScriptForDestination(mn->pubkey.GetID());
    CTxDestination address1;
    ExtractDestination(pubkey, address1);
    CBitcoinAddress address2(address1);
    QTableWidgetItem *pubkeyItem = new QTableWidgetItem(QString::fromStdString(address2.ToString()));

    ui->tableWidget->setItem(mnRow, 0, addressItem);
    ui->tableWidget->setItem(mnRow, 1, rankItem);
    ui->tableWidget->setItem(mnRow, 2, activeItem);
    ui->tableWidget->setItem(mnRow, 3, activeSecondsItem);
    ui->tableWidget->setItem(mnRow, 4, lastSeenItem);
    ui->tableWidget->setItem(mnRow, 5, pubkeyItem);
}

void MasternodeManager::setClientModel(ClientModel *model)
{
    this->clientModel = model;
    if(model)
    {
    }
}

void MasternodeManager::setWalletModel(WalletModel *model)
{
    this->walletModel = model;
    if(model && model->getOptionsModel())
    {
    }

}

