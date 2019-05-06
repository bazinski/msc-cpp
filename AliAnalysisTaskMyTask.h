/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. */
/* See cxx source for full Copyright notice */
/* $Id$ */

#ifndef AliAnalysisTaskMyTask_H
#define AliAnalysisTaskMyTask_H

#include "AliAnalysisTaskSE.h"
#include <iostream>
#include <fstream>
using namespace std;
#include "TH3.h"
#include "TTree.h"
#include "TNtuple.h"

class AliAnalysisTaskMyTask : public AliAnalysisTaskSE  
{
    public:
                                AliAnalysisTaskMyTask();
                                AliAnalysisTaskMyTask(const char *name);
        virtual                 ~AliAnalysisTaskMyTask();

        virtual void            UserCreateOutputObjects();
        virtual void            UserExec(Option_t* option);
        virtual void            Terminate(Option_t* option);

        void                    PrintTrdTracklets(AliESDEvent * fESD);
        void                    PrintTrdTracklet(AliESDTrdTracklet * fESDtracklet, Int_t id);
        void                    PrintTrdTracks(AliESDEvent * fESD);
        void                    PrintTracks(AliESDEvent * fESD);
        void                    PrintTrack(AliESDtrack* track);
        void                    PrintTrackXYZ(Double_t * xyz);

    private:
        AliESDEvent*            fESD;           //! input event
        TList*                  fOutputList;    //! output list
        TH2F*                   fHistPt;        //! dummy histogram
        TNtuple*                fTracklet;        //! dummy histogram
        ofstream*               summary;        //! summary output text file
        int                     eventCount;     //! indicator for output comma appends
        Double_t                minY, maxY;
        map<AliESDTrdTracklet *, Int_t> * mp;

        AliAnalysisTaskMyTask(const AliAnalysisTaskMyTask&); // not implemented
        AliAnalysisTaskMyTask& operator=(const AliAnalysisTaskMyTask&); // not implemented

        ClassDef(AliAnalysisTaskMyTask, 1);
};

#endif
