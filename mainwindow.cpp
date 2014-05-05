#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QComboBox>
#include <QStatusBar>
#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->stringCount, SIGNAL(valueChanged(int)), ui->oglWidget, SLOT(updateStringCount(int)));
    connect(ui->oglWidget, SIGNAL(signalFPS(QString)), ui->statusbar, SLOT(showMessage(QString)));
    connect(ui->fontMethod, SIGNAL(currentIndexChanged(QString)), ui->oglWidget, SLOT(setFontRenderer(QString)));
    connect(ui->varyHeight, SIGNAL(toggled(bool)), ui->oglWidget, SLOT(varyHeight(bool)));
    connect(ui->rotate, SIGNAL(toggled(bool)), ui->oglWidget, SLOT(rotate(bool)));
    connect(ui->varyColor, SIGNAL(toggled(bool)), ui->oglWidget, SLOT(varyColor(bool)));

    ui->fontMethod->addItems(QStringList(FontRenderer::listRenderer()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
