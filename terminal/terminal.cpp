#include "terminal.h"
#include "ui_terminal.h"
#include "chart.h"

#include <QTimer>
#include <QMessageBox>
#include <QDateTime>


const int kTimerTime = 1000;

Terminal::Terminal(QWidget* parent /*= nullptr*/, Qt::WindowFlags fl /* = Qt::WindowFlags()*/):
  QDialog(parent, fl),
  ui_(new Ui::Terminal())
{
  ui_->setupUi(this);
  chart_ = new Chart();
  ui_->plot->setChart(chart_);

  ui_->plot->chart()->setTheme(QChart::ChartThemeDark);
  setStyleSheet("QDialog {"
		"background-color: #202330;"
		"}"
		"QLabel {"
		"color: #d6d6d6}"
		"QPushButton:enabled"
		"{"
		"background-color: #4e505f;"
		"color: #e7e7e7;"
		"}"
		"QPushButton:disabled {"
		"background-color: #ebe9d1;"
		"color: #676767;"
		"}"	
		);

  ui_->money->setText(QString::number(money_));
  ui_->dt->setText(QDateTime::currentDateTimeUtc().toString("dd-MM-yyyy hh:mm"));
  
  connect(ui_->buy, &QPushButton::released, this, &Terminal::openDial);
  connect(&timer_, &QTimer::timeout, this, &Terminal::handleTimeout);
  
  timer_.setInterval(kTimerTime);
  timer_.start();
}


Terminal::~Terminal()
{
  delete ui_;
  ui_ = nullptr;
}


void Terminal::openDial()
{ 
  if (money_ < 10) {
    QMessageBox::critical(this, "You can't buy", "Not enough money!");
    ui_->buy->setDown(false);
    ui_->buy->setEnabled(true);
    return;
  }

  ui_->buy->setEnabled(false);
  
  cost_ = chart_->setPoint();
  money_ -= 10;
  
  ui_->money->setText(QString::number(money_));
  ui_->buy->setText("15");
  
  QTimer::singleShot(15000, this, SLOT(closeDial()));
}


void Terminal::closeDial()
{
  float curCost = chart_->getCost();

  if (curCost > cost_) {
    money_ += 20;
    ui_->money->setText(QString::number(money_));
  }

  chart_->removePoint();
  ui_->buy->setDown(false);
  ui_->buy->setText("Buy");
  ui_->buy->setEnabled(true);
}


void Terminal::handleTimeout()
{
  chart_->setNewCost(QDateTime::currentDateTimeUtc());
  
  ui_->dt->setText(QDateTime::currentDateTimeUtc().toString("dd-MM-yyyy hh:mm"));
  if (ui_->buy->isDown() || !ui_->buy->isEnabled()) {
    ui_->buy->setText(QString::number(ui_->buy->text().toInt() - 1));
  }
}


void Terminal::keyPressEvent(QKeyEvent *event)
{
  switch (event->key()) {
  case Qt::Key_Plus:
    chart_->zoomInBoth();
    break;
  case Qt::Key_Minus:
    chart_->zoomOutBoth();
    break;
  case Qt::Key_Left:
    chart_->scrollLeft();
    break;
  case Qt::Key_Right:
    chart_->scrollRight();
    break;
  case Qt::Key_Up:
    chart_->scrollUp();
    break;
  case Qt::Key_Down:
    chart_->scrollDown();
    break;
  case Qt::Key_R:
    chart_->resetView();
    break;
  default:
    QDialog::keyPressEvent(event);
    break;
  }
}
