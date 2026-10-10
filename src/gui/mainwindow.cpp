#include <QFileDialog>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMessageBox>
#include <QHeaderView>
#include <QMessageBox>
#include <string>
#include <sstream>
#include <cmath>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../include/IFD.hpp"
#include "../../include/IFDReader.hpp"
#include "../../include/ReaderFactory.hpp"


constexpr const char* APP_NAME = "ExifViewer ver.0.10";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(APP_NAME);
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
        tr("Images (*.jpg, *.jpeg, *.JPG, *.JPEG);;All Files(*)")
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

    std::unique_ptr<FileReader> reader = ReaderFactory::create(fileName.toStdString());
    //auto reader = std::make_unique<JPEGReader>(fileName.toStdString());

    enEndian endian;

    std::vector<std::uint8_t> data;
    reader->parse(data);

    auto rifd = std::make_unique<IFDReader>(data);

    endian = reader->getEndian();

    std::vector<stIFD> ifds;
    rifd->readIFDs(ifds);

    QStandardItemModel *model = new QStandardItemModel(10, 5, this);

    model->setHeaderData(0, Qt::Horizontal, "Tag");
    model->setHeaderData(1, Qt::Horizontal, "Type");
    model->setHeaderData(2, Qt::Horizontal, "Count");
    model->setHeaderData(3, Qt::Horizontal, "Description");
    model->setHeaderData(4, Qt::Horizontal, "Value");

    ui->tableView->setModel(model);

    ui->tableView->setColumnWidth(0, 60);
    ui->tableView->setColumnWidth(1, 60);
    ui->tableView->setColumnWidth(2, 100);
    ui->tableView->setColumnWidth(3, 220);
    ui->tableView->setColumnWidth(4, 1200);

    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableView->verticalHeader()->setDefaultSectionSize(26);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

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
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::SLONG)){
            std::vector<std::int32_t> v = std::get<std::vector<std::int32_t>>(vals.values);
            for (auto i = v.begin(); i != v.end(); i++){
                ss << "0x" << std::hex << std::right << std::setw(8) << std::setfill('0') << *i << " ";
            }
        }
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::SRATIONAL)){
            std::vector<stSRational> v = std::get<std::vector<stSRational>>(vals.values);
            for (auto i = v.begin(); i != v.end(); i++){
                ss << "0x" << std::hex << std::right << std::setw(8) << std::setfill('0') << i->slong1 << "/";
                ss << "0x" << std::hex << std::right << std::setw(8) << std::setfill('0') << i->slong2 << " ";
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
            std::uint32_t at = std::pow(2, (stRat[0].slong1 / stRat[0].slong2));
            ui->labelShutterSpeed->setText(QString("1/%1").arg(at));
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
            //dbLat = lats[0].long1 + (lats[1].long1 / 60.0) + (lats[2].long1 / 3600.0);
            double deg = (lats[0].long2 != 0) ? (double)lats[0].long1 / lats[0].long2 : 0;
            double min = (lats[1].long2 != 0) ? (double)lats[1].long1 / lats[1].long2 : 0;
            double sec = (lats[2].long2 != 0) ? (double)lats[2].long1 / lats[2].long2 : 0;
            dbLat = deg + (min / 60.0) + (sec / 3600.0);
        }
        if (ifd.tag == 0x0001){
            strLat += std::get<std::string>(vals.values);      // Latitude symbol 'N'/'S'
        }
        if (ifd.tag == 0x0004){     // GPSLatitude
            std::vector<stRational> longs = std::get<std::vector<stRational>>(vals.values);
            sloc << longs[0].long1 << "°" << longs[1].long1 << "\'" << (longs[2].long1 / 100.0) << "\"";
            strLong += sloc.str();
            //dbLong = longs[0].long1 + (longs[1].long1 / 60.0) + (longs[2].long1 / 3600.0);
            double deg = (longs[0].long2 != 0) ? (double)longs[0].long1 / longs[0].long2 : 0;
            double min = (longs[1].long2 != 0) ? (double)longs[1].long1 / longs[1].long2 : 0;
            double sec = (longs[2].long2 != 0) ? (double)longs[2].long1 / longs[2].long2 : 0;
            dbLong = deg + (min / 60.0) + (sec / 3600.0);
        }
        if (ifd.tag == 0x0003){
            strLong += std::get<std::string>(vals.values);      // Longitude symbol 'E'/'W'
        }
        if (strLat.length() > 0 && strLong.length() > 0){
            QString qstrURL = QString("https://www.google.com/maps/search/?api=1&query=%1, %2").arg(dbLat).arg(dbLong);
            //QString qstrLocation = QString("%1 %2").arg(dbLat).arg(dbLong);
            QString qstrLocation = QString("%1 %2").arg(strLat).arg(strLong);
            QString qstrLink = QString("<a href=\"%1\">%2</a>").arg(qstrURL).arg(qstrLocation);

            ui->labelLocation->setText(qstrLink);
        }

        row++;
    }

    std::string title = std::string(APP_NAME) + "  [" + fileName.toStdString() + "]";
    setWindowTitle(title.c_str());
}


void MainWindow::on_action_Quit_triggered()
{
    close();
}


void MainWindow::on_action_About_triggered()
{
    QMessageBox::information(this, "About", APP_NAME);
}

