/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* AliAnaysisTaskMyTask
 *
 * empty task which can serve as a starting point for building an analysis
 * as an example, one histogram is filled
 */

#include "TChain.h"
#include "TH1F.h"
#include "TList.h"
#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliESDEvent.h"
#include "AliESDTrack.h"
#include "AliESDTrdTrack.h"
#include "AliESDTrdTracklet.h"
#include "AliESDInputHandler.h"
#include "AliAnalysisTaskMyTask.h"

#include <iostream>
using namespace std;

class AliAnalysisTaskMyTask; // your analysis class

using namespace std; // std namespace: so you can do things like 'cout'

ClassImp(AliAnalysisTaskMyTask) // classimp: necessary for root

    AliAnalysisTaskMyTask::AliAnalysisTaskMyTask() : AliAnalysisTaskSE(),
                                                     fESD(0), fOutputList(0), fHistPt(0), 
                                                     summary(0), eventCount(0), minY(0), maxY(0),
                                                     fTracklet(0)
{
    // default constructor, don't allocate memory here!
    // this is used by root for IO purposes, it needs to remain empty
}
//_____________________________________________________________________________
AliAnalysisTaskMyTask::AliAnalysisTaskMyTask(const char *name) : AliAnalysisTaskSE(name),
                                                                 fESD(0), fOutputList(0), fHistPt(0), 
                                                                 summary(0), eventCount(0), minY(0), maxY(0),
                                                                 fTracklet(0)
{
    // constructor
    DefineInput(0, TChain::Class()); // define the input of the analysis: in this case we take a 'chain' of events
                                     // this chain is created by the analysis manager, so no need to worry about it,
                                     // it does its work automatically
    DefineOutput(1, TList::Class()); // define the ouptut of the analysis: in this case it's a list of histograms
                                     // you can add more output objects by calling DefineOutput(2, classname::Class())
                                     // if you add more output objects, make sure to call PostData for all of them, and to
                                     // make changes to your AddTask macro!
}
//_____________________________________________________________________________
AliAnalysisTaskMyTask::~AliAnalysisTaskMyTask()
{
    // destructor
    if (fOutputList)
    {
        delete fOutputList; // at the end of your task, it is deleted from memory by calling this function
    }

    delete summary;
}
//_____________________________________________________________________________
void AliAnalysisTaskMyTask::UserCreateOutputObjects()
{
    // create output objects
    //
    // this function is called ONCE at the start of your analysis (RUNTIME)
    // here you ceate the histograms that you want to use
    //
    fOutputList = new TList();    // this is a list which will contain all of your histograms
                                  // at the end of the analysis, the contents of this list are written
                                  // to the output file
    fOutputList->SetOwner(kTRUE); // memory stuff: the list is owner of all objects it contains and will delete them
                                  // if requested (dont worry about this now)

    fHistPt = new TH2F("fHistPt", "fHistPt", 100, -90, 90, 6, 0, 6);       // create your histogra
    fOutputList->Add(fHistPt);          // don't forget to add it to the list! the list will be written to file, so if you want
                                        // your histogram in the output file, add it to the list!

    fTracklet = new TNtuple("fTracklet", "fTracklet", "x:y:z:ly");
    fOutputList->Add(fTracklet);
                                        
    summary = new ofstream();
    summary->open("/mnt/jsroot/data.js");
    *summary << "function getData() {\n\treturn [";

    PostData(1, fOutputList); // postdata will notify the analysis manager of changes / updates to the
                              // fOutputList object. the manager will in the end take care of writing your output to file
                              // so it needs to know what's in the output
}

void AliAnalysisTaskMyTask::PrintTrdTracklet(AliESDTrdTracklet *tracklet, Int_t id)
{
    *summary << "\t\t{" << endl;
    *summary << "\t\t\t\"Id\": " << id << "," << endl;
    *summary << "\t\t\t\"Ref\": \"" << tracklet << "\"," << endl;
    *summary << "\t\t\t\"HCId\": " << tracklet->GetHCId() << "," << endl;
    *summary << "\t\t\t\"Label\": " << tracklet->GetLabel() << "," << endl;
    *summary << "\t\t\t\"TrackletWord\": " << tracklet->GetTrackletWord() << "," << endl;
    *summary << "\t\t\t\"BinDy\": " << tracklet->GetBinDy() << "," << endl;
    *summary << "\t\t\t\"BinY\": " << tracklet->GetBinY() << "," << endl;
    *summary << "\t\t\t\"BinZ\": " << tracklet->GetBinZ() << "," << endl;
    *summary << "\t\t\t\"Detector\": " << tracklet->GetDetector() << "," << endl;
    *summary << "\t\t\t\"DyDx\": " << tracklet->GetDyDx() << "," << endl;
    *summary << "\t\t\t\"LocalY\": " << tracklet->GetLocalY() << "," << endl;
    *summary << "\t\t\t\"PID\": " << tracklet->GetPID() << endl;
    *summary << "\t\t}";    
}

