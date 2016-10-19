// -*- C++ -*-
//
// Class:      ElectronFootPrintProducer
// 
/*
 Description: This class implements the search for footprint in gedElectrons

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Andrea Carlo Marini
//         Created:  Wed, 19 Oct 2016 13:38:32 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

// EGamma
//
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "RecoEgamma/EgammaIsolationAlgos/plugins/ParticleBasedIsoProducer.h"
// ROOT
#include "TLorentzVector.h"

//
// class declaration
//

class ElectronFootPrintProducer : public edm::stream::EDProducer<> {
   public:
      explicit ElectronFootPrintProducer(const edm::ParameterSet&);
      ~ElectronFootPrintProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      void beginStream(edm::StreamID) override;
      void produce(edm::Event&, const edm::EventSetup&) override;
      void endStream() override;

      void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
      std::string label_{"valueMap"};
      std::unique_ptr<PFBlockBasedIsolation> thePFBlockBasedIsolation_;

      // token
      edm::EDGetTokenT<reco::GsfElectronCollection> electronsT_;
      edm::EDGetTokenT<reco::PFCandidateCollection> pfCandidatesT_;
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
ElectronFootPrintProducer::ElectronFootPrintProducer(const edm::ParameterSet& iConfig)
{
   // name of the value map
   label_ = iConfig.getParameter<std::string>("label");

   //consumes 
   pfCandidatesT_ = consumes<reco::PFCandidateCollection>(iConfig.getParameter<edm::InputTag>("pfCandidates"));
   electronsT_    = consumes<reco::GsfElectronCollection>(iConfig.getParameter<edm::InputTag>("electrons"));
   // produces  
   produces< edm::ValueMap<TLorentzVector> > (label_); 
  
}


ElectronFootPrintProducer::~ElectronFootPrintProducer()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
ElectronFootPrintProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

     // get ged electrons
     edm::Handle<reco::GsfElectronCollection> electronsHandle;
     iEvent.getByToken(electronsT_,electronsHandle);

     // Get the  PF candidates collection
     edm::Handle<reco::PFCandidateCollection> pfCandidateHandle;
     iEvent.getByToken(pfCandidatesT_,pfCandidateHandle);

     // store the ValueMap
     std::vector<TLorentzVector> product;

     // Loop over gedElectrons
     for(const auto& ele : *electronsHandle) {
	     reco::SuperClusterRef sc  =  ele.superCluster ();
	     TLorentzVector fp; // footprint
	     // -- ECAL SC based matching
	     /*
	     for(const auto & cand: pfCandidateHandle)
	     {
		bool inFootPrint=false;
		if (sc == cand.superClusterRef() ) { inFootPrint = true;}
		//if (  ele.bestTrackRef() == cand.trackRef() ) { inFootPrint = true;} // ??
		if (inFootPrint)
		{
			TLorentzVector candV ( cand.px(),cand.py(),cand.pz(),cand.energy() );	
			fp+=candV;
		}
	     }
	     */
	     // -- PF Based --
	     for (int iC=0; iC< ele.numberOfSourceCandidatePtrs() ;++iC)
	     {
	    		const auto& sc = ele.sourceCandidatePtr(iC);  
			TLorentzVector candV ( cand.px(),cand.py(),cand.pz(),cand.energy() );	
			fp+=candV;
	     }
	
	     product.push_back(fp);
     } // end loop over electrons

     // put in Event
     std::unique_ptr<edm::ValueMap<TLorentzVector>> out(new edm::ValueMap<TLorentzVector>());
     edm::ValueMap<TLorentzVector>::Filler filler(*out);
     filler.insert(electronsHandle, product.begin(), product.end());
     filler.fill();
     iEvent.put(out, label_.c_str());
}

void 
ElectronFootPrintProducer::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){
	 thePFBlockBasedIsolation_ . reset( new PFBlockBasedIsolation() );
	 edm::ParameterSet pfBlockBasedIsolationSetUp = conf_.getParameter<edm::ParameterSet>("pfBlockBasedIsolationSetUp"); 
	 thePFBlockBasedIsolation_ ->setup(pfBlockBasedIsolationSetUp);
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
ElectronFootPrintProducer::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
ElectronFootPrintProducer::endStream() {
}

 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ElectronFootPrintProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ElectronFootPrintProducer);
