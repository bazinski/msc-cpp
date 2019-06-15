/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. */
/* See cxx source for full Copyright notice */
/* $Id$ */

#ifndef AliAnalysisTaskMyTask_H
#define AliAnalysisTaskMyTask_H

const std::string TAB(" ");

class AliAnalysisTaskMyTask : public AliAnalysisTaskSE  
{
    public:
                                AliAnalysisTaskMyTask();
                                AliAnalysisTaskMyTask(const char *name);
        virtual                 ~AliAnalysisTaskMyTask();

        virtual void            UserCreateOutputObjects();
        virtual void            UserExec(Option_t* option);
        virtual void            Terminate(Option_t* option);

        void                    PrintTrdTrackletArray(AliESDEvent * fESD, std::string indent);
        void                    PrintTrdTracklet(AliESDTrdTracklet * fESDtracklet, std::string indent);
        void                    PrintTrdTrackArray(AliESDEvent * fESD, std::string indent);
        void                    PrintTracks(AliESDEvent * fESD);
        void                    PrintEsdTrack(AliESDtrack* track, std::string indent);

    private:
        AliESDEvent*            fESD;           //! input event
        TList*                  fOutputList;    //! output list
        TH2F*                   fHistPt;        //! dummy histogram
        TNtuple*                fTracklet;        //! dummy histogram
        ofstream*               summary;        //! summary output text file
        int                     eventCount;     //! indicator for output comma appends
        Double_t                minY, maxY;
        map<AliESDTrdTracklet *, Int_t> * mp;

        AliTRDdigitsManager* fDigMan; //! digits manager
        AliTRDgeometry* fGeo; //! TRD geometry

        TFile* OpenDigitsFile(TString inputfile, TString digfile, TString opt);

        AliAnalysisTaskMyTask(const AliAnalysisTaskMyTask&); // not implemented
        AliAnalysisTaskMyTask& operator=(const AliAnalysisTaskMyTask&); // not implemented

        ClassDef(AliAnalysisTaskMyTask, 1);
};

#endif
