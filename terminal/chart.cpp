#include "chart.h"

#include <QAbstractAxis>
#include <QLineSeries>
#include <QScatterSeries>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QDateTime>
#include <QRandomGenerator>


Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags fl):
    QChart(QChart::ChartTypeCartesian, parent, fl),
    series_(new QLineSeries(this)),
    point_(new QScatterSeries(this)),
    axisX_(new QDateTimeAxis(this)),
    axisY_(new QValueAxis(this))
{  
  legend()->hide();
  setAnimationOptions(QChart::SeriesAnimations);
    
  addSeries(series_);
  addSeries(point_);  
  
  QDateTime dt = QDateTime::currentDateTimeUtc();
  dt.setTime(QTime(dt.time().hour(),
		   dt.time().minute(),
		   (dt.time().second()/5)*5));

  axisX_->setTickCount(13);
  axisX_->setRange(dt.addSecs(-30), dt.addSecs(30));
  axisX_->setFormat("mm:ss"); 
  addAxis(axisX_, Qt::AlignBottom);
  series_->attachAxis(axisX_);
  point_->attachAxis(axisX_);

  axisY_->setRange(0, 10);
  addAxis(axisY_, Qt::AlignLeft);
  series_->attachAxis(axisY_);
  point_->attachAxis(axisY_);
  
  series_->append(dt.toMSecsSinceEpoch(), 5);
}


void Chart::setNewCost(const QDateTime& dt)
{
  //add new point
  int64_t x = dt.toMSecsSinceEpoch();
  
  float Yprev = series_->at(series_->count() - 1).y();
  float y = Yprev + QRandomGenerator::global()->bounded(3) - 1;
  if (y < 0) {
    y = 0;
  } 
 
  series_->setColor(Qt::cyan); 
  series_->append(x, y);

  //autoscroll
  if (!isUserPos_) {
    if (x - axisX_->min().toMSecsSinceEpoch() > 35000) {
      scrollRight(false);
    }
    
    if (y < axisY_->min() + 1 && y >= 1) {
      scrollDown(false);
    } else if (y > axisY_->max() - 1) {
      scrollUp(false);
    }
  }
}

float Chart::getCost()
{
  return series_->at(series_->count() - 1).y();
}

//set user purchase point
float Chart::setPoint()
{
  setAnimationOptions(QChart::NoAnimation);
  
  auto cur = series_->at(series_->count() - 1);    
  point_->setColor(Qt::yellow);

  point_->append(cur);
  setAnimationOptions(QChart::SeriesAnimations);
  
  return cur.y();
}

//remove user purchase point
void Chart::removePoint()
{
  point_->clear();
}

//!< reset all user manipulations with view
void Chart::resetView()
{
  setAnimationOptions(QChart::NoAnimation);

  zoomReset();
  zoomFactor_ = 0; 
  
  int64_t x = series_->at(series_->count() - 1).x();
  QDateTime dt = QDateTime::fromMSecsSinceEpoch(x, Qt::UTC);
  dt.setTime(QTime(dt.time().hour(),
		   dt.time().minute(),
		   (dt.time().second()/5)*5));  
  axisX_->setRange(dt.addSecs(-30), dt.addSecs(30));

  float y = series_->at(series_->count() - 1).y();
  int rmin = qMax(0, int(y - 5));
  axisY_->setRange(rmin, rmin + 10);

  isUserPos_ = false;  
  
  setAnimationOptions(QChart::SeriesAnimations);
}


void Chart::zoomInBoth()
{
  setAnimationOptions(QChart::NoAnimation);
  if (zoomFactor_ < 2) {
    isUserPos_ = true;
    ++zoomFactor_;
    zoomIn();
  }
  setAnimationOptions(QChart::SeriesAnimations);
}

void Chart::zoomOutBoth()
{
  setAnimationOptions(QChart::NoAnimation);
  if (zoomFactor_ > -6) {
    isUserPos_ = true;
    --zoomFactor_;
    zoomOut();
  }
  setAnimationOptions(QChart::SeriesAnimations);
}

void Chart::scrollLeft(bool isUser /*= true*/)
{
  if (isUser) {
    isUserPos_ = true;
  }
  
  setAnimationOptions(QChart::NoAnimation);
  qreal sx = plotArea().width() / (axisX_->tickCount() - 1);
  scroll(-sx, 0);
  setAnimationOptions(QChart::SeriesAnimations);
}

void Chart::scrollRight(bool isUser /*= true*/)
{
  if (isUser) {
    isUserPos_ = true;
  }
  
  setAnimationOptions(QChart::NoAnimation);
  qreal sx = plotArea().width() / (axisX_->tickCount() - 1);
  scroll(sx, 0);
  setAnimationOptions(QChart::SeriesAnimations);
}

void Chart::scrollUp(bool isUser /*= true*/)
{
  if (isUser) {
    isUserPos_ = true;
  }
  
  setAnimationOptions(QChart::NoAnimation);
  qreal sy = plotArea().height() / 10;
  scroll(0, sy);
  setAnimationOptions(QChart::SeriesAnimations);
}


void Chart::scrollDown(bool isUser /*= true*/)
{
  if (isUser) {
    isUserPos_ = true;
  }
  
  setAnimationOptions(QChart::NoAnimation);
  qreal sy = plotArea().height() / 10;
  scroll(0, -sy);
  setAnimationOptions(QChart::SeriesAnimations);
}
