#ifndef g4rcIO_HH
#define g4rcIO_HH

#include "TROOT.h"
#include "TObject.h"
#include "G4Run.hh"
#include "g4rctypes.hh"

#include "G4String.hh"

#include "g4rcEvent.hh"

class TFile;
class TTree;

class g4rcDetectorHit;
class GemHit;

#define __IO_MAXHIT 10000
#define __MAXHIT_GEM 300
#define __FILENAMELEN 255

// Units for output
#define __E_UNIT GeV
#define __L_UNIT m
#define __T_UNIT ns
#define __ANG_UNIT rad
#define __ASYMM_SCALE 1e-9 // ppb

class g4rcIO {
    public:
	 g4rcIO();
	~g4rcIO();

	void SetFilename(G4String  fn);
	G4String GetFilename(){return fFilename;}

	void FillTree();
	void Flush();
	void WriteTree();

	void WriteRun();

	void InitializeTree();
	Double_t fHRSangle;	

    private:
	TFile *fFile;
	TTree *fTree;

	char fFilename[__FILENAMELEN];

	//  Interfaces and buffers to the tree
	//  This is potentially going to get very long...

	// Event data
    public:
	void SetEventData(g4rcEvent *);
    private:
	Double_t fEvPart_X;
	Double_t fEvPart_Y;
	Double_t fEvPart_P;
	Int_t fEvPart_PID;

	//  DetectorHit
    public:
	void AddDetectorHit(g4rcDetectorHit *);
	void AddGem1Hit(GemHit *);
	void AddGem2Hit(GemHit *);
    private:
	Int_t fNDetHit;
	Int_t fDetHit_det[__IO_MAXHIT];

	Int_t fDetHit_trid[__IO_MAXHIT];
	Int_t fDetHit_pid[__IO_MAXHIT];
	Int_t fDetHit_mtrid[__IO_MAXHIT];

	Double_t fDetHit_X[__IO_MAXHIT];
	Double_t fDetHit_Y[__IO_MAXHIT];
	Double_t fDetHit_Z[__IO_MAXHIT];

	Double_t fDetHit_Px[__IO_MAXHIT];
	Double_t fDetHit_Py[__IO_MAXHIT];
	Double_t fDetHit_Pz[__IO_MAXHIT];
	Double_t fDetHit_P[__IO_MAXHIT];
	Double_t fDetHit_E[__IO_MAXHIT];
	Double_t fDetHit_M[__IO_MAXHIT];

	Double_t fDetHit_Edep[__IO_MAXHIT];
	
	//gem detectors
	
	//gem_1
	int fGEM1N;
    int fGEM1PID[__MAXHIT_GEM]; // Particle ID
    int fGEM1TID[__MAXHIT_GEM]; // Track ID
    int fGEM1PTID[__MAXHIT_GEM]; // Parent Track ID
    int fGEM1DID[__MAXHIT_GEM];
    double fGEM1X[__MAXHIT_GEM];
    double fGEM1Y[__MAXHIT_GEM];
    double fGEM1Z[__MAXHIT_GEM];
    double fGEM1Momentum[__MAXHIT_GEM];
    double fGEM1Theta[__MAXHIT_GEM];
    double fGEM1Phi[__MAXHIT_GEM];
    double fGEM1Time[__MAXHIT_GEM];
    double fGEM1Edep[__MAXHIT_GEM];
    double fGEM1TrackL[__MAXHIT_GEM];
	
	//gem_2
	int fGEM2N;
    int fGEM2PID[__MAXHIT_GEM]; // Particle ID
    int fGEM2TID[__MAXHIT_GEM]; // Track ID
    int fGEM2PTID[__MAXHIT_GEM]; // Parent Track ID
    int fGEM2DID[__MAXHIT_GEM];
    double fGEM2X[__MAXHIT_GEM];
    double fGEM2Y[__MAXHIT_GEM];
    double fGEM2Z[__MAXHIT_GEM];
    double fGEM2Momentum[__MAXHIT_GEM];
    double fGEM2Theta[__MAXHIT_GEM];
    double fGEM2Phi[__MAXHIT_GEM];
    double fGEM2Time[__MAXHIT_GEM];
    double fGEM2Edep[__MAXHIT_GEM];
    double fGEM2TrackL[__MAXHIT_GEM];
};

#endif//g4rcIO_HH
