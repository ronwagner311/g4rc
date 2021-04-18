#include "g4rcIO.hh"

#include "G4SystemOfUnits.hh"
#include "G4AffineTransform.hh"

#include <TFile.h>
#include <TTree.h>
#include <TClonesArray.h>

#include "G4ParticleDefinition.hh"

#include "g4rcDetectorHit.hh"
#include "GemHit.hh"
#include "g4rcEvent.hh"

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <assert.h>

#include "TString.h"


g4rcIO::g4rcIO(){
    fTree = NULL;
    InitializeTree();
    // Default filename
     	strcpy(fFilename, "g4rcout.root");
	fHRSangle = 17.5*deg;
	fFile = NULL;
}

g4rcIO::~g4rcIO(){
    if( fTree ){ delete fTree; }
    fTree = NULL;
    if( fFile ){ delete fFile; }
    fFile = NULL;
}

void g4rcIO::SetFilename(G4String fn){
    G4cout << "Setting output file to " << fn << G4endl;
    strcpy(fFilename, fn.data());
}

void g4rcIO::InitializeTree(){
    if( fFile ){
	fFile->Close();
	delete fFile;
    }

    fFile = new TFile(fFilename, "RECREATE");

    if( fTree ){ delete fTree; }

    fTree = new TTree("T", "TRITIUM radiative effects simulation");

    // Event information
    fTree->Branch("ev.pid",   &fEvPart_PID, "ev.pid/I");
    fTree->Branch("ev.vx",    &fEvPart_X,   "ev.vx/D");
    fTree->Branch("ev.vy",    &fEvPart_Y,   "ev.vy/D");
    fTree->Branch("ev.p",     &fEvPart_P,   "ev.p/D");

    // DetectorHit
    fTree->Branch("hit.n",    &fNDetHit,     "hit.n/I");
    fTree->Branch("hit.det",  &fDetHit_det,  "hit.det[hit.n]/I");

    fTree->Branch("hit.pid",  &fDetHit_pid,   "hit.pid[hit.n]/I");
    fTree->Branch("hit.trid", &fDetHit_trid,  "hit.trid[hit.n]/I");
    fTree->Branch("hit.mtrid",&fDetHit_mtrid, "hit.mtrid[hit.n]/I");

    fTree->Branch("hit.x",    &fDetHit_X,   "hit.x[hit.n]/D");
    fTree->Branch("hit.y",    &fDetHit_Y,   "hit.y[hit.n]/D");
    fTree->Branch("hit.z",    &fDetHit_Z,   "hit.z[hit.n]/D");

    fTree->Branch("hit.px",   &fDetHit_Px,   "hit.px[hit.n]/D");
    fTree->Branch("hit.py",   &fDetHit_Py,   "hit.py[hit.n]/D");
    fTree->Branch("hit.pz",   &fDetHit_Pz,   "hit.pz[hit.n]/D");

    fTree->Branch("hit.p",    &fDetHit_P,   "hit.p[hit.n]/D");
    fTree->Branch("hit.e",    &fDetHit_E,   "hit.e[hit.n]/D");
    fTree->Branch("hit.m",    &fDetHit_M,   "hit.m[hit.n]/D");

    fTree->Branch("hit.edep", &fDetHit_Edep, "hit.edep[hit.n]/D");
	
	//gem1hits
	G4String fAbbrev = "GEM1";
	const char *abbr = fAbbrev.data();
	
	tree->Branch(Form("%s.N", abbr), &fGEM1N, Form("%s.N/I", abbr));
    tree->Branch(Form("%s.PID", abbr), fGEM1PID, Form("%s.PID[%s.N]/I", abbr, abbr));
    tree->Branch(Form("%s.TID", abbr), fGEM1TID, Form("%s.TID[%s.N]/I", abbr, abbr));
    tree->Branch(Form("%s.PTID", abbr), fGEM1PTID, Form("%s.PTID[%s.N]/I", abbr, abbr));
    tree->Branch(Form("%s.X", abbr), fGEM1X, Form("%s.X[%s.N]/D", abbr, abbr));
    tree->Branch(Form("%s.Y", abbr), fGEM1Y, Form("%s.Y[%s.N]/D", abbr, abbr));
    tree->Branch(Form("%s.Z", abbr), fGEM1Z, Form("%s.Z[%s.N]/D", abbr, abbr));
    tree->Branch(Form("%s.P", abbr), fGEM1Momentum, Form("%s.P[%s.N]/D", abbr, abbr));
    tree->Branch(Form("%s.Theta", abbr), fGEM1Theta, Form("%s.Theta[%s.N]/D", abbr, abbr));
    tree->Branch(Form("%s.Phi", abbr), fGEM1Phi, Form("%s.Phi[%s.N]/D", abbr, abbr));
    tree->Branch(Form("%s.Time", abbr), fGEM1Time, Form("%s.Time[%s.N]/D", abbr, abbr));
    tree->Branch(Form("%s.Edep", abbr), fGEM1Edep, Form("%s.Edep[%s.N]/D", abbr, abbr));
    tree->Branch(Form("%s.TrackL", abbr), fGEM1TrackL, Form("%s.TrackL[%s.N]/D", abbr, abbr));
    tree->Branch(Form("%s.DID", abbr), fGEM1DID, Form("%s.DID[%s.N]/I", abbr, abbr));
	
	//gem2hits
	fAbbrev = "GEM2";
	abbr = fAbbrev.data();
	
	tree->Branch(Form("%s.N", abbr), &fGEM2N, Form("%s.N/I", abbr));
    tree->Branch(Form("%s.PID", abbr), fGEM2PID, Form("%s.PID[%s.N]/I", abbr, abbr));
    tree->Branch(Form("%s.TID", abbr), fGEM2TID, Form("%s.TID[%s.N]/I", abbr, abbr));
    tree->Branch(Form("%s.PTID", abbr), fGEM2PTID, Form("%s.PTID[%s.N]/I", abbr, abbr));
    tree->Branch(Form("%s.X", abbr), fGEM2X, Form("%s.X[%s.N]/D", abbr, abbr));
    tree->Branch(Form("%s.Y", abbr), fGEM2Y, Form("%s.Y[%s.N]/D", abbr, abbr));
    tree->Branch(Form("%s.Z", abbr), fGEM2Z, Form("%s.Z[%s.N]/D", abbr, abbr));
    tree->Branch(Form("%s.P", abbr), fGEM2Momentum, Form("%s.P[%s.N]/D", abbr, abbr));
    tree->Branch(Form("%s.Theta", abbr), fGEM2Theta, Form("%s.Theta[%s.N]/D", abbr, abbr));
    tree->Branch(Form("%s.Phi", abbr), fGEM2Phi, Form("%s.Phi[%s.N]/D", abbr, abbr));
    tree->Branch(Form("%s.Time", abbr), fGEM2Time, Form("%s.Time[%s.N]/D", abbr, abbr));
    tree->Branch(Form("%s.Edep", abbr), fGEM2Edep, Form("%s.Edep[%s.N]/D", abbr, abbr));
    tree->Branch(Form("%s.TrackL", abbr), fGEM2TrackL, Form("%s.TrackL[%s.N]/D", abbr, abbr));
    tree->Branch(Form("%s.DID", abbr), fGEM2DID, Form("%s.DID[%s.N]/I", abbr, abbr));
    return;
}

