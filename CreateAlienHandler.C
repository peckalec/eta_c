
AliAnalysisGrid* CreateAlienHandler(const char* mode = "full",
                                    const char* gridDataDirectory = "/alice/data/2018/LHC18r" // fill with location of data on aliensh
                                    )
{
   
   cout << "Creating Alien Handler..." << endl;
   AliAnalysisAlien *plugin = new AliAnalysisAlien();
   plugin->SetOverwriteMode();
   plugin->SetRunMode("terminate"); //"terminate"); or "full");
   plugin->SetNtestFiles(1);
   plugin->SetAPIVersion("V1.1x");
   //plugin->SetROOTVersion("v5-34-30-alice-7"); //("v5-34-30-1"); //("v5-34-08-6");
   //plugin->SetAliROOTVersion("v5-07-18-1"); //("v5-06-28"); //("vAN-20140920");
   plugin->SetAliPhysicsVersion("v5-09-09-01-1"); //"v5-07-18-01-1"); //("vAN-20150630"); //("vAN-20140920");
   plugin->SetGridDataDir(gridDataDirectory);
   plugin->SetDataPattern("*/pass1/*AliAOD.root"); //("*/pass1_pidfix/*AOD.root");
   plugin->SetRunPrefix("000");

   plugin->AddRunNumber("296690 296691 296693");

   
   plugin->SetGridWorkingDir("18r.296690.296691.296693_co4_0");
   plugin->SetGridOutputDir("18r.296690.296691.296693_co4_0");
   plugin->SetAnalysisSource("AliAnalysisTaskUpcEtaCAWP.cxx");
   plugin->SetAdditionalLibs("AliAnalysisTaskUpcEtaCAWP.h AliAnalysisTaskUpcEtaCAWP.cxx libTree.so libVMC.so libGeom.so libGui.so libXMLParser.so libMinuit.so libMinuit2.so libProof.so libPhysics.so libSTEERBase.so libESD.so libAOD.so libOADB.so libANALYSIS.so libCDB.so libRAWDatabase.so libSTEER.so libANALYSISalice.so libCORRFW.so libTOFbase.so libRAWDatabase.so libRAWDatarec.so libTPCbase.so libTPCrec.so libITSbase.so libITSrec.so libTRDbase.so libSTAT.so libTRDsim.so libTRDrec.so libTRDcalib.so libHMPIDbase.so libPWGHFbase.so libPWGDQdielectron.so libPWGHFhfe.so libEMCALUtils.so libPHOSUtils.so libPWGCaloTrackCorrBase.so libEMCALraw.so libEMCALbase.so libEMCALrec.so libVZERObase.so libVZEROrec.so libHLTbase.so libHLTinterface.so libHLTsim.so libHLTrec.so libAliHLTTrigger.so libTENDER.so libTENDERSupplies.so");
   plugin->SetDefaultOutputs();
   plugin->SetAnalysisMacro("EtaC.C");
   plugin->SetSplitMaxInputFileNumber(20);
   plugin->SetExecutable("EtaC_grid.sh");
   plugin->SetTTL(30000);
   plugin->SetInputFormat("xml-single");
   plugin->SetJDLName("EtaC_JDL.jdl");
   plugin->SetPrice(1);      
   plugin->SetSplitMode("se");
   return plugin;
}
