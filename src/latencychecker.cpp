// Part of BNC, a utility for retrieving decoding and
// converting GNSS data streams from NTRIP broadcasters.
//
// Copyright (C) 2007
// German Federal Agency for Cartography and Geodesy (BKG)
// http://www.bkg.bund.de
// Czech Technical University Prague, Department of Geodesy
// http://www.fsv.cvut.cz
//
// Email: euref-ip@bkg.bund.de
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/* -------------------------------------------------------------------------
 * BKG NTRIP Client
 * -------------------------------------------------------------------------
 *
 * Class:      latencyChecker
 *
 * Purpose:    Check incoming GNSS data for latencies, gaps etc.
 *
 * Author:     G. Weber
 *
 * Created:    02-Feb-2009
 *
 * Changes:    
 *
 * -----------------------------------------------------------------------*/

#include <iostream>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "latencychecker.h"
#include "bnccore.h"
#include "bncutils.h"
#include "bncsettings.h"

using namespace std;

// Constructor
//////////////////////////////////////////////////////////////////////////////
latencyChecker::latencyChecker(QByteArray staID) {

  _staID = staID;

  connect(this, SIGNAL(newMessage(QByteArray,bool)), 
          BNC_CORE, SLOT(slotMessage(const QByteArray,bool)));

  bncSettings settings;

  // Notice threshold
  // ----------------
  QString adviseObsRate = settings.value("adviseObsRate").toString();
  _inspSegm = 0;
  if      ( adviseObsRate.isEmpty() ) { 
    _inspSegm = 0; 
  }
  else if ( adviseObsRate.indexOf("5 Hz")   != -1 ) { 
    _inspSegm = 20; 
  }
  else if ( adviseObsRate.indexOf("1 Hz")   != -1 ) { 
    _inspSegm = 10; 
  }
  else if ( adviseObsRate.indexOf("0.5 Hz") != -1 ) { 
    _inspSegm = 20; 
  }
  else if ( adviseObsRate.indexOf("0.2 Hz") != -1 ) { 
    _inspSegm = 40; 
  }
  else if ( adviseObsRate.indexOf("0.1 Hz") != -1 ) { 
    _inspSegm = 50; 
  }
  _adviseFail = settings.value("adviseFail").toInt();
  _adviseReco = settings.value("adviseReco").toInt();
  _adviseScript = settings.value("adviseScript").toString();
  expandEnvVar(_adviseScript);

  // Latency interval/average
  // ------------------------
  _miscIntr = 1;
  QString miscIntr = settings.value("miscIntr").toString();
  if      ( miscIntr.isEmpty() ) { 
    _miscIntr = 1; 
  }
  else if ( miscIntr.indexOf("2 sec")   != -1 ) { 
    _miscIntr = 2; 
  }
  else if ( miscIntr.indexOf("10 sec")  != -1 ) { 
    _miscIntr = 10; 
  }
  else if ( miscIntr.indexOf("1 min")   != -1 ) { 
    _miscIntr = 60; 
  }
  else if ( miscIntr.left(5).indexOf("5 min")   != -1 ) { 
    _miscIntr = 300; 
  }
  else if ( miscIntr.indexOf("15 min")  != -1 ) { 
    _miscIntr = 900; 
  }
  else if ( miscIntr.indexOf("1 hour")  != -1 ) { 
    _miscIntr = 3600; 
  }
  else if ( miscIntr.indexOf("6 hours") != -1 ) { 
    _miscIntr = 21600; 
  }
  else if ( miscIntr.indexOf("1 day")   != -1 ) { 
    _miscIntr = 86400; 
  }

  // RTCM message types
  // ------------------
  _checkMountPoint = settings.value("miscMount").toString();

  // Initialize private members
  // --------------------------
  _maxDt      = 1000.0;
  _wrongEpoch = false;
  _checkSeg   = false;
  _numSucc    = 0;
  _secSucc    = 0;
  _secFail    = 0;
  _initPause  = 0;
  _currPause  = 0;
  _followSec  = false;
  _oldSecGPS  = 0;
  _newSecGPS  = 0;
  _numGaps    = 0;
  _diffSecGPS = 0;
  _numLat     = 0;
  _sumLat     = 0.0;
  _sumLatQ    = 0.0;
  _meanDiff   = 0.0;
  _minLat     =  _maxDt;
  _maxLat     = -_maxDt;
  _curLat     = 0.0;

  _checkTime = QDateTime::currentDateTime();
  _decodeSucc = QDateTime::currentDateTime();

  _decodeStop = QDateTime::currentDateTime();
  _begDateTimeOut = QDateTime::currentDateTime();
  _endDateTimeOut = QDateTime::currentDateTime();
  _fromReconnect = false;

  _decodeStopCorr = QDateTime::currentDateTime();
  _begDateTimeCorr = QDateTime::currentDateTime();
  _endDateTimeCorr = QDateTime::currentDateTime();
  
}

