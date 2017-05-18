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

#ifndef BNCRINEX_H
#define BNCRINEX_H

#include <QtCore>
#include <fstream>

#include "bncconst.h"
#include "satObs.h"
#include "rinex/rnxobsfile.h"

class bncRinex {
 public:
   bncRinex(const QByteArray& statID, const QUrl& mountPoint, 
            const QByteArray& latitude, const QByteArray& longitude,
            const QByteArray& nmea, const QByteArray& ntripVersion); 
   ~bncRinex();

   int  samplingRate() const {return _samplingRate;}
   void deepCopy(t_satObs obs);
   void dumpEpoch(const QByteArray& format, const bncTime& maxTime);
   void setReconnectFlag(bool flag){_reconnectFlag = flag;}

   static QString nextEpochStr(const QDateTime& datTim,
                               const QString& intStr, 
                               bool rnxV3filenames,
                               QDateTime* nextEpoch = 0);
   static std::string asciiSatLine(const t_satObs& obs);

 private:
   void resolveFileName(const QDateTime& datTim);
   bool readSkeleton();
   void writeHeader(const QByteArray& format, const bncTime& firstObsTime);
   void closeFile();
   t_irc downloadSkeleton();

   QByteArray      _statID;
   QByteArray      _fName;
   QList<t_satObs> _obs;
   std::ofstream   _out;
   bool            _headerWritten;
   QDateTime       _nextCloseEpoch;
   QString         _rnxScriptName;
   QUrl            _mountPoint;
   QString         _pgmName;
   QString         _userName;
   QString         _sklName;
   bool            _writeRinexFileOnlyWithSkl;
   bool            _rnxV3filenames;
   QByteArray      _latitude;
   QByteArray      _longitude;
   QByteArray      _nmea;
   QByteArray      _ntripVersion;
   bool            _reconnectFlag;
   QDate           _skeletonDate;
   int             _samplingRate;
   QStringList     _addComments;

   QMap<QString, int>  _slip_cnt_L1;
   QMap<QString, int>  _slip_cnt_L2;
   QMap<QString, int>  _slip_cnt_L5;

   t_rnxObsHeader _sklHeader;
   t_rnxObsHeader _header;
};

#endif