void g4rcIO::FillTree(){
    if( !fTree ){ 
	fprintf(stderr, "Error %s: %s line %d - Trying to fill non-existant tree\n", __PRETTY_FUNCTION__, __FILE__, __LINE__ );
	return; 
    }

    fTree->Fill();
}

void g4rcIO::Flush(){
    	//  Set arrays to 0
    	fNDetHit = 0;
		fGEM1N = 0;
		fGEM2N = 0;
}

void g4rcIO::WriteTree(){
    assert( fFile );
    assert( fTree );

    if( !fFile->IsOpen() ){
	G4cerr << "ERROR: " << __FILE__ << " line " << __LINE__ << ": TFile not open" << G4endl;
	exit(1);
    }

    G4cout << "Writing output to " << fFile->GetName() << "... ";

    fFile->cd();

    fTree->Write("T", TObject::kOverwrite);

    fTree->ResetBranchAddresses();
    delete fTree;
    fTree = NULL;

    fFile->Close();

    delete fFile;
    fFile = NULL;

    G4cout << "written" << G4endl;

    return;
}

///////////////////////////////////////////////////////////////////////////////
// Interfaces to output section ///////////////////////////////////////////////

// Event Data

void g4rcIO::SetEventData(g4rcEvent *ev){
    fEvPart_PID = ev->fPartType[0]->GetPDGEncoding();

    fEvPart_X = ev->fPartPos[0].x()/__L_UNIT;
    fEvPart_Y = ev->fPartPos[0].y()/__L_UNIT;

    fEvPart_P = ev->fPartMom[0].mag()/__E_UNIT;

    return;
}


