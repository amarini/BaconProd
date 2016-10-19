import FWCore.ParameterSet.Config as cms
from RecoEgamma.EgammaIsolationAlgos.pfBlockBasedIsolation_cfi import *

electronFootPrint = cms.EDProducer('ElectronFootPrintProducer',
	label=cms.string("electronFootPrint"),
	electrons = cms.InputTag("gedGsfElectrons"), 
	pfCandidates = cms.InputTag("particleFlow"),
	pfBlockBasedIsolationSetUp=cms.PSet(pfBlockBasedIsolation) 
)
