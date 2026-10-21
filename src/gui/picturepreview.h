#ifndef PICTUREPREVIEW_H
#define PICTUREPREVIEW_H

#include <QDialog>

namespace Ui {
class picturePreview;
}

class picturePreview : public QDialog
{
    Q_OBJECT

public:
    explicit picturePreview(QWidget *parent = nullptr);
    ~picturePreview();
    void setImagePath(const QString &imagePath);

private:
    Ui::picturePreview *ui;
};

#endif // PICTUREPREVIEW_H
