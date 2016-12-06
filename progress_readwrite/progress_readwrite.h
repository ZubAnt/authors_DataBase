#ifndef PROGRESS_WRITE_H
#define PROGRESS_WRITE_H

#include <QWidget>

namespace Ui {
class progress_write;
}

class progress_write : public QWidget
{
    Q_OBJECT

public:
    explicit progress_write(QWidget *parent = 0);
    void set_progressbar_percent(const int percent);
    void set_progressbar_process(const QString &process);
    ~progress_write();

private:
    Ui::progress_write *ui;
};

#endif // PROGRESS_WRITE_H
