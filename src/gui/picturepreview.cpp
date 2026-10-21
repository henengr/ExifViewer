#include "picturepreview.h"
#include "ui_picturepreview.h"

picturePreview::picturePreview(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::picturePreview)
{
    ui->setupUi(this);

    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowMaximizeButtonHint;
    setWindowFlags(flags);
}

picturePreview::~picturePreview()
{
    delete ui;
}

void picturePreview::setImagePath(const QString &imagePath)
{
    QString html = QString("<img src=\"%1\" width=\"100%\">").arg(imagePath);
    ui->textBrowser->setHtml(html);
}

