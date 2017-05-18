#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <vector>
#include <newmat/newmat.h>
#include "pppInclude.h"



namespace BNC_PPP
{

    class t_pppOptions
    {
        public:
            t_pppOptions();
            ~t_pppOptions();

            std::vector<char>              systems() const;
            const std::vector<t_lc::type>& LCs(char system) const;
            std::vector<t_lc::type>        ambLCs(char system) const;
            std::vector<t_lc::type>        codeLCs(char system) const;
            bool useSystem(char system) const {return LCs(system).size() > 0;}
            bool useOrbClkCorr() const;
            bool estTrp() const {return _aprSigTrp > 0.0 || _noiseTrp > 0.0;}
            bool xyzAprRoverSet() const
            {
                //return (_xyzAprRover[0] != 0.0 || _xyzAprRover[1] != 0.0 || _xyzAprRover[2] != 0.0);
                return (_xyzAprRover(1) != 0.0 || _xyzAprRover(2) != 0.0 || _xyzAprRover(3) != 0.0);
            }

            bool                    _realTime;
            std::string             _crdFile;
            std::string             _corrMount;
            std::string             _rinexObs;
            std::string             _rinexNav;
            std::string             _corrFile;
            double                  _corrWaitTime;
            std::string             _roverName;
            NEWMAT::ColumnVector    _xyzAprRover;
            NEWMAT::ColumnVector    _neuEccRover;
            std::string             _recNameRover;
            std::string             _antNameRover;
            std::string             _antexFileName;
            double                  _sigmaC1;
            double                  _sigmaL1;
            double                  _maxResC1;
            double                  _maxResL1;
            bool                    _eleWgtCode;
            bool                    _eleWgtPhase;
            double                  _minEle;
            int                     _minObs;
            NEWMAT::ColumnVector    _aprSigCrd;
            NEWMAT::ColumnVector    _noiseCrd;
            double                  _noiseClk;
            double                  _aprSigTrp;
            double                  _noiseTrp;
            int                     _nmeaPort;
            double                  _aprSigAmb;
            double                  _seedingTime;
            std::vector<t_lc::type> _LCsGPS;
            std::vector<t_lc::type> _LCsGLONASS;
            std::vector<t_lc::type> _LCsGalileo;
            std::vector<t_lc::type> _LCsBDS;
    };

}

#endif
