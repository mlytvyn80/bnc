#ifndef BNCCLOCKRINEX_H
#define BNCCLOCKRINEX_H

#include <fstream>

#include <QDateTime>
#include <QString>

#include "bncoutf.h"

class bncClockRinex : public bncoutf {
 public:
  bncClockRinex(const QString& sklFileName, const QString& intr, int sampl);
  virtual ~bncClockRinex();
  virtual t_irc write(int GPSweek, double GPSweeks, const QString& prn, 
                      double sp3Clk);

 private:
  virtual void writeHeader(const QDateTime& datTim);
  bool _append;
};

#endif
