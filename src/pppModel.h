#ifndef PPPMODEL_H
#define PPPMODEL_H

#include <math.h>
#include <newmat/newmat.h>
#include "bnctime.h"
#include "t_prn.h"
#include "satObs.h"
#include "bncutils.h"

namespace BNC_PPP {

class t_astro {
 public:
  static NEWMAT::ColumnVector Sun(double Mjd_TT);
  static NEWMAT::ColumnVector Moon(double Mjd_TT);
  static NEWMAT::Matrix rotX(double Angle);
  static NEWMAT::Matrix rotY(double Angle);
  static NEWMAT::Matrix rotZ(double Angle);

 private:
  static const double RHO_DEG;
  static const double RHO_SEC;
  static const double MJD_J2000;

  static double GMST(double Mjd_UT1);
  static NEWMAT::Matrix NutMatrix(double Mjd_TT);
  static NEWMAT::Matrix PrecMatrix (double Mjd_1, double Mjd_2);
};

class t_tides {
 public:
  t_tides() {
    _lastMjd = 0.0;
    _rSun    = 0.0;
    _rMoon   = 0.0;
  }
  ~t_tides() {}
  NEWMAT::ColumnVector displacement(const bncTime& time, const NEWMAT::ColumnVector& xyz);
 private:
  double       _lastMjd;
  NEWMAT::ColumnVector _xSun;
  NEWMAT::ColumnVector _xMoon;
  double       _rSun;
  double       _rMoon;
};

class t_windUp {
 public:
  t_windUp();
  ~t_windUp() {}
  double value(const bncTime& etime, const NEWMAT::ColumnVector& rRec, t_prn prn,
               const NEWMAT::ColumnVector& rSat);
 private:
  double lastEtime[t_prn::MAXPRN+1];
  double sumWind[t_prn::MAXPRN+1];
};

class t_tropo {
 public:
  static double delay_saast(const NEWMAT::ColumnVector& xyz, double Ele);
};

class t_iono {
 public:
  t_iono();
  ~t_iono();
  double stec(const t_vTec* vTec, double signalPropagationTime,
      const NEWMAT::ColumnVector& rSat, const bncTime& epochTime,
      const NEWMAT::ColumnVector& xyzSta);
 private:
  double vtecSingleLayerContribution(const t_vTecLayer& vTecLayer);
  void piercePoint(double layerHeight, double epoch, const double* geocSta,
      double sphEle, double sphAzi);
  double _psiPP;
  double _phiPP;
  double _lambdaPP;
  double _lonS;


};

}

#endif