// Destructor
//////////////////////////////////////////////////////////////////////////////
latencyChecker::~latencyChecker() {
}

// Perform 'Begin outage' check
//////////////////////////////////////////////////////////////////////////////
void latencyChecker::checkReconnect() {

  if (_inspSegm == 0) { return;}

  // Begin outage threshold
  // ----------------------
  if (!_fromReconnect) {
    _endDateTimeOut = QDateTime::currentDateTime();
  } 
  _fromReconnect = true;

  if ( _decodeStop.isValid() ) {
    _begDateTimeOut = QDateTime::currentDateTime();
    if ( _endDateTimeOut.secsTo(QDateTime::currentDateTime()) >  _adviseFail * 60 ) {
      _begDateOut = _endDateTimeOut.toUTC().date().toString("yy-MM-dd");
      _begTimeOut = _endDateTimeOut.toUTC().time().toString("hh:mm:ss");
      emit(newMessage((_staID + ": Failure threshold exceeded, outage since "
                    + _begDateOut + " " + _begTimeOut + " UTC").toLatin1(), true));
      callScript(("Begin_Outage "
                    + _begDateOut + " " + _begTimeOut + " UTC").toLatin1());
      _decodeStop.setDate(QDate());
      _decodeStop.setTime(QTime());
      _decodeStart = QDateTime::currentDateTime();
    }
  }
}

// Perform Corrupt and 'End outage' check
//////////////////////////////////////////////////////////////////////////////
void latencyChecker::checkOutage(bool decoded) {

  if (_inspSegm == 0) { return;}

  if (decoded) { _numSucc += 1; }

  if (!_checkPause.isValid() || _checkPause.secsTo(QDateTime::currentDateTime()) >= _currPause )  {
    if (!_checkSeg) {
      if ( _checkTime.secsTo(QDateTime::currentDateTime()) > _inspSegm ) {
        _checkSeg = true;
      }
    }

    // Check - once per inspect segment
    // --------------------------------
    if (_checkSeg) {

      _checkTime = QDateTime::currentDateTime();

      if (_numSucc > 0) {
        _secSucc += _inspSegm;
        _secFail = 0;
        _decodeSucc = QDateTime::currentDateTime();
        if (_secSucc > _adviseReco * 60) {
          _secSucc = _adviseReco * 60 + 1;
        }
        _numSucc = 0;
        _currPause = _initPause;
        _checkPause.setDate(QDate());
        _checkPause.setTime(QTime());
      }
      else {
        _secFail += _inspSegm;
        _secSucc = 0;
        if (_secFail > _adviseFail * 60) {
          _secFail = _adviseFail * 60 + 1;
        }
        if (!_checkPause.isValid()) {
          _checkPause = QDateTime::currentDateTime();
        }
        else {
          _checkPause.setDate(QDate());
          _checkPause.setTime(QTime());
          _secFail = _secFail + _currPause - _inspSegm;
          _currPause = _currPause * 2;
          if (_currPause > 960) {
            _currPause = 960;
          }
        }
      }

      // Begin corrupt threshold
      // -----------------------
      if (_secSucc > 0) {
        _endDateTimeCorr = QDateTime::currentDateTime();
      }

      if (_secFail > 0) {
        _begDateTimeCorr = QDateTime::currentDateTime();
      }

      if ( _decodeStopCorr.isValid() ) {
        _begDateTimeCorr = QDateTime::currentDateTime();
        if ( _endDateTimeCorr.secsTo(QDateTime::currentDateTime()) > _adviseFail * 60 ) {
          _begDateCorr = _endDateTimeCorr.toUTC().date().toString("yy-MM-dd");
          _begTimeCorr = _endDateTimeCorr.toUTC().time().toString("hh:mm:ss");
          emit(newMessage((_staID + ": Failure threshold exceeded, corrupted since "
                    + _begDateCorr + " " + _begTimeCorr + " UTC").toLatin1(), true));
          callScript(("Begin_Corrupted "
                    + _begDateCorr + " " + _begTimeCorr + " UTC").toLatin1());
          _secSucc = 0;
          _numSucc = 0;
          _decodeStopCorr.setDate(QDate());
          _decodeStopCorr.setTime(QTime());
          _decodeStartCorr = QDateTime::currentDateTime();
        }
      } 
      else {

        // End corrupt threshold
        // ---------------------
        if ( _decodeStartCorr.isValid() ) {
          _endDateTimeCorr = QDateTime::currentDateTime();
          if ( _begDateTimeCorr.secsTo(QDateTime::currentDateTime()) > _adviseReco * 60 ) {
            _endDateCorr = _begDateTimeCorr.toUTC().date().toString("yy-MM-dd");
            _endTimeCorr = _begDateTimeCorr.toUTC().time().toString("hh:mm:ss");
            emit(newMessage((_staID + ": Recovery threshold exceeded, corruption ended "
                        + _endDateCorr + " " + _endTimeCorr + " UTC").toLatin1(), true));
            callScript(("End_Corrupted "
                        + _endDateCorr + " " + _endTimeCorr + " UTC Begin was "
                        + _begDateCorr + " " + _begTimeCorr + " UTC").toLatin1());
            _decodeStartCorr.setDate(QDate());
            _decodeStartCorr.setTime(QTime());
            _decodeStopCorr = QDateTime::currentDateTime();
            _secFail = 0;
          }
        }
      }
      _checkSeg = false;
    }
  }

  // End outage threshold
  // --------------------
  if (_fromReconnect) {
    _begDateTimeOut = QDateTime::currentDateTime();
  } 
  _fromReconnect = false;

  if ( _decodeStart.isValid() ) {
    _endDateTimeOut = QDateTime::currentDateTime();
    if ( _begDateTimeOut.secsTo(QDateTime::currentDateTime()) >  _adviseReco * 60 ) {
      _endDateOut = _begDateTimeOut.toUTC().date().toString("yy-MM-dd");
      _endTimeOut = _begDateTimeOut.toUTC().time().toString("hh:mm:ss");
      emit(newMessage((_staID + ": Recovery threshold exceeded, outage ended "
                    + _endDateOut + " " + _endTimeOut + " UTC").toLatin1(), true));
      callScript(("End_Outage "
                    + _endDateOut + " " + _endTimeOut + " UTC Begin was "
                    + _begDateOut + " " + _begTimeOut + " UTC").toLatin1());
      _decodeStart.setDate(QDate());
      _decodeStart.setTime(QTime());
      _decodeStop = QDateTime::currentDateTime();
    }
  }
}

