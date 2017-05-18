#include <iostream>
#include <math.h>
#include <newmat/newmat.h>

#include "bncephuploadcaster.h"
#include "bncsettings.h"
#include "RTCM3/ephEncoder.h"

using namespace std;

// Constructor
////////////////////////////////////////////////////////////////////////////
bncEphUploadCaster::bncEphUploadCaster() : bncEphUser(true) {
  bncSettings settings;

  QString mountpoint = settings.value("uploadEphMountpoint").toString();
  if (mountpoint.isEmpty()) {
    _ephUploadCaster = 0;
  }
  else {
    QString outHost  = settings.value("uploadEphHost").toString();
    int     outPort  = settings.value("uploadEphPort").toInt();
    QString password = settings.value("uploadEphPassword").toString();
    int     sampl    = settings.value("uploadEphSample").toInt();

    _ephUploadCaster = new bncUploadCaster(mountpoint, outHost, outPort,
                                           password, -1, sampl);

    connect(_ephUploadCaster, SIGNAL(newBytes(QByteArray,double)),
          this, SIGNAL(newBytes(QByteArray,double)));

    _ephUploadCaster->start();
  }
}

// Destructor
////////////////////////////////////////////////////////////////////////////
bncEphUploadCaster::~bncEphUploadCaster() {
  if (_ephUploadCaster) {
    _ephUploadCaster->deleteSafely();
  }
}

// List of Stored Ephemeris changed (virtual)
////////////////////////////////////////////////////////////////////////////
void bncEphUploadCaster::ephBufferChanged() {
  if (_ephUploadCaster) {
    QByteArray outBuffer;

    QDateTime now = currentDateAndTimeGPS();
    bncTime currentTime(now.toString(Qt::ISODate).toStdString());

    QListIterator<QString> it(prnList());
    while (it.hasNext()) {
      const t_eph* eph = ephLast(it.next());

      bncTime toc = eph->TOC();
      double timeDiff = fabs(toc - currentTime);

      const t_ephGPS*  ephGPS  = dynamic_cast<const t_ephGPS*>(eph);
      const t_ephGlo*  ephGlo  = dynamic_cast<const t_ephGlo*>(eph);
      const t_ephGal*  ephGal  = dynamic_cast<const t_ephGal*>(eph);
      const t_ephSBAS* ephSBAS = dynamic_cast<const t_ephSBAS*>(eph);
      const t_ephBDS*  ephBDS  = dynamic_cast<const t_ephBDS*>(eph);

      unsigned char Array[80];
      int size = 0;

      if (ephGPS) {
        if (timeDiff <= 4*3600) {
          size = t_ephEncoder::RTCM3(*ephGPS, Array);
        }
      }
      else if (ephGlo) {
        if (timeDiff <= 1*3600) {
          size = t_ephEncoder::RTCM3(*ephGlo, Array);
        }
      }
      else if (ephGal) {
        if (timeDiff <= 4*3600) {
          size = t_ephEncoder::RTCM3(*ephGal, Array);
        }
      }
      else if (ephSBAS) {
        if (timeDiff <= 600) {
          size = t_ephEncoder::RTCM3(*ephSBAS, Array);
        }
      }
      else if (ephBDS) {
        if (timeDiff <= 6*3600) {
          size = t_ephEncoder::RTCM3(*ephBDS, Array);
        }
      }
      if (size > 0) {
        outBuffer += QByteArray((char*) Array, size);
      }
    }
    if (outBuffer.size() > 0) {
      _ephUploadCaster->setOutBuffer(outBuffer);
    }
  }
}
