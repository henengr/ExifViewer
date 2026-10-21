#include "picturepreview.h"
#include "ui_picturepreview.h"

picturePreview::picturePreview(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::picturePreview)
{
    ui->setupUi(this);
}

picturePreview::~picturePreview()
{
    delete ui;
}
