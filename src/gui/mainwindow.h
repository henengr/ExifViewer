#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <filesystem>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_action_Open_triggered();

    void on_action_Quit_triggered();

    void on_action_About_triggered();

    void openPicturePreview();
private:
    Ui::MainWindow *ui;
    std::filesystem::path imagePath;
};
#endif // MAINWINDOW_H