// DetectorHit

void g4rcIO::AddDetectorHit(g4rcDetectorHit *hit){
    int n = fNDetHit;
    //printf("%d hits in detector\n", fNDetHit );

    if( n >= __IO_MAXHIT ){
//	G4cerr << "WARNING: " << __PRETTY_FUNCTION__ << " line " << __LINE__ << ":  Buffer size exceeded!" << G4endl;
	return;
    }

    fDetHit_det[n]  = hit->fDetID;

    fDetHit_trid[n] = hit->fTrID;
    fDetHit_mtrid[n]= hit->fmTrID;
    fDetHit_pid[n]  = hit->fPID;

    fDetHit_X[n]  = hit->f3X.x()/__L_UNIT;
    fDetHit_Y[n]  = hit->f3X.y()/__L_UNIT;
    fDetHit_Z[n]  = hit->f3X.z()/__L_UNIT;

    fDetHit_Px[n]  = hit->f3P.x()/__E_UNIT;
    fDetHit_Py[n]  = hit->f3P.y()/__E_UNIT;
    fDetHit_Pz[n]  = hit->f3P.z()/__E_UNIT;

    fDetHit_P[n]  = hit->fP/__E_UNIT;
    fDetHit_E[n]  = hit->fE/__E_UNIT;
    fDetHit_M[n]  = hit->fM/__E_UNIT;

    fDetHit_Edep[n] = hit->fEdep/__E_UNIT;

    fNDetHit++;

    return;
}

//gem1Hit

void g4rcIO::AddGem1Hit(GemHit *hit){
    int n = fGEM1N;
    //printf("%d hits in detector\n", fNDetHit );

    if( n >= __MAXHIT_GEM ){
//	G4cerr << "WARNING: " << __PRETTY_FUNCTION__ << " line " << __LINE__ << ":  Buffer size exceeded!" << G4endl;
	return;
    }

    fGEM1PID[i] = hit->GetPID();
	fGEM1TID[i] = hit->GetTrackID();
    fGEM1PTID[i] = hit->GetParentTrackID();
	fGEM1DID[i] = hit->GetDetectorID();
	fGEM1X[i] = hit->GetInPos().x();
	fGEM1Y[i] = hit->GetInPos().y();
	fGEM1Z[i] = hit->GetInPos().z();
	fGEM1Momentum[i] = hit->GetInMom().mag();
	fGEM1Theta[i] = hit->GetInMom().theta();
	fGEM1Phi[i] = hit->GetInMom().phi();
	fGEM1Time[i] = hit->GetTime();
	fGEM1Edep[i] = hit->GetEdep();
	fGEM1TrackL[i] = hit->GetTrackLength();

    fGEM1N++;

    return;
}

void g4rcIO::AddGem2Hit(GemHit *hit){
    int n = fGEM2N;
    //printf("%d hits in detector\n", fNDetHit );

    if( n >= __MAXHIT_GEM ){
//	G4cerr << "WARNING: " << __PRETTY_FUNCTION__ << " line " << __LINE__ << ":  Buffer size exceeded!" << G4endl;
	return;
    }

    fGEM2PID[i] = hit->GetPID();
	fGEM2TID[i] = hit->GetTrackID();
    fGEM2PTID[i] = hit->GetParentTrackID();
	fGEM2DID[i] = hit->GetDetectorID();
	fGEM2X[i] = hit->GetInPos().x();
	fGEM2Y[i] = hit->GetInPos().y();
	fGEM2Z[i] = hit->GetInPos().z();
	fGEM2Momentum[i] = hit->GetInMom().mag();
	fGEM2Theta[i] = hit->GetInMom().theta();
	fGEM2Phi[i] = hit->GetInMom().phi();
	fGEM2Time[i] = hit->GetTime();
	fGEM2Edep[i] = hit->GetEdep();
	fGEM2TrackL[i] = hit->GetTrackLength();

    fGEM2N++;

    return;
}