// Perform latency checks (observations)
//////////////////////////////////////////////////////////////////////////////
void latencyChecker::checkObsLatency(const QList<t_satObs>& obsList) {

  if (_miscIntr > 0 ) {

    QListIterator<t_satObs> it(obsList);
    while (it.hasNext()) {
      const t_satObs& obs = it.next();
      bool wrongObservationEpoch = checkForWrongObsEpoch(obs._time);
      _newSecGPS = static_cast<int>(obs._time.gpssec());
      if (_newSecGPS != _oldSecGPS && !wrongObservationEpoch) {
        if (_newSecGPS % _miscIntr < _oldSecGPS % _miscIntr) {
          if (_numLat > 0) {
            if (_meanDiff > 0.0) {
              if ( _checkMountPoint == _staID || _checkMountPoint == "ALL" ) {
                emit( newMessage(QString("%1: Mean latency %2 sec, min %3, max %4, rms %5, %6 epochs, %7 gaps")
                  .arg(_staID.data())
                  .arg(int(_sumLat/_numLat*100)/100.)
                  .arg(int(_minLat*100)/100.)
                  .arg(int(_maxLat*100)/100.)
                  .arg(int((sqrt((_sumLatQ - _sumLat * _sumLat / _numLat)/_numLat))*100)/100.)
                  .arg(_numLat)
                  .arg(_numGaps)
                  .toLatin1(), true) );
              }
            } else {
              if ( _checkMountPoint == _staID || _checkMountPoint == "ALL" ) {
                emit( newMessage(QString("%1: Mean latency %2 sec, min %3, max %4, rms %5, %6 epochs")
                  .arg(_staID.data())
                  .arg(int(_sumLat/_numLat*100)/100.)
                  .arg(int(_minLat*100)/100.)
                  .arg(int(_maxLat*100)/100.)
                  .arg(int((sqrt((_sumLatQ - _sumLat * _sumLat / _numLat)/_numLat))*100)/100.)
                  .arg(_numLat)
                  .toLatin1(), true) );
              }
            }
          }
          _meanDiff  = _diffSecGPS / _numLat;
          _diffSecGPS = 0;
          _numGaps    = 0;
          _sumLat     = 0.0;
          _sumLatQ    = 0.0;
          _numLat     = 0;
          _minLat     = _maxDt;
          _maxLat     = -_maxDt;
        }
        if (_followSec) {
          _diffSecGPS += _newSecGPS - _oldSecGPS;
          if (_meanDiff>0.) {
            if (_newSecGPS - _oldSecGPS > 1.5 * _meanDiff) {
              _numGaps += 1;
            }
          }
        }

        // Compute the observations latency
        // --------------------------------
        int      week;
        double   sec;
        currentGPSWeeks(week, sec);
        const double secPerWeek = 7.0 * 24.0 * 3600.0;
        if (week < int(obs._time.gpsw())) {
          week += 1;
          sec  -= secPerWeek;
        }
        if (week > int(obs._time.gpsw())) {
          week -= 1;
          sec  += secPerWeek;
        }
         _curLat   = sec - obs._time.gpssec();
        _sumLat  += _curLat;
        _sumLatQ += _curLat * _curLat;
        if (_curLat < _minLat) {
          _minLat = _curLat;
        }
        if (_curLat >= _maxLat) {
          _maxLat = _curLat;
        }
        _numLat += 1;
        _oldSecGPS = _newSecGPS;
        _followSec = true;
      }
    }
  }
}

