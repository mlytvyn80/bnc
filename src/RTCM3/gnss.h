// Part of BNC, a utility for retrieving decoding and
// converting GNSS data streams from NTRIP broadcasters.
//
// Copyright (C) 2015
// German Federal Agency for Cartography and Geodesy (BKG)
// http://www.bkg.bund.de
// Alberding GmbH
// http://www.alberding.eu
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

#ifndef GNSS_H
#define GNSS_H

#define LIGHTSPEED         2.99792458e8    /* m/sec */
#define GPS_FREQU_L1       1575420000.0  /* Hz */
#define GPS_FREQU_L2       1227600000.0  /* Hz */
#define GPS_FREQU_L5       1176450000.0  /* Hz */
#define GPS_WAVELENGTH_L1  (LIGHTSPEED / GPS_FREQU_L1) /* m */
#define GPS_WAVELENGTH_L2  (LIGHTSPEED / GPS_FREQU_L2) /* m */
#define GPS_WAVELENGTH_L5  (LIGHTSPEED / GPS_FREQU_L5) /* m */

#define GLO_FREQU_L1_BASE  1602000000.0  /* Hz */
#define GLO_FREQU_L2_BASE  1246000000.0  /* Hz */
#define GLO_FREQU_L1_STEP      562500.0  /* Hz */
#define GLO_FREQU_L2_STEP      437500.0  /* Hz */
#define GLO_FREQU_L1(a)      (GLO_FREQU_L1_BASE+(a)*GLO_FREQU_L1_STEP)
#define GLO_FREQU_L2(a)      (GLO_FREQU_L2_BASE+(a)*GLO_FREQU_L2_STEP)
#define GLO_WAVELENGTH_L1(a) (LIGHTSPEED / GLO_FREQU_L1(a)) /* m */
#define GLO_WAVELENGTH_L2(a) (LIGHTSPEED / GLO_FREQU_L2(a)) /* m */

#define GAL_FREQU_E1       1575420000.0  /* Hz */
#define GAL_FREQU_E5A      1176450000.0  /* Hz */
#define GAL_FREQU_E5AB     1191795000.0  /* Hz */
#define GAL_FREQU_E5B      1207140000.0  /* Hz */
#define GAL_FREQU_E6       1278750000.0  /* Hz */
#define GAL_WAVELENGTH_E1     (LIGHTSPEED / GAL_FREQU_E1) /* m */
#define GAL_WAVELENGTH_E5A    (LIGHTSPEED / GAL_FREQU_E5A) /* m */
#define GAL_WAVELENGTH_E5AB   (LIGHTSPEED / GAL_FREQU_E5AB) /* m */
#define GAL_WAVELENGTH_E5B    (LIGHTSPEED / GAL_FREQU_E5B) /* m */
#define GAL_WAVELENGTH_E6     (LIGHTSPEED / GAL_FREQU_E6) /* m */

#define QZSS_FREQU_L1       1575420000.0  /* Hz */
#define QZSS_FREQU_L2       1227600000.0  /* Hz */
#define QZSS_FREQU_L5       1176450000.0  /* Hz */
#define QZSS_FREQU_LEX      1278750000.0 /* Hz */
#define QZSS_WAVELENGTH_L1  (LIGHTSPEED / QZSS_FREQU_L1) /* m */
#define QZSS_WAVELENGTH_L2  (LIGHTSPEED / QZSS_FREQU_L2) /* m */
#define QZSS_WAVELENGTH_L5  (LIGHTSPEED / QZSS_FREQU_L5) /* m */
#define QZSS_WAVELENGTH_LEX (LIGHTSPEED / QZSS_FREQU_LEX) /* m */

#define BDS_FREQU_B1       1561098000.0  /* Hz */
#define BDS_FREQU_B2       1207140000.0  /* Hz */
#define BDS_FREQU_B3       1268520000.0  /* Hz */
#define BDS_WAVELENGTH_B1  (LIGHTSPEED / BDS_FREQU_B1) /* m */
#define BDS_WAVELENGTH_B2  (LIGHTSPEED / BDS_FREQU_B2) /* m */
#define BDS_WAVELENGTH_B3  (LIGHTSPEED / BDS_FREQU_B3) /* m */

#define R2R_PI          3.1415926535898

#define RTCM3ID_BDS 63

#endif /* GNSS_H */
