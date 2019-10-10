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
        Bool_t                  UserNotify();
        virtual void            UserExec(Option_t* option);
        virtual void            Terminate(Option_t* option);

        void                    PrintTrdTrackletArray(AliESDEvent * fESD, std::string indent);
        void                    PrintTrdTracklet(AliESDTrdTracklet * fESDtracklet, std::string indent);
        void                    PrintTrdTrackArray(AliESDEvent * fESD, std::string indent);
        void                    PrintTracks(AliESDEvent * fESD);
        void                    PrintEsdTrack(AliESDtrack* track, std::string indent);

        const char*             fOutputPath;                //! Path to folder on disk where output will be generated
        const char*             fOutputRelativeFolder;      //! Relative path that will be referenced in AJAX requests
        const char*             fOutputName;                //! Output name that will be used for the .js file and .json output folder
        TString                 fDigitsInputFileName;       //! Name of digits file for reading
        Bool_t                  fExportEsdTracks;           // Flag determining whether ESD tracks should also be exported, rather than just TRD tracks
        
    private:
        Bool_t                  ReadDigits();
  
        AliESDEvent*            fESD;           //! input event
        TList*                  fOutputList;    //! output list
        TH2F*                   fHistPt;        //! dummy histogram
        TNtuple*                fTracklet;      //! dummy histogram
        ofstream*               summary;        //! summary output text file
        int                     eventCount;     //! indicator for output comma appends
        Double_t                minY, maxY;
        Bool_t                  usedDetectors[540];
        
        map<AliESDTrdTracklet *, Int_t> * mp;
        map<AliESDTrdTracklet *, TString> * fTrackletMap;

        AliTRDdigitsManager* fDigMan; //! digits manager
        AliTRDgeometry* fGeo; //! TRD geometry

        TFile* OpenDigitsFile(TString inputfile, TString digfile, TString opt);

        TFile* fDigitsInputFile;             //! Digits file for reading

        Int_t fEventNoInFile;

        AliAnalysisTaskMyTask(const AliAnalysisTaskMyTask&); // not implemented
        AliAnalysisTaskMyTask& operator=(const AliAnalysisTaskMyTask&); // not implemented

        ClassDef(AliAnalysisTaskMyTask, 1);
};

#endif
