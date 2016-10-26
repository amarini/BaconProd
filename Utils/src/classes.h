#include "DataFormats/Common/interface/ValueMap.h"  
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "TLorentzVector.h"

namespace {
	struct dictionary {
	edm::ValueMap<TLorentzVector> my_ValueMapTLorentzVector;
	edm::Wrapper<edm::ValueMap<TLorentzVector> > my_WrapperValueMapTLorentzVector;
	};
}