void AliAnalysisTaskMyTask::PrintTrdTracklets(AliESDEvent *fESD)
{
    Int_t nTRDTracklets(fESD->GetNumberOfTrdTracklets());

    *summary << "\t\"TrdTracklets\": [" << endl;
    for (Int_t idx = 0; idx < nTRDTracklets; idx++)
    {
        AliESDTrdTracklet *tracklet = fESD->GetTrdTracklet(idx);
        PrintTrdTracklet(tracklet, idx);
        *summary << (idx + 1 == nTRDTracklets ? "" : ",") << endl;
    }
    *summary << "\t]" << endl;
}

void AliAnalysisTaskMyTask::PrintTrdTracks(AliESDEvent *fESD)
{
    Int_t nTRDTracks(fESD->GetNumberOfTrdTracks());

    *summary << "\t\"TrdTracks\": [" << endl;
    for (Int_t idx = 0; idx < nTRDTracks; idx++)
    {
        AliESDTrdTrack *track = fESD->GetTrdTrack(idx);
        *summary << "\t\t{" << endl;
        //*summary << "\t\t\tGetTrackWord\": " << track->GetTrackWord() << endl;
        //*summary << "\t\t\tGetExtendedTrackWord\": " << track->GetExtendedTrackWord() << endl;
        *summary << "\t\t\t\"Id\": " << idx << "," << endl;
        *summary << "\t\t\t\"Ref\": \"" << track << "\"," << endl;
        *summary << "\t\t\t\"A\": " << track->GetA() << "," << endl;
        *summary << "\t\t\t\"B\": " << track->GetB() << "," << endl;
        *summary << "\t\t\t\"C\": " << track->GetC() << "," << endl;
        *summary << "\t\t\t\"Y\": " << track->GetY() << "," << endl;
        *summary << "\t\t\t\"LayerMask\": " << track->GetLayerMask() << "," << endl;
        *summary << "\t\t\t\"PID\": " << track->GetPID() << "," << endl;
        *summary << "\t\t\t\"Pt\": " << track->GetPt() << "," << endl;
        *summary << "\t\t\t\"Stack\": " << track->GetStack() << "," << endl;
        *summary << "\t\t\t\"Sector\": " << track->GetSector() << "," << endl;
        *summary << "\t\t\t\"Flags\": " << (Int_t)track->GetFlags() << "," << endl;
        *summary << "\t\t\t\"FlagsTiming\": " << (Int_t)track->GetFlagsTiming() << "," << endl;
        *summary << "\t\t\t\"TrackInTime\": " << track->GetTrackInTime() << "," << endl;
        *summary << "\t\t\t\"Label\": " << track->GetLabel() << "," << endl;
        
        AliVTrack *trackMatch = track->GetTrackMatch();
        if (trackMatch != nullptr)
        {
            *summary << "\t\t\t\"TrackMatch\": ";
            PrintTrack((AliESDtrack *)trackMatch);
            *summary << "," << endl;
        }

        *summary << "\t\t\t\"TrdTracklets\": [" << endl;
        bool first = true;
        for (Int_t layerIndex = 0; layerIndex < 6; layerIndex++)
        {
            AliESDTrdTracklet *tracklet = track->GetTracklet(layerIndex);
            if (tracklet != nullptr)
            {
                *summary << (first ? "" : ",") << endl;
                first = false;
                PrintTrdTracklet(track->GetTracklet(layerIndex), layerIndex);
            }
        }
        *summary << "]" << endl;
        //*summary << "\t\t\tGetTrackletIndex\": " << track->GetTrackletIndex() << endl;
        *summary << "\t\t}" << (idx + 1 == nTRDTracks ? "" : ",") << endl;
    }
    *summary << "\t]," << endl;
}

void AliAnalysisTaskMyTask::PrintTrackXYZ(Double_t * xyz) {
    *summary << "\t\t\t\t[" << xyz[0] << ", " << xyz[1] << ", " << xyz[2] << "]," << endl;
}

void AliAnalysisTaskMyTask::PrintTrack(AliESDtrack *track)
{
    *summary << "\t\t{" << endl;
    *summary << "\t\t\t\t\"Ref\": \"" << track << "\"," << endl;

    *summary << "\t\t\t\t\"Path\": [" << endl;
    
    Double_t b = track->GetESDEvent()->GetMagneticField();

    Double_t * xyz = new Double_t[3];
    for (Int_t x = 1; x <= 470; x+=10)
        if (track->GetXYZAt(x, b, xyz))
            PrintTrackXYZ(xyz);
    delete [] xyz;

    *summary << "\t\t\t\t]," << endl;

    //*summary << "\t\t\tGetTrackWord\": " << track->GetTrackWord() << endl;
    //*summary << "\t\t\tGetExtendedTrackWord\": " << track->GetExtendedTrackWord() << endl;
    //*summary << "\t\t\tGetTrackletIndex\": " << track->GetTrackletIndex() << endl;
    *summary << "\t\t\t}";
}

