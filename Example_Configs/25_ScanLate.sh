#!/bin/bash

../bnc -nw -conf /dev/null \
       -key mountPoints "//Example:Configs@mgex.igs-ip.net:2101/CUT07 RTCM_3.2 AUS -32.00 115.89 no 1" \
       -key miscMount CUT07 \
       -key miscScanRTCM 2 \
       -key miscIntr "10 sec" \
       -key logFile Output/SCAN.log &

psID=`echo $!`
sleep 20
kill $psID