// Perform latency checks (corrections)
//////////////////////////////////////////////////////////////////////////////
void latencyChecker::checkCorrLatency(int corrGPSEpochTime) {

  if (corrGPSEpochTime < 0) {
    return;
  }

  if (_miscIntr > 0) {

    _newSecGPS = corrGPSEpochTime;

    int week;
    double sec;
    currentGPSWeeks(week, sec);
    double dt = fabs(sec - _newSecGPS);
    const double secPerWeek = 7.0 * 24.0 * 3600.0;
    if (dt > 0.5 * secPerWeek) {
      if (sec > _newSecGPS) {
        sec  -= secPerWeek;
      } else {
        sec  += secPerWeek;
      }
    }
    if (_newSecGPS != _oldSecGPS) {
      if (int(_newSecGPS) % _miscIntr < int(_oldSecGPS) % _miscIntr) {
        if (_numLat>0) {
          QString late;
          if (_meanDiff>0.) {
            late = QString(": Mean latency %1 sec, min %2, max %3, rms %4, %5 epochs, %6 gaps")
            .arg(int(_sumLat/_numLat*100)/100.)
            .arg(int(_minLat*100)/100.)
            .arg(int(_maxLat*100)/100.)
            .arg(int((sqrt((_sumLatQ - _sumLat * _sumLat / _numLat)/_numLat))*100)/100.)
            .arg(_numLat)
            .arg(_numGaps);
            if ( _checkMountPoint == _staID || _checkMountPoint == "ALL" ) {
              emit(newMessage(QString(_staID + late ).toLatin1(), true) );
            }
          } 
          else {
            late = QString(": Mean latency %1 sec, min %2, max %3, rms %4, %5 epochs")
            .arg(int(_sumLat/_numLat*100)/100.)
            .arg(int(_minLat*100)/100.)
            .arg(int(_maxLat*100)/100.)
            .arg(int((sqrt((_sumLatQ - _sumLat * _sumLat / _numLat)/_numLat))*100)/100.)
            .arg(_numLat);
            if ( _checkMountPoint == _staID || _checkMountPoint == "ALL" ) {
            emit(newMessage(QString(_staID + late ).toLatin1(), true) );
            }
          }
        }
        _meanDiff = int(_diffSecGPS)/_numLat;
        _diffSecGPS = 0;
        _numGaps    = 0;
        _sumLat     = 0.0;
        _sumLatQ    = 0.0;
        _numLat     = 0;
        _minLat     = 1000.;
        _maxLat     = -1000.;
      }
      if (_followSec) {
        _diffSecGPS += _newSecGPS - _oldSecGPS;
        if (_meanDiff>0.) {
          if (_newSecGPS - _oldSecGPS > 1.5 * _meanDiff) {
            _numGaps += 1;
          }
        }
      }
      _curLat   = sec - _newSecGPS;
      _sumLat  += _curLat;
      _sumLatQ += _curLat * _curLat;
      if (_curLat < _minLat) {
        _minLat = _curLat;
      }
      if (_curLat >= _maxLat) {
        _maxLat = _curLat;
      }
      _numLat += 1;
      _oldSecGPS = _newSecGPS;
      _followSec = true;
    }
  }
}

// Call advisory notice script    
////////////////////////////////////////////////////////////////////////////
void latencyChecker::callScript(const char* comment) {
  if (!_adviseScript.isEmpty()) {
#ifdef WIN32
    Sleep(1);
    QProcess::startDetached(_adviseScript, QStringList() << _staID << comment) ;
#else
    sleep(1);
    QProcess::startDetached("nohup", QStringList() << _adviseScript << _staID << comment) ;
#endif
  }
}