void AliAnalysisTaskMyTask::PrintTracks(AliESDEvent *fESD)
{
    Int_t nTRDTracks(fESD->GetNumberOfTracks());

    *summary << "\t\"TRDTracks\": [" << endl;
    for (Int_t idx = 0; idx < nTRDTracks; idx++)
    {
        AliESDTrdTrack *track = fESD->GetTrdTrack(idx);
        *summary << "\t\t{" << endl;
        //*summary << "\t\t\tGetTrackWord\": " << track->GetTrackWord() << endl;
        //*summary << "\t\t\tGetExtendedTrackWord\": " << track->GetExtendedTrackWord() << endl;
        *summary << "\t\t\t\"Id\": " << idx << "," << endl;
        *summary << "\t\t\t\"A\": " << track->GetA() << "," << endl;
        *summary << "\t\t\t\"B\": " << track->GetB() << "," << endl;
        *summary << "\t\t\t\"C\": " << track->GetC() << "," << endl;
        *summary << "\t\t\t\"Y\": " << track->GetY() << "," << endl;
        *summary << "\t\t\t\"LayerMask\": " << track->GetLayerMask() << "," << endl;
        *summary << "\t\t\t\"PID\": " << track->GetPID() << "," << endl;
        *summary << "\t\t\t\"Pt\": " << track->GetPt() << "," << endl;
        *summary << "\t\t\t\"Stack\": " << track->GetStack() << "," << endl;
        *summary << "\t\t\t\"Sector\": " << track->GetSector() << "," << endl;
        *summary << "\t\t\t\"Flags\": " << (Int_t)track->GetFlags() << "," << endl;
        *summary << "\t\t\t\"FlagsTiming\": " << (Int_t)track->GetFlagsTiming() << "," << endl;
        *summary << "\t\t\t\"TrackInTime\": " << track->GetTrackInTime() << "," << endl;
        *summary << "\t\t\t\"Label\": " << track->GetLabel() << endl;
        //*summary << "\t\t\tGetTrackletIndex\": " << track->GetTrackletIndex() << endl;
        *summary << "\t\t}" << (idx + 1 == nTRDTracks ? "" : ",") << endl;
    }
    *summary << "\t]," << endl;
}
//_____________________________________________________________________________
void AliAnalysisTaskMyTask::UserExec(Option_t *)
{
    fESD = dynamic_cast<AliESDEvent *>(InputEvent());
    const AliESDVertex * primaryVertex = fESD->GetPrimaryVertex();
    
    if (!fESD)
        return; // if the pointer to the event is empty (getting it failed) skip this event

    Int_t nTracks(fESD->GetNumberOfTracks()); // see how many tracks there are in the event
    Int_t nTRDTracklets(fESD->GetNumberOfTrdTracklets());

    if (fESD->GetNumberOfTrdTracks() > 0)
    {
        //cout << primaryVertex->GetX() << endl;
        if (eventCount++ > 5)
        {
            Int_t nTRDTracklets(fESD->GetNumberOfTrdTracklets());

            for (Int_t idx = 0; idx < nTRDTracklets; idx++)
            {    
                AliESDTrdTracklet *tracklet = fESD->GetTrdTracklet(idx);
                Double_t localY = tracklet->GetLocalY();
                Int_t layer = tracklet->GetDetector() % 6;
                fHistPt->Fill(localY, layer);
                
                Int_t biny = tracklet->GetBinY();
                Int_t binz = tracklet->GetBinZ();
                fTracklet->Fill(layer, biny, binz, localY);

                minY = min(minY, localY);
                maxY = max(maxY, localY);
            }
            //return;
        }
        else {
            if (eventCount > 1) {
                *summary << "," << endl;
            }

            // *summary << "{ \"x\": " << primaryVertex->GetX() << "}";            
            // return;

            *summary << "{"
                    << "\n\t\"Event\": " << fESD->GetEventNumberInFile() << ","
                    << "\n\t\"Ref\": \"" << eventCount << "\","
                    << "\n\t\"nTracks\": " << fESD->GetNumberOfTracks() << ","
                    << "\n\t\"nTrdTracks\": " << fESD->GetNumberOfTrdTracks() << ","
                    << "\n\t\"nTrdTracklets\": " << fESD->GetNumberOfTrdTracklets() << "," << endl;

            this->PrintTrdTracks(fESD);
            this->PrintTrdTracklets(fESD);

            *summary << "}";            
        }
    }

    PostData(1, fOutputList); // stream the results the analysis of this event to
                              // the output manager which will take care of writing
                              // it to a file
}
//_____________________________________________________________________________
void AliAnalysisTaskMyTask::Terminate(Option_t *)
{
    // terminate
    // called at the END of the analysis (when all events are processed)
    *summary << "];\n}" << endl;
    summary->close();

    cout << endl << endl << "Range: " << minY << " " << maxY << endl;
}
//_____________________________________________________________________________
