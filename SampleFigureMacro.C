{
//##########################################################################
//This macro demonstrates some common ways to format 1D and 2D histograms,
//add legends, titles, etc., change line colors, set axis ranges, save
//the histograms as figures among other things. 
//   There are other options, like for changing to dashed, dotted, etc.
//line styles, using points or various symbols rather than lines for the
//histograms, plotting with error bars, changing label text sizes, changing
//locations of legends on the figure, etc. in the documentation for root
//histograms, probably the tutorials at root.cern.ch would be a good
//starting point for other options.
//   NOTE: The figures are saved in the Figures/ subdirectory.
//########################################################################## 

//You can add flags to turn options on and off later.
//Sometimes this is and convenient.
//This flag will draw 2D Pt Vs Minv figures for the 1Kstar channel
//and format them, if set to true.
Bool_t AllTheHistos = kFALSE;

//Open the output file.
TFile f("AnalysisResults_2.root");
Printf("File Opened:");
f.ls();

//Print the output containers of the TDirectory (EtaCUpc) in the output file.
//Then change into that directory.
EtaCUpc->ls();
EtaCUpc->cd();

//Print the Output Container contents for ListHistKstar.
HistogramList->Print();

TCanvas my_canvas("mycanvas","canvas title");
//Get the histos
//TH1D *myEvents = (TH1D*)HistogramList->FindObject("Events Histogram");
//TH2D *myTPCsignal = (TH2D*)HistogramList->FindObject("PID cuts")->FindObject("fPionTPCvsKaonTPCLowP");
TH2D *myPtVsMinvEtaC = (TH2D*)HistogramList->FindObject("Pt V Minv Channel Match");
TH2D *myPtVsMinvEtaCTight = (TH2D*)HistogramList->FindObject("Pt V Minv Channel Match Tight");
TH1D *myMinvEtaCNotTight = (TH1D*)HistogramList->FindObject("Minv Channel Match, Low Pt");
TH1D *myMinvEtaC = (TH1D*)HistogramList->FindObject("Minv Channel Match, Low Pt Tight");
TH1D *myMinvEtaCadd = new TH1D("myMinvEtaCadd", "Deviation from Background",2400,0,6);
TH1D *myMinvEtaCcontent = new TH1D("myMinvEtaCadd", "minv_data",2400,0,6);
TH1D *myMinvEtaCerror = new TH1D("myMinvEtaCadd", "minv_fit",2400,0,6);
TH2D *myTPCsignal = (TH2D*)HistogramList->FindObject("PID cuts")->FindObject("TPC and TOF Signals")->FindObject("dEdx V P All");
Double_t rebinner = 12;
Double_t bins = 2400/rebinner;
myMinvEtaCadd->Rebin(rebinner);
myMinvEtaCcontent->Rebin(rebinner);
myMinvEtaCerror->Rebin(rebinner);
Printf("Histos Added");


myTPCsignal->SetStats(0);
myTPCsignal->Draw("COLZ");
my_canvas.Draw();
my_canvas.SaveAs("Figures/TPCsignal.png");
my_canvas->Clear();


TF1 *myfit = new TF1("myfit","expo",2,4);
TF1 *mypolyfit = new TF1("mypolyfit","[0]*x*x + [1]*x + [2]",2.2,3.4);
myfit->SetParameters(11.4505,-2.81053); //fit parameters for 30 MeV bins
//myfit->SetParameters(11.155,-2.859); //fit parameters for 20 MeV bins
TF1 *myZero = new TF1("myZero","0",2.2,3.4);

myMinvEtaC->SetXTitle("m_{inv} (GeV/c^{2})");
myMinvEtaC->SetYTitle("Events / 30 MeV");
myMinvEtaC->SetTitle("Final #eta_{c} Candidates, Transverse Momentum < 0.11 GeV/c");
myMinvEtaC->SetStats(0);
myMinvEtaC->Rebin(rebinner);

myMinvEtaC->Fit("mypolyfit", "R");
myMinvEtaC->SetTitle("#eta_{c} Candidates, With Background Fit");
myMinvEtaC->GetXaxis()->SetRangeUser(2.3,3.3);
myMinvEtaC->Draw("E");
myfit->SetLineColor(1);
myfit->Draw("SAME");
my_canvas.Draw();
my_canvas.SaveAs("Figures/minv_fitzoom_30.png");
//my_canvas->Clear();

Double_t chi2 = mypolyfit->GetChisquare();
Printf("Chi^2:");
Printf("%lf",chi2);
Double_t NDF = mypolyfit->GetNDF();
Printf("Degrees of Freedom:");
Printf("%lf",NDF);


myMinvEtaC->Clone("myMinvdiff");
myMinvdiff->Sumw2(kTRUE);
myMinvdiff->Add(mypolyfit,-1);

for (Int_t i = 1; i < bins; i++) {
  Double_t minv = i*0.03-0.015;
  Double_t exponential = mypolyfit(minv);
  Double_t experror = exponential*sqrt(0.116*0.116+0.049*0.049*minv*minv);
  Double_t error = myMinvdiff->GetBinError(i);
  Double_t newerror = sqrt(error*error+experror*experror);

  myMinvdiff->SetBinError(i, newerror);
}

myMinvdiff->GetXaxis()->SetRangeUser(2.3,3.3);
myMinvdiff->SetTitle("Excess #eta_{c} Candidates Over Background");
myMinvdiff->Draw("E");
myZero->Draw("SAME");
my_canvas.Draw();
my_canvas.SaveAs("Figures/minv_fitdiff_30.png");
//my_canvas->Clear();

for (Int_t i = 1; i < bins; i++) {
  Double_t content = myMinvdiff->GetBinContent(i);
  Double_t error = myMinvdiff->GetBinError(i);
  myMinvEtaCcontent->SetBinContent(i,content);
  myMinvEtaCerror->SetBinContent(i,error);
  
}
Double_t bin149c = myMinvdiff->GetBinContent(99);
Double_t bin149e = myMinvdiff->GetBinError(99);
Double_t bin150c = myMinvdiff->GetBinContent(100);
Double_t bin150e = myMinvdiff->GetBinError(100);
Double_t bin151c = myMinvdiff->GetBinContent(101);
Double_t bin151e = myMinvdiff->GetBinError(101);
Double_t bin156c = myMinvdiff->GetBinContent(104);
Double_t bin156e = myMinvdiff->GetBinError(104);

Printf("%lf",bin149c);
Printf("%lf",bin149e);
Printf("%lf",bin150c);
Printf("%lf",bin150e);
Printf("%lf",bin151c);
Printf("%lf",bin151e);
Printf("%lf",bin156c);
Printf("%lf",bin156e);

//Pt Vs Minv Figures - Example with 2D histos.
if(AllTheHistos) {
myPtVsMinvEtaC->SetXTitle("m_{inv} (GeV/c^{2})");
myPtVsMinvEtaC->GetXaxis()->SetRangeUser(1,4);
myPtVsMinvEtaC->SetYTitle("p_{T} (GeV/c)");
myPtVsMinvEtaC->SetTitle("p_{T} (GeV/c)");
myPtVsMinvEtaC->SetTitle("#eta_{c} Channel Matches, Transverse Momentum < 0.11 GeV/c");
myPtVsMinvEtaC->SetStats(0);
myPtVsMinvEtaC->Draw("colz");
my_canvas.SetLogy(1);
my_canvas.Draw();
my_canvas.SaveAs("Figures/ptvminv_30.png");
my_canvas->Clear();

myPtVsMinvEtaCTight->SetXTitle("m_{inv} (GeV/c^{2})");
myPtVsMinvEtaCTight->GetXaxis()->SetRangeUser(1,4);
myPtVsMinvEtaCTight->SetYTitle("p_{T} (GeV/c)");
myPtVsMinvEtaCTight->SetTitle("Final #eta_{c} Channel Matches, Transverse Momentum < 0.11 GeV/c");
myPtVsMinvEtaCTight->SetStats(0);
myPtVsMinvEtaCTight->Draw("colz");
my_canvas.SetLogy(1);
my_canvas.Draw();
my_canvas.SaveAs("Figures/ptvminv_30_tight.png");
my_canvas->Clear();


myMinvEtaCNotTight->SetXTitle("m_{inv} (GeV/c^{2})");
myMinvEtaCNotTight->SetYTitle("Events / 30 MeV");
myMinvEtaCNotTight->SetTitle("#eta_{c} Candidates, Transverse Momentum < 0.11 GeV/c");
myMinvEtaCNotTight->Rebin(12);
myMinvEtaCNotTight->GetXaxis()->SetRangeUser(1,4);
//myMinvEtaCTight->Fit("fit", "R");
myMinvEtaCNotTight->SetStats(0);
myMinvEtaCNotTight->Draw("E");
my_canvas.SetLogy(0);
my_canvas.Draw();
my_canvas.SaveAs("Figures/minv_30.png");
}

}