#ifndef BNCEPHUPLOADCASTER_H
#define BNCEPHUPLOADCASTER_H


#include "bncuploadcaster.h"
#include "bncephuser.h"

class bncEphUploadCaster : public bncEphUser {
 Q_OBJECT
 public:
  bncEphUploadCaster();
  virtual ~bncEphUploadCaster();
 signals:
  void newBytes(QByteArray staID, double nbyte);
 protected:
  virtual void ephBufferChanged();
 private:
  bncUploadCaster* _ephUploadCaster;
};

#endif
