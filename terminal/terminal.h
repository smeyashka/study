#ifndef TASK_PLOT_H
#define TASK_PLOT_H

#include <QDialog>
#include <QChartView>
#include <QChart>
#include <QTimer>

QT_CHARTS_USE_NAMESPACE


namespace Ui {
  class Terminal;
}

class Chart;

class Terminal : public QDialog {
  Q_OBJECT
public:
  explicit Terminal(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());
  ~Terminal();

public slots:
  void openDial();
  void closeDial();
  void handleTimeout();

protected:
  void keyPressEvent(QKeyEvent *event);
    
private:

  Ui::Terminal* ui_ = nullptr;
  Chart* chart_ = nullptr;
  QTimer timer_;

  uint64_t money_ = 1000; //!< money in the account
  float cost_ = 0;        //!< user purchase
};

#endif
