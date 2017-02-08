#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void finished();

private:
    Ui::MainWindow *ui;
};

#endif // BASEWINDOW_H
