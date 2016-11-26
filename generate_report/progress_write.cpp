#include "progress_write.h"
#include "ui_progress_write.h"

progress_write::progress_write(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::progress_write)
{
    ui->setupUi(this);
}

progress_write::~progress_write()
{
    delete ui;
}

void progress_write::set_progressbar(const int percent)
{
    ui->progressBar_write_report->setValue(percent);
}
