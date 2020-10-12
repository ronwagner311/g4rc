/*!
  g4rc - Simulation of radiative effects for JLab tritium experiments
*/

#include "CLHEP/Random/Random.h"

#include "g4rcRunAction.hh"
#include "g4rcPrimaryGeneratorAction.hh"
#include "g4rcEventAction.hh"
#include "g4rcSteppingAction.hh"
#include "g4rcDetectorConstruction.hh"

#include "g4rcIO.hh"
#include "g4rcMessenger.hh"

//  Standard physics list
#include "G4PhysListFactory.hh"
#include "G4RunManager.hh"

#include "PhysListPureEm.hh"
#include "PhysListEmModified.hh"

#include "G4UnitsTable.hh"

#include "G4StepLimiterPhysics.hh"

#include "G4RunManagerKernel.hh"

//to make gui.mac work
#include <G4UImanager.hh>
#include <G4UIExecutive.hh>
#include <G4UIterminal.hh>

#ifdef G4UI_USE_QT
#include "G4UIQt.hh"
#endif

#ifdef G4UI_USE_XM
#include "G4UIXm.hh"
#endif

#ifdef G4UI_USE_TCSH
#include "G4UItcsh.hh"
#endif

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include <sys/types.h>
#include <sys/stat.h>

// How do I change the display size of g4rc to be smaller for better viewing on small screens?
// How do I get changes of g4rc detector construction and primary event generators to affect changes if the main function has already called the default values hardcoded inerDetectorConstruction* detector = new g4rcDetectorConstruction();

int main(int argc, char** argv){

    // Initialize the CLHEP random engine used by
    // "shoot" type functions
    unsigned int seed = time(0) + (int) getpid();

    unsigned int devrandseed = 0;
    //  /dev/urandom doens't block
    FILE *fdrand = fopen("/dev/urandom", "r");
    if( fdrand ){
	fread(&devrandseed, sizeof(int), 1, fdrand);
	seed += devrandseed;
	fclose(fdrand);
    }


	std::cout << "\n THE SEED FOR THIS RUN IS " << seed << std::endl;

    CLHEP::HepRandom::createInstance();
    CLHEP::HepRandom::setTheSeed(seed);

    g4rcIO *io = new g4rcIO();

    //-------------------------------
    // Initialization of Run manager
    //-------------------------------
    G4cout << "RunManager construction starting...." << G4endl;
    G4RunManager * runManager = new G4RunManager;

    g4rcMessenger *rmmess = new g4rcMessenger();
    rmmess->SetIO(io);

    // Detector geometry
    G4VUserDetectorConstruction* detector = new g4rcDetectorConstruction();
    runManager->SetUserInitialization(detector);
    rmmess->SetDetCon( ((g4rcDetectorConstruction *) detector) );

/*
    // Physics we want to use
    G4int verbose = 0;
    G4PhysListFactory factory;
    G4VModularPhysicsList* physlist = factory.GetReferencePhysList("FTFP_BERT");
    physlist->SetVerboseLevel(verbose);
    runManager->SetUserInitialization(physlist);
  */

	G4VModularPhysicsList *physicsList = NULL;

        physicsList = new PhysListPureEm("EM", true, 1);
   
	physicsList->RegisterPhysics(new G4StepLimiterPhysics());

        physicsList->RemovePhysics("G4EmStandard");
        physicsList->RegisterPhysics(new PhysListEmModified());

	runManager->SetUserInitialization(physicsList);

    //-------------------------------
    // UserAction classes
    //-------------------------------
    G4UserRunAction* run_action = new g4rcRunAction;
    ((g4rcRunAction *) run_action)->SetIO(io);
    runManager->SetUserAction(run_action);

    G4VUserPrimaryGeneratorAction* gen_action = new g4rcPrimaryGeneratorAction;
    ((g4rcPrimaryGeneratorAction *) gen_action)->SetIO(io);
    rmmess->SetPriGen((g4rcPrimaryGeneratorAction *)gen_action);
    runManager->SetUserAction(gen_action);

    G4UserEventAction* event_action = new g4rcEventAction;
    rmmess->SetEvAct((g4rcEventAction* ) event_action);
    ((g4rcEventAction *) event_action)->SetIO(io);

    runManager->SetUserAction(event_action);
    G4UserSteppingAction* stepping_action = new g4rcSteppingAction;
    runManager->SetUserAction(stepping_action);
    rmmess->SetStepAct((g4rcSteppingAction *) stepping_action);
    ((g4rcEventAction *) event_action)->SetSteppingAction((g4rcSteppingAction *) stepping_action);

    // Initialize Run manager
		////////////////////////////////////////////////////////////////////////////
		
		//runManager->Initialize(); 

		// do initialization in all macro files, 
		//see remoll examples for assistance. 

    G4UIsession* session = 0;

    //----------------
    // Visualization:
    //----------------

    if (argc==1)   // Define UI session for interactive mode.
    {

	// G4UIterminal is a (dumb) terminal.
	
#if defined(G4UI_USE_QT)
	session = new G4UIQt(argc,argv);
#elif defined(G4UI_USE_WIN32)
	session = new G4UIWin32();
#elif defined(G4UI_USE_XM)
	session = new G4UIXm(argc,argv);
#elif defined(G4UI_USE_TCSH)
	session = new G4UIterminal(new G4UItcsh);
#else
	session = new G4UIterminal();
#endif

    }


#ifdef G4VIS_USE
    // Visualization, if you choose to have it!
    //
    // Simple graded message scheme - give first letter or a digit:
    //  0) quiet,         // Nothing is printed.
    //  1) startup,       // Startup and endup messages are printed...
    //  2) errors,        // ...and errors...
    //  3) warnings,      // ...and warnings...
    //  4) confirmations, // ...and confirming messages...
    //  5) parameters,    // ...and parameters of scenes and views...
    //  6) all            // ...and everything available.

    //this is the initializing the run manager?? Right?
    G4VisManager* visManager = new G4VisExecutive;
    //visManager -> SetVerboseLevel (1);
    visManager ->Initialize();
#endif

    //get the pointer to the User Interface manager
    G4UImanager * UI = G4UImanager::GetUIpointer();

    if (session)   // Define UI session for interactive mode.
    {
	// G4UIterminal is a (dumb) terminal.
	//UI->ApplyCommand("/control/execute myVis.mac");

#if defined(G4UI_USE_XM) || defined(G4UI_USE_WIN32) || defined(G4UI_USE_QT)
	// Customize the G4UIXm,Win32 menubar with a macro file :
	UI->ApplyCommand("/control/execute macros/gui.mac");
#endif

	session->SessionStart();
	delete session;
    }
    else           // Batch mode - not using the GUI
    {
#ifdef G4VIS_USE
	visManager->SetVerboseLevel("quiet");
#endif
	//these line will execute a macro without the GUI
	//in GEANT4 a macro is executed when it is passed to the command, /control/execute
	G4String command = "/control/execute ";
	G4String fileName = argv[1];

	/* Copy contents of macro into buffer to be written out
	 * into ROOT file
	 * */

	UI->ApplyCommand(command+fileName);
    }

    //if one used the GUI then delete it
#ifdef G4VIS_USE
    delete visManager;
#endif


    return 0;
}
