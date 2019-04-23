TObject * result = nullptr;
TFile * f0, * f1;
TList * f2;
TNtuple * nt = nullptr;
TH2 * th;

void test() {
    f0 = new TFile("AnalysisResults.root");
    cout << "File loaded" << endl;
    f1 = (TFile *)f0->Get("MyTask");
    cout << "MyTask loaded" << endl;
    f2 = (TList *)f1->Get("MyOutputContainer");
    cout << "MyOutputContainer loaded: " << f2 << endl;
    //result = (TObject *)f2->GetObjectChecked("fTracklet", "TNtuple");
    nt = (TNtuple *)f2->At(1);

    nt->SetLineColor(kRed);
    nt->Draw("ly", "x==5");
    nt->SetLineColor(kGreen);
    nt->Draw("ly", "x==3", "same");
    nt->SetLineColor(kBlue);
    nt->Draw("ly", "x==0", "same");

    
    //th = new TH2F("th", "th", 8, -1, 7, 200, -90, 90);
    //nt->Project("th", "y:x");
    
//     TIter next(f2);
//     TKey *key;
//     Int_t c = 0;
//     while ((key = (TKey*)next()) && c++ < 5) {
//         // if (key->IsFolder()) {
//         //     dir->cd(key->GetName());
//         //     TDirectory *subdir = gDirectory;
//         //     readdir(subdir);
//         //     dirsav->cd();
//         //     continue;
//         // }
//         // result = key->ReadObj();
//         cout << key->GetName() << " " << key->ClassName() << endl;
//    }
}