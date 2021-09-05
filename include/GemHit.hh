//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GemHit_h
#define GemHit_h 1

#include "G4VHit.hh"

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4VPhysicalVolume.hh"

#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GemHit : public G4VHit
{
public:
    GemHit();
    virtual ~GemHit();

    inline void *operator new (size_t);
    inline void operator delete (void *);

    bool operator ==(const GemHit &) const;

    void Print();
    void Clear();

    inline G4int GetPID() const;
    inline G4int GetTrackID() const;
    inline G4int GetParentTrackID() const;
    inline G4int GetDetectorID() const;
    inline G4ThreeVector GetInPos() const;
    inline G4ThreeVector GetOutPos() const;
    inline G4ThreeVector GetInMom() const;
    inline G4ThreeVector GetOutMom() const;
    inline G4double GetTime() const;
    inline G4double GetEdep() const;
    inline G4double GetTrackLength() const;
    inline const G4VPhysicalVolume *GetPhysV() const;
    inline G4int GetCopyNo() const;
	inline G4int GetDetectorNum() const;

    inline void SetPID(G4int &val);
    inline void SetTrackID(G4int &val);
    inline void SetParentTrackID(G4int &val);
    inline void SetDetectorID(G4int &val);
    inline void SetInPos(G4ThreeVector &xyz);
    inline void SetOutPos(G4ThreeVector &xyz);
    inline void SetInMom(G4ThreeVector &pxpypz);
    inline void SetOutMom(G4ThreeVector &pxpypz);
    inline void SetTime(G4double &val);
    inline void SetEdep(G4double &val);
    inline void SetTrackLength(G4double &val);
    inline void SetPhysV(G4VPhysicalVolume *val);
    inline void SetCopyNo(G4int &val);
	inline void SetDetectorNum(G4int &val);

    inline void AddEdep(G4double &val);
    inline void AddTrackLength(G4double &val);

private:
    G4int         fPID;
    G4int         fTrackID;
    G4int         fPTrackID;
    G4int         fDetID;
    G4ThreeVector fInPos;
    G4ThreeVector fOutPos;
    G4ThreeVector fInMom;
    G4ThreeVector fOutMom;
    G4double      fTime;
    G4double      fEdep;
    G4double      fTrackLen;
    const G4VPhysicalVolume *fPhysV;
    G4int         fCopyNo;
	G4int		  fDetNum;
};

typedef G4THitsCollection<GemHit> GemHitsCollection;

extern G4Allocator<GemHit> GemHitAllocator;

inline void *GemHit::operator new (size_t)
{
    void *aHit;
    aHit = (void *)GemHitAllocator.MallocSingle();
    return aHit;
}

inline void GemHit::operator delete (void *aHit)
{
    GemHitAllocator.FreeSingle((GemHit *)aHit);
}

inline G4int GemHit::GetPID() const
{
    return fPID;
}

inline G4int GemHit::GetTrackID() const
{
    return fTrackID;
}

inline G4int GemHit::GetParentTrackID() const
{
    return fPTrackID;
}

inline G4int GemHit::GetDetectorID() const
{
    return fDetID;
}

inline G4ThreeVector GemHit::GetInPos() const
{
    return fInPos;
}

inline G4ThreeVector GemHit::GetOutPos() const
{
    return fOutPos;
}

inline G4ThreeVector GemHit::GetInMom() const
{
    return fInMom;
}

inline G4ThreeVector GemHit::GetOutMom() const
{
    return fOutMom;
}

inline G4double GemHit::GetTime() const
{
    return fTime;
}

inline G4double GemHit::GetEdep() const
{
    return fEdep;
}

inline G4double GemHit::GetTrackLength() const
{
    return fTrackLen;
}

inline const G4VPhysicalVolume *GemHit::GetPhysV() const
{
    return fPhysV;
}

inline G4int GemHit::GetCopyNo() const
{
    return fCopyNo;
}

inline G4int GemHit::GetDetectorNum() const
{
    return fDetNum;
}
inline void GemHit::SetPID(G4int &val)
{
    fPID = val;
}

inline void GemHit::SetTrackID(G4int &val)
{
    fTrackID = val;
}

inline void GemHit::SetParentTrackID(G4int &val)
{
    fPTrackID = val;
}

inline void GemHit::SetDetectorID(G4int &val)
{
    fDetID = val;
}

inline void GemHit::SetInPos(G4ThreeVector &xyz)
{
    fInPos = xyz;
}

inline void GemHit::SetOutPos(G4ThreeVector &xyz)
{
    fOutPos = xyz;
}

inline void GemHit::SetInMom(G4ThreeVector &pxpypz)
{
    fInMom = pxpypz;
}

inline void GemHit::SetOutMom(G4ThreeVector &pxpypz)
{
    fOutMom = pxpypz;
}

inline void GemHit::SetTime(G4double &val)
{
    fTime = val;
}

inline void GemHit::SetEdep(G4double &val)
{
    fEdep = val;
}

inline void GemHit::SetTrackLength(G4double &val)
{
    fTrackLen = val;
}

inline void GemHit::SetPhysV(G4VPhysicalVolume *val)
{
    fPhysV = val;
}

inline void GemHit::SetCopyNo(G4int &val)
{
    fCopyNo = val;
}

inline void GemHit::SetDetectorNum(G4int &val)
{
	fDetNum = val;
}

inline void GemHit::AddEdep(G4double &val)
{
    fEdep += val;
}

inline void GemHit::AddTrackLength(G4double &val)
{
    fTrackLen += val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
