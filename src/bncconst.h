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

#include <string>

#ifndef BNCCONST_H
#define BNCCONST_H

enum t_irc {failure = -1, success, fatal}; // return code

class t_frequency {
 public:
  enum type {dummy = 0, G1, G2, G5, R1, R2, 
                        E1, // E1  / 1575.42          
                        E5, // E5a / 1176.45          
                        E7, // E5b / 1207.140         
                        E8, // E5(E5a+E5b) / 1191.795 
                        E6, // E6  / 1278.75
                        J1, // 1575.42
                        J2, // 1227.60
                        J5, // 1176.45
                        J6, // Lex(6) 1278.75
                        S1, // 1575.42
                        S5, // 1176.45
                        C2, // 1561.098
                        C7, // 1207.14
                        C6, // 1268.52
             max};

  static std::string toString(type tt) {
    if      (tt == G1) return "G1";
    else if (tt == G2) return "G2";
    else if (tt == G5) return "G5";
    else if (tt == R1) return "R1";
    else if (tt == R2) return "R2";
    else if (tt == E1) return "E1";
    else if (tt == E5) return "E5";
    else if (tt == E6) return "E6";
    else if (tt == E7) return "E7";
    else if (tt == E8) return "E8";
    else if (tt == J1) return "J1";
    else if (tt == J2) return "J2";
    else if (tt == J5) return "J5";
    else if (tt == J6) return "J6";
    else if (tt == S1) return "S1";
    else if (tt == S5) return "S5";
    else if (tt == C2) return "C2";
    else if (tt == C7) return "C7";
    else if (tt == C6) return "C6";
    return std::string();
  }
  static enum type toInt(std::string s) {
    if      (s == "G1") return G1;
    else if (s == "G2") return G2;
    else if (s == "G5") return G5;
    else if (s == "R1") return R1;
    else if (s == "R2") return R2;
    else if (s == "E1") return E1;
    else if (s == "E5") return E5;
    else if (s == "E6") return E6;
    else if (s == "E7") return E7;
    else if (s == "E8") return E8;
    else if (s == "J1") return J1;
    else if (s == "J2") return J2;
    else if (s == "J5") return J5;
    else if (s == "J6") return J6;
    else if (s == "S1") return S1;
    else if (s == "S5") return S5;
    else if (s == "C2") return C2;
    else if (s == "C7") return C7;
    else if (s == "C6") return C6;
    return type();
  }
};

class t_CST {
 public:
  static double freq(t_frequency::type fType, int slotNum);
  static double lambda(t_frequency::type fType, int slotNum);

  static const double c;
  static const double omega;
  static const double aell;
  static const double fInv;
  static const double rgeoc;
};


#endif
