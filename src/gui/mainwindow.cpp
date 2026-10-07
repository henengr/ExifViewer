#include <QFileDialog>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMessageBox>
#include <QHeaderView>
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../include/JFIF.hpp"
#include "../../include/IFD.hpp"
#include "../../include/IFDReader.hpp"
#include "../../include/JPEGReader.hpp"

namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Open file"),
        "",
        tr("Images (*.jpg, *.jpeg);;All Files(*)")
    );

    if (fileName.isEmpty()){
        return;
    }

    qDebug() << "Selected file:" << fileName;

    QPixmap pixmap(fileName);
    if (pixmap.isNull()) {
        QMessageBox::warning(this, tr("Error"), tr("Loading an image failed."));
        return;
    }

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addPixmap(pixmap);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);



    fs::path filePath = fileName.toStdWString();

    if (!fs::exists(filePath)){
        qDebug() << "File not found. :" << filePath.string();
        return;
    }

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()){
        qDebug() << "File could not be opened. :" << filePath.string();
        return;
    }

    auto pjpg = std::make_unique<JPEGReader>(file);

    std::int32_t ret;
    enEndian endian;
    stAPP1 app1;

    ret = pjpg->readSegment(static_cast<std::uint16_t>(enSegmentMarker::APP1));
    if (ret < 0){
        qDebug() << "Invalid file. : " << filePath.string();
        return;
    }

    ret = pjpg->parseAPP1(endian, app1);
    if (ret < 0){
        qDebug() << "File parsing error. " << filePath.string();
        return;
    }

    std::vector<std::uint8_t> &data = app1.data;
    auto rifd = std::make_unique<IFDReader>(data);

    std::vector<stIFD> ifds;
    rifd->readIFDs(ifds);

    QStandardItemModel *model = new QStandardItemModel(10, 5, this);

    model->setHeaderData(0, Qt::Horizontal, "Tag");
    model->setHeaderData(1, Qt::Horizontal, "Type");
    model->setHeaderData(2, Qt::Horizontal, "Count");
    model->setHeaderData(3, Qt::Horizontal, "Description");
    model->setHeaderData(4, Qt::Horizontal, "Value");

    int row = 0;
    std::string strLat, strLong;
    double dbLat, dbLong;
    for (stIFD ifd: ifds){
        QStandardItem *itemTag = new QStandardItem(QString("0x%1").arg(ifd.tag, 4, 16, QChar('0')));
        itemTag->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        model->setItem(row, 0, itemTag);
        //delete itemTag;

        QStandardItem *itemType = new QStandardItem(QString("0x%1").arg(ifd.type, 4, 16, QChar('0')));
        itemType->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        model->setItem(row, 1, itemType);
        //delete itemType;

        QStandardItem *itemCount = new QStandardItem(QString("0x%1").arg(ifd.count, 8, 16, QChar('0')));
        itemCount->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        model->setItem(row, 2, itemCount);
        //delete itemType;

        QStandardItem *itemDesc = new QStandardItem(QString("%1").arg(ifd.description));
        itemDesc->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        model->setItem(row, 3, itemDesc);
        //delete itemType;

        stIFDValue vals;
        IFDReader::getVariantIFD(ifd, endian, vals);
        std::stringstream ss;
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::BYTE) ||
            vals.IFD.type == static_cast<uint16_t>(enIFDType::UNDEFINED)){
            std::vector<std::uint8_t> v = std::get<std::vector<std::uint8_t>>(vals.values);
            for (auto i = v.begin(); i != v.end(); i++){
                ss << "0x" << std::hex << std::right << std::setw(2) << std::setfill('0') << static_cast<int>(*i) << " ";
            }
        }
        if (vals.IFD.type == static_cast<std::uint16_t>(enIFDType::ASCII) ||
            vals.IFD.type == static_cast<std::uint16_t>(enIFDType::UTF8)){
            ss << std::get<std::string>(vals.values);
        }
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::SHORT)){
            std::vector<std::uint16_t> v = std::get<std::vector<std::uint16_t>>(vals.values);
            for (auto i = v.begin(); i != v.end(); i++){
                ss << "0x" << std::hex << std::right << std::setw(4) << std::setfill('0') << *i << " ";
            }
        }
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::LONG)){
            std::vector<std::uint32_t> v = std::get<std::vector<std::uint32_t>>(vals.values);
            for (auto i = v.begin(); i != v.end(); i++){
                ss << "0x" << std::hex << std::right << std::setw(8) << std::setfill('0') << *i << " ";
            }
        }
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::RATIONAL)){
            std::vector<stRational> v = std::get<std::vector<stRational>>(vals.values);
            for (auto i = v.begin(); i != v.end(); i++){
                ss << "0x" << std::hex << std::right << std::setw(8) << std::setfill('0') << i->long1 << "/";
                ss << "0x" << std::hex << std::right << std::setw(8) << std::setfill('0') << i->long2 << " ";
            }
        }

        QStandardItem *itemValue = new QStandardItem(QString("%1").arg(ss.str()));
        itemValue->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        model->setItem(row, 4, itemValue);
        //delete itemValue;

        if (ifd.tag == 0x010f){     // Make
            std::string str = std::get<std::string>(vals.values);
            ui->labelVendor->setText(QString::fromStdString(str));
        }
        if (ifd.tag == 0x0110){     // Model
            std::string str = std::get<std::string>(vals.values);
            ui->labelModel->setText(QString::fromStdString(str));
        }
        if (ifd.tag == 0x8827){     // PhotographicSensitivity
            std::vector<std::uint16_t> v = std::get<std::vector<std::uint16_t>>(vals.values);
            ui->labelISO->setText(QString("%1").arg(v[0]));
        }
        if (ifd.tag == 0x9201){     // ShutterSpeedValue
            std::vector<stSRational> stRat = std::get<std::vector<stSRational>>(vals.values);
            ui->labelShutterSpeed->setText(QString("%1").arg(stRat[0].slong1 / stRat[1].slong2));
        }
        if (ifd.tag == 0x9003){     // DateTimeOriginal
            std::string str = std::get<std::string>(vals.values);
            ui->labelDateTimeOrg->setText(QString::fromStdString(str));
        }

        std::stringstream sloc;
        if (ifd.tag == 0x0002){     // GPSLatitude
            std::vector<stRational> lats = std::get<std::vector<stRational>>(vals.values);
            sloc << lats[0].long1 << "°" << lats[1].long1 << "\'" << (lats[2].long1 / 100.0) << "\"";
            strLat += sloc.str();
            dbLat = lats[0].long1 + (lats[1].long1 / 60.0) + (lats[2].long1 / 3600.0);
        }
        if (ifd.tag == 0x0001){
            strLat += std::get<std::string>(vals.values);      // Latitude symbol 'N'/'S'
        }
        if (ifd.tag == 0x0004){     // GPSLatitude
            std::vector<stRational> longs = std::get<std::vector<stRational>>(vals.values);
            sloc << longs[0].long1 << "°" << longs[1].long1 << "\'" << (longs[2].long1 / 100.0) << "\"";
            strLong += sloc.str();
            dbLong = longs[0].long1 + (longs[1].long1 / 60.0) + (longs[2].long1 / 3600.0);
        }
        if (ifd.tag == 0x0003){
            strLong += std::get<std::string>(vals.values);      // Longitude symbol 'E'/'W'
        }
        if (strLat.length() > 0 && strLong.length() > 0){
            std::string strLocation = strLat + " " + strLong;
            std::string strURL = "https://www.google.com/maps/place/" + strLat + "+" + strLong;
            //std::string strURL = "https://www.google.com/maps/search/?api=1&query=" +
                                 std::to_string(dbLat) + "%2C%20" + std::to_string(dbLong);
            // https://www.google.com/maps/place/35%C2%B040'57.0%22N+139%C2%B045'10.0%22E
            // https://www.google.com/maps/search/?api=1&query=36.807786%2C%20139.711822
            // https://www.google.com/maps/search/?api=1&query=37.983333N36%C2%B059%274417

            QString linkHtml = QString("<a href=\"%1\">%2</a>")
                                   .arg(QString::fromStdString(strURL))
                                   .arg(QString::fromStdString(strLocation));

            ui->labelLocation->setText(linkHtml);
        }

        row++;
    }

    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableView->verticalHeader()->setDefaultSectionSize(30);

    ui->tableView->setColumnWidth(0, 100);
    ui->tableView->setColumnWidth(1, 100);
    ui->tableView->setColumnWidth(2, 150);
    ui->tableView->setColumnWidth(3, 250);
    ui->tableView->setColumnWidth(4, 600);

    ui->tableView->setModel(model);
    //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void MainWindow::on_action_Quit_triggered()
{
    close();
}

