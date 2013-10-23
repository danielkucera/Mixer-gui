#ifndef TEST_H
#define TEST_H

#include <QWidget>

namespace Ui {
class Test;
}

class Test : public QWidget
{
    Q_OBJECT
    
public:
    explicit Test(QWidget *parent = 0);
    ~Test();
    
private:
    Ui::Test *ui;
};

#endif // TEST_H
