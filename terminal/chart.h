#ifndef TASK_CHART_H
#define TASK_CHART_H

#include <QChart>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QScatterSeries;
class QValueAxis;
class QDateTimeAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

//NOTE
//Chart determines the price itself.
//This is not very good for the general case,
//but it is more convenient for these fake data with hardcoded plot settings.

class Chart: public QChart
{
  Q_OBJECT
public:
  Chart(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = {});
  ~Chart() = default;

  void setNewCost(const QDateTime& dt);
  float getCost();
  
  float setPoint();
  void removePoint();

  void resetView();
  void zoomInBoth();
  void zoomOutBoth();
  void scrollLeft(bool isUser = true);
  void scrollRight(bool isUser = true);
  void scrollUp(bool isUser = true);
  void scrollDown(bool isUser = true);
  
private:
  QLineSeries* series_ = nullptr;   //!< line for cost
  QScatterSeries* point_ = nullptr; //!< the moment of purchase

  QDateTimeAxis* axisX_ = nullptr;
  QValueAxis* axisY_ = nullptr;
 
  bool isUserPos_ = false; //!< true - if user defined position of graphics
  int zoomFactor_ = 0;     //!< calc zooms to limit them (for beauty)
};


#endif 
