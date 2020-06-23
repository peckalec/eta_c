#include <sstream>
void RunEtaC(bool submit = true)
{
	//Keep track of CPU time, clock time
	TStopwatch timer;
	timer.Start();

	gSystem->Load("libTree.so");
	gSystem->Load("libGeom.so");
	gSystem->Load("libVMC.so");
	gSystem->Load("libSTEERBase.so");
	gSystem->Load("libESD.so");
	gSystem->Load("libAOD.so");
	gSystem->Load("libANALYSIS");
	gSystem->Load("libANALYSISalice");
	gSystem->AddIncludePath("-I$ALICE_ROOT/include");

	//Load and compile user written code the way the plugin will load and compile it
	gROOT->ProcessLine(".L AliAnalysisTaskUpcEtaCAWP.cxx+g");

	char *kTreeName = "aodTree"; //CHANGED "esdTree" ;
	TChain * chain = new TChain(kTreeName, "myAODTree"); //CHANGED "myESDTree");

	AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
	if (!mgr) mgr = new AliAnalysisManager("local test train");

	// Conditional to check if running on grid, loads needed libraries and AlienHandler
	if (submit)
	{
		gSystem->Load("libNetx.so");
		gSystem->Load("libgapiUI.so");
		gSystem->Load("libRAliEn.so");
		gROOT->LoadMacro("CreateAlienHandler.C");

		AliAnalysisGrid *alienHandler = CreateAlienHandler();
		if (!alienHandler)
		{
			printf("Error: Handler didn't work.");
			return;
		}
		mgr->SetGridHandler(alienHandler);
	}
	else chain->Add("../local_input_files/*AliAOD*");

	InputHandlerSetup();
	((AliInputEventHandler*)mgr->GetInputEventHandler())->SetNeedField(1);

	gROOT->LoadMacro("$ALICE_PHYSICS/TENDER/TenderSupplies/AddTaskTender.C");
	gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");

	AliAnalysisTaskPIDResponse *taskPID = AddTaskPIDResponse(kFALSE, kTRUE, kFALSE, 2, kFALSE, "", kTRUE, kTRUE, 2);
	AliAnalysisTaskSE *tender = AddTaskTender(kFALSE, kFALSE, kFALSE, kFALSE, kFALSE, kFALSE, kFALSE, kFALSE, kFALSE);

	AliAnalysisTaskUpcEtaCAWP *task1 = new AliAnalysisTaskUpcEtaCAWP("TaskUpcEtaCAOD");
	mgr->AddTask(task1);

	gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");



	AliAnalysisDataContainer *cinput1 = mgr->GetCommonInputContainer();
	AliAnalysisDataContainer *coutput1 = mgr->CreateContainer("Trigger List", TList::Class(), AliAnalysisManager::kOutputContainer, "EtaC.root");
	AliAnalysisDataContainer *coutput2 = mgr->CreateContainer("Histogram List", TList::Class(), AliAnalysisManager::kOutputContainer, "EtaC.root");
	AliAnalysisDataContainer *coutput3 = mgr->CreateContainer("Kstar List", TList::Class(), AliAnalysisManager::kOutputContainer, "EtaC.root");
	AliAnalysisDataContainer *coutput4 = mgr->CreateContainer("2Rho4Pion List", TList::Class(), AliAnalysisManager::kOutputContainer, "EtaC.root");
	AliAnalysisDataContainer *coutput5 = mgr->CreateContainer("K0s3PiPi4K2K4Pi List", TList::Class(), AliAnalysisManager::kOutputContainer, "EtaC.root");
	mgr->ConnectInput(task1, 0, cinput1);
	mgr->ConnectOutput(task1, 1, coutput1);
	mgr->ConnectOutput(task1, 2, coutput2);
	mgr->ConnectOutput(task1, 3, coutput3);
	mgr->ConnectOutput(task1, 4, coutput4);
	mgr->ConnectOutput(task1, 5, coutput5);


	mgr->SetDebugLevel(0);

	if (!mgr->InitAnalysis()) return;
	mgr->PrintStatus();
	if (submit) {
		mgr->StartAnalysis("grid");

	}
	else {
		mgr->StartAnalysis("local", chain);
	}
	timer.Stop();
	timer.Print();
}

//CHANGED from format = "esd"
Bool_t InputHandlerSetup(TString format = "aod", Bool_t useKine = kFALSE)
{

	format.ToLower();

	AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

	AliAnalysisDataContainer *cin = mgr->GetCommonInputContainer();

	if (cin) return;

	if (!format.CompareTo("esd"))
	{
		AliESDInputHandler *esdInputHandler = dynamic_cast<AliESDInputHandler*>(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());

		if (!esdInputHandler)
		{
			Info("CustomAnalysisTaskInputSetup", "Creating esdInputHandler ...");
			esdInputHandler = new AliESDInputHandler();
			mgr->SetInputEventHandler(esdInputHandler);
		}

		if (useKine)
		{
			AliMCEventHandler* mcInputHandler = dynamic_cast<AliMCEventHandler*>(AliAnalysisManager::GetAnalysisManager()->GetMCtruthEventHandler());

			if (!mcInputHandler)
			{
				Info("CustomAnalysisTaskInputSetup", "Creating mcInputHandler ...");
				AliMCEventHandler* mcInputHandler = new AliMCEventHandler();
				mgr->SetMCtruthEventHandler(mcInputHandler);
			}
		}

	}
	else if (!format.CompareTo("aod"))
	{
		AliAODInputHandler *aodInputHandler = dynamic_cast<AliAODInputHandler*>(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());

		if (!aodInputHandler)
		{
			Info("CustomAnalysisTaskInputSetup", "Creating aodInputHandler ...");
			aodInputHandler = new AliAODInputHandler();
			mgr->SetInputEventHandler(aodInputHandler);
		}

		TString     kDeltaAODJetName = "AliAOD.Jets.root";
		if (kDeltaAODJetName.Length()) {
		}
	}

}