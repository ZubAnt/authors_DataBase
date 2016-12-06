#include "progress_readwrite.h"
#include "ui_progress_readwrite.h"

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

void progress_write::set_progressbar_percent(const int percent)
{
    ui->progressbar_write_data->setValue(percent);
}

void progress_write::set_progressbar_process(const QString &process)
{
    ui->label_process->setText(process);
}
